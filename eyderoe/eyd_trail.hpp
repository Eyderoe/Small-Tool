#ifndef TOOL_H
#define TOOL_H
#include "eyd_tool.hpp"
#endif

namespace eyderoe {

class funcTrail
{
    private:
        int screenY{}, screenX{}, blockX{}, blockY{};
        long double xMin{}, xMax{}, yMin{}, yMax{};
        long double *listX = nullptr, *listY = nullptr;
        long double scaleTime{};
        int listLength{};
        int mode{};   //0:normal 1:scientific
        int legal{}, ever_legal{};    //0:legal 1:illegal,been illegal
        int calculate ();
        int makeScreen () const;
        int makeGrid () const;
        int makeCurve ();
        int makeNote () const;
        int makeSpot (int, int, int) const;
        int makeMouse ();
    public:
        funcTrail (int, int);
        int load (long double *, long double *, int);
        int show ();
};
funcTrail::funcTrail (int x, int mode = 0)
{
    screenX = x;
    screenY = x * 0.75;
    blockX = screenX / 50;
    blockY = screenY / 50;
    scaleTime = 1.2;
    legal = 0;
    ever_legal = 0;
    this->mode = mode;
}
int funcTrail::load (long double *x, long double *y, int length)
{   //length = screenX + 1
    listX = x;
    listY = y;
    listLength = length;
    calculate();
    return 0;
}
int funcTrail::calculate ()
{
    int continueCheck;
    xMin = listX[0];
    xMax = listX[listLength - 1];
    yMin = listY[0];
    yMax = listY[0];
    for (int i = 0 ; i < listLength ; ++i) {
        if (std::isinf(listY[i]) == 1 || std::isnan(listY[i]) == 1) {
            legal = 1;
            ever_legal = 1;
            std::cout << "error !!!" << std::endl;
            std::cout << "appear " << listY[i] << " in list[" << i << "]" << std::endl;
            std::cout << "input 1 to continue: ";
            std::cin >> continueCheck;
            if (continueCheck == 1) {
                legal = 0;
                if (i != listLength - 1)
                    listY[i] = listY[i + 1];
                else
                    listY[i] = listY[i - 1];
            }
            if (i == 0) {
                yMin = listY[0];
                yMax = listY[0];
            }
        }
        if (listY[i] > yMax)
            yMax = listY[i];
        if (listY[i] < yMin)
            yMin = listY[i];
    }
    return 0;
}
int funcTrail::show ()
{
    if (legal == 1)
        return 1;
    makeScreen();
    makeGrid();
    makeNote();
    makeCurve();
    makeMouse();
    std::cin.get();
    return 0;
}
int funcTrail::makeScreen () const
{
    initgraph(screenX, screenY);
    setbkcolor(WHITE);
    cleardevice();
    return 0;
}
int funcTrail::makeGrid () const
{
    int init_x = 0, init_y = screenY, i;
    setlinecolor(GREEN);
    i = -1;
    while (init_x <= screenX) {
        setlinestyle(PS_DASH, 1);
        if ((i + 1) % 5 == 0)
            setlinestyle(PS_DASH, 2);
        line(init_x, 0, init_x, screenY);
        init_x += 50;
        i += 1;
    }
    setlinecolor(BLUE);
    i = -1;
    while (init_y >= 0) {
        setlinestyle(PS_DASH, 1);
        if ((i + 1) % 5 == 0)
            setlinestyle(PS_DASH, 2);
        line(0, init_y, screenX, init_y);
        init_y -= 50;
        i += 1;
    }
    return 0;
}
int funcTrail::makeSpot (int x, int y, int r = 1) const
{
    setfillcolor(BLACK);
    setlinecolor(BLACK);
    fillcircle(x, screenY - y, r);
    return 0;
}
int funcTrail::makeCurve ()
{
    long double yScale, yChange;
    long double ySmooth;
    yScale = (yMax - yMin) / screenY;
    yScale *= scaleTime;
    yChange = yMin / yScale;
    for (int i = 0 ; i < listLength ; ++i) {
        makeSpot(i, listY[i] / yScale - yChange + 5, 1);
        if (i != listLength - 1)  // smoothing
        {
            ySmooth = (listY[i + 1] - listY[i]) / 3;
            makeSpot(i, (listY[i] + ySmooth * 1) / yScale - yChange + 5, 0);
            makeSpot(i + 1, (listY[i] + ySmooth * 2) / yScale - yChange + 5, 0);
        }
    }
    return 0;
}
int funcTrail::makeNote () const
{
    long double xPerBlock, yPerBlock;
    std::string forX, forY, frontX, frontY, origin;
    frontX = "x per block: ";
    frontY = "y per block: ";
    xPerBlock = (xMax - xMin) / blockX;
    yPerBlock = (yMax - yMin) / blockY;
    yPerBlock *= scaleTime;
    setbkmode(TRANSPARENT);
    settextstyle(32, 0, _T("Microsoft YaHei UI"));
    settextcolor(0x00b2ff);
    if (mode == 0) {
        frontX += cutString(std::to_string(xPerBlock), 3);
        frontY += cutString(std::to_string(yPerBlock), 3);
    }
    else {
        frontX += transMultStr(xPerBlock);
        frontY += transMultStr(yPerBlock);
    }
    const char *textX = frontX.data();
    const char *textY = frontY.data();
    outtextxy(2, 0, textX);
    outtextxy(2, 32, textY);
    if (ever_legal == 1) {
        settextcolor(0x0000fc);
        outtextxy(2, 64, "master caution! appear nan or inf");
        settextcolor(0x00b2ff);
    }
    // origin
    makeSpot(0, 5, 5);
    origin = "( ";
    if (mode == 0)
        origin += cutString(std::to_string(xMin), 3);
    else
        origin += transMultStr(xMin);
    origin += " , ";
    if (mode == 0)
        origin += cutString(std::to_string(yMin), 3);
    else
        origin += transMultStr(yMin);
    origin += " )";
    const char *textOrigin = origin.data();
    outtextxy(2, screenY - 40, textOrigin);
    return 0;
}
int funcTrail::makeMouse ()
{
    ExMessage m{};
    int x{0};
    std::string screen;
    while (true) {
        m = getmessage(EX_MOUSE|EX_KEY);
        switch (m.message) {
            case WM_MOUSEMOVE:
                if (x != m.x) {
                    if (mode == 0) {
                        std::cout << "( " << listX[m.x] << " , ";
                        std::cout << listY[m.x] << " )" << std::endl;
                    }
                    else {
                        screen = "( ";
                        screen += transMultStr(listX[m.x]);
                        screen += " , ";
                        screen += transMultStr(listY[m.x]);
                        screen += " )";
                        std::cout << screen << std::endl;
                    }
                    x = m.x;
                }
        }
        if (x == screenX + 1)
            break;
    }
    return 0;
}

}
