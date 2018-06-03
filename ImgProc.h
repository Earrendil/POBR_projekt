#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <map>

class ImgProc
{
public:
	static ImgProc& getInstance() {
		static ImgProc instance;
		return instance;
	}

	cv::Mat& ImgProc::threshold(cv::Mat&, uchar);
	cv::Mat dilate(cv::Mat&, uint);
	cv::Mat erode(cv::Mat&, uint);
	cv::Mat contrast(cv::Mat&, float);
	cv::Mat deleteNonGrey(cv::Mat&, uint, uint);
	cv::Mat deleteNonSkin(cv::Mat&);
	cv::Mat cutBoundings(cv::Mat&, uint);
	int calcFieldGrey(cv::Mat&, std::string&, bool = false);
	std::map<std::string, long double> calcMomentInvariants(cv::Mat &, std::string&, bool = false);

private:
	ImgProc() {};
	ImgProc(ImgProc const&) = delete;
	void operator=(ImgProc const&) = delete;

	bool isBlack(uchar);
	long double m_pq(cv::Mat&, int, int, std::string& = std::string("mpq"), bool = false);
};

