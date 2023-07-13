#ifndef EYD_TOOL_HPP
#define EYD_TOOL_HPP
#include "eyd_tool.hpp"
#endif

namespace eyderoe {

constexpr long double gravity = 6.67e+5;//e-11

class massPoint {
        struct vectors {
            long double x;
            long double y;
            inline vectors operator* (long double a) const {
                return {x * a, y * a};
            }
            inline vectors operator/ (long double a) const {
                return {x / a, y / a};
            }
            inline void operator+= (vectors a) {
                x += a.x;
                y += a.y;
            }
        };
    private:
        vectors position{};
        vectors positionTemp{};
        vectors velocity{};
        vectors acceleration{};
        long double mass{};
    public:
        massPoint (vectors p, vectors v, long double m);
        void between (const massPoint &another, long double step);
        inline void update ();
        inline int getX () const;
        inline int getY () const;
};
massPoint::massPoint (vectors p, vectors v, long double m)
    : position(p), positionTemp(p), velocity(v) {
    mass = m * 1e+12;
}
void massPoint::between (const massPoint &another, long double step) {
    // 距离
    vectors distance{};
    long double distances;
    distance.x = another.position.x - position.x;
    distance.y = another.position.y - position.y;
    distances = distance.x * distance.x + distance.y * distance.y;
    // 力
    vectors force{};
    long double forces;
    forces = gravity * mass * another.mass / distances;
    force.x = forces * ((another.position.x - position.x) / distances);
    force.y = forces * ((another.position.y - position.y) / distances);
    // 加速度
    acceleration = force / mass;
    // 速度
    velocity += acceleration * step;
    // 位置
    positionTemp += velocity * step;
}
void massPoint::update () {
    position = positionTemp;
}
int massPoint::getY () const {
    return int(position.y);
}
int massPoint::getX () const {
    return int(position.x);
}

class physicalSystem {
    private:
        std::vector<massPoint> pointList{};
        long double step;
        int *colorList = nullptr;
        bool trail;
    public:
        void add (massPoint copy);
        [[noreturn]] void show ();
        explicit physicalSystem (bool trail);
        void calculation ();
        void draw ();
        static inline int RGBtoBGR (int color);
};
void physicalSystem::add (massPoint copy) {
    pointList.push_back(copy);
}
[[noreturn]] void physicalSystem::show () {
    initgraph(800, 600);
    setbkcolor(0x696969);
    cleardevice();
    while (true) {
        calculation();
        draw();
    }
}
physicalSystem::physicalSystem (bool trail) {
    step = 1e-12;
    this->trail = trail;
    // 立春 雨水 谷雨 小暑 立秋 小寒 in rgb
    colorList = new int[12]{0xfff799, 0xecd452, 0xf9d3e3, 0xdd7694, 0xdcc7e1, 0xa67eb7, \
                                0xf5b087, 0xef845d, 0x88abda, 0x5976ba, 0xa4c9cc, 0x509296};
}
void physicalSystem::calculation () {
    for (int i = 0 ; i < pointList.size() ; ++i)
        for (int j = 0 ; j < pointList.size() ; ++j)
            if (i != j)
                pointList[i].between(pointList[j], step);
}
void physicalSystem::draw () {
    for (auto &j : pointList)
        j.update();
    if (!trail)
        cleardevice();
    for (int i = 0 ; i < pointList.size() ; ++i) {
        int colorNow;
        colorNow = RGBtoBGR(colorList[i]);
        setfillcolor(colorNow);
        setlinecolor(colorNow);
        fillcircle(pointList[i].getX(), pointList[i].getY(), 3);
    }
}
int physicalSystem::RGBtoBGR (int color) {
    return ((color >> 16)&0x0000ff)|(color&0x00ff00)|((color << 16)&0xff0000);
}

}
