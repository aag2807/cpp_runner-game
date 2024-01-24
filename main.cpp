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

void UpdateAnimData(Anim *data, float deltaTime, int frameCount);

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

    const int nebula_size = 2;
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

    Texture2D background = LoadTexture("../textures/far-buildings.png");
    Texture2D midground = LoadTexture("../textures/back-buildings.png");
    Texture2D foreground = LoadTexture("../textures/foreground.png");

    float bgX{};
    float mgX{};
    float fgX{};

    float finishLine = nebulae[nebula_size - 1].Pos.x + 400;
    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);


        //drawing background
        bgX -= 20 * deltaTime;
        if (bgX <= -background.width * 2)
        {
            bgX = 0;
        }
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        mgX -= 40 * deltaTime;
        fgX -= 80 * deltaTime;

        if (mgX <= -midground.width * 2)
        {
            mgX = 0;
        }

        Vector2 mg1Pos{bgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{bgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        if (fgX <= -foreground.width * 2)
        {
            mgX = 0;
        }

        Vector2 fgPos1{bgX, 0.0};
        DrawTextureEx(foreground, fgPos1, 0.0, 2.0, WHITE);
        Vector2 fgPos2{bgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fgPos2, 0.0, 2.0, WHITE);

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

        finishLine += nebulaVelocity * deltaTime;

        scarfyData.Pos.y += deltaTime * velocity;

        if (!isInAir)
        {
            UpdateAnimData(&scarfyData, deltaTime, 5);
        }

        //nebula animation update
        for (auto &nebulaData: nebulae)
        {
            UpdateAnimData(&nebulaData, deltaTime, 7);
        }

        for (Anim nebula: nebulae)
        {
            float pad = 50;

            Rectangle nebRect = Rectangle{
                    nebula.Pos.x + pad,
                    nebula.Pos.y + pad,
                    nebula.Rec.width - (2 * pad),
                    nebula.Rec.height - (2 * pad)
            };

            Rectangle scarfyRect = {
                    scarfyData.Pos.x,
                    scarfyData.Pos.y,
                    scarfyData.Rec.width,
                    scarfyData.Rec.height,
            };

            if (CheckCollisionRecs(nebRect, scarfyRect))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Crashed !", window.width / 2 - 100, 20, 50, RED);
        }
        else if(scarfyData.Pos.x >= finishLine)
        {
            DrawText("You WON!", window.width / 2 - 100, 20, 50, RED);
        }
        else
        {
            //nebula drawings
            for (auto &nebulaData: nebulae)
            {
                DrawTextureRec(nebula, nebulaData.Rec, nebulaData.Pos, BLUE);
            }

            DrawTextureRec(character, scarfyData.Rec, scarfyData.Pos, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(character);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}

bool IsScarfyOnTheGround(const Dimensions &window, const Anim &scarfyData)
{
    return scarfyData.Pos.y >= window.height - scarfyData.Rec.height;
}


void UpdateAnimData(Anim *data, float deltaTime, int frameCount)
{
    data->RunningTime += deltaTime;
    if (data->RunningTime > data->UpdateTime)
    {
        data->RunningTime = 0.00;
        data->Rec.x = (data->Frame * data->Rec.width);
        data->Frame++;
        if (data->Frame > frameCount)
        {
            data->Frame = 0;
        }
    }
}








