#include "raylib.h"
#include "math.h"
#include "silentradiancesimple.h"

// SILENTRADIANCE
#define SILENTRADIANCE_OFF
#ifdef SILENTRADIANCE_ON
    /* Extern C tells the compiler this is C code instead of C++ code
    There are minor differences on how parameters are handled
    and function calls are done.
    */
    extern "C" {
        #include "silentradiancesimple.h"
        #include <math.h>   
        #include <pthread.h>

        #ifndef logit
            #define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
        #endif

        #define PACKETMAXSIZE 4000

        static unsigned char packetbuffer[PACKETMAXSIZE];
        static int recvStringLen;

        void *DoSound(void *arg) {
            init_receiver_sound(1);
            while (1) {
                playOneSoundBuffer();
            }
            return NULL;
        }

        void *DoSilentRadiance(void *arg) {
            while (1) {
                if(we_are_streaming_web==-1) {
                    
                } else {
                    if (get_packet_from_web_stream(&recvStringLen,packetbuffer)) {
                        process_packet(recvStringLen,packetbuffer);
                        while (get_packet_from_web_stream(&recvStringLen,packetbuffer)) {
                            process_packet(recvStringLen,packetbuffer);
                        }    
                    }
                }
            }    
        }

        void InitSilentRadiance(char *url) {
            init_web_stream(url);
            init_processor();
            {
                pthread_t tid[1];
                int err;
                err = pthread_create(tid,NULL,&DoSilentRadiance,NULL);
                if (err != 0) {logit("\ncan't create thread :[%s]", strerror(err))};
            }
            {
                pthread_t tid[1];
                int err;
                err = pthread_create(tid,NULL,&DoSound,NULL);
                if (err != 0) {logit("\ncan't create thread :[%s]", strerror(err))};
            }
        }

    } /* extern C */
#endif //SILENTRADIANCE_ON
// END SILENT RADIANCE


// HELPER FUNCTIONS
struct Collision
{
    bool CircleCircle(int x1, int y1, int r1, int x2, int y2, int r2)
    {
        int c1c2 = sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
        if ((c1c2) <= r1 + r2) {
            return true;
        } else {
            return false;
        }
    }
} collision;

// MAIN GAME CLASS
class GameManager
{
    public:
        // Screen Bounds
        int GAMEWIDTH = 540;
        int GAMEHEIGHT = 960;
        
        // Game State
        int gameState = 0;
            // 0 -> Gameplay
        
        class GameplayScene
        {
            public:
                // timer for spawning entities
                int spawnTimerMax = 60;
                int spawnTimer = spawnTimerMax;
                
                // playerShip
                class PlayerShip
                {
                    public:
                        // stores position of player
                        int x = 0;
                        int y = 896;
                        
                        // stores where the player should lerp to
                        int xto = 0;
                        int yto = 0;
                        
                        // stores the column the player is in
                        int column = 1;
                        
                        // handles the column "flash effect"
                        int columnAlpha = 0;
                    
                        // UPDATE
                        void Update()
                        {
                            // managing the updating the column
                            int inp = 0;
                            if (IsKeyPressed(KEY_RIGHT)) inp++;
                            if (IsKeyPressed(KEY_LEFT))  inp--;
                            this->column+=inp;
                            if (this->column > 2) {this->column = 2; inp = 0; }
                            if (this->column < 0) {this->column = 0; inp = 0; }
                            
                            // setting the xto
                            this->xto = (270+((this->column)-1)*180);
                            
                            // lerping to the xto
                            this->x = (this->xto+this->x)/2;
                            
                            // managing column alpha
                            if (inp != 0) {
                                columnAlpha = 255;
                            } else {
                                columnAlpha = columnAlpha/1.1;
                            }
                        }
                        // DRAW
                        void Draw()
                        {
                            DrawRectangle(this->xto-90,-32,180,992,(Color){255,255,255,this->columnAlpha});
                            DrawCircle(this->x,this->y,32,DARKBLUE);
                        }
                        // RESET
                        void Reset()
                        {
                            this->column = 1;
                            this->xto = (270+((this->column)-1)*180);
                            this->x = this->xto;
                        }
                } playerShip;
                
                // AsteroidHandler
                class AsteroidHandler
                {
                    public:
                        // Asteroid (18 max)
                        class Asteroid
                        {
                            public:
                                // stores position of an individual asteroid
                                int x;
                                int y;
                                
                                // UPDATE
                                void Update()
                                {
                                    this->y+=8;
                                }
                                // DRAW
                                void Draw()
                                {
                                    DrawCircle(this->x,this->y,64,BROWN);
                                }
                        } asteroid [20];
                        
                        // stores the current number of asteroids
                        int count = 0;
                        
                        // CREATE
                        void Create(int X, int Y)
                        {
                            this->asteroid[count].x = X;
                            this->asteroid[count].y = Y;
                            this->count++;
                        }
                        // DESTROY
                        void Destroy(int I)
                        {
                            for(int i=I;i<20;i++)
                            {
                                this->asteroid[i] = this->asteroid[i+1];
                            }
                            this->count--;
                        }
                        // RESET
                        void Reset()
                        {
                            for(int i=0;i<count;i++)
                            {
                                this->asteroid[i].x = 0;
                                this->asteroid[i].y = 0;
                            }
                            this->count = 0;
                        }
                        
                        // UPDATE
                        void Update()
                        {
                            for(int i=0;i<this->count;i++)
                            {
                                this->asteroid[i].Update();
                            }
                        }
                        // DRAW
                        void Draw()
                        {
                            for(int i=0;i<this->count;i++)
                            {
                                this->asteroid[i].Draw();
                            }
                        }
                } asteroidHandler;
                
                // SCENE RESET
                void Reset()
                {
                    this->playerShip.Reset();
                    this->asteroidHandler.Reset();
                }
                
                // SCENE INIT
                GameplayScene()
                {
                    this->asteroidHandler.Create(50,0);
                    this->asteroidHandler.Create(30,0);
                }
                // SCENE UPDATE
                void Update()
                {
                    // Updating
                    this->playerShip.Update();
                    this->asteroidHandler.Update();
                    
                    // Spawning new asteroids
                    if (spawnTimer > 0)
                    {
                        spawnTimer--;
                    }
                    else
                    {
                        if (GetRandomValue(0,1) == 0) {
                            asteroidHandler.Create(180,-32);
                        }
                        if (GetRandomValue(0,1) == 0) {
                            asteroidHandler.Create(270,-32);
                        }
                        if (GetRandomValue(0,1) == 0) {
                            asteroidHandler.Create(360,-32);
                        }
                        spawnTimer = spawnTimerMax;
                    }
                    
                    // Checking for collisions
                    //for(int i=0;i<asteroidHandler.count;i++)
                    //{
                    //    if (collision.CircleCircle(playerShip.x,playerShip.y,32,asteroidHandler.asteroid[i].x,asteroidHandler.asteroid[i].y,16))
                    //    {
                    //        Reset();
                    //    }
                    //}
                }
                // SCENE DRAW
                void Draw()
                {
                    this->playerShip.Draw();
                    this->asteroidHandler.Draw();
                }
        } gameplayScene;
        
        // GAME UPDATE
        void Update()
        {
            switch (gameState)
            {
                case 0:
                    gameplayScene.Update();
                break;
            }
        }   
        // GAME DRAW
        void Draw()
        {
        switch (gameState)
        {
            case 0:
                gameplayScene.Draw();
            break;
        }
    }
} gameManager;

// MAIN
int main(void)
{
    // InitSilentRadiance
    #ifdef SILENTRADIANCE_ON
        InitSilentRadiance("https://pt.silentradiance.com/download.cgi");
    #endif
    
    // Init window
    InitWindow(gameManager.GAMEWIDTH, gameManager.GAMEHEIGHT, "");
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // UPDATE
        gameManager.Update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        gameManager.Draw();
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}