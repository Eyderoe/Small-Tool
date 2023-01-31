# funcTrail 图像绘制
顺序读取listY数值进行描点作图\
限制：单值函数
> funcTrail a(screen,0) // screen为窗口宽度,1表示采用科学计数法\
> a.load (listX, listY, listLength) // listLength = screen+1 \
> a.show ()
# bccSubtitle Transform 字幕转换
b站自动翻译弹幕转换
> bbc to str: 转换bcc格式到srt\
> reNum: 重新进行字幕编号，并检查时间轴错误
# youtubeSubtitle Transform 字幕转换
油管字幕转换\
需在文件尾手动标注文件尾\
加入了令人振奋的新功能 - 删除重复语段
> youtubeTransform 主程序\
> youtubeDef 文件尾设置及相关函数
