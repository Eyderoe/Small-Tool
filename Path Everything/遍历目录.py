import os
import time
import psutil

txt = open('Word.txt', mode='w')
localtime = time.asctime(time.localtime(time.time()))
txt.write(localtime)
txt.write('\n')
txt.write('\n')
# disk = 'QWERTYUIOPASDFGHJKLZXCVBNM'
disk = "EF"

for i in disk:
    txt.write("Disk " + str(i) + "\n")
    i += ":"
    disk_usage = psutil.disk_usage(i)
    total_space = disk_usage.total
    used_space = disk_usage.used
    free_space = disk_usage.free
    perc_space = disk_usage.percent
    txt.write("Total: " + str(round(total_space / 10 ** 9, 2)) + ' G\n')
    txt.write("Used: " + str(round(used_space / 10 ** 9, 2)) + ' G\n')
    txt.write("Free: " + str(round(free_space / 10 ** 9, 2)) + ' G\n')
    txt.write("Percent Use: " + str(perc_space) + ' %\n')
    txt.write("-------------------------------\n")
txt.write('\n')

for i in disk:
    i = i + ':\\'
    for root, dirs, files in os.walk(i):
        for file in files:
            # 获取文件所属目录
            # print(root)
            # 获取文件路径
            try:
                txt.write(os.path.join(root, file))
                txt.write('\n')
            except:
                pass
