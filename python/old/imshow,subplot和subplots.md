# cv.imshow

```python
import cv2 as cv
# 2.显示图像
# 参数1: 窗口名称
# 参数2: 要加载的图像
cv.imshow('image',img)
# 要写上 cv.waitKey(0)
cv.waitKey(0)
```





# plt.imshow

```python
import matplotlib.pyplot as plt

# 3 图像显示 rgb => bgr
plt.imshow(img[:,:,::-1])
# title和后面的可以不写
plt.title('匹配结果'), plt.xticks([]), plt.yticks([])

#灰度图片
plt.imshow(img,cmap=plt.cm.gray)

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```





# subplot显示图片

```python
import matplotlib.pyplot as plt

# 4.图像显示
plt.figure(figsize=(10, 8), dpi=100)	#这一行可以不要
# rgb => bgr
plt.subplot(131), plt.imshow(img[:, :, ::-1]), plt.title('原图')
plt.subplot(132), plt.imshow(erosion[:, :, ::-1]), plt.title('腐蚀')
plt.subplot(133), plt.imshow(dilate[:, :, ::-1]), plt.title('膨胀')

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```



## subplot显示灰度图片

```python
import matplotlib.pyplot as plt

# 5.图像显示
plt.figure(figsize=(10, 8), dpi=100)	#这一行可以不要
# 221 一共两行两列,这是第一个
# 这里两个语句换行写也没问题
plt.subplot(221), plt.imshow(img, cmap=plt.cm.gray), plt.title('原图')
#plt.xticks([]), plt.yticks([])
# 221 一共两行两列,这是第二个
plt.subplot(222), plt.imshow(result, cmap=plt.cm.gray), plt.title('Sobel滤波后结果')

# 221 一共两行两列,这是第三个
plt.subplot(223), plt.imshow(img, cmap=plt.cm.gray), plt.title('原图')
# 221 一共两行两列,这是第四个
plt.subplot(224), plt.imshow(result1, cmap=plt.cm.gray), plt.title('Scharr滤波后结果')


# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```





# subplots 用来显示多张图片,规定行和列

## 显示一维图片

> **一行多列或者多行一列的情况下,下面的 axes[] 里面直接写顺序即可**

```python
import matplotlib.pyplot as plt
# 3.2 使用matplotlib显示图像
fig, axes = plt.subplots(nrows=1, ncols=3, figsize=(10, 8), dpi=100)
#  ::-1 是将 cv.imread 读取的 bgr 转换为 rgb
axes[0].imshow(res[:, :, ::-1])
axes[0].set_title("绝对尺度,放大")
axes[1].imshow(img[:, :, ::-1])
axes[1].set_title("原图")
axes[2].imshow(res1[:, :, ::-1])
axes[2].set_title("相对尺度,缩小")

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi'] # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```



## 显示一维灰度图片

```python
import matplotlib.pyplot as plt
# 3. 结果展示
fig,axes=plt.subplots(nrows=1, ncols=2, figsize=(10,8), dpi=100)
axes[0].imshow(img, cmap=plt.cm.gray)
axes[0].set_title("原图")
axes[1].imshow(dst, cmap=plt.cm.gray)
axes[1].set_title("均衡化后结果")

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi'] # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```



## 显示二维图片

```python
import matplotlib.pyplot as plt
# 4.图像展示
fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))
axes[0, 0].imshow(img1[:, :, ::-1])
axes[0, 0].set_title("原图")
axes[0, 1].imshow(cvOpen[:, :, ::-1])
axes[0, 1].set_title("开运算结果")
axes[1, 0].imshow(img2)
axes[1, 0].set_title("原图")
axes[1, 1].imshow(cvClose[:, :, ::-1])
axes[1, 1].set_title("闭运算结果")

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi'] # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```



## 显示二维灰度图片

```python
import matplotlib.pyplot as plt
fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))
# cmap=plt.cm.gray 代表灰度
axes[0, 0].imshow(img, cmap=plt.cm.gray)
axes[0, 0].set_title("原图")
axes[0, 1].imshow(mask, cmap=plt.cm.gray)
axes[0, 1].set_title("蒙版数据")
axes[1, 0].imshow(masked_img, cmap=plt.cm.gray)
axes[1, 0].set_title("掩膜后数据")
axes[1, 1].plot(mask_histr)
axes[1, 1].grid()
axes[1, 1].set_title("灰度直方图")

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi'] # 指定默认字体
plt.rcParams['axes.unicode_minus'] = False  # 解决保存图像是负号'-'显示为方块的问题
plt.show()
```





# plot折线图

## plot绘制灰度图

```python
from matplotlib import pyplot as plt

plt.figure(figsize=(10, 5), dpi=100)
# histr 是直方图
plt.plot(histr)
plt.grid()
plt.show()
```

## 结合subplots

```python
from matplotlib import pyplot as plt

fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))
axes[0, 0].imshow(img, cmap=plt.cm.gray)
axes[0, 0].set_title("原图")
axes[0, 1].imshow(mask, cmap=plt.cm.gray)
axes[0, 1].set_title("蒙版数据")
axes[1, 0].imshow(masked_img, cmap=plt.cm.gray)
axes[1, 0].set_title("掩膜后数据")
# 折线图
axes[1, 1].plot(mask_histr)
axes[1, 1].grid()
axes[1, 1].set_title("灰度直方图")
```

