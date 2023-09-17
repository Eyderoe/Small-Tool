#include <cstring>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <memory>
#include <vector>
#include <cmath>

// 乱码原因是string存储格式为gbk(2字节)，而这里输入的是utf-8(3字节)
// 才发现csdn上有编码对应表 没有意义的答辩程序

namespace eyderoe {

class utf8Converter {
        static inline void masterCaution (const std::string &message, int code);
        static inline short pow16 (short y);
        static short str2short (const char *num); // 001F -> 31
        static std::unique_ptr<char[]> short2str (short num); // 31 -> 001F
        struct record { // 通过统计非中文字符数量，可以得到新的插入位置。
            char ascii{};
            int loc{};
        };
    private:
        short *unicode{};
        short *gbk{};
        int length{};
        std::vector<record> recordList;
        void readMapping ();
        void establishRecord (std::string &a); // 删除ASCII 建表
        void resumeRecord (std::string &a); // 根据表 恢复ASCII
        void deleteRecord (std::string &a); // 根据表 删除ASCII gb是通过先调用uni函数实现的
    public:
        std::string toUnicode (std::string utf8);
        std::string toGBK (const std::string &utf8);
        short toCodeUnicode (short gbkCode, bool isPrint);
        short toCodeGBK (short uniCode, bool isPrint);
        utf8Converter ();
        ~utf8Converter ();
};
void utf8Converter::masterCaution (const std::string &message, int code = 0) {
    std::cerr << "MASTER CAUTION !!!" << std::endl << message;
    exit(code);
}
std::string utf8Converter::toGBK (const std::string &utf8) {
    std::string uniStr = toUnicode(utf8);
    std::string gbkStr;
    deleteRecord(uniStr);
    for (int i = 0 ; i < uniStr.size() ; i += 2) {
        short uniNum{}, gbkNum{};
        uniNum = short(int(uniNum)|((uniStr[i] << 8)&0xff00));
        uniNum = short(int(uniNum)|((uniStr[i + 1])&0x00ff));
        for (int j = 0 ; j < length ; ++j) {  // unicode -> gbk 数据有点多，其实可以改成二分法查询
            if (unicode[j] == uniNum)
                gbkNum = gbk[j];
        }
        gbkStr.push_back(char((int(gbkNum >> 8))&0x00ff));
        gbkStr.push_back(char(int(gbkNum)&0x00ff));
    }
    resumeRecord(gbkStr);
    return gbkStr;
}
std::string utf8Converter::toUnicode (std::string utf8) {
    std::string uni;
    establishRecord(utf8);
    for (int i = 0 ; i < utf8.size() ; i += 3) {
        char first = utf8[i], second = utf8[i + 1], third = utf8[i + 2];
        char temp1 = {}, temp2 = {}; // 这里其实可以写个内联函数的 算了
        temp1 = char(int(temp1)|((first << 4)&0xf0)); // 前4位 为 第一个后4位
        temp1 = char(int(temp1)|((second >> 2)&0x0f)); // 后4位 为 第二个中4位
        temp2 = char(int(temp2)|((second << 6)&0xc0)); // 前2位 为 第二个后2位
        temp2 = char(int(temp2)|((third << 0)&0x30)); // 中2位 为 第二个中2位
        temp2 = char(int(temp2)|((third << 0)&0x0f)); // 后4位 为 第二个后4位
        uni.push_back(temp1);
        uni.push_back(temp2);
    }
    resumeRecord(uni);
    return uni;
}
utf8Converter::utf8Converter () {
    readMapping();
}
void utf8Converter::readMapping () {
    const char *path = R"(D:\Clion\unicode2gbk.txt)";
    std::FILE *mappingTable = fopen(path, "r");
    if (!mappingTable)
        masterCaution("file error !");
    char *temp = new char[5];
    int timer = 0;
    std::vector<short> unicodeVec;
    std::vector<short> gbkVec;
    while (true) {
        int back;
        back = fscanf_s(mappingTable, "%s", temp, 5);
        if (back == EOF)
            break;
        if (timer % 2 == 0)
            unicodeVec.push_back(str2short(temp));
        else
            gbkVec.push_back(str2short(temp));
        timer += 1;
    }
    length = int(unicodeVec.size());
    unicode = new short[length];
    gbk = new short[length];
    std::copy(unicodeVec.begin(), unicodeVec.end(), unicode);
    std::copy(gbkVec.begin(), gbkVec.end(), gbk);
    if (unicodeVec.size() != gbkVec.size())
        masterCaution("length disagree !");
    delete[] temp;
    fclose(mappingTable);
}
short utf8Converter::str2short (const char *num) {
    // 这里的num一定是 char num[5]
    short sum = 0;
    for (int i = 3 ; i >= 0 ; --i) {
        short iChar;
        if (num[i] <= '9' && num[i] >= '0')//数字
            iChar = short(num[i] - '0');
        else//字母
            iChar = short(num[i] - 'A' + 10);
        auto temp = pow16(short(3 - i)) * iChar;
        sum = short(short(sum) + short(temp)); // 为什么这样写? 因为不这样Clang-Tidy会报黄，pow16也改写了一遍 艹
    }
    return sum;
}
short utf8Converter::toCodeUnicode (short gbkCode, bool isPrint = false) {
    for (int i = 0 ; i < length ; ++i) {
        if (gbk[i] == gbkCode) {
            if (isPrint)
                std::cout << short2str(unicode[i]).get() << std::endl;
            return unicode[i];
        }
    }
    masterCaution("no unicode found !");
    return 0;
}
short utf8Converter::toCodeGBK (short uniCode, bool isPrint = false) {
    for (int i = 0 ; i < length ; ++i) {
        if (unicode[i] == uniCode) {
            if (isPrint)
                std::cout << short2str(gbk[i]).get() << std::endl;
            return gbk[i];
        }
    }
    masterCaution("no gbk found !");
    return 0;
}
utf8Converter::~utf8Converter () {
    delete[] unicode;
    delete[] gbk;
}
std::unique_ptr<char[]> utf8Converter::short2str (short num) {
    // 因为这一坨扔回去就输出下 没啦，让gpt改的智能指针。可能泄露，可能不会。
    std::unique_ptr<char[]> b(new char[5]);
    b[4] = '\0';
    for (int i = 3 ; i >= 0 ; --i) {
        char temp = char(0x000f&(num >> (4 * (3 - i))));
        if (temp <= 9)
            b[i] = char(int(temp) + int('0'));
        else
            b[i] = char(int(temp) + int('A') - 10);
    }
    return b;
}
void utf8Converter::establishRecord (std::string &a) {
    recordList.clear();
    std::vector<int> location;
    int non = 0;
    for (int i = 0 ; i < a.size() ; ++i) {
        if (a[i] >= ' ' && a[i] <= '~') { // 如果出现了ASCII那几个字符。这个if判断条件可能是正确的
            non += 1;
            recordList.push_back({a[i], i - ((i + 1) - non) / 3});
            location.push_back(i);
        }
    }
    for (int i = int(location.size()) - 1 ; i >= 0 ; --i)
        a.erase(a.begin() + location[i]);
}
void utf8Converter::resumeRecord (std::string &a) {
    for (auto &i : recordList)
        a.insert(i.loc, std::string(1, i.ascii));
}
void utf8Converter::deleteRecord (std::string &a) {
    for (int i = int(recordList.size()) - 1 ; i >= 0 ; --i)
        a.erase(a.begin() + recordList[i].loc);
}
short utf8Converter::pow16 (short y) {
    if (y == 0)
        return 1;
    short base{16};
    for (short i = 0 ; i < (y - 1) ; ++i)
        base *= 16;
    return base;
}

}