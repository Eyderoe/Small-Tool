import PyPDF2

# 定义源文件和目标文件
source_file = open('before.pdf', 'rb')
output_file = open('after.pdf', 'wb')

# 将源文件读入pdfReader对象中
pdfReader = PyPDF2.PdfReader (source_file)

# 创建一个pdfWriter对象，将页面合并到新文件中
pdfWriter = PyPDF2.PdfWriter()

# 遍历每个页码，并进行操作
for pageNum in range(len(pdfReader.pages)):
    # 检查当前页码是否为奇数
    if (pageNum+1) % 2 != 0:
        # 获取当前页对象，并将所有奇数页旋转270度
        pageObj = pdfReader.pages[pageNum]
        pageObj.rotate(270)
        # 将操作后的页对象添加到pdfWriter对象中
        pdfWriter.add_page(pageObj)
    else:
        pageObj = pdfReader.pages[pageNum]
        pageObj.rotate(90)
        pdfWriter.add_page(pageObj)

# 将操作后的pdfWriter对象写入到新文件中
pdfWriter.write(output_file)

# 关闭文件
output_file.close()
source_file.close()
