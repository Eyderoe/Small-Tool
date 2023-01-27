name = input("file name: ")

a = open(name + ".srt", 'r', encoding='utf-8')
b = open(name + "_reNum.srt", 'w', encoding='utf-8')
lineNum = 0
i = 1


def timeCheck(timeLine):
    spaceLoc = [13, 17, 30]
    ii = 0
    for eachStr in timeLine:
        ii += 1
        if eachStr == ' ' and (ii not in spaceLoc):
            print("error in ",timeLine,end='')


for iLine in a:
    lineNum += 1
    if lineNum % 4 == 1:
        b.write(str(i) + '\n')
        i += 1
        continue
    elif lineNum % 4 == 2:
        b.write(iLine)
        timeCheck(iLine)
        i += 1
        continue
    elif lineNum % 4 == 0:
        b.write('\n')
        continue
    b.write(iLine)
