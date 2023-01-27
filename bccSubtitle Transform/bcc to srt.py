# {"font_size":0.4,"font_color":"#FFFFFF","background_alpha":0.5,"background_color":"#9C27B0","Stroke":"none","body":[{"from":8.24,"to":9.58,"location":2,"content":"我最后一次见到我的家人"},{"from":486.5,"to":488.4,"location":2,"content":"一个人很难过"},{"from":491.879,"to":495.04,"location":2,"content":"帮我度过"}]}
# {"from":3370.76,"to":3374.06,"location":2,"content":"针很快就会完全发挥作用"}
# 3
# 00:03:38,840 --> 00:03:40,299
# 我不想沾血在上面

import math


def timeChange(origin):
    pointLocation = origin.find('.')
    if -1 == pointLocation:
        origin += ".0"
        pointLocation = origin.find('.')
    ms = origin[pointLocation + 1:]
    if len(ms) == 2:
        ms += '0'
    elif len(ms) == 1:
        ms += "00"
    s = int(origin[:pointLocation])
    m = math.floor(s / 60)
    s -= m * 60
    h = math.floor(m / 60)
    m -= h * 60

    def numChange(num):
        num = str(num)
        if int(num) < 10:
            num = '0' + num
        return num

    s = numChange(s)
    m = numChange(m)
    h = numChange(h)
    return h + ':' + m + ':' + s + ',' + ms


fileName = input("file name : ")
bccFile = open(fileName + ".bcc", 'r', encoding="utf-8")
srtFile = open(fileName + ".srt", 'w', encoding="utf-8")
while True:
    readChar = bccFile.read(1)
    if readChar == '[':
        break
times = 1
while True:
    readChar = bccFile.read(1)
    if readChar == ']':
        break
    srtFile.write(str(times) + '\n')
    times += 1
    # {"from":8.24,"to":9.58,"location":2,"content":"我最后一次见到我的家人"}
    for i in range(4):
        tempStr = ""
        end = ',' if i != 3 else '}'
        partReadChar = bccFile.read(1)
        while partReadChar != ':':
            partReadChar = bccFile.read(1)
        partReadChar = bccFile.read(1)
        while partReadChar != end:
            tempStr += partReadChar
            partReadChar = bccFile.read(1)

        print(tempStr)
        if i == 0:
            tempStr = timeChange(tempStr)
            srtFile.write(tempStr+" --> ")
        elif i == 1:
            tempStr = timeChange(tempStr)
            srtFile.write(tempStr+'\n')
        elif i == 3:
            tempStr = tempStr[1:-1]
            srtFile.write(tempStr+'\n\n')
