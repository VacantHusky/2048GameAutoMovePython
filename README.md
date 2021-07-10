# 2048GameAutoMovePython
使用python3编写2048游戏及自动玩2048

演示视频：https://www.bilibili.com/video/BV1zg411u7tH/

使用pygame模块绘制游戏界面。
# 一、2048python
2048python目录里是纯python写的。
启动方式：
```shell script
cd ./2048python
python3 main.py
```

# 二、2048python&c
2048python&c目录里是将2048策略算法使用c语言重写，然后编译成so文件，以提高运行速度。

## 1.编译so文件
项目中的2048.so文件是在我电脑上编译的，如果你的电脑是64位，安装的python也是64位的，且cpu什么的和我差不多，那么可以直接使用我编译的so文件。

需要安装gcc，并且得看你的python是32位还是64位，gcc也得是相应的位数。

编译：
```shell script
# 进入目录
cd "./2048python&c"
gcc 2048ai.c -shared -o ./2048.so
```

## 2.运行
```shell script
python3 main.py
```
