
// ComponentDetectorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ComponentDetector.h"
#include "ComponentDetectorDlg.h"
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComponentDetectorDlg dialog



CComponentDetectorDlg::CComponentDetectorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPONENTDETECTOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComponentDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CComponentDetectorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CComponentDetectorDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BT_SHOW_VID, &CComponentDetectorDlg::OnBnClickedBtShowVid)
	ON_BN_CLICKED(IDC_BUTTON1, &CComponentDetectorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CComponentDetectorDlg message handlers

BOOL CComponentDetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CComponentDetectorDlg::OnPaint()
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
HCURSOR CComponentDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





	/*

{
	AfxMessageBox((LPCTSTR)"shit");

	cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(1);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return;
	}

	cv::UMat bgr_frame, gray, canny;

	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;

		cv::imshow("raw", bgr_frame);

		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", gray);

		cv::Canny(gray, canny, 10, 100, 3, true);
		cv::imshow("canny", canny);

		char c = cv::waitKey(10);
		if (c == 27) break;
	}

	cap.release();
	return;

}
	*/



void CComponentDetectorDlg::OnBnClickedButton1()
{




	AfxMessageBox((LPCTSTR)"shit");

	cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);

	
	cv::VideoCapture cap;
	cap.open(1);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return;
	}

	cv::UMat bgr_frame, gray, canny;

	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;

		cv::imshow("raw", bgr_frame);

		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", gray);

		cv::Canny(gray, canny, 10, 100, 3, true);
		cv::imshow("canny", canny);

		char c = cv::waitKey(10);
		if (c == 27) break;
	}

	cap.release();
	return;
}
