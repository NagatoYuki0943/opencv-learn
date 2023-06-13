'''
OpenCV图像处理-直方图

cv2.calcHist(images,channels,mask,histSize,ranges[,hist[,accumulate]])
    images: 原图像。当传入函数时应该用中括号 [] 括起来，例如：[img]。
    channels: 如果输入图像是灰度图，它的值就是 [0]；如果是彩色图像的话，传入的参数可以是 [0]，[1]，[2] 它们分别对应着通道 B，G，R。 　　
    mask: 掩模图像。要统计整幅图像的直方图就把它设为 None。
        但是如果你想统计图像某一部分的直方图的话，你就需要制作一个掩模图像，并使用它。（后边有例子） 　　
    histSize:BIN 的数目。也应该用中括号括起来，例如：[256]。 　　
    ranges: 像素值范围，通常为 [0，256]

'''

import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt


# 1 直接以灰度图的方式读入
img = cv.imread('../image/cat.jpeg', 0)


# 2. 统计直方图
# images: 原图像。当传入函数时应该用中括号 [] 括起来，例如：[img]。
# channels: 如果输入图像是灰度图，它的值就是 [0]；如果是彩色图像的话，传入的参数可以是 [0]，[1]，[2] 它们分别对应着通道 B，G，R。 　　
# mask: 掩模图像。要统计整幅图像的直方图就把它设为 None。
#     但是如果你想统计图像某一部分的直方图的话，你就需要制作一个掩模图像，并使用它。（后边有例子） 　　
# histSize:BIN的数目。也应该用中括号括起来，例如：[256]。 　　
# ranges: 像素值范围，通常为 [0，256]
histr1 = cv.calcHist([img], [0], None, [256], [0, 256])

# 一次只能有一个直方图
# histr2 = cv.calcHist([img], [1], None, [256], [0, 256])
# histr3 = cv.calcHist([img], [2], None, [256], [0, 256])


# 3.绘制灰度图
plt.figure(figsize=(10, 5), dpi=100)
# plot 折线图
plt.plot(histr1),plt.grid()
plt.show()

