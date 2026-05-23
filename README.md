# RayDotCAD 

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![Raylib](https://img.shields.io/badge/Raylib-5.0-red.svg?style=flat)
![Architecture](https://img.shields.io/badge/Architecture-Scene_Graph-success.svg?style=flat)

**RayDotCAD** is a lightweight, highly structured 2D Computer-Aided Design (CAD) engine built from scratch in C++ using Raylib. 

Designed with a custom **Godot-inspired Scene Graph architecture**, this project serves as both a functional geometry playground and a showcase of advanced C++ design patterns. It handles real-time spatial mathematics, dynamic UI layouts, and state-driven entity management without relying on heavy external GUI frameworks.

## ✨ Features

* **Dynamic Geometry Creation:** Draw and manipulate Polygons, Rectangles, and Circles/Ellipses in real-time.
* **Smart Grid Engine:** Infinite, world-space panning grid with major/minor engineering sub-divisions and dynamic axis centering.
* **Real-Time Measurements:** Context-aware rendering of edge lengths and radii in physical units (`mm`), powered by an adaptable text-masking system.
* **Precision Controls:** * Hold `CTRL` for strict grid-snapping.
  * Hold `SHIFT` to lock ellipse axes into perfect circles.
* **Interactive Tooling:** Fluid, color-coded UX states (Active Drawing, Hover/Glassy Highlights, and Emerald Selection modes).
* **Memory-Safe Deletion:** Safely decouple and free geometry from the engine using the UI toolbar.

## 🏗️ Engine Architecture

RayDotCAD is built to be highly modular and extensible. If you are reading the source code, you will notice several distinct architectural choices that mimic modern game engines:

1. **The Scene Graph (`Node` System):** Every shape, camera, and UI element is a Node. Memory lifecycles, rendering orders, and update loops cascade cleanly down the tree (e.g., `addChild()`, `removeChild()`).
2. **Inversion of Control (Event Signals):** Shapes and UI buttons are completely decoupled from the main program logic. Using `std::function` lambda pipes, tools broadcast state changes into the void, allowing the centralized `Program` factory to catch and assign active geometry safely.
3. **Template Factories:** Shape initialization is handled via a generic `createNewShape<T>()` template, standardizing camera attachments and event hooks automatically.
4. **Decoupled UI Grid:** Buttons and Checkboxes are positioned using a custom `UIGridLayout` math utility, avoiding hard-coded screen coordinates.

## 🎮 Controls

| Action | Keybinding / Input |
| :--- | :--- |
| **Pan Camera** | Middle Mouse Button (Drag) |
| **Zoom Camera** | Mouse Scroll Wheel |
| **Place Vertex** | Left Click |
| **Undo Vertex** | Right Click |
| **Snap to Grid** | Hold `CTRL` |
| **Lock Circle Proportions** | Hold `SHIFT` |
| **Select Object** | Left Click (on existing shape) |
| **Delete Selected** | UI Delete Button |

## 🚀 Getting Started

### Prerequisites
* A C++17 compatible compiler (MSVC, GCC, or Clang)
* [Raylib](https://github.com/raysan5/raylib) installed and configured on your system.
* Visual Studio / CMake (depending on your environment setup).

### Building the Project (Visual Studio)
1. Clone this repository: 
   ```bash
   git clone [https://github.com/szabibalint360-tech/RayDotCAD.git](https://github.com/szabibalint360-tech/RayDotCAD.git)
