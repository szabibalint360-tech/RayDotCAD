#pragma once

#include "../Node.h"

constexpr int POINT_SIZE = 10;

class Point {
public:
    Vector2 position;
    Point() : position({ 0.0f ,0.0f }) {}

    Point(float x, float y) {
        position = { x,y };
    }
    Point(Vector2 v) {
        position = v;
    }
    void setPos(Vector2 v) {
        position = v;
    }
    Vector2 getPos() const {
        return position;
    }
    Rectangle getVisualRec() {
        return{ position.x,position.y,POINT_SIZE,POINT_SIZE };
    }
    void draw() {
        Vector2 origin = { POINT_SIZE / 2.0f, POINT_SIZE / 2.0f };
        DrawRectanglePro(getVisualRec(), origin, 45.0f, GRAY);
    }
};

class Shape : public Node {
public:
    static constexpr float GRID_SPACING = 50.0f;  
    bool isActive = true;
    float lineThickness = 3.0f;
    Color lineColor = RED;
    Color fillColor = ENGINE.CAD_CYAN;
    vector<Point> points;
    Point* movingPoint = nullptr;
    Camera2D* viewingCamera = nullptr;

    Rectangle inputEditorRec = { 0,0,windowWidth * 0.75,windowHeight };


    Shape(Camera2D* camera) {
        if (camera != nullptr);
        viewingCamera = camera;
    }

    Vector2 GetSnappedPosition(Vector2 rawPos) const {
        float snappedX = roundf(rawPos.x / GRID_SPACING) * GRID_SPACING;
        float snappedY = roundf(rawPos.y / GRID_SPACING) * GRID_SPACING;
        return { snappedX, snappedY };
    }
    void editModeInput() {

    }
    void staticModeInput() {

    }
    void update(double deltaTime) override {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), inputEditorRec) == false) return;//not inside editor environment

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
    Vector2 GetMouseWorldPos() {
        return GetScreenToWorld2D(GetMousePosition(), *viewingCamera);
    }
    // Returns the signed 2D cross product of vectors AB and BC.
    // In Raylib screen space (Y-down), a NEGATIVE value means the
    // vertex B is a convex (left-turning) ear candidate.
    float cross2D(Vector2 a, Vector2 b, Vector2 c) {
        return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
    }

    bool IsPointInTriangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c) {
        // Uses cross products — works regardless of winding because we
        // check all three edges return the same sign for interior points.
        float d1 = (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
        float d2 = (p.x - c.x) * (b.y - c.y) - (b.x - c.x) * (p.y - c.y);
        float d3 = (p.x - a.x) * (c.y - a.y) - (c.x - a.x) * (p.y - a.y);

        bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
        return !(has_neg && has_pos);
    }

    void fillPolygon() {
        if (points.size() < 3) return;

        vector<Vector2> vertices;
        vertices.reserve(points.size());
        for (const auto& pt : points) {
            vertices.push_back(pt.getPos());
        }

        // 1. Calculate the Signed Area to find the true winding direction
        float signedArea = 0.0f;
        for (size_t i = 0; i < vertices.size(); i++) {
            size_t j = (i + 1) % vertices.size();
            signedArea += (vertices[i].x * vertices[j].y) - (vertices[j].x * vertices[i].y);
        }

        // In Raylib's Y-down coordinate system:
        // Positive Area = Clockwise (CW) 
        // Negative Area = Counter-Clockwise (CCW)
        bool isClockwise = (signedArea > 0.0f);

        // 2. Ear clipping loop
        while (vertices.size() >= 3) {
            bool earFound = false;

            for (size_t i = 0; i < vertices.size(); i++) {
                size_t prevIdx = (i == 0) ? vertices.size() - 1 : i - 1;
                size_t nextIdx = (i + 1) % vertices.size();

                Vector2 a = vertices[prevIdx];
                Vector2 b = vertices[i];     // The potential ear tip
                Vector2 c = vertices[nextIdx];

                // 3. Dynamic Convexity Check based on the overall shape's winding
                float crossProduct = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
                bool isConvex = isClockwise ? (crossProduct >= 0.0f) : (crossProduct <= 0.0f);

                if (!isConvex) continue; // Skip concave vertices (dents)

                // 4. Confirm no other vertex is inside this ear triangle
                bool isAnEar = true;
                for (size_t j = 0; j < vertices.size(); j++) {
                    if (j == prevIdx || j == i || j == nextIdx) continue;

                    if (IsPointInTriangle(vertices[j], a, b, c)) {
                        isAnEar = false;
                        break;
                    }
                }

                // 5. Clip the ear and draw!
                if (isAnEar) {
                    // Raylib's DrawTriangle REQUIRES Counter-Clockwise order to render properly.
                    // If our polygon is CW, we pass the vertices backwards (c, b, a) to satisfy the GPU.
                    if (isClockwise) {
                        DrawTriangle(c, b, a, fillColor);
                    }
                    else {
                        DrawTriangle(a, b, c, fillColor);
                    }

                    vertices.erase(vertices.begin() + i); // Slice the ear off
                    earFound = true;
                    break;
                }
            }

            // Failsafe: Break if we enter an infinite loop due to self-intersecting lines
            if (!earFound) break;
        }
    }

    // Sum of edge lengths around the closed polygon.
    float calculatePerimeter() const {
        if (points.size() < 2) return 0.0f;
        float perimeter = 0.0f;
        for (size_t i = 0; i < points.size(); i++) {
            Vector2 a = points[i].getPos();
            Vector2 b = points[(i + 1) % points.size()].getPos();
            float dx = b.x - a.x;
            float dy = b.y - a.y;
            perimeter += sqrtf(dx * dx + dy * dy);
        }
        return perimeter;
    }

    // Shoelace formula — works for any simple (non-self-intersecting) polygon.
    // Returns the absolute area in square pixels.
    float calculateArea() const {
        if (points.size() < 3) return 0.0f;
        float sum = 0.0f;
        for (size_t i = 0; i < points.size(); i++) {
            Vector2 a = points[i].getPos();
            Vector2 b = points[(i + 1) % points.size()].getPos();
            sum += (a.x * b.y) - (b.x * a.y);
        }
        return fabsf(sum) * 0.5f;
    }

    void draw() override {
        fillPolygon();

        for (size_t i = 0; i < points.size(); i++) {
            DrawLineEx(points[i].getPos(),
                points[(i + 1) % points.size()].getPos(),
                lineThickness, lineColor);
            points[i].draw();
        }
        for (size_t i = 0; i < points.size(); i++) {
            points[i].draw();
        }
    }
    // Returns true if line segment AB intersects line segment CD
    bool DoLinesIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
        // Standard Line-Intersection math using cross-products
        auto ccw = [](Vector2 p1, Vector2 p2, Vector2 p3) {
            return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
            };
        return (ccw(a, c, d) != ccw(b, c, d)) && (ccw(a, b, c) != ccw(a, b, d));
    }
    bool isValidShape() {
        return checkSelfIntersection();
    }
    bool checkSelfIntersection() {
        if (points.size() < 4) return false; // Triangles can't self-intersect

        size_t numPoints = points.size();

        // Compare every line segment against every other line segment
        for (size_t i = 0; i < numPoints; i++) {
            Vector2 a1 = points[i].getPos();
            Vector2 a2 = points[(i + 1) % numPoints].getPos(); // Line 1

            for (size_t j = i + 2; j < numPoints; j++) {
                // Avoid checking adjacent lines (they share a vertex, so they naturally touch)
                if ((j + 1) % numPoints == i) continue;

                Vector2 b1 = points[j].getPos();
                Vector2 b2 = points[(j + 1) % numPoints].getPos(); // Line 2

                if (DoLinesIntersect(a1, a2, b1, b2)) {
                    return true; // Boom, intersection found!
                }
            }
        }
        return false;
    }
    string GetName() {
        return name;
    }
    void drawTextLabels() {
        if (!ENGINE.SHOW_POINT_COORDINATES) return;

        int fontSize = 14; // This size will now be absolute screen pixels!

        for (const auto& pt : points) {
            // Convert the zoomed/panned world point to standard screen coordinates
            Vector2 screenPos = GetWorldToScreen2D(pt.position, *viewingCamera);

            // Build coordinate string smoothly
            string coordText = "(" + to_string((int)pt.position.x) + "," + to_string((int)pt.position.y) + ")";

            // SMART UX OFFSET: Prevent text crossing over your red polygon lines!
            // Move the text down 12 pixels and right 12 pixels relative to the node circle.
            float textX = screenPos.x + 12.0f;
            float textY = screenPos.y + 12.0f;

            // Draw a tiny dark background drop shadow behind text for perfect contrast on gray grids
            DrawUIText(coordText.c_str(), textX + 1, textY + 1, fontSize, ColorAlpha(BLACK, 0.5f));

            // Draw the crisp foreground text
            DrawUIText(coordText.c_str(), textX, textY, fontSize, DARKGRAY);
        }
    }
    void drawUIProperties() {
        drawTextLabels();
        // 1. DYNAMIC SIDEBAR LAYOUT SELECTION (No Hardcoding)
        float sidebarWidth = windowWidth * 0.25f; // Sidebar takes up exactly 25% of the screen
        float sidebarX = windowWidth - sidebarWidth; // Position it flush against the right edge

        // Create a sleek, semi-translucent dark slate console container
        DrawRectangle(sidebarX, 0, sidebarWidth, windowHeight, Fade(DARKGRAY, 0.9f));
        DrawLineEx({ sidebarX, 0 }, { sidebarX, (float)windowHeight }, 2.0f, MAROON);

        // 2. TEXT MARGINS & ADJUSTABLE POSITIONING
        float paddingX = sidebarX + 25.0f;
        float currentY = 30.0f;
        int titleFontSize = 24;
        int bodyFontSize = 18;
        int tableFontSize = 14;

        // Header Title
        DrawUIText("SHAPE STATS [CAD]", paddingX, currentY, titleFontSize, RAYWHITE);
        currentY += 45.0f;

        // 3. RUNTIME CALCULATIONS

        bool isIntersecting = checkSelfIntersection();
        float totalPerimeter = calculatePerimeter();
        float totalArea = calculateArea();

        // 4. DISPLAY SHAPE INTEGRITY MONITOR (The CAD Flag)
        if (isIntersecting) {
            DrawRectangle(paddingX - 5, currentY - 2, sidebarWidth - 40, 30, Fade(RED, 0.3f));
            DrawUIText("STATUS: SELF-INTERSECTING ERROR", paddingX, currentY, bodyFontSize, RED);
        }
        else {
            DrawUIText("STATUS: GEOMETRY VALID", paddingX, currentY, bodyFontSize, LIME);
        }
        currentY += 40.0f;

        // 5. RENDER TEXT STRINGS (Using standard C-strings cleanly for Raylib)
        char buffer[128];

        // Perimeter UI Row
        snprintf(buffer, sizeof(buffer), "Perimeter: %.2f px", totalPerimeter);
        DrawUIText(buffer, paddingX, currentY, bodyFontSize, LIGHTGRAY);
        currentY += 30.0f;

        // Area UI Row (With warning context if self-intersecting)
        if (isIntersecting) {
            snprintf(buffer, sizeof(buffer), "Area: -- (Invalid Topology)");
            DrawUIText(buffer, paddingX, currentY, bodyFontSize, ORANGE);
        }
        else {
            snprintf(buffer, sizeof(buffer), "Area: %.2f px²", totalArea);
            DrawUIText(buffer, paddingX, currentY, bodyFontSize, LIGHTGRAY);
        }
        currentY += 35.0f;

        // Total Vertices UI Row
        snprintf(buffer, sizeof(buffer), "Total Vertices: %zu", points.size());
        DrawUIText(buffer, paddingX, currentY, bodyFontSize, RAYWHITE);
        currentY += 40.0f;

        // Separator line before table
        DrawLineEx({ paddingX, currentY }, { (float)windowWidth - 25.0f, currentY }, 1.0f, GRAY);
        currentY += 20.0f;

        // 6. DYNAMIC VERTEX COORDINATE DISPLAY TABLE
        DrawUIText("Vertex Coordinates Matrix:", paddingX, currentY, bodyFontSize, GOLD);
        currentY += 30.0f;

        // Loop coordinates and scroll text downwards
        for (size_t i = 0; i < points.size(); i++) {
            // Safe check to prevent string text rows from running off the bottom margin of the window
            if (currentY > windowHeight - 40) {
                DrawUIText("... [Table Overflow]", paddingX, currentY, tableFontSize, ORANGE);
                break;
            }

            Vector2 pos = points[i].getPos();
            snprintf(buffer, sizeof(buffer), "P%-2zu ->  X: %6.1f  |  Y: %6.1f", i, pos.x, pos.y);

            // Highlight the currently dragged point in your layout table
            Color rowColor = (&points[i] == movingPoint) ? SKYBLUE : RAYWHITE;
            DrawUIText(buffer, paddingX + 10, currentY, tableFontSize, rowColor);

            currentY += 22.0f; // Increment row spacing
        }
    }
};