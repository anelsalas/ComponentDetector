#pragma once
#include "pch.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>


using namespace std;

class MyConvexHull
{
public:
	MyConvexHull() : epsilonpercent(0.2), epsilon(0) {};
	~MyConvexHull() {};
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
		cv::Mat inimg, blured, canny, outarray ;
		inputImage.copyTo(outputImage);
		cv::blur(inputImage, blured, cv::Size(3, 3));
		cv::Canny(blured, canny, 87, 400, 3, true);
		cv::threshold(canny, outarray, 50, 255, cv::THRESH_BINARY); // apply binary thresholding

		// find contours
		cv::findContours(outarray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		findHulls();

		extractNonConvexHulls();

		//findHullCentroids();

		extractEmbeddedHulls();


		drawHullCentroids(outputImage);

		drawHulls(outputImage);


		totalFound = hull.size();
		
    }
	void findHulls()
	{
		// create hull array for convex hull points
		hull.resize(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			cv::convexHull(cv::Mat(contours[i]), hull[i], false);
		}


	}
	void findHullCentroids()
	{
		// find centroids
		cv::Moments hullMom;
		for (auto hulls : finalHulls)
		{
			hullMom = cv::moments(hulls, false);
			hullCentroids.push_back(cv::Point(hullMom.m10 / hullMom.m00, hullMom.m01 / hullMom.m00));
			hullMoments.push_back(hullMom);
		}
	}

	void drawHullCentroids(cv::Mat& outImg)
	{
		for (auto centroits : hullCentroids)
		{
			cv::circle(outImg, centroits, 1, cv::Scalar(0, 0, 255), 1, 8, 0);
		}
	}

	vector<vector<cv::Point> >  getOuterHullVector()
	{
		vector<vector<cv::Point> > outerHulls;

		for (auto centr : hullCentroids)
		{
			for (auto hull : finalHulls)
			{
				if (cv::pointPolygonTest(hull, centr, false) >= 0)
					break;

			}

		}


		return outerHulls;

	}
	void extractEmbeddedHulls()
	{
		// area = M['m00']
		// Remove hulls within hulls
		uint16_t momentIter(0), hullIter(0);
		std::vector < uint16_t > hullsToDelete;
		//hull.clear();
		bool localfound = true;
		bool foundinner = false;
		uint16_t shit(0);
		while (shit < 20)// localfound == true)
		{
			shit++;
			findHullCentroids();
			localfound == false;
			for (size_t outerIter(0); outerIter < finalHulls.size(); outerIter++)
			{
				// for each hull check if its centroid is inside any other hull
				for (size_t innerIter(outerIter + 1); innerIter < finalHulls.size(); innerIter++)
				{
					// returns 1 if inside, or 0 if in the edge
					if (cv::pointPolygonTest(finalHulls[innerIter], hullCentroids[outerIter], false) >= 0)
					{
						localfound = true;
						// its inside, so delete the smallest
						if (isHullLarger(hullMoments[outerIter], hullMoments[innerIter]))
						{
							finalHulls[innerIter] = finalHulls.back();
							finalHulls.pop_back();
							//finalHulls[innerIter].erase();
						}
						else
						{
							finalHulls[outerIter] = finalHulls.back();
							finalHulls.pop_back();
							//finalHulls[innerIter].erase();
						}
						break;
					}
				}
				if (localfound) break;
				if (outerIter == finalHulls.size()) localfound = false;
			}
		}

		/*
							hull.push_back(finalHulls[innerIter]);
					found = true;
					
					if (hullMoments[outerIter].m00 >= hullMoments[innerIter].m00)
						if (std::find(hullsToDelete.begin(), hullsToDelete.end(), outerIter) == hullsToDelete.end())
							hullsToDelete.push_back(outerIter);
						else
							if (std::find(hullsToDelete.begin(), hullsToDelete.end(), innerIter) == hullsToDelete.end())
								hullsToDelete.push_back(innerIter);
					
		*/

	}
	bool isHullLarger(cv::Moments moment1, cv::Moments moment2)
	{
		if (moment1.m00 >= moment2.m00)
			return true;

	}

	void extractNonConvexHulls()
	{
		finalHulls.clear();

		for (auto myhully : hull)
		{
			if (cv::isContourConvex(myhully))
				finalHulls.push_back(myhully);

		}


	}

	void drawHulls(cv::Mat& outImg)
	{
		//ostringstream buf;
		for (int i = 0; i < finalHulls.size(); i++)
		{
			cv::Scalar color = cv::Scalar(0, 0, 255); // red - color for convex hull

			// draw ith convex hull
			//if(cv::isContourConvex(hull[i]))
			drawContours(outImg, finalHulls, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
		}
	}
};


