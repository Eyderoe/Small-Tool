#include <cstdio>
#include <string>
#include <cmath>
#include <iostream>
#include <graphics.h>
#include <cmath>
#include <vector>
#include <cstdarg>
#include <cfloat>

namespace eyderoe {


//重新分配内存，但数组。用STL应该会更好？
template<typename alfa>
alfa *renew (alfa *list_old, int num_old, int num_new = -1);
//截取小数
long double cutDecimal (long double input, int n);
//科学计数法
long double transMult (long double numy, char mult);
//截取字符串
std::string cutString (std::string str, int cutLength);
//转换为科学计数法表示的字符串
std::string transMultStr (long double a);

template<typename alfa>
alfa *renew (alfa *list_old, int num_old, int num_new)
{
    if (num_new == -1)
        num_new = num_old + 1;
    if (num_old <= 0 || num_new <= 0 || num_new < num_old) {
        std::cerr << "error in length" << std::endl;
        return nullptr;
    }
    int i;
    alfa *list_new = new alfa[num_new]();
    for (i = 0 ; i < num_old ; ++i) 
        list_new[i] = std::move(list_old[i]);
    delete[] list_old;
    return list_new;
}

long double cutDecimal (long double input, int n)
{
    unsigned long long loc;
    std::string a = std::to_string(input);
    long double output;
    loc = a.find('.');
    loc += (n + 1);
    while (a[loc] != '\0') {
        a[loc] = '0';
        loc += 1;
    }
    output = std::stold(a);
    return output;
}

long double transMult (long double numy, char mult)
{
    if ('k' == mult or'K' == mult) numy *= 1e+3;
    else if ('M' == mult) numy *= 1e+6;
    else if ('G' == mult) numy *= 1e+9;
    else if ('1' == mult) numy *= 1;
    else if ('m' == mult) numy *= 1e-3;
    else if ('u' == mult) numy *= 1e-6;
    else if ('n' == mult) numy *= 1e-9;
    else if ('p' == mult) numy *= 1e-12;
    else std::cerr << "unrecognized unit: " << mult << std::endl;
    return numy;
}

std::string cutString (std::string str, int cutLength)
{
    int length;
    std::string alfa;
    length = (int) str.length() - cutLength;
    for (int i = 0 ; i < length ; ++i)
        alfa += str[i];
    return alfa;
}

std::string transMultStr (long double a)
{
    std::string strReturn;
    int isPlus = 1; //1:+ 0:-
    if (a < 0) {
        a = -a;
        isPlus = 0;
    }
    long double multList[] = {1e+9, 1e+6, 1e+3, 1, 1e-3, 1e-6, 1e-9, 1e-12};
    char multName[] = {'G', 'M', 'K', ' ', 'm', 'u', 'n', 'p'};
    for (int i = 0 ; i < 8 ; ++i) {
        if (a / multList[i] >= 1 || i == 7) {
            a /= multList[i];
            if (i == 7 && a < 1e-6) {
                strReturn = "0";
                break;
            }
            strReturn = std::to_string(a);
            if (i != 7)
                strReturn = cutString(strReturn, 3);
            if (i != 3) {
                strReturn += ' ';
                strReturn += multName[i];
            }
            break;
        }
    }
    if (isPlus == 0)
        strReturn = '-' + strReturn;
    return strReturn;
}

}
