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


cv::Mat ImgProc::erode(cv::Mat& I, uint times) {
	cv::Mat _I = I.clone();
	for (int q = 0; q < times; ++q) {
		for (int i = 1; i < I.rows - 1; ++i) {
			for (int j = 1; j < I.cols - 1; ++j) {
				//for (int i = 0; i < I.rows; ++i) {
				//for (int j = 0; j < I.cols; ++j) {
				for (int ii = i - 1; ii <= i + 1; ++ii) {
					for (int jj = j - 1; jj <= j + 1; ++jj) {

						if (I.at<uchar>(ii, jj) == 255) {
							_I.at<uchar>(i, j) = 255;
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

cv::Mat ImgProc::contrast(cv::Mat &I, float contrast)
{
	cv::Mat _I = I.clone();
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j) {
			_I.at<uchar>(i, j) = I.at<uchar>(i, j) * contrast;
		}
	}
	return _I;
}

bool inRange(uint value, uint high, uint low) {
	if (value >= low && value <= high)
		return true;
	else
		return false;
}

bool isGrey(cv::Vec3b pixel, uint deviation) {
	uint high = pixel[0] + deviation;
	uint low = pixel[0] - deviation;
	if (inRange(pixel[1], high, low) && inRange(pixel[2], high, low))
		return true;
	else
		return false;
}

bool isDark(cv::Vec3b pixel, uint darkness) {
	if ((pixel[0] + pixel[1] + pixel[2]) < 3 * darkness) {
		return true;
	}
	else
		return false;
}

cv::Mat ImgProc::deleteNonGrey(cv::Mat &I, uint deviation, uint darkness)
{
	cv::Mat_<cv::Vec3b> _I = I.clone();
	for (int i = 1; i < I.rows - 1; ++i) {
		for (int j = 1; j < I.cols - 1; ++j) {
			if (!isGrey(_I(i, j), deviation) && !isDark(_I(i, j), darkness)) {
				_I(i, j) = cv::Vec3b(255, 255, 255);
			}

		}
	}
	return _I;
}


bool isSkinColor(cv::Vec3b pixel) {
	if ((pixel[0] < 15 && pixel[1] > 30 )) {
		return true;
	}
	else
		//if (pixel[1] !=0 && pixel[1] !=2)
		//std::cout << (int)pixel[1] << std::endl;
		return false;
}

cv::Mat ImgProc::deleteNonSkin(cv::Mat &I)
{
	cv::Mat_<cv::Vec3b> _I = I.clone();
	for (int i = 0; i < I.rows ; ++i) {
		for (int j = 0; j < I.cols; ++j) {
			if (!isSkinColor(I.at<cv::Vec3b>(i,j))) {
				_I(i, j) = cv::Vec3b(255, 255, 255);
			}

		}
	}
	return _I;
}

cv::Mat ImgProc::cutBoundings(cv::Mat &I, uint margin)
{
	cv::Mat cutted = I(cv::Rect(margin, margin, I.cols - margin, I.rows - margin));
	return cutted;
}

int ImgProc::calcFieldGrey(cv::Mat& I, std::string& windowname, bool colorTest) {
	cv::Mat _I;
	if (colorTest == true) {
		_I = I.clone();
	}
	int field = 0;
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j) {

			if (isBlack(I.at<uchar>(i, j)))
			{
				++field;
				if (colorTest == true) {
					_I.at<uchar>(i, j) = 128;
				}
			}
		}
	}
	if (colorTest == true) {
		cv::imwrite(windowname.append(" 05 FIELD.jpg"), _I);
	}
	return field;
}

std::map<std::string, long double> ImgProc::calcMomentInvariants(cv::Mat &I, std::string& windowname, bool colorTest)
{
	std::map<std::string, long double> coeffs;

	long double m00 = m_pq(I, 0, 0);
	long double m01 = m_pq(I, 0, 1);
	long double m02 = m_pq(I, 0, 2);
	long double m03 = m_pq(I, 0, 3);
	long double m10 = m_pq(I, 1, 0);
	long double m11 = m_pq(I, 1, 1);
	long double m12 = m_pq(I, 1, 2);
	long double m20 = m_pq(I, 2, 0, windowname, colorTest);
	long double m21 = m_pq(I, 2, 1);
	long double m30 = m_pq(I, 3, 0);

	long double i_ = m10 / m00;
	long double j_ =  m01 / m00;

	long double M02 = m02 - pow(m01, 2.0) / m00;
	long double M03 = m03 - 3 * m02 * j_ + 2 * m01 * pow(j_, 2.0);
	long double M11 = m11 - m10 * m01 / m00;
	long double M12 = m12 - 2 * m11 * j_ - m02 * i_ + 2 * m10 * pow(j_, 2.0);
	long double M20 = m20 - pow(m10, 2.0) / m00;
	long double M21 = m21 - 2 * m11 * i_ - m20 * j_ + 2 * m01 * pow(i_, 2.0);
	long double M30 = m30 - 3 * m20 * i_ + 2 * m10 * pow(i_, 2.0);

	long double M1 = (M20 + M02) / pow(m00, 2.0);
	coeffs["M1"] = M1;

	long double M3 = (pow(M30 + 3 * M12, 2.0) + (3 * M21 - M03, 2.0)) / pow(m00, 5.0);
	coeffs["M3"] = M3;

	long double M7 = (M20 * M02 - pow(M11, 2.0)) / pow(m00, 4.0);
	coeffs["M7"] = M7;

	return coeffs;
}



bool ImgProc::isBlack(uchar pixel) {
	if (pixel == 0)
		return true;
	else
		return false;
}

long double ImgProc::m_pq(cv::Mat& I, int p, int q, std::string& windowname, bool colorTest) {
	cv::Mat _I;
	if (colorTest == true) {
		_I = I.clone();
	}

	//int hist[256];
	//for (int q = 0; q <= 255; ++q) {
	//	hist[q] = 0;
	//}
	//cv::Mat greyMat;
	//cv::cvtColor(I, greyMat, CV_BGR2GRAY);
	long double m = 0.0;
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j) {
			float x_ij = 0.0;
			//	hist[(int)greyMat.at<uchar>(i, j)]++;
			//if ((int)I.at<uchar>(i, j) < 255) {
			if (isBlack(I.at<uchar>(i, j))) {
				if (colorTest == true) {
					_I.at<uchar>(i, j) = 200;
				}
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
	if (colorTest == true) {
		cv:imwrite(windowname.append(" 06 mpq.jpg"), _I);
	}
	return m;
}


