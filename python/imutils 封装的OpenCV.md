# 2.2. imutils

**学习目标**

- 了解imutils的功能
- 知道imutils的常见用法

------

# 1.imutils功能简介

imutils是在OPenCV基础上的一个封装，达到更为简结的调用OPenCV接口的目的，它可以轻松的实现图像的平移，旋转，缩放，骨架化等一系列的操作。

安装方法：

```
pip install imutils
```

在安装前应确认已安装numpy,scipy,matplotlib和opencv。

# 2.imutils的使用方法

## 2.1 图像平移 translate

OpenCV中也提供了图像平移的实现，要先计算平移矩阵，然后利用仿射变换实现平移，在imutils中可直接进行图像的平移。

```python
translated = imutils.translate(img,x,y)
```

参数：

- img:要移动的图像
- x:沿x轴方向移动的像素个数
- y: 沿y轴方向移动的像素个数

```python
import cv2 as cv
import imutils 

# 读取通道是 BGR
image = cv.imread('image/001.jpg')

translate = imutils.translate(image, 100, 50)

plt.figure()
plt.subplot(1, 2, 1)
#  BGR -> RGB
plt.imshow(image[:, :,::-1])
plt.title("原图")

plt.subplot(1, 2, 2)
#  BGR -> RGB
plt.imshow(translate[:, :,::-1])
plt.title("平移后")

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
# 3.显示图像
plt.show()
```

![image-20210720163659582](2.2 imutils.assets/image-20210720163659582.png)

## 2.2 图像缩放 resize

图片的缩放在OPenCV中要注意确保保持宽高比。而在imutils中自动保持原有图片的宽高比，只指定宽度weight或Height即可。

```python
resize = imutils.resize(img, height, width)
```

参数：

- img: 要缩放的图像
- height: 宽度
- width: 高度

```python
import cv2 as cv
import imutils 
import matplotlib.pyplot as plt

image = cv.imread('image/002.jpg')
image = cv.cvtColor(image, cv.COLOR_BGR2RGB)
image.shape
# (534, 540, 3)

resize = imutils.resize(image, height=300)
resize.shape
# (300, 303, 3)

fig, axes = plt.subplots(ncols=2, nrows=1)

axes[0].imshow(image)
axes[0].set_title('原图')

axes[1].imshow(resize)
axes[1].set_title('缩放')

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
# 3.显示图像
plt.show()
```

下图是对图像进行缩放后的结果：

![image-20210720165132093](2.2 imutils.assets/image-20210720165132093.png)



## 2.3 图像旋转 rotate rotate_round

在OpenCV中进行旋转时使用的是仿射变换，在这里图像旋转方法是`imutils.rotate()`，跟2个参数，第一个是图片数据，第二个是旋转的角度，旋转是朝逆时针方向。同时`imutils`还提供了另一个相似的方法, `rotate_round()`，它就是按顺时针旋转的。

```python
rotate = imutils.rotate(image, 90)  
rotate = imutils.rotate_round(image, 90)  
```

参数：

- img: 要旋转的图像
- 角度: 逆时针角度rotate   顺时针角度rotate_round





```python
import cv2 as cv
import imutils 
import matplotlib.pyplot as plt

image = cv.imread('image/003.jpg')
image = cv.cvtColor(image, cv.COLOR_BGR2RGB)

# 逆时针旋转
rotate = imutils.rotate(image, 90)  
# 顺时针旋转
rotate_round = imutils.rotate_bound(image, 90)

fig, axes = plt.subplots(ncols=3, nrows=1,figsize=[10, 10])

axes[0].imshow(image)
axes[0].set_title('原图')

axes[1].imshow(rotate)
axes[1].set_title('逆时针90')

axes[2].imshow(rotate_round)
axes[2].set_title('顺时针90')

# 3.显示图像
plt.show()
```

结果如下：

![image-20210720165759889](2.2 imutils.assets/image-20210720165759889.png)



## 2.4 骨架提取 skeletonize

骨架提取，是指对图片中的物体进行拓扑骨架(topological  skeleton)构建的过程。imutils提供的方法是skeletonize()，第二个参数是结构参数的尺寸(structuring  element)，相当于是一个粒度，越小需要处理的时间越长。

```python
skeleton = imutils.skeletonize(gray, size=(3, 3))  
```



参数：

- img: 要提取的图像
- size: 第二个参数是结构参数的尺寸(structuring  element)，相当于是一个粒度，越小需要处理的时间越长



```python
import cv2 as cv
import imutils 
import matplotlib.pyplot as plt

# 1 图像读取
image = cv.imread('./image/004.jpg')

# 2 灰度化
gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)

# 3 骨架提取
skeleton  = imutils.skeletonize(gray, size=(3, 3))

# 4 图像展示
fig, axes = plt.subplots(ncols=3, nrows=1, figsize=(10, 8), dpi=100)

axes[0].imshow(image)
axes[0].set_title('原图')

axes[1].imshow(gray, cmap='gray')
axes[1].set_title('灰度图')

axes[2].imshow(skeleton, cmap='gray')
axes[2].set_title('骨架提取')

plt.show()
```

效果如下：

![image-20210720170909243](2.2 imutils 封装的OpenCV.assets/image-20210720170909243.png)





## 2.5 Matplotlib显示 imutils.opencv2matplotlib

在OpenCV的Python绑定中，图像以BGR顺序表示为NumPy数组。使用该`cv2.imshow`功能时效果很好。但是，如果打算使用Matplotlib，该`plt.imshow`函数将假定图像按RGB顺序排列。调用`cv2.cvtColor`解决此问题，也可以使用`opencv2matplotlib`便捷功能。

```python
img = cv.imread("lion.jpeg")
plt.figure()
plt.imshow(imutils.opencv2matplotlib(img))
```

## 2.5 OpenCV版本的检测

OpenCV 4发布之后，随着主要版本的更新，向后兼容性问题尤为突出。在使用OPenCV时，应检查当前正在使用哪个版本的OpenCV，然后使用适当的函数或方法。在imutils中的`is_cv2()、is_cv3()和is_cv4()`是可用于自动确定当前环境的OpenCV的版本简单的功能。

```python
print("OPenCV版本: {}".format(cv2.__version__))
print("OPenCV是2.X? {}".format(imutils.is_cv2()))
print("OPenCV是3.X? {}".format(imutils.is_cv3()))
print("OPenCV是4.X? {}".format(imutils.is_cv4()))
```

输出是： ![image-20191218174015535](2.2 imutils.assets/image-20191218174015535.png)

------

**总结**

1. imutils是基于OPenCV的便利的图像处理工具包
2. 使用imutils可以轻松的完成图像平移，缩放，旋转，骨架提取等功能