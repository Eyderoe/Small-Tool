#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace eyderoe {

// 1. arrayLists 结构体，存放指定类型的 type **
// 2. readLines 函数，返回上面的那个结构体

template<typename biff>
struct arrayLists {
    int num = 0;
    int *nums = nullptr;
    biff **arrays = nullptr;

    friend std::ostream &operator<< (std::ostream &os, const arrayLists<biff> a) {
        for (int i = 0 ; i < a.num ; ++i) // 第i行
        {
            for (int j = 0 ; j < a.nums[i] ; ++j) {
                os << a.arrays[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }
};

template<typename wilf>
inline wilf *trans (wilf *a, std::vector<wilf> &b) { // b转换为a
    if (a)
        std::cerr << "not empty!" << std::endl;
    a = new wilf[b.size()];
    for (int i = 0 ; i < b.size() ; ++i)
        a[i] = std::move(b[i]);
    return a;
}

template<typename wilma>
arrayLists<wilma> readLines () {
    // 感觉自己写了陀答辩出来
    FILE *numFile = nullptr; // numFile为读取的文件
    arrayLists<wilma> sendBack; // sendBack为返回的主要文件
    wilma *tempLDList = nullptr; // tempLDList为一行数字存储的地方
    char tempChar; // tempChar为每次scanf读取的字符
    int endMark; // endMark为scanf返回值
    std::string strNum{}; // 每个数字临时放置的地方
    std::vector<wilma> tempWilmaVector{}; // tempWilmaVector为一行数字临时存储的地方
    std::vector<wilma *> tempListVector{}; // tempListVector为数组首地址存放的地方
    std::vector<int> numsVector{};

    numFile = fopen("../outputFile.txt", "r");
    while (true) {
        endMark = fscanf(numFile, "%c", &tempChar);
        // 空格 换行符 EOF 说明一个数字已经结束，否则添加
        if (tempChar == ' ' || tempChar == '\n' || endMark == EOF) {
            if (!(strNum.empty())) {
                tempWilmaVector.push_back(wilma(std::stold(strNum))); // 先转换为long double再转换为模板类型
                strNum.clear();
            }
            else;   //这里怎么有个else
        }
        else
            strNum.push_back(tempChar);
        // 换行符 EOF 说明一行已经结束
        if (tempChar == '\n' || endMark == EOF) {
            if (!tempWilmaVector.empty()) {
                tempLDList = trans(tempLDList, tempWilmaVector);
                tempListVector.push_back(tempLDList);
                numsVector.push_back(tempWilmaVector.size());
                tempWilmaVector.clear();
                tempLDList = nullptr;
            }
        }
        // EOF 说明文本结束
        if (endMark == EOF)
            break;
    }
    fclose(numFile);
    sendBack.num = numsVector.size();
    sendBack.nums = trans(sendBack.nums, numsVector);
    sendBack.arrays = trans(sendBack.arrays, tempListVector);
    return sendBack;
}

}