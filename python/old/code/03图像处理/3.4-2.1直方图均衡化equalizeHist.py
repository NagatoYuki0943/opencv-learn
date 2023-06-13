'''
OpenCV图像处理-直方图

“直方图均衡化”是把原始图像的灰度直方图从比较集中的某个灰度区间变成在更广泛灰度范围内的分布。
直方图均衡化就是对图像进行非线性拉伸，重新分配图像像素值，使一定灰度范围内的像素数量大致相同。
这种方法提高图像整体的对比度，特别是有用数据的像素值分布比较接近时，
在X光图像中使用广泛，可以提高骨架结构的显示，另外在曝光过度或不足的图像中可以更好的突出细节。


dst = cv.equalizeHist(img)
参数：
    img: 灰度图像
返回：
    dst : 均衡化后的结果
'''

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# 1. 直接以灰度图的方式读入
img = cv.imread('../image/cat.jpeg', 0)


# 2, 均衡化处理
dst = cv.equalizeHist(img)


# 3. 结果展示
fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(10, 8), dpi=100)
axes[0].imshow(img, cmap=plt.cm.gray)
axes[0].set_title("原图")
axes[1].imshow(dst, cmap=plt.cm.gray)
axes[1].set_title("均衡化后结果")


# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
