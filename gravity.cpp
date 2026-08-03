#include "raylib.h"
#include <cmath>
#include <iterator>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>

// gravitational constant
constexpr double G = 6.67430e-11;

// timestep
constexpr double dt = 86400.0;      // 1 day per frame

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

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gravity");
    SetTargetFPS(60);
    const int count = std::size(bodies);

    while (!WindowShouldClose()) {

        // --- physics --- //
        // set to zero
        for (int i = 0; i < count; i++) {
            bodies[i].fx = 0.0;
            bodies[i].fy = 0.0;
        }

        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                // separation
                double dx = bodies[j].position.x - bodies[i].position.x;
                double dy = bodies[j].position.y - bodies[i].position.y;
                // distance
                double r = std::sqrt(dx * dx + dy * dy);
                // force of magnitude
                double f = G * bodies[i].mass * bodies[j].mass / (r * r);
                // magnitude times unit direction
                double fx = f * (dx / r);
                double fy = f * (dy / r);
                // accumulator
                bodies[i].fx += fx;
                bodies[i].fy += fy;
                bodies[j].fx -= fx;
                bodies[j].fy -= fy;
            }
        }

        for (int i = 0; i < count; i++) {
            // acceleration
            double ax = bodies[i].fx / bodies[i].mass;
            double ay = bodies[i].fy / bodies[i].mass;
            // integration
            bodies[i].velocity.x += ax * dt;
            bodies[i].velocity.y += ay * dt;
            bodies[i].position.x += bodies[i].velocity.x * dt;
            bodies[i].position.y += bodies[i].velocity.y * dt;
        }

        // test
        // static int frame = 0;
        // frame++;
        // if (frame % 30 == 0) {
        //    double dx = bodies[3].position.x - bodies[0].position.x;
        //    double dy = bodies[3].position.y - bodies[0].position.y;
        //    printf("frame %d  r = %.4e\n", frame, std::sqrt(dx*dx + dy*dy));
        // }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < count; i++) {
            double screen_x = (double)(SCREEN_WIDTH  / 2 + bodies[i].position.x * SCALE);
            double screen_y = (double)(SCREEN_HEIGHT / 2 - bodies[i].position.y * SCALE);

            DrawCircle((int)screen_x, (int)screen_y, 6, bodies[i].color);
        }

        EndDrawing();

    }
    CloseWindow();
    return 0;
}