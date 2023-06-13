'''
大小尺度都能找到

SIFT和SURF算法是受专利保护的，在使用他们时我们是要付费的，但是ORB（Oriented Fast and Rotated Brief）不需要，
它可以用来对图像中的关键点快速创建特征向量，并用这些特征向量来识别图像中的对象。

SIFT算法的实质是在不同的尺度空间上查找关键点(特征点)，并计算出关键点的方向。
SIFT所查找到的关键点是一些十分突出，不会因光照，仿射变换和噪音等因素而变化的点，如角点、边缘点、暗区的亮点及亮区的暗点等。

cv.drawKeypoints(image, keypoints, outputimage, color, flags)
参数：
    image: 原始图像
    keypoints：关键点信息，将其绘制在图像上
    outputimage：输出图片，可以是原始图像
    color：颜色设置，通过修改（b,g,r）的值,更改画笔的颜色，b=蓝色，g=绿色，r=红色。
    flags：绘图功能的标识设置
        cv2.DRAW_MATCHES_FLAGS_DEFAULT：创建输出图像矩阵，使用现存的输出图像绘制匹配对和特征点，对每一个关键点只绘制中间点
        cv2.DRAW_MATCHES_FLAGS_DRAW_OVER_OUTIMG：不创建输出图像矩阵，而是在输出图像上绘制匹配对
        cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS：对每一个特征点绘制带大小和方向的关键点图形
        cv2.DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS：单点的特征点不被绘制

SURF算法：
    对SIFT算法的改进，在尺度空间极值检测，关键点方向确定，关键点描述方面都有改进，提高效率
'''

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# 1.读取图像
img = cv.imread('../image/tv.jpg')
# 1.1转化为灰度图像
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)


# 2.sift关键字检测
# 2.1 实例化sift对象
# 这个版本没有这个方法
sift = cv.xfeatures2d.SIFT_create()

# 2.2 关键点检测：kp关键点信息包括方向，尺度，位置信息，des是关键点的描述符
kp,des = sift.detectAndCompute(gray,None)

# 2.3 在图像上绘制关键点的检测结果
cv.drawKeypoints(img, kp, img, flags=cv.DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS)

# 3 图像显示
plt.figure(figsize=(8,6),dpi=100)
plt.imshow(img[:,:,::-1]),plt.title('sift检测')
# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()

