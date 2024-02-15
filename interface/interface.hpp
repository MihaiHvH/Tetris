#pragma once

#include "../structs.hpp"
#include "../graphics/graphics.hpp"
#include <vector>

class pInterface {
    private:
    void(*render)(void);
    void drawSquare(std::pair<double, double> pos, pPieceColor color);
    void renderSquare(std::pair<double, double> pos, pPieceColor color, std::vector<std::pair<double, double>> &posMax, bool show = true);
    void updateMaxPos(pPiece piece);
    void computeHitbox(pPiece &piece, std::pair<double, double> pos);
    void checkLevelOrScoreIncrease();
    public:
    pInterface(void(*pRender)(void));
    ~pInterface();

    bool bInit = false;
    void init();

    void computeBorder();
    void drawPieces();
    void drawFrame();
    void drawPiece(pPiece &piece, bool onlyCalcPosMax = false);
    void drawNextPiece();
    void drawBorder();
    void drawGameOverScreen();
    void checkGameOver();

    void onKeyPress(unsigned char key);

    void automaticMove();
    std::pair<double, double> predictFallingPosition();
    void delRow(int row);

    int genRandomNumber(int min, int max);
    pPieceColor genRandomPieceColor();
    ePieceType genRandomPieceType();

    pScreen screen;
    pGraphics graphics;
    pPieceColor cyan, blue, yellow, green, purple, red, orange, block;
    pPieceColor nullCol;
    std::vector<pPiece> pieces;
    std::vector<std::pair<double, double>> border;

    void spawnPiece();
};