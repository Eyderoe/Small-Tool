# **ComplexNum-Impedance**
a class for calculate complex num and impedance in circuit  
#### two declare ways
>``virnum(2,-7)`` _means 2-7i_\
>``virnum('c',20,'p',10,'k')`` _means 20pF capacitor in 10K(Hz) circuit_\
>``virnum('r',5.1,'1')`` _means 5.1ohm resistor_
#### basic operation
``virnum a(1,2)``
> ``+ - * / Im Re ln() exp()``
#### other functions
``virnum a(1,2)``\
``virnum b('l',20,'u',10,'k')``
>``a.conjugate()`` _will return 1-2i_\
>``a.getLength()`` , ``a.getAngle()`` , ``a.getRadAngle()``\
>``a.quichFactorial(long long)`` _loss precision_\
>``b.changeFreq(12,'k')`` _change frequency to 12K(Hz)_\
>``b.changeFreq()`` _only available on capacitor and inductor_
# **funcTrail 图像绘制**
顺序读取listY数值进行描点作图\
限制：单值函数
> funcTrail a(screen,0) // screen为窗口宽度,1表示采用科学计数法\
> a.load (listX, listY, listLength) // listLength = screen+1 \
> a.show ()
# **pyFile 文件操作**
仿python文件操作\
摆烂ing
# **Logic Gate**
逻辑门类，能够处理数值变动，断开连接
#### 支持类型
> ``单输入`` 0导线 3非\
> ``双输入`` 1或 2与 4或非 5与非 6异或 7同或\
> ``四输入`` 8与或非 9与或 10或与 11与非-与非 12或非-或非\
> ``自定义`` 其他类型逻辑门
#### 操作
>``getOutput ()`` 获取输出值\
>``setInput (int pin, bool value)`` 输入值\
>``setInput (int pin, logicGate *frombool, bool isOpposite=false)`` 连接到其他元件的输出
# **helang**
何言何语 powerCon(a, b) 
> ``powerCon(1|5| 89 |21,5)``
# **massPoint**
项 目 废掉 啦~~~~\
质点间万有引力模拟\
massPoint逻辑精度损失 massPoint_newShift不可修复的错误

