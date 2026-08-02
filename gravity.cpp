#include "raylib.h"

// masses for planetary objects in solar system
constexpr double SUN_MASS = 1.989e30;
constexpr double MERCURY_MASS = 3.301e23;
constexpr double VENUS_MASS = 4.867e24;
constexpr double EARTH_MASS = 5.972e24;
// constexpr double MOON_MASS = 7.346e22;           - Moon commented out until I can figure out what to do with it since its not relative to the sun.
constexpr double MARS_MASS = 6.417e23;
constexpr double JUPITER_MASS = 1.898e27;
constexpr double SATURN_MASS = 5.683e26;
constexpr double URANUS_MASS = 8.681e25;
constexpr double NEPTUNE_MASS = 1.024e26;
constexpr double PLUTO_MASS = 1.303e22;

// gravitational constant
constexpr double G = 6.67430e-11;

// window
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

struct Vec2 {
    double x, y;
};

struct Body {
    const char* name;
    double mass;
    double radius;
    Color color;
    Vec2 position;
    Vec2 velocity;
};

static Body bodies[] = {
    { "Sun", SUN_MASS, 6.957e8, GOLD, { 0.000e0, 0.000e0 },  {0.000e0, 0.000e0 } },
    { "Mercury", MERCURY_MASS, 2.439e6, DARKGRAY, {5.180e10, 2.589e10 }, {-2.114e4, 4.232e4 } },
    { "Venus", VENUS_MASS, 6.052e6, BEIGE, {-2.304e10, -1.057e11 }, {3.421e4, -7.460e3 } },
    { "Earth", EARTH_MASS, 6.371e6, BLUE, {9.646e10, -1.143e11 }, {2.274e4, 1.919e4 } },
    // { "Moon", MOON_MASS, 1.737e6, LIGHTGRAY, { 2.718e8, 2.718e8 }, { -7.227e2, -7.227e2 } },
    { "Mars", MARS_MASS, 3.389e6, MAROON, { 1.653e11, 1.569e11, }, { -1.657e4, 1.745e4 } },
    { "Jupiter", JUPITER_MASS, 6.991e7, BROWN, { -4.203e11, 6.665e11 }, { -1.119e4, -6.754e3 } },
    { "Saturn", SATURN_MASS, 5.823e7, GOLD, { 1.385e12, 3.699e11 }, { -2.497e3, 9.352e3 } },
    { "Uranus", URANUS_MASS, 2.536e7, SKYBLUE, { 1.077e12, 2.663e12 }, { -6.307e3, 2.551e3 } },
    { "Neptune", NEPTUNE_MASS, 2.462e7, DARKBLUE, { 4.491e12, 1.901e11 }, { -2.296e2, 5.425e3 } },
    { "Pluto", PLUTO_MASS, 1.188e6, LIGHTGRAY, { 7.737e11, -5.855e12 }, { 4.698e3, 6.208e2 } }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gravity");
    CloseWindow();
    return 0;
}