#include "DerivedShapes.h"


// OVAL / CIRCLE SHAPE IMPLEMENTATION
CircleShape::CircleShape(Camera2D* camera) : Shape(camera) {
    name = "Oval/Circle";
}

void CircleShape::activeInput() {
    // 1. START DRAWING (Center Anchor)
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!CheckCollisionPointRec(GetMousePosition(), inputEditorRec)) return;

        isCreating = true;
        centerPoint = GetMouseWorldPos();

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            centerPoint = GetSnappedPosition(centerPoint);
        }

        // Use points[0] to track center position for the base UI system
        points.assign(1, Point(centerPoint));
    }

    // 2. DRAGGING & RESIZING
    if (isCreating && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 currentMouse = GetMouseWorldPos();

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            currentMouse = GetSnappedPosition(currentMouse);
        }

        radiusX = fabsf(currentMouse.x - centerPoint.x);
        radiusY = fabsf(currentMouse.y - centerPoint.y);

        // 30-Year-Old Tech: Hold SHIFT to force a perfect Circle!
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            float maxRadius = fmaxf(radiusX, radiusY);
            radiusX = maxRadius;
            radiusY = maxRadius;
        }

        points[0].setPos(centerPoint); // Keep base tracker updated
    }

    // 3. FINALIZE
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isCreating = false;
    }
}

void CircleShape::draw() {
    if (radiusX <= 0.0f || radiusY <= 0.0f) return;

    // Draw the filled interior
    DrawEllipse((int)centerPoint.x, (int)centerPoint.y, radiusX, radiusY, getCurrentColor());

    // Draw the clean outer line thickness
    // Note: Raylib doesn't support thickness on ellipses out of the box, 
    // so we step through a simple 360-degree loop to render a clean line thickness.
    int segments = 60;
    for (int i = 0; i < segments; i++) {
        float angle1 = ((float)i / segments) * 2.0f * PI;
        float angle2 = (((float)i + 1) / segments) * 2.0f * PI;

        Vector2 p1 = { centerPoint.x + cosf(angle1) * radiusX, centerPoint.y + sinf(angle1) * radiusY };
        Vector2 p2 = { centerPoint.x + cosf(angle2) * radiusX, centerPoint.y + sinf(angle2) * radiusY };

        DrawLineEx(p1, p2, lineThickness, lineColor);
    }

    // Draw the center anchor point so the user can interact/see it
    if (!points.empty()) points[0].draw();
}
bool CircleShape::containsPoint(Vector2 p) const {//using oval formula

    if (radiusX <= 0.0f || radiusY <= 0.0f) return false;

    float dx = p.x - centerPoint.x;
    float dy = p.y - centerPoint.y;

    float normalizedDistance = (dx * dx) / (radiusX * radiusX) + (dy * dy) / (radiusY * radiusY);

    return normalizedDistance <= 1.0f;
}

float CircleShape::calculateArea() const {
    // Ramanujan / Standard Area of an Ellipse: PI * A * B
    return PI * radiusX * radiusY;
}

float CircleShape::calculatePerimeter() const {
    // Infinite series approximation for Ellipse perimeter (Ramanujan first approximation)
    float a = radiusX;
    float b = radiusY;
    if (a == b) return 2.0f * PI * a; // Perfect circle exception

    return PI * (3.0f * (a + b) - sqrtf((3.0f * a + b) * (a + 3.0f * b)));
}