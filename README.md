# 外挂设计与分析

------

> * 01一个简单的内存外挂
> * 02从基址读取数据
> * 03自动模拟操作功能
> * 04简单DLL注入游戏
> * 05简单DLL劫持
> * 06瞄准辅助
> * 07简单进程保护


------
<br><br>

## 01一个简单的内存外挂<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/03.jpg)<br>
通过C语言编写一个简单的外挂，通过API函数修改游戏数据，从而实现作弊功能
<br><br><br><br><br><br>


## 02从基址读取数据<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/02%E4%BB%8E%E5%9F%BA%E5%9D%80%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/pictures/03.jpg?raw=true)<br>
通过C语言编写一个外挂，通过API函数获取游戏数据，从而实现作弊功能
<br><br><br><br><br><br>


## 03自动模拟操作功能<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/3.jpg)<br>
通过C语言编写一个外挂，通过API函数模拟进行连连看操作，实现自动进行游戏
<br><br><br><br><br><br>

## 04简单DLL注入游戏<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/04%E7%AE%80%E5%8D%95DLL%E6%B3%A8%E5%85%A5%E6%B8%B8%E6%88%8F/pictures/03.jpg?raw=true)<br>
通过C语言编写一个DLL文件和一个EXE文件，其中DLL包含作弊功能，运行EXE后将DLL注入游戏，使得游戏仅靠自身进程便可以实现作弊功能
<br><br><br><br><br><br>

## 05简单DLL劫持<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/4.jpg?raw=true)<br>
通过C语言编写一个游戏要加载的系统DLL文件(lpk.dll)，其中假DLL包含劫持功能和作弊功能且拥有相同的导出函数，将该假DLL放入游戏相同目录下，游戏打开时会自动加载该假DLL，使游戏直接包含作弊功能
<br><br><br><br><br><br>

## 06瞄准辅助<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/cross0.gif?raw=true)<br>
通过简单GDI绘图，使游戏窗口上出现设计好的辅助线作为准星，并且可手动调节准星的形状、大小、颜色等
<br><br><br><br><br><br>

## 07简单进程保护<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/3.jpg?raw=true)<br>
通过编写简单的进程hook保护驱动并运行，分析如何解除驱动保护
<br><br><br><br><br><br>
