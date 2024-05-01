#include "raylib.h"

int main()
{
    const unsigned short baseScreenWidth = 800;
    const unsigned short baseScreenHeight = 600;

    float SWU = 1.0f; // Screen Width unit
    float SHU = 1.0f; // Screen Height unit

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow((int)baseScreenWidth, (int)baseScreenHeight, "Screen Width/Height Unit System");
    
    Vector2 size = { .x = 200.0f, .y = 100.0f };
    Vector2 position = { -size.x, (float)baseScreenHeight/2 - size.y/2 };
    Vector2 velocity = { .x = 1.0f, .y = 1.0f };

    SetTargetFPS(60);

    double startTime = GetTime();
    double timeDifference = 0.0;
    while (!WindowShouldClose())
    {
        double dt = GetFrameTime() * 10; // try it later

        // Move the object
        position.x += velocity.x * dt;// * dt; // Example movement
        if (position.x > GetScreenWidth()) {
            position.x = -size.x; // this might break everything
            timeDifference = GetTime() - startTime;
            startTime = GetTime();
        }

        if (IsWindowResized()) {
            position.x /= SWU;
            position.y /= SHU;
            size.x /= SWU;
            size.y /= SHU;
            velocity.x /= SWU;
            velocity.y /= SHU;

            SWU = (float)GetScreenWidth() / baseScreenWidth;
            SHU = (float)GetScreenHeight() / baseScreenHeight;

            position.x *= SWU;
            position.y *= SHU;
            size.x *= SWU;
            size.y *= SHU;
            velocity.x *= SWU;
            velocity.y *= SHU;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the object in the 2D world
        DrawRectangle(position.x, position.y, size.x, size.y, RED);
        DrawText(TextFormat("%.2f", timeDifference), 0, 0, 24, BLACK);
        DrawText(TextFormat("%.1f", position.x), 0, 24, 24, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}