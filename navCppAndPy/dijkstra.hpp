#include <iostream>
#include <vector>

namespace dijkstra {

class map {
        struct node {
            int id{};
            double value{};
            node *next{nullptr};
        };
        struct head {
            bool isMarked{false};
            int frontPoint{-2}; // -1表示本身 -2表示无前置路径
            double distance{-1}; // -1表示inf
            node *next{nullptr};
            friend std::ostream &operator<< (std::ostream &out, const head a) {
                out << "isMarked:" << a.isMarked << " frontPoint:" << a.frontPoint << " distance:" << a.distance
                    << std::endl;
                return out;
            }
        };
    private:
        int lengths;
        bool directed;
        head *headList;
        int endPoint{};
        inline void trueAdd (int from, int to, double value);
    public:
        map (int length, bool isDirected);
        ~map ();
        void add (int from, int to, double value);
        void setAim (int start, int end);
        void calculation ();
        void printPath ();
};
map::map (int length, bool isDirected)
    : lengths(length), directed(isDirected) {
    headList = new head[lengths];
}
void map::calculation () { // 应该不是最优，后面还可以再改改。
    while (true) {
        int bestPoint;
        bool isFind = false;
        // 找到这一轮最优的节点
        for (int i = 0 ; i < lengths ; ++i) {
            auto nowHead = headList + i;
            if (!(nowHead)->isMarked) { // 首先没有被标记过
                if (!isFind && (nowHead->distance != -1)) { // 如果当前还没有找到过适合的节点，而且这个节点距离不是inf
                    bestPoint = i;
                    isFind = true;
                }
                else { // 比较该节点和当前最优之间谁最优
                    if ((nowHead->distance != -1) && (nowHead->distance < (headList + bestPoint)->distance))
                        bestPoint = i;
                }
            }
        }
        (headList + bestPoint)->isMarked = true;
        // 分发
        node *neighbor = (headList + bestPoint)->next;
        while (neighbor) {
            head *neighborHead = headList + neighbor->id;
            if (!(neighborHead->isMarked)) { // 这个节点没有被标记过。只更新未被标记过的
                if (neighborHead->distance == -1) { // 节点没有被访问过，距离inf
                    neighborHead->frontPoint = bestPoint;
                    neighborHead->distance = neighbor->value + (headList + bestPoint)->distance;
                }
                else { // 节点曾经被访问过，判断最短
                    if (neighborHead->distance > neighbor->value + (headList + bestPoint)->distance) {
                        neighborHead->frontPoint = bestPoint;
                        neighborHead->distance = neighbor->value + (headList + bestPoint)->distance;
                    }
                }

            }
            neighbor = neighbor->next;
        }
        // 结束条件
        if (!isFind)
            break;
    }
}
void map::add (int from, int to, double value) {
    if (directed)
        trueAdd(from, to, value);
    else {
        trueAdd(from, to, value);
        trueAdd(to, from, value);
    }
}
void map::setAim (int start, int end) {
    // lengths算的元素个数，id序号是从0开始的。
    if (start >= lengths || end >= lengths)
        std::cerr << "MasterCaution:OutOfRange" << std::endl;
    if (start == end)
        std::cout << "DESTINATION CONFLICT\n" << "-START...CHECK\n" << "-END.....CHECK\n" << "--------------\n";
    (headList + start)->distance = 0;
    (headList + start)->frontPoint = -1;
    endPoint = end;
}
map::~map () {
    for (int i = 0 ; i < lengths ; ++i) {
        node *p, *q;
        p = (headList + i)->next;
        while (p) {
            q = p->next;
            delete p;
            p = q;
        }
    }
    delete[] headList;
}
void map::printPath () {
    std::vector<int> path;
    head *p = headList + endPoint;
    bool find = true;
    while ((p->frontPoint) != -1) {
        if (p->frontPoint == -2) {
            std::cerr << "NoPath" << std::endl;
            find = false;
            break;
        }
        path.push_back(int(p - headList));
        p = headList + p->frontPoint;
    }
    if (find) {
        path.push_back(int(p - headList));
        int timer = 0;
        std::cout << "distance:" << (headList + endPoint)->distance << std::endl;
        for (auto it = path.rbegin() ; it != path.rend() ; ++it) {
            timer++;
            std::cout << *it << ((timer != path.size()) ? '-' : ' ');
            if (timer % 7 == 0)
                std::cout << "\n";
        }
    }
}
void map::trueAdd (int from, int to, double value) {
    node *added = new node, *temp;
    added->id = to;
    added->value = value;
    temp = headList[from].next;
    headList[from].next = added;
    added->next = temp;
    if (value <= 0)
        std::cout << "DISTANCE FAIL\n" << "-VALUE...CHECK\n" << "--------------" << std::endl;
}

}