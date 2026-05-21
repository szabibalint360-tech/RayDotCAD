#pragma once
#include "Control.h"
#include "Camera.h"

class Scene : public Node {
public:
    CameraNode* camera = nullptr;
    std::vector<Control*> ui_children; // Stays fixed on screen (Buttons, sidebars)

    virtual void process(double deltaTime) {}

    void addChild(Node* child) {
        if (child == nullptr) return;
        child->parent_ = this;

        // If it's a UI element, it goes to the dedicated UI layers
        if (Control* control = dynamic_cast<Control*>(child)) {
            ui_children.push_back(control);
            return;
        }

        // World space elements (drawn relative to the camera)
        children_.push_back(child);
    }

    void update(double deltaTime) override {
        process(deltaTime);

        // Process world coordinates logic
        Node::update(deltaTime);

        // Process UI coordinates logic
        updateUI(deltaTime);
    }

    virtual void updateUI(double deltaTime) {
        for (Control* ui_element : ui_children) {
            ui_element->update(deltaTime);
        }
    }

    virtual void drawUI() {
        // UI drawing layer is completely flat, untouched by Camera scaling
        for (Control* ui_element : ui_children) {
            ui_element->draw();
        }
    }

    void draw() override {
        // 1. Pass world components (like your Polygon line segments) inside the camera matrix
        if (camera != nullptr) camera->beginDraw();

        Node::draw();

        if (camera != nullptr) camera->endDraw();

        // 2. Draw HUD panels and CAD options statically on top of the viewport
        drawUI();
    }

    // Safe memory cleanup for separate vectors
    ~Scene() {
        for (auto ui : ui_children) delete ui;
    }
};