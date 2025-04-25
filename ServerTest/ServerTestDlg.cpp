
// ServerTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ServerTest.h"
#include "ServerTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CServerTestDlg ��ȭ ����



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


// CServerTestDlg �޽��� ó����

BOOL CServerTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CServerTestApp* pApp = (CServerTestApp*)AfxGetApp();
	GetDlgItem(IDC_SVR_PORT)->SetWindowText(pApp->GetHostPort());
	GetDlgItem(IDC_IPADDRESS)->SetWindowText(pApp->GetHostAddress());

	SetTimer(0, 300, NULL);
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CServerTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	StopServer();

	CDialogEx::OnOK();
}


void CServerTestDlg::OnDestroy()
{
	StopServer();

	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}



void CServerTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�. 
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}
