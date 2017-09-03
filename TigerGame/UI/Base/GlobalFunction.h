#pragma once

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

typedef struct
{
	int  red;              // [0,255]
	int  green;            // [0,255]
	int  blue;             // [0,255]
}COLOR_RGB;

typedef struct
{
	float hue;              // [0,360]
	float saturation;       // [0,100]
	float luminance;        // [0,100]
}COLOR_HSL;

// 加载图片
bool LoadImage(const CString strPathFile, CBitmap &bitmap, CSize &size);
// 读取图片
bool LoadImage(UINT nID, CBitmap &bitmap, CSize &size, CString strType);
// 加载图片
bool ImageFromIDResource(UINT nID, CString strType, Image * & pImg);
// 加载图片
bool ImageFrommMemory(const void *buf, size_t size, Image * & pImg);
// 取得图片平均颜色
bool GetAverageColor(CDC *pDC, Image *&pImage, COLORREF &clrImage);
// 取得图片平均颜色
bool GetAverageColor(CDC *pDC, CBitmap &bitmap, const CSize &sizeImage, COLORREF &clrImage);
// 取得图片大小
bool GetSize(CBitmap &bitmap, CSize &size);
// 取得字体大小
Size GetTextBounds(const Font& font, const StringFormat& strFormat, const CString& strText);
// 取得字体大小
Rect GetTextBoundsRect(const Font& font, const StringFormat& strFormat, const CString& strText);
// 取得字体大小
Size GetTextBounds(const Font& font, const CString& strText);
// 取得位置
CPoint GetOriginPoint(int nWidth, int nHeight, int nChildWidth, int nChildHeight, UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER);
// 取得位置
CPoint GetOriginPoint(CRect rc, int nChildWidth, int nChildHeight, UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER);
// 转换数字
CString DecimalFormat(int nNumber);


// 绘画函数

// 画垂直过渡
int DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rcDes, const CRect &rcSrc, int nBeginTransparent = 0, int nEndTransparent = 100);
// 画水平过渡
int DrawHorizontalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rcDes, const CRect &rcSrc, int nBeginTransparent = 0, int nEndTransparent = 100);
// 画右下角过渡
void DrawRightBottomTransition(CDC &dc, CDC &dcTemp, CRect rc, const int nOverRegio, const COLORREF clrBackground);
// 画图片边框
void DrawImageFrame(Graphics &graphics, Image *pIamge, const CRect &rcControl, int nX, int nY, int nW, int nH, int nFrameSide = 4);
// 画过渡圆角矩形
void DrawRectangle(CDC &dcDes, const CRect &rcDes, bool bUp = TRUE, int nBeginTransparent = 60, int nEndTransparent = 90);


//获取程序所在目录
CString GetCurrentDirectory();
// Converts RGB to HSL
void RGBtoHSL(const COLORREF clr, COLOR_HSL *hsl);
// Converts HSL to RGB
void HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB &rgb);
