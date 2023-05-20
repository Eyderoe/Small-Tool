import time


def getLoc(text, char):
    locList = []
    loc = 0
    for i in text:
        if i == char:
            locList.append(loc)
        loc += 1
    return locList


def wpsn(text, loc):  # 优化
    ii = 0
    # ["PINAB", ['30.579167', '106.305278'],"UGOPO",['30.106111','30.106111'],"261","3.14"]
    listReturn = ["", ['', ''], "", ['', ''], "", ""]
    listReturn[0] = text[loc[0] + 1: loc[1]]
    listReturn[1][0] = text[loc[1] + 1: loc[2]]
    listReturn[1][1] = text[loc[2] + 1: loc[3]]
    listReturn[2] = text[loc[3] + 1: loc[4]]
    listReturn[3][0] = text[loc[4] + 1: loc[5]]
    listReturn[3][1] = text[loc[5] + 1: loc[6]]
    listReturn[4] = text[loc[7] + 1: loc[8]]
    listReturn[5] = text[loc[8] + 1: -1]
    return listReturn.copy()


def locChange(loc):
    # loc=[32.98,-32.72]
    strReturn = ""
    one = float(loc[0])
    two = float(loc[1])
    if one >= 0:
        strReturn += 'N'
    else:
        one = -one
        strReturn += 'S'
    one = str(one)
    for i in range(2, one.find('.'), -1):  # 前补齐
        one = "0" + one
    for i in range(9 - len(one)):  # 后补齐
        one += '0'
    strReturn += one

    strReturn += ','
    if two >= 0:
        strReturn += 'E'
    else:
        two = -two
        strReturn += 'W'
    two = str(two)
    for i in range(3, two.find('.'), -1):
        two = "0" + two
    for i in range(10 - len(two)):
        two += '0'
    strReturn += two
    return strReturn


atsFile = open("ATS.txt", mode='r')
airwayList = open("airway.eyder", mode='w')
waypointList = open("waypoint.eyder", mode='w')
atsList = open("ats.eyder", mode='w')
confige = open("config.eyder", mode='w')

airwayContain = []  # 编号从 9,000,000开始
wnA_J = []  # 编号从 1,000,000开始
wlA_J = []
wnK_T = []  # 编号从 3,000,000开始
wlK_T = []
wnU_Z = []  # 编号从 5,000,000开始
wlU_Z = []
checkPoint = 0
path = 0
located = -1
starTime = time.time()

while True:
    eachLine = atsFile.readline()
    checkPoint += 1
    if checkPoint % 10000 == 0:
        print("loading: ", int(checkPoint / 2000), '%')
    # 文件尾检测
    if eachLine == '':
        break
    # 内容检测
    charNum = eachLine.count(',')
    # 航路名称
    if charNum == 2:
        loc = getLoc(eachLine, ',')
        airwayName = eachLine[loc[0] + 1: loc[1]]
        # 航路添加
        located = -1
        checky = 1
        for i in range(len(airwayContain) - 1, -1, -1):
            if airwayContain[i] == airwayName:
                located = i
                checky = 0
                break
        if checky == 1:
            located = len(airwayContain)
            airwayContain.append(airwayName)
    # 航点路径
    elif charNum == 9:
        path += 1
        loc = getLoc(eachLine, ',')
        fromTo = wpsn(eachLine, loc)
        # ["PINAB", ['30.579167', '106.305278'],"UGOPO",['30.106111','30.106111'],"261","3.14"]
        LocNum = []
        # [3000000,3000001,7000000,261,3.14] 航点1 航点2 航路 航向 距离

        # 航路点添加
        for ii in range(0, 3, 2):
            name = fromTo[ii]
            lang = fromTo[ii + 1]
            check = 0
            if 'A' <= name[0] <= 'J':
                location = len(wnA_J)  # 起始查找位置
                for i in range(len(wnA_J) - 1, -1, -1):
                    location -= 1  # 标准位置
                    if (name == wnA_J[i]) and (lang == wlA_J[i]):
                        check = 1
                        break
                if check == 0:
                    location = len(wnA_J)  # 航路点第一次出现的情况
                    wnA_J.append(name)
                    wlA_J.append(lang.copy())
                LocNum.append(1000000 + location)
            elif 'K' <= name[0] <= 'T':
                location = len(wnK_T)
                for i in range(len(wnK_T) - 1, -1, -1):
                    location -= 1
                    if (name == wnK_T[i]) and (lang == wlK_T[i]):
                        check = 1
                        break
                if check == 0:
                    location = len(wnK_T)
                    wnK_T.append(name)
                    wlK_T.append(lang.copy())
                LocNum.append(3000000 + location)
            else:
                location = len(wnU_Z)
                for i in range(len(wnU_Z) - 1, -1, -1):
                    location -= 1
                    if (name == wnU_Z[i]) and (lang == wlU_Z[i]):
                        check = 1
                        break
                if check == 0:
                    location = len(wnU_Z)
                    wnU_Z.append(name)
                    wlU_Z.append(lang.copy())
                LocNum.append(5000000 + location)
        # 重写对应表 每次碰到航路点的情况 写一次
        atsList.write(str(LocNum[0]) + ',')
        atsList.write(str(LocNum[1]) + ',')
        atsList.write(str(9000000 + located) + ',')
        for k in range(len(fromTo[4]), 3):
            fromTo[4] = '0' + fromTo[4]
        atsList.write(fromTo[4] + ',')
        atsList.write(fromTo[5] + '\n')

for i in range(len(airwayContain)):
    # 7000000,A1
    airwayList.write(str(9000000 + i) + ',')
    airwayList.write(airwayContain[i])
    airwayList.write('\n')
for i in range(len(wnA_J)):
    # 1000000,N30.43,E038.72,PINAB
    waypointList.write(str(1000000 + i) + ',')
    waypointList.write(locChange(wlA_J[i]))
    waypointList.write(',' + wnA_J[i] + '\n')
for i in range(len(wnK_T)):
    waypointList.write(str(3000000 + i) + ',')
    waypointList.write(locChange(wlK_T[i]))
    waypointList.write(',' + wnK_T[i] + '\n')
for i in range(len(wnU_Z)):
    waypointList.write(str(5000000 + i) + ',')
    waypointList.write(locChange(wlU_Z[i]))
    waypointList.write(',' + wnU_Z[i] + '\n')
confige.write(str(len(wnA_J)) + ',' + str(len(wnK_T)) + ',' + str(len(wnU_Z)))
confige.write(',' + str(len(airwayContain)) + ',' + str(path))

endTime = time.time()
print(int(endTime - starTime), 's')
airwayList.close()
waypointList.close()
atsList.close()
