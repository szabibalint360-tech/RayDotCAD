#pragma once
#include "../Shape.h"

// RECTANGLE SHAPE
class RectangleShape : public Shape {
private:
    Vector2 createStartPos = { 0.0f, 0.0f };
    bool isCreating = false;

public:
    RectangleShape(Camera2D* camera);

    void activeInput() override;
    // Uses base class draw(), calculateArea(), and calculatePerimeter() automatically!
};

// OVAL / CIRCLE SHAPE
class CircleShape : public Shape {
private:
    Vector2 centerPoint = { 0.0f, 0.0f };
    float radiusX = 0.0f;
    float radiusY = 0.0f;
    bool isCreating = false;

public:
    CircleShape(Camera2D* camera);

    void activeInput() override;

    void draw() override;

    bool containsPoint(Vector2 p) const override;
    // Overridden math because circles don't use polygon vertices
    float calculatePerimeter() const;
    float calculateArea() const;
};