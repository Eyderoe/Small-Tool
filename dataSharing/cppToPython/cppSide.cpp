#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>

namespace eyderoe {

// 1. arrayOutput(容器[地址或引用]，元素个数，模式[追加或覆盖])
// 2. linking_list单向链表，只支持写入和[]索引。

template<typename kipper>
long long arrayOutput (kipper *array, long long num, bool cover) {
    // cpp不能检查数组元素个数，所以需要传入和返回元素个数，并推荐调用时输出。
    std::ofstream outfile;
    outfile << std::setprecision(std::numeric_limits<long double>::digits10);
    if (cover)
        outfile.open("../outputFile.txt", std::ios::out);
    else
        outfile.open("../outputFile.txt", std::ios::app);
    for (int i = 0 ; i < num ; ++i)
        outfile << array[i] << " ";
    outfile << std::endl;
    outfile.close();
    return num;
}

template<typename floppy>
long long arrayOutput (floppy &array, long long num, bool cover) {
    // cpp不能检查数组元素个数，所以需要传入和返回元素个数，并推荐调用时输出。
    std::ofstream outfile;
    outfile << std::setprecision(std::numeric_limits<long double>::digits10);
    if (cover)
        outfile.open("./outputFile.txt", std::ios::out);
    else
        outfile.open("./outputFile.txt", std::ios::app);
    for (int i = 0 ; i < num ; ++i)
        outfile << array[i] << " ";
    outfile << std::endl;
    outfile.close();
    return num;
}

template<typename chip>
class linking_list // cpp11引入了移动复制函数，所以vector效率不算太差，这个class好像没有太大必要。
{
    private:
        struct node
        {
            chip data{};
            node *next = nullptr;
        };
        node *head = nullptr;
        node *tail = nullptr;
    public:
        void add (chip dat);
        ~linking_list ();
        chip operator[] (long long index);
};
template<typename chip>
chip linking_list<chip>::operator[] (long long index) {
    if (index < 0)
    {
        std::cerr << "index out of range.";
        return 0;
    }
    node *a = head;
    for (int i = 0 ; i < index ; ++i)
        if (a->next)
            a = a->next;
        else
        {
            std::cerr << "index out of range.";
            return 0;
        }
    return a->data;
}
template<typename chip>
linking_list<chip>::~linking_list () {
    node *p = head, *q = nullptr;
    while (p)
    {
        q = p->next;
        delete p;
        p = q;
    }
    head = nullptr;
    tail = nullptr;
}
template<typename chip>
void linking_list<chip>::add (chip dat) {
    node *temp = nullptr;
    if (!head)
    {
        head = new node;
        tail = head;
    }
    else
    {
        temp = new node;
        tail->next = temp;
        tail = temp;
    }
    tail->data = dat;
}

}