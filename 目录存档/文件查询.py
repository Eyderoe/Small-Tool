import os


def search_and_store(name, search, store, what):
    for i in search:
        if searchMode == 1:  # 全部匹配
            check = 1
            for j in what:
                if j.lower() not in i.lower():
                    check = 0
                    break
            if check == 1:
                print(i)
                store.write(name + "   " + i)
        if searchMode == 2:  # 全部匹配
            check = 0
            for j in what:
                if j.lower() in i.lower():
                    check = 1
                    break
            if check == 1:
                store.write(name + "   " + i)


searchWhat = ["くいろ"]
searchMode = 1
resultFile = open("search result.txt", 'w')

dir_path = os.getcwd()
for file_name in os.listdir(dir_path):
    if os.path.isfile(os.path.join(dir_path, file_name)):
        if ".txt" in file_name:
            searchFile = open(file_name, 'r')
            search_and_store(file_name[:-4], searchFile, resultFile, searchWhat)
            searchFile.close()
resultFile.close()
