#ifndef TOOL_H
#define TOOL_H
#include "eyd_tool.hpp"
#endif

class massPoint
{
        struct threeDimension
        {
            long double x;
            long double y;
            long double z;
        };
        typedef threeDimension triD;

    private:
        triD position{};
        triD velocity{};
        triD acceleration{};
        static massPoint ** system;
        static int systemContain;
    public:
        massPoint (triD pos, triD v, triD a);
};
massPoint::massPoint (triD pos, triD v, triD a)
{
    position = pos;
    velocity = v;
    acceleration = a;
}

