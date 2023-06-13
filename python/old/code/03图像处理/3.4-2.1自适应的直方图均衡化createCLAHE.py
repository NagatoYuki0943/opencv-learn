'''
OpenCV图像处理-直方图

将图像分成小块,每一个小块进行直方图均衡化,再使用


上述的直方图均衡，我们考虑的是图像的全局对比度。的确在进行完直方图均衡化之后，
图片背景的对比度被改变了，在猫腿这里太暗，我们丢失了很多信息，所以在许多情况下，这样做的效果并不好

为了解决这个问题， 需要使用自适应的直方图均衡化。
此时， 整幅图像会被分成很多小块，这些小块被称为“tiles”（在 OpenCV 中 tiles 的 大小默认是 8x8），
然后再对每一个小块分别进行直方图均衡化。所以在每一个的区域中， 直方图会集中在某一个小的区域中）。
如果有噪声的话，噪声会被放大。为了避免这种情况的出现要使用对比度限制。对于每个小块来说，
如果直方图中的 bin 超过对比度的上限的话，就把 其中的像素点均匀分散到其他 bins 中，然后在进行直方图均衡化。
最后，为了 去除每一个小块之间的边界，再使用双线性差值，对每一小块进行拼接。

cv.createCLAHE(clipLimit, tileGridSize)
参数：
    clipLimit: 对比度限制，默认是40
    tileGridSize: 分块的大小，默认为8∗88*88∗8
'''
import matplotlib.pyplot as plt
import numpy as np
import cv2 as cv

# 1.以灰度图形式读取图像
img = cv.imread("../image/cat.jpeg", 0)

# 2.创建一个自适应均衡化的对象,并应用于图像
# 对比度限制,分块的大小
clahe = cv.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
cl1 = clahe.apply(img)


# 3.图像显示
plt.figure(figsize=(10, 8), dpi=100)
plt.subplot(211), plt.imshow(img, cmap=plt.cm.gray), plt.title('原图')
plt.subplot(212), plt.imshow(cl1, cmap=plt.cm.gray), plt.title('自适应均衡化后的结果')


# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
