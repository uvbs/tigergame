
// TigerGame.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TigerGame.h"
#include "TigerGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTigerGameApp

BEGIN_MESSAGE_MAP(CTigerGameApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTigerGameApp ����

CTigerGameApp::CTigerGameApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	typedef BOOL (__stdcall *Fun_SetProcessDPIAware)(VOID);
	HINSTANCE hNtDll = LoadLibrary (TEXT("User32.dll"));
	if(0 != hNtDll)
	{
		Fun_SetProcessDPIAware SetProcessDPIAware = (Fun_SetProcessDPIAware)GetProcAddress (hNtDll,"SetProcessDPIAware");
		if(0 != SetProcessDPIAware)
			SetProcessDPIAware();

		FreeLibrary(hNtDll);
	}
}


// Ψһ��һ�� CTigerGameApp ����

CTigerGameApp theApp;


// CTigerGameApp ��ʼ��

BOOL CTigerGameApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));	

	CTigerGameDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	//�ر�gdiplus�Ļ���
	Gdiplus::GdiplusShutdown(gdiplusToken);

	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
