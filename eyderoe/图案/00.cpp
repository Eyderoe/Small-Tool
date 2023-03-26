int main ()
{
    eyderoe::massPoint a {{80,80},{0,0},60, true};
    eyderoe::massPoint b {{200,200},{0,0},20, true};
    eyderoe::massPoint c {{180,100},{0,0},30, true};
    eyderoe::massPoint d {{100,180},{0,0},50, true};
    eyderoe::massPoint e {{200,300},{0,0},80, true};
    eyderoe::massPoint f {{300,170},{0,0},30, true};
    eyderoe::massPoint g {{500,500},{0,0},0.5, false};
    eyderoe::physicsSystem system(800);
    system.addPoint(a);
    system.addPoint(b);
    system.addPoint(c);
    system.addPoint(d);
    system.addPoint(e);
    system.addPoint(f);
    system.addPoint(g);
    system.show();
}
