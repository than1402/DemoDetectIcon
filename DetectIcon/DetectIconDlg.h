
// DetectIconDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Config.h"
#include "PictureCtrl.h"
// CDetectIconDlg dialog
class CDetectIconDlg : public CDialogEx
{
// Construction
public:
	CDetectIconDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DETECTICON_DIALOG };

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
	CPictureCtrl m_ctrl_picture;
	CPictureCtrl m_ctrl_icon;
	CButton m_btn_browse;
	CString cs_path;
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnFindNextIcon();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSaveIcon();
public:
	string s_img_path;
	Mat src_root;
	DetectIterface* m_pDIcon;
	int m_icon_count;
public:
	//Function
	string convertCStringToString(CString cs_text);
	void showIconInfo(CRect icon_info);
	
};
