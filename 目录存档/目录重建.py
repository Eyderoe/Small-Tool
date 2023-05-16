import os

fileName = "Word.txt"
rebuildPath = R"F:\test"

file = open(fileName, 'r')
check = 0
timer = 0
for i in file:
    timer += 1
    if timer % 10000 == 0:
        print(timer)
    if "-" in i and check == 0:
        check = 1
    if check == 1 and i == "\n":
        check = 2
        continue

    if check == 2:
        filePath = rebuildPath + "\\" + i[0] + i[2:-1]
        loc = filePath.rfind("\\")
        fileTruePath = filePath[:loc]
        if not os.path.exists(fileTruePath):
            os.makedirs(fileTruePath)
        tempFile = open(rebuildPath + "\\" + i[0] + i[2:-1], 'w')
        tempFile.close()
