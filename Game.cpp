#include "Game.h"
#include <cstdlib>
#include <ctime>
#include "raylib.h"
#include <cmath>

// Game class constructor: initializes window, camera, and player/platforms
Game::Game() : platformManager(screenH * 0.7f) {
    InitWindow(screenW, screenH, "Slymania");
    SetTargetFPS(60);
    std::srand((unsigned)time(nullptr));

    camera.offset = { screenW * 0.3f, screenH * 0.5f };
    camera.target = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    background = LoadTexture("src/assets/background.png");
    platformManager.SeedInitial(-100.0f, 8);

    if (!platformManager.platforms.empty()) {
        auto &first = platformManager.platforms.front();
        player.Init({ first.rect.x + first.rect.width * 0.5f, first.rect.y - player.radius });
    }

    lastBonusDistance = 0; // Track last distance bonus applied
}

// Destructor
Game::~Game() {
    UnloadTexture(background);
    CloseWindow();
}

// Main loop
void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

// Update game logic
void Game::Update() {
    if (!gameOver) {
        float dt = GetFrameTime();
        elapsedTime += dt;

        cameraSpeed = 100.0f + elapsedTime * 1.0f;
        if (cameraSpeed > 999.0f) cameraSpeed = 999.0f;

        camera.target.x += cameraSpeed * dt;
        distanceTraveled += cameraSpeed * dt;

        // --- Add 1 life every 100 meters traveled ---
        int meters = (int)(distanceTraveled / 10.0f); // convert to meters
        if (meters / 100 > lastBonusDistance / 100) {
            lives++; // add 1 life
            if (lives > 5) lives = 5; // cap at 5
            lastBonusDistance = meters; // update last bonus checkpoint
        }

        player.Update(dt);
        HandleCollisions(dt);

        float cleanupX = camera.target.x - screenW * 1.2f;
        platformManager.Cleanup(cleanupX);

        float generateUntilX = camera.target.x + screenW * 1.8f;
        platformManager.Generate(generateUntilX, dt);

        float camBottom = camera.target.y + (screenH - camera.offset.y);
        float camLeft = camera.target.x - camera.offset.x;
        if (player.pos.y - player.radius > camBottom + 40) Respawn(true);
        if (player.pos.x + player.radius < camLeft + 6) Respawn(true);

        camera.target.y = screenH * 0.5f;
    } else {
        if (IsKeyPressed(KEY_R)) Reset();
    }
}

// Handle collisions
void Game::HandleCollisions(float dt) {
    player.onGround = false;
    Rectangle playerBB = player.GetBounds();

    for (auto &p : platformManager.platforms) {
        if (CheckCollisionRecs(playerBB, p.rect)) {
            float playerBottom = player.pos.y + player.radius;
            float platTop = p.rect.y;

            if (player.vel.y >= 0 && playerBottom - player.vel.y * dt <= platTop + 3) {
                player.LandOn(platTop);
            } else {
                if (player.pos.x < p.rect.x) player.StopHorizontalAt(p.rect.x - player.radius - 1);
                else player.StopHorizontalAt(p.rect.x + p.rect.width + player.radius + 1);
            }
        }
    }
}

// Respawn
void Game::Respawn(bool loseLife) {
    if (loseLife) {
        lives--;
        if (lives <= 0) { gameOver = true; return; }
    }

    float camLeft = camera.target.x - camera.offset.x;
    Platform* respawnPlat = nullptr;
    for (auto &p : platformManager.platforms) {
        if (p.rect.x + p.rect.width >= camLeft - 50) { respawnPlat = &p; break; }
    }
    if (!respawnPlat && !platformManager.platforms.empty())
        respawnPlat = &platformManager.platforms.front();

    if (respawnPlat) {
        player.pos.x = respawnPlat->rect.x + respawnPlat->rect.width * 0.5f;
        player.pos.y = respawnPlat->rect.y - player.radius - 1;
        player.vel = { 0,0 };
        player.onGround = true;
        camera.target.x = player.pos.x + 100;
    }
}

// Reset game
void Game::Reset() {
    platformManager.platforms.clear();
    platformManager.SeedInitial(-100.0f, 8);

    if (!platformManager.platforms.empty()) {
        auto &first = platformManager.platforms.front();
        player.Init({ first.rect.x + first.rect.width * 0.5f, first.rect.y - player.radius });
    }

    lives = 5;
    lastBonusDistance = 0;
    gameOver = false;
    camera.target.x = 0;
    distanceTraveled = 0;
}

// Draw
void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    float bgW = (float)background.width;
    float bgH = (float)background.height;

    float startX = floor((camera.target.x * parallaxFactor - camera.offset.x) / bgW) * bgW;
    float startY = floor((camera.target.y - camera.offset.y) / bgH) * bgH;

    for (float x = startX; x < camera.target.x - camera.offset.x + screenW; x += bgW) {
        for (float y = startY; y < camera.target.y - camera.offset.y + screenH; y += bgH) {
            DrawTexture(background, (int)x, (int)y, WHITE);
        }
    }

    platformManager.Draw();
    player.Draw();

    EndMode2D();

    DrawRectangle(8, 8, 220, 76, Fade(BLACK, 0.35f));
    DrawRectangleLines(8, 8, 220, 76, BLUE);

    DrawText(TextFormat("FP: %d", lives), 16, 16, 20, RED);
    DrawText(TextFormat("Distance: %d m", (int)(distanceTraveled / 10.0f)), 16, 40, 16, RED);

    if (gameOver) DrawText("GAME OVER - Press R to Restart", screenW / 2 - 180, screenH / 2 - 10, 20, RED);

    EndDrawing();
}
