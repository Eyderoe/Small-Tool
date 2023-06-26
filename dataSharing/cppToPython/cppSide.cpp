#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

namespace eyderoe {

// 1. arrayOutput(数组[地址]，元素个数，模式[追加或覆盖])
// 2. arrayOutput(容器[引用]，模式[追加或覆盖])
// 3. linking_list奇怪的单向链表，但高效。

template<typename kipper>
long long arrayOutput (kipper *array, long long num, bool cover) {
    // cpp不能检查数组元素个数，所以需要传入和返回元素个数，并推荐调用时输出。
    std::ofstream outfile;
    outfile << std::setprecision(std::numeric_limits<long double>::digits10);
    outfile.open("../outputFile.txt", cover ? std::ios::out : std::ios::app);
    for (long long i = 0 ; i < num ; ++i)
        outfile << array[i] << " ";
    outfile << std::endl;
    outfile.close();
    return num;
}

template<typename floppy>
size_t arrayOutput (floppy &array, bool cover) {
    // 容器需要支持[]索引，还需要size()方法
    std::ofstream outfile;
    size_t num; // 因为vector.size()那些都是返回的size_t
    num = array.size();
    outfile.open("../outputFile.txt", cover ? std::ios::out : std::ios::app);
    outfile << std::setprecision(std::numeric_limits<long double>::digits10);
    for (size_t i = 0 ; i < num ; ++i)
        outfile << array[i] << " ";
    outfile << std::endl;
    outfile.close();
    return num;
}

template<typename chip>
class linking_list // 奇怪的单向链表
{
    private:
        struct node {
            chip data{};
            node *next = nullptr;
        };
        size_t num = 0, aimIndex = 0;
        node *head = nullptr;
        node *tail = nullptr;
        node *index = nullptr;
    public:
        void add (chip dat); // 正常的添加元素
        ~linking_list (); // 正常的析构函数
        chip operator[] (size_t inop); // 不正常的索引
        int reset (size_t indexNum); // 重置索引
        size_t size (); // 返回大小
};
template<typename chip>
size_t linking_list<chip>::size () {
    return num;
}
template<typename chip>
int linking_list<chip>::reset (size_t indexNum) {
    // 设置下一次输出的位置，下一次将输出[indexNum]
    aimIndex = indexNum;
    if (!indexNum) {
        index = nullptr;
        return 0;
    }
    else // 后面index函数除了[0]以外是先挪位置后输出，所以这个放在前面就好
    {
        if (!head) {
            std::cerr << "EmptyHead" << std::endl;
            return 1;
        }
        index = head;
        for (int i = 0 ; i < indexNum - 1 ; ++i) {
            if (!index->next) {
                std::cerr << "OutOfRange:" << indexNum << std::endl;
                return 2;
            }
            index = index->next;
        }
        return 0;
    }
}
template<typename chip>
chip linking_list<chip>::operator[] (size_t inop) {
    // 每次索引就会向后面挪一个位置，输入是无效的。重置要用reset方法
    if (aimIndex != inop)
        std::cerr << "MasterCaution:IndexDisagree " << aimIndex << "<->" << inop << std::endl;
    ++aimIndex;
    if (!index) {
        index = head;
        if (!head) {
            std::cerr << "EmptyHead" << std::endl;
            return 0;
        }
        return head->data;
    }
    if (index->next) {
        index = index->next;
        return index->data;
    }
    std::cerr << "OutOfRange:" << inop << std::endl;
    return 0;
}
template<typename chip>
linking_list<chip>::~linking_list () {
    node *p = head, *q = nullptr;
    while (p) {
        q = p->next;
        delete p;
        p = q;
    }
}
template<typename chip>
void linking_list<chip>::add (chip dat) {
    node *temp = nullptr;
    num += 1;
    if (!head) {
        head = new node;
        tail = head;
    }
    else {
        temp = new node;
        tail->next = temp;
        tail = temp;
    }
    tail->data = dat;
}

}