def isEmpty(iLine):
    for iWord in iLine:
        if iWord != ' ' and iWord != '\n':
            return 1
    return 0


def contentJudge(line):
    EOFM = "EndedOfFilesMark"
    if isEmpty(line) == 0:  # 空行
        return [0]
    if "-->" in line:  # 时间轴
        timeLine = line[:29]
        timeLine = timeLine.replace('.', ',')
        return [1, timeLine]
    if EOFM in line:  # 文件尾
        return [2]
    if '<' in line:  # 正常时间轴的内容
        contentLine = line[:-1]
        while ('<' in contentLine) and ('>' in contentLine):
            lLoc = contentLine.find('<')
            rLoc = contentLine.find('>')
            contentLine = contentLine[:lLoc] + contentLine[rLoc + 1:]
        contentLine = contentLine[:-3]
        return [3, contentLine]
    else:  # 不正常时间轴的内容
        return [4, line[:-1]]


def deleteSame(old, new):
    minSize = min(len(old), len(new))
    # print("old:<{0}>, new:<{1}> ".format(old,new))
    for i in range(minSize, 0, -1):
        if old[0 - i:] == new[:i]:
            if isEmpty(new[i:]) == 0:   # 内容被删完了
                return -2
            return new[i:]
    return -1
