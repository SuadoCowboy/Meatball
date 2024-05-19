#include "raylib.h"

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    const float targetAspectRatio = 4.0f / 3.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib - aspect ratio");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Calculate new viewport dimensions
        float windowWidth = (float)GetScreenWidth();
        float windowHeight = (float)GetScreenHeight();
        float windowAspectRatio = windowWidth / windowHeight;

        int newWidth = windowWidth;
        int newHeight = windowHeight;
        int xOffset = 0;
        int yOffset = 0;

        if (windowAspectRatio > targetAspectRatio)
        {
            newWidth = (int)(windowHeight * targetAspectRatio);
            xOffset = (int)((windowWidth - newWidth) / 2);
        }
        else if (windowAspectRatio < targetAspectRatio)
        {
            newHeight = (int)(windowWidth / targetAspectRatio);
            yOffset = (int)((windowHeight - newHeight) / 2);
        }

        // Update the viewport
        BeginDrawing();
        ClearBackground(BLACK);
        BeginScissorMode(xOffset, yOffset, newWidth, newHeight);
        DrawRectangle(xOffset, yOffset, newWidth, newHeight, RAYWHITE);

        // Draw content
        DrawRectangle(xOffset + newWidth / 4, yOffset + newHeight / 4, newWidth / 2, newHeight / 2, RED);

        EndScissorMode();
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}