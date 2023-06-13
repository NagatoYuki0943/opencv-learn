'''
meanshift算法的原理很简单。假设你有一堆点集，还有一个小的窗口，
这个窗口可能是圆形的，现在你可能要移动这个窗口到点集密度最大的区域当中。

在OpenCV中实现Meanshift的API是：
    cv.meanShift(probImage, window, criteria)
参数：
    probImage:  ROI区域，即目标的直方图的反向投影
    window：    初始搜索窗口，就是定义ROI的rect
    criteria:   确定窗口搜索停止的准则，主要有迭代次数达到设置的最大值，窗口中心的漂移值大于某个设定的限值等。
实现Meanshift的主要流程是：
    读取视频文件：cv.videoCapture()
    感兴趣区域设置：获取第一帧图像，并设置目标区域，即感兴趣区域
    计算直方图：计算感兴趣区域的HSV直方图，并进行归一化
    目标追踪：设置窗口搜索停止条件，直方图反向投影，进行目标追踪，并在目标位置绘制矩形框。

'''
import numpy as np
import cv2 as cv

# 1.获取图像
cap = cv.VideoCapture('../video/DOG.wmv')


# 2.获取第一帧图像，并指定目标位置
ret, frame = cap.read()
# 2.1 目标位置(行, 高, 列, 宽)
r, h, c, w = 197, 141, 0, 208
#设置跟踪窗口 列, 行, 宽, 高
track_window = (c, r, w, h)

# 2.2 指定目标的感兴趣区域
# frame是宽和高,这里是两个切片
roi = frame[r:r + h, c:c + w]


# 3.计算直方图
# 3.1 转换色彩空间（HSV）
hsv_roi = cv.cvtColor(roi, cv.COLOR_BGR2HSV)
# 3.2 去除低亮度的值
mask = cv.inRange(hsv_roi, np.array((0., 60., 32.)), np.array((180., 255., 255.)))
# 3.3 计算直方图      mask放在参数3
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
        ret, track_window = cv.meanShift(dst, track_window, term_crit)

        # 4.5 将追踪的位置绘制在视频上，并进行显示
        x, y, w, h = track_window
        img2 = cv.rectangle(frame, (x, y), (x + w, y + h), 255, 2)
        cv.imshow('frame', img2)

        # 每一帧间隔为60ms 且 按了 q 就退出
        if cv.waitKey(60) & 0xFF == ord('q'):
            break

    else:
        break

# 5. 资源释放
cap.release()
cv.destroyAllWindows()
