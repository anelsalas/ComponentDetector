
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
#include <opencv2/imgcodecs.hpp>

#include "MyHogDetector.h"
#include "MyConvexHull.h"
#include "MyAproximateSquares.h"

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
	DDX_Control(pDX, IDC_STATIC_MAIN_PICTURE_CONTROL, mMainPictureControl);
}

BEGIN_MESSAGE_MAP(CComponentDetectorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CComponentDetectorDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CComponentDetectorDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON2, &CComponentDetectorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CComponentDetectorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CComponentDetectorDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CComponentDetectorDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CComponentDetectorDlg::OnBnClickedButton6)
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



// Canny Video Capture
void CComponentDetectorDlg::OnBnClickedButton1()
{

	//AfxMessageBox((LPCTSTR)"shit");

	//cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("canny_sharr", cv::WINDOW_AUTOSIZE);
	CBitmap mainbitmap;

	int edgeThresh = 1;
	int edgeThreshScharr = 1;
	
	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return;
	}

	cv::Mat bgr_frame,blured,sharred, gray, canny, edge1, edge2, image, cedge;
	cv::Mat dx, dy;
	MyHogDetector Mydetector;
	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;


		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		blur(gray, blured, cv::Size(3, 3));



		cv::Canny(blured, canny, 87, 400, 3, true);

		int64 t = cv::getTickCount();
		vector<cv::Rect> found = Mydetector.detect(canny);
		t = cv::getTickCount() - t;

		// show the window
		{
			ostringstream buf;
			buf << "Mode: " << Mydetector.modeName() << " ||| "
				<< "FPS: " << fixed << setprecision(1) << (cv::getTickFrequency() / (double)t)
				<< "Found: " << found.size();
			putText(bgr_frame, buf.str(), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
		}
		for (vector<cv::Rect>::iterator i = found.begin(); i != found.end(); ++i)
		{
			cv::Rect& r = *i;
			Mydetector.adjustRect(r);
			rectangle(bgr_frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
		}


		//cv::Scharr(blured,sharred,)


	/// Canny detector with scharr
/*
		cv::Scharr(blured, dx, CV_16SC1, 1, 0);
		cv::Scharr(blured, dy, CV_16SC1, 0, 1);
		Canny(dx, dy, edge2, edgeThreshScharr, edgeThreshScharr * 3);

		/// Using Canny's output as a mask, we display our result
		cedge = cv::Scalar::all(0);
		//image.copyTo(cedge, edge2);
		cv::imshow("canny scharred", edge2);
*/
		//cv::imshow("raw", bgr_frame);
		cv::imshow("canny_sharr", canny);
		cv::imshow("canny", bgr_frame);

		char key = cv::waitKey(10);
		if (key == 27 || key == 'q') break;// ESC
	}

	cap.release();
	return;
}
/*
void CComponentDetectorDlg::DrawCVImage(System::Windows::Forms::Control^ control, cv::Mat& colorImage)
{
	System::Drawing::Graphics^ graphics = control->CreateGraphics();
	System::IntPtr ptr(colorImage.ptr());
	System::Drawing::Bitmap^ b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
	System::Drawing::RectangleF rect(0, 0, control->Width, control->Height);
	graphics->DrawImage(b, rect);
	delete graphics;

	////////////// other shit to try to put the mat into a windows frame
			mainbitmap.CreateBitmap(canny.rows,
			canny.cols,
			canny.channels(),
			(UINT) canny.size.p,  //cols * canny.rows,
			canny.data);


		mainbitmap.LoadBitmap(IDC_STATIC_MAIN_PICTURE_CONTROL);
		this->UpdateWindow();

		//STATIC_MAIN_PICTURE_CONTROL
}
*/


void CComponentDetectorDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

// Convex Hull Video Capture
void CComponentDetectorDlg::OnBnClickedButton2()
{
	cv::namedWindow("ConvexHull", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return;
	}

	cv::Mat bgr_frame, blured, threshold_output, gray, chull,canny, convexHull;
	
	for (;;)
	{
		MyConvexHull myDetectConvHull;
		cap >> bgr_frame;
		//bgr_frame.copyTo(gray);
		if (bgr_frame.empty()) break;
		uint16_t found(0);
		
		int64 t = cv::getTickCount();
		myDetectConvHull.detect( bgr_frame, convexHull, found);

/*
		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		cv::blur(gray, blured, cv::Size(3, 3));
		cv::Canny(blured, canny, 87, 400, 3, true);
		cv::threshold(canny, threshold_output, 50, 255, cv::THRESH_BINARY); // apply binary thresholding

		vector< vector<cv::Point> > contours; // list of contour points
		vector<cv::Vec4i> hierarchy;
		// find contours
		cv::findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));


		// create hull array for convex hull points
		vector< vector<cv::Point> > hull(contours.size());
		for (int i = 0; i < contours.size(); i++)
			cv::convexHull(cv::Mat(contours[i]), hull[i], false);

		for (int i = 0; i < contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(0, 0, 255); // blue - color for convex hull
			// draw ith convex hull
			drawContours(bgr_frame, hull, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
		}
*/
		t = cv::getTickCount() - t;

		ostringstream buf;
		buf << "Convex Hull" 
			<< " FPS: " << fixed << setprecision(1) << (cv::getTickFrequency() / (double)t) << " ||| "
			<< "Found: " << found;
		putText(convexHull, buf.str(), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);


		cv::imshow("ConvexHull", convexHull );
		//cv::imshow("gray", canny);

		char key = cv::waitKey(10);
		if (key == 27 || key == 'q') break;// ESC
	}

	cap.release();
	return;

}

// Generalized Hugh Video capture
void CComponentDetectorDlg::OnBnClickedButton3()
{
	cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Generalized Hugh", cv::WINDOW_AUTOSIZE);

	CBitmap mainbitmap;

	cv::VideoCapture cap;
	cap.open(1);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return;
	}

	cv::Mat bgr_frame, blured, threshold_output, gray, canny, hugh;
	cv::Mat dx, dy;
	MyHogDetector Mydetector;
	for (;;)
	{
		int64 t = cv::getTickCount();
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;


		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		cv::blur(gray, blured, cv::Size(3, 3));
		cv::Canny(blured, canny, 87, 400, 3, true);
		cv::threshold(canny, threshold_output, 50, 255, cv::THRESH_BINARY); // apply binary thresholding

		vector< vector<cv::Point> > contours; // list of contour points
		vector<cv::Vec4i> hierarchy;
		// find contours
		cv::findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));


		//vector<int> hull;


		// create hull array for convex hull points
		vector< vector<cv::Point> > hull(contours.size());
		for (int i = 0; i < contours.size(); i++)
			cv::convexHull(cv::Mat(contours[i]), hull[i], false);


		//drawContours(canny, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());


		for (int i = 0; i < contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(0, 0, 255); // blue - color for convex hull
			// draw ith convex hull
			drawContours(bgr_frame, hull, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
		}
		t = cv::getTickCount() - t;

		// show the window
		{
			ostringstream buf;
			buf << "Mode: " << Mydetector.modeName() << " ||| "
				<< "FPS: " << fixed << setprecision(1) << (cv::getTickFrequency() / (double)t)
				<< "Found: " << hull.size();
			putText(bgr_frame, buf.str(), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
		}
		/*
		for (vector<cv::Rect>::iterator i = found.begin(); i != found.end(); ++i)
		{
			cv::Rect& r = *i;
			Mydetector.adjustRect(r);
			rectangle(bgr_frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
		}
		*/
		cv::imshow("raw", bgr_frame);
		cv::imshow("Generalized Hugh", hugh);

		char key = cv::waitKey(10);
		if (key == 27 || key == 'q') break;// ESC
	}

	cap.release();
	return;

}

// Approximate Squarres Video Capture
void CComponentDetectorDlg::OnBnClickedButton4()
{
	cv::namedWindow("AproxSquares", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	//bgr_frame = cv::imread("E:\\Source\\cpp\\ComponentDetector\\SMDParts2.jpg");
	cv::VideoCapture cap;
	cap.open(0);
	cv::Mat bgr_frame;

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return;
	}

	cv::Mat blured, threshold_output, gray, chull, canny, apSqs;

	for (;;)
	{
		MyAproximateSquares myDetectAproxSqs;
		cap >> bgr_frame;
		//bgr_frame.copyTo(gray);
		if (bgr_frame.empty()) break;
		uint16_t found(0);

		int64 t = cv::getTickCount();
		myDetectAproxSqs.detect(bgr_frame, apSqs, found);

		t = cv::getTickCount() - t;

		ostringstream buf;
		buf << "Convex Hull"
			<< " FPS: " << fixed << setprecision(1) << (cv::getTickFrequency() / (double)t) << " ||| "
			<< "Found: " << found;
		putText(apSqs, buf.str(), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);


		cv::imshow("AproxSquares", apSqs);
		//cv::imshow("gray", canny);

		char key = cv::waitKey(10);
		if (key == 27 || key == 'q') break;// ESC
	}

	//cap.release();
	return;
}

// Convex Hull Image
void CComponentDetectorDlg::OnBnClickedButton5()
{
	cv::namedWindow("ConvexHull", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);



	cv::Mat bgr_frame, blured, threshold_output, gray, chull, canny, convexHull;
	bgr_frame = cv::imread("E:\\Source\\cpp\\ComponentDetector\\SMDParts2.jpg");
	for (;;)
	{
		MyConvexHull myDetectConvHull;
		//bgr_frame.copyTo(gray);
		if (bgr_frame.empty()) break;
		uint16_t found(0);

		int64 t = cv::getTickCount();
		myDetectConvHull.detect(bgr_frame, convexHull, found);

		/*
				cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
				cv::blur(gray, blured, cv::Size(3, 3));
				cv::Canny(blured, canny, 87, 400, 3, true);
				cv::threshold(canny, threshold_output, 50, 255, cv::THRESH_BINARY); // apply binary thresholding

				vector< vector<cv::Point> > contours; // list of contour points
				vector<cv::Vec4i> hierarchy;
				// find contours
				cv::findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));


				// create hull array for convex hull points
				vector< vector<cv::Point> > hull(contours.size());
				for (int i = 0; i < contours.size(); i++)
					cv::convexHull(cv::Mat(contours[i]), hull[i], false);

				for (int i = 0; i < contours.size(); i++)
				{
					cv::Scalar color = cv::Scalar(0, 0, 255); // blue - color for convex hull
					// draw ith convex hull
					drawContours(bgr_frame, hull, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
				}
		*/
		t = cv::getTickCount() - t;

		ostringstream buf;
		buf << "Convex Hull"
			<< " FPS: " << fixed << setprecision(1) << (cv::getTickFrequency() / (double)t) << " ||| "
			<< "Found: " << found;
		putText(convexHull, buf.str(), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);


		cv::imshow("ConvexHull", convexHull);
		//cv::imshow("gray", canny);

		char key = cv::waitKey(10);
		if (key == 27 || key == 'q') break;// ESC
	}

	return;
}

// Aproximate Squares Image
void CComponentDetectorDlg::OnBnClickedButton6()
{
	cv::namedWindow("AproxSquares", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	cv::Mat bgr_frame;
	bgr_frame = cv::imread("E:\\Source\\cpp\\ComponentDetector\\SMDParts2.jpg");
	//cv::VideoCapture cap;
	//cap.open(0);

	//if (!cap.isOpened())
	//{
	//	std::cerr << "Couldn't open capture." << std::endl;
	//	return;
	//}

	cv::Mat blured, threshold_output, gray, chull, canny, apSqs;

	for (;;)
	{
		MyAproximateSquares myDetectAproxSqs;
		//cap >> bgr_frame;
		//bgr_frame.copyTo(gray);
		if (bgr_frame.empty()) break;
		uint16_t found(0);

		int64 t = cv::getTickCount();
		myDetectAproxSqs.detect(bgr_frame, apSqs, found);

		t = cv::getTickCount() - t;

		ostringstream buf;
		buf << "Convex Hull"
			<< " FPS: " << fixed << setprecision(1) << (cv::getTickFrequency() / (double)t) << " ||| "
			<< "Found: " << found;
		putText(apSqs, buf.str(), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);


		cv::imshow("AproxSquares", apSqs);
		//cv::imshow("gray", canny);

		char key = cv::waitKey(10);
		if (key == 27 || key == 'q') break;// ESC
	}

	//cap.release();
	return;
}
