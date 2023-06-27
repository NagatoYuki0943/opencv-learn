import numpy as np
import cv2


image = cv2.imread("../images/gray.jpg", flags=0)

cv2.namedWindow(winname="Image")

image1 = image.copy()
cv2.imshow("Image", image1)

THRESH = 100
MAXVAL = 255

def f1(thresh):
    # thresh = cv2.getTrackbarPos(trackbarname="thresh", winname="Image")
    global THRESH, MAXVAL
    THRESH = thresh
    retval, image1 = cv2.threshold(
        src=image,
        thresh=THRESH, # 阈值
        maxval=MAXVAL, # dst图像中最大值 只对 THRESH_BINARY 和 THRESH_BINARY_INV 有效果
        type=cv2.THRESH_BINARY,
    )
    cv2.imshow("Image", image1)
    # 键盘检测函数，0xFF是因为64位机器
    k = cv2.waitKey(0) & 0xFF
    # print k
    if k == ord("q"):
        exit()

cv2.createTrackbar(
    "thresh",
    "Image",
    THRESH,
    MAXVAL,
    f1
)

cv2.destroyAllWindows()
