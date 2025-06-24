#include "raylib.h"

#include "math.h"
#include "stdio.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define G 100

typedef struct gravObj {
	float x;
	float y;
	float vx;
	float vy;
	float radius;
	float mass;
} GRAVOBJ;

float gravForce(float m1, float m2, float r) 
{
	return (G * m1 * m2)/(r * r);
}

void updateMotion(GRAVOBJ* obj1, GRAVOBJ* obj2)
{
	float dx = obj2->x - obj1->x;
	float dy = obj2->y - obj1->y;
	float radius = sqrt(dx * dx + dy * dy);
	Vector2 forceVector = {0, 0};
	float accel = 0;

	float deltaTime = GetFrameTime();

	// grav accel
	if (radius >= 100.0f) 
	{
		forceVector.x = dx / radius;
		forceVector.y = dy / radius;
		accel = gravForce(obj1->mass, obj2->mass, radius) / obj1->mass;
		obj1->vx += forceVector.x * accel * deltaTime;
		obj1->vy += forceVector.y * accel * deltaTime;
	}
	
	// deltaPos = {velocity.x * deltaTime, velocity.y * deltaTime};
	obj1->x += obj1->vx * deltaTime;
	obj1->y += obj1->vy * deltaTime;
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing");

    float rotation = 0.0f;

	Vector2 velocity = {0,0};
	
	// TODO: change this to a struct typedef with mass included
	GRAVOBJ player = {(float)screenWidth/4,(float)screenHeight/4, 40, 10, 0, 0};
	GRAVOBJ planet = {(float)screenWidth/2,(float)screenHeight/2, 40, 100000, 0 , 0};

	float radius = 0;
	float accel = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
        // Update
        //----------------------------------------------------------------------------------
		rotation += 0.2f;

		updateMotion(&player, &planet);

		if (IsKeyDown(KEY_RIGHT)) player.vx += 2.0f;
        if (IsKeyDown(KEY_LEFT)) player.vx -= 2.0f;
        if (IsKeyDown(KEY_UP)) player.vy -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) player.vy += 2.0f;
		
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Traveler", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, player.x, player.y, WHITE);            

		// Polygon shapes and lines
		DrawPoly((Vector2){planet.x, planet.y}, 6, 80, rotation, BROWN);
		DrawPolyLines((Vector2){planet.x, planet.y}, 6, 90, rotation, BROWN);
		DrawPolyLinesEx((Vector2){planet.x, planet.y}, 6, 85, rotation, 6, BEIGE);
	
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
