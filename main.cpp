#include "raylib.h"

// global variables

// window settings
const unsigned int wHeight = 350;
const unsigned int wWidth = 400;

int main() {
    // Initialize the window
    InitWindow(wWidth,wHeight,"Epic Game");

    // Set the maximum FPS for our game
    SetTargetFPS(60);

    // load the Bane texture
    Texture2D bane = LoadTexture("assets/bane.png");

    // Bane 
    // _position starts in the middle of screen
    float x_pos = wWidth/2 - bane.width/2;
    float y_pos = wHeight/2 - bane.height/2;
    float speed = 5;

    // The main game loop
    while (!WindowShouldClose()) // check if the window is closed 
    {
        BeginDrawing();
        // this function clears the screen to render new thing :D


        // Move the player (Bane)
        if(IsKeyDown(KEY_RIGHT)) x_pos+= speed;
        if(IsKeyDown(KEY_LEFT)) x_pos-= speed;
        if(IsKeyDown(KEY_DOWN)) y_pos+= speed;
        if(IsKeyDown(KEY_UP)) y_pos-= speed;

        // check if the player hit the screen border
        if (x_pos + bane.width >= wWidth) x_pos = wWidth - bane.width; // check the right side
        else if (x_pos < 0) x_pos = 0; // left
        if (y_pos < 0) y_pos = 0; // top
        else if (y_pos + bane.height >= wHeight) y_pos = wHeight - bane.height; // bottom

        ClearBackground(RAYWHITE); // a better color for now 

        // draw the BANE 
        DrawTexture(bane,x_pos,y_pos,WHITE);


        // Other 

        EndDrawing();
    }

    // Close the window
    CloseWindow();

    return 0;
}