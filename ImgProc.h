#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

class ImgProc
{
public:
	static ImgProc& getInstance() {
		static ImgProc instance;
		return instance;
	}

	cv::Mat& ImgProc::threshold(cv::Mat&, uchar);
	cv::Mat dilate(cv::Mat&, uint);
	int calcFieldGrey(cv::Mat&);

private:
	ImgProc() {};
	ImgProc(ImgProc const&) = delete;
	void operator=(ImgProc const&) = delete;

	bool isBlack(uchar);
	long double m_pq(cv::Mat&, int, int)
};

