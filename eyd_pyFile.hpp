#include <cstdio>
#include <string>
#include <iostream>

namespace eyderoe {

struct locationPtr { int line;int Char; };

class pythonFile
{
        struct line
        {
            char *lineStr = nullptr;
            int charNum = 0; // 每行理论最少包含一换行符
            line *front = nullptr;
            line *next = nullptr;
        };
    private:
        FILE *filePtr = nullptr; // 文件指针
        locationPtr loc{}; // 指针当前位置
        int lineNum{}; // 文件总行数
        line *chainHead = nullptr; // 内容链表，空头指针
        line *chainTail = nullptr; // 内容链表，尾指针
        line *chainNow = nullptr; // 内容链表，当前位置

        int readFile ();
        int writeChain (std::string &eachLine);
    public:
        ~pythonFile ();

        int open (const std::string &filePath, const std::string &fileMode);
        int close (int mode);
        char *next ();
        int read (int size = -1);
        char *readline ();
        int seek (int offset, int whence);
        locationPtr tell ();
        int write ();
};
int pythonFile::close (int mode = 1)
{
    if (filePtr != nullptr) {
        fclose(filePtr);
        filePtr = nullptr;
        // 释放链表
        line *p = chainHead;
        while (p->next != nullptr) {
            p = p->next;
            delete p->front;
        }
        delete p;
        return 1;
    }
    if (mode == 1)
        std::cout << "Master Caution ! 0x01" << std::endl;
    return 0;
}
int pythonFile::open (const std::string &filePath, const std::string &fileMode)
{
    if (filePtr != nullptr) {
        std::cout << "Master Caution ! 0x00" << std::endl;
        return 1;
    }
    loc = {0, 0};
    const char *name, *mode;
    name = filePath.data();
    mode = fileMode.data();
    filePtr = fopen(name, mode);
    lineNum = 0;
    chainHead = new line;
    chainTail = chainHead;
    chainNow = chainHead;
    readFile();
    return 0;
}
pythonFile::~pythonFile ()
{
    close(0);
}
char * pythonFile::next ()
{
    if (filePtr == nullptr) {
        std::cout << "Master Caution ! 0x02" << std::endl;
        return nullptr;
    }
    if (chainNow->next != nullptr)
        return (chainNow->next)->lineStr;
    return nullptr;
}
int pythonFile::seek (int offset, int whence)
{
    return 0;
}
char *pythonFile::readline ()
{
    if (filePtr == nullptr) {
        std::cout << "Master Caution ! 0x02" << std::endl;
        return nullptr;
    }
    return nullptr;
}
locationPtr pythonFile::tell ()
{
    return loc;
}
int pythonFile::write ()
{
    return 0;
}
int pythonFile::read (int size)
{
    return 0;
}
int pythonFile::readFile ()
{   // 突然感觉自己写复杂了，什么腊鸡
    char getChar;
    std::string eachLine;
    eachLine = "";
    while (true) {
        getChar = (char) fgetc(filePtr);
        if (getChar == EOF){
            if (!eachLine.empty()){
                lineNum += 1;
                writeChain(eachLine);
            }
            break;
        }
        eachLine += getChar;
        if (getChar == '\n') {
            lineNum += 1;
            writeChain(eachLine);
        }
    }
    return 0;
}
int pythonFile::writeChain (std::string &eachLine)
{   // 解决最后一行无换行符
    line *spawn;
    spawn = new line;
    spawn->lineStr = new char[eachLine.length() + 1];
    for (int i = 0 ; i < eachLine.length() ; ++i)
        spawn->lineStr[i] = eachLine[i];
    spawn->lineStr[eachLine.length()] = '\0';
    spawn->charNum = (int) eachLine.length(); // 谨记字符串结尾的'\0'
    eachLine = "";
    chainTail->next = spawn;
    spawn->front = chainTail;
    chainTail = spawn;
    return 0;
}

}