import PyPDF2

# 定义源文件和目标文件
source_file = open('before.pdf', 'rb')

# 将源文件读入pdfReader对象中
pdfReader = PyPDF2.PdfReader(source_file)

cutNum = 2
outputFileName = "Tutorial"
allPage = len(pdfReader.pages)
eachCut = allPage // cutNum
# 很脑瘫但有效的写法
pages = []
timer = 0
for i in range(cutNum):
    a = []
    for j in range(eachCut):
        a.append(j + timer * eachCut)
    pages.append(a.copy())
    timer += 1
for i in range(allPage - allPage % eachCut, allPage):
    pages[timer - 1].append(i)

for i in range(cutNum):
    output_file = open(outputFileName + "_" + str(i) + ".pdf", 'wb')
    # 创建一个pdfWriter对象，将页面合并到新文件中
    pdfWriter = PyPDF2.PdfWriter()

    # 遍历每个页码
    for pageNum in pages[i]:
        pageObject = pdfReader.pages[pageNum]
        pdfWriter.add_page(pageObject)

    # 将pdfWriter对象写入到新文件中
    pdfWriter.write(output_file)
    output_file.close()
source_file.close()
