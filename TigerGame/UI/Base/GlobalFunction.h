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

// ����ͼƬ
bool LoadImage(const CString strPathFile, CBitmap &bitmap, CSize &size);
// ��ȡͼƬ
bool LoadImage(UINT nID, CBitmap &bitmap, CSize &size, CString strType);
// ����ͼƬ
bool ImageFromIDResource(UINT nID, CString strType, Image * & pImg);
// ����ͼƬ
bool ImageFrommMemory(const void *buf, size_t size, Image * & pImg);
// ȡ��ͼƬƽ����ɫ
bool GetAverageColor(CDC *pDC, Image *&pImage, COLORREF &clrImage);
// ȡ��ͼƬƽ����ɫ
bool GetAverageColor(CDC *pDC, CBitmap &bitmap, const CSize &sizeImage, COLORREF &clrImage);
// ȡ��ͼƬ��С
bool GetSize(CBitmap &bitmap, CSize &size);
// ȡ�������С
Size GetTextBounds(const Font& font, const StringFormat& strFormat, const CString& strText);
// ȡ�������С
Rect GetTextBoundsRect(const Font& font, const StringFormat& strFormat, const CString& strText);
// ȡ�������С
Size GetTextBounds(const Font& font, const CString& strText);
// ȡ��λ��
CPoint GetOriginPoint(int nWidth, int nHeight, int nChildWidth, int nChildHeight, UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER);
// ȡ��λ��
CPoint GetOriginPoint(CRect rc, int nChildWidth, int nChildHeight, UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER);
// ת������
CString DecimalFormat(int nNumber);


// �滭����

// ����ֱ����
int DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rcDes, const CRect &rcSrc, int nBeginTransparent = 0, int nEndTransparent = 100);
// ��ˮƽ����
int DrawHorizontalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rcDes, const CRect &rcSrc, int nBeginTransparent = 0, int nEndTransparent = 100);
// �����½ǹ���
void DrawRightBottomTransition(CDC &dc, CDC &dcTemp, CRect rc, const int nOverRegio, const COLORREF clrBackground);
// ��ͼƬ�߿�
void DrawImageFrame(Graphics &graphics, Image *pIamge, const CRect &rcControl, int nX, int nY, int nW, int nH, int nFrameSide = 4);
// ������Բ�Ǿ���
void DrawRectangle(CDC &dcDes, const CRect &rcDes, bool bUp = TRUE, int nBeginTransparent = 60, int nEndTransparent = 90);


//��ȡ��������Ŀ¼
CString GetCurrentDirectory();
// Converts RGB to HSL
void RGBtoHSL(const COLORREF clr, COLOR_HSL *hsl);
// Converts HSL to RGB
void HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB &rgb);
