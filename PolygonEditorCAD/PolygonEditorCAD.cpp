#include "Program.h"

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED);
    InitWindow(windowWidth, windowHeight, "CAD");
    
    ENGINE::initialize();
    
    SetTargetFPS(60);
    
    Program* Main = new Program();

    while (!WindowShouldClose()) {
        Main->update(GetFrameTime());
        BeginDrawing();
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        ClearBackground(RAYWHITE);
        Main->draw();
        EndDrawing();
    }

    delete Main;

    ENGINE::shutdown();
    CloseWindow();
    return 0;
}