'''
小尺度检测角点,大尺度不能检测

Shi-Tomasi算法是对Harris角点检测算法的改进，一般会比Harris算法得到更好的角点。Harris 算法的角点响应函数是将矩阵 M 的行列式值与 M 的迹相减，利用差值判断是否为角点。

在OpenCV中实现Shi-Tomasi角点检测使用API:
    corners = cv2.goodFeaturesToTrack ( image, maxcorners, qualityLevel, minDistance )
参数：
    Image: 输入灰度图像
    maxCorners : 获取角点数的数目。
    qualityLevel：该参数指出最低可接受的角点质量水平，在0-1之间。
    minDistance：角点之间最小的欧式距离，避免得到相邻特征点。
返回值:


'''

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# 1.读取图像
img = cv.imread('../image/tv.jpg')
# 1.1转化为灰度图像
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)


# 2.角点检测
# 图像, 角点数目, 最低可接受的角点质量水平, 角点之间最小的欧式距离
corners = cv.goodFeaturesToTrack(gray, 1000, 0.01, 10)


# 3.绘制角点
for i in corners:
    x, y = i.ravel()
    cv.circle(img, (x, y), 2, (0, 0, 255), 1)


# 4.图像显示
plt.figure(figsize=(10, 8),dpi=100)
plt.imshow(img[:, :, ::-1]),plt.title('shi-tomasi角点检测')
plt.xticks([]), plt.yticks([])  # 这一行可以不写

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
