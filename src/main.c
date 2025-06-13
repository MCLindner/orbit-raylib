#include "raylib.h"

#include "math.h"
#include "stdio.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define G 100

float gravForce(float m1, float m2, float r) 
{
		return (G * m1 * m2)/(r * r);
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
	Rectangle player = {(float)screenWidth/4,(float)screenHeight/4, 40,40};
	Rectangle planet = {(float)screenWidth/2,(float)screenHeight/2, 40,40};

	float radius = 0;
	float accel = 0;
	float m1 = 10;
	float m2 = 100000;

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
		float dx = planet.x - player.x;
		float dy = planet.y - player.y;
		radius = sqrt(dx * dx + dy * dy);
		Vector2 forceVector = { (planet.x - player.x)/radius, (planet.y - player.y)/radius};

		float deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_RIGHT)) velocity.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) velocity.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) velocity.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) velocity.y += 2.0f;
		
		// grav accel
		if (radius >= 100) 
		{
			accel = gravForce(m1, m2, radius) / m1;
			velocity.x += forceVector.x * accel * deltaTime;
			velocity.y += forceVector.y * accel * deltaTime;
		}
		
		player.x += velocity.x * deltaTime;
		player.y += velocity.y * deltaTime;
		
		// Debug prints
		// printf("radius");
		// printf("%f \n", radius);

		// printf("accel");
		// printf("%f \n", accel);

		// printf("vel");
		// printf("%f \n", velocity.x);
		// printf("%f \n", velocity.y);
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
