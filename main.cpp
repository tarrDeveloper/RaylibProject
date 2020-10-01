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
                        // stores the positions of all the asteroids (5 max asteroids)
                        int x [5];
                        int y [5];
                        
                        // stores the current number of asteroids
                        int count = 0;
                    
                        // UPDATE
                        void Update()
                        {
                            for(int i=0;i<this->count;i++)
                            {
                                this->y[i]+=8;
                                if (this->y[i]>992)
                                {
                                    this->x[i] = 270+((GetRandomValue(0,2))-1)*180;
                                    this->y[i] = -32;   
                                }
                            }                        
                        }
                        // DRAW
                        void Draw()
                        {
                            for(int i=0;i<this->count;i++)
                            {
                                DrawCircle(this->x[i],this->y[i],24,BROWN);
                            }
                        }
                        // CREATE
                        void Create(int X, int Y)
                        {
                            this->count++;
                            this->x[this->count-1] = X;
                            this->y[this->count-1] = Y;
                        }
                        // DESTROY
                        void Destroy(int I)
                        {
                            for(int i=I-1;i<4;i++)
                            {
                                this->x[i] = this->x[i+1];
                                this->y[i] = this->y[i+1];
                                this->count--;
                            }
                        }
                        // RESET
                        void Reset()
                        {
                            for(int i=0;i<5;i++)
                            {
                                this->x[i] = 270+((GetRandomValue(0,2))-1)*180;
                                this->y[i] = i*192;
                            }
                        }
                } asteroidHandler;
                
                // SpawnHandler -> Depends on Asteroid and PowerUpHandler
                class SpawnHandler
                {
                    
                } spawnHandler;
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
                    
                    // Checking for collisions
                    for(int i=0;i<asteroidHandler.count;i++)
                    {
                        if (collision.CircleCircle(playerShip.x,playerShip.y,32,asteroidHandler.x[i],asteroidHandler.y[i],16))
                        {
                            Reset();
                        }
                    }
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