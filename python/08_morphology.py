import numpy as np
import cv2


image = cv2.imread("../images/squirrel.jpg")

cv2.namedWindow(winname="Image")

image1 = image.copy()
kernel = np.ones((3, 3))

while True:
    cv2.imshow("Image", image1)
    # 键盘检测函数，0xFF是因为64位机器
    k = cv2.waitKey(0) & 0xFF
    # print k
    if k == ord('e'):
        # 加上iterations是为了记住这个参数，不加也行
        image1 = cv2.erode(
            src=image1,
            kernel=kernel,
            iterations=1
        )
        print("erode")
    elif k == ord('d'):
        image1 = cv2.dilate(
            image1,
            kernel,
            iterations=1
        )
        print("dilate")
    elif k == ord('o'):
        image1 = cv2.morphologyEx(
            src=image1,
            op=cv2.MORPH_OPEN,
            kernel=kernel,
            iterations=1,
        )
        print("open")
    elif k == ord('c'):
        image1 = cv2.morphologyEx(
            src=image1,
            op=cv2.MORPH_CLOSE,
            kernel=kernel,
            iterations=1,
        )
        print("close")
    elif k == ord('r'):
        image1 = image.copy()
        print("origin image")
    elif k == ord('q'):
        break

cv2.destroyAllWindows()
