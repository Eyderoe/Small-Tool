#include "map_spawn.hpp"

namespace eyderoe {

// 窗口生成
void spawnScreen ();
// 左侧时，改变区块颜色标识
void blockColorChange (int x, int y, int state);
// 右侧时，检测按键
int switchDetect (int x, int y, int state);
// 识别鼠标状态，作出反应
int deepDistribute (int x, int y, int state);
// 区块网格生成
void spawnGrid ();
// 右侧按钮生成
void spawnSwitch ();
// 生成右侧注解
void spawnNote ();
// 检测鼠标状态
void mapMaking ();
// 改变区块颜色（因为移动格子也要重新绘图，所以就独立出来了
void fillBlock (int x, int y);
// 存储这张地图
void storeMap ();
// 读取一张地图
void readMap ();

// --------------------------------------------------------------------------------------------------------------------
int width = 853, height = 603, separation = 40, separationNum = 15;
int *cell = nullptr;
bool simplify = false; //简易的存储和复杂的存储
int colorList[] = {BLUE, RED, YELLOW, GREEN, BROWN, CYAN, ROSE, \
                        BLACK, PINK, ORANGE, CHARTREUSE};
// 发现忘了一个隐藏道具 无所谓啦 4 5
const char *noteList[] = {"wall", "box", "thorn_H", "thorn_L", "thorn_H with box", \
                        "thorn_L with box", "enemy", "you", "aim", "lock", "key"};
// 注意：是第y行x列，cell存储是一行一行存储 搞混了就很hmmm
// --------------------------------------------------------------------------------------------------------------------



void spawnScreen () {
    initgraph(width, height);
    setbkcolor(WHITE);
    cleardevice();
}

void fillBlock (int x, int y) {
    int fillColor;
    fillColor = (cell[x * separationNum + y] == 0) ? 0xffffff : colorList[cell[x * separationNum + y] - 1];
    setfillcolor(fillColor);
    setlinestyle(PS_SOLID, 1);
    setlinecolor((fillColor == 0xffffff) ? WHITE : BLACK);
    int Left = x * 40 + 10, Top = y * 40 + 10;
    fillrectangle(Left, Top, Left + 20, Top + 20);
}

void blockColorChange (int x, int y, int state) {
    // cell状态：0空气 1墙 2箱子 3高刺 4矮刺 5高刺盒 6矮刺盒 7敌人 8人物 9目标 10锁 11钥匙
    int location = x * separationNum + y;
    switch (state)
    {
        case 1:
            cell[location] += 1;
            break;
        case 2:
            cell[location] = 0;
            break;
        case 3:
            cell[location] += 1;
            break;
        case -3:
            cell[location] -= 1;
            break;
        default:;
    }
    if (cell[location] >= 12) cell[location] = 0;
    if (cell[location] <= -1) cell[location] = 11;
    fillBlock(x, y);
}

void moveMap (int moveKey) {
    // 警告
    settextcolor(0x00b2ff);
    outtextxy(700, 40, "Caution");
    //0左 1右 2上 3下
    int block;
    block = height / separation;
    int *temp = new int[block]();
    if (moveKey == 2)
    {
        for (int i = 0 ; i < block ; ++i)
            temp[i] = cell[i * block];
        for (int i = 0 ; i < block * block ; ++i)
            if (i % block != 0)
                cell[i - 1] = cell[i];
        for (int i = 0 ; i < block ; ++i)
            cell[(i + 1) * block - 1] = temp[i];
    }
    else if (moveKey == 3)
    {
        for (int i = 0 ; i < block ; ++i)
            temp[i] = cell[(i + 1) * block - 1];
        for (int i = block * block - 1 ; i >= 0 ; --i)
            if ((i + 1) % block != 0)
                cell[i + 1] = cell[i];
        for (int i = 0 ; i < block ; ++i)
            cell[i * block] = temp[i];
    }
    else if (moveKey == 0)
    {
        for (int i = 0 ; i < block ; ++i)
            temp[i] = cell[i];
        for (int i = block ; i < block * block ; ++i)
            cell[i - block] = cell[i];
        for (int i = 0 ; i < block ; ++i)
            cell[block * (block - 1) + i] = temp[i];
    }
    else if (moveKey == 1)
    {
        for (int i = 0 ; i < block ; ++i)
            temp[i] = cell[block * (block - 1) + i];
        for (int i = block * (block - 1) - 1 ; i >= 0 ; --i)
            cell[i + block] = cell[i];
        for (int i = 0 ; i < block ; ++i)
            cell[i] = temp[i];
    }
    else; // 读取地图更新颜色就顺便借一下这里
    delete[]temp;
    //色值改变
    for (int x = 0 ; x < block ; ++x)
        for (int y = 0 ; y < block ; ++y)
        {
            // 这里必须要有一个延时，不然程序下一秒就会融化。5/21注：好像又莫名其妙的可以跑起来了
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
            fillBlock(x, y);
        }
    // 警告消除
    setfillcolor(WHITE);
    setlinecolor(WHITE);
    fillrectangle(695, 40, 797, 70);
}

int switchDetect (int x, int y, int state) {
    if (state != 1)return 7; //在右半平面右键、滑轮
    int left = 758, top = 300, moveKey = -1;
    for (int i = 0 ; i < 4 ; ++i)
        if ((x >= left && x <= left + 40) && (y >= top + i * 50 && y <= top + i * 50 + 40))
            moveKey = i;
    if (moveKey != -1)
    {
        moveMap(moveKey);
        return 4;
    }
    if ((x >= 650 && x <= 800) && (y >= 520 && y <= 570))
        return 5;
    if (x >= 708 && x <= 748 && (y >= 350 && y <= 390))
    {
        memset(cell, 0, sizeof(int) * separationNum * separationNum);
        moveMap(4);
        return 7;
    }


    return 6;
}

int deepDistribute (int x, int y, int state) {
    // 左侧：左键1右键2滚轮±3 右侧：方向4结束5空白6清零7
    if (x < 603)
    {
        int blockX, blockY;
        blockX = (x - 3) / separation;
        blockY = (y - 3) / separation;
        blockColorChange(blockX, blockY, state);
        return state;
    }
    else
        return switchDetect(x, y, state);
}

void spawnGrid () {
    setlinecolor(BLUE);
    for (int i = 0 ; i <= separationNum ; ++i)
    {
        setlinestyle(PS_DASH, 2);
        line(0, i * separation, height, i * separation);
        line(i * separation, 0, i * separation, height);
    }
}

void spawnSwitch () {
    setlinestyle(PS_DASHDOTDOT, 5);
    rectangle(650, 520, 800, 570);
    outtextxy(698, 529, "Store");
    setlinestyle(PS_SOLID, 3);
    int left = 758, top = 300;
    const char *letter[] = {"L", "R", "U", "D"};
    rectangle(708, 350, 748, 390);
    outtextxy(713, 355, "CL");
    for (int i = 0 ; i < 4 ; ++i)
    {
        rectangle(left, top + i * 50, left + 40, top + i * 50 + 40);
        outtextxy(left + 13, top + i * 50 + 5, letter[i]);
    }
}

void spawnNote () {
    setlinecolor(BLACK);
    settextcolor(BLACK);
    settextstyle(32, 0, _T("Microsoft YaHei UI"));
    int start = 20, sep = 45;
    for (int i = 0 ; i < 11 ; ++i)
    {
        setfillcolor(colorList[i]);
        fillcircle(620, start + sep * i, 7);
        outtextxy(640, start + sep * i - 10, noteList[i]);
    }
}

void readMap () {
    std::FILE *map;
    map = fopen(("./map.eyderoe"), "r");
    int num, block = separationNum;
    char mode;
    fscanf(map, "%c", &mode);
    if (mode == 's')
    {
        fscanf(map, "%d", &num);
        for (int i = 0 ; i < num * num ; ++i)
            fscanf(map, "%d", &cell[i]);
    }
    else
    {
        int minX, minY, maxX, maxY, calX, calY;
        fscanf(map, "%d %d %d %d", &minX, &minY, &maxX, &maxY);
        calX = maxX - minX + 1;
        calY = maxY - minY + 1;
        if (calX >= 0 && calY >= 0) // 说明起码有一个方块存在，究极嵌套
            for (int y = 0 ; y < block ; ++y)
                for (int x = 0 ; x < block ; ++x)
                    if ((y >= minY && y <= maxY) && (x >= minX && x <= maxX))
                        fscanf(map, "%d", &cell[y * block + x]);
    }
    fclose(map);
}

void mapMaking () {
    ExMessage mouseInfo{};
    int judge = 0;
    while (true)
    {
        mouseInfo = getmessage(EX_MOUSE|EX_KEY);
        switch (mouseInfo.message)
        {
            case WM_LBUTTONDOWN:
                judge = deepDistribute(mouseInfo.x, mouseInfo.y, 1);
                break;
            case WM_RBUTTONDOWN:
                judge = deepDistribute(mouseInfo.x, mouseInfo.y, 2);
                break;
            case WM_MOUSEWHEEL:
                // 这里已经是滚轮移动的情况
                if (mouseInfo.wheel > 0)
                    judge = deepDistribute(mouseInfo.x, mouseInfo.y, 3);
                else
                    judge = deepDistribute(mouseInfo.x, mouseInfo.y, -3);
                break;
        }
        if (judge == 5)
            break;
    }

}

void storeMap () {
    std::FILE *map;
    map = fopen("./map.eyderoe", "w");
    if (simplify)
    {
        fprintf(map, "%c %d %c", 's', separationNum, '\n');
        for (int i = 0 ; i < separationNum * separationNum ; ++i)
            fprintf(map, "%d ", cell[i]);
    }
    else
    {
        int minY = 114, minX = 514, maxY = -1919, maxX = -810, block; // 格子是范围
        block = separationNum;
        for (int y = 0 ; y < block ; ++y)
        {
            for (int x = 0 ; x < block ; ++x)
            {
                if (cell[y * block + x] != 0)
                {
                    minX = (x < minX) ? x : minX;
                    minY = (y < minY) ? y : minY;
                    maxX = (x > maxX) ? x : maxX;
                    maxY = (y > maxY) ? y : maxY;
                }
            }
        }
        fprintf(map, "%c %d %d %d %d\n", 'c', minX, minY, maxX, maxY);
        for (int y = minY ; y <= maxY ; ++y)
            for (int x = minX ; x <= maxX ; ++x)
                fprintf(map, "%d ", cell[y * block + x]);
    }
    fclose(map);
}

void mapSpawn (bool read) {
    cell = new int[separationNum * separationNum]();
    if (read)readMap();
    spawnScreen();
    setorigin(1, 1);
    spawnGrid();
    spawnNote();
    spawnSwitch();
    if (read) moveMap(4);
    mapMaking();
    storeMap();
    delete[] cell;
}

}
