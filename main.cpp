#include "raylib.h"
#include "src/dimension.h"


struct Anim
{
    Rectangle Rec;
    Vector2 Pos;
    int Frame;
    float UpdateTime;
    float RunningTime;
};

bool IsScarfyOnTheGround(const Dimensions &window, const Anim &scarfyData);

int main()
{
    const Dimensions window = CreateNewDimension(512, 380);
    InitWindow(window.width, window.height, "Dasher");

    //nebula variables
    int nebulaVelocity = -600;
    Texture2D nebula = LoadTexture("../textures/12_nebula_spritesheet.png");
    Anim nebulaData = Anim{
            Rectangle{0.0, 0.0, static_cast<float>(nebula.width / 8), static_cast<float>(nebula.height / 8)},
            Vector2{static_cast<float>(window.width + 400), window.height - nebulaData.Rec.height},
            0,
            1.00 / 12.00,
            0
    };

    Anim nebulaData2 = Anim{
            Rectangle{0.0, 0.0, static_cast<float>(nebula.width / 8), static_cast<float>(nebula.height / 8)},
            Vector2{static_cast<float>(window.width + 1500), window.height - nebulaData.Rec.height},
            0,
            1.00 / 16.00,
            0
    };

    const int nebula_size = 5;
    Anim nebulae[nebula_size];

    for (int i = 0; i < nebula_size; i++)
    {
        int threshhold = 800 + (1000 * i);
        nebulae[i] = Anim{
                Rectangle{0.0, 0.0, static_cast<float>(nebula.width / 8), static_cast<float>(nebula.height / 8)},
                Vector2{static_cast<float>(window.width + threshhold), window.height - nebulaData.Rec.height},
                0,
                1.00 / 16.00,
                0
        };
    }

    //character variables
    Texture2D character = LoadTexture("../textures/scarfy.png");
    Anim scarfyData{};
    scarfyData.Rec.width = character.width / 6;
    scarfyData.Rec.height = character.height;
    scarfyData.Rec.x = 0;
    scarfyData.Rec.y = 0;
    scarfyData.Pos.x = (window.width / 2) - (scarfyData.Rec.width / 2);
    scarfyData.Pos.y = window.height - scarfyData.Rec.height;
    scarfyData.Frame = 0;
    scarfyData.UpdateTime = 1.00 / 12.00;
    scarfyData.RunningTime = 0;

    int velocity = 0;
    // acceleration due to gravity (pixels/frame)/ frame
    const int gravity = 1000;
    const int jumpForce = 600;
    bool isInAir = false;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        if (IsScarfyOnTheGround(window, scarfyData))
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

        //update nebula position
        for (auto &nebulaData: nebulae)
        {
            nebulaData.Pos.x += nebulaVelocity * deltaTime;
        }

        scarfyData.Pos.y += deltaTime * velocity;

        if (!isInAir)
        {
            scarfyData.RunningTime += deltaTime;
            if (scarfyData.RunningTime >= scarfyData.UpdateTime)
            {
                scarfyData.RunningTime = 0.00;
                scarfyData.Rec.x = (scarfyData.Frame * scarfyData.Rec.width);
                scarfyData.Frame++;
                if (scarfyData.Frame > 5)
                {
                    scarfyData.Frame = 0;
                }
            }
        }
        //nebula animation update
        for (int i = 0; i < nebula_size; i++)
        {
            nebulae[i].RunningTime += deltaTime;
            if (nebulae[i].RunningTime > nebulae[i].UpdateTime)
            {
                nebulae[i].RunningTime = 0.00;
                nebulae[i].Rec.x = (nebulae[i].Frame * nebulae[i].Rec.width);
                nebulae[i].Frame++;
                if (nebulae[i].Frame > 7)
                {
                    nebulae[i].Frame = 0;
                }
            }
        }

        //nebula drawings
        for (auto &nebulaData: nebulae)
        {
            DrawTextureRec(nebula, nebulaData.Rec, nebulaData.Pos, BLUE);
        }

        DrawTextureRec(character, scarfyData.Rec, scarfyData.Pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(character);
    UnloadTexture(nebula);
    CloseWindow();
}

bool IsScarfyOnTheGround(const Dimensions &window, const Anim &scarfyData)
{
    return scarfyData.Pos.y >= window.height - scarfyData.Rec.height;
}











