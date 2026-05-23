#include "Shape.h"

Shape::Shape(Camera2D* camera) {
    if (camera != nullptr) {
        viewingCamera = camera;
    }
    // Initialize standard workspace boundaries
    inputEditorRec = { 0, 0, (float)GetScreenWidth() * 0.75f, (float)GetScreenHeight() };
}
void Shape::activeInput() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!CheckCollisionPointRec(GetMousePosition(), inputEditorRec)) return;

        Vector2 mouseWorldPos = GetMouseWorldPos();

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            mouseWorldPos = GetSnappedPosition(mouseWorldPos);
        }

        points.push_back(Point(mouseWorldPos));
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            movingPoint = &(points.back());
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        if (!points.empty()) {
            points.pop_back();
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        movingPoint = nullptr;
    }

    if (movingPoint != nullptr) {
        Vector2 mouseWorldPos = GetMouseWorldPos();

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            mouseWorldPos = GetSnappedPosition(mouseWorldPos);
        }

        movingPoint->setPos(mouseWorldPos);
    }
}
void Shape::passiveInput() {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *viewingCamera);
    
    isHovered = containsPoint(mouseWorldPos);

    if (isHovered) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            float currentTime = GetTime();

            if (currentTime - lastClickTime < DOUBLE_CLICK_TIME) {

                onActivatedCallback(this);
            }
            else {
                // SINGLE CLICK -> Visual Selection Highlight
                isSelected = !isSelected; // Toggle selection highlight
            }

            // Save the timestamp of this click for the next comparison
            lastClickTime = currentTime;
        }
    }
}
void Shape::update(double deltaTime) {
    if (isActive) {
        activeInput();
    }
    else {
        passiveInput();
    }
}

void Shape::draw() {
    fillPolygon();

    // DRAW EDGES & MEASUREMENTS
    for (size_t i = 0; i < points.size(); i++) {
        Vector2 p1 = points[i].getPos();
        Vector2 p2 = points[(i + 1) % points.size()].getPos();

        // Draw the solid line
        DrawLineEx(p1, p2, lineThickness, lineColor);

        if (ENGINE::SHOW_LINE_LENGTH && points.size() > 1) {
            float dist = Vector2Distance(p1, p2);

            // Only draw if
            if (dist > 0.1f) {
                // Calculate the exact center of the edge
                Vector2 midPoint = { (p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f };

                // Format the distance
                char lengthText[32];
                snprintf(lengthText, sizeof(lengthText), "%.1fmm", dist);

                // CAD Aesthetics: Offset the text slightly
                int fontSize = 10;
                int textWidth = MeasureText(lengthText, fontSize);

                // Draw a subtle background box so the text is readable over the grid
                DrawRectangle((int)midPoint.x + 4, (int)midPoint.y - 12, textWidth + 10, fontSize + 4, ColorAlpha(WHITE, 0.8f));

                // Draw the text
                DrawUIText(lengthText, (int)midPoint.x + 6, (int)midPoint.y - 10, fontSize, DARKBLUE);
            }
        }
    }
    // DRAW VERTEX POINTS 
    for (size_t i = 0; i < points.size(); i++) {
        points[i].draw();
    }
}
bool Shape::isEmpty() {
    return points.empty();
}
Vector2 Shape::GetSnappedPosition(Vector2 rawPos) const {
    float snappedX = roundf(rawPos.x / ENGINE::GRID_SPACING) * ENGINE::GRID_SPACING;
    float snappedY = roundf(rawPos.y / ENGINE::GRID_SPACING) * ENGINE::GRID_SPACING;
    return { snappedX, snappedY };
}

Vector2 Shape::GetMouseWorldPos() const {
    return GetScreenToWorld2D(GetMousePosition(), *viewingCamera);
}