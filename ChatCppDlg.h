// ChatCppDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ChatCpp.h"
#include "Sock.h"
#include "vector"




const CString g_EmptyName = "Ваше имя?";
const CString g_strStopChat = "Остановить чат";
const CString g_strExitFromChat = "Выход из чата";


struct SENDBUFFER
{
	SENDBUFFER() 
	{
		typemessage = 0; 
		countpeople = 0;
		stopchat = false;
		ZeroMemory(name, sizeof(TCHAR)*14); 
		ZeroMemory(buffer, sizeof(TCHAR)*202);
	}

	int typemessage;
	int countpeople;
	bool stopchat;
	TCHAR name[14];
	TCHAR buffer[202];
};



// CChatCppDlg dialog
class CChatCppDlg : public CDialog
{
// Construction
public:
	CChatCppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHATCPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnBnClickedRadioClient();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonStopchat();
	afx_msg void OnClose();
	
	inline CString IntToStr(int number)
	{
		CString str;
		str.Format("%d", number);
		return str;
	}

	void OnAccept(void);
	void OnConnect(BOOL Error);
	void OnReceive(void);

private:

	// запрещает доступ к управлениям
	void DisabledControl(bool server);

	// разрешить доступ к управлениям после закрытия сокетов
	void EnabledControl(void);

	void StopChat(void);

	// Послать сообщение в чат.
	void SendToChat(CString strMessage);

	void SendBuffer(SENDBUFFER sb, bool toserver);

	void SendDisconnect(void);

	// Сервер отправляет клиентам количество людей в чате.
	void SendCountPeople(void);

	
	CButton m_ButtonSend;
	CButton m_ButtonStartServer;
	CButton m_ButtonStartClient;
	CButton m_ButtonStopChat;
	CButton m_ButtonSend2;
	CButton m_ButtonSend3;
	CEdit m_wndChat;
	CEdit m_wndName;
	CEdit m_wndIPAddress;
	CEdit m_wndPort;
	CEdit m_wndSend;
	CStatic m_wndCountPeople;

	CSock m_mainSocket;
	std::vector<CSock*> m_vecSockets;

	enum m_TypeMessage {tmCountPeople=1, tmChat, tmDisconnect};

	// Запрос имени чатующего.
	bool QueryName(void);
public:
	afx_msg void OnEnChangeEditChat();
	afx_msg void OnBnClickedButtonSend2();

	afx_msg void OnBnClickedButtonSend3();
};
