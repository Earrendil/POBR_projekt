#define _USE_MATH_DEFINES

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include "ImgProc.h"

void computeHand(std::string filename, std::string windowname, std::ofstream& file) {
	file << "---------------- image: " << filename << " ---------------------" << std::endl;
	ImgProc& imgProc = ImgProc::getInstance();
	//READ IMAGE
	cv::Mat img = cv::imread(filename);
	std::string img_filename = std::string(windowname).append(".jpg");
	cv::imwrite(img_filename, img);
	//CONVERT TO HSV
	cv::Mat img_hsv;
	cv::cvtColor(img, img_hsv, CV_BGR2HSV);
	std::string img_hsv_filename = std::string(windowname).append(" 110 HSV").append(".jpg");
	cv::imwrite(img_hsv_filename, img_hsv);
	//DELETE NON SKIN COLORS
	cv::Mat img_skin = imgProc.deleteNonSkin(img_hsv);
	std::string img_skin_filename = std::string(windowname).append(" 120 delete non skin").append(".jpg");
	cv::imwrite(img_skin_filename, img_skin);
	//CONVERT TO GREY SCALE
	cv::Mat img_grey2;
	cv::cvtColor(img_skin, img_grey2, CV_BGR2GRAY);
	std::string img_grey_filename2 = std::string(windowname).append(" 125 GREY").append(".jpg");
	cv::imwrite(img_grey_filename2, img_grey2);
	//THRESHOLD DELETED NON GREY
	cv::Mat img_thresh_grey = imgProc.threshold(img_grey2, 170);
	std::string img_thresh__grey_filename = std::string(windowname).append(" 130 threshold deleted non gray").append(".jpg");
	cv::imwrite(img_thresh__grey_filename, img_thresh_grey);
	//DILATE
	cv::Mat img_dilate2 = imgProc.dilate(img_thresh_grey, 3);
	std::string img_dilate_filename2 = std::string(windowname).append(" 140 dilate").append(".jpg");
	cv::imwrite(img_dilate_filename2, img_dilate2);
	//CALCULATE MOMENT INVARIANTS
	std::map<std::string, long double> coeffs = imgProc.calcMomentInvariants(img_dilate2, windowname, true);
	file << "M1: " << coeffs["M1"] << ",      M3: " << coeffs["M3"] << ",      M7: " << coeffs["M7"] << std::endl;
	//CALCULATE FIELD
	int field = imgProc.calcFieldGrey(img_dilate2, windowname, true);
	float img_field = (float)img_dilate2.rows * (float)img_dilate2.cols;
	float field_percentage = (float)field / img_field;
	file << "FIELD: " << field << ", img_field: " << img_field << ", percentage: " << field_percentage << std::endl;

	//IS IT HAND?
	std::cout << filename << std::endl;
	if (field_percentage >= 0.35 && field_percentage <= 0.52 &&
		coeffs["M7"] >= 0.0076 && coeffs["M7"] <= 0.0097) {
		std::cout << "DETECTED HAND" << std::endl;
	}
}

void computeGlove(std::string filename, std::string windowname, std::ofstream& file) {
	file << "---------------- image: " << filename << " ---------------------" << std::endl;
	ImgProc& imgProc = ImgProc::getInstance();
	//READ IMAGE
	cv::Mat img = cv::imread(filename);
	std::string img_filename = std::string(windowname).append(".jpg");
	cv::imwrite(img_filename, img);
	//DELETE NON GREY FIELD
	cv::Mat img_del_non_grey = imgProc.deleteNonGrey(img, 25, 50);
	std::string img_del_non_grey_filename = std::string(windowname).append(" 110 delete non grey").append(".jpg");
	cv::imwrite(img_del_non_grey_filename, img_del_non_grey);
	//CONTRAST
	//cv::Mat img_cont = imgProc.contrast(img_del_non_grey, 0.80);
	//std::string img_cont_filename = std::string(windowname).append(" 115 contrast").append(".jpg");
	//cv::imwrite(img_cont_filename, img_cont);
	//CONVERT TO GREY SCALE
	cv::Mat img_grey2;
	cv::cvtColor(img_del_non_grey, img_grey2, CV_BGR2GRAY);
	std::string img_grey_filename2 = std::string(windowname).append(" 120 GREY").append(".jpg");
	cv::imwrite(img_grey_filename2, img_grey2);
	//THRESHOLD DELETED NON GREY
	cv::Mat img_thresh_grey = imgProc.threshold(img_grey2, 70);
	std::string img_thresh__grey_filename = std::string(windowname).append(" 130 threshold deleted non gray").append(".jpg");
	cv::imwrite(img_thresh__grey_filename, img_thresh_grey);
	//ERODE
	cv::Mat img_erode2 = imgProc.erode(img_thresh_grey, 1);
	std::string img_erode_filename2 = std::string(windowname).append(" 135 erode").append(".jpg");
	cv::imwrite(img_erode_filename2, img_erode2);
	//DILATE
	cv::Mat img_dilate2 = imgProc.dilate(img_erode2, 8);
	std::string img_dilate_filename2 = std::string(windowname).append(" 140 dilate").append(".jpg");
	cv::imwrite(img_dilate_filename2, img_dilate2);
	//CALCULATE MOMENT INVARIANTS
	std::map<std::string, long double> coeffs = imgProc.calcMomentInvariants(img_dilate2, windowname, true);
	file << "M1: " << coeffs["M1"] << ",      M3: " << coeffs["M3"] << ",      M7: " << coeffs["M7"] << std::endl;
	//CALCULATE FIELD
	int field = imgProc.calcFieldGrey(img_dilate2, windowname, true);
	float img_field = (float)img_dilate2.rows * (float)img_dilate2.cols;
	float field_percentage = (float)field / img_field;
	file << "FIELD: " << field << ", img_field: "<<img_field<<", percentage: "<<field_percentage<<std::endl;

	//IS IT ROVER GLOVE?
	//std::cout << filename << std::endl;
	if (field_percentage >= 0.12  && field_percentage <= 0.45 &&
		coeffs["M7"] >= 0.006  && coeffs["M7"] <= 0.0081 &&
		coeffs["M1"] >= 0.16   && coeffs["M1"] <= 0.19) {
		std::cout << "DETECTED ROVER GLOVE" << std::endl;
	}

	//IS IT REGULAR GLOVE?
	if (field_percentage >= 0.30 && field_percentage <= 0.497 &&
		coeffs["M7"] >= 0.00739  && coeffs["M7"] <= 0.00967 &&
		coeffs["M1"] >= 0.19  && coeffs["M1"]<= 0.24) {
		std::cout << "DETECTED REGULAR GLOVE" << std::endl;
	}
	
}

int main(int, char *[]) {
	std::ofstream results;
	results.open("results.txt");

	//HAND IMAGES
	computeHand("d1.jpg", "dlon 1", results);
	computeGlove("d1.jpg", "dlon 1", results);
	computeHand("d2.jpg", "dlon 2", results);
	computeGlove("d2.jpg", "dlon 2", results);
	computeHand("d3.jpg", "dlon 3", results);
	computeGlove("d3.jpg", "dlon 3", results);
	computeHand("d4.jpg", "dlon 4", results);
	computeGlove("d4.jpg", "dlon 4", results);
	computeHand("d6.jpg", "dlon 6", results);
	computeGlove("d6.jpg", "dlon 6", results);
	computeHand("d7.jpg", "dlon 7", results);
	computeGlove("d7.jpg", "dlon 7", results);
	computeHand("d8.jpg", "dlon 8", results);
	computeGlove("d8.jpg", "dlon 8", results);
	computeHand("d9.jpg", "dlon 9", results);
	computeGlove("d9.jpg", "dlon 9", results);

	//ROVER GLOVES IMAGES
	computeHand("bez1.jpg", "bez palcow 1", results);
	computeGlove("bez1.jpg", "bez palcow 1", results);
	computeHand("bez2.jpg", "bez palcow 2", results);
	computeGlove("bez2.jpg", "bez palcow 2", results);
	computeHand("bez3.jpg", "bez palcow 3", results);
	computeGlove("bez3.jpg", "bez palcow 3", results);
	computeHand("bez4.jpg", "bez palcow 4", results);
	computeGlove("bez4.jpg", "bez palcow 4", results);
	computeHand("bez6.jpg", "bez palcow 6", results);
	computeGlove("bez6.jpg", "bez palcow 6", results);
	computeHand("bez7.jpg", "bez palcow 7", results);
	computeGlove("bez7.jpg", "bez palcow 7", results);
	computeHand("bez8.jpg", "bez palcow 8", results);
	computeGlove("bez8.jpg", "bez palcow 8", results);
	
	//REGULAR GLOVES IMAGES
	computeHand("z1.jpg", "z palcami 1", results);
	computeGlove("z1.jpg", "z palcami 1", results);
	computeHand("z2.jpg", "z palcami 2", results);
	computeGlove("z2.jpg", "z palcami 2", results);
	computeHand("z3.jpg", "z palcami 3", results);
	computeGlove("z3.jpg", "z palcami 3", results);
	computeHand("z4.jpg", "z palcami 4", results);
	computeGlove("z4.jpg", "z palcami 4", results);
	computeHand("z5.jpg", "z palcami 5", results);
	computeGlove("z5.jpg", "z palcami 5", results);
	computeHand("z6.jpg", "z palcami 6", results);
	computeGlove("z6.jpg", "z palcami 6", results);
	computeHand("z7.jpg", "z palcami 7", results);
	computeGlove("z7.jpg", "z palcami 7", results);
	computeHand("z8.jpg", "z palcami 8", results);
	computeGlove("z8.jpg", "z palcami 8", results);

	results.close();
	system("pause");
    cv::waitKey(-1);
    return 0;
}
