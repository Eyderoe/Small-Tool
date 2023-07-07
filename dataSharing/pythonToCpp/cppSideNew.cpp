#include <iostream>
#include <vector>
#include <string>

namespace eyderoe {

// 1. arrayList 结构体 存放一个数组
// 2. listFile 类 getLine() 获取某行，并返回指定类型的结构体

template<typename alfa>
struct arrayList {
    int num{};
    alfa *list = nullptr;
    friend std::ostream &operator<< (std::ostream &os, const arrayList<alfa> a) {
        for (int i = 0 ; i < a.num ; ++i)
            std::cout << a.list[i] << " ";
        std::cout << std::endl;
        return os;
    }
};

class listFile {
    private:
        std::FILE *filee = nullptr;
    public:
        int reopen ();
        template<typename bravo>
        arrayList<bravo> getLine (arrayList<bravo> c, int lineNum);
        ~listFile ();
};
int listFile::reopen () {
    if (!filee)
        fclose(filee);
    filee = nullptr;
    filee = fopen("../outputFile.txt", "r");
    return 0;
}
listFile::~listFile () {
    fclose(filee);
}
template<typename bravo>
arrayList<bravo> listFile::getLine (arrayList<bravo> c, int lineNum) {
    char temp;
    int endMark;
    std::string strNum;
    std::vector<bravo> vectorNum;
    reopen();
    while (lineNum - 1) {
        fscanf(filee, "%c", &temp);
        if (temp == '\n')
            lineNum--;
    }
    while (true) {
        endMark = fscanf(filee, "%c", &temp);
        // 空格 换行符 EOF 说明一个数字已经结束，否则添加
        if (temp == ' ' || temp == '\n' || endMark == EOF) {
            if (!(strNum.empty())) {
                vectorNum.push_back(bravo(std::stold(strNum))); // 先转换为long double再转换为模板类型
                strNum.clear();
            }
            else; // 这里怎么有个else????????
        }
        else
            strNum.push_back(temp);
        // 换行符 EOF 说明一行已经结束
        if (temp == '\n' || endMark == EOF)
            break;
    }
    c.num = vectorNum.size();
    c.list = new bravo[c.num];
    for (int i = 0 ; i < c.num ; ++i)
        c.list[i] = std::move(vectorNum[i]);
    return c;
}

}