
// ServerTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

#include "SingleInstance.h"



// CServerTestApp:
// �� Ŭ������ ������ ���ؼ��� ServerTest.cpp�� �����Ͻʽÿ�.
//

class CServerTestApp : public CWinApp
{
	CSingleInstance m_singleInstance;
	CString m_strHostAddress, m_strHostPort;

	void wsaStartup();
	void wsaEndup();


public:
	CServerTestApp();
	~CServerTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	CString GetHostAddress();
	CString GetHostPort();
	void SetHostAddress(CString sAddr);
	void SetHostPort(CString sPort);

	DECLARE_MESSAGE_MAP()
};

extern CServerTestApp theApp;