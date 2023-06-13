'''
读取视频：
    读取视频:
        cap = cv.VideoCapture()

    判断读取成功:
        cap.isOpened()

    读取每一帧图像:
        ret,frame = cap.read()
           索引  flags                       意义
            0   cv2.CAP_PROP_POS_MSEC       视频文件的当前位置(ms)
            1   cv2.CAP_PROP_POS_FRAMES     从0开始索引帧,帧位置
            2   cv2.CAP_PROP_POS_AVI_RATIO  视频文件的相对位置(0表示开始,1表示结束
            3   cv2.CAP_PROP_FRAME_WIDTH    视频流的帧宽度
            4   cv2.CAP_PROP_FRAME_HEIGHT   视频流的帧高度
            5   cv2.CAP_PROP_FPS            帧率
            6   cv2.CAP_PROP_FOURCC         编解码器四字符代码
            7   cv2.CAP_PROP_FRAME_COUNT    视频文件的帧
    播放视频通过 imshow('name', frame)

    获取属性:
        cap.get(proid)

    设置属性:
        cap.set(proid,value)

    资源释放:
        cap.release()
'''

import numpy as np
import cv2 as cv

# 1.创建读取视频的对象
cap = cv.VideoCapture("../video/DOG.wmv")


# 2.获取视频的某些属性
# 参数：
# propId: 从0到18的数字，每个数字表示视频的属性
# 常用属性有：
#  索引  flags                       意义
#   0   cv2.CAP_PROP_POS_MSEC       视频文件的当前位置(ms)
#   1   cv2.CAP_PROP_POS_FRAMES     从0开始索引帧,帧位置
#   2   cv2.CAP_PROP_POS_AVI_RATIO  视频文件的相对位置(0表示开始,1表示结束
#   3   cv2.CAP_PROP_FRAME_WIDTH    视频流的帧宽度
#   4   cv2.CAP_PROP_FRAME_HEIGHT   视频流的帧高度
#   5   cv2.CAP_PROP_FPS            帧率
#   6   cv2.CAP_PROP_FOURCC         编解码器四字符代码
#   7   cv2.CAP_PROP_FRAME_COUNT    视频文件的帧

fps = cap.get(5)
print(fps)  # 10.0

# 2.2 修改视频的属性信息
# 参数：
#     proid: 属性的索引，与上面的表格相对应
#     value: 修改后的属性值
# cap.set(5, 30)

print(cap.isOpened())  # True


# 3.判断图像是否读取成功
while (cap.isOpened()):
    # 4.获取视频的一帧图像
    # 参数：
    #     ret: 若获取成功返回True，获取失败，返回False
    #     Frame: 获取到的某一帧的图像
    ret, frame = cap.read()

    # 5.调用cv.imshow()显示图像，在显示图像时使用cv.waitkey()设置适当的持续时间，
    # 如果太低视频会播放的非常快，如果太高就会播放的非常慢，通常情况下我们设置25ms就可以了。
    if ret == True:
        cv.imshow('frame', frame)

    # 5.每一帧间隔为25ms 且 按了 q 就退出
    if cv.waitKey(25) & 0xFF == ord('q'):
        break


# 6.释放视频对象
cap.release()
cv.destoryAllwindows()
