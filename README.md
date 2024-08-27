# 外挂设计与分析 Cheat Design and Analysis

------

> * 01一个简单的内存外挂 A Simple Memory Cheat
> * 02从基址读取数据 Reading Data from the Base Address
> * 03自动模拟操作功能 Automating Simulated Operations
> * 04简单DLL注入游戏 Simple DLL Injection into a Game
> * 05简单DLL劫持 Simple DLL Hijacking
> * 06瞄准辅助 Aiming Assistance
> * 07简单进程保护 Simple Process Protection


------
<br><br>

## 01一个简单的内存外挂 A Simple Memory Cheat<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/03.jpg)<br>
通过C语言编写一个简单的外挂，通过API函数修改游戏数据，从而实现作弊功能<br>
Write a simple cheat in C language that modifies game data using API functions to achieve cheating functionality.
<br><br><br><br><br><br>


## 02从基址读取数据 Reading Data from the Base Address<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/02%E4%BB%8E%E5%9F%BA%E5%9D%80%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/pictures/03.jpg?raw=true)<br>
通过C语言编写一个外挂，通过API函数获取游戏数据，从而实现作弊功能<br>
Write a cheat in C language that retrieves game data using API functions to achieve cheating functionality.
<br><br><br><br><br><br>


## 03自动模拟操作功能 Automating Simulated Operations<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/3.jpg)<br>
通过C语言编写一个外挂，通过API函数模拟进行连连看操作，实现自动进行游戏<br>
Write a cheat in C language that simulates Lianliankan operations using API functions to automate the gameplay.
<br><br><br><br><br><br>

## 04简单DLL注入游戏 Simple DLL Injection into a Game<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/04%E7%AE%80%E5%8D%95DLL%E6%B3%A8%E5%85%A5%E6%B8%B8%E6%88%8F/pictures/03.jpg?raw=true)<br>
通过C语言编写一个DLL文件和一个EXE文件，其中DLL包含作弊功能，运行EXE后将DLL注入游戏，使得游戏仅靠自身进程便可以实现作弊功能<br>
Write a DLL file and an EXE file in C language, where the DLL contains the cheating functionality. Upon running the EXE, the DLL will be injected into the game, enabling the game to perform cheats solely through its own process.
<br><br><br><br><br><br>

## 05简单DLL劫持 Simple DLL Hijacking<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/4.jpg?raw=true)<br>
通过C语言编写一个游戏要加载的系统DLL文件(lpk.dll)，其中假DLL包含劫持功能和作弊功能且拥有相同的导出函数，将该假DLL放入游戏相同目录下，游戏打开时会自动加载该假DLL，使游戏直接包含作弊功能<br>
Write a system DLL file (lpk.dll) in C language that the game needs to load. This fake DLL will contain hijacking and cheating functionalities and will have the same exported functions. Place this fake DLL in the same directory as the game, so when the game starts, it will automatically load the fake DLL, thereby integrating cheating functionalities directly into the game.
<br><br><br><br><br><br>

## 06瞄准辅助 Aiming Assistance<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/cross0.gif?raw=true)<br>
通过简单GDI绘图，使游戏窗口上出现设计好的辅助线作为准星，并且可手动调节准星的形状、大小、颜色等<br>
Use simple GDI drawing to display designed crosshair lines on the game window as an aiming aid, with the ability to manually adjust the shape, size, color, etc., of the crosshair.
<br><br><br><br><br><br>

## 07简单进程保护 Simple Process Protection<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/raw/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/3.jpg?raw=true)<br>
通过编写简单的进程hook保护驱动并运行，分析如何解除驱动保护<br>
Write and execute a simple process hook protection driver, and analyze how to disable driver protection.
<br><br><br><br><br><br>
