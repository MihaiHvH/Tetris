#pragma once

#include <cstdlib>
#include <utility>
#include <string>
#include <vector>

struct pColor {
    int r;
    int g;
    int b;
    int a;
};

struct pScreen {
    std::string windowName = "Tetris";
    std::pair<int, int> size;
    std::pair<int, int> initialSize = { 680, 640 };
    std::pair<int, int> mousePointer;
    std::pair<int, int> pieceSize = { 40, 40 };
    bool leftClick = false;
    bool rightClick = false;
    bool leftClickDrag = false;
    bool automaticMove = false;
    bool gameOver = false;
    int score = 0;
    int level = 0;
    int lines = 0;
};

enum ePieceType {
    I,
    O,
    T,
    L,
    J,
    Z,
    S,
    null
};

struct pPieceColor {
    pColor main;
    pColor up;
    pColor left;
    pColor down;
    pColor right;
};

struct pPiece {
    std::vector<std::pair<int, int>> hitbox;
    std::pair<int, int> pos;
    std::pair<int, int> posMax;
    pPieceColor color;
    ePieceType type;
    int rotation = 0;
};