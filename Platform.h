#ifndef PLATFORM_H // Prevent multiple inclusions of this header file
#define PLATFORM_H

#include "raylib.h" // Include Raylib library for Rectangle and Color types

// Structure representing a platform in the game
struct Platform {
    Rectangle rect; // Rectangle defining the platform's position and size
    Color color;    // Color of the platform
};

#endif // PLATFORM_H