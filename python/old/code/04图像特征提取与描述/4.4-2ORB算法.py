'''
SIFT和SURF算法是受专利保护的，在使用他们时我们是要付费的，但是ORB（Oriented Fast and Rotated Brief）不需要，
它可以用来对图像中的关键点快速创建特征向量，并用这些特征向量来识别图像中的对象。

在OPenCV中实现ORB算法，使用的是：
1.实例化ORB
    orb = cv.xfeatures2d.orb_create(nfeatures)
    参数：
        nfeatures: 特征点的最大数量

2.利用orb.detectAndCompute()检测关键点并计算
    kp,des = orb.detectAndCompute(gray,None)
    参数：
        gray: 进行关键点检测的图像，注意是灰度图像
    返回：
        kp: 关键点信息，包括位置，尺度，方向信息
        des: 关键点描述符，每个关键点BRIEF特征向量，二进制字符串，

3.将关键点检测结果绘制在图像上
    cv.drawKeypoints(image, keypoints, outputimage, color, flags)
'''

import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt

# 1 读取图像
img = cv.imread('../image/tv.jpg')

# 2 ORB角点检测
# 2.1 实例化ORB对象
# 参数检测角点最大数量
orb = cv.ORB_create(nfeatures=1000)

# 2.2 检测关键点,并计算特征描述符
kp, des = orb.detectAndCompute(img, None)
print(des.shape)    # (1000, 32)


# 3 将关键点绘制在图像上
img2 = cv.drawKeypoints(img, kp, None, color=(0, 0, 255), flags=0)


# 4. 绘制图像
plt.figure(figsize=(10, 8), dpi=100)
plt.imshow(img2[:, :, ::-1])
plt.show()
