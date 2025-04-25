#pragma once

#include "..\\ServerSrc\\CEasyThread.h"
#include "..\\ServerSrc\\SocketComm.h"

extern class CTcpIpServer;

class CTcpIpAccept : public CSocketComm, public CEasyThread
{
	HWND m_hParentWnd;
	CTcpIpServer* m_pParent;

	CString m_strServerIP;
	sockaddr m_ClientIP;
	int m_nClientID;
	BYTE* m_pReceiveBuffer;
	int m_nBufferId;
	CString m_strReceived;

public:
	CTcpIpAccept(CTcpIpServer* pParent = NULL);
	virtual ~CTcpIpAccept();

	CRITICAL_SECTION m_sc;

	void Init(SOCKET sock, sockaddr ClientIP, int nClientID);
	void Start();
	BOOL Stop();
	sockaddr GetIP();

	BYTE* m_pCurrentBuffer;
	virtual int Running();
	virtual void OnDataReceived();

};


