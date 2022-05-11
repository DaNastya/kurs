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
	// Необходимая инициализация для чата
	SetWindowText("Нет сети!");

	// Ограничение количества вводимых символов.
	m_wndName.SetLimitText(12); 


	m_wndName.SetWindowText(g_EmptyName);


	// Ограничение количества символов в сообщении.
	m_wndSend.SetLimitText(200); 

	// // Адрес по умолчанию для тестирования.
	m_wndIPAddress.SetWindowText("127.0.0.1"); 

	// Порт по умолчанию.
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


// Запускаем сервер
void CChatCppDlg::OnBnClickedRadioServer()
{
	
	// Контроль несанкционированного запуска сервера:
	// Если кнопка не в состоянии нажатой,
	// если сокет в работе (т.е. только с нулевым сокетом можно начинать работать),
	// очень неудобно если в чате все будут под одинаковыми именами.
	if(m_ButtonStartServer.GetCheck() != BST_CHECKED) return;	

	if(m_mainSocket.m_hSocket != INVALID_SOCKET) return;

	if(QueryName() == false)
	{
		AfxMessageBox("Введите свое имя для чата!");
		StopChat();
		return;
	}

	CString strPort;
	m_wndPort.GetWindowText(strPort);


	// Класс CAsyncSocket упрощает процедуру создания сокета,
	// вкладывая в функцию Create() непосредственное создание 
	// сокета и связывание его с одним из IP адресом доступном на компьютере.
	// atoi конвертирует  строку ansi to int
	if(m_mainSocket.Create(atoi(strPort)) == FALSE)
	{
		AfxMessageBox("Ошибка создания сервера!\nВозможно данный порт уже используется!");
		StopChat();
	}
	else
	{
		// Вводим сокет в режим прослушивания запросов на подключения.
		if(m_mainSocket.Listen() == FALSE)
		{
			StopChat();
		}
		else
		{
			// Если все в порядке.
			DisabledControl(true);
			SetWindowText("Сервер ждет подключения!");
			// В чате только сервер.
			m_wndCountPeople.SetWindowText("В чате 1 чел.");
		}
	}
}

// Запускаем клиента
void CChatCppDlg::OnBnClickedRadioClient()
{

	// Контроль несанкционированного запуска клиента:
	// Если кнопка не в состоянии нажатой,
	// если сокет в работе (т.е. только с нулевым сокетом можно начинать работать),
	// очень неудобно если в чате все будут под одинаковыми именами..
	// В этих случаях выходим из метода.

	if(m_ButtonStartClient.GetCheck() != BST_CHECKED) return;

	if(m_mainSocket.m_hSocket != INVALID_SOCKET) return;

	if(CChatCppDlg::QueryName() == false)
	{
		AfxMessageBox("Введите свое имя для чата!");
		StopChat();
		return;
	}


	// Класс CAsyncSocket упрощает процедуру создания сокета,
	// вкладывая в функцию Create() непосредственное создание 
	// сокета и связывание его с одним из IP адресом доступном на компьютере.
	if(m_mainSocket.Create() == TRUE)
	{
		CString strAddress;
		m_wndIPAddress.GetWindowText(strAddress);
		CString strPort;
		m_wndPort.GetWindowText(strPort);
		if(m_mainSocket.Connect(strAddress, atoi(strPort)) == FALSE)
		{
			// В ассинхронном режиме код этой ошибки
			// считается как ожидание события подключения,
			// т.е. практически успешный возврат.
			if(GetLastError() == WSAEWOULDBLOCK)
			{
				DisabledControl(false);
			}
			else
			{
				// Если какая-либо ошибка возникла,
				// приводим приложение в первоначальное состояние,
				// готовое к следующей попытке соединения.
				StopChat();
			}
		}
	}

}


// Нажали кнопку "Выйти из чата".
void CChatCppDlg::OnBnClickedButtonStopchat()
{
	StopChat();
}

// Запрещает доступ к управлениям при работе 
// приложения в режиме сервера или клиента.
// Цель запрета - избежать исключения от 
// случайного нажатия "неправильных" кнопок.
void CChatCppDlg::DisabledControl(bool server)
{
	// Запреты.
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

	// Разрешения.
	// Разрешить возможность выхода из чата.
	m_ButtonStopChat.EnableWindow(TRUE);
}

// Разрешить доступ к управлениям после закрытия сокетов.
// Цель запрета - избежать исключения от 
// случайного нажатия "неправильных" кнопок.
void CChatCppDlg::EnabledControl(void)
{
	// Разрешения.
	m_wndIPAddress.EnableWindow(TRUE);
	m_wndPort.EnableWindow(TRUE);
	m_ButtonStartClient.EnableWindow(TRUE);
	m_ButtonStartServer.EnableWindow(TRUE);
	

	// Запреты.
	m_ButtonStopChat.EnableWindow(FALSE);
	m_ButtonSend.EnableWindow(FALSE);
	m_ButtonSend2.EnableWindow(FALSE);
	m_ButtonSend3.EnableWindow(FALSE);

}

// Принимаем запросы на подключения
void CChatCppDlg::OnAccept(void)
{
	CSock* pSock = new CSock;
	pSock->m_pParent = this;

	// Если все в порядке добавим рабочий сокет в список 
	// подключенных рабочих сокетов.
	if(m_mainSocket.Accept(*pSock) == TRUE)
	{
		m_vecSockets.push_back(pSock);
		m_ButtonSend.EnableWindow(TRUE);
		m_ButtonSend2.EnableWindow(TRUE);
		m_ButtonSend3.EnableWindow(TRUE);
		SendCountPeople();

		SetWindowText("Сеть работает!");
	}
	else 
	{
		delete pSock;
	}
}


// Выход из чата,
// если это сработало на стороне сервера,
// то это полная остановка чата.
void CChatCppDlg::StopChat(void)
{
	// Отсылаем сигнал об отключении от чата.
	SendDisconnect();

	m_mainSocket.Close();
	for(int i = 0; i < (int)m_vecSockets.size(); i++)
	{
		m_vecSockets[i]->Close();
		delete m_vecSockets[i];
	}
	// Очистим вектор от ненужных элементов.
	m_vecSockets.clear();

	m_ButtonStartServer.SetCheck(BST_UNCHECKED);
	m_ButtonStartClient.SetCheck(BST_UNCHECKED);

	// Разрешим доступ к управлению для
	// повторных попыток.
	EnabledControl();

	// В чате нет никого.
	m_wndCountPeople.SetWindowText("В чате 0 чел.");
	SetWindowText("Нет сети!");
}


// Отправка подготовленного сообщения.
void CChatCppDlg::OnBnClickedButtonSend()
{
	CString strChat;
	
	m_wndSend.GetWindowText(strChat);

	SendToChat(strChat);

}

// Извлечение сообщений из сети чата.
void CChatCppDlg::OnReceive(void)
{
	
	SENDBUFFER sb;
	if(m_ButtonStartServer.GetCheck() == BST_CHECKED)
	{
		// Сервер несет большую нагрузку при получении сообщений.
		for(int index = 0; index < (int)m_vecSockets.size(); index++)
		{
			m_vecSockets[index]->Receive(&sb, sizeof(SENDBUFFER));
			// Если кто-то отключился, удаляем этого клиента 
			// из списка доступных клиентов.
			if(sb.typemessage == m_TypeMessage::tmDisconnect)
			{
				m_vecSockets[index]->Close();
				delete m_vecSockets[index];
				m_vecSockets.erase(m_vecSockets.begin() + index);
				
				SendCountPeople();
				
				// Отсылка принятого севером сообщения в сеть другим клиентам, 
				// как зеркало, клиент может работать с другими клиентами в 
				// сети только через сервер.
				SendBuffer(sb, false);

				break;
			}
			// Рассылаем сообщения клиента по сети чата.
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

	// Обработка принятого сообщения на основе
	// его типа.
	switch(sb.typemessage)
	{
	case m_TypeMessage::tmCountPeople:
		{
			m_wndCountPeople.SetWindowText("В чате " + 
				IntToStr(sb.countpeople) + " чел.");
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

			// Если принято сообщение об остановки чата(отключении сервера),
			// изменим содержимое сообщения.
			if(sb.stopchat == true)
			{
				StopChat();
				strChat +=  CString(sb.name) + ": Чат остановлен!" + "\r\n";
			}
			else
			{
				strChat +=  CString(sb.name) + " - покинул(а) чат!" + "\r\n";
			}
			m_wndChat.SetWindowText(strChat);
			int number_line = m_wndChat.GetLineCount();
			m_wndChat.LineScroll(number_line);
			
			
		}
		break;
	default:
		AfxMessageBox("Неизвестное сообщение!");
		break;
	}
}

// При закрытии приложения отправим в чат
// информацию об отключении чатующего.
void CChatCppDlg::OnClose()
{
	StopChat();

	CDialog::OnClose();
}

// Послать строку-сообщение в чат.
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

// Послать буфер подготовленного сообщения в сеть.
void CChatCppDlg::SendBuffer(SENDBUFFER sb, bool toserver)
{

	// Если слкет не создан, нечего делать в этой функции.
	if(m_mainSocket.m_hSocket == INVALID_SOCKET) return;

	if(m_ButtonStartServer.GetCheck() == BST_CHECKED)
	{
		for(int i = 0; i < (int)m_vecSockets.size(); i++)
		{
			int send = m_vecSockets[i]->Send(&sb, sizeof(SENDBUFFER));//где-то тут отправляется сообщение
			if(send == sizeof(SENDBUFFER))
			{
				m_wndSend.SetWindowText("");
			}
		}
		
		// Если непосредственно отправку осуществляет сервер,
		// отобразим его сообщение в его же окне отправки,
		// флаг toserver необходим поскольку данная функция
		// может работать в режиме зеркала см. CChatCppDlg::OnReceive(void).
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
				strChat +=  CString(sb.name) + ": " + "Чат остановлен!"  + "\r\n";
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


// Формируем и отправляем сообщение об отключении от сети.
void CChatCppDlg::SendDisconnect(void)
{
	SENDBUFFER sb;
	CString s;
	m_wndName.GetWindowText(s);
	int len = s.GetLength();
	memcpy(sb.name, s.GetBuffer(), sizeof(TCHAR)*len);
	sb.typemessage = m_TypeMessage::tmDisconnect;
	
	// Отсоединение сервера останавливает чат.
	if(m_ButtonStartServer.GetCheck() == BST_CHECKED)
		sb.stopchat = true;

	SendBuffer(sb, true);

}

// Событие подключения, вызывается на стороне клиента.
void CChatCppDlg::OnConnect(BOOL Error)
{
	if(Error == TRUE)
	{
		AfxMessageBox("Попытка подключения была отвергнута!\nВозможно сервер еще не создан!");
		StopChat();
	}
	else
	{
		m_ButtonSend.EnableWindow(TRUE);
		m_ButtonSend2.EnableWindow(TRUE);
		m_ButtonSend3.EnableWindow(TRUE);
		SetWindowText("Сеть работает!");
	}
}

// Сервер отправляет клиентам количество людей в чате.
void CChatCppDlg::SendCountPeople(void)
{
	int countpeople = 1/*сервер*/ + (int)m_vecSockets.size()/*клиенты*/;

	//
	m_wndCountPeople.SetWindowText("В чате " +  IntToStr(countpeople) + " чел.");

	// Отправим сообщение о количествах людей в чате.
	SENDBUFFER sb;
	sb.typemessage = m_TypeMessage::tmCountPeople;
	sb.countpeople = countpeople;

	for(int i = 0; i < (int)m_vecSockets.size(); i++)
	{
		// Всем клиентам рассылаем количество людей в чате.
		m_vecSockets[i]->Send(&sb, sizeof(SENDBUFFER));
	}
}

// Запрос имени чатующего перед созданием сокета.
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
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialog::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}





void CChatCppDlg::OnBnClickedButtonSend2()
{
	CString strChat;
	std::ifstream fin("history.txt");
	std::string buf;
	if (!fin.is_open()) // если файл не открыт
		AfxMessageBox("Файл не может быть открыт!");
	else if(history_flag==false) {
		char buf[218];//буфер для чтения стандартного размера
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
