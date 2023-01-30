import youtubeDef as utube

# isEmpty 查看一行是否为空
# contentJudge 判断语句类型

originFile = open('test.txt', 'r')
srtFile = open('trans.srt', 'w')
timeLineTimes = 0  # 一个时间轴正常，一个字幕正常
i = 1
timeValue = ''
contentValue = ''
oldContentValue = ''
oldStr = ' '
while True:
    Line = originFile.readline()
    returnValue = utube.contentJudge(Line)
    # print(returnValue, timeLineTimes)
    if returnValue[0] == 2:
        break
    if returnValue[0] == 1:
        timeLineTimes += 1
    if returnValue[0] == 1 and timeLineTimes % 2 == 1:  # 一节字幕,时间轴第一次
        if timeLineTimes != 1:  # 把上一个给写入了
            if utube.isEmpty(contentValue) == 0:  # 空语句情况
                continue

            alreadyDel = utube.deleteSame(oldStr, contentValue)
            # print("old:<{0}>, new:<{1}> ".format(oldStr, contentValue))
            # print(alreadyDel)
            # -1表示不存在重复字段 -2表示两行完全一致 正常返回裁剪后字符串
            if alreadyDel == -2:
                continue
            elif alreadyDel == -1:
                oldStr = contentValue
            else:
                contentValue = alreadyDel
                oldStr = contentValue

            srtFile.write(str(i) + '\n')
            srtFile.write(timeValue + '\n')
            srtFile.write(contentValue + "\n\n")
            i += 1
        timeValue = returnValue[1]
        contentValue = ''
    if returnValue[0] == 4:
        contentValue += returnValue[1]
originFile.close()
srtFile.close()
