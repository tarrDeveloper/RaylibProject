#include "raylib.h"
    
// SHIP CLASS
class PlayerShip
{
    // stores position of player
    int x;
    int y;
    
    // stores where the player should lerp to
    int xto;
    int yto;
    
    // stores column position of player
    int column;
    
    // stores the fade of the column
    int columnAlpha = 0;
    
    public:
        PlayerShip(int c)
        {
            this->column = c;
            this->x = (270+((this->column)-1)*180);
            this->y = 960-32;
        }
        void Update()
        {
            // managing the column
            int inp = 0;
            if (IsKeyPressed(KEY_RIGHT)) inp++;
            if (IsKeyPressed(KEY_LEFT))  inp--;
            this->column+=inp;
            if (this->column > 2) {this->column = 2; inp = 0;}
            if (this->column < 0) {this->column = 0; inp = 0;}
            
            // managing the x and y lerp
            this->xto = (270+((this->column)-1)*180);
            
            // managing the lerp
            this->x = (this->xto+this->x)/2;
            
            // managing the column alpha
            if (inp != 0)
            {
                columnAlpha = 255;
            }
            else
            {
                columnAlpha = columnAlpha/1.1;
            } 
        }
        void Draw(int OFFX = 0, int OFFY = 0)
        {
            DrawRectangle(this->xto-90+OFFX,-32,180,992,(Color){255,255,255,columnAlpha});
            DrawCircle(this->x+OFFX,this->y+OFFY,32,DARKBLUE); 
        }
        
        // functions that get values
        int X() {return this->x; }
        int Y() {return this->y; }
};

class AsteroidManager
{
    // stores the positions of all the asteroids (10 max asteroids)
    int column [5];
    int y [5];
    
    public:
        AsteroidManager()
        {
            for(int i=0;i<5;i++)
            {
                int random = GetRandomValue(0,2);
                this->column[i] = GetRandomValue(0,2);
                this->y[i] = i*192;
            }
        }
        void Update()
        {
            for(int i=0;i<5;i++)
            {
                if (this->column[i] != -1)
                {
                    this->y[i]+=8;
                    if (this->y[i]>960)
                    {
                        this->y[i] = -32;
                        this->column[i] = GetRandomValue(0,2);
                    }
                }
            }
        }
        void Draw(int OFFX = 0, int OFFY = 0)
        {
            for(int i=0;i<10;i++)
            {
                DrawCircle((270+((this->column[i])-1)*180)+OFFX,this->y[i]+OFFY,24,BROWN);
            }
        }
};

// MAIN
int main(void)
{
    // Init window
    InitWindow(540, 960, "");
    SetTargetFPS(60);
    
    // Init the ship
    PlayerShip playerShip = PlayerShip(1);
    
    // Init the asteroid manager
    AsteroidManager asteroidManager = AsteroidManager();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        asteroidManager.Update();
        playerShip.Update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        asteroidManager.Draw(0,0);
        playerShip.Draw(0,0);
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}