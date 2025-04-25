#include "stdafx.h"
#include "TcpIpAccept.h"
#include "TcpIpServer.h"


CTcpIpAccept::CTcpIpAccept(CTcpIpServer* pParent/*=NULL*/)
{
	::InitializeCriticalSection(&m_sc);

	m_pParent = pParent;
	m_nClientID = -1;

	m_pReceiveBuffer = new BYTE[16384]; // 1mb
	m_nBufferId = 0;
	m_pCurrentBuffer = new BYTE[16384]; // 1mb
}

CTcpIpAccept::~CTcpIpAccept()
{
	if(m_pReceiveBuffer)
	{
		delete[] m_pReceiveBuffer;
		m_pReceiveBuffer = NULL;
	}
	if (m_pCurrentBuffer)
	{
		delete[] m_pCurrentBuffer; 
		m_pCurrentBuffer = NULL;
	}

	::DeleteCriticalSection(&m_sc);
}


void CTcpIpAccept::Init(SOCKET sock, sockaddr ClientIP, int nClientID)
{
	m_hComm = (HANDLE)sock;
	m_ClientIP = ClientIP;
	m_nClientID = nClientID;
}

sockaddr CTcpIpAccept::GetIP()
{
	return m_ClientIP;
}

void CTcpIpAccept::Start()
{
	PrepareThread();
	ResumeThread();
}

int CTcpIpAccept::Running()
{
	::EnterCriticalSection(&m_sc);

	char buffer[1024];
	sprintf(buffer, "Connection is success.");
	int nLen = strlen(buffer);

	WriteComm((BYTE*)buffer, nLen, 5000);

	//m_bConnected = 1;
	int i = 0;

	DWORD	dwBytes = 0L;
	DWORD	dwTimeout = 5000;

	while (IsOpen() && !m_bStop)
	{
		fd_set	fdRead = { 0 };
		TIMEVAL	stTime;
		TIMEVAL	*pstTime = NULL;

		//160107 LGH 
		if (INFINITE != dwTimeout) {
			stTime.tv_sec = dwTimeout / 1000;
			stTime.tv_usec = (dwTimeout * 1000) % 1000000;
			pstTime = &stTime;
		}

		SOCKET s = (SOCKET)m_hComm;
		// Set Descriptor
		if (!FD_ISSET(s, &fdRead))
			FD_SET(s, &fdRead);

		// Select function set read timeout
		DWORD dwBytesRead = 0L;
		int res = select(s + 1, &fdRead, NULL, NULL, pstTime);
		if (res > 0)
		{
			BYTE buffer[1024] = { 0, };
			int nIdx = 1024; //protocol의 헤더 길이만큼 딴다

			res = recv(s, (LPSTR)&buffer, nIdx, 0);
			if (res > 0)
			{
				memcpy(&m_pReceiveBuffer[m_nBufferId], &buffer[0], res);
				memset(m_pCurrentBuffer, 0, 16384);
				memcpy(m_pCurrentBuffer, &m_pReceiveBuffer[m_nBufferId], res);
				m_pCurrentBuffer[res] = _T('\0');
				m_nBufferId += res;

				OnDataReceived();

				int nLeftLength = m_nBufferId - (i+1);
				int nStartID = m_nBufferId - i;//3-1= id[2]

				if (nLeftLength > 0)
					memmove(&m_pReceiveBuffer[0], &m_pReceiveBuffer[nStartID], nLeftLength);
				else
				{
					m_nBufferId = 0;
					memset(m_pReceiveBuffer, 0, 16384);
				}

				dwBytesRead = res;
			}
			else
			{
				// 연결 상태 끊어짐 확인.
				((CTcpIpServer*)m_pParent)->wmAcceptClosed((WPARAM)0, (LPARAM)m_nClientID);

				::LeaveCriticalSection(&m_sc);
				return 0; // Terminate Thread
			}
		}
		else
		{
			dwBytesRead = (DWORD)(-1L); 
		}

		// Error? - need to signal error
		if (dwBytes == (DWORD)-1L)
		{
			// Do not send event if we are closing

			break;
		}

		Sleep(0);
	}

	StopComm();

	::LeaveCriticalSection(&m_sc);
	return 0; // Terminate Thread
}

void CTcpIpAccept::OnDataReceived()
{
	m_strReceived = CharToString((char*)m_pCurrentBuffer);

	((CTcpIpServer*)m_pParent)->wmAcceptReceived((WPARAM)0, (LPARAM)(LPCTSTR)m_strReceived);
}

BOOL  CTcpIpAccept::Stop()
{
	m_bStop = 1;

	if (IsOpen())
	{
		StopComm();
		return TRUE;
	}

	return FALSE;
}