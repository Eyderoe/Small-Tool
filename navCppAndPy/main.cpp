#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>

const int startAJ = 1000000;
const int startKT = 3000000;
const int startUZ = 5000000;
const int startAW = 9000000;

struct airway
{
    int num{};
    char name[10]{};
};
struct waypoint
{
    int num{};
    char name[10]{};
    double longitude{};
    double latitude{};
};
struct edge
{
    int num{};
    edge *next = nullptr;
    double distance{};
    int heading{};
    int airwayNum{};
};
struct vertex
{
    int num{};
    edge *next = nullptr;
};
inline int converter (int num, int AJ, int KT)
{
    //renum to base(0)
    if (num >= startUZ)
        num = num - startUZ + (AJ+KT);
    else if (num >= startKT)
        num = num - startKT + AJ;
    else
        num -= startAJ;
    return num;
}
int readConfige (int *a, int *b, int *c, int *d, int *e);
airway *readAirway (airway *a, int b);
waypoint *readWaypoint (waypoint *base, int AJ, int KT, int UZ);
vertex *establishVertexList (vertex *a, int max);
int establishEdge (vertex *list, int AJ, int KT, int np);
int infoGet (int *a, int *b, waypoint *list, int all);
int checkIn (waypoint *list, int all, char *aim,int * a);

int main ()
{
    int numPointAJ, numPointKT, numPointUZ, numAirway, numPath;
    int from, to;
    // config
    readConfige(&numPointAJ, &numPointKT, &numPointUZ, &numAirway, &numPath);
    // airway
    airway *airwayData = nullptr;
    airwayData = readAirway(airwayData, numAirway);
    // waypoint
    waypoint *waypointData = nullptr;
    waypointData = readWaypoint(waypointData, numPointAJ, numPointKT, numPointUZ);
    // vertex
    vertex *vertexList = nullptr;
    vertexList = establishVertexList(vertexList, numPointAJ + numPointKT + numPointUZ);
    // edge
    establishEdge(vertexList, numPointAJ, numPointKT, numPath);
    // info get
    infoGet(&from, &to, waypointData, numPointAJ + numPointKT + numPointUZ);
    // start searching
    return 0;
}

int checkIn (waypoint *list, int all, char *aim,int * a)
{   // hmm
    bool isFind = false;
    int *numList= nullptr , *newList= nullptr, listLoc=0 , listMax = 3 , i;
    numList = new int [listMax];
    for (i = 0 ; i < all ; ++i) {
        if (strcmp(aim, (list + i)->name) == 0) {
            isFind = true;
            if (listLoc != listMax){
                *(numList+listLoc) = i;
                listLoc += 1;
            }
            else{
                newList = new int [listMax+3];
                for (int j = 0 ; j < listMax ; ++j) {
                    newList[j] = numList[j];
                }
                delete [] numList;
                numList = newList;
                newList = nullptr;
                listMax += 3;
                *(numList+listLoc) = i;
                listLoc += 1;
            }
            std::cout << listLoc << ": ";
            std::cout << (list + i)->name << " " << (list + i)->latitude << ",";
            std::cout << (list + i)->longitude << std::endl;
        }
    }
    if (!isFind)
        std::cout << "404 enroute waypoint" << std::endl;
    else{
        if (listLoc == 1)
            *a = i;
        else{
            int choose;
            std::cout << "choose: ";
            std::cin >> choose;
            *a = numList[choose-1];
        }
    }
    return 0;
}

int infoGet (int *a, int *b, waypoint *list, int all)
{
    char fromChar[10], toChar[10];
    std::cout << "from: ";
    std::cin >> fromChar;
    checkIn(list,all,fromChar,a);
    std::cout << "\nto: ";
    std::cin >> toChar;
    checkIn(list,all,toChar,b);
    return 0;
}

int establishEdge (vertex *list, int AJ, int KT, int np)
{
    FILE *pathFile = fopen(R"(D:\C\clion\navCppAndPy\ats.eyder)", "r");
    // 3000000,1000000,9000000,261,18.37
    int from, to, airway, heading;
    double distance;
    edge *node = nullptr;
    for (int i = 0 ; i < np ; ++i) {
        fscanf(pathFile, "%d%*c%d%*c%d%*c%d%*c%lf", &from, &to, &airway, &heading, &distance);
        from = converter(from, AJ, KT);
        to = converter(to, AJ, KT);
        node = new edge[1];
        node->num = to;
        node->distance = distance;
        node->heading = heading;
        node->airwayNum = airway;
        node->next = (list + from)->next;
        (list + from)->next = node;
    }
    fclose(pathFile);
    std::cout << "loading path: " << np << std::endl;
    std::cout << "Adjacency list established\n" << std::endl;
    return 0;
}

vertex *establishVertexList (vertex *a, int max)
{
    a = new vertex[max];
    for (int i = 0 ; i < max ; ++i) {
        (a + i)->num = i;
    }
    return a;
}

waypoint *readWaypoint (waypoint *base, int AJ, int KT, int UZ)
{
    FILE *waypointFile = fopen(R"(D:\C\clion\navCppAndPy\waypoint.eyder)", "r");
    base = new waypoint[AJ + KT + UZ];
    int num = 0;
    double longitude = 0, latitude = 0;
    char clongitude = 0, clatitude = 0;
    for (int i = 0 ; i < (AJ + KT + UZ) ; ++i) {
        // 1000000,N33.364503,E135.441514,ALBAT
        fscanf(waypointFile, "%d%*c%c%lf%*c%c%lf%*c%s", \
                                    &num, &clatitude, &latitude, &clongitude, &longitude, (base + i)->name);
        num = converter(num, AJ, KT);
        (base + i)->num = num;
        (base + i)->latitude = (clatitude == 'N') ? latitude : -latitude;
        (base + i)->longitude = (longitude == 'E') ? longitude : -longitude;
    }
    fclose(waypointFile);
    std::cout << "loading waypoint: " << AJ + KT + UZ << std::endl;
    return base;
}

airway *readAirway (airway *a, int b)
{
    FILE *airwayFile = fopen(R"(D:\C\clion\navCppAndPy\airway.eyder)", "r");
    int tempNum;
    a = new airway[b];
    for (int i = 0 ; i < b ; ++i) {
        fscanf(airwayFile, "%d%*c%s", &tempNum, (a + i)->name);
        (a+i)->num = -1 - (tempNum-startAW);
    }
    fclose(airwayFile);
    std::cout << "loading airway: " << b << std::endl;
    return a;
}

int readConfige (int *a, int *b, int *c, int *d, int *e)
{
    FILE *confige = fopen(R"(D:\C\clion\navCppAndPy\config.eyder)", "r");
    fscanf(confige, "%d%*c%d%*c%d%*c%d%*c%d", a, b, c, d, e);
    fclose(confige);
    return 0;
}
