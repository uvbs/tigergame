
// TigerGame.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTigerGameApp:
// �йش����ʵ�֣������ TigerGame.cpp
//

class CTigerGameApp : public CWinApp
{
public:
	CTigerGameApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTigerGameApp theApp;