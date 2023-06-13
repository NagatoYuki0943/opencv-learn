'''
小尺度检测角点,大尺度不能检测

Harris角点检测的思想是通过图像的局部的小窗口观察图像，角点的特征是窗口沿任意方向移动都会导致图像灰度的明显变化

在OpenCV中实现Hariis检测使用的API是：
    dst=cv.cornerHarris(src, blockSize, ksize, k)
参数：
    img：数据类型为 ﬂoat32 的输入图像。
    blockSize：角点检测中要考虑的邻域大小。
    ksize：sobel求导使用的核大小
    k ：角点检测方程中的自由参数，取值参数为 [0.04，0.06].
返回值:


'''

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# 1.读取图像
img = cv.imread('../image/chessboard.jpg')
# 2.1转化为灰度图像
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)


# 2.角点检测
# 2.1输入的图像必须是float32
gray = np.float32(gray)

# 图像, 角点邻域大小, sober求导核大小, 点检测方程中的自由参数 0.04~0.05之间
dst = cv.cornerHarris(gray, 2, 3, 0.04)
print(dst)


# 3.设置阈值,将角点绘制出来,阈值根据图像进行选择
# 获取坐标,改变颜色
img[dst > 0.001 * dst.max()] = [0, 0, 255]


# 4.图像显示
plt.imshow(img[:, :, ::-1]), plt.title("Harris角点检测")

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
