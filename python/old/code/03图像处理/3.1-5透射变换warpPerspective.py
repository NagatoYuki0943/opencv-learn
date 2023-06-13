'''
OpenCV图像处理-几何变换

透射变换是视角变化的结果，是指利用透视中心、像点、目标点三点共线的条件，
按透视旋转定律使承影面（透视面）绕迹线（透视轴）旋转某一角度，破坏原有的投影光线束，
仍能保持承影面上投影几何图形不变的变换。

在opencv中，我们要找到四个点，其中任意三个不共线，然后获取变换矩阵T，再进行透射变换。
通过函数cv.getPerspectiveTransform找到变换矩阵，将cv.warpPerspective应用于此3x3变换矩阵。

'''

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# 1 读取图像
img = cv.imread("../image/image2.jpg")


# 2 透射变换
rows, cols = img.shape[:2]
# 2.1 创建变换矩阵
# 原始点
pts1 = np.float32([[56, 65], [368, 52], [28, 387], [389, 390]])
# 目标点
pts2 = np.float32([[100, 145], [300, 100], [80, 290], [310, 300]])
# 找到变换矩阵
T = cv.getPerspectiveTransform(pts1, pts2)

# 2.2 进行变换
dst = cv.warpPerspective(img, T, (cols, rows))


# 3 图像显示
plt.figure(figsize=(10, 8), dpi=100)
plt.subplot(121), plt.imshow(img[:, :, ::-1]), plt.title("原图")
plt.subplot(122), plt.imshow(dst[:, :, ::-1]), plt.title("透射后结果")

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
