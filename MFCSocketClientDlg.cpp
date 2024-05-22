// MFCSocketClientDlg.cpp: 구현 파일
//

#pragma once
#include "pch.h"
#include "framework.h"
#include "MFCSocketClient.h"
#include "MFCSocketClientDlg.h"
#include "afxdialogex.h"
#include "oci.h"
#include "iostream"
#include "vector"
#include "map"

#define UNICODE

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


// CMFCSocketClientDlg 대화 상자



CMFCSocketClientDlg::CMFCSocketClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSOCKETCLIENT_DIALOG, pParent)
	, m_strListData(_T(""))
	, m_strSendData(_T(""))
	, m_strServerAddress(_T(""))
	, m_strID(_T(""))
	, m_strPW(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LIST, m_strListData);
	DDX_Text(pDX, IDC_EDIT_SEND_DATA, m_strSendData);
	DDX_Control(pDX, IDC_EDIT_LIST, m_edListData);
	//  DDX_Control(pDX, IDC_EDIT_ID, m_strID);
	DDX_Control(pDX, IDC_EDIT_ID, m_edID);
	DDX_Control(pDX, IDC_EDIT_PW, m_edPW);
	DDX_Control(pDX, IDC_BUTTON_EMO, m_Button_Emo);
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_edSendData);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PW, m_strPW);
}

BEGIN_MESSAGE_MAP(CMFCSocketClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_RECEIVE_MESSAGE, OnReceiveData)
	ON_MESSAGE(UM_CLOSE_MESSAGE, OnCloseSocket)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CMFCSocketClientDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFCSocketClientDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMFCSocketClientDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SIGNUP, &CMFCSocketClientDlg::OnBnClickedButtonSignup)
	ON_BN_CLICKED(IDC_BUTTON_EMO_STAR, &CMFCSocketClientDlg::OnBnClickedButtonEmoStar)
	ON_BN_CLICKED(IDC_BUTTON_EMO_HEART, &CMFCSocketClientDlg::OnBnClickedButtonEmoHeart)
	ON_BN_CLICKED(IDC_BUTTON_EMO_FLOWER, &CMFCSocketClientDlg::OnBnClickedButtonEmoFlower)
	ON_BN_CLICKED(IDC_BUTTON_EMO_MUSIC, &CMFCSocketClientDlg::OnBnClickedButtonEmoMusic)
END_MESSAGE_MAP()

struct UserDATA {
	std::string Mute;
	std::string BleakList;
	bool operator==(const UserDATA& otherthis) const {
		return (Mute == otherthis.Mute);
		return (BleakList == otherthis.BleakList);
	}
	std::string filter;
	std::string username;
	std::string password;
	std::map<std::string, SOCKET> userSocketMap;

};

void CMFCSocketClientDlg::AddMessage(CString strMsg)
{
	UpdateData(); // 현재 다이얼로그 컨트롤과 멤버 변수 간의 데이터 동기화

	if (m_strListData.IsEmpty())
		m_strListData = strMsg;
	else
		m_strListData += _T("\r\n\r\n") + strMsg; // 이전 메시지와 구분하기 위해 줄바꿈 추가

	UpdateData(FALSE); // 멤버 변수의 값을 다이얼로그 컨트롤에 반영

	m_edListData.LineScroll(m_edListData.GetLineCount()); // 스크롤을 맨 아래로 이동
}


//void CMFCSocketClientDlg::ChatSettingRCVE(const char* arg, CString IParam)
//{
//	UpdateData();
//	//OCI데이터베이스 헨들러 생성부분입니다.
//	OCIEnv* env;
//	OCIError* error;
//	OCIHandleAlloc(OCI_DEFAULT, (void**)&env, OCI_HTYPE_ENV, 0, NULL);
//	OCIHandleAlloc(env, (void**)&error, OCI_HTYPE_ERROR, 0, NULL);
//
//	//OCI데이터베이스 연결부분입니다.
//	OCISvcCtx* svc;
//	OCIServer* srv;
//	OCISession* sess;
//	OCIHandleAlloc(env, (void**)&svc, OCI_HTYPE_SVCCTX, 0, NULL);
//	OCIHandleAlloc(env, (void**)&srv, OCI_HTYPE_SERVER, 0, NULL);
//	OCIHandleAlloc(env, (void**)&sess, OCI_HTYPE_SESSION, 0, NULL);
//	OCIServerAttach(srv, error, (text*)"DATABASE_NAME", strlen("DATABASE_NAME"), OCI_DEFAULT);
//	OCIAttrSet(svc, OCI_HTYPE_SVCCTX, srv, 0, OCI_ATTR_SERVER, error);
//	OCIAttrSet(sess, OCI_HTYPE_SESSION, (void*)"USERNAME", strlen("USERNAME"), OCI_ATTR_USERNAME, error);
//	OCIAttrSet(sess, OCI_HTYPE_SESSION, (void*)"PASSWORD", strlen("PASSWORD"), OCI_ATTR_PASSWORD, error);
//	OCISessionBegin(svc, error, sess, OCI_CRED_RDBMS, OCI_DEFAULT);
//	OCIAttrSet(svc, OCI_HTYPE_SVCCTX, sess, 0, OCI_ATTR_SESSION, error);
//
//	//SQL쿼리 실행입니다.
//	OCIStmt* USER;
//	OCIHandleAlloc(env, (void**)&USER, OCI_HTYPE_STMT, 0, NULL);
//	OCIStmtPrepare(USER, error, (text*)"SELECT USERNAME FROME USERS", strlen("SELECT USERNAME FROME USERS"), OCI_NTV_SYNTAX, OCI_DEFAULT);
//	OCIStmtExecute(svc, USER, error, 1, 0, NULL, NULL, OCI_STMT_SCROLLABLE_READONLY);
//	OCIStmt* PW;
//	OCIHandleAlloc(env, (void**)&PW, OCI_HTYPE_STMT, 0, NULL);
//	OCIStmtPrepare(PW, error, (text*)"SELECT PASSWORD FROM USER", strlen("SELECT PASSWORD FROM USER"), OCI_NTV_SYNTAX, OCI_DEFAULT);
//	OCIStmtExecute(svc, PW, error, 1, 0, NULL, NULL, OCI_STMT_SCROLLABLE_READONLY);
//
//	//결과입니다
//	OCIParam* param;
//	OCIDefine* define;
//	text columnName[50];
//	OCIDefine* defines[2];
//	sword status;
//	ub2 type, length;
//	std::vector<UserDATA> users;
//
//	// USER ID값입니다
//	char USERresult[100];
//	while (OCIStmtFetch2(USER, error, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
//		UserDATA user;
//		OCIAttrGet(defines, OCI_HTYPE_DEFINE, USERresult, 0, OCI_ATTR_USERNAME, error);
//		user.username = USERresult;
//		users.push_back(user); // 벡터에 유저 추가
//	}
//	// USER PASSWORDR값입니다
//	char PASSWORDresult[100];
//	while (OCIStmtFetch2(PW, error, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
//		UserDATA user;
//		OCIAttrGet(defines, OCI_HTYPE_DEFINE, PASSWORDresult, 0, OCI_ATTR_PASSWORD, error);
//		user.password = PASSWORDresult;
//		users.push_back(user); // 벡터에 비밀번호 추가
//	}
//}

BOOL CMFCSocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	dataSocket.Create(port);

	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_EMO_STAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EMO_HEART)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EMO_FLOWER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EMO_MUSIC)->EnableWindow(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCSocketClientDlg::OnPaint()
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
HCURSOR CMFCSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//CString 문자열을 잘라 배열에 저장하는 함수
void CMFCSocketClientDlg::SplitString(CString strData, CString Seperator, CStringArray& arr)
{
	CString data = strData;
	int Position = 0;
	CString Token;

	Token = data.Tokenize(Seperator, Position);

	if (Token != L"")
		arr.Add(Token);

	while (!Token.IsEmpty())
	{
		// Get next token.
		Token = data.Tokenize(Seperator, Position);

		if (Token != L"")
		{
			arr.Add(Token);
		}
	}

	data.Empty();
	Token.Empty();
}

void CMFCSocketClientDlg::OnBnClickedButtonLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	if (m_strID.IsEmpty() || m_strPW.IsEmpty())
	{
		AddMessage("아이디와 비밀번호를 올바르게 입력해 주십시오.");
	}
	else
	{
		dataSocket.Connect(strIP, port);

		CString strSend;					//ID PW 병합
		strSend.Format(_T("TYPE:LOGIN{@}%s{@}%s"), m_strID, m_strPW);

		dataSocket.Send(strSend, strSend.GetLength() + 1);
	}
}


void CMFCSocketClientDlg::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	CString strSend;
	strSend.Format(_T("[%s]:%s"), name, m_strSendData);

	dataSocket.Send(strSend, strSend.GetLength() + 1);

	GetDlgItem(IDC_EDIT_SEND_DATA)->SetFocus();
	m_strSendData = "";
	UpdateData(FALSE);

	//AddMessage(strSend); //테스트 때는 자기 자신에게 메세지를 보내기 때문에 임시로 비활성화
}

LRESULT CMFCSocketClientDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	char Rcvdata[1024];
	CDataSocket* pDataSocket = (CDataSocket*)wParam;
	pDataSocket->Receive(Rcvdata, sizeof(Rcvdata));
	CString strMsg;
	strMsg.Format(_T("%hs"), Rcvdata);

	if (strMsg.Find("TYPE:LOGIN_TRUE", 0) == 0)
	{
		AddMessage("로그인 성공");

		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SIGNUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EMO_STAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EMO_HEART)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EMO_FLOWER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EMO_MUSIC)->EnableWindow(TRUE);

		CStringArray arr;
		SplitString(strMsg, "{@}", arr);
		if (arr.GetSize() >= 2)
		{
			name = arr[1];
		}
	}
	else if (strMsg.Find("TYPE:LOGIN_FALSE", 0) == 0)
	{
		AddMessage("로그인 실패");
	}
	else if (strMsg.Find("TYPE:SIGNIN_TRUE", 0) == 0)
	{
		AddMessage("회원가입 성공");
	}
	else if (strMsg.Find("TYPE:SIGNIN_FALSE", 0) == 0)
	{
		AddMessage("회원가입 실패");
	}
	else {
		AddMessage(strMsg);
	}

	UpdateData();

	return 0;
}

LRESULT CMFCSocketClientDlg::OnCloseSocket(WPARAM wParam, LPARAM lparam)
{
	dataSocket.Close();

	AddMessage("서버가 연결을 종료 했습니다.");
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);

	return 0;
}


void CMFCSocketClientDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (dataSocket.m_hSocket != INVALID_SOCKET)
	{
		dataSocket.Close();
		AddMessage("서버와의 접속을 종료합니다.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SIGNUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EMO_STAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EMO_HEART)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EMO_FLOWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EMO_MUSIC)->EnableWindow(FALSE);
	}
	else
		AddMessage("이미 종료되었습니다.");
}

BOOL CMFCSocketClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN && GetFocus()->GetDlgCtrlID() == IDC_EDIT_SEND_DATA)
		{
			OnBnClickedButtonSend();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMFCSocketClientDlg::OnBnClickedButtonSignup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	if (m_strID.IsEmpty() || m_strPW.IsEmpty())
	{
		AddMessage("아이디 또는 비밀번호 값이 없습니다.");
	}
	else {
		dataSocket.Connect(strIP, port);

		CString strSend;
		strSend.Format(_T("TYPE:SIGNIN{@}%s{@}%s"), m_strID, m_strPW);

		dataSocket.Send(strSend, strSend.GetLength() + 1);
	}
}

void CMFCSocketClientDlg::OnBnClickedButtonEmoStar()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString emo;
	auto uni = TEXT("☆");

	m_edSendData.GetWindowText(emo);
	m_edSendData.SetWindowText(emo+uni);
	
	UpdateData();
}

void CMFCSocketClientDlg::OnBnClickedButtonEmoHeart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CString emo;
	auto uni = TEXT("♥");

	m_edSendData.GetWindowText(emo);
	m_edSendData.SetWindowText(emo + uni);

	UpdateData();
}

void CMFCSocketClientDlg::OnBnClickedButtonEmoFlower()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString emo;
	auto uni = TEXT("※");

	m_edSendData.GetWindowText(emo);
	m_edSendData.SetWindowText(emo + uni);

	UpdateData();
}


void CMFCSocketClientDlg::OnBnClickedButtonEmoMusic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString emo;
	auto uni = TEXT("♬");

	m_edSendData.GetWindowText(emo);
	m_edSendData.SetWindowText(emo + uni);

	UpdateData();
}
