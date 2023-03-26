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
        struct location { long double x;long double y; };
    private:
        int mode;   // 1为显示轨迹
        int screenX;    // 屏幕宽度
        int *colorList = nullptr;   // 质点颜色
        int colorNum;   // 预设颜色数量
        long double precision{};    // 计算步长
        int timer;  // 计时器，作用同下
        int backGroundColor;    // 背景色
        int copyNum;    // 轨迹备份数量[解决频闪，但好像不是这个出的问题]
        std::vector<massPoint> pointList;   // 质点当前所有信息
        location *pointTrailList = nullptr; // 质点位置信息，用于绘图

        int makeScreen () const;
        int calculate ();
        int makeCopyList ();    // 初始化轨迹列表
        int makeCopy (); //每次计算后填写轨迹列表
        int removePoint ();  //移除最后一个轨迹点，还是解决不了问题，方案不行。现合并了makeSpot()
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
    timer = 0;
    if (mode == 1)
        copyNum = 1;
    else
        copyNum = 3;
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
        calculate();
        makeCopy();
        removePoint();
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
    // 一般情况精度不会损失。a在1e-6，deltaV在1e-1
    if (!A.movable)
        return 1;
    long double a;
    // 步长的加速度
    a = f / A.mas;
    if (a * precision * precision < LDBL_EPSILON)
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
    pointTrailList = new location[pointList.size() * copyNum];
    for (int i = 0 ; i < pointList.size() ; ++i) {  //每个元素
        for (int j = 0 ; j < copyNum ; ++j) {   //每个元素复制copyNum个
            pointTrailList[i * copyNum + j] = {pointList[i].pos.x, pointList[i].pos.y};
        }
    }
    return 0;
}
int physicsSystem::makeCopy ()
{
    if (timer == copyNum)
        timer = 0;
    for (int i = 0 ; i < pointList.size() ; ++i) {
        pointTrailList[i * copyNum + timer] = {pointList[i].pos.x, pointList[i].pos.y};
    }
    timer += 1;
    return 0;
}
int physicsSystem::removePoint ()
{
    int loc, color, locted;
    loc = timer;    // makeCopy()最后timer+1
    if (timer == copyNum)
        loc = 0;
    locted = timer - 1;
    //移除最后一个点 新增一个点
    for (int i = 0 ; i < pointList.size() ; ++i) {
        color = BGRConverter(colorList[i]);
        if (mode == 0) {
            setfillcolor(backGroundColor);
            setlinecolor(backGroundColor);
            fillcircle((int) pointTrailList[i * copyNum + loc].x, (int) pointTrailList[i * copyNum + loc].y, 5);
        }
        setfillcolor(color);
        setlinecolor(color);
        fillcircle((int) pointTrailList[i * copyNum + locted].x, (int) pointTrailList[i * copyNum + locted].y, 5);
    }
    return 0;
}

}
