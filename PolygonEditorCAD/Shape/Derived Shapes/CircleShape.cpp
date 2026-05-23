#include "DerivedShapes.h"


// OVAL / CIRCLE SHAPE IMPLEMENTATION

CircleShape::CircleShape(Camera2D* camera) : Shape(camera) {
    name = "Oval/Circle";
}

void CircleShape::activeInput() {
    // START DRAWING (Center Anchor)
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

    // DRAGGING & RESIZING
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

    // FINALIZE
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isCreating = false;
    }
}

void CircleShape::draw() {
    if (radiusX <= 0.0f || radiusY <= 0.0f) return;

    // Draw the filled interior
    DrawEllipse((int)centerPoint.x, (int)centerPoint.y, radiusX, radiusY, getCurrentColor());

    // Draw the clean outer line thickness
    int segments = 60;
    for (int i = 0; i < segments; i++) {
        float angle1 = ((float)i / segments) * 2.0f * PI;
        float angle2 = (((float)i + 1) / segments) * 2.0f * PI;

        Vector2 p1 = { centerPoint.x + cosf(angle1) * radiusX, centerPoint.y + sinf(angle1) * radiusY };
        Vector2 p2 = { centerPoint.x + cosf(angle2) * radiusX, centerPoint.y + sinf(angle2) * radiusY };

        DrawLineEx(p1, p2, lineThickness, lineColor);
    }

    // Draw Radius Measurements
    if (ENGINE::SHOW_LINE_LENGTH) {
        int fontSize = 10;

        // HORIZONTAL RADIUS 
        Vector2 rightEdge = { centerPoint.x + radiusX, centerPoint.y };
        Vector2 midX = { centerPoint.x + (radiusX / 2.0f), centerPoint.y };

        DrawLineEx(centerPoint, rightEdge, 1.0f, ColorAlpha(DARKGRAY, 0.5f));

        char textX[32];
        // Check if circle or an ellipse
        if (fabsf(radiusX - radiusY) < 0.1f) {
            snprintf(textX, sizeof(textX), "R: %.1f mm", radiusX);
        }
        else {
            snprintf(textX, sizeof(textX), "Rx: %.1f mm", radiusX);
        }

        int widthX = MeasureText(textX, fontSize);
        // Center the text perfectly on the guide line
        DrawRectangle((int)midX.x - (widthX / 2) - 2, (int)midX.y - 12, widthX + 4, fontSize + 4, ColorAlpha(WHITE, 0.8f));
        DrawText(textX, (int)midX.x - (widthX / 2), (int)midX.y - 10, fontSize, DARKBLUE);

        // VERTICAL RADIUS only drawn if Ellipse
        if (fabsf(radiusX - radiusY) >= 0.1f) {
            Vector2 bottomEdge = { centerPoint.x, centerPoint.y + radiusY };
            Vector2 midY = { centerPoint.x, centerPoint.y + (radiusY / 2.0f) };

            DrawLineEx(centerPoint, bottomEdge, 1.0f, ColorAlpha(DARKGRAY, 0.5f));

            char textY[32];
            snprintf(textY, sizeof(textY), "Ry: %.1f mm", radiusY);
            int widthY = MeasureText(textY, fontSize);

            // Offset text to the right of the vertical line
            DrawRectangle((int)midY.x + 4, (int)midY.y - (fontSize / 2) - 2, widthY + 4, fontSize + 4, ColorAlpha(WHITE, 0.8f));
            DrawText(textY, (int)midY.x + 6, (int)midY.y - (fontSize / 2), fontSize, DARKBLUE);
        }
    }

    // Draw the center anchor point
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
    return PI * radiusX * radiusY;
}

float CircleShape::calculatePerimeter() const {
    // Infinite series approximation for Ellipse perimeter (Ramanujan first approximation)
    float a = radiusX;
    float b = radiusY;
    if (a == b) return 2.0f * PI * a; // Perfect circle exception

    return PI * (3.0f * (a + b) - sqrtf((3.0f * a + b) * (a + 3.0f * b)));
}