
// EnableEscapeCommandDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "EnableEscapeCommand.h"
#include "EnableEscapeCommandDlg.h"
#include "afxdialogex.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEnableEscapeCommandDlg *pThis;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CEnableEscapeCommandDlg 对话框



CEnableEscapeCommandDlg::CEnableEscapeCommandDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENABLEESCAPECOMMAND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnableEscapeCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VID, m_VID);
	DDX_Control(pDX, IDC_EDIT_PID, m_PID);
	DDX_Control(pDX, IDC_BUTTON_ENABLE, m_BtnEnable);
	DDX_Control(pDX, IDC_BUTTON_DISABLE, m_BtnDisable);
	DDX_Control(pDX, IDC_EDIT_SHOW, m_EditShow);
}

BEGIN_MESSAGE_MAP(CEnableEscapeCommandDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, &CEnableEscapeCommandDlg::OnBnClickedButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE, &CEnableEscapeCommandDlg::OnBnClickedButtonDisable)
END_MESSAGE_MAP()


// CEnableEscapeCommandDlg 消息处理程序

BOOL CEnableEscapeCommandDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pThis = this;
	m_VID.SetWindowText(_T("072F"));
	m_PID.SetWindowText(_T("2200"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEnableEscapeCommandDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEnableEscapeCommandDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEnableEscapeCommandDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void showText(CString text)
{
	pThis->m_EditShow.SetWindowText(text);
}

/* Unicode */
#ifdef UNICODE
typedef wchar_t char_t;
#define strlen_t wcslen
#define sprintf_t wsprintf
#define strcpy_t wcscpy
#define strcmp_t wcscmp

#ifndef _T 
#   define _T(x) L##x
#endif

#else
typedef char char_t;
#define strlen_t strlen
#define sprintf_t sprintf
#define strcpy_t strcpy
#define strcmp_t strcmp

#ifndef _T 
#   define _T(x) x
#endif

#endif
#define USBRootKeyPath "SYSTEM\\CurrentControlSet\\Enum\\USB"
#define EscapeCommandRegName "EscapeCommandEnable"
#define KeyNameMaxSize 255

BOOL EscapeCmdCtrl(WORD vid, WORD pid, BOOL enable)
{
	DWORD ret;
	HKEY hKey = NULL;
	CString path = _T(USBRootKeyPath);
	CString cStr;

	path += "\\VID_";
	cStr.Format(_T("%04X"), vid);
	path += cStr;
	path += "&PID_";
	cStr.Format(_T("%04X"), pid);
	path += cStr;
	showText(path);

	LOG(INFO) << (LPCTSTR)path;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		if (ret == ERROR_FILE_NOT_FOUND) {
			showText(_T("Key not found."));
			return TRUE;
		}
		else {
			showText(_T("Error opening key.\n"));
			return FALSE;
		}
	}

	int i = 0;
	char_t name[KeyNameMaxSize + 1];
	DWORD size;
	HKEY hDstKey;
	BYTE value[4] = {0x00, 0x00, 0x00, 0x00};
	value[0] = (enable ? 0x01 : 0x00);

	while (true) {
		LOG(INFO) << "-------------------";
		LOG(INFO) << i;
		size = sizeof(name);
		ret = RegEnumKeyEx(hKey, i++, name, &size, NULL, NULL, NULL, NULL);
		if (ret == ERROR_NO_MORE_ITEMS)
			break;
		path = name;
		path += "\\Device Parameters";

		LOG(INFO) << (LPCTSTR)path;
		if (RegCreateKeyExW(hKey, path, 0, NULL, 0, /*KEY_WOW64_64KEY | */KEY_ALL_ACCESS, NULL, &hDstKey, NULL) != ERROR_SUCCESS)
		{
			showText(path + _T(" RegCreateKeyExW失败"));
			goto err;
		}
		LOG(INFO) << "RegCreateKeyExW成功";
		if (RegSetValueExW(hDstKey, _T(EscapeCommandRegName), 0, REG_DWORD, value, sizeof(value)) != ERROR_SUCCESS)
		{
			showText(path + _T("写入注册表EscapeCommand失败"));
			goto err;
		}
		LOG(INFO) << "RegSetValueExW成功";
		if (!enable)
			RegDeleteValue(hDstKey, _T(EscapeCommandRegName));
		RegCloseKey(hDstKey);

		path += "\\WUDFUsbccidDriver";
		LOG(INFO) << (LPCTSTR)path;
		if (RegCreateKeyExW(hKey, path, 0, NULL, 0, /*KEY_WOW64_64KEY | */KEY_ALL_ACCESS, NULL, &hDstKey, NULL) != ERROR_SUCCESS)
		{
			showText(path + _T("RegCreateKeyExW失败"));
			goto err;
		}
		LOG(INFO) << "RegCreateKeyExW成功";
		if (RegSetValueExW(hDstKey, _T(EscapeCommandRegName), 0, REG_DWORD, value, sizeof(value)) != ERROR_SUCCESS)
		{
			showText(path + _T("写入WUDF注册表EscapeCommandEnable失败"));
			goto err;
		}
		LOG(INFO) << "RegSetValueExW成功";
		if (!enable)
			RegDeleteValue(hDstKey, _T(EscapeCommandRegName));
		RegCloseKey(hDstKey);
		cStr.Format(_T("%d"), i);
		showText(cStr);
	}
	RegCloseKey(hKey);
	showText(_T("EscapeCommand注册表键值写入成功"));
	return true;
err:
	return false;
}

void CEnableEscapeCommandDlg::OnBnClickedButtonEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cStr;
	WORD vid, pid;

	m_VID.GetWindowText(cStr);
	if (cStr.IsEmpty())
		return;
	vid = _tcstol(cStr, NULL, 16);

	m_PID.GetWindowText(cStr);
	if (cStr.IsEmpty())
		return;
	pid = _tcstol(cStr, NULL, 16);

	EscapeCmdCtrl(vid, pid, true);
}


void CEnableEscapeCommandDlg::OnBnClickedButtonDisable()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cStr;
	WORD vid, pid;

	m_VID.GetWindowText(cStr);
	if (cStr.IsEmpty())
		return;
	vid = _tcstol(cStr, NULL, 16);

	m_PID.GetWindowText(cStr);
	if (cStr.IsEmpty())
		return;
	pid = _tcstol(cStr, NULL, 16);

	EscapeCmdCtrl(vid, pid, false);
}
