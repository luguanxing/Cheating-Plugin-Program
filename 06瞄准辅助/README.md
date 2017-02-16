# 06瞄准辅助

------
通过简单GDI绘图，使游戏窗口上出现设计好的辅助线作为准星，并且可手动调节准星的形状、大小、颜色等

> * 设计思路分析
> * 对象分析
> * 要用的函数介绍
> * 编写测试效果
> * 总体评价

------
##设计思路分析
1、开启线程，在循环中查找游戏窗口<br>
2、游戏窗口存在时读取配置不断在游戏窗口和自身窗口上绘制辅助线<br>
<br><br>

## 对象分析
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/game0.jpg?raw=true)<br>
游戏对象为Cs1.6(3248)和CsOkgogogo<br>
<br><br>

## 主要函数介绍
**HDC GetDC(HWND hWnd)**<br>
检索显示设备上下文环境的句柄<br>
<br>
**CDC**<br>
CDC类定义的是设备上下文对象的类<br>
<br>
**CPoint CDC::MoveTo(int x,int y);**<br>
**CPoint CDC::LineTo(int x,int y);**<br>
绘制直线<br>
<br>
**BOOL CDC::Ellipse(int x1,int y1，int x2,int y2);**<br>
绘制圆形<br>
<br>
**BOOL CRgn::CreateRectRgn(int x1,int y1,int x2,int y2);**<br>
建立方形区域<br>
<br>
**BOOL CRgn::CreateEllipticRgn(int x1,int y1,int x2,int y2);**<br>
建立圆形区域<br>
<br>
**int CombineRgn(CRgn* pRgn1,CRgn* pRgn2,int nCombineMode );**<br>
合并区域(圆点准心为使得中间透明采取大圆-小圆的区域)<br>
<br>
**BOOL FillRgn(CRgn* pRgn,CBrush* pBrush );**<br>
填充形状<br>
<br><br>




## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/cross0.gif?raw=true)<br>
**打开辅助测试准星形状**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/cross1.gif?raw=true)<br>
**打开辅助测试准星颜色**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/game3.jpg?raw=true)<br>
**隐藏游戏自带准星**<br>
首先设置Cs中的自带准星透明(进入CS控制台输入cl_crosshair_color 0 0 0或修改strike目录下config.cfg的值)<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/cross2.gif?raw=true)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/06%E7%9E%84%E5%87%86%E8%BE%85%E5%8A%A9/pictures/cross3.gif?raw=true)<br>
**设置复制准星后进入游戏**<br>
这是预设好的准星能显示在游戏中
<br><br><br><br><br><br>




```
    //十字准星
	cdc.MoveTo(width/2 - crosssize, height/2);
	cdc.LineTo(width/2 + crosssize, height/2);
	cdc.MoveTo(width/2, height/2-crosssize);
	cdc.LineTo(width/2, height/2+crosssize);
```
<br><br><br>

```
    //单点准星
	CBrush brush,*oldbrush;
	oldbrush=cdc.SelectObject(&brush);
	cdc.Ellipse(width/2 -1, height/2-1, width/2+1, height/2+1);
	cdc.SelectObject(oldbrush);
```
<br><br><br>

```
    //圆点准星
	CRgn r,r1,r2,rgn;
	r.CreateEllipticRgn(width/2-crossweight,height/2-crossweight,width/2+crossweight,height/2+crossweight);   //中心点
	r1.CreateEllipticRgn(width/2-crosssize-crossweight,height/2-crosssize-crossweight,width/2+crosssize+crossweight,height/2+crosssize+crossweight);
	r2.CreateEllipticRgn(width/2-crosssize,height/2-crosssize,width/2+crosssize,height/2+crosssize);
	rgn.CreateRectRgn(width/2-crosssize-crossweight-5,height/2-crosssize-crossweight-5,width/2+crosssize+crossweight+5,height/2+crosssize+crossweight+5);
	rgn.CombineRgn(&r1,&r2,RGN_DIFF);	//合并(大圆减小圆成环)
	CBrush bsh(RGB(red,green,blue));
	cdc.FillRgn(&r,&bsh);
	cdc.FillRgn(&rgn,&bsh);
	rgn.DeleteObject();
	r.DeleteObject();
	r1.DeleteObject();
	r2.DeleteObject();
```
<br><br><br>



## 总体评价

辅助窗口在绘制自身窗口上的准星时容易出现绘图无效区域,但不影响游戏(原因待查明)，也可以不将准星绘制在自身窗口上
