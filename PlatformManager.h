#ifndef PLATFORM_MANAGER_H // Prevent multiple inclusions of this header file
#define PLATFORM_MANAGER_H

#include <vector>      // For using std::vector
#include "Platform.h"  // Include Platform struct definition
#include "raylib.h"    // For Texture2D

// Class responsible for managing all platforms in the game
class PlatformManager {
public:
    std::vector<Platform> platforms; // List of platforms currently in the game

    // Basic configuration for platform generation
    int minPlatWidth{64};   // Minimum width of a platform (multiple of 16)
    int maxPlatWidth{160};  // Maximum width of a platform (multiple of 16)
    int minGap{32};         // Minimum gap between platforms (multiple of 16)
    int maxGap{128};        // Maximum gap between platforms (multiple of 16)
    float basePlatformY;    // Y position for the base platform

    Texture2D platformTexture;   // Texture used for rendering platforms

    PlatformManager(float baseY); // Constructor
    ~PlatformManager();           // Destructor

    // Resource management for platform textures
    void LoadResources();
    void UnloadResources();

    // Platform generation and cleanup
    void SeedInitial(float startX, int count);      // Seed initial platforms
    void Cleanup(float cleanupX);                   // Remove platforms past a certain X
    void Generate(float untilX, float dt);          // Generate platforms up to a certain X
    void Draw() const;                              // Draw all platforms

private:
    int RandRange16(int minVal, int maxVal);        // Utility for random multiple of 16
    float elapsedTime = 0.0f;                       // Time accumulator for generation logic
};

#endif // PLATFORM_MANAGER_H
