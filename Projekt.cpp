#define _USE_MATH_DEFINES

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <cmath>
#include "ImgProc.h"

struct Box
{
	int x_min;
	int x_max;
	int y_min;
	int y_max;
};

//
//long double m_pq(cv::Mat& I, int p, int q){
//	//cv::Mat_<cv::Vec3b> _I = I.clone();
//	//int hist[256];
//	//for (int q = 0; q <= 255; ++q) {
//	//	hist[q] = 0;
//	//}
//	cv::Mat greyMat;
//	cv::cvtColor(I, greyMat, CV_BGR2GRAY);
//	long double m = 0.0;
//	for (int i = 0; i < greyMat.rows; ++i) {
//		for (int j = 0; j < greyMat.cols; ++j){
//			float x_ij = 0.0;
//		//	hist[(int)greyMat.at<uchar>(i, j)]++;
//			if ((int)greyMat.at<uchar>(i, j) < 255){
//			//	_I(i,j) = cv::Vec3b(255, 255, 0);
//				//std::cout << "(" << i << ", " << j << "): " << (int)greyMat.at<uchar>(i, j) << std::endl;
//				x_ij = 1.0;
//			}
//			else {
//				x_ij = 0.0;
//			}
//			m += pow(i, p) * pow(j, q) * x_ij;
//		}
//	}
//	//for (int q = 0; q <= 255; ++q) {
//		//std::cout << q << ": " << hist[q] << std::endl;
//	//}
////cv:imshow("mpq", _I);
//	return m;
//}

void nachylenie(cv::Mat& I_r0){
	//std::cout << "cols: " << I_r0.cols << ", rows: " << I_r0.rows << std::endl;
	long double m10_ = m_pq(I_r0, 1, 0);
	long double m00_ = m_pq(I_r0, 0, 0);
	long double m01_ = m_pq(I_r0, 0, 1);
	float x_sr_ciezkosci = m10_ / m00_;
	float y_sr_ciezkosci = m01_ / m00_;
	float x_mean = (I_r0.cols) / 2;
	float y_mean = (I_r0.rows) / 2;
	cv::Mat_<cv::Vec3b> _I_r0 = I_r0;
	for (int x_ = (int)x_sr_ciezkosci - 1; x_ <= (int)x_sr_ciezkosci + 1; x_++) {
		for (int y_ = (int)y_sr_ciezkosci - 1; y_ <= (int)y_sr_ciezkosci + 1; y_++) {
			_I_r0(x_, y_) = (255, 255, 255);
		}
	}
	for (int x_ = x_mean - 1; x_ <= x_mean + 1; x_++) {
		for (int y_ = y_mean - 1; y_ <= y_mean + 1; y_++) {
			_I_r0((int)y_, (int)x_) = (0, 0, 0);
		}
	}

	I_r0 = _I_r0;
	std::string windowName = "STRZALKI";
	cv::namedWindow(windowName);
	cv::imshow(windowName, I_r0);
	cv::waitKey(0);
	cv::destroyWindow(windowName);

	//std::cout << " x_mean: " << x_mean << " y_mean: " << y_mean << std::endl;
	//std::cout << " x_sr_ciezkosci: " << (int)x_sr_ciezkosci << " y_sr_ciezkosci: " << (int)y_sr_ciezkosci << std::endl;
	float xx = abs(x_sr_ciezkosci - x_mean);
	float yy = y_sr_ciezkosci - y_mean;

	double dist;

	dist = pow(xx, 2) + pow(yy, 2);      
	dist = sqrt(dist);

	double a = xx;
	double b = dist;
	double nachylenie = a / b;
	//double nachylenie2 = std::atan2(y_sr_ciezkosci, x_sr_ciezkosci);
	double nachylenie3 = std::atan2(y_sr_ciezkosci - y_mean, x_sr_ciezkosci - x_mean);

	//double degrees1 = nachylenie * (180.0 / M_PI);
	//double degrees2 = nachylenie2 * (180.0 / 3.141592653589793238463);
	double degrees3 = nachylenie3 * (180.0 / M_PI);


	//std::cout << "nachylenie: " << nachylenie << " , w stopniach: "<< degrees1<< std::endl;
	//std::cout << "nachylenie2: " << nachylenie2 << " , w stopniach: " << degrees2 << std::endl;
	std::cout << "nachylenie3: " << nachylenie3 << " , w stopniach: " << degrees3 << std::endl;
}



Box boundingBox(cv::Mat& I, int r){
	Box bb;
	bb.x_max = 0;
	bb.y_max = 0;
	bb.x_min = I.cols;
	bb.y_min = I.rows;
	cv::Mat_<cv::Vec3b> _I = I;
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j){
			if (_I(i, j)[2] == r){
				if (j < bb.x_min) bb.x_min = j;
				if (j > bb.x_max) bb.x_max = j;
				if (i < bb.y_min) bb.y_min = i;
				if (i > bb.y_max) bb.y_max = i;
			}
		}
	}

	return bb;
}


bool isBlack(cv::Vec3b _I){
	if ((_I[0] == 0) && (_I[1] == 0) && (_I[2] == 0))
	{
		return true;
	}
	else {
		return false;
	}
}

bool isWhite(cv::Vec3b _I){
	if ((_I[0] == 255) && (_I[1] == 255) && (_I[2] == 255))
	{
		return true;
	}
	else {
		return false;
	}
}

int calculateField(cv::Mat_<cv::Vec3b>& I) {
	int field = 0;
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j) {

			// Pole S
			if (!isWhite(I(i, j)))
			{
				++field;
				//_I(i, j) = cv::Vec3b(0, 0, 0); - OK!
			}
		}
	}
	return field;
}
//
//bool isBlack(uchar pixel) {
//	if (pixel == 0)
//		return true;
//	else
//		return false;
//}
//
//int calcFieldGrey(cv::Mat& I) {
//	cv::Mat _I = I.clone();
//	int field = 0;
//	for (int i = 0; i < I.rows; ++i) {
//		for (int j = 0; j < I.cols; ++j) {
//
//			// Pole S
//			if (isBlack(I.at<uchar>(i, j)))
//			{
//				++field;
//				_I.at<uchar>(i, j) = 128;
//			}
//		}
//	}
//	cv::imshow("FIELD", _I);
//	return field;
//}

int calculateCirciut(cv::Mat_<cv::Vec3b>& _I) {
	cv::Mat_<cv::Vec3b>& I = _I.clone();
	int circuit = 0;
	std::cout << "rows: " << _I.rows << ", cols" << _I.cols << std::endl;
	for (int i = 0; i < _I.rows; ++i) {
		for (int j = 0; j < _I.cols; ++j) {
			// Obwod L
				for (int ii = i - 1; ii <= i + 1; ++ii) {
					/*if (ii < 0 || ii >= _I.rows) {
						std::cout << "ii: " << ii << std::endl;
						goto stop;
					}*/
					for (int jj = j - 1; jj <= j + 1; ++jj) {
						if (ii < 0 || ii >= _I.rows || jj < 0 || jj >= _I.cols) {
						//std::cout << "jj: " << jj << std::endl;
							if (!isWhite(_I(i, j))) {
								++circuit;
								I(i, j) = cv::Vec3b(255, 0, 0);
								goto stop;
							}
						}
						if (!isWhite(_I(i, j)) && (_I(i, j) != _I(ii, jj))) {
							++circuit;
							I(i, j) = cv::Vec3b(255, 0, 0); 
							goto stop;
						}
					}
				}


		stop:;
		}
	}
	cv::imshow("obwod", I);
	cv::imwrite("obwod.jpg", I);
	return circuit;
}

cv::Mat& perform(cv::Mat& I, bool strzalki = false, char* nazwa = "strzalki_1_r0"){
  CV_Assert(I.depth() != sizeof(uchar));
  switch (I.channels()) {
  case 1:
	  for (int i = 0; i < I.rows; ++i)
		  for (int j = 0; j < I.cols; ++j)
			  I.at<uchar>(i, j) = (I.at<uchar>(i, j) / 32) * 32;
	  break;
  case 3:
	  cv::Mat_<cv::Vec3b> _I = I.clone();
	  float pole = 0.0;
	  float obwod = 0.0;
	  float w3 = 0.0;
	  /*
	  for (int i = 0; i < _I.rows; ++i) {
		  for (int j = 0; j < _I.cols; ++j) {

			  // Pole S
			  if (!isWhite(_I(i, j)))
			  {
				  ++pole;
				  //_I(i, j) = cv::Vec3b(0, 0, 0); - OK!
			  }
			  // Obwod L
			  for (int ii = i - 1; ii <= i + 1; ++ii) {
				  for (int jj = j - 1; jj <= j + 1; ++jj) {
					  if (!isWhite(_I(i, j)) && (_I(i, j) != _I(ii, jj))) {
						  ++obwod;
						  goto stop;
					  }
				  }
			  }


		  stop:;
		  }
	  }
	  */
	  //W3
	  //w3 = obwod / (2 * std::sqrt(M_PI * pole)) - 1;
	  w3 = calculateCirciut(_I) / (2 * std::sqrt(M_PI * calculateField(_I))) - 1;
	  //M1
	  long double m20 = m_pq(I, 2, 0);
	  long double m10 = m_pq(I, 1, 0);
	  long double m00 = m_pq(I, 0, 0);
	  long double m01 = m_pq(I, 0, 1);
	  long double m11 = m_pq(I, 1, 1);
	  long double m02 = m_pq(I, 0, 2);
	  long double M20 = m20 - pow(m10, 2.0) / m00;
	  long double M02 = m02 - pow(m01, 2.0) / m00;
	  long double M11 = m11 - m10 * m01 / m00;

	  long double M1 = (M20 + M02) / pow(m00, 2.0);
	  long double M7 = (M20 * M02 - pow(M11, 2.0)) / pow(m00, 4.0);

	  //M7
	  // I = _I;
	  std::cout << "S = " << calculateField(_I) << " L = " << calculateCirciut(_I) << "         W3 = " << w3 << " M1 =  " << M1 << " M7 = " << M7 << std::endl;
	  if (strzalki) {

		  cv::imshow(nazwa, _I);
		  nachylenie(I);

	  }
	  break;
  }
  return I;
}

cv::Mat selectMax(cv::Mat& I){
    CV_Assert(I.depth() != sizeof(uchar));
    cv::Mat  res(I.rows,I.cols, CV_8UC3);
    switch(I.channels())  {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        cv::Mat_<cv::Vec3b> _R = res;
        for( int i = 0; i < I.rows; ++i)
            for( int j = 0; j < I.cols; ++j ){
                int sel = (_I(i,j)[0] < _I(i,j)[1])?1:0;
                sel = _I(i,j)[sel] < _I(i,j)[2]?2:sel;
                _R(i,j)[0] = sel==0?255:0;
                _R(i,j)[1] = sel==1?255:0;
                _R(i,j)[2] = sel==2?255:0;
            }
        res = _R;
        break;
    }
    return res;
}
/*
cv::Mat& threshold(cv::Mat& I, uchar thresh) {
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


cv::Mat dilate_moje(cv::Mat& I, uint times) {
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
*/


int main(int, char *[]) {
	ImgProc& imgProc = ImgProc::getInstance();

	cv::Mat zpalcami1 = cv::imread("zpalcami1.jpg");
	cv::imshow("Z palcami 1", zpalcami1);
	cv::Mat zpalcami1_grey;
	cv::cvtColor(zpalcami1, zpalcami1_grey, CV_BGR2GRAY);
	cv::imshow("Z palcami 1 GREY", zpalcami1_grey);
	//cv::Mat thresh = threshold(zpalcami1_grey, 128);
	cv::Mat thresh = imgProc.threshold(zpalcami1_grey, 128);
	cv::imshow("Z palcami 1 GREY threshold", thresh);
	cv::Mat dil = imgProc.dilate(thresh, 3);
	cv::imshow("Z palcami 1 GREY threshold dilate", dil);

	int field = imgProc.calcFieldGrey(dil);
	std::cout << "FIELD: " << field << std::endl;
	//cv::Mat dil2 = dilate_moje(dil);
	//cv::imshow("Z palcami 1 GREY threshold dilate 2x", dil2);
	//cv::Mat dil3 = dilate_moje(dil2);
	//cv::imshow("Z palcami 1 GREY threshold dilate 3x", dil3);
	/*
    std::cout << "Start ..." << std::endl;
	cv::Mat elipsa = cv::imread("elipsa.dib");
	std::cout << "Plik elipsa: ";
	perform(elipsa);
	/*
	cv::Mat elipsa1 = cv::imread("elipsa1.dib");
	std::cout << "Plik elipsa1: ";
	perform(elipsa1);
	cv::Mat kolo = cv::imread("kolo.dib");
	std::cout << "Plik kolo: ";
	perform(kolo);
	cv::Mat prost = cv::imread("prost.dib");
	std::cout << "Plik prost: ";
	perform(prost);
	cv::Mat troj = cv::imread("troj.dib");
	std::cout << "Plik troj: ";
	perform(troj);

	cv::Mat strzalki_1 = cv::imread("strzalki_1.dib");
	std::cout << std::endl << "Strzalki_1"<<std::endl;
	Box bb_r0 = boundingBox(strzalki_1, 0);
	cv::Mat strzalki_1_r0 = strzalki_1(cv::Rect(bb_r0.x_min, bb_r0.y_min, bb_r0.x_max - bb_r0.x_min, bb_r0.y_max - bb_r0.y_min));
	perform(strzalki_1_r0, true, "strzalki_1_r0");
	/*
	Box bb_r45 = boundingBox(strzalki_1, 45);
	cv::Mat strzalki_1_r45 = strzalki_1(cv::Rect(bb_r45.x_min, bb_r45.y_min, bb_r45.x_max - bb_r45.x_min, bb_r45.y_max - bb_r45.y_min));
	perform(strzalki_1_r45, true, "strzalki_1_r45");

	Box bb_r90 = boundingBox(strzalki_1, 90);
	cv::Mat strzalki_1_r90 = strzalki_1(cv::Rect(bb_r90.x_min, bb_r90.y_min, bb_r90.x_max - bb_r90.x_min, bb_r90.y_max - bb_r90.y_min));
	perform(strzalki_1_r90, true, "strzalki_1_r90");

	Box bb_r135 = boundingBox(strzalki_1, 135);
	cv::Mat strzalki_1_r135 = strzalki_1(cv::Rect(bb_r135.x_min, bb_r135.y_min, bb_r135.x_max - bb_r135.x_min, bb_r135.y_max - bb_r135.y_min));
	perform(strzalki_1_r135, true, "strzalki_1_r135");

	Box bb_r180 = boundingBox(strzalki_1, 180);
	cv::Mat strzalki_1_r180 = strzalki_1(cv::Rect(bb_r180.x_min, bb_r180.y_min, bb_r180.x_max - bb_r180.x_min, bb_r180.y_max - bb_r180.y_min));
	perform(strzalki_1_r180, true, "strzalki_1_r180");


	
	cv::Mat strzalki_2 = cv::imread("strzalki_2.dib");
	std::cout << std::endl << "strzalki_2" << std::endl;

	bb_r0 = boundingBox(strzalki_2, 0);
	cv::Mat strzalki_2_r0 = strzalki_2(cv::Rect(bb_r0.x_min, bb_r0.y_min, bb_r0.x_max - bb_r0.x_min, bb_r0.y_max - bb_r0.y_min));
	perform(strzalki_2_r0, true, "strzalki_2_r0");

	 bb_r45 = boundingBox(strzalki_2, 45);
	cv::Mat strzalki_2_r45 = strzalki_2(cv::Rect(bb_r45.x_min, bb_r45.y_min, bb_r45.x_max - bb_r45.x_min, bb_r45.y_max - bb_r45.y_min));
	perform(strzalki_2_r45, true, "strzalki_2_r45");

	 bb_r90 = boundingBox(strzalki_2, 90);
	cv::Mat strzalki_2_r90 = strzalki_2(cv::Rect(bb_r90.x_min, bb_r90.y_min, bb_r90.x_max - bb_r90.x_min, bb_r90.y_max - bb_r90.y_min));
	perform(strzalki_2_r90, true, "strzalki_2_r90");
	
	 bb_r135 = boundingBox(strzalki_2, 135);
	cv::Mat strzalki_2_r135 = strzalki_2(cv::Rect(bb_r135.x_min, bb_r135.y_min, bb_r135.x_max - bb_r135.x_min, bb_r135.y_max - bb_r135.y_min));
	perform(strzalki_2_r135, true, "strzalki_2_r135");

	 bb_r180 = boundingBox(strzalki_2, 180);
	cv::Mat strzalki_2_r180 = strzalki_2(cv::Rect(bb_r180.x_min, bb_r180.y_min, bb_r180.x_max - bb_r180.x_min, bb_r180.y_max - bb_r180.y_min));
	perform(strzalki_2_r180, true, "strzalki_2_r180");
	*/
	
	

    cv::waitKey(-1);
    return 0;
}
