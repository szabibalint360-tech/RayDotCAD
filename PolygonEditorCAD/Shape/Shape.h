#pragma once

#include "../Core/Node.h"

// Global constants
constexpr int POINT_SIZE = 10;

class Point {
public:
    Vector2 position;

    Point() : position({ 0.0f, 0.0f }) {}
    Point(float x, float y) : position({ x, y }) {}
    Point(Vector2 v) : position(v) {}

    void setPos(Vector2 v) { position = v; }
    Vector2 getPos() const { return position; }

    Rectangle getVisualRec() const {
        return { position.x, position.y, (float)POINT_SIZE, (float)POINT_SIZE };
    }

    void draw() const {
        Vector2 origin = { POINT_SIZE / 2.0f, POINT_SIZE / 2.0f };
        DrawRectanglePro(getVisualRec(), origin, 45.0f, GRAY);
    }
};

// Shape Class Blueprint
class Shape : public Node {
protected:
    // Properties
    float lineThickness = 3.0f;

    Color lineColor = PALETTE::MIDNIGHT_VIOLET;
    Color fillColor = PALETTE::CAD_CYAN;

    vector<Point> points;
    Point* movingPoint = nullptr;
    Camera2D* viewingCamera = nullptr;

    Rectangle inputEditorRec = { 0, 0, (float)windowWidth * 0.75f, (float)windowHeight};
public:    
    bool isActive = true;
    // Utilities
    
    // Core
    Shape(Camera2D* camera);
    virtual void activeInput();//editing resizing
    virtual void passiveInput();//check if selected
    void update(double deltaTime) override;
    void draw() override;
    // UI & Rendering Handlers
    void fillPolygon();
    void drawTextLabels();
    void drawUIProperties();
    bool isEmpty();
    // Geometric Math & Calculations
    Vector2 GetSnappedPosition(Vector2 rawPos) const;
    Vector2 GetMouseWorldPos() const;
    float calculatePerimeter() const;
    float calculateArea() const;

    // Intersection Math
    float cross2D(Vector2 a, Vector2 b, Vector2 c) const;
    bool IsPointInTriangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c) const;
    bool DoLinesIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) const;

    // Validation
    bool isValidShape();
    bool checkSelfIntersection();

};