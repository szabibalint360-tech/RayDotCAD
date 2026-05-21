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

    for (size_t i = 0; i < points.size(); i++) {
        DrawLineEx(points[i].getPos(),
            points[(i + 1) % points.size()].getPos(),
            lineThickness, lineColor);
    }
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