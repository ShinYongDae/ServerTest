
// ServerTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ServerTest.h"
#include "ServerTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerTestDlg 대화 상자



CServerTestDlg::CServerTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVERTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strPort = _T("2000");
	m_pServer = NULL;
}

CServerTestDlg::~CServerTestDlg()
{
	if (m_pServer)
	{
		delete m_pServer;
		m_pServer = NULL;
		Sleep(30);
	}
}

void CServerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_editMsgList);
}

BEGIN_MESSAGE_MAP(CServerTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CServerTestDlg::OnBnClickedBtnStart)
	ON_MESSAGE(WM_SERVER_RECEIVED, wmAcceptReceived)
	ON_BN_CLICKED(IDOK, &CServerTestDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_STOP, &CServerTestDlg::OnBnClickedBtnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SEND, &CServerTestDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CServerTestDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CServerTestDlg 메시지 처리기

BOOL CServerTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CServerTestApp* pApp = (CServerTestApp*)AfxGetApp();
	GetDlgItem(IDC_SVR_PORT)->SetWindowText(pApp->GetHostPort());
	GetDlgItem(IDC_IPADDRESS)->SetWindowText(pApp->GetHostAddress());

	SetTimer(0, 300, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CServerTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CServerTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CServerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerTestDlg::StartServer(CString sAddr, CString sPort)
{
	if (!m_pServer)
	{
		m_pServer = new CTcpIpServer(this);
		m_pServer->Init(sAddr, _tstoi(sPort));
		m_pServer->Start();
	}
}

void CServerTestDlg::StopServer()
{
	if (m_pServer)
	{
		if (!m_pServer->Stop()) // Called Destroy Function.
		{
			delete m_pServer;
		}
		m_pServer = NULL;
		Sleep(30);
	}
}

void CServerTestDlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CServerTestApp* pApp = (CServerTestApp*)AfxGetApp();

	CString sPort, sAddr;
	GetDlgItem(IDC_SVR_PORT)->GetWindowText(sPort);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(sAddr);
	pApp->SetHostAddress(sAddr); 
	pApp->SetHostPort(sPort);

	StartServer(sAddr, sPort);
}

void CServerTestDlg::OnBnClickedBtnStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StopServer();
}

LRESULT CServerTestDlg::wmAcceptReceived(WPARAM wParam, LPARAM lParam)
{
	CString sReceived = (LPCTSTR)lParam;

	CString sDisp;
	GetDlgItem(IDC_MESSAGE_LIST)->GetWindowText(sDisp);
	sDisp += sReceived + _T("\r\n");
	GetDlgItem(IDC_MESSAGE_LIST)->SetWindowText(sDisp);

	return (LRESULT)1;
}

void CServerTestDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StopServer();

	CDialogEx::OnOK();
}


void CServerTestDlg::OnDestroy()
{
	StopServer();

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void CServerTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 0:
		KillTimer(0);
		DispClientStatus();
		SetTimer(0, 300, NULL);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CServerTestDlg::DispClientStatus()
{
	if(m_pServer)
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_pServer->IsConnected(0));
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_pServer->IsConnected(1));
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(m_pServer->IsConnected(2));
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(m_pServer->IsConnected(3));
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(m_pServer->IsConnected(4));
		((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(m_pServer->IsConnected(5));
		((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(m_pServer->IsConnected(6));
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(m_pServer->IsConnected(7));
	}
}


void CServerTestDlg::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다. 
	CString sClientID, sMsg;
	GetDlgItem(IDC_CLIENT_ID)->GetWindowText(sClientID);
	GetDlgItem(IDC_TXT_MESSAGE)->GetWindowText(sMsg);

	if (sClientID.IsEmpty() || sMsg.IsEmpty())
		return;

	if (m_pServer)
	{
		int nClientID = _tstoi(sClientID);
		if (nClientID < 1 || nClientID > 8)
			return;

		m_pServer->WriteComm(nClientID - 1, sMsg);
	}
}


void CServerTestDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}
