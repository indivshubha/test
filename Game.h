#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Player.h"
#include "PlatformManager.h"

// Main Game class: manages game loop, state, and objects
class Game {
public:
    Game();      // Constructor: initializes game
    ~Game();     // Destructor: clean up
    void Run();  // Runs the main game loop

private:
    const int screenW = 1280; // Window width
    const int screenH = 720;  // Window height
    Camera2D camera{};        // 2D camera for scrolling
    Player player;            // Player object
    PlatformManager platformManager; // Manages platforms
    int lives{5};             // Player lives
    bool gameOver{false};     // Game over flag
    float cameraSpeed{100.0f}; // Camera horizontal speed
    float distanceTraveled{0.0f}; // Total distance traveled
    float elapsedTime{0.0f};      // Elapsed game time
    int lastBonusDistance{0};      // Last distance bonus applied

    // Background texture
    Texture2D background;
    float parallaxFactor{0.5f}; // Adjust for depth effect (1.0 = fixed to camera, <1 = slower)

    void Update();                    // Updates game logic
    void HandleCollisions(float dt);  // Handles player/platform collisions
    void Respawn(bool loseLife);      // Respawns player, optionally loses a life
    void Reset();                     // Resets game state
    void Draw();                      // Draws game scene and UI
};

#endif
