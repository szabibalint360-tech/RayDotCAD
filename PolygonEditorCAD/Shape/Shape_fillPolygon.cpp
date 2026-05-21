#include "Shape.h"

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

    vector<Vector2> vertices;
    vertices.reserve(points.size());
    for (const auto& pt : points) {
        vertices.push_back(pt.getPos());
    }

    float signedArea = 0.0f;
    for (size_t i = 0; i < vertices.size(); i++) {
        size_t j = (i + 1) % vertices.size();
        signedArea += (vertices[i].x * vertices[j].y) - (vertices[j].x * vertices[i].y);
    }

    bool isClockwise = (signedArea > 0.0f);

    while (vertices.size() >= 3) {
        bool earFound = false;

        for (size_t i = 0; i < vertices.size(); i++) {
            size_t prevIdx = (i == 0) ? vertices.size() - 1 : i - 1;
            size_t nextIdx = (i + 1) % vertices.size();

            Vector2 a = vertices[prevIdx];
            Vector2 b = vertices[i];
            Vector2 c = vertices[nextIdx];

            float crossProduct = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
            bool isConvex = isClockwise ? (crossProduct >= 0.0f) : (crossProduct <= 0.0f);

            if (!isConvex) continue;

            bool isAnEar = true;
            for (size_t j = 0; j < vertices.size(); j++) {
                if (j == prevIdx || j == i || j == nextIdx) continue;

                if (IsPointInTriangle(vertices[j], a, b, c)) {
                    isAnEar = false;
                    break;
                }
            }

            if (isAnEar) {
                if (isClockwise) {
                    DrawTriangle(c, b, a, fillColor);
                }
                else {
                    DrawTriangle(a, b, c, fillColor);
                }

                vertices.erase(vertices.begin() + i);
                earFound = true;
                break;
            }
        }

        if (!earFound) break;
    }
}