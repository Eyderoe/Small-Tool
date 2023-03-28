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
        int mode;   // 1为显示轨迹
        int screenX;
        int *colorList = nullptr;   // 质点颜色
        int colorNum;
        long double precision{};    // 计算步长
        bool timer;  // 计时器
        int backGroundColor;
        std::vector<massPoint> pointList;   // addPoint()放进的地方
        massPoint *pointTrailList = nullptr;    // 绘图用
        int cSL{};   // calculateStartLoc
        int sSL{}; // storeStartLoc

        int makeScreen () const;
        int calculate ();
        int makeCopyList ();
        int changePoint ();
        static inline int BGRConverter (int RGB);
        static inline long double calculateR (long double x, long double y);
        static inline long double calculateF (long double r, long double m1, long double m2);
        inline int calculateDeltaV (long double dX, long double dY, long double r, long double f, massPoint &A) const;
    public:
        explicit physicsSystem (int screenX, int mode);
        ~physicsSystem ();
        int addPoint (massPoint point);
        [[noreturn]] void show ();
};
physicsSystem::physicsSystem (int screenX, int mode = 0)
{
    this->mode = mode;
    this->screenX = screenX;
    backGroundColor = 0x9b9c8d;
    precision = 1e-12;
    colorNum = 12;
    timer = false;
    // 立春 雨水 谷雨 小暑 立秋 小寒
    colorList = new int[colorNum]{0xfff799, 0xecd452, 0xf9d3e3, 0xdd7694, 0xdcc7e1, 0xa67eb7, \
                                0xf5b087, 0xef845d, 0x88abda, 0x5976ba, 0xa4c9cc, 0x509296};
}
int physicsSystem::addPoint (massPoint point)
{
    point.vol.x *= 1e+8;    // 万有引力常数异常进行的修正
    point.vol.y *= 1e+8;
    pointList.push_back(point);
    if (point.mas <= 0)
        std::cerr << "error in mass: " << point.mas << std::endl;
    return 0;
}
physicsSystem::~physicsSystem ()
{
    pointList.clear();
    delete[] pointTrailList;
}
int physicsSystem::makeScreen () const
{
    initgraph(screenX, (int) (0.75 * screenX));
    setbkcolor(backGroundColor);
    cleardevice();
    return 0;
}
[[noreturn]] void physicsSystem::show ()
{
    makeScreen();
    makeCopyList();
    while (true) {
        timer = !timer;
        cSL = (int) timer * pointList.size();
        sSL = (int) pointList.size() - cSL;
        // 这里之前没有任何问题
        calculate();
        changePoint();
    }
}
int physicsSystem::calculate ()
{
    long double r, f, deltaX, deltaY;
    for (int i = 0 ; i < pointList.size() - 1 ; ++i) {
        for (int j = i + 1 ; j < pointList.size() ; ++j) {
            // 以cSL的修正进行计算
            r = calculateR(pointTrailList[i + cSL].pos.x, pointTrailList[j + cSL].pos.y);
            f = calculateF(r, pointTrailList[i + cSL].mas, pointTrailList[j + cSL].mas);
            deltaX = pointTrailList[j + cSL].pos.x - pointTrailList[i + cSL].pos.x;
            deltaY = pointTrailList[j + cSL].pos.y - pointTrailList[i + cSL].pos.y;
            // 以sSL的修正存放结果
            calculateDeltaV(deltaX, deltaY, r, f, pointTrailList[i + sSL]);
            calculateDeltaV(-deltaX, -deltaY, r, f, pointTrailList[j + sSL]);
        }
    }
    return 0;
}
int physicsSystem::BGRConverter (int RGB)   //RGB -> BGR
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
    if (a * precision * precision * 1e-1 < LDBL_EPSILON)
        std::cerr << "MasterCaution! PrecisionLost" << std::endl;
    a *= precision;
    // 步长的速度
    A.vol.x += a * (dX / r);
    A.vol.y += a * (dY / r);
    // 步长位移的修正
    A.pos.x += A.vol.x * precision;
    A.pos.y += A.vol.y * precision;
    return 0;
}
int physicsSystem::makeCopyList ()
{
    pointTrailList = new massPoint[pointList.size() * 2];
    for (int i = 0 ; i < pointList.size() ; ++i) {  //每个元素
        pointTrailList[i] = pointList[i];
        pointTrailList[i + pointList.size()] = pointList[i];
    }
    return 0;
}
int physicsSystem::changePoint ()
{
    int color;
    for (int i = 0 ; i < pointList.size() ; ++i) {
        color = BGRConverter(colorList[i]);
        if (mode == 0) {
            setfillcolor(backGroundColor);
            setlinecolor(backGroundColor);
            fillcircle((int) pointTrailList[i + cSL].pos.x, (int) pointTrailList[i + cSL].pos.y, 5);
        }
        setfillcolor(color);
        setlinecolor(color);
        fillcircle((int) pointTrailList[i + sSL].pos.x, (int) pointTrailList[i + sSL].pos.y, 5);
    }
    return 0;
}

}
