int main ()
{
    eyderoe::massPoint a {{200,200},{20,20},20, true};
    eyderoe::massPoint b {{200,200},{20,-20},20, true};
    eyderoe::massPoint c {{200,200},{0,0},20, false};
    eyderoe::massPoint d {{400,200},{0,0},20, false};
    eyderoe::physicsSystem system(800);
    system.addPoint(a);
    system.addPoint(b);
    system.addPoint(c);
    system.addPoint(d);
    system.show();
}