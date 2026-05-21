#pragma once
#include "Setup.h"
// A simple struct to handle UI Grid Mathematics
class UIGridLayout {
private:
    Vector2 anchor;   // Bottom-Right corner anchor
    Vector2 itemSize; // Size of each UI element
    Vector2 gap;      // Gap between columns (x) and rows (y)
    int totalCols;    // How many columns wide the grid is

public:
    UIGridLayout(float screenW, float screenH, Vector2 size, Vector2 spacing, float padRight, float padBottom, int cols = 2) {
        itemSize = size;
        gap = spacing;
        totalCols = cols;
        // Set anchor to the absolute bottom-right limit of the screen
        anchor = { screenW - padRight, screenH - padBottom };
    }

    // Row 0 is the BOTTOM row. Col 0 is the LEFT column.
    Vector2 getCellPos(int col, int row) {
        // Calculate the total width of the grid to find the left-most starting edge
        float totalWidth = (totalCols * itemSize.x) + ((totalCols - 1) * gap.x);
        float startX = anchor.x - totalWidth;

        // Calculate specific X and Y based on indices
        float x = startX + (col * (itemSize.x + gap.x));
        float y = anchor.y - itemSize.y - (row * (itemSize.y + gap.y));

        return { x, y };
    }
};
