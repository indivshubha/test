#ifndef PLATFORM_MANAGER_H
#define PLATFORM_MANAGER_H

#include <vector>
#include "Platform.h"
#include "raylib.h"

class PlatformManager {
public:
    std::vector<Platform> platforms;

    int minGap{32};
    int maxGap{128};
    float basePlatformY;

    PlatformManager(float baseY);
    ~PlatformManager();

    void SeedInitial(float startX, int count);
    void Cleanup(float cleanupX);
    void Generate(float untilX, float dt);
    void Draw() const;

private:
    int RandRange16(int minVal, int maxVal);
    float elapsedTime = 0.0f;
};

#endif // PLATFORM_MANAGER_H
