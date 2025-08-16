#include "Game.h"
#include <cstdlib>
#include <ctime>
#include "raylib.h"

// Game class constructor: initializes window, camera, and player/platforms
Game::Game() : platformManager(screenH * 0.7f) {
    // Initialize the game window
    InitWindow(screenW, screenH, "Slymania");
    SetTargetFPS(60);

    // Seed random number generator
    std::srand((unsigned)time(nullptr));

    // Set up camera properties
    camera.offset = { screenW * 0.3f, screenH * 0.5f };
    camera.target = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Seed initial platforms
    platformManager.SeedInitial(-100.0f, 8);

    // Initialize player position on the first platform
    if (!platformManager.platforms.empty()) {
        auto &first = platformManager.platforms.front();
        player.Init({ first.rect.x + first.rect.width * 0.5f, first.rect.y - player.radius });
    }
}

// Main game loop: runs until window is closed
void Game::Run() {
    while (!WindowShouldClose()) {
        Update(); // Update game state
        Draw();   // Render game
    }
    CloseWindow(); // Clean up and close window
}

// Update game logic: handles movement, collisions, camera, and respawn
void Game::Update() {
    if (!gameOver) {
        float dt = GetFrameTime(); // Time since last frame

        elapsedTime += dt; // Track total elapsed time

        // Increase camera speed over time, capped at 999
        cameraSpeed = 100.0f + elapsedTime * 1.0f;
        if (cameraSpeed > 999.0f) cameraSpeed = 999.0f;

        // Move camera and track distance
        camera.target.x += cameraSpeed * dt;
        distanceTraveled += cameraSpeed * dt;

        // Update player and handle collisions
        player.Update(dt);
        HandleCollisions(dt);

        // Remove platforms that are far behind the camera
        float cleanupX = camera.target.x - screenW * 1.2f;
        platformManager.Cleanup(cleanupX);

        // Generate new platforms ahead of the camera
        float generateUntilX = camera.target.x + screenW * 1.8f;
        platformManager.Generate(generateUntilX, dt);

        // Check if player falls below or moves too far left, then respawn
        float camBottom = camera.target.y + (screenH - camera.offset.y);
        float camLeft = camera.target.x - camera.offset.x;
        if (player.pos.y - player.radius > camBottom + 40) Respawn(true);
        if (player.pos.x + player.radius < camLeft + 6) Respawn(true);

        // Keep camera vertically centered
        camera.target.y = screenH * 0.5f;
    } else {
        // If game over, allow restart with R key
        if (IsKeyPressed(KEY_R)) Reset();
    }
}

// Handle collisions between player and platforms
void Game::HandleCollisions(float dt) {
    player.onGround = false; // Assume player is airborne
    Rectangle playerBB = player.GetBounds(); // Get player bounding box

    // Check collision with each platform
    for (auto &p : platformManager.platforms) {
        if (CheckCollisionRecs(playerBB, p.rect)) {
            float playerBottom = player.pos.y + player.radius;
            float platTop = p.rect.y;
            // If player is falling and lands on top of platform
            if (player.vel.y >= 0 && playerBottom - player.vel.y * dt <= platTop + 3) {
                player.LandOn(platTop);
            } else {
                // If colliding from the side, stop horizontal movement
                if (player.pos.x < p.rect.x) player.StopHorizontalAt(p.rect.x - player.radius - 1);
                else player.StopHorizontalAt(p.rect.x + p.rect.width + player.radius + 1);
            }
        }
    }
}

// Respawn player on nearest platform, optionally lose a life
void Game::Respawn(bool loseLife) {
    if (loseLife) {
        lives--;
        if (lives <= 0) { gameOver = true; return; } // End game if out of lives
    }
    float camLeft = camera.target.x - camera.offset.x;
    Platform* respawnPlat = nullptr;
    // Find platform to respawn on
    for (auto &p : platformManager.platforms) {
        if (p.rect.x + p.rect.width >= camLeft - 50) { respawnPlat = &p; break; }
    }
    // If none found, use first platform
    if (!respawnPlat && !platformManager.platforms.empty())
        respawnPlat = &platformManager.platforms.front();
    // Set player position and reset velocity
    if (respawnPlat) {
        player.pos.x = respawnPlat->rect.x + respawnPlat->rect.width * 0.5f;
        player.pos.y = respawnPlat->rect.y - player.radius - 1;
        player.vel = { 0,0 };
        player.onGround = true;
        camera.target.x = player.pos.x + 100;
    }
}

// Reset game state to initial conditions
void Game::Reset() {
    platformManager.platforms.clear(); // Remove all platforms
    platformManager.SeedInitial(-100.0f, 8); // Seed initial platforms
    if (!platformManager.platforms.empty()) {
        auto &first = platformManager.platforms.front();
        player.Init({ first.rect.x + first.rect.width * 0.5f, first.rect.y - player.radius });
    }
    lives = 5; // Reset lives
    gameOver = false; // Clear game over flag
    camera.target.x = 0; // Reset camera position
    distanceTraveled = 0; // Reset distance
}

// Draw game scene and UI
void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE); // Set background color

    BeginMode2D(camera); // Enable camera mode

    // Draw vertical grid lines for background
    for (int gx = -2000; gx < 50000; gx += 200)
        DrawLineEx(Vector2{ (float)gx, 0 }, Vector2{ (float)gx, 2000 }, 1.0f, Fade(GRAY, 0.08f));

    platformManager.Draw(); // Draw platforms
    player.Draw();          // Draw player
    EndMode2D();

    // Draw UI panel
    DrawRectangle(8, 8, 220, 76, Fade(BLACK, 0.35f));
    DrawRectangleLines(8, 8, 220, 76, BLUE);
    DrawText(TextFormat("Lives: %d", lives), 16, 16, 20, RED);
    DrawText(TextFormat("Distance: %d m", (int)(distanceTraveled / 10.0f)), 16, 40, 16, BLACK);
    if (gameOver) DrawText("GAME OVER - Press R to Restart", screenW / 2 - 180, screenH / 2 - 10, 20, RED);
    
    EndDrawing();
}