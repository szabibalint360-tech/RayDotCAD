#pragma once
#include "Core/Scene.h"
#include "Shape/Derived Shapes/DerivedShapes.h"

class Program : public Scene {
private:

    static constexpr int GRID_HALF_EXT = 100;
    static constexpr int GRID_MAJOR_INTERVAL = 5;
    static constexpr float GRID_THICKNESS_AXIS = 3.0f;
    static constexpr float GRID_THICKNESS_MAJOR = 1.5f;
    static constexpr float GRID_THICKNESS_MINOR = 0.8f;

    Color GRID_COLOR = { 200, 200, 200,  80 };   // minor lines, low alpha
    Color GRID_COLOR_MAJOR = { 200, 200, 200, 178 };   // major lines
    Color GRID_AXIS = { 180, 180, 160, 255 };    // axis, fully opaque

    Shape* activeShape = nullptr;

    // UI
    CheckBox* checkBoxShowPoint = nullptr;
    Button* btnDeselect = nullptr;
    Button* btnDelete = nullptr;
    Button* btnPolygon = nullptr;
    Button* btnRectangle = nullptr;
    Button* btnCircle = nullptr;
public:
    //setup
    Program();
    ~Program();
    void setupButtons();
    void setupButtonEvents();
    // Central shape switching — deactivates old, activates new
    void setActiveShape(Shape* next);
    template<typename T>
    void createNewShape();
    void drawGrid();
    void draw() override;
    void drawUI() override;
};