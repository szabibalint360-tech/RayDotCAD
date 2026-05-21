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
        ClearBackground(RAYWHITE);
        Main->draw();
        EndDrawing();
    }

    ENGINE::shutdown();
    CloseWindow();
    return 0;
}