#include "stdafx.h"
#include "TcpIpServer.h"


IMPLEMENT_DYNCREATE(CTcpIpServer, CWinThread)
BEGIN_MESSAGE_MAP(CTcpIpServer, CWinThread)
	//{{AFX_MSG_MAP(CTcpIpServer)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTcpIpServer::CTcpIpServer(CWnd* pParent/*=NULL*/)
{
	::InitializeCriticalSection(&m_sc);

	int i = 0;
	m_pParent = pParent;
	m_hParentWnd = pParent->GetSafeHwnd();
	m_nConnectedID = -1;

	for (i = 0; i < MAX_CLIENT; i++)
	{		
		m_pClientAddr[i] = NULL;
	}	

}

CTcpIpServer::~CTcpIpServer()
{
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if(m_pClientAddr[i])
		{ 
			delete m_pClientAddr[i];
			m_pClientAddr[i] = NULL;
			Sleep(30);
		}
	}

	::DeleteCriticalSection(&m_sc);
}


void CTcpIpServer::Init(CString strServerIP, int nPort)
{
	m_strServerIP = strServerIP;
	m_nNetworkPort = nPort;

	CString strPort;
	strPort.Format(_T("%d"), m_nNetworkPort);

	BOOL bSuccess = ListenFrom(m_strServerIP, strPort, AF_INET);	// 리슨(listen) 소켓이다 이건!!
																	// Should we run as server mode
}

void CTcpIpServer::Start()
{
	PrepareThread();
	ResumeThread();
}

int CTcpIpServer::Running()
{
	::EnterCriticalSection(&m_sc);

	int i = 0;

	if (m_bStop)
	{
		::LeaveCriticalSection(&m_sc);
		return 0; // Terminate Thread
	}


	SOCKET		sock = (SOCKET)m_hComm;
	sockaddr	client_addr;
	socklen_t	client_addr_size = sizeof(client_addr);

	sock = WaitForConnection(sock, (sockaddr *)&client_addr, (int*)&client_addr_size); //클라이언트 소켓을 받고 (accept)
									
	if (sock != INVALID_SOCKET)
	{
		for (i = 0; i < MAX_CLIENT; i++)
		{
			if (!m_pClientAddr[i])
			{
				//CTcpIpAccept* pClientAddr = new CTcpIpAccept(this);
				m_nConnectedID = i;
				m_pClientAddr[m_nConnectedID] = new CTcpIpAccept(this);
				m_pClientAddr[m_nConnectedID]->Init(sock, (sockaddr)client_addr, m_nConnectedID);
				m_pClientAddr[m_nConnectedID]->Start();
				break;
			}
		}

		if (i >= MAX_CLIENT)
		{
			::LeaveCriticalSection(&m_sc);
			return 1;
		}

	}
	else
	{
		// Do not send event if we are closing

		if (m_bStop)
		{
			::LeaveCriticalSection(&m_sc);
			return 0; // Terminate Thread
		}

		::LeaveCriticalSection(&m_sc);
		return 1; 
	}

	::LeaveCriticalSection(&m_sc);
	return 1;
}



void CTcpIpServer::wmAcceptReceived(WPARAM wParam, LPARAM lParam)
{
	CString sReceived = (LPCTSTR)lParam;

	::SendMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)0, (LPARAM)(LPCTSTR)sReceived);
}

void CTcpIpServer::wmAcceptClosed(WPARAM wParam, LPARAM lParam)
{
	int nClientID = (int)lParam;

	if (m_pClientAddr[nClientID])
	{
		m_pClientAddr[nClientID] = NULL;
	}
}

BOOL CTcpIpServer::Stop()
{
	int i = 0;

	m_bStop = 1;
	if (IsOpen())
	{
		for (i = 0; i < MAX_CLIENT; i++)
		{
			if(m_pClientAddr[i])
			{
				if (!m_pClientAddr[i]->Stop())	// Called Destroy Function.
				{
					delete m_pClientAddr[i];
				}
				m_pClientAddr[i] = NULL;
				Sleep(30);
			}
		}

		StopComm();
		return TRUE;
	}

	return FALSE;
}

BOOL CTcpIpServer::IsConnected(int nClientID)
{
	if (nClientID >= MAX_CLIENT)
		return FALSE;

	return (m_pClientAddr[nClientID] != NULL);
}

int CTcpIpServer::WriteComm(int nClientID, CString sMsg)
{
	if (m_pClientAddr[nClientID])
	{
		char buffer[1024];
		StringToChar(sMsg, buffer);
		int nLen = strlen(buffer);

		m_pClientAddr[nClientID]->WriteComm((BYTE*)buffer, nLen, 5000);
	}

	return 0;
}