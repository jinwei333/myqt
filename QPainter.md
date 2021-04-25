QPaintDevice 

The QPaintDevice class is the base class of objects that can be painted on with QPainter

- **QPaintDevice类**表示QPainter的绘图设备(画布)。
- QpaintDevice子类有QImage、QOpenGLPaintDevice、QWidget 等，所以, QPainter可以在QImage、QOpenGLPaintDevice、QWidget上进行绘制图形。
- QPainter只能在类对象的**paintEvent()函数**中绘制图形。



**QPainter类的成员角色有:**

- **QPen**     : 用于绘制几何图形的边缘,由颜色,宽度,线风格等参数组成
- **QBrush**   : 用于填充几何图形的调色板,由颜色和填充风格组成
- **QFont**    : 用于文本绘制
- **QPixmap** : 绘制图片,可以加速显示,带有屏幕截图,窗口截图等支持,适合小图片
- **QImage**   : 绘制图片,可以直接读取图像文件进行像素访问,适合大图片
- **QBitmap** : QPixmap的一个子类，主要用于显示单色位图
- **QPicture** : 绘图装置，用于记录和重播 QPainter 的绘图指令

 **逻辑坐标（窗口）**

编程用的是逻辑坐标



setWindow(10, 10, 400, 400);

“将逻辑坐标相应的部位（QRect（10,10,400,400））映射到绘图设备

 **物理坐标（视口）**

