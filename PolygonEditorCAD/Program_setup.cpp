#include "Program.h"
#include "Core/UIGridLayout.h"

void Program::setupButtons() {
    // Layout Metrics
    const Vector2 BTN_SIZE = { 130.0f, 38.0f }; // Slightly narrower so 2 cols fit nicely
    const Vector2 BTN_GAP = { 10.0f, 10.0f };
    const float PADDING_RIGHT = 16.0f;
    const float PADDING_BOTTOM = 16.0f;

    // Initialize the Grid Engine
    UIGridLayout grid((float)windowWidth, (float)windowHeight, BTN_SIZE, BTN_GAP, PADDING_RIGHT, PADDING_BOTTOM, 2);

    // THE LAMBDA BUILDERS 
    auto makeBtn = [&](const char* label, int col, int row, Color colr) -> Button* {
        Button* b = new Button(label, BTN_SIZE, colr);
        b->position = grid.getCellPos(col, row); // Let the utility do the math!
        addChild(b);
        return b;
        };

    auto makeBox = [&](const char* label, bool defaultState, int alignToCol, int row) -> CheckBox* {
        CheckBox* cb = new CheckBox(label, defaultState);
        // Align X to a specific column, but push Y up above the grid
        Vector2 referencePos = grid.getCellPos(alignToCol, row);
        cb->position = { referencePos.x, referencePos.y - 30.0f };
        addChild(cb);
        return cb;
        };


    // INSTANTIATE UI CONTROLS

    // Column 0: Creation Tools | Column 1: Modification Tools
    btnPolygon = makeBtn("+ Polygon", 0, 2, ColorFromHSV(220, 0.6f, 0.55f));

    btnRectangle = makeBtn("+ Rectangle", 0, 1, ColorFromHSV(160, 0.6f, 0.45f));
    btnDelete = makeBtn("[ Delete ]", 1, 1, MAROON); // Red for danger!

    btnCircle = makeBtn("+ Circle", 0, 0, ColorFromHSV(30, 0.7f, 0.55f));
    btnDeselect = makeBtn("[ Deselect ]", 1, 0, DARKGRAY);

    // Checkboxes (Aligned to Col 0, sitting virtually on "Row 3")
    checkBoxShowPoint = makeBox("Show Coordinates", true, 0, 3);

    // EVENT BINDINGS
    setupButtonEvents();


    activeShape = nullptr;
}

template<typename T>
void Program::createNewShape() {
    Camera2D* rlCam = camera->getRaylibCamera();

    // Instantiate the specific derived shape type dynamically
    T* newShape = new T(rlCam);

    // One single place to connect our decoupled event pipelines!
    newShape->connect_activated([this](Shape* target) {
        this->setActiveShape(target);
        });

    addChild(newShape);

    // Set it as our current active tool
    setActiveShape(newShape);
}

void Program::setActiveShape(Shape* next) {
    if (activeShape != nullptr) {
        activeShape->setActive(false);

        if (activeShape->isEmpty()) {
            removeChild(activeShape);
            delete activeShape;
        }
    }
    activeShape = next;

    if (activeShape != nullptr) {
        activeShape->setActive(true);
    }
}
void Program::setupButtonEvents() {

    checkBoxShowPoint->connect_toggled([this](bool v) {
        ENGINE::SHOW_POINT_COORDINATES = v;
        });
    // Modify State Tools
    btnDeselect->connect_pressed([this]() {
        setActiveShape(nullptr);
        });

    // Delete Pipeline
    btnDelete->connect_pressed([this]() {
        if (activeShape != nullptr) {
            removeChild(activeShape);
            delete activeShape;       
            activeShape = nullptr;    
        }
        });

    // Create State Tools
    btnPolygon->connect_pressed([this]() { 
        createNewShape<Shape>(); 
        });
    btnRectangle->connect_pressed([this]() { 
        createNewShape<RectangleShape>(); 
        });
    btnCircle->connect_pressed([this]() { 
        createNewShape<CircleShape>(); 
        });
}