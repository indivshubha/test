#include "Player.h"
#include "raylib.h"

// Initialize the player with starting position and default values
void Player::Init(const Vector2 &startPos) {
    pos = startPos;
    vel = {0, 0};
    onGround = false;

    state = IDLE;
    direction = RIGHT;

    // Player movement configuration
    moveSpeed = 200.0f;   
    elapsedTime = 0.0f;   
    speedIncreaseRate = 1.0f; 
    maxMoveSpeed = 999.0f;    

    LoadTextures(); // Load player textures
}

// Update player state and movement based on input and physics
void Player::Update(float dt) {

    elapsedTime += dt;

    // Gradually increase move speed up to a maximum
    moveSpeed += speedIncreaseRate * dt;
    if (moveSpeed > maxMoveSpeed) moveSpeed = maxMoveSpeed;

    // Handle horizontal movement input
    float inputX = 0.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) inputX -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputX += 1.0f;

    // Check for dash input
    bool dashHeld = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    float hVel = inputX * moveSpeed; // Calculate horizontal velocity
    if (dashHeld && inputX != 0) hVel *= dashMultiplier;

    vel.x = hVel;

    // Update facing direction
    if (inputX < 0) direction = LEFT;
    else if (inputX > 0) direction = RIGHT;

    // Update player state (idle or moving)
    if (inputX == 0) state = IDLE;
    else state = MOVING;

    // Handle jump input
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && onGround) {
        isJumping = true;  
        jumpHoldTime = 0.0f;    
        vel.y = -jumpImpulse;   
        onGround = false;
    }

    // Handle jump hold for variable jump height
    bool jumpHeld = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);

    if (isJumping && jumpHeld && jumpHoldTime < maxJumpHoldTime) {
        vel.y = -jumpImpulse;
        jumpHoldTime += dt;
    } 
    else {
        isJumping = false;
    }

    // Apply gravity and clamp fall speed
    vel.y += gravity * dt;
    if (vel.y > maxFallSpeed) vel.y = maxFallSpeed;

    // Update position based on velocity
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
}

// Get the bounding rectangle for collision detection
Rectangle Player::GetBounds() const {
    return { pos.x - radius + 1, pos.y - radius, radius * 2 - 2, radius * 2 };
}

// Called when the player lands on a platform
void Player::LandOn(float platformY) {
    int texHeight = texIdleRight.height;
    pos.y = platformY - (texHeight / 2.0f);
    vel.y = 0;
    onGround = true;
}

// Stop horizontal movement and set new X position
void Player::StopHorizontalAt(float newX) {
    pos.x = newX;
    vel.x = 0;
}

// Draw the player using the correct texture and orientation
void Player::Draw() const {
    Texture2D currentTex;

    if (direction == LEFT) {
        if (state == IDLE)
            currentTex = texIdleLeft;
        else
            currentTex = texMoveLeft;
    } else { // RIGHT
        if (state == IDLE)
            currentTex = texIdleRight;
        else
            currentTex = texMoveRight;
    }

    float scale = 0.55f;
    // Scale is used to visually align the player with the platform
    Vector2 origin = { (currentTex.width) / 2.0f, (currentTex.height*scale) / 2.0f };
    DrawTexturePro(currentTex, 
                   Rectangle{0, 0, (float)currentTex.width, (float)currentTex.height}, 
                   Rectangle{pos.x, pos.y, (float)currentTex.width, (float)currentTex.height}, 
                   origin, 0.0f, WHITE);
}

// Load player textures for different states and directions
void Player::LoadTextures() {
    texIdleLeft = LoadTexture("src/assets/idle-left.png");
    texIdleRight = LoadTexture("src/assets/idle-right.png");
    texMoveLeft = LoadTexture("src/assets/idle-left.png");
    texMoveRight = LoadTexture("src/assets/idle-right.png");
}

// Unload player textures to free resources
void Player::UnloadTextures() {
    UnloadTexture(texIdleLeft);
    UnloadTexture(texIdleRight);
    UnloadTexture(texMoveLeft);
    UnloadTexture(texMoveRight);
}

// Destructor: unload textures when player is destroyed
Player::~Player() {
    UnloadTextures();
}