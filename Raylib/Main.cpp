#include "raylib.h";

bool collisions(Vector2& position, int size, Rectangle& rec);
void restart(Vector2& position, int velocityX, int velocityY, bool& begin, int& randomX, int& randomY);
void inputDetected(Rectangle& player, Rectangle& enemy);
void checkCollisions(Vector2& ballPosition, int ballRadius, int& ballVelocityX, Rectangle& player, Rectangle& enemy, Sound sound);
void ballColisions(Vector2& ballPosition, int ballRadius, int& ballVelocityY, int& enemyScore, int& playerScore, bool& begin, float& timer, int count, Rectangle& enemy, Rectangle& player);
void endMatch(int& enemyScore, int& playerScore, int finalScore, bool& gameFinish);

    int main(void)
    {
        // Initialization
        //--------------------------------------------------------------------------------------
        const int screenWidth = 800;
        const int screenHeight = 450;
        float rectangleHeight = 80;
        float rectangleWidth = 10;
        int ballVelocityX = 250;
        int ballVelocityY = 250;
        int ballRadius = 15;
        int randomX, randomY;
        int playerScore = 0, enemyScore = 0;
        int finalScore = 8;
        bool begin = true, gameFinish = false;
        const int count = 3;
        float timer = count;

        InitWindow(screenWidth, screenHeight, "PONG");
        InitAudioDevice();     

        Music music = LoadMusicStream("MajorLoss.mp3"); 
        Sound sound = LoadSound("Sonido.ogg");
        PlayMusicStream(music);
        
        Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
        Rectangle player = { 20, (float)screenHeight / 2 , rectangleWidth, rectangleHeight };
        Rectangle enemy = { 780, (float)screenHeight / 2, rectangleWidth, rectangleHeight };
        
        SetTargetFPS(60);
        do
        {
            // Update
            inputDetected(player, enemy);
            UpdateMusicStream(music);

#if _DEBUG
            if (IsKeyPressed(KEY_SPACE))
            {
                ballVelocityX *= -1;
            }
#endif
            if (timer <= 0)
            {

                restart(ballPosition, ballVelocityX, ballVelocityY, begin, randomX, randomY);
                ballColisions(ballPosition, ballRadius, ballVelocityY, enemyScore, playerScore, begin, timer, count, enemy, player);
                checkCollisions(ballPosition, ballRadius, ballVelocityX, player, enemy, sound);
            }
            else
            {
                timer -= GetFrameTime();
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(GOLD);
            DrawLineEx({ 400 , 0 }, { 400, 450 }, 5, BLACK);
            DrawCircleV(ballPosition, ballRadius, RED);
            DrawRectangleRec(player, DARKBLUE);
            DrawRectangleRec(enemy, BLUE);
            DrawText(TextFormat("%i", playerScore), 200, 100, 50, BLACK);
            DrawText(TextFormat("%i", enemyScore), 600, 100, 50, BLACK);
            if (timer >= 0)
            {
                DrawText(TextFormat("%i", (int)timer), screenWidth/2 - MeasureText(TextFormat("%i", (int)timer), 50) / 2 , screenHeight/2 - 25, 50, SKYBLUE);
            }
            endMatch(enemyScore, playerScore, finalScore, gameFinish);
            

            EndDrawing();

            } while (!WindowShouldClose() && !gameFinish);

        UnloadMusicStream(music);   
        UnloadSound(sound);
        CloseAudioDevice();
        CloseWindow();

        return 0;
    }

    void ballColisions(Vector2 & ballPosition, int ballRadius, int& ballVelocityY, int& enemyScore, int& playerScore, bool& begin, float& timer, int count, Rectangle& enemy, Rectangle& player)
    {
        if (ballPosition.y + ballRadius >= GetScreenHeight() || ballPosition.y - ballRadius <= 0)
        {
            ballVelocityY *= -1;
        }
        if (ballPosition.x + ballRadius >= GetScreenWidth())
        {
            playerScore++;
            begin = true;
            timer = count;
            ballPosition.x = GetScreenWidth() / 2;
            ballPosition.y = GetScreenHeight() / 2;
            enemy.y = GetScreenHeight() / 2;
            player.y = GetScreenHeight() / 2;
        }
        else if (ballPosition.x + ballRadius <= 0)
        {
            enemyScore++;
            begin = true;
            timer = count;
            ballPosition.x = GetScreenWidth() / 2;
            ballPosition.y = GetScreenHeight() / 2;
            enemy.y = GetScreenHeight() / 2;
            player.y = GetScreenHeight() / 2;
        }
    }
    void restart(Vector2 & position, int velocityX, int velocityY, bool& begin, int& randomX, int& randomY)
    {
        if (begin)
        {
            position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
            randomX = GetRandomValue(1, 2);
            randomY = GetRandomValue(1, 2);
        }

        if (randomX == 1)
        {
            position.x += velocityX * GetFrameTime();
        }
        else if (randomX == 2)
        {
            position.x -= velocityX * GetFrameTime();
        }

        if (randomY == 1)
        {
            position.y += velocityY * GetFrameTime();
        }
        else if (randomY == 2)
        {
            position.y -= velocityY * GetFrameTime();
        }
        begin = false;
    }
    void inputDetected(Rectangle & player, Rectangle & enemy)
    {
        float velocity = 250.00f * GetFrameTime();
        if ((player.y + player.height) <= GetScreenHeight())
        {
            if (IsKeyDown(KEY_S)) player.y += velocity;
        }
        if (player.y >= 0)
        {
            if (IsKeyDown(KEY_W)) player.y -= velocity;
        }
        if ((enemy.y + enemy.height) <= GetScreenHeight())
        {
            if (IsKeyDown(KEY_DOWN)) enemy.y += velocity;
        }
        if (enemy.y >= 0)
        {
            if (IsKeyDown(KEY_UP)) enemy.y -= velocity;
        }
    }
    void checkCollisions(Vector2& ballPosition, int ballRadius, int& ballVelocityX, Rectangle& player, Rectangle& enemy, Sound sound)
    {
        if (collisions(ballPosition, ballRadius, player))
        {
                ballVelocityX *= -1;
                ballPosition.x += ballRadius;
                PlaySound(sound);
        }
        if (collisions(ballPosition, ballRadius, enemy))
        {
                ballVelocityX *= -1;
                ballPosition.x -= ballRadius;
                PlaySound(sound);
        }
    }
    bool collisions(Vector2 & position, int size, Rectangle & rec)
    {
        return CheckCollisionCircleRec(position, size, rec);
    }
    void endMatch(int& enemyScore, int& playerScore, int finalScore, bool& gameFinish)
    {
 
            if (playerScore == finalScore)
            {
                DrawText("Player 1 has won", 125, GetScreenHeight() / 2, 60, DARKBLUE);
                DrawText("Create by Ramiro Guzman", 125, 425, 20, RED);
                gameFinish = true;
            }
            if (enemyScore == finalScore)
            {
                DrawText("Player 2 has won", 125, GetScreenHeight() / 2, 60, BLUE);
                DrawText("Create by Ramiro Guzman", 125, 425, 20, RED);
                gameFinish = true;
            }

      }




