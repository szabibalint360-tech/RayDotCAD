#include "DerivedShapes.h"
// ==========================================
// RECTANGLE SHAPE IMPLEMENTATION
// ==========================================
RectangleShape::RectangleShape(Camera2D* camera) : Shape(camera) {
    name = "Rectangle";
}

void RectangleShape::activeInput() {
    // 1. START DRAWING (Click Anchor)
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!CheckCollisionPointRec(GetMousePosition(), inputEditorRec)) return;

        isCreating = true;
        createStartPos = GetMouseWorldPos();

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            createStartPos = GetSnappedPosition(createStartPos);
        }

        // Initialize the 4 corners of the polygon
        points.assign(4, Point(createStartPos));
    }

    // 2. DRAGGING & RESIZING
    if (isCreating && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 currentMouse = GetMouseWorldPos();

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            currentMouse = GetSnappedPosition(currentMouse);
        }

        float width = currentMouse.x - createStartPos.x;
        float height = currentMouse.y - createStartPos.y;

        // 30-Year-Old Tech: Hold SHIFT to force a perfect Square!
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            float maxSide = fmaxf(fabsf(width), fabsf(height));
            width = (width >= 0.0f) ? maxSide : -maxSide;
            height = (height >= 0.0f) ? maxSide : -maxSide;
        }

        // Re-calculate the 4 bounding box corner vertices on the fly
        points[0].setPos(createStartPos);                                       // Top Left
        points[1].setPos({ createStartPos.x + width, createStartPos.y });          // Top Right
        points[2].setPos({ createStartPos.x + width, createStartPos.y + height }); // Bottom Right
        points[3].setPos({ createStartPos.x, createStartPos.y + height });         // Bottom Left
    }

    // 3. FINALIZE SHAPE
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isCreating = false;
    }
}
