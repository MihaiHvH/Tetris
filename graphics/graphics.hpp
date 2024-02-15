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
    
    void drawPolygon(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3, std::pair<double, double> p4, pColor color);
    void drawRect(std::pair<double, double> pos, std::pair<double, double> size, pColor color);
    void drawFilledCircle(std::pair<double, double> pos, double r, pColor color);
    void drawText(std::pair<double, double> pos, void *font, const char* str, pColor color);
    void drawTriangle(std::pair<double, double> point1, std::pair<double, double> point2, std::pair<double, double> point3, pColor color);
    bool mouseInRegion(std::pair<double, double> mousePointer, std::pair<double, double> pos, std::pair<double, double> size);

    std::pair<double, double> getTextSize(const char* str, void* font);

    pColor createNewColor(int r, int g, int b, int a = 255);
};