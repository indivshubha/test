#include "PlatformManager.h"
#include "raylib.h"
#include <cstdlib>

// Constructor: initializes base platform Y and loads resources
PlatformManager::PlatformManager(float baseY) : basePlatformY(baseY) {
    LoadResources();
}

// Loads the texture for platforms
void PlatformManager::LoadResources() {
    platformTexture = LoadTexture("assets/platform-1.png");
}

// Returns a random multiple of 16 between minVal and maxVal
int PlatformManager::RandRange16(int minVal, int maxVal) {
    int range = (maxVal - minVal) / 16 + 1;
    int r = std::rand() % range;
    return minVal + r * 16;
}

// Seeds initial platforms at the start of the game
void PlatformManager::SeedInitial(float startX, int count) {
    float x = startX;
    for (int i = 0; i < count; ++i) {
        int w = RandRange16(minPlatWidth, maxPlatWidth);
        Platform p;
        p.rect.x = x;
        p.rect.y = basePlatformY - RandRange16(0, 112); // Random vertical offset, multiples of 16
        p.rect.width = (float)w;
        p.rect.height = 19;
        p.color = BLACK;
        platforms.push_back(p);
        x += w + RandRange16(minGap, maxGap); // Move to next platform
    }
}

// Removes platforms that are no longer needed to save memory
void PlatformManager::Cleanup(float cleanupX) {
    while (!platforms.empty() && platforms.front().rect.x + platforms.front().rect.width < cleanupX) {
        platforms.erase(platforms.begin());
    }
}

// Generates new platforms as the player progresses
void PlatformManager::Generate(float untilX, float dt) {
    elapsedTime += dt;

    if (platforms.empty()) {
        Platform p;
        p.rect.x = untilX - 200;
        p.rect.width = 200;
        p.rect.y = basePlatformY;
        p.rect.height = 19;
        p.color = BLACK;
        platforms.push_back(p);
    }

    while (platforms.back().rect.x + platforms.back().rect.width < untilX) {
        Platform last = platforms.back();

        // Dynamically increase gap between platforms over time
        float gapIncreaseSpeed = 1.0f;  // pixels per second
        int dynamicMinGap = minGap + (int)(elapsedTime * gapIncreaseSpeed);
        int dynamicMaxGap = maxGap + (int)(elapsedTime * gapIncreaseSpeed);

        int gap = RandRange16(dynamicMinGap, dynamicMaxGap);
        int w = RandRange16(minPlatWidth, maxPlatWidth);

        Platform p;
        p.rect.x = last.rect.x + last.rect.width + gap;

        // Vertical offset in multiples of 16
        const int maxVerticalGap16 = 48; // ±48 pixels
        int verticalOffset = RandRange16(-maxVerticalGap16, maxVerticalGap16);
        p.rect.y = last.rect.y + verticalOffset;

        p.rect.width = (float)w;
        p.rect.height = 19;
        p.color = BLACK;

        platforms.push_back(p);
    }
}

// Draws all platforms on the screen
void PlatformManager::Draw() const {
    for (const auto &p : platforms) {
        Rectangle src = { 0, 0, (float)platformTexture.width, (float)platformTexture.height };
        Rectangle dest = { p.rect.x, p.rect.y, p.rect.width, p.rect.height };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(platformTexture, src, dest, origin, 0.0f, WHITE);

        DrawRectangleLinesEx(p.rect, 2.0f, DARKBROWN);
    }
}

// Destructor: unloads resources
PlatformManager::~PlatformManager() {
    UnloadResources();
}

// Unloads the platform texture
void PlatformManager::UnloadResources() {
    UnloadTexture(platformTexture);
}
