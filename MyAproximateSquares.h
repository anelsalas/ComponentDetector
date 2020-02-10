#pragma once
#include "pch.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>

using namespace std;

class MyAproximateSquares
{
public:
	MyAproximateSquares() : epsilonpercent(0.01), epsilon(0) {};
	~MyAproximateSquares() {};
	vector<vector<cv::Point> > contours; // list of contour points
	vector<vector<cv::Point> > hull;
	vector<vector<cv::Point> > finalHulls;
	vector<vector<cv::Point> > aproxSquare;
	vector<cv::Vec4i> hierarchy;
	double epsilonpercent;
	double epsilon;

	std::vector <cv::Moments> hullMoments;
	std::vector <cv::Moments> aproxMoments;
	std::vector <cv::Point> hullCentroids;
	std::vector <cv::Point> aproxSqCentroids;

	void detect(cv::Mat inputImage, cv::Mat& outputImage, uint16_t& totalFound)
	{
		//cv::OutputArray outputBinaryArray(;
		//outputBinaryArray.create;
		cv::Mat inimg, blured, canny, outarray;
		inputImage.copyTo(outputImage);
		cv::blur(inputImage, blured, cv::Size(3, 3));
		cv::Canny(blured, canny, 87, 400, 3, true);
		cv::threshold(canny, outarray, 50, 255, cv::THRESH_BINARY); // apply binary thresholding

		// find contours
		cv::findContours(outarray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		// create hull array for convex hull points

		findAproximateSquares();
		findAproxCentroids();

		drawAproxSqCentroids(outputImage);

		// area = M['m00']
		// Remove hulls within hulls
		uint16_t momentIter(0), hullIter(0);
		std::vector < uint16_t > hullsToDelete;
		for (size_t outerIter(0); outerIter < hull.size(); outerIter++)
		{
			// for each hull check if its centroid is inside any other hull
			for (size_t innerIter(outerIter + 1); innerIter < hull.size() - outerIter; innerIter++)
			{
				// returns 1 if inside, or 0 if in the edge
				if (cv::pointPolygonTest(hull[outerIter], hullCentroids[innerIter], false) >= 0)
				{
					if (hullMoments[outerIter].m00 >= hullMoments[innerIter].m00)
						if (std::find(hullsToDelete.begin(), hullsToDelete.end(), outerIter) == hullsToDelete.end())
							hullsToDelete.push_back(outerIter);
						else
							if (std::find(hullsToDelete.begin(), hullsToDelete.end(), innerIter) == hullsToDelete.end())
								hullsToDelete.push_back(innerIter);
				}
				else hullsToDelete.push_back(outerIter);
			}
		}
		ostringstream buf;
		for (int i = 0; i < contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(0, 0, 255); // red - color for convex hull
			cv::Scalar color2 = cv::Scalar(0, 255, 0); // green - color for contours

			//drawContours(outputImage, hull, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
			if (cv::isContourConvex(aproxSquare[i]))
				drawContours(outputImage, aproxSquare, i, color2, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
			//buf << i;
			//putText(outputImage, buf.str(),hull[i].[0], cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
		}
		totalFound = hull.size();

	}
	void findHullCentroids()
	{
		// find centroids
		cv::Moments hullMom;
		for (auto hulls : hull)
		{
			hullMom = cv::moments(hulls, false);
			hullCentroids.push_back(cv::Point(hullMom.m10 / hullMom.m00, hullMom.m01 / hullMom.m00));
			hullMoments.push_back(hullMom);
		}
	}

	void findAproxCentroids()
	{
		// find centroids
		cv::Moments aproxMom;
		for (auto aproxSquaress : aproxSquare)
		{
			aproxMom = cv::moments(aproxSquaress, false);
			aproxSqCentroids.push_back(cv::Point(aproxMom.m10 / aproxMom.m00, aproxMom.m01 / aproxMom.m00));
			aproxMoments.push_back(aproxMom);
		}
	}

	void findAproximateSquares()
	{
		aproxSquare.resize(contours.size());

		for (size_t i = 0; i < contours.size(); i++)
		{
			epsilon = epsilonpercent * cv::arcLength(contours[i], true);
			cv::approxPolyDP(cv::Mat(contours[i]), aproxSquare[i], epsilon, true);
		}
	}

	void drawHullCentroids(cv::Mat& outImg)
	{
		for (auto centroits : hullCentroids)
		{
			cv::circle(outImg, centroits, 1, cv::Scalar(0, 0, 255), 1, 8, 0);
		}
	}
	void drawAproxSqCentroids(cv::Mat& outImg)
	{
		for (auto centroits : aproxSqCentroids)
		{
			cv::circle(outImg, centroits, 1, cv::Scalar(0, 0, 255), 1, 8, 0);
		}
	}
};

