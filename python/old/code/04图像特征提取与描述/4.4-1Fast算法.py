'''
FAST(全称Features from accelerated segment test)是一种用于角点检测的算法，
该算法的原理是取图像中检测点，以该点为圆心的周围邻域内像素点判断检测点是否为角点，
通俗的讲就是若一个像素周围有一定数量的像素与该点像素值不同，则认为其为角点。


OpenCV中的FAST检测算法是用传统方法实现的，
1.实例化fast
    fast = =cv.FastFeatureDetector_create( threshold, nonmaxSuppression)
    参数：
        threshold：阈值t，有默认值10
        nonmaxSuppression：是否进行非极大值抑制，默认值True
    返回：
        Fast：创建的FastFeatureDetector对象

2.利用fast.detect检测关键点，没有对应的关键点描述
    kp = fast.detect(grayImg, None)
    参数：
        gray: 进行关键点检测的图像，注意是灰度图像
    返回：
        kp: 关键点信息，包括位置，尺度，方向信息

3.将关键点检测结果绘制在图像上，与在sift中是一样的
    cv.drawKeypoints(image, keypoints, outputimage, color, flags)

Fast算法得出的角点多

'''

import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt

# 1 读取图像
img = cv.imread('../image/tv.jpg')


# 2.fast角点检测
# 2.1 创建一个Fast对象，传入阈值，注意：可以处理彩色空间图像
fast = cv.FastFeatureDetector_create(threshold=30)

# 2.2 检测图像上的关键点
kp = fast.detect(img, None)

# 2.3 在图像上绘制关键点
img2 = cv.drawKeypoints(img, kp, None, color=(0, 0, 255))

# 2.4 输出默认参数
print("Threshold: {}".format(fast.getThreshold()))
print("nonmaxSuppression:{}".format(fast.getNonmaxSuppression()))
print("neighborhood: {}".format(fast.getType()))
print("Total Keypoints with nonmaxSuppression: {}".format(len(kp)))

# 2.5 关闭非极大值抑制
fast.setNonmaxSuppression(0)
kp = fast.detect(img, None)
print("Total Keypoints without nonmaxSuppression: {}".format(len(kp)))

# 2.6 绘制为进行非极大值抑制的结果  加入的效果要好
img3 = cv.drawKeypoints(img, kp, None, color=(0, 0, 255))


# 3 绘制图像
fig, axes = plt.subplots(nrows=1, ncols=3, figsize=(10, 8), dpi=100)
axes[0].imshow(img[:, :, ::-1])
axes[0].set_title("原图")
axes[1].imshow(img2[:, :, ::-1])
axes[1].set_title("加入非极大值抑制")
axes[2].imshow(img3[:, :, ::-1])
axes[2].set_title("未加入非极大值抑制")

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
