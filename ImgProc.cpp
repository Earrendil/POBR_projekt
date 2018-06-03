#include "ImgProc.h"
#include <iostream>


cv::Mat& ImgProc::threshold(cv::Mat& I, uchar thresh) {
	switch (I.channels()) {
	case 3:
		std::cout << "THRESHOLD will only be performed on grey scale images." << std::endl;
		return I;
	case 1:
		for (int i = 0; i < I.rows; ++i) {
			for (int j = 0; j < I.cols; ++j) {
				if (I.at<uchar>(i, j) < thresh) {
					I.at<uchar>(i, j) = 0;
				}
				else {
					I.at<uchar>(i, j) = 255;
				}
			}
		}
		return I;
	}
}


cv::Mat ImgProc::dilate(cv::Mat& I, uint times) {
	cv::Mat _I = I.clone();
	for (int q = 0; q < times; ++q) {
		for (int i = 1; i < I.rows - 1; ++i) {
			for (int j = 1; j < I.cols - 1; ++j) {
				//for (int i = 0; i < I.rows; ++i) {
				//for (int j = 0; j < I.cols; ++j) {
				for (int ii = i - 1; ii <= i + 1; ++ii) {
					for (int jj = j - 1; jj <= j + 1; ++jj) {

						if (I.at<uchar>(ii, jj) == 0) {
							_I.at<uchar>(i, j) = 0;
							goto stop;
						}
					}
				}
			stop:;
			}
		}
		_I.copyTo(I);
	}

	return _I;
}



int ImgProc::calcFieldGrey(cv::Mat& I) {
	cv::Mat _I = I.clone();
	int field = 0;
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j) {

			if (isBlack(I.at<uchar>(i, j)))
			{
				++field;
				_I.at<uchar>(i, j) = 128;
			}
		}
	}
	cv::imshow("FIELD", _I);
	return field;
}



bool ImgProc::isBlack(uchar pixel) {
	if (pixel == 0)
		return true;
	else
		return false;
}

long double ImgProc::m_pq(cv::Mat& I, int p, int q) {
	//cv::Mat_<cv::Vec3b> _I = I.clone();
	//int hist[256];
	//for (int q = 0; q <= 255; ++q) {
	//	hist[q] = 0;
	//}
	cv::Mat greyMat;
	cv::cvtColor(I, greyMat, CV_BGR2GRAY);
	long double m = 0.0;
	for (int i = 0; i < greyMat.rows; ++i) {
		for (int j = 0; j < greyMat.cols; ++j) {
			float x_ij = 0.0;
			//	hist[(int)greyMat.at<uchar>(i, j)]++;
			//if ((int)greyMat.at<uchar>(i, j) < 255) {
			if (isBlack(greyMat.at<uchar>(i, j))) {
				//	_I(i,j) = cv::Vec3b(255, 255, 0);
				//std::cout << "(" << i << ", " << j << "): " << (int)greyMat.at<uchar>(i, j) << std::endl;
				x_ij = 1.0;
			}
			else {
				x_ij = 0.0;
			}
			m += pow(i, p) * pow(j, q) * x_ij;
		}
	}
	//for (int q = 0; q <= 255; ++q) {
	//std::cout << q << ": " << hist[q] << std::endl;
	//}
	//cv:imshow("mpq", _I);
	return m;
}


