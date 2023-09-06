#include "raylib.h"
#include "vector"
#include "string"

// global variables

// window settings
const unsigned int wHeight = 400;
const unsigned int wWidth = 550;

// Don't forget to like and subscribe to my channel :D
// https://www.youtube.com/@sudoAliyt
// My girlfriend :( There is no such a thing 
// Girls are not real
// My github : https://github.com/Haj4li



// Sprite class

class Sprite2D
{
private:
    Texture2D texture;
    int current_frame = 0;
    int frame_counter = 0;
    Rectangle rect;
    int frames = 1;
    
    int lived_for = 0;

public:
    // for the flip :/
    bool flipped = false;

    // -1 = immortal
    int life_span = -1;
    bool is_dead = false;

    int anim_speed = 10;
    void setFrame(int frame){current_frame = frame;}
    bool animated = true;

    Rectangle get_rect() {
        return Rectangle{position.x,position.y,rect.width,rect.height};
    }

    Vector2 position = {0,0};
    Vector2 getSize(){return Vector2{(float)texture.width,(float)texture.height};}
    void MoveBy(float x, float y);

    void Init(const char* texture_name,int frames = 1);
    ~Sprite2D(){UnloadTexture(texture);}
    void Draw();
};



// Game variables
#define FPS 60
Sprite2D bane;
Sprite2D badman;

Vector2 bane_size;

float control = 0;

float bane_power = 0;
float max_power = 100;

bool game_over = false;

// 60 seconds
float timer = FPS * 60;

void Restart();



int main() {
    // Initialize the window
    InitWindow(wWidth,wHeight,"Epic Game");

    // Set the maximum FPS for our game
    SetTargetFPS(FPS);

    // load the background 
    Texture2D map = LoadTexture("assets/map.png");

    // Create a game object 

    bane.Init("assets/bane.png");

    badman.Init("assets/bad.png",2);
    badman.anim_speed = 4;

    

    float speed = 5;
    
    float badman_speed = 2;
    

    // initialize fire list
    std::vector<Sprite2D*> fires;

    // Start or Restart game
    Restart();

    // The main game loop
    while (!WindowShouldClose()) // check if the window is closed 
    {
        BeginDrawing();
        // this function clears the screen to render new thing :D

        // timer 
        if (timer > 0 && !game_over)
        {
            timer--;
        }
        else
        {
            game_over = true;
        }
        
        // Stop the game if game_over is true
        // Or keep running when it's false
        if (!game_over)
        {
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

            // check dead sprites (fires)
            for (auto it = fires.begin(); it != fires.end(); ) {
                if ((*it)->is_dead) {
                    delete *it;
                    it = fires.erase(it);
                } else {
                    ++it;
                }
            }


            // spawn fire when space is hit and also check if bane has enough power

            if (IsKeyPressed(KEY_SPACE) && bane_power >= 20)
            {
                Sprite2D* tmp = new Sprite2D();
                tmp->Init("assets/fire32f2.png",8);
                tmp->position = bane.position;
                tmp->life_span = 4;
                fires.push_back(tmp);
                bane_power -= 20;
            }

            // Check Badman & Bane Collision

            if (CheckCollisionRecs(badman.get_rect(),bane.get_rect()))
            {
                bane_power -= 10; // bane lose control and power
                control -= 1;
            }
            else{
                // Badman Follow Bane

                if (badman.position.x < bane.position.x)
                    badman.MoveBy(badman_speed,0);
                else if (badman.position.x > bane.position.x)
                    badman.MoveBy(-badman_speed,0);
                
                if (badman.position.y < bane.position.y)
                    badman.MoveBy(0,badman_speed);
                else if (badman.position.y > bane.position.y)
                    badman.MoveBy(0,-badman_speed);
            }
            
            if (bane_power < 0)
            {
                bane_power = 0;
            }
            else if (bane_power < max_power)
            {
                // add 0.15 to power every frame
                bane_power += 0.15;
            }

            // check if the player hit the screen border
            if (bane.position.x + (int)bane_size.x >= wWidth) bane.position.x = wWidth - (int)bane_size.x; // check the right side
            else if (bane.position.x < 0) bane.position.x = 0; // left
            if (bane.position.y < 0) bane.position.y = 0; // top
            else if (bane.position.y + (int)bane_size.y >= wHeight) bane.position.y = wHeight - (int)bane_size.y; // bottom
        }
        else
        {
            if (IsKeyPressed(KEY_R)) // Restart the game
            {
                Restart();
            }
        }
        

        ClearBackground(RAYWHITE); // a better color for now 

        // draw the map 
        DrawTexture(map,0,0,WHITE);

        // draw fires
        for (const auto& fire: fires)
        {
            if (!fire->is_dead)
            {
                // more fire more control
                
                control += 0.05;

                fire->Draw();
            }
        }
        // loss control if there is no fire on city
        if (fires.size() == 0 && !game_over)
        {
            control -= 0.03;
        }
        if (control < 0)
        {
            control = 0;
        }

        // chec control
        if (control >= 100)
        {
            control = 100;
            game_over = true;
        }
        

        // draw the BANE 

        bane.Draw();

        // draw Badman

        badman.Draw();
        
        // draw UI

        // draw control points
        std::string sctext = "Control: " + std::to_string((int)control) + " %";
        DrawText(sctext.c_str(), 18, 30, 20, BLACK);

        // draw timer
        std::string timetext = "Time: " + std::to_string((int)(timer / FPS));
        DrawText(timetext.c_str(), 250, 30, 20, BLACK);
        

        // draw a help message
        DrawText("move with WASD, SPACE to start fire\nTake Control Of Your City!", 18, wHeight-45, 18, RED);

        // show game over message

        if (game_over)
        {
            DrawRectangle(wWidth/5-5,wHeight/2-5,450,100, BLACK);
            if (control >= 100) // Win 
            {
                DrawText("You won the game!\nPress R to Restart.",wWidth/5,wHeight/2,28,RED);
            }
            else{ // Lose
                DrawText("You lost control over the city!\nPress R to Restart.",wWidth/5,wHeight/2,28,RED);
            }
        }

        // draw the power
        DrawRectangle(18,10,200,20, BLACK);

        float powerWidth = bane_power / max_power * 196;

        if (bane_power > 20)
        {
            DrawRectangle(20,12,powerWidth,16, GREEN);
        }
        else{
            DrawRectangle(20,12,powerWidth,16, RED);
        }

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

void Restart(){
    badman.position = Vector2{wWidth/2,50};

    bane_size = bane.getSize();
    bane.position = Vector2{wWidth/2 - bane_size.x/2,wHeight/2 -bane_size.y/2}; // set the position to center of the screen

    // Reset the variables
    control = 0;
    bane_power = 0;
    max_power = 100;

    timer = FPS * 60;

    game_over = false;

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
    // check the life
    if (life_span > 0)
    {
        lived_for++;
        if ((life_span*FPS) <= lived_for)
        {
            is_dead = true;
        }
    }
    if (frame_counter >= (FPS/anim_speed))
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
