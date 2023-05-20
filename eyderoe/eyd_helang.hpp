#include <vector>
#include <iostream>
#define powerCon(a, b) powerControl(#a,b)

int vectorProcess (std::vector<int> num)
{
    int sum = 0, base = 1, size;
    size = int(num.size());
    for (int i = 0 ; i < size ; ++i) {
        sum += num.back() * base;
        num.pop_back();
        base *= 10;
    }
    std::cout << sum << std::endl;
    return sum;
}

void powerControl (const char *a, int b)
{
    std::cout << a << " : "<< b<<std::endl;
    std::vector<int> temp, list;
    int i = 0;
    while (true) {
        char read = a[i];
        if (read == ' ')
        {
            i+=1;
            continue;
        }
        else if (read == '|') {
            list.push_back(vectorProcess(temp));
            temp.clear();
        }
        else if (read == '\0') {
            list.push_back(vectorProcess(temp));
            temp.clear();
            break;
        }
        else
            temp.push_back(read - '0');
        i += 1;
    }
}