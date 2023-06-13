'''
保存视频
    保存视频：
        out = cv.VideoWrite()
    视频写入：
        out.write()
    资源释放：
        out.release()

# 在OpenCV中我们保存视频使用的是VedioWriter对象，在其中指定输出文件的名称，如下所示：

1.创建视频写入的对象
    out = cv2.VideoWriter(filename,fourcc, fps, frameSize)
    参数：
        filename：视频保存的位置
        fourcc：指定视频编解码器的4字节代码
        fps：帧率
        frameSize：帧大小


fourcc 设置视频的编解码器，如下所示，
    cv2.VideoWriter_fourcc( c1, c2, c3, c4 )
# 参数：
    c1,c2,c3,c4: 是视频编解码器的4字节代码，在fourcc.org中找到可用代码列表，与平台紧密相关，常用的有：
    在Windows中：DIVX（.avi）
    在OS中：MJPG（.mp4），DIVX（.avi），X264（.mkv）。
    利用cap.read()获取视频中的每一帧图像，并使用out.write()将某一帧图像写入视频中。
    使用cap.release()和out.release()释放资源。
'''
import cv2 as cv
import numpy as np

# 1.读取视频
cap = cv.VideoCapture("../video/DOG.wmv")


# 2.获取图像的属性（宽和高，）,并将其转换为整数
frame_width = int(cap.get(3))
frame_height = int(cap.get(4))
print(frame_width, frame_height)  # 444 480


# 3. 创建保存视频的对象，设置编码格式，帧率，图像的宽高等
# 写入位置;  编码:MJPG（.mp4）;  帧率;  宽高
out = cv.VideoWriter('output.mp4', cv.VideoWriter_fourcc('M', 'J', 'P', 'G'), 10, (frame_width, frame_height))
while (True):
    # 4.获取视频中的每一帧图像
    ret, frame = cap.read()

    if ret == True:
        # 5.将每一帧图像写入到输出文件中
        out.write(frame)
    else:
        break


# 6.释放资源
cap.release()
out.release()
cv.destroyAllWindows()