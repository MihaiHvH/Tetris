#include "../main.hpp"
#include "interface.hpp"

#include <GL/glext.h>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <random>
#include <chrono>
#include <ratio>
#include <sched.h>
#include <string>
#include <thread>

pInterface::pInterface(void(*pRender)(void)) {
    render = pRender;

    cyan.main = graphics.createNewColor(0, 240, 240);
    cyan.up = graphics.createNewColor(153, 255, 255);
    cyan.left = graphics.createNewColor(0, 216, 216);
    cyan.down = graphics.createNewColor(0, 120, 120);
    cyan.right = graphics.createNewColor(0, 216, 216);

    blue.main = graphics.createNewColor(0, 0, 240);
    blue.up = graphics.createNewColor(153, 153, 255);
    blue.left = graphics.createNewColor(0, 0, 216);
    blue.down = graphics.createNewColor(0, 0, 120);
    blue.right = graphics.createNewColor(0, 0, 216);

    orange.main = graphics.createNewColor(240, 160, 0);
    orange.up = graphics.createNewColor(255, 221, 153);
    orange.left = graphics.createNewColor(216, 144, 0);
    orange.down = graphics.createNewColor(120, 80, 0);
    orange.right = graphics.createNewColor(216, 144, 0);
    
    yellow.main = graphics.createNewColor(240, 240, 0);
    yellow.up = graphics.createNewColor(255, 255, 153);
    yellow.left = graphics.createNewColor(216, 216, 0);
    yellow.down = graphics.createNewColor(120, 120, 0);
    yellow.right = graphics.createNewColor(216, 216, 0);

    green.main = graphics.createNewColor(0, 240, 0);
    green.up = graphics.createNewColor(153, 255, 153);
    green.left = graphics.createNewColor(0, 216, 0);
    green.down = graphics.createNewColor(0, 120, 0);
    green.right = graphics.createNewColor(0, 216, 0);

    purple.main = graphics.createNewColor(160, 0, 240);
    purple.up = graphics.createNewColor(221, 153, 255);
    purple.left = graphics.createNewColor(144, 0, 216);
    purple.down = graphics.createNewColor(80, 0, 120);
    purple.right = graphics.createNewColor(144, 0, 216);

    red.main = graphics.createNewColor(240, 0, 0);
    red.up = graphics.createNewColor(255, 153, 153);
    red.left = graphics.createNewColor(216, 0, 0);
    red.down = graphics.createNewColor(120, 0, 0);
    red.right = graphics.createNewColor(216, 0, 0);

    block.main = graphics.createNewColor(91, 89, 90);
    block.up = graphics.createNewColor(157, 157, 157);
    block.left = graphics.createNewColor(123, 123, 123);
    block.down = graphics.createNewColor(39, 42, 41);
    block.right = graphics.createNewColor(60, 57, 58);

    nullCol.main = graphics.createNewColor(0, 0, 0);
    nullCol.up = graphics.createNewColor(0, 0, 0);
    nullCol.left = graphics.createNewColor(0, 0, 0);
    nullCol.down = graphics.createNewColor(0, 0, 0);
    nullCol.right = graphics.createNewColor(0, 0, 0);

    init();
}

void pInterface::init() {
    if (!bInit) {
        for (int i = 0; i <= 15; ++i) {
            if (i <= 10) border.push_back({ i * screen.pieceSize.first, 15 * screen.pieceSize.second });
            border.push_back({ 0 * screen.pieceSize.first, i * screen.pieceSize.second }), border.push_back({ 11 * screen.pieceSize.first, i * screen.pieceSize.second });
        }
        spawnPiece();
        spawnPiece();
    }
    bInit = true;
}

pInterface::~pInterface() {

}

int pInterface::genRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

pPieceColor pInterface::genRandomPieceColor() {
    int r = genRandomNumber(1, 7);
    if (r == 1)
        return cyan;
    else if (r == 2) 
        return blue;
    else if (r == 3)
        return orange;
    else if (r == 4)
        return green;
    else if (r == 5)
        return purple;
    else if (r == 6)
        return red;
    else if (r == 7)
        return yellow;
    return nullCol;
}

ePieceType pInterface::genRandomPieceType() {
    int r = genRandomNumber(1, 7);
    if (r == 1)
        return ePieceType::T;
    else if (r == 2) 
        return ePieceType::I;
    else if (r == 3)
        return ePieceType::O;
    else if (r == 4)
        return ePieceType::S;
    else if (r == 5)
        return ePieceType::Z;
    else if (r == 6)
        return ePieceType::L;
    else if (r == 7)
        return ePieceType::J;
    return ePieceType::null;
}

void pInterface::drawPieces() {
    for (int i = 0; i < (int)pieces.size() - 2; ++i) {
        for (auto k : pieces.at(i).hitbox)
            drawSquare(k, pieces.at(i).color);
    }
    drawPiece(pieces.at(pieces.size() - 2));
}

void pInterface::drawNextPiece() {
    if (pieces.size() > 1) {
        pPiece nextPiece;
        nextPiece = pieces.at(pieces.size() - 1);
        drawPiece(nextPiece, true);
        nextPiece.pos = {
            12 * screen.pieceSize.first + (3 * screen.pieceSize.first - (nextPiece.posMax.first - nextPiece.pos.first)) / 2,
            screen.pieceSize.second + (3 * screen.pieceSize.second - (nextPiece.posMax.second - nextPiece.pos.second)) / 2
        };
        drawPiece(nextPiece);
    }
}

void pInterface::drawFrame() {
    for (int i = 0; i <= 11; ++i) {
        drawSquare({ screen.pieceSize.first * i, 0 }, block);
        drawSquare({ screen.pieceSize.first * i, screen.pieceSize.second * 15 }, block);    
    }
    for (int i = 1; i <= 14; ++i) {
        drawSquare({ 0, i * screen.pieceSize.second }, block);
        drawSquare({ 11 * screen.pieceSize.first, i * screen.pieceSize.second }, block);
        drawSquare({ 16 * screen.pieceSize.first, i * screen.pieceSize.second }, block);
    }
    for (int i = 12; i <= 16; ++i) {
        drawSquare({ screen.pieceSize.first * i, 0 }, block);
        drawSquare({ screen.pieceSize.first * i, screen.pieceSize.second * 15 }, block);
        drawSquare({ screen.pieceSize.first * i, screen.pieceSize.second * 5 }, block);
    }
}

void pInterface::drawGameOverScreen() {
    if (screen.gameOver) {
        pColor red = graphics.createNewColor(255, 0, 0);
        void* font = GLUT_BITMAP_HELVETICA_18;        
        std::pair<int, int> scoreTextSize = graphics.getTextSize(std::string("Score: ").append(std::to_string(screen.score)).c_str(), font);
        std::pair<int, int> levelTextSize = graphics.getTextSize(std::string("Level: ").append(std::to_string(screen.level)).c_str(), font);
        std::pair<int, int> linesTextSize = graphics.getTextSize(std::string("Lines: ").append(std::to_string(screen.lines)).c_str(), font);
        graphics.drawRect({ 0, 6 * screen.pieceSize.second }, { 17 * screen.pieceSize.first, 4 * screen.pieceSize.second }, red);
        graphics.drawText({ (17 * screen.pieceSize.first) / 2 - scoreTextSize.first / 2, 6 * screen.pieceSize.second + screen.pieceSize.second / 2 + scoreTextSize.second / 2 }, font, std::string("Score: ").append(std::to_string(screen.score)).c_str(), graphics.black);
        graphics.drawText({ (17 * screen.pieceSize.first) / 2 - levelTextSize.first / 2, 7 * screen.pieceSize.second + screen.pieceSize.second / 2 + levelTextSize.second / 2 }, font, std::string("Level: ").append(std::to_string(screen.level)).c_str(), graphics.black);
        graphics.drawText({ (17 * screen.pieceSize.first) / 2 - linesTextSize.first / 2, 8 * screen.pieceSize.second + screen.pieceSize.second / 2 + linesTextSize.second / 2 }, font, std::string("Lines: ").append(std::to_string(screen.lines)).c_str(), graphics.black);
    }
}

void pInterface::spawnPiece() {
    pPiece newPiece;

    newPiece.pos = { 5 * screen.pieceSize.first, 2 * screen.pieceSize.second };
    newPiece.color = genRandomPieceColor();
    newPiece.rotation = 0;
    newPiece.type = genRandomPieceType();
    pieces.push_back(newPiece);
    checkGameOver();
}

void pInterface::drawSquare(std::pair<int, int> pos, pPieceColor color) {
    graphics.drawPolygon(pos, { pos.first + screen.pieceSize.first, pos.second }, 
                        { pos.first + screen.pieceSize.first - screen.pieceSize.first / 8, pos.second + screen.pieceSize.second / 8 },
                        { pos.first + screen.pieceSize.first / 8, pos.second + screen.pieceSize.second / 8 }, color.up);
    graphics.drawPolygon(pos, { pos.first + screen.pieceSize.first / 8, pos.second + screen.pieceSize.second / 8 }, 
                        { pos.first + screen.pieceSize.first / 8, pos.second + screen.pieceSize.second - screen.pieceSize.second / 8 },
                        { pos.first, pos.second + screen.pieceSize.second }, color.left);
    graphics.drawPolygon({ pos.first + screen.pieceSize.first / 8, pos.second + screen.pieceSize.second - screen.pieceSize.second / 8 },
                        { pos.first + screen.pieceSize.first - screen.pieceSize.first / 8, pos.second + screen.pieceSize.second - screen.pieceSize.second / 8 },
                        { pos.first + screen.pieceSize.first, pos.second + screen.pieceSize.second },
                        { pos.first, pos.second + screen.pieceSize.second }, color.down);
    graphics.drawPolygon({ pos.first + screen.pieceSize.first - screen.pieceSize.first / 8, pos.second + screen.pieceSize.second / 8 },
                        { pos.first + screen.pieceSize.first, pos.second },
                        { pos.first + screen.pieceSize.first, pos.second + screen.pieceSize.second },
                        { pos.first + screen.pieceSize.first - screen.pieceSize.first / 8, pos.second + screen.pieceSize.second - screen.pieceSize.second / 8 }, color.right);
    graphics.drawRect({ pos.first + screen.pieceSize.first / 8, pos.second + screen.pieceSize.second / 8 }, { screen.pieceSize.first - screen.pieceSize.first / 4, screen.pieceSize.second - screen.pieceSize.second / 4 }, color.main);
}

void pInterface::computeHitbox(pPiece &piece, std::pair<int, int> pos) {
    piece.hitbox.push_back(pos);
}

void pInterface::renderSquare(std::pair<int, int> pos, pPieceColor color, std::vector<std::pair<int, int>> &posMax, bool show) {
    if (show) drawSquare(pos, color);
    posMax.push_back(pos);
    std::vector<int> Xmax;
    std::vector<int> Ymax;

    for (auto i : posMax)
        Xmax.push_back(i.first), Ymax.push_back(i.second);

    std::sort(Xmax.begin(), Xmax.end(), std::greater<int>());
    std::sort(Ymax.begin(), Ymax.end(), std::greater<int>());
    posMax.at(0) = { Xmax.at(0), Ymax.at(0) };
}

void pInterface::drawPiece(pPiece &piece, bool onlyCalcPosMax) {
    std::vector<std::pair<int, int>> posMax;
    piece.hitbox.clear();
    switch (piece.type) {
        case ePieceType::I:
        for (int i = 0; i <= 3; ++i) {
            if (piece.rotation == 0 || piece.rotation == 360 || piece.rotation == 180) computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
            if (piece.rotation == 90 || piece.rotation == 270) computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax); 
        }
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::J:
            if (piece.rotation == 0 || piece.rotation == 360) {
                computeHitbox(piece, { piece.pos.first + 2 * screen.pieceSize.first, piece.pos.second + screen.pieceSize.second });
                renderSquare({ piece.pos.first + 2 * screen.pieceSize.first, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 90) {
                computeHitbox(piece, { piece.pos.first, piece.pos.second + 2 * screen.pieceSize.second });
                renderSquare({ piece.pos.first, piece.pos.second + 2 * screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 180) {
                computeHitbox(piece, piece.pos);
                renderSquare(piece.pos, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 270) {
                computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second });
                renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
            }
            
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::L:
            if (piece.rotation == 0 || piece.rotation == 360) {
                computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second });
                renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 90) {
                computeHitbox(piece, piece.pos);
                renderSquare(piece.pos, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 180) {
                computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * 2, piece.pos.second });
                renderSquare({ piece.pos.first + screen.pieceSize.first * 2, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 270) {
                computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + 2 * screen.pieceSize.second });
                renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + 2 * screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
            }
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::O:
            for (int i = 0; i <= 1; ++i)
                for (int j = 0; j <= 1; ++j)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second * j }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second * j }, piece.color, posMax, !onlyCalcPosMax);
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::Z:
            if (piece.rotation == 0 || piece.rotation == 360 || piece.rotation == 180)
                for (int i = 0; i <= 1; ++i) {
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second });
                    renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * (i+1), piece.pos.second + screen.pieceSize.second });
                    renderSquare({ piece.pos.first + screen.pieceSize.first * (i+1), piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                }
            if (piece.rotation == 90 || piece.rotation == 270)
                for (int i = 0; i <= 1; ++i) {
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i });
                    renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
                    computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second * (i+1) });
                    renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second * (i+1) }, piece.color, posMax, !onlyCalcPosMax);
                }
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::S:
            if (piece.rotation == 0 || piece.rotation == 360 || piece.rotation == 180)
                for (int i = 0; i <= 1; ++i) {
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second });
                    renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * (i+1), piece.pos.second });
                    renderSquare({ piece.pos.first + screen.pieceSize.first * (i+1), piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);    
                }
            if (piece.rotation == 90 || piece.rotation == 270)
                for (int i = 0; i <= 1; ++i) {
                    computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second * i });
                    renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * (i+1) });
                    renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * (i+1) }, piece.color, posMax, !onlyCalcPosMax);
                }
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::T:
            if (piece.rotation == 0 || piece.rotation == 360) {
                computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second });
                renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 90) {
                computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second });
                renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 180){
                computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second });
                renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }), renderSquare({ piece.pos.first + screen.pieceSize.first * i, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
            }
            if (piece.rotation == 270) {
                computeHitbox(piece, { piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second });
                renderSquare({ piece.pos.first + screen.pieceSize.first, piece.pos.second + screen.pieceSize.second }, piece.color, posMax, !onlyCalcPosMax);
                for (int i = 0; i <= 2; ++i)
                    computeHitbox(piece, { piece.pos.first, piece.pos.second + screen.pieceSize.second * i }), renderSquare({ piece.pos.first, piece.pos.second + screen.pieceSize.second * i }, piece.color, posMax, !onlyCalcPosMax);
            }
        piece.posMax = posMax.at(0);
        break;
        case ePieceType::null:
            ;
        break;
    }
}

void pInterface::drawBorder() {
    pColor red = graphics.createNewColor(255, 0, 0);
    for (auto i : border)
        graphics.drawRect({ i.first, i.second }, screen.pieceSize, red);
}

void pInterface::computeBorder() {
    border.clear();
    for (int j = 0; j <= (int)pieces.size() - 3; ++j)
        for (auto k : pieces.at(j).hitbox)
            border.push_back(k);
    for (int i = 0; i <= 15; ++i) {
        if (i <= 10) border.push_back({ i * screen.pieceSize.first, 15 * screen.pieceSize.second });
        border.push_back({ 0 * screen.pieceSize.first, i * screen.pieceSize.second }), border.push_back({ 11 * screen.pieceSize.first, i * screen.pieceSize.second });
    }
}

std::pair<int, int> pInterface::predictFallingPosition() {
    computeBorder();
    std::vector<std::pair<std::pair<int, int>, int>> low;
    if (pieces.size() < 2) return { -1, -1 };
    for (int i = pieces.at(pieces.size() - 2).pos.first / screen.pieceSize.first; i <= pieces.at(pieces.size() - 2).posMax.first / screen.pieceSize.first; ++i) {
        std::vector<std::pair<int, int>> hitbox;
        for (auto j : pieces.at(pieces.size() - 2).hitbox)
            if (j.first / screen.pieceSize.first == i)
                    hitbox.push_back(j);
        std::sort(hitbox.begin(), hitbox.end(), [](auto first, auto second) {
          return first.second > second.second;
        });
        std::pair<int, int> nearestBorder = {hitbox.at(0).first, -1};
        for (auto k : border) {
            if (k.first == hitbox.at(0).first && k.second - hitbox.at(0).second >= 0) {
                if (nearestBorder.second == -1)
                    nearestBorder.second = k.second;
                else if (nearestBorder.second > k.second)
                    nearestBorder.second = k.second;
            }
        }
        low.push_back({hitbox.at(0), nearestBorder.second - hitbox.at(0).second });
    }
    std::sort(low.begin(), low.end(), [](auto first, auto second) {
      return first.second < second.second;
    });
    return { pieces.at(pieces.size() - 2).pos.first, pieces.at(pieces.size() - 2).pos.second + low.at(0).second - screen.pieceSize.second };
}

void pInterface::delRow(int row) {
    for (int r = 0; r < pieces.size() - 2; ++r)
        for (int k = 0; k < pieces.at(r).hitbox.size(); ++k)
            if (pieces.at(r).hitbox.at(k).second == row * screen.pieceSize.second)
                pieces.at(r).hitbox.erase(pieces.at(r).hitbox.begin() + k), --k;
    for (int r = 0; r < pieces.size() - 2; ++r)
        for (int k = 0; k < pieces.at(r).hitbox.size(); ++k) 
            if (pieces.at(r).hitbox.at(k).second < row * screen.pieceSize.second)
                pieces.at(r).hitbox.at(k).second += screen.pieceSize.second;
    computeBorder();
}

void pInterface::checkLevelOrScoreIncrease() {
    screen.automaticMove = false;
    int ln = 0;
    int rws[101] = {};
    for (int j = 14; j >= 1; --j) {
        int cnt = 0;
        for (int i = 1; i <= 10; ++i)
            for (int r = 0; r < pieces.size() - 2; ++r)
                for (auto k : pieces.at(r).hitbox)
                    if (k == std::make_pair(screen.pieceSize.first * i, screen.pieceSize.second * j))
                        ++cnt;
        if (cnt == 10)
            rws[j]++;
    }
    for (int i = 1; i <= 14 - ln; ++i) {
        if (rws[i+ln] == 1) delRow(i+(ln++));
        while (rws[i+ln] == 1 && rws[i + ln] == rws[i+ln+1] && ln + i + 1 <= 14)
            delRow(i+(ln++));
    }
    if (ln > 0) {
        screen.lines += ln;
        screen.level = int(screen.lines / 10);
        if (ln == 1)
            screen.score += (screen.level * 40 + 40);
        else if (ln == 2)
            screen.score += (screen.level + 1) * 100;
        else if (ln == 3)
            screen.score += (screen.level + 1) * 300;
        else if (ln == 4)
            screen.score += (screen.level + 1) * 1200;
    }
    computeBorder();
    screen.automaticMove = false;
}

void pInterface::automaticMove() {
    if (pieces.size() >= 2) {
        checkGameOver();
        pieces.at(pieces.size() - 2).pos.second += screen.pieceSize.second;
        checkGameOver();
        if (predictFallingPosition().second == pieces.at(pieces.size() - 2).pos.second) {
            pieces.at(pieces.size() - 2).pos.second -= screen.pieceSize.second;
            spawnPiece();
            computeBorder();
            checkLevelOrScoreIncrease();
            checkGameOver();
            render();
        }
    }
}

void pInterface::onKeyPress(unsigned char key) {
    if (key == 'a' && pieces.size() >= 2) {
        bool makeMove = true;
        for (auto i : pieces.at(pieces.size() - 2).hitbox)
            for (auto j : border)
                if (i.first - screen.pieceSize.first == j.first && i.second == j.second) { makeMove = false; break; }
        if (makeMove) {
            pieces.at(pieces.size() - 2).pos.first -= screen.pieceSize.first;
            pieces.at(pieces.size() - 2).posMax.first -= screen.pieceSize.first;
            render();
        }
    }
    if (key == 'd' && pieces.size() >= 2) {
        bool makeMove = true;
        for (auto i : pieces.at(pieces.size() - 2).hitbox)
            for (auto j : border)
                if (i.first + screen.pieceSize.first == j.first && i.second == j.second) { makeMove = false; break; }
        if (makeMove) {
            pieces.at(pieces.size() - 2).pos.first += screen.pieceSize.first;
            pieces.at(pieces.size() - 2).posMax.first += screen.pieceSize.first;
            render();
        }
    }
    if (key == 'w' && pieces.size() >= 2) {
        bool makeMove = true;

        pPiece newPiece = pieces.at(pieces.size() - 2);
        if (newPiece.rotation < 360) newPiece.rotation += 90;
        else if (newPiece.rotation == 360) newPiece.rotation = 0;
        drawPiece(newPiece, true);
        
        for (auto i : newPiece.hitbox) {
            for (auto j : border) {
                if (i == j) {
                    makeMove = false;
                    break;
                }
            }
        }

        if (makeMove) {
            if (pieces.at(pieces.size() - 2).rotation < 360) pieces.at(pieces.size() - 2).rotation += 90;
            else if (pieces.at(pieces.size() - 2).rotation == 360) pieces.at(pieces.size() - 2).rotation = 90;
        }
        
        render();
    }
    if (key == 's' && pieces.size() >= 2) {
        pieces.at(pieces.size() - 2).pos = predictFallingPosition();
        render();
        spawnPiece();
        computeBorder();
        checkLevelOrScoreIncrease();
        checkGameOver();
        render();
    }
}

void pInterface::checkGameOver() {
    if (pieces.size() >= 2) {
        for (int r = 0; r < pieces.size() - 2; ++r)
            for (auto k : pieces.at(r).hitbox)
                if (k.first == pieces.at(pieces.size() - 2).pos.first && k.second == pieces.at(pieces.size() - 2).pos.second)
                    screen.gameOver = true;
    }
}