#include "Program.h"

Program::Program() {
    camera = new CameraNode();
    addChild(camera);

    // Crucial: Call this here so buttons are actually built at startup!
    setupButtons();
}
Program::~Program() {
    for (int i = (int)children_.size() - 1; i >= 0; i--) {
        Node* child = children_[i];
        removeChild(child);
        delete child;
    }
}
void Program::setupButtons() {
    // Checkbox
    checkBoxShowPoint = new CheckBox("Show Coordinates", true);
    checkBoxShowPoint->position = { windowWidth - 300.0f, windowHeight - 60.0f };
    addChild(checkBoxShowPoint);
    checkBoxShowPoint->connect_toggled([this](bool v) {
        ENGINE::SHOW_POINT_COORDINATES = v;
        });

    // Toolbar sizing Layout Constants
    const Vector2 BTN_SIZE = { 160.0f, 38.0f };
    const float   PADDING_RIGHT = 16.0f;
    const float   PADDING_BOTTOM = 16.0f;
    const float   BTN_GAP = 10.0f;

    // Bottom Right Placement calculations
    const float   BTN_X = (float)windowWidth - BTN_SIZE.x - PADDING_RIGHT;
    const float   BASE_Y = (float)windowHeight - BTN_SIZE.y - PADDING_BOTTOM;

    // The power of C++ Lambda Builders
    auto makeBtn = [&](const char* label, float y, Color col) -> Button* {
        Button* b = new Button(label, BTN_SIZE, col);
        b->position = { BTN_X, y };
        addChild(b);
        return b;
        };

    // Stack upwards dynamically
    btnCircle = makeBtn("+ Circle", BASE_Y, ColorFromHSV(30, 0.7f, 0.55f));
    btnRectangle = makeBtn("+ Rectangle", BASE_Y - (BTN_SIZE.y + BTN_GAP), ColorFromHSV(160, 0.6f, 0.45f));
    btnPolygon = makeBtn("+ Polygon", BASE_Y - (BTN_SIZE.y + BTN_GAP) * 2, ColorFromHSV(220, 0.6f, 0.55f));
    btnDeselect = makeBtn("[ Deselect ]", BASE_Y - (BTN_SIZE.y + BTN_GAP) * 3, DARKGRAY);

    // Button callbacks with explicit memory lifecycle safety
    btnDeselect->connect_pressed([this]() {
        setActiveShape(nullptr);
        });

    btnPolygon->connect_pressed([this]() {
        auto* s = new Shape(&(camera->camera));
        setActiveShape(s); // Set active first to handle previous cleanups smoothly
        });

    btnRectangle->connect_pressed([this]() {
        auto* s = new RectangleShape(&(camera->camera));
        setActiveShape(s);
        });

    btnCircle->connect_pressed([this]() {
        auto* s = new CircleShape(&(camera->camera));
        setActiveShape(s);
        });

    activeShape = nullptr;
}

// Central shape switching — manages memory, deactivates old, mounts and activates new
void Program::setActiveShape(Shape* next) {
    if (activeShape != nullptr) {
        activeShape->isActive = false;

        // If the shape is inactive and completely empty (e.g., user clicked a tool 
        // but deselected without drawing anything), purge it from existence.
        if (activeShape->isEmpty()) {
            removeChild(activeShape);
            delete activeShape;
        }
    }

    activeShape = next;

    if (activeShape != nullptr) {
        activeShape->isActive = true;
        addChild(activeShape); // Add the shape node to the tree ONLY when it is chosen
    }
}

void Program::drawGrid() {
    float ext = GRID_HALF_EXT * ENGINE::GRID_SPACING;
    for (int i = -GRID_HALF_EXT; i <= GRID_HALF_EXT; i++) {
        float x = i * ENGINE::GRID_SPACING;
        DrawLineV({ x, -ext }, { x, ext }, (i == 0) ? GRID_AXIS : GRID_COLOR);
    }
    for (int i = -GRID_HALF_EXT; i <= GRID_HALF_EXT; i++) {
        float y = i * ENGINE::GRID_SPACING;
        DrawLineV({ -ext, y }, { ext, y }, (i == 0) ? GRID_AXIS : GRID_COLOR);
    }
}

void Program::draw(){
    if (camera) camera->beginDraw();
    drawGrid();
    Node::draw(); // Draws active shape nodes automatically
    if (camera) camera->endDraw();
    drawUI();
}

void Program::drawUI(){

    if (activeShape == nullptr) {
        //DrawUIText("Select a shape tool to begin.", windowWidth * 0.35f, windowHeight * 0.5f, 20, GRAY); 
    }else{
        activeShape->drawUIProperties();
    }

    Scene::drawUI();//UI buttons
}