'''
OpenCV图像处理-直方图

掩膜是用选定的图像、图形或物体，对要处理的图像进行遮挡，来控制图像 处理的区域。

'''

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# 1. 直接以灰度图的方式读入
img = cv.imread('../image/cat.jpeg', 0)

# 2. 创建蒙版
# 全黑色          大小相同
mask = np.zeros(img.shape[:2], np.uint8)
# 创建白色区域(透明)
mask[400:650, 200:500] = 255

# 3.掩模
masked_img = cv.bitwise_and(img, img, mask=mask)

# 4. 统计掩膜后图像的灰度图
# 原图, brg对应012, 掩膜, histSiz:BIN的数目, ranges:像素值范围
mask_histr = cv.calcHist([img], [0], mask, [256], [1, 256])


# 5. 图像展示
fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))
axes[0, 0].imshow(img, cmap=plt.cm.gray)
axes[0, 0].set_title("原图")
axes[0, 1].imshow(mask, cmap=plt.cm.gray)
axes[0, 1].set_title("蒙版数据")
axes[1, 0].imshow(masked_img, cmap=plt.cm.gray)
axes[1, 0].set_title("掩膜后数据")
# 折线图
axes[1, 1].plot(mask_histr)
axes[1, 1].grid()
axes[1, 1].set_title("灰度直方图")

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
