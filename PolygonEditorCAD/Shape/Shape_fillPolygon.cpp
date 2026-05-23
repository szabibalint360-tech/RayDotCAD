#include "Shape.h"

Color Shape::getCurrentColor() {
    if (isActive) {
        return GetColor(0x3BA99CA0);
    }
    if (isSelected) {
        return GetColor(0x2ECC71A0);
    }
    if (isHovered) {
        return GetColor(0xF18F0155);
    }
    return fillColor;
}

bool Shape::IsPointInTriangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c) const {
    float d1 = (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
    float d2 = (p.x - c.x) * (b.y - c.y) - (b.x - c.x) * (p.y - c.y);
    float d3 = (p.x - a.x) * (c.y - a.y) - (c.x - a.x) * (p.y - a.y);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

void Shape::fillPolygon() {
    if (points.size() < 3) return;

    list<size_t> indices;
    for (size_t i = 0; i < points.size(); i++)
        indices.push_back(i);

    // Shoelace formula
    float signedArea = 0.0f;
    for (size_t i = 0; i < points.size(); i++) {
        size_t j = (i + 1) % points.size();
        Vector2 a = points[i].getPos();
        Vector2 b = points[j].getPos();
        signedArea += (a.x * b.y) - (b.x * a.y);
    }
    bool isClockwise = (signedArea > 0.0f);

    //lambda helper
    auto cross2D = [](Vector2 a, Vector2 b, Vector2 c) -> float {
        return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
        };

    while (indices.size() >= 3) {
        bool earFound = false;

        for (auto it = indices.begin(); it != indices.end(); ++it) {
            // Get prev and next iterators with wrap-around
            auto prevIt = (it == indices.begin()) ? prev(indices.end()) : prev(it);
            auto nextIt = next(it);
            if (nextIt == indices.end()) nextIt = indices.begin();

            Vector2 a = points[*prevIt].getPos();
            Vector2 b = points[*it].getPos();
            Vector2 c = points[*nextIt].getPos();

            float cp = cross2D(a, b, c);
            bool isConvex = isClockwise ? (cp >= 0.0f) : (cp <= 0.0f);
            if (!isConvex) continue;

            bool isAnEar = true;
            for (auto jt = indices.begin(); jt != indices.end(); ++jt) {
                if (jt == prevIt || jt == it || jt == nextIt) continue;
                if (IsPointInTriangle(points[*jt].getPos(), a, b, c)) {
                    isAnEar = false;
                    break;
                }
            }

            if (isAnEar) {
                isClockwise ? 
                    DrawTriangle(c, b, a, getCurrentColor()):
                    DrawTriangle(a, b, c, getCurrentColor());
                indices.erase(it); // O(1) — no data shifting
                earFound = true;
                break;
            }
        }

        if (!earFound) break;
    }
}