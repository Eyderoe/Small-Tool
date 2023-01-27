# funcTrail 函数图像绘制
基本原理：依次读取listY数值进行打点与插值。\
限制：单值函数
> funcTrail a(screen,0) // screen为窗口宽度,1表示采用国际单位制\
a.load (listX, listY, listLength) // listLength最好为 screen+1 \
a.show ()
