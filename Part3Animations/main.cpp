#include "raylib.h"

// global variables

// window settings
const unsigned int wHeight = 350;
const unsigned int wWidth = 400;

class Sprite2D
{
private:
    Texture2D texture;
    int current_frame = 0;
    int frame_counter = 0;
    Rectangle rect;
    int frames = 1;

public:
    int anim_speed = 10;
    void setFrame(int frame){current_frame = frame;}
    bool animated = true;

    Vector2 position = {0,0};
    Vector2 getSize(){return Vector2{(float)texture.width,(float)texture.height};}
    void MoveBy(float x, float y);

    void Init(const char* texture_name,int frames = 1);
    ~Sprite2D(){UnloadTexture(texture);}
    void Draw();
};
void Sprite2D::Init(const char* texture_name,int frames)
{
    
    texture = LoadTexture(texture_name);
    this->frames = frames;
    rect = {0.0f,0.0f,(float)texture.width/frames,(float)texture.height} ;
}
void Sprite2D::MoveBy(float x, float y)
{
    if (x != 0 ){position.x += x;}
    if (y != 0){position.y += y;}
}
void Sprite2D::Draw()
{
    frame_counter++;

    if (frame_counter >= (60/anim_speed))
    {
        frame_counter = 0;
        if (animated){current_frame++;}
        

        if (current_frame > frames-1) current_frame = 0;

        rect.x = (float)current_frame*(float)texture.width/frames;
    }
    DrawTextureRec(texture, rect, position, WHITE);
}
int main() {
    // Initialize the window
    InitWindow(wWidth,wHeight,"Epic Game");

    // Set the maximum FPS for our game
    SetTargetFPS(60);

    // Create a game object 
    Sprite2D bane;
    bane.Init("assets/bane.png");
    Vector2 bane_size = bane.getSize();
    bane.position = Vector2{wWidth/2 - bane_size.x/2,wHeight/2 -bane_size.y/2}; // set the position to center of the screen

    float speed = 5;

    Sprite2D fire;
    fire.Init("assets/fire32f.png",8);
    fire.position = bane.position;

    // The main game loop
    while (!WindowShouldClose()) // check if the window is closed 
    {
        BeginDrawing();
        // this function clears the screen to render new thing :D

        // Move the player (Bane)
        if(IsKeyDown(KEY_RIGHT)) bane.MoveBy(speed,0);
        if(IsKeyDown(KEY_LEFT)) bane.MoveBy(-speed,0);
        if(IsKeyDown(KEY_DOWN)) bane.MoveBy(0,speed);
        if(IsKeyDown(KEY_UP)) bane.MoveBy(0,-speed);

        // check if the player hit the screen border
        if (bane.position.x + (int)bane_size.x >= wWidth) bane.position.x = wWidth - (int)bane_size.x; // check the right side
        else if (bane.position.x < 0) bane.position.x = 0; // left
        if (bane.position.y < 0) bane.position.y = 0; // top
        else if (bane.position.y + (int)bane_size.y >= wHeight) bane.position.y = wHeight - (int)bane_size.y; // bottom

        ClearBackground(RAYWHITE); // a better color for now 

        // draw the BANE 
        fire.Draw();
        bane.Draw();
        

        // Other 

        EndDrawing();
    }

    // Close the window
    CloseWindow();

    return 0;
}