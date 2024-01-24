#include "raylib.h"
#include "src/dimension.h"

int main()
{
    const Dimensions window = CreateNewDimension(512, 380);
    InitWindow(window.width, window.height, "Dasher");

    //nebula variables
    Texture2D  nebula = LoadTexture("../textures/12_nebula_spritesheet.png");

    //character variables
    Texture2D character = LoadTexture("../textures/scarfy.png");
    Rectangle characterRectangle;
    characterRectangle.width = character.width / 6;
    characterRectangle.height = character.height;
    characterRectangle.x = 0;
    characterRectangle.y = 0;

    Vector2 characterPosition;
    characterPosition.x = (window.width / 2) - (characterRectangle.width / 2);
    characterPosition.y = window.height - characterRectangle.height;

    int velocity = 0;
    // acceleration due to gravity (pixels/frame)/ frame
    const int gravity = 1000;
    const int jumpForce = 600;
    bool isInAir = false;
    int currentFrame = 0;
    const float updateTime = 1.00 / 12.00;
    float runningTime = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        if (characterPosition.y >= window.height - characterRectangle.height)
        {
            velocity = 0;
            isInAir = false;
        } else
        {
            velocity += deltaTime * gravity;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity -= jumpForce;
            isInAir = true;
        }

        characterPosition.y += deltaTime * velocity;

        runningTime += deltaTime;
        if (runningTime >= updateTime)
        {
            runningTime = 0.00;
            characterRectangle.x = (currentFrame * characterRectangle.width);
            currentFrame++;
            if (currentFrame > 5)
            {
                currentFrame = 0;
            }
        }

        DrawTextureRec(character, characterRectangle, characterPosition, WHITE);

        EndDrawing();
    }

    UnloadTexture(character);
    UnloadTexture(nebula);
    CloseWindow();
}
