#include "raylib.h"

extern "C" {
#include <math.h>   
}

#define SILENTRADIANCE_OFF


// SILENTRADIANCE
#ifdef SILENTRADIANCE_ON
    /* Extern C tells the compiler this is C code instead of C++ code
    There are minor differences on how parameters are handled
    and function calls are done.  this is usually set in the makefile
    */
    extern "C" {
        #include "silentradiancesimple.h"
        #include "simple_packet_summary.h"
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
float Lerp(float a, float b, float f)
{
    return a + (f*(b - a));
}


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


// TIME CLASS
class MyTime
{
    // stores the deltaTime
    float deltaTime = 0;
    
    public:
        // UPDATE
        void Update()
        {
            deltaTime = GetFrameTime()*GetFPS();
        }
        
       float DeltaTime() { return deltaTime; }
} myTime;

// BACKGROUND HANDLER
class Background
{
    int offX = 0;
    int offY = 0;
    
    public:
        void Update(int OFFX = 0)
        {
            offY+=1;
            if (offY >= 180)
            {
                offY -= 180;
            }
            offX = OFFX;
        }
        void Draw()
        {
            for(int i=-1;i<5;i++)
            {
                for(int j=-1;j<8;j++)
                {
                    //DrawCircle(i*32,j*32,8,BLACK);
                    DrawRectangle((i*180)+1+offX,(j*180)+offY,178,178,BLACK);
                    //DrawRectangle(xto-90,-32,180,992,(Color){255,255,255,columnAlpha});
                }
            }
        }
} background;

// PLAYER CLASS
class Player
{
    // stores position
    int x = 0;
    int y = 896;
        
    // stores lerp data
    int xstart = 0;
    int ystart = 896;
    int xto = 0;
    int yto = 896;
    float lerpStatus = 1;
        
    // stores which "column" the player is in (0, 1, 2)
    int column = 1;
    
    // stores the column alpha for the visual flash
    float columnAlpha = 0;
    
    public:
        // RESET
        void Reset()
        {
            x = 0;
            y = 896;
            xstart = 0;
            ystart = 896;
            xto = 0;
            yto = 896;
            lerpStatus = 1;
            column = 1;
        }
        // UPDATE
        void Update(float dt)
        {
            // Get input
            int inp = 0;
            if (IsKeyPressed(KEY_RIGHT)) inp++;
            if (IsKeyPressed(KEY_LEFT))  inp--;
            
            // Apply input to the column
            column+=inp;
            if (column > 2) {column = 2; inp = 0; }
            if (column < 0) {column = 0; inp = 0; }
            
            // Set the xto if there is new input
            if (inp != 0)
            {
                lerpStatus = 0;
                xstart = x;
                xto = (270+((this->column)-1)*180);
                ystart = y;
                yto = y;
            }
            
            // Moving the ship to the xto
            if (lerpStatus < 1)
            {
                lerpStatus += .07*dt;
            }
            if (lerpStatus > 1)
            {
                lerpStatus = 1;
            }
            float t = lerpStatus/1;
            t = sin(t * 3.14 * 0.5f);
            x = Lerp(xstart, xto, t);
            
            // AlphaLerp
           columnAlpha = Lerp(50,0,lerpStatus);
        }        
        // DRAW
        void Draw()
        {
            DrawRectangle(xto-90 - ((x-270)/8) ,-32,180,1024,(Color){255,255,255,columnAlpha});
            DrawCircle(x,y,32,RED);
        }
        
        // GETTERS
        int X() { return x; }
        int Y() { return y; }
} player;

// ASTEROIDS
class Asteroids
{
    // stores the x and y positions of all asteoids
    int x[20];
    int y[20];
    // stores the current number of asteroids
    int count = 0;
    
    // stores spawn data
    int spawnInterval = 60;
    int spawnTimer = spawnInterval;
    int spawnColumn = 0;
    
    public:
        // Creates and asteroid
        void Create(int xnew, int ynew)
        {
            x[count] = xnew;
            y[count] = ynew;
            count++;
        }
        // Destroys an asteroid
        void Destroy(int index)
        {
            for(int i=index;i<count;i++)
            {
                x[i] = x[i+1];
                y[i] = y[i+1];
            }
            count--;
        }
        // DestroyAll
        void DestroyAll()
        {
            count = 0;
        }
        
        // UPDATE (updates all asteroids)
        void Update(float dt)
        {
            // spawning asteroids
            if (spawnTimer > 0)
            {
                spawnTimer-=dt;
            }
            else
            {
                // switching the openening
                switch (spawnColumn)
                {
                    case 0:
                        spawnColumn = 1;
                        break;
                    case 1:
                        spawnColumn = 2*GetRandomValue(0,1);
                        break;
                    case 2:
                        spawnColumn = 1;
                        break;
                }
                // creating asteoids in all non-empty spaces
                if (spawnColumn != 0) {Create(90,-32); }
                if (spawnColumn != 1) {Create(270,-32); }
                if (spawnColumn != 2) {Create(450,-32); }
                // resetting spawntimer
                spawnTimer = spawnInterval;
            }
            
            // updating all of the asteroids
            for(int i=0;i<count;i++)
            {
                y[i] += 12*dt;
                
            }
            for(int i=0;i<count;i++)
            {
                if (y[i] > 992)
                {
                    Destroy(i);
                    i--;
                }
            }   
        }
        // DRAW (draws all asteroids)
        void Draw()
        {
            for(int i=0;i<count;i++)
            {
                DrawCircle(x[i],y[i],36,BROWN);
            }
        }
} asteroids;

// MAIN
int main(void)
{
    // InitSilentRadiance
    #ifdef SILENTRADIANCE_ON
        InitSilentRadiance("https://pt.silentradiance.com/download.cgi");
    #endif
    
    // Init window
    InitWindow(540, 960, "");
    SetTargetFPS(120);
    SetConfigFlags(FLAG_VSYNC_HINT);
    
    // Init camera
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ 540/2, 960/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    // Background
    Color thisBack =  BROWN;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Silent Radiance ---------------------
        #ifdef SILENTRADIANCE_ON
            compute_packet_summary();
            struct packet *p = NULL; // pointer to packet
            // find current packet
            if (packet_summary.now_frame != 1)
            {
                int index = (packet_summary.start_index + packet_summary.now_frame - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
                if (index<0) index += PACKET_SUMMARY_SIZE;
                p = packet_summary.packets+index;
                if (p->has_statistics == 0) p = NULL;
            }
            
            if (p)
            {
                if (p->has_beat)
                {
                    //thisBack = BLUE;
                }
                if (p->has_onset)
                {
                    //thisBack = BLUE;
                }
                // pitch is p->pitch
                // db is a rough db level - just 4 levels
                // p->folded flas is flags like beat, onset, but folded over a few packets to better match with the video frame rate
                //radianceData.beatScale = 1 + p->pitch/10000;
            }
            
            /*if ((packet_summary.commanded_background_color[0] != 0.f)  ||
                (packet_summary.commanded_background_color[1] != 0.f)  ||
                (packet_summary.commanded_background_color[2] != 0.f))
            { /* if the dj sets a color, use that as a background
      
                this_background = (Color){(unsigned char)(packet_summary.commanded_background_color[0]*255.f),
	                    (unsigned char)(packet_summary.commanded_background_color[1]*255.f),
                        (unsigned char)(packet_summary.commanded_background_color[2]*255.f),255};
            }
            else
            {
                this_background = RAYWHITE;
            }*/    		
        #endif
        // --------------------------------------
        
        // UPDATE
        myTime.Update();
        player.Update(myTime.DeltaTime());
        asteroids.Update(myTime.DeltaTime());
        
        // Camera and BK
        background.Update(-(player.X()-270)/8);
        camera.target = (Vector2){Lerp(player.X(),270,0.9),960/2};
        camera.zoom = 0.995 + .05*sin(1.5*GetTime());

        // Draw
        BeginDrawing();
        BeginMode2D(camera);
        //#ifdef SILENTRADIANCE_ON
            //ClearBackground(this_background);
        //#else
        //thisBack = (Color){GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255};
        ClearBackground(thisBack);
        background.Draw();
        //#endif
        player.Draw();
        asteroids.Draw();
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}


/* end of file main.cpp*/