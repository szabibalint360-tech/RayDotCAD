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

        // dedicated UI layer
        if (Control* control = dynamic_cast<Control*>(child)) {
            ui_children.push_back(control);
            return;
        }

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
        // UI layer independent of Camera scaling
        for (Control* ui_element : ui_children) {
            ui_element->draw();
        }
    }

    void draw() override {

        if (camera != nullptr) camera->beginDraw();

        Node::draw();

        if (camera != nullptr) camera->endDraw();

        drawUI();
    }

    // Safe memory cleanup for separate vectors
    ~Scene() {
        for (auto ui : ui_children) delete ui;
    }
};