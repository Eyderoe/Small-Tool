#ifndef EYD_TOOL_HPP
#define EYD_TOOL_HPP
#include "eyd_tool.hpp"
#endif

namespace eyderoe {

constexpr long double GravityConstant = 6.674255e+20; //6.674255e-11

struct massPoint
{
    struct threeDimension { long double x;long double y; };
    threeDimension pos;
    threeDimension vol;
    long double mas;
    bool movable;
};

class physicsSystem
{
    private:
        int screenX;
        int *colorList = nullptr;
        int colorNum;
        long double precision{};
        std::vector<massPoint> pointList;

        int makeScreen () const;
        int calculate ();
        int makeGrid ();
        int makeSpot ();
        static inline int RGBConverter (int RGB);
        static inline long double calculateR (long double x, long double y);
        static inline long double calculateF (long double r, long double m1, long double m2);
        inline int calculateDeltaV (long double dX, long double dY, long double r, long double f, massPoint &A) const;
    public:
        explicit physicsSystem (int screenX);
        ~physicsSystem ();
        int addPoint (massPoint point);
        [[noreturn]] void show ();
};
physicsSystem::physicsSystem (int screenX)
{
    this->screenX = screenX;
    precision = 1e-12;
    colorNum = 12;
    // 立春 雨水 谷雨 小暑 立秋 小寒
    colorList = new int[colorNum]{0xfff799, 0xecd452, 0xf9d3e3, 0xdd7694, 0xdcc7e1, 0xa67eb7, \
                                0xf5b087, 0xef845d, 0x88abda, 0x5976ba, 0xa4c9cc, 0x509296};
}
int physicsSystem::addPoint (massPoint point)
{
    point.vol.x *= 1e+8;
    point.vol.y *= 1e+8;
    pointList.push_back(point);
    if (point.mas <= 0)
        std::cerr << "error in mass";
    return 0;
}
physicsSystem::~physicsSystem ()
{
    pointList.clear();
}
int physicsSystem::makeScreen () const
{
    initgraph(screenX, (int) (0.75 * screenX));
    setbkcolor(0x9b9c8d);
    cleardevice();
    return 0;
}
[[noreturn]] void physicsSystem::show ()
{
    makeScreen();
    while (true) {
        calculate();
        makeGrid();
        makeSpot();
    }
}
int physicsSystem::calculate () //计算每个点的相互作用
{
    long double r, f, deltaX, deltaY;
    for (int i = 0 ; i < pointList.size() - 1 ; ++i) {
        for (int j = i + 1 ; j < pointList.size() ; ++j) {
            //遍历任意两个点
            r = calculateR(pointList[i].pos.x, pointList[j].pos.y); //计算距离
            f = calculateF(r, pointList[i].mas, pointList[j].mas);  //计算万有引力
            deltaX = pointList[j].pos.x - pointList[i].pos.x;   //计算delta x
            deltaY = pointList[j].pos.y - pointList[i].pos.y;
            calculateDeltaV(deltaX, deltaY, r, f, pointList[i]);    //计算速度增量 位置增量
            calculateDeltaV(-deltaX, -deltaY, r, f, pointList[j]);
        }
    }
    return 0;
}
int physicsSystem::makeSpot ()
{
    int color;
    for (int i = 0 ; i < pointList.size() ; ++i) {
        if (i < colorNum)
            color = RGBConverter(colorList[i]);
        else
            std::cerr << "color empty error!" << std::endl;
        setfillcolor(color);
        setlinecolor(color);
        fillcircle((int) pointList[i].pos.x, (int) pointList[i].pos.y, 5);
    }
    return 0;
}
int physicsSystem::makeGrid ()
{
    //cleardevice();
    return 0;
}
int physicsSystem::RGBConverter (int RGB)   //RGB -> BGR
{
    return ((RGB >> 16)&0x0000ff)|(RGB&0x00ff00)|((RGB << 16)&0xff0000);
}
long double physicsSystem::calculateR (long double x, long double y)
{
    return sqrtl(x * x + y * y);
}
long double physicsSystem::calculateF (long double r, long double m1, long double m2)
{
    return GravityConstant * m1 * m2 / (r * r);
}
int physicsSystem::calculateDeltaV (long double dX, long double dY, long double r, long double f, massPoint &A) const
{
    if (!A.movable)
        return 1;
    long double a;
    // 步长的加速度
    a = f / A.mas;
    a *= precision;
    // 步长的速度
    A.vol.x += a * (dX / r);
    A.vol.y += a * (dY / r);
    // 步长位移的修正
    A.pos.x += A.vol.x * precision;
    A.pos.y += A.vol.y * precision;
    return 0;
}

}
