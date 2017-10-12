
// DetectIconDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DetectIcon.h"
#include "DetectIconDlg.h"
#include "afxdialogex.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDetectIconDlg dialog



CDetectIconDlg::CDetectIconDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetectIconDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDIcon = NULL;
	m_icon_count = 0;
}

void CDetectIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_ctrl_picture);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_btn_browse);
	DDX_Control(pDX, IDC_STATIC_ICON, m_ctrl_icon);
}

BEGIN_MESSAGE_MAP(CDetectIconDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CDetectIconDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_FIND_NEXT_ICON, &CDetectIconDlg::OnBnClickedBtnFindNextIcon)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SAVE_ICON, &CDetectIconDlg::OnBnClickedBtnSaveIcon)
END_MESSAGE_MAP()


// CDetectIconDlg message handlers

BOOL CDetectIconDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDetectIconDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDetectIconDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDetectIconDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectIconDlg::OnBnClickedBtnBrowse()
{
	// TODO: Add your control notification handler code here
	CString filter = _T("image files (*.bmp; *.jpg; *.jpeg; *.png) |*.bmp;*.jpg;*.jpeg;*.png|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.bitmap"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK){
		m_pDIcon = new DetectIterface();
		cs_path = dlg.GetPathName();		
		s_img_path = convertCStringToString(cs_path);
		//image_path.Load(cs_path);
		m_ctrl_picture.Load(cs_path);
		m_pDIcon->setImagePath(s_img_path);
		m_pDIcon->onLoad();
		OnBnClickedBtnFindNextIcon();
	}
	dlg.DestroyWindow();
}

void CDetectIconDlg::OnBnClickedBtnFindNextIcon()
{
	// TODO: Add your control notification handler code here
	if (m_pDIcon){
		vector<SRect> srect = m_pDIcon->getListIcon();
		if (m_icon_count < srect.size()){
			int left, top, right, bottom;
			left = srect[m_icon_count].point_start.x;
			top = srect[m_icon_count].point_start.y;
			right = srect[m_icon_count].point_end.x;
			bottom = srect[m_icon_count].point_end.y;
			m_ctrl_icon.Load(cs_path, CRect(left, top, right, bottom));
			m_icon_count++;
			showIconInfo(CRect(left, top, right, bottom));
		}
		else{
			m_icon_count = 0;
		}
	}
}

void CDetectIconDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (m_pDIcon){
		delete m_pDIcon;
		m_pDIcon = NULL;
	}
	CDialogEx::OnClose();
}

string CDetectIconDlg::convertCStringToString(CString cs_text)
{
	CT2CA ConvertToAscii(cs_text);
	string s_text(ConvertToAscii);
	return s_text;
}

void CDetectIconDlg::showIconInfo(CRect icon_info)
{
	CString cs_text;
	cs_text.Format(_T("Left :%d"), icon_info.left);
	SetDlgItemText(IDC_TEXT_LEFT, cs_text);

	cs_text.Format(_T("Right :%d"), icon_info.right);
	SetDlgItemText(IDC_TEXT_RIGHT, cs_text);

	cs_text.Format(_T("Top :%d"), icon_info.top);
	SetDlgItemText(IDC_TEXT_TOP, cs_text);

	cs_text.Format(_T("Bottom :%d"), icon_info.bottom);
	SetDlgItemText(IDC_TEXT_BOTTOM, cs_text);

	cs_text.Format(_T("Width :%d"), icon_info.Width());
	SetDlgItemText(IDC_TEXT_WIDTH, cs_text);

	cs_text.Format(_T("Height :%d"), icon_info.Height());
	SetDlgItemText(IDC_TEXT_HEIGHT, cs_text);
}

void CDetectIconDlg::OnBnClickedBtnSaveIcon()
{
	// TODO: Add your control notification handler code here
	if (m_pDIcon){
		char buff[20];
		sprintf_s(buff, "image line %d.jpg", (m_icon_count - 1));
		m_pDIcon->saveImage(buff, (m_icon_count -1));
	}
}

