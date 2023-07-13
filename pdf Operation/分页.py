import PyPDF2


def determine_page(list1, list2, max):
    for i in list1:
        if '-' not in str(i):
            if i < max:
                list2.append(i - 1)
        else:
            ranger = i.split('-')
            for j in range(int(ranger[0]), int(ranger[1]) + 1):
                if j < max:
                    list2.append(j - 1)


# 定义源文件和目标文件
source_file = open('fcom.pdf', 'rb')
output_file = open('test.pdf', 'wb')

# 读取
pdfReader = PyPDF2.PdfReader(source_file)
pageMax = len(pdfReader.pages)
aim = [6643, 7036, 6654, 7036, 6756, "6767-6768", 7036, 6898, 7036, 6035, 7036, "6683-6687", 7036, 6635, 7036,
       "6823-6824"]
actual = []
determine_page(aim, actual, pageMax)

# 写入
pdfWriter = PyPDF2.PdfWriter()
for pageNum in actual:
    pageObject = pdfReader.pages[pageNum]
    pdfWriter.add_page(pageObject)
pdfWriter.write(output_file)

# 关闭文件
output_file.close()
source_file.close()
