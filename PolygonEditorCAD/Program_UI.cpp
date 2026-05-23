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

void Program::drawGrid() {
    Camera2D* rlCam = camera->getRaylibCamera();
    Vector2 screenCenter = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    Vector2 worldCenter = GetScreenToWorld2D(screenCenter, *rlCam);
    float   spacing = ENGINE::GRID_SPACING;
    float   ext = GRID_HALF_EXT * spacing;

    // VERTICAL
    for (int i = -GRID_HALF_EXT; i <= GRID_HALF_EXT; i++) {
        float x = roundf(worldCenter.x / spacing) * spacing + (i * spacing);
        int   worldIndex = (int)roundf(x / spacing);

        Color lineColor;
        float thickness;

        if (worldIndex == 0) {
            lineColor = GRID_AXIS;
            thickness = GRID_THICKNESS_AXIS;
        }
        else if (worldIndex % GRID_MAJOR_INTERVAL == 0) {
            lineColor = GRID_COLOR_MAJOR;
            thickness = GRID_THICKNESS_MAJOR;
        }
        else {
            lineColor = GRID_COLOR;
            thickness = GRID_THICKNESS_MINOR;
        }

        DrawLineEx({ x, worldCenter.y - ext },
            { x, worldCenter.y + ext }, thickness, lineColor);
    }

    // HORIZONTAL
    for (int i = -GRID_HALF_EXT; i <= GRID_HALF_EXT; i++) {
        float y = roundf(worldCenter.y / spacing) * spacing + (i * spacing);
        int   worldIndex = (int)roundf(y / spacing);

        Color lineColor;
        float thickness;

        if (worldIndex == 0) {
            lineColor = GRID_AXIS;
            thickness = GRID_THICKNESS_AXIS;
    }
        else if (worldIndex % GRID_MAJOR_INTERVAL == 0) {
            lineColor = GRID_COLOR_MAJOR;
            thickness = GRID_THICKNESS_MAJOR;
    }
        else {
            lineColor = GRID_COLOR;
            thickness = GRID_THICKNESS_MINOR;
}

        DrawLineEx({ worldCenter.x - ext, y },
            { worldCenter.x + ext, y }, thickness, lineColor);
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