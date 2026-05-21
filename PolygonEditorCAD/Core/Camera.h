#pragma once
#include "Node.h"

class CameraNode : public Node {
private:
    Camera2D raylibCamera = { 0 };
    Node* target = nullptr;
    Vector2  offset = { windowWidth / 2.0f, windowHeight / 2.0f };
    float    zoom = 1.0f;
    float    rotation = 0.0f;

    // pan state
    bool isPanning = false;
    Vector2 panOrigin = { 0, 0 }; // screen pos where drag started
    Vector2 camAtPan = { 0, 0 }; // raylibCamera.target when drag started

    // zoom config
    static constexpr float ZOOM_STEP = 0.1f;
    static constexpr float ZOOM_MIN = 0.1f;
    static constexpr float ZOOM_MAX = 20.0f;
public:
    CameraNode() {
        raylibCamera.offset = offset;
        raylibCamera.zoom = zoom;
    }

    Camera2D* getRaylibCamera() { return &raylibCamera; }

    void setTarget(Node* node) { target = node; }

    void update(double deltaTime) override {
        // ZOOM (scroll wheel, zoom toward mouse cursor)
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            Vector2 mouseScreen = GetMousePosition();
            // World point under cursor before zoom
            Vector2 worldBefore = GetScreenToWorld2D(mouseScreen, raylibCamera);

            zoom = Clamp(zoom + wheel * ZOOM_STEP * zoom, ZOOM_MIN, ZOOM_MAX);
            raylibCamera.zoom = zoom;

            // Shift raylibCamera.target so that same world point stays under cursor
            Vector2 worldAfter = GetScreenToWorld2D(mouseScreen, raylibCamera);
            raylibCamera.target.x += worldBefore.x - worldAfter.x;
            raylibCamera.target.y += worldBefore.y - worldAfter.y;
        }

        //  PAN (middle mouse button) 
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
            isPanning = true;
            panOrigin = GetMousePosition();
            camAtPan = raylibCamera.target;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
            isPanning = false;
        }
        if (isPanning) {
            Vector2 mouse = GetMousePosition();
            // Delta in screen pixels → divide by zoom to get world delta
            raylibCamera.target.x = camAtPan.x - (mouse.x - panOrigin.x) / zoom;
            raylibCamera.target.y = camAtPan.y - (mouse.y - panOrigin.y) / zoom;
        }

        // optional: follow a node target (overrides pan if set)
        if (target != nullptr)
            raylibCamera.target = target->position;

        raylibCamera.offset = offset;
        raylibCamera.rotation = rotation;
        Node::update(deltaTime);
    }

    void beginDraw() { BeginMode2D(raylibCamera); }
    void endDraw() { EndMode2D(); }
};