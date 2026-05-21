#pragma once
#include "Node.h"

class Control : public Node {
public:
    Control() { name = "Control"; }

    // Controls usually don't use the camera, so we use their local position
    virtual void draw() override {
        if (!visible) return;
        // Logic for buttons/labels goes here
        Node::draw();
    }
};

class Button : public Control {
public:
    string text;
    Vector2 size;
    Color baseColor;
    bool isHovered;
    bool isPressed;

private:
    // MODERN SIGNAL: Can hold raw functions, member functions, or stateful lambdas
    function<void()> on_pressed_callback = nullptr;

public:
    Button(string text, Vector2 size, Color color = DARKGRAY)
        : text(text), size(size), baseColor(color), isHovered(false), isPressed(false) {
        name = "Button";
    }

    // Godot-like connect style naming
    void connect_pressed(function<void()> callback) {
        on_pressed_callback = callback;
    }

    void update(double deltaTime) override {
        if (!visible) return;

        Vector2 globalposition = position;
        Rectangle bounds = { globalposition.x, globalposition.y, size.x, size.y };

        Vector2 mouseposition = GetMousePosition();
        isHovered = CheckCollisionPointRec(mouseposition, bounds);

        if (isHovered) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                isPressed = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isPressed) {
                isPressed = false;

                // FIRE THE EVENT: Self-contained execution!
                if (on_pressed_callback != nullptr) {
                    on_pressed_callback();
                }
            }
        }
        else {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                isPressed = false;
            }
        }

        Control::update(deltaTime);
    }

    void draw() override {
        if (!visible) return;

        Color renderColor = baseColor;
        if (isPressed) {
            renderColor = ColorAlpha(baseColor, 0.6f);
        }
        else if (isHovered) {
            renderColor = ColorAlpha(baseColor, 0.8f);
        }

        DrawRectangleV(position, size, renderColor);
        DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 1.5f, RAYWHITE);

        int fontSize = 16;
        int textWidth = MeasureText(text.c_str(), fontSize);
        float textX = position.x + (size.x - textWidth) / 2.0f;
        float textY = position.y + (size.y - fontSize) / 2.0f;

        DrawUIText(text.c_str(), textX, textY, fontSize, RAYWHITE);

        Control::draw();
    }
};

class CheckBox : public Control {
private:
    string label;
    float boxSize;
    bool checked;
    bool isHovered;

    // THE SIGNAL: Stores an anonymous function signature that accepts a bool
    function<void(bool)> on_toggled_callback = nullptr;

public:
    CheckBox(string label, bool startChecked = false, float size = 24.0f)
        : label(label), checked(startChecked), boxSize(size), isHovered(false) {
        name = "CheckBox";
    }

    // Connect function to the signal
    void connect_toggled(function<void(bool)> callback) {
        on_toggled_callback = callback;
    }

    void update(double deltaTime) override {
        if (!visible) return;

        Rectangle bounds = { position.x, position.y, boxSize, boxSize };
        Vector2 mouseposition = GetMousePosition();
        isHovered = CheckCollisionPointRec(mouseposition, bounds);

        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            checked = !checked; // Flip state

            // FIRE THE EVENT: Notify anyone listening that the state changed!
            if (on_toggled_callback != nullptr) {
                on_toggled_callback(checked);
            }
        }

        Control::update(deltaTime);
    }

    void draw() override {
        if (!visible) return;

        // (Keep your clean drawing logic from before...)
        Color border = isHovered ? GOLD : RAYWHITE;
        DrawRectangleLinesEx({ position.x, position.y, boxSize, boxSize }, 2.0f, border);
        DrawRectangleV({ position.x + 3, position.y + 3 }, { boxSize - 6, boxSize - 6 }, Fade(BLACK, 0.3f));
        if (checked) {
            DrawRectangleV({ position.x + 6, position.y + 6 }, { boxSize - 12, boxSize - 12 }, GetColor(0x489FB5));
        }
        DrawUIText(label.c_str(), position.x + boxSize + 12.0f, position.y + (boxSize - 16) / 2.0f, 16, LIGHTGRAY);

        Control::draw();
    }
};