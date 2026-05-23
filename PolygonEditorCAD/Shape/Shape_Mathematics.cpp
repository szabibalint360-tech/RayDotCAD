#include "Shape.h"

float Shape::cross2D(Vector2 a, Vector2 b, Vector2 c) const {
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

float Shape::calculatePerimeter() const {
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

float Shape::calculateArea() const {//shoelace formula
    if (points.size() < 3) return 0.0f;
    float sum = 0.0f;
    for (size_t i = 0; i < points.size(); i++) {
        Vector2 a = points[i].getPos();
        Vector2 b = points[(i + 1) % points.size()].getPos();
        sum += (a.x * b.y) - (b.x * a.y);
    }
    return fabsf(sum) * 0.5f;
}

bool Shape::DoLinesIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) const {
    auto ccw = [](Vector2 p1, Vector2 p2, Vector2 p3) {
        return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
        };
    return (ccw(a, c, d) != ccw(b, c, d)) && (ccw(a, b, c) != ccw(a, b, d));
}

bool Shape::isValidShape() {
    return checkSelfIntersection();
}
bool Shape::containsPoint(Vector2 p) const {
    size_t numPoints = points.size();

    if (numPoints < 3) return false;

    bool inside = false;

    for (size_t i = 0, j = numPoints - 1; i < numPoints; j = i++) {
        Vector2 va = points[i].position;
        Vector2 vb = points[j].position;

        if (((va.y > p.y) != (vb.y > p.y)) &&
            (p.x < (vb.x - va.x) * (p.y - va.y) / (vb.y - va.y) + va.x)) {

            inside = !inside;
        }
    }

    return inside;
}
bool Shape::checkSelfIntersection() {
    if (points.size() < 4) return false;

    size_t numPoints = points.size();

    for (size_t i = 0; i < numPoints; i++) {
        Vector2 a1 = points[i].getPos();
        Vector2 a2 = points[(i + 1) % numPoints].getPos();

        for (size_t j = i + 2; j < numPoints; j++) {
            if ((j + 1) % numPoints == i) continue;

            Vector2 b1 = points[j].getPos();
            Vector2 b2 = points[(j + 1) % numPoints].getPos();

            if (DoLinesIntersect(a1, a2, b1, b2)) {
                return true;
            }
        }
    }
    return false;
}
