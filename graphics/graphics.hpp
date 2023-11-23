#pragma once

#include <utility>
#include "../structs.hpp"

class pGraphics {
    private:

    public:
    pGraphics();
    ~pGraphics();
    
    class pButon;
    class pTextBox;
    class pCheckBox;
    class pImage;
    class pSlider;

    pColor white, black;
    
    void drawPolygon(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3, std::pair<int, int> p4, pColor color);
    void drawRect(std::pair<int, int> pos, std::pair<int, int> size, pColor color);
    void drawFilledCircle(std::pair<int, int> pos, double r, pColor color);
    void drawText(std::pair<int, int> pos, void *font, const char* str, pColor color);
    void drawTriangle(std::pair<int, int> point1, std::pair<int, int> point2, std::pair<int, int> point3, pColor color);
    bool mouseInRegion(std::pair<int, int> mousePointer, std::pair<int, int> pos, std::pair<int, int> size);

    std::pair<int, int> getTextSize(const char* str, void* font);

    pColor createNewColor(int r, int g, int b, int a = 255);
};