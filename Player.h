#ifndef PLAYER_H // Prevent multiple inclusions of this header file
#define PLAYER_H

#include "raylib.h" // Include Raylib for Vector2, Texture2D, Rectangle


// Enum for player state (idle or moving)
enum PlayerState {
    IDLE,
    MOVING
};

// Enum for player facing direction
enum PlayerDirection {
    LEFT,
    RIGHT
};

// Class representing the player character
class Player {

    // Basic player configuration and jump logic
    float elapsedTime = 0.0f;           // Time since start, used for speed increase
    float speedIncreaseRate = 1.0f;     // Rate at which speed increases
    float maxMoveSpeed = 999.0f;        // Maximum allowed move speed

    float jumpHoldTime = 0.0f;          // Time jump key is held
    const float maxJumpHoldTime = 0.25f;// Maximum jump hold duration
    bool isJumping = false;             // Is the player currently jumping

public:
    Vector2 pos{};                      // Player position
    Vector2 vel{};                      // Player velocity
    float radius{32.0f};                // Player collision radius
    bool onGround{false};               // Is the player on the ground

    // Player movement settings
    float moveSpeed{180.0f};            // Current move speed
    float dashMultiplier{2.5f};         // Dash speed multiplier
    float jumpImpulse{240.0f};          // Jump impulse strength
    float gravity{1000.0f};             // Gravity applied to player
    float maxFallSpeed{800.0f};         // Maximum fall speed

    // Player methods
    void Init(const Vector2 &startPos); // Initialize player
    void LoadTextures();                // Load player textures
    void UnloadTextures();              // Unload player textures
    void Update(float dt);              // Update player state
    Rectangle GetBounds() const;        // Get player collision bounds
    void LandOn(float platformY);       // Land on a platform
    void StopHorizontalAt(float newX);  // Stop horizontal movement
    void Draw() const;                  // Draw player

    // Textures for different states and directions
    Texture2D texIdleLeft{};
    Texture2D texIdleRight{};
    Texture2D texMoveLeft{};
    Texture2D texMoveRight{};

    PlayerState state{IDLE};            // Current player state
    PlayerDirection direction{RIGHT};   // Current facing direction

    // Destructor
    ~Player();

};

#endif // PLAYER_H