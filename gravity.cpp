#include "raylib.h"
#include <cmath>
#include <iterator>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>

// gravitational constant
constexpr double G = 6.67430e-11;

// timestep
constexpr double dt = 21600.0;      // 6 hours per frame

// window
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

// meters to pixels
constexpr double SCALE = 6e-11;

struct Vec3 {
    double x, y, z;
};

struct Body {
    std::string name;
    double mass;
    double radius;
    Color color;
    Vec3 position;
    Vec3 velocity;
    Vec3 force;
};

std::vector<std::vector<std::string>> loadCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << "\n";
        return data;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}

double parseDouble(const std::string& s, int row, const char* field) {
    try {
        return std::stod(s);
    } catch (const std::exception&) {
        std::cerr << "row " << row << ", field " << field << ": could not parse \"" << s << "\"\n";
        throw;
    }
}

std::vector<Body> buildBodies(const std::vector<std::vector<std::string>>& data) {
    std::vector<Body> bodies;

    for (int i = 0; i < (int)data.size(); i++) {
        const std::vector<std::string>& row = data[i];

        if (row.empty()) {
            continue;
        }

        if (row[0].empty() || row[0][0] == '#') {
            continue;
        }

        if (row[0] == "Name") {
            continue;
        }

        if ((int)row.size() != 12) {
            continue;
        }

        Body body;
        body.name = row[0];
        body.mass = parseDouble(row[1], i, "Mass");
        body.radius = parseDouble(row[2], i, "Radius");
        unsigned char r =  (unsigned char)parseDouble(row[3], i, "R");
        unsigned char g = (unsigned char)parseDouble(row[4], i, "G");
        unsigned char b = (unsigned char)parseDouble(row[5], i, "B");
        body.color = {r, g, b, 255};
        body.position.x = parseDouble(row[6], i, "Pos_X");
        body.position.y = parseDouble(row[7], i, "Pos_Y");
        body.position.z = parseDouble(row[8], i, "Pos_Z");
        body.velocity.x = parseDouble(row[9], i, "Vel_X");
        body.velocity.y = parseDouble(row[10], i, "Vel_Y");
        body.velocity.z = parseDouble(row[11], i, "Vel_Z");
        body.force = {0, 0, 0};

        bodies.push_back(body);
    }

    return bodies;
}
int main() {
    std::vector<Body> bodies = buildBodies(loadCSV("ephemeris.csv"));

    if (bodies.empty()) {
        std::cerr << "No bodies loaded, exiting\n";

        return 1;
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gravity");
    SetTargetFPS(60);
    const int count = static_cast<int>(bodies.size());

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 800.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_CUSTOM;

    float radius = 25.0f;
    float theta = 0.0f;
    float phi = 1.5708f;

    while (!WindowShouldClose()) {

        // --- physics --- //
        // set to zero
        for (int i = 0; i < count; i++) {
            bodies[i].force.x = 0.0;
            bodies[i].force.y = 0.0;
            bodies[i].force.z = 0.0;
        }

        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                // separation
                double dx = bodies[j].position.x - bodies[i].position.x;
                double dy = bodies[j].position.y - bodies[i].position.y;
                double dz = bodies[j].position.z - bodies[i].position.z;
                // distance
                double r = std::sqrt(dx * dx + dy * dy + dz * dz);
                // force of magnitude
                double f = G * bodies[i].mass * bodies[j].mass / (r * r);
                // magnitude times unit direction
                double fx = f * (dx / r);
                double fy = f * (dy / r);
                double fz = f * (dz / r);
                // accumulator
                bodies[i].force.x += fx;
                bodies[i].force.y += fy;
                bodies[i].force.z += fz;
                bodies[j].force.x -= fx;
                bodies[j].force.y -= fy;
                bodies[j].force.z -= fz;
            }
        }

        for (int i = 0; i < count; i++) {
            // acceleration
            double ax = bodies[i].force.x / bodies[i].mass;
            double ay = bodies[i].force.y / bodies[i].mass;
            double az = bodies[i].force.z / bodies[i].mass;
            // integration
            bodies[i].velocity.x += ax * dt;
            bodies[i].velocity.y += ay * dt;
            bodies[i].velocity.z += az * dt;
            bodies[i].position.x += bodies[i].velocity.x * dt;
            bodies[i].position.y += bodies[i].velocity.y * dt;
            bodies[i].position.z += bodies[i].velocity.z * dt;
        }

        // --- Orbital period check ---
        // Earth's distance from the Sun, every 30 frames. The peaks should recur
        // about every 360 frames, which is one Earth year at one day per timestep.
        // Confirms the integration is producing a real orbit rather than a drift.

        // static int frame = 0;
        // frame++;
        // if (frame % 30 == 0) {
        //    double dx = bodies[3].position.x - bodies[0].position.x;
        //    double dy = bodies[3].position.y - bodies[0].position.y;
        //    printf("frame %d  r = %.4e\n", frame, std::sqrt(dx*dx + dy*dy));
        // }

        // --- Orbital range check ---
        // Every body's distance from the Sun in AU, once a second. Compare against
        // known perihelion and aphelion values to catch bad initial data. Mercury
        // should swing between 0.307 and 0.467, Earth should stay near 1.0.

        // static int frame = 0;
        // if (++frame % 60 == 0) {
        //    for (int i = 0; i < count; i++) {
        //        double dx = bodies[i].position.x - bodies[0].position.x;
        //        double dy = bodies[i].position.y - bodies[0].position.y;
        //        double dz = bodies[i].position.z - bodies[0].position.z;
        //        double r = std::sqrt(dx*dx + dy*dy + dz*dz) / 1.496e11;
        //        printf("%-8s %.3f AU\n", bodies[i].name.c_str(), r);
        //    }
        //    printf("\n");
        // }

        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            radius -= wheel * radius * 0.1f;
            if (radius < 3.0f) radius = 3.0f;
            if (radius > 800.0f) radius = 800.0f;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();

            theta -= delta.x * 0.005f;
            phi += delta.y * 0.005f;

            if (phi > 3.09f) phi = 3.09f;
            if (phi < 0.5f) phi = 0.5f;
        }

        camera.position.x = camera.target.x + radius * sinf(phi) * sinf(theta);
        camera.position.y = camera.target.y + radius * cosf(phi);
        camera.position.z = camera.target.z + radius * sinf(phi) * cosf(theta);

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

        for (int i = 0; i < count; i++) {
            Vector3 pos = {
                (float)(bodies[i].position.x * SCALE),
                (float)(bodies[i].position.y * SCALE),
                (float)(bodies[i].position.z * SCALE)
            };

            DrawSphere(pos, 1, bodies[i].color);
        }

                DrawGrid(80, 9.0f);
                DrawCubeWires({ 0, 0, 0}, 720, 720, 720, DARKGRAY);

            EndMode3D();
            DrawFPS(10, 10);
        DrawText("Left Click + Drag to Orbit. Scroll to Zoom.", 10, 40, 20, GREEN);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}