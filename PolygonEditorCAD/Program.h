#pragma once
#include "Core/Scene.h"
#include "Shape/Derived Shapes/DerivedShapes.h"

class Program : public Scene {
public:
    static constexpr int GRID_HALF_EXT = 100;
    Color GRID_COLOR = { 200, 200, 200, 80 };
    Color GRID_AXIS = { 180, 180, 160 };

    Shape* activeShape = nullptr;

    // UI
    CheckBox* checkBoxShowPoint = nullptr;
    Button* btnDeselect = nullptr;
    Button* btnPolygon = nullptr;
    Button* btnRectangle = nullptr;
    Button* btnCircle = nullptr;

    //setup
    Program();
    ~Program();
    void setupButtons();

    // Central shape switching — deactivates old, activates new
    void setActiveShape(Shape* next);
    void drawGrid();
    void draw() override;
    void drawUI() override;
};