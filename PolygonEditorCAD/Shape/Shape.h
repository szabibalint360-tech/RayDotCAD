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
    int movingPointIndex = -1;

    bool isActive = true;
    bool isHovered = false;
    bool isSelected = false; 

    float lastClickTime = 0.0f;
    const float DOUBLE_CLICK_TIME = 0.25f; // 250ms window

    function<void(Shape*)> onActivatedCallback = nullptr;//Shape event
public:    

    // Utilities
    void connect_activated(function<void(Shape*)> callback) {
        onActivatedCallback = callback;
    }
    void setActive(bool state) { isActive = state; if (state == false) isSelected = false; }
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
    Color getCurrentColor();
    bool isEmpty();
    // Geometric Math & Calculations
    virtual bool containsPoint(Vector2 p) const;
    Vector2 GetSnappedPosition(Vector2 rawPos) const;
    Vector2 GetMouseWorldPos() const;
    float calculatePerimeter() const;
    float calculateArea() const;

    // Intersection Math
    bool IsPointInTriangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c) const;
    bool DoLinesIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) const;

    // Validation
    bool isValidShape();
    bool checkSelfIntersection();

};