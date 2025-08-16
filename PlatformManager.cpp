#include "PlatformManager.h"
#include <cstdlib>

// Constructor
PlatformManager::PlatformManager(float baseY) : basePlatformY(baseY) {}

// Random multiple of 16
int PlatformManager::RandRange16(int minVal, int maxVal) {
    int range = (maxVal - minVal) / 16 + 1;
    int r = std::rand() % range;
    return minVal + r * 16;
}

// Seed starting platforms
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

// Cleanup old platforms
void PlatformManager::Cleanup(float cleanupX) {
    while (!platforms.empty() &&
           platforms.front().rect.x + platforms.front().rect.width < cleanupX) {
        platforms.erase(platforms.begin());
    }
}

// Generate new platforms dynamically
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

// Draw platforms as solid dark 
void PlatformManager::Draw() const {
    for (const auto &p : platforms) {
        DrawRectangleRec(p.rect, {0,0,0,125});
    }
}

// Destructor
PlatformManager::~PlatformManager() {}
