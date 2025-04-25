#pragma once

#include "CEasyThread.h"
#include "TcpIpAccept.h"
#include "SocketComm.h"

#define MAX_CLIENT	8

class CTcpIpServer : public CSocketComm, public CEasyThread
{
	DECLARE_DYNCREATE(CTcpIpServer)

	HWND m_hParentWnd;
	CWnd* m_pParent;

	CTcpIpAccept* m_pClientAddr[MAX_CLIENT]; // Number of Max connection is 10...
	int m_nConnectedID;
	int m_nNetworkPort;
	CString m_strServerIP;

public:
	CTcpIpServer(CWnd* pParent = NULL);
	virtual ~CTcpIpServer();

	CRITICAL_SECTION m_sc;

	void Init(CString strServerIP, int nPort);
	void Start();
	BOOL Stop();
	virtual int Running();
	void wmAcceptReceived(WPARAM wParam, LPARAM lParam);
	void wmAcceptClosed(WPARAM wParam, LPARAM lParam);
	BOOL IsConnected(int nClientID);
	int WriteComm(int nClientID, CString sMsg);

// Generated message map functions
protected:
	//{{AFX_MSG(CTcpIpServer)

	DECLARE_MESSAGE_MAP()
};


extern CTcpIpServer g_sr1000Data;