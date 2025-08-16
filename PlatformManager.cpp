#include "PlatformManager.h"
#include <cstdlib>

// Constructor: sets the base Y position for platforms
PlatformManager::PlatformManager(float baseY) : basePlatformY(baseY) {}

// Returns a random multiple of 16 between minVal and maxVal
int PlatformManager::RandRange16(int minVal, int maxVal) {
    int range = (maxVal - minVal) / 16 + 1;
    int r = std::rand() % range;
    return minVal + r * 16;
}

// Seeds the initial set of platforms at game start
void PlatformManager::SeedInitial(float startX, int count) {
    float x = startX;
    int widths[4] = {64, 80, 96, 112};

    for (int i = 0; i < count; ++i) {
        int width = widths[std::rand() % 4];

        Platform p;
        p.rect.x = x;
        p.rect.y = basePlatformY - RandRange16(0, 112);
        p.rect.width  = (float)width;
        p.rect.height = 16;
        p.color = {0,0,0,125};

        platforms.push_back(p);
        x += p.rect.width + RandRange16(minGap, maxGap);
    }
}

// Removes platforms that have moved out of view (to the left)
void PlatformManager::Cleanup(float cleanupX) {
    while (!platforms.empty() &&
           platforms.front().rect.x + platforms.front().rect.width < cleanupX) {
        platforms.erase(platforms.begin());
    }
}

// Dynamically generates new platforms as the player progresses
void PlatformManager::Generate(float untilX, float dt) {
    elapsedTime += dt;

    if (platforms.empty()) {
        Platform p;
        p.rect.x = untilX - 200;
        p.rect.width = 200;
        p.rect.y = basePlatformY;
        p.rect.height = 16;
        p.color = {0,0,0,125};
        platforms.push_back(p);
    }

    int widths[4] = {64, 80, 96, 112};

    while (platforms.back().rect.x + platforms.back().rect.width < untilX) {
        Platform last = platforms.back();

        float gapIncreaseSpeed = 1.0f;
        int dynamicMinGap = minGap + (int)(elapsedTime * gapIncreaseSpeed);
        int dynamicMaxGap = maxGap + (int)(elapsedTime * gapIncreaseSpeed);

        int gap = RandRange16(dynamicMinGap, dynamicMaxGap);
        int width = widths[std::rand() % 4];

        Platform p;
        p.rect.x = last.rect.x + last.rect.width + gap;

        // Vertical offset
        const int maxVerticalGap16 = 48;
        int verticalOffset = RandRange16(-maxVerticalGap16, maxVerticalGap16);
        p.rect.y = last.rect.y + verticalOffset;

        p.rect.width  = (float)width;
        p.rect.height = 16;
        p.color = {0,0,0,125};

        platforms.push_back(p);
    }
}

// Draws all platforms to the screen
void PlatformManager::Draw() const {
    for (const auto &p : platforms) {
        DrawRectangleRec(p.rect, {0,0,0,125});
    }
}

// Destructor: currently does nothing, but defined for completeness
PlatformManager::~PlatformManager() {}
