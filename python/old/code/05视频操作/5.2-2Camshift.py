'''
大家认真看下上面的结果，有一个问题，就是检测的窗口的大小是固定的，
而狗狗由近及远是一个逐渐变小的过程，固定的窗口是不合适的。
所以我们需要根据目标的大小和角度来对窗口的大小和角度进行修正。CamShift可以帮我们解决这个问题。

CamShift算法全称是“Continuously Adaptive Mean-Shift”（连续自适应MeanShift算法），
是对MeanShift算法的改进算法，可随着跟踪目标的大小变化实时调整搜索窗口的大小，具有较好的跟踪效果。

Camshift算法首先应用meanshift，一旦meanshift收敛，它就会更新窗口的大小，
还计算最佳拟合椭圆的方向，从而根据目标的位置和大小更新搜索窗口。如下图所示：

Meanshift和camshift算法都各有优势，自然也有劣势：
    Meanshift算法：简单，迭代次数少，但无法解决目标的遮挡问题并且不能适应运动目标的的形状和大小变化。

    camshift算法：可适应运动目标的大小形状的改变，具有较好的跟踪效果，但当背景色和目标颜色接近时，
    容易使目标的区域变大，最终有可能导致目标跟踪丢失。


总结
    meanshift
        原理：一个迭代的步骤，即先算出当前点的偏移均值，移动该点到其偏移均值，然后以此为新的起始点，继续移动，直到满足一定的条件结束。
            API：cv.meanshift()

        优缺点：简单，迭代次数少，但无法解决目标的遮挡问题并且不能适应运动目标的的形状和大小变化

    camshift
        原理：对meanshift算法的改进，首先应用meanshift，一旦meanshift收敛，它就会更新窗口的大小，
            还计算最佳拟合椭圆的方向，从而根据目标的位置和大小更新搜索窗口。
        API：cv.camshift()

        优缺点：可适应运动目标的大小形状的改变，具有较好的跟踪效果，但当背景色和目标颜色接近时，
            容易使目标的区域变大，最终有可能导致目标跟踪丢失

'''

import numpy as np
import cv2 as cv

# 1.获取图像
cap = cv.VideoCapture('../video/DOG.wmv')


# 2.获取第一帧图像，并指定目标位置
ret, frame = cap.read()
# 2.1 目标位置(行, 高, 列, 宽)
r, h, c, w = 197, 141, 0, 208
track_window = (c, r, w, h)

# 2.2 指定目标的感兴趣区域
roi = frame[r:r + h, c:c + w]


# 3.计算直方图
# 3.1 转换色彩空间（HSV）
hsv_roi = cv.cvtColor(roi, cv.COLOR_BGR2HSV)
# 3.2 去除低亮度的值
mask = cv.inRange(hsv_roi, np.array((0., 60., 32.)), np.array((180., 255., 255.)))
# 3.3 计算直方图
roi_hist = cv.calcHist([hsv_roi], [0], None, [180], [0, 180])
# 3.4 归一化
cv.normalize(roi_hist, roi_hist, 0, 255, cv.NORM_MINMAX)


# 4. 目标追踪
# 4.1 设置窗口搜索终止条件：最大迭代次数，窗口中心漂移最小值
term_crit = (cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 1)

while True:
    # 4.2 获取每一帧图像
    ret, frame = cap.read()
    if ret == True:
        # 4.3 计算直方图的反向投影
        hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
        dst = cv.calcBackProject([hsv], [0], roi_hist, [0, 180], 1)

        # 4.4 进行meanshift追踪
        ret, track_window = cv.CamShift(dst, track_window, term_crit)

        # 4.5 将追踪的位置绘制在视频上，并进行显示
        # 绘制追踪结果
        pts = cv.boxPoints(ret)
        pts = np.int0(pts)
        img2 = cv.polylines(frame, [pts], True, 255, 2)
        cv.imshow('frame', img2)

        # 每一帧间隔为60ms 且 按了 q 就退出
        if cv.waitKey(60) & 0xFF == ord('q'):
            break

    else:
        break


# 5. 资源释放
cap.release()
cv.destroyAllWindows()
