#include "raylib.h"
#include "vector"

// global variables

// window settings
const unsigned int wHeight = 400;
const unsigned int wWidth = 550;

class Sprite2D
{
private:
    Texture2D texture;
    int current_frame = 0;
    int frame_counter = 0;
    Rectangle rect;
    int frames = 1;

public:
    // for the flip :/
    bool flipped = false;

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

int main() {
    // Initialize the window
    InitWindow(wWidth,wHeight,"Epic Game");

    // Set the maximum FPS for our game
    SetTargetFPS(60);

    // load the background 
    Texture2D map = LoadTexture("assets/map.png");

    // Create a game object 
    Sprite2D bane;
    bane.Init("assets/bane.png");
    Vector2 bane_size = bane.getSize();
    bane.position = Vector2{wWidth/2 - bane_size.x/2,wHeight/2 -bane_size.y/2}; // set the position to center of the screen

    float speed = 5;
    

    // initialize fire list
    std::vector<Sprite2D*> fires;


    // The main game loop
    while (!WindowShouldClose()) // check if the window is closed 
    {
        BeginDrawing();
        // this function clears the screen to render new thing :D

        // Move the player (Bane)
        if(IsKeyDown(KEY_D))
        {
            bane.MoveBy(speed,0);
            bane.flipped = false;
        }
        if(IsKeyDown(KEY_A))
        {
            bane.MoveBy(-speed,0);
            bane.flipped = true;
        }
        if(IsKeyDown(KEY_S)) bane.MoveBy(0,speed);
        if(IsKeyDown(KEY_W)) bane.MoveBy(0,-speed);

        // spawn fire when space is hit
        if (IsKeyPressed(KEY_SPACE))
        {
            Sprite2D* tmp = new Sprite2D();
            tmp->Init("assets/fire32f2.png",8);
            tmp->position = bane.position;
            fires.push_back(tmp);
        }


        // check if the player hit the screen border
        if (bane.position.x + (int)bane_size.x >= wWidth) bane.position.x = wWidth - (int)bane_size.x; // check the right side
        else if (bane.position.x < 0) bane.position.x = 0; // left
        if (bane.position.y < 0) bane.position.y = 0; // top
        else if (bane.position.y + (int)bane_size.y >= wHeight) bane.position.y = wHeight - (int)bane_size.y; // bottom

        ClearBackground(RAYWHITE); // a better color for now 

        // draw the map 
        DrawTexture(map,0,0,WHITE);

        // draw fires
        for (const auto& fire: fires)
        {
            fire->Draw();
        }
        

        // draw the BANE 

        bane.Draw();
        

        // Other 

        EndDrawing();
    }

    // delete the fires :/
    for (const auto& fire: fires)
    {
        delete fire;
    }

    // Close the window
    CloseWindow();

    return 0;
}

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
    if (flipped){
        Rectangle flippedRec = { rect.x + rect.width, rect.y, -rect.width, rect.height };
        Vector2 flippedPosition = { position.x, position.y };
        DrawTextureRec(texture, flippedRec, flippedPosition, WHITE);
    }
    else
    {
        DrawTextureRec(texture, rect, position, WHITE);
    }
}
