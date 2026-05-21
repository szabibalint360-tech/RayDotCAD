#pragma once
#include "Node.h"

class CameraNode : public Node {
public:
    Camera2D camera = { 0 };
    Node* target = nullptr;
    Vector2  offset = { windowWidth / 2.0f, windowHeight / 2.0f };
    float    zoom = 1.0f;
    float    rotation = 0.0f;

    // pan state
    bool isPanning = false;
    Vector2 panOrigin = { 0, 0 }; // screen pos where drag started
    Vector2 camAtPan = { 0, 0 }; // camera.target when drag started

    // zoom config
    static constexpr float ZOOM_STEP = 0.1f;
    static constexpr float ZOOM_MIN = 0.1f;
    static constexpr float ZOOM_MAX = 20.0f;

    CameraNode() {
        camera.offset = offset;
        camera.zoom = zoom;
    }

    void setTarget(Node* node) { target = node; }

    void update(double deltaTime) override {
        // ZOOM (scroll wheel, zoom toward mouse cursor)
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            Vector2 mouseScreen = GetMousePosition();
            // World point under cursor before zoom
            Vector2 worldBefore = GetScreenToWorld2D(mouseScreen, camera);

            zoom = Clamp(zoom + wheel * ZOOM_STEP * zoom, ZOOM_MIN, ZOOM_MAX);
            camera.zoom = zoom;

            // Shift camera.target so that same world point stays under cursor
            Vector2 worldAfter = GetScreenToWorld2D(mouseScreen, camera);
            camera.target.x += worldBefore.x - worldAfter.x;
            camera.target.y += worldBefore.y - worldAfter.y;
        }

        // --- PAN (middle mouse button) ---
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
            isPanning = true;
            panOrigin = GetMousePosition();
            camAtPan = camera.target;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
            isPanning = false;
        }
        if (isPanning) {
            Vector2 mouse = GetMousePosition();
            // Delta in screen pixels → divide by zoom to get world delta
            camera.target.x = camAtPan.x - (mouse.x - panOrigin.x) / zoom;
            camera.target.y = camAtPan.y - (mouse.y - panOrigin.y) / zoom;
        }

        // optional: follow a node target (overrides pan if set)
        if (target != nullptr)
            camera.target = target->position;

        camera.offset = offset;
        camera.rotation = rotation;
        Node::update(deltaTime);
    }

    void beginDraw() { BeginMode2D(camera); }
    void endDraw() { EndMode2D(); }
};