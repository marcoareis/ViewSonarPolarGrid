#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <iostream>
#include <math.h>

int main(int argc, char** argv) {

	cv::Mat3b image = cv::Mat3b::zeros(1000, 1000);
	cv::Point2f input(0, 2000);
	cv::Point center(image.rows / 2, image.cols / 2);

	double ratio = M_PI / 180.0;

	double radiusLimit = image.rows / 2.0;
	double radiusIncrement = radiusLimit * 0.1;
	double radialLimit = 180 * ratio;
	double radialIncrement = radialLimit / 9;

	//creating the grid


	//drawing the circles
	for (uint r = 0; r <= radiusLimit; r += radiusIncrement) {
		cv::ellipse(image, center, cv::Size(r, r), 0, 210, 330,
				cv::Scalar(255, 255, 255), 0, CV_AA);
	}

	double theta;

	//drawing the lines
	for (theta = M_PI / 6; theta <= 5 * radialLimit / 6; theta +=
			radialIncrement) {

		cv::Point plotPoint(radiusLimit * cos(theta + M_PI), radiusLimit * sin(theta + M_PI));
		cv::line(image, center, center + plotPoint, cv::Scalar(255, 255, 255), 0, CV_AA);

	}

	//  delimiting each sector

	int my_bin, my_beam;
	std::vector<std::vector<cv::Point> > ListOfPolygons;

	for (my_bin = 1; my_bin <= 10; ++my_bin) {
		for (my_beam = 1; my_beam <= 6; ++my_beam) {

			double minBin, maxBin;
			minBin = (my_bin - 1) * radiusIncrement;
			maxBin = my_bin * radiusIncrement;

			double minAngle, maxAngle;
			minAngle = (10 + my_beam * 20) * ratio;
			maxAngle = (30 + my_beam * 20) * ratio;

			// Cathing the points from the limits

			std::vector<cv::Point> UpperPolygonPoints;
			std::vector<cv::Point> LowerPolygonPoints;
			float step = radialIncrement / 10.0;

			for (theta = minAngle; theta <= maxAngle*1.0001; theta += step) {

				// catching the upper limit

				cv::Point plotPointEnd(maxBin * cos(theta + M_PI), maxBin * sin(theta + M_PI));
				UpperPolygonPoints.push_back(center + plotPointEnd);

				//catching the lower limit

				cv::Point plotPointBegin(minBin * cos(theta + M_PI), minBin * sin(theta + M_PI));
				LowerPolygonPoints.push_back(center + plotPointBegin);

			}
			std::reverse(UpperPolygonPoints.begin(), UpperPolygonPoints.end());


			// Polygon Points Coordinates

			std::vector<cv::Point> PolygonPoints;

			for (uint k1 = 0; k1 < LowerPolygonPoints.size(); ++k1) {
				PolygonPoints.push_back(LowerPolygonPoints[k1]);
			}

			for (uint k2 = 0; k2 < UpperPolygonPoints.size(); ++k2) {
				PolygonPoints.push_back(UpperPolygonPoints[k2]);
			}

			PolygonPoints.push_back(LowerPolygonPoints[0]);

			ListOfPolygons.push_back(PolygonPoints);

		}
	}


	cv::circle(image, cv::Point(105, 268), 2, cv::Scalar(255, 255, 0), 3);

	cv::imshow("Sonar View Grid", image);
	cv::waitKey(0);

	//sweeping the image

	std::vector<std::vector<cv::Point> > SectorInnerPts;
	std::vector<cv::Point> tempVector;


	for (int sector = 0; sector < 59; sector++) {
		for (int imgy = 0; imgy <= 500; ++imgy) {
			for (int imgx = 0; imgx <= 1000; ++imgx) {

				if (pointPolygonTest(ListOfPolygons[sector] , cv::Point(imgx, imgy), false) > 0 ){
					tempVector.push_back(cv::Point(imgx, imgy));
				}
			}
		}
		SectorInnerPts.push_back(tempVector);
	}

	std::cout << "This is the size ------>>" << SectorInnerPts[0].size() << std::endl;

	return 0;
}

