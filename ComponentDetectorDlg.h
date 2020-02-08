
// ComponentDetectorDlg.h : header file
//
#include <iostream>
#include <strsafe.h>
#pragma once
#include "Resource.h"


// CComponentDetectorDlg dialog
class CComponentDetectorDlg : public CDialogEx
{
// Construction
public:
	CComponentDetectorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPONENTDETECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic mMainPictureControl;
	CBitmap Bmp1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
