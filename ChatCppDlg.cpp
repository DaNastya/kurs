// ChatCppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatCppDlg.h"
#include ".\chatcppdlg.h"
#include "CAboutDlg.h"
#include ".\caboutdlg.h"
#include <fstream>
#include <iostream>
#define IDC_BUTTON_SEND3  1018

boolean history_flag = false;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatCppDlg dialog


CChatCppDlg::CChatCppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatCppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatCppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_SERVER, m_ButtonStartServer);
	DDX_Control(pDX, IDC_RADIO_CLIENT, m_ButtonStartClient);
	DDX_Control(pDX, IDC_EDIT_NAME, m_wndName);
	DDX_Control(pDX, IDC_EDIT_IP, m_wndIPAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, m_wndPort);
	DDX_Control(pDX, IDC_EDIT_SEND, m_wndSend);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ButtonSend);
	DDX_Control(pDX, IDC_BUTTON_SEND2, m_ButtonSend2);
	DDX_Control(pDX, IDC_BUTTON_SEND3, m_ButtonSend3);
	DDX_Control(pDX, IDC_BUTTON_STOPCHAT, m_ButtonStopChat);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_wndChat);
	DDX_Control(pDX, IDC_STATIC_COUNTPEOPLE, m_wndCountPeople);
}

BEGIN_MESSAGE_MAP(CChatCppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_SERVER, OnBnClickedRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, OnBnClickedRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_STOPCHAT, OnBnClickedButtonStopchat)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnBnClickedButtonSend)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_CHAT, &CChatCppDlg::OnEnChangeEditChat)
	ON_BN_CLICKED(IDC_BUTTON_SEND2, &CChatCppDlg::OnBnClickedButtonSend2)
	ON_BN_CLICKED(IDC_BUTTON_SEND3, &CChatCppDlg::OnBnClickedButtonSend3)
END_MESSAGE_MAP()


// CChatCppDlg message handlers

BOOL CChatCppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	//////////////////////////////////////////////////////////////////////////////
	// ����������� ������������� ��� ����
	SetWindowText("��� ����!");

	// ����������� ���������� �������� ��������.
	m_wndName.SetLimitText(12); 


	m_wndName.SetWindowText(g_EmptyName);


	// ����������� ���������� �������� � ���������.
	m_wndSend.SetLimitText(200); 

	// // ����� �� ��������� ��� ������������.
	m_wndIPAddress.SetWindowText("127.0.0.1"); 

	// ���� �� ���������.
	m_wndPort.SetWindowText("500"); 

	m_ButtonStopChat.SetWindowText(g_strExitFromChat);

	m_mainSocket.m_pParent = this;

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatCppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CChatCppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatCppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ��������� ������
void CChatCppDlg::OnBnClickedRadioServer()
{
	
	// �������� �������������������� ������� �������:
	// ���� ������ �� � ��������� �������,
	// ���� ����� � ������ (�.�. ������ � ������� ������� ����� �������� ��������),
	// ����� �������� ���� � ���� ��� ����� ��� ����������� �������.
	if(m_ButtonStartServer.GetCheck() != BST_CHECKED) return;	

	if(m_mainSocket.m_hSocket != INVALID_SOCKET) return;

	if(QueryName() == false)
	{
		AfxMessageBox("������� ���� ��� ��� ����!");
		StopChat();
		return;
	}

	CString strPort;
	m_wndPort.GetWindowText(strPort);


	// ����� CAsyncSocket �������� ��������� �������� ������,
	// ��������� � ������� Create() ���������������� �������� 
	// ������ � ���������� ��� � ����� �� IP ������� ��������� �� ����������.
	// atoi ������������  ������ ansi to int
	if(m_mainSocket.Create(atoi(strPort)) == FALSE)
	{
		AfxMessageBox("������ �������� �������!\n�������� ������ ���� ��� ������������!");
		StopChat();
	}
	else
	{
		// ������ ����� � ����� ������������� �������� �� �����������.
		if(m_mainSocket.Listen() == FALSE)
		{
			StopChat();
		}
		else
		{
			// ���� ��� � �������.
			DisabledControl(true);
			SetWindowText("������ ���� �����������!");
			// � ���� ������ ������.
			m_wndCountPeople.SetWindowText("� ���� 1 ���.");
		}
	}
}

// ��������� �������
void CChatCppDlg::OnBnClickedRadioClient()
{

	// �������� �������������������� ������� �������:
	// ���� ������ �� � ��������� �������,
	// ���� ����� � ������ (�.�. ������ � ������� ������� ����� �������� ��������),
	// ����� �������� ���� � ���� ��� ����� ��� ����������� �������..
	// � ���� ������� ������� �� ������.

	if(m_ButtonStartClient.GetCheck() != BST_CHECKED) return;

	if(m_mainSocket.m_hSocket != INVALID_SOCKET) return;

	if(CChatCppDlg::QueryName() == false)
	{
		AfxMessageBox("������� ���� ��� ��� ����!");
		StopChat();
		return;
	}


	// ����� CAsyncSocket �������� ��������� �������� ������,
	// ��������� � ������� Create() ���������������� �������� 
	// ������ � ���������� ��� � ����� �� IP ������� ��������� �� ����������.
	if(m_mainSocket.Create() == TRUE)
	{
		CString strAddress;
		m_wndIPAddress.GetWindowText(strAddress);
		CString strPort;
		m_wndPort.GetWindowText(strPort);
		if(m_mainSocket.Connect(strAddress, atoi(strPort)) == FALSE)
		{
			// � ������������ ������ ��� ���� ������
			// ��������� ��� �������� ������� �����������,
			// �.�. ����������� �������� �������.
			if(GetLastError() == WSAEWOULDBLOCK)
			{
				DisabledControl(false);
			}
			else
			{
				// ���� �����-���� ������ ��������,
				// �������� ���������� � �������������� ���������,
				// ������� � ��������� ������� ����������.
				StopChat();
			}
		}
	}

}


// ������ ������ "����� �� ����".
void CChatCppDlg::OnBnClickedButtonStopchat()
{
	StopChat();
}

// ��������� ������ � ����������� ��� ������ 
// ���������� � ������ ������� ��� �������.
// ���� ������� - �������� ���������� �� 
// ���������� ������� "������������" ������.
void CChatCppDlg::DisabledControl(bool server)
{
	// �������.
	m_wndIPAddress.EnableWindow(FALSE);
	m_wndPort.EnableWindow(FALSE);
	m_ButtonSend.EnableWindow(FALSE);
	m_ButtonSend2.EnableWindow(FALSE);
	m_ButtonSend3.EnableWindow(FALSE);

	if(server == true)
	{
		m_ButtonStopChat.SetWindowText(g_strStopChat);
		m_ButtonStartClient.EnableWindow(FALSE);
	}
	else
	{
		m_ButtonStopChat.SetWindowText(g_strExitFromChat);
		m_ButtonStartServer.EnableWindow(FALSE);
	}

	// ����������.
	// ��������� ����������� ������ �� ����.
	m_ButtonStopChat.EnableWindow(TRUE);
}

// ��������� ������ � ����������� ����� �������� �������.
// ���� ������� - �������� ���������� �� 
// ���������� ������� "������������" ������.
void CChatCppDlg::EnabledControl(void)
{
	// ����������.
	m_wndIPAddress.EnableWindow(TRUE);
	m_wndPort.EnableWindow(TRUE);
	m_ButtonStartClient.EnableWindow(TRUE);
	m_ButtonStartServer.EnableWindow(TRUE);
	

	// �������.
	m_ButtonStopChat.EnableWindow(FALSE);
	m_ButtonSend.EnableWindow(FALSE);
	m_ButtonSend2.EnableWindow(FALSE);
	m_ButtonSend3.EnableWindow(FALSE);

}

// ��������� ������� �� �����������
void CChatCppDlg::OnAccept(void)
{
	CSock* pSock = new CSock;
	pSock->m_pParent = this;

	// ���� ��� � ������� ������� ������� ����� � ������ 
	// ������������ ������� �������.
	if(m_mainSocket.Accept(*pSock) == TRUE)
	{
		m_vecSockets.push_back(pSock);
		m_ButtonSend.EnableWindow(TRUE);
		m_ButtonSend2.EnableWindow(TRUE);
		m_ButtonSend3.EnableWindow(TRUE);
		SendCountPeople();

		SetWindowText("���� ��������!");
	}
	else 
	{
		delete pSock;
	}
}


// ����� �� ����,
// ���� ��� ��������� �� ������� �������,
// �� ��� ������ ��������� ����.
void CChatCppDlg::StopChat(void)
{
	// �������� ������ �� ���������� �� ����.
	SendDisconnect();

	m_mainSocket.Close();
	for(int i = 0; i < (int)m_vecSockets.size(); i++)
	{
		m_vecSockets[i]->Close();
		delete m_vecSockets[i];
	}
	// ������� ������ �� �������� ���������.
	m_vecSockets.clear();

	m_ButtonStartServer.SetCheck(BST_UNCHECKED);
	m_ButtonStartClient.SetCheck(BST_UNCHECKED);

	// �������� ������ � ���������� ���
	// ��������� �������.
	EnabledControl();

	// � ���� ��� ������.
	m_wndCountPeople.SetWindowText("� ���� 0 ���.");
	SetWindowText("��� ����!");
}


// �������� ��������������� ���������.
void CChatCppDlg::OnBnClickedButtonSend()
{
	CString strChat;
	
	m_wndSend.GetWindowText(strChat);

	SendToChat(strChat);

}

// ���������� ��������� �� ���� ����.
void CChatCppDlg::OnReceive(void)
{
	
	SENDBUFFER sb;
	if(m_ButtonStartServer.GetCheck() == BST_CHECKED)
	{
		// ������ ����� ������� �������� ��� ��������� ���������.
		for(int index = 0; index < (int)m_vecSockets.size(); index++)
		{
			m_vecSockets[index]->Receive(&sb, sizeof(SENDBUFFER));
			// ���� ���-�� ����������, ������� ����� ������� 
			// �� ������ ��������� ��������.
			if(sb.typemessage == m_TypeMessage::tmDisconnect)
			{
				m_vecSockets[index]->Close();
				delete m_vecSockets[index];
				m_vecSockets.erase(m_vecSockets.begin() + index);
				
				SendCountPeople();
				
				// ������� ��������� ������� ��������� � ���� ������ ��������, 
				// ��� �������, ������ ����� �������� � ������� ��������� � 
				// ���� ������ ����� ������.
				SendBuffer(sb, false);

				break;
			}
			// ��������� ��������� ������� �� ���� ����.
			if(sb.typemessage == m_TypeMessage::tmChat)
			{

				SendBuffer(sb, false);
				break;
			}
		}
		
	}
	else if(m_ButtonStartClient.GetCheck() == BST_CHECKED)
	{
		m_mainSocket.Receive(&sb, sizeof(SENDBUFFER));
	}

	// ��������� ��������� ��������� �� ������
	// ��� ����.
	switch(sb.typemessage)
	{
	case m_TypeMessage::tmCountPeople:
		{
			m_wndCountPeople.SetWindowText("� ���� " + 
				IntToStr(sb.countpeople) + " ���.");
		}
		break;
	case m_TypeMessage::tmChat:
		{
			
			CString strChat;
			m_wndChat.GetWindowText(strChat);

			strChat +=  CString(sb.name) + ": " + CString(sb.buffer) + "\r\n";
			
			m_wndChat.SetWindowText(strChat);
			int number_line = m_wndChat.GetLineCount();
			m_wndChat.LineScroll(number_line);
			if (m_ButtonStartClient.GetCheck() == BST_CHECKED)
			{
				std::ofstream fout("history.txt", std::ios_base::app);
				fout << sb.name << ": " << sb.buffer << "\r\n";
				fout.close();
			}
			
		}
		break;
	case m_TypeMessage::tmDisconnect:
		{
			CString strChat;
			m_wndChat.GetWindowText(strChat);

			// ���� ������� ��������� �� ��������� ����(���������� �������),
			// ������� ���������� ���������.
			if(sb.stopchat == true)
			{
				StopChat();
				strChat +=  CString(sb.name) + ": ��� ����������!" + "\r\n";
			}
			else
			{
				strChat +=  CString(sb.name) + " - �������(�) ���!" + "\r\n";
			}
			m_wndChat.SetWindowText(strChat);
			int number_line = m_wndChat.GetLineCount();
			m_wndChat.LineScroll(number_line);
			
			
		}
		break;
	default:
		AfxMessageBox("����������� ���������!");
		break;
	}
}

// ��� �������� ���������� �������� � ���
// ���������� �� ���������� ���������.
void CChatCppDlg::OnClose()
{
	StopChat();

	CDialog::OnClose();
}

// ������� ������-��������� � ���.
void CChatCppDlg::SendToChat(CString strMessage)
{
	SENDBUFFER sb;
	
	int len = strMessage.GetLength();
	memcpy(sb.buffer, strMessage.GetBuffer(), sizeof(TCHAR)*len);
	m_wndName.GetWindowText(strMessage);
	len = strMessage.GetLength();
	memcpy(sb.name, strMessage.GetBuffer(), sizeof(TCHAR)*len);
	sb.typemessage = m_TypeMessage::tmChat;

	SendBuffer(sb, true);
}

// ������� ����� ��������������� ��������� � ����.
void CChatCppDlg::SendBuffer(SENDBUFFER sb, bool toserver)
{

	// ���� ����� �� ������, ������ ������ � ���� �������.
	if(m_mainSocket.m_hSocket == INVALID_SOCKET) return;

	if(m_ButtonStartServer.GetCheck() == BST_CHECKED)
	{
		for(int i = 0; i < (int)m_vecSockets.size(); i++)
		{
			int send = m_vecSockets[i]->Send(&sb, sizeof(SENDBUFFER));//���-�� ��� ������������ ���������
			if(send == sizeof(SENDBUFFER))
			{
				m_wndSend.SetWindowText("");
			}
		}
		
		// ���� ��������������� �������� ������������ ������,
		// ��������� ��� ��������� � ��� �� ���� ��������,
		// ���� toserver ��������� ��������� ������ �������
		// ����� �������� � ������ ������� ��. CChatCppDlg::OnReceive(void).
		if(toserver == true )
		{
			if(sb.typemessage == m_TypeMessage::tmChat)
			{
				CString strChat;
				m_wndChat.GetWindowText(strChat);
				strChat += CString(sb.name) + ": " + CString(sb.buffer)  + "\r\n";
				m_wndChat.SetWindowText(strChat);
				int number_line = m_wndChat.GetLineCount();
				m_wndChat.LineScroll(number_line);
			}
			if(sb.typemessage == m_TypeMessage::tmDisconnect)
			{
				CString strChat;
				m_wndChat.GetWindowText(strChat);
				strChat +=  CString(sb.name) + ": " + "��� ����������!"  + "\r\n";
				m_wndChat.SetWindowText(strChat);
				int number_line = m_wndChat.GetLineCount();
				m_wndChat.LineScroll(number_line);
			}
		}

	}
	else if(m_ButtonStartClient.GetCheck() == BST_CHECKED)
	{
		int send = m_mainSocket.Send(&sb, sizeof(SENDBUFFER));
		if(send == sizeof(SENDBUFFER))
			m_wndSend.SetWindowText("");
	}
}


// ��������� � ���������� ��������� �� ���������� �� ����.
void CChatCppDlg::SendDisconnect(void)
{
	SENDBUFFER sb;
	CString s;
	m_wndName.GetWindowText(s);
	int len = s.GetLength();
	memcpy(sb.name, s.GetBuffer(), sizeof(TCHAR)*len);
	sb.typemessage = m_TypeMessage::tmDisconnect;
	
	// ������������ ������� ������������� ���.
	if(m_ButtonStartServer.GetCheck() == BST_CHECKED)
		sb.stopchat = true;

	SendBuffer(sb, true);

}

// ������� �����������, ���������� �� ������� �������.
void CChatCppDlg::OnConnect(BOOL Error)
{
	if(Error == TRUE)
	{
		AfxMessageBox("������� ����������� ���� ����������!\n�������� ������ ��� �� ������!");
		StopChat();
	}
	else
	{
		m_ButtonSend.EnableWindow(TRUE);
		m_ButtonSend2.EnableWindow(TRUE);
		m_ButtonSend3.EnableWindow(TRUE);
		SetWindowText("���� ��������!");
	}
}

// ������ ���������� �������� ���������� ����� � ����.
void CChatCppDlg::SendCountPeople(void)
{
	int countpeople = 1/*������*/ + (int)m_vecSockets.size()/*�������*/;

	//
	m_wndCountPeople.SetWindowText("� ���� " +  IntToStr(countpeople) + " ���.");

	// �������� ��������� � ����������� ����� � ����.
	SENDBUFFER sb;
	sb.typemessage = m_TypeMessage::tmCountPeople;
	sb.countpeople = countpeople;

	for(int i = 0; i < (int)m_vecSockets.size(); i++)
	{
		// ���� �������� ��������� ���������� ����� � ����.
		m_vecSockets[i]->Send(&sb, sizeof(SENDBUFFER));
	}
}

// ������ ����� ��������� ����� ��������� ������.
bool CChatCppDlg::QueryName(void)
{
	CString strName;
	m_wndName.GetWindowText(strName);
	if(strName == g_EmptyName || strName.IsEmpty() == true)
		return false;

	return true;
}



void CChatCppDlg::OnEnChangeEditChat()
{
	// TODO:  ���� ��� ������� ���������� RICHEDIT, �� ������� ���������� �� �����
	// send this notification unless you override the CDialog::OnInitDialog()
	// ������� � ����� CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  �������� ��� �������� ����������
}





void CChatCppDlg::OnBnClickedButtonSend2()
{
	CString strChat;
	std::ifstream fin("history.txt");
	std::string buf;
	if (!fin.is_open()) // ���� ���� �� ������
		AfxMessageBox("���� �� ����� ���� ������!");
	else if(history_flag==false) {
		char buf[218];//����� ��� ������ ������������ �������
		while (fin.getline(buf, 218))
		{
			strChat += CString(buf)+"\r\n";
			m_wndChat.SetWindowText(strChat);
			int number_line = m_wndChat.GetLineCount();
			m_wndChat.LineScroll(number_line);
			history_flag = true;
		}
	}
	fin.close();
}

void CChatCppDlg::OnBnClickedButtonSend3()
{
	std::fstream history("history.txt", std::ios::out);
	history.close();
	m_wndChat.SetWindowText("");
}
