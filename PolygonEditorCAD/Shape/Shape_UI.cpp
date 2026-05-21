#include "Shape.h"



void Shape::drawTextLabels() {

    int fontSize = 16;

    for (const auto& pt : points) {
        Vector2 screenPos = GetWorldToScreen2D(pt.position, *viewingCamera);

        // ZERO dynamic heap allocations are happening here
        const char* coordText = TextFormat("(%d, %d)", (int)pt.position.x, (int)pt.position.y);

        float textX = screenPos.x + 12.0f;
        float textY = screenPos.y + 12.0f;

        //DrawUIText(coordText, (int)textX + 1, (int)textY + 1, fontSize, ColorAlpha(BLACK, 0.5f));
        DrawUIText(coordText, (int)textX, (int)textY, fontSize, DARKGRAY);
    }
}

void Shape::drawUIProperties() {
    if (ENGINE::SHOW_POINT_COORDINATES) {
        drawTextLabels();
    }

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    float sidebarWidth = windowWidth * 0.25f;
    float sidebarX = windowWidth - sidebarWidth;

    DrawRectangle((int)sidebarX, 0, (int)sidebarWidth, windowHeight, Fade(DARKGRAY, 0.9f));
    DrawLineEx({ sidebarX, 0 }, { sidebarX, (float)windowHeight }, 2.0f, MAROON);

    float paddingX = sidebarX + 25.0f;
    float currentY = 30.0f;
    int titleFontSize = 24;
    int bodyFontSize = 18;
    int tableFontSize = 14;

    // Custom DrawUIText replaced with standard Raylib DrawUIText for compatibility, adjust if you have a wrapper
    DrawUIText("SHAPE STATS [CAD]", (int)paddingX, (int)currentY, titleFontSize, RAYWHITE);
    currentY += 45.0f;

    bool isIntersecting = checkSelfIntersection();
    float totalPerimeter = calculatePerimeter();
    float totalArea = calculateArea();

    if (isIntersecting) {
        DrawRectangle((int)paddingX - 5, (int)currentY - 2, (int)sidebarWidth - 40, 30, Fade(RED, 0.3f));
        DrawUIText("STATUS: SELF-INTERSECTING ERROR", (int)paddingX, (int)currentY, bodyFontSize, RED);
    }
    else {
        DrawUIText("STATUS: GEOMETRY VALID", (int)paddingX, (int)currentY, bodyFontSize, LIME);
    }
    currentY += 40.0f;

    char buffer[128];

    snprintf(buffer, sizeof(buffer), "Perimeter: %.2f px", totalPerimeter);
    DrawUIText(buffer, (int)paddingX, (int)currentY, bodyFontSize, LIGHTGRAY);
    currentY += 30.0f;

    if (isIntersecting) {
        snprintf(buffer, sizeof(buffer), "Area: -- (Invalid Topology)");
        DrawUIText(buffer, (int)paddingX, (int)currentY, bodyFontSize, ORANGE);
    }
    else {
        snprintf(buffer, sizeof(buffer), "Area: %.2f px²", totalArea);
        DrawUIText(buffer, (int)paddingX, (int)currentY, bodyFontSize, LIGHTGRAY);
    }
    currentY += 35.0f;

    snprintf(buffer, sizeof(buffer), "Total Vertices: %zu", points.size());
    DrawUIText(buffer, (int)paddingX, (int)currentY, bodyFontSize, RAYWHITE);
    currentY += 40.0f;

    DrawLineEx({ paddingX, currentY }, { (float)windowWidth - 25.0f, currentY }, 1.0f, GRAY);
    currentY += 20.0f;

    DrawUIText("Vertex Coordinates Matrix:", (int)paddingX, (int)currentY, bodyFontSize, GOLD);
    currentY += 30.0f;

    for (size_t i = 0; i < points.size(); i++) {
        if (currentY > windowHeight - 40) {
            DrawUIText("... [Table Overflow]", (int)paddingX, (int)currentY, tableFontSize, ORANGE);
            break;
        }

        Vector2 pos = points[i].getPos();
        snprintf(buffer, sizeof(buffer), "P%-2zu ->  X: %6.1f  |  Y: %6.1f", i, pos.x, pos.y);

        Color rowColor = (&points[i] == movingPoint) ? SKYBLUE : RAYWHITE;
        DrawUIText(buffer, (int)paddingX + 10, (int)currentY, tableFontSize, rowColor);

        currentY += 22.0f;
    }
}