#include "Subsample.h"

void VVC_4_Tap(Mat& USubsample, Mat& VSubsample, Mat& Ufinal, Mat& Vfinal) {
	Mat U_422(USubsample.rows * 2, USubsample.cols, CV_64FC1, Scalar::all(0));
	Mat V_422(VSubsample.rows * 2, VSubsample.cols, CV_64FC1, Scalar::all(0));
	Mat c_filter = (Mat_<double>(1, 4) << -4.0, 36.0, 36.0, -4.0);
	Mat d0_filter = (Mat_<double>(1, 4) << -2.0, 16.0, 54.0, -4.0);
	Mat d1_filter = (Mat_<double>(1, 4) << -4.0, 54.0, 16.0, -2.0);

	for (int j = 0; j < USubsample.cols; j++) {
		for (int i = 0; i < USubsample.rows; i++) {
			for (int index = 0; index < 4; index++) {
				U_422.at<double>(2 * i, j) += d0_filter.at<double>(0, index) * USubsample.at<double>(Clip(i + index - 2, 0, USubsample.rows - 1), j);
				V_422.at<double>(2 * i, j) += d0_filter.at<double>(0, index) * VSubsample.at<double>(Clip(i + index - 2, 0, VSubsample.rows - 1), j);
				U_422.at<double>(2 * i + 1, j) += d1_filter.at<double>(0, index) * USubsample.at<double>(Clip(i + index - 1, 0, USubsample.rows - 1), j);
				V_422.at<double>(2 * i + 1, j) += d1_filter.at<double>(0, index) * VSubsample.at<double>(Clip(i + index - 1, 0, VSubsample.rows - 1), j);
			}
		}
	}

	Ufinal = Mat::zeros(USubsample.rows * 2, USubsample.cols * 2, CV_64FC1);
	Vfinal = Mat::zeros(VSubsample.rows * 2, VSubsample.cols * 2, CV_64FC1);

	for (int i = 0; i < USubsample.rows * 2; i++) {
		for (int j = 0; j < USubsample.cols; j++) {
			Ufinal.at<double>(i, 2 * j) = (U_422.at<double>(i, j) + 32) / 64;
			Vfinal.at<double>(i, 2 * j) = (V_422.at<double>(i, j) + 32) / 64;
			for (int index = 0; index < 4; index++) {
				Ufinal.at<double>(i, 2 * j + 1) += (c_filter.at<double>(0, index) * U_422.at<double>(i, Clip(j + index - 1, 0, USubsample.cols - 1)));
				Vfinal.at<double>(i, 2 * j + 1) += (c_filter.at<double>(0, index) * V_422.at<double>(i, Clip(j + index - 1, 0, VSubsample.cols - 1)));
			}
			Ufinal.at<double>(i, 2 * j + 1) = (Ufinal.at<double>(i, 2 * j + 1) + 2048) / 4096;
			Vfinal.at<double>(i, 2 * j + 1) = (Vfinal.at<double>(i, 2 * j + 1) + 2048) / 4096;
		}
	}
}

void shift_function(int height, int width, string DS, string US, Mat& dstU, Mat& dstV) {
	Mat map_x(height, width, CV_32F);
	Mat map_y(height, width, CV_32F);
	Mat map_xU(height, width, CV_32F);
	Mat map_yU(height, width, CV_32F);
	Mat map_xV(height, width, CV_32F);
	Mat map_yV(height, width, CV_32F);

	if (DS.compare("D") == 0 || DS.compare("MPEG_B") == 0) {

	}
	else if (DS.compare("Chen") == 0)
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				if (US.compare("BILI") == 0 || US.compare("BIC") == 0) { 
					//實驗室位移
					map_xV.at<float>(i, j) = j - 1;
					map_yV.at<float>(i, j) = i;
					//Resize位移
					//map_xV.at<float>(i, j) = j - 0.5;
					//map_yV.at<float>(i, j) = i + 0.5;
				}
				else if (US.compare("VVC_4_Tap") == 0) { //4-tap位移
					map_xV.at<float>(i, j) = j - 1;
					map_yV.at<float>(i, j) = i + 0.5;
				}
			}
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				if (US.compare("BILI") == 0 || US.compare("BIC") == 0) { 
					//實驗室位移
					map_xU.at<float>(i, j) = j;
					map_yU.at<float>(i, j) = i - 1;
					//Resize位移
					//map_xU.at<float>(i, j) = j + 0.5;
					//map_yU.at<float>(i, j) = i - 0.5;
				}
				else if (US.compare("VVC_4_Tap") == 0) {
					//4-tap位移
					map_xU.at<float>(i, j) = j;
					map_yU.at<float>(i, j) = i - 0.5;
				}
			}
	}
	else if (DS.compare("L") == 0) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				map_x.at<float>(i, j) = j;
				map_y.at<float>(i, j) = i - 0.5;
			}
		//Chroma_Shift_420L(US_U_plane, US_V_plane);

	}
	else if (DS.compare("R") == 0) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				map_x.at<float>(i, j) = j - 1;
				map_y.at<float>(i, j) = i - 0.5;
			}
		//Chroma_Shift_420A(US_U_plane, US_V_plane);
	}
	else {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				if (DS.compare("GDCS") == 0) {
					map_x.at<float>(i, j) = j - 0.5;
					map_y.at<float>(i, j) = i;
				}
				else {
					map_x.at<float>(i, j) = j - 0.5;
					map_y.at<float>(i, j) = i - 0.5;
				}
			}
		//Chroma_Shift_420A(US_U_plane, US_V_plane);
	}

	if (US.compare("BIC") == 0 || US.compare("BIC_2") == 0)
	{
		if (DS.compare("Chen") == 0)
		{
			remap(dstU, dstU, map_xU, map_yU, CV_INTER_CUBIC, BORDER_REFLECT);
			remap(dstV, dstV, map_xV, map_yV, CV_INTER_CUBIC, BORDER_REFLECT);
		}
		else
		{
			remap(dstU, dstU, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT);
			remap(dstV, dstV, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT);
		}
	}
	else
	{
		if (DS.compare("Chen") == 0)
		{
			remap(dstU, dstU, map_xU, map_yU, CV_INTER_LINEAR, BORDER_REFLECT);
			remap(dstV, dstV, map_xV, map_yV, CV_INTER_LINEAR, BORDER_REFLECT);
		}
		else
		{
			remap(dstU, dstU, map_x, map_y, CV_INTER_LINEAR, BORDER_REFLECT);
			remap(dstV, dstV, map_x, map_y, CV_INTER_LINEAR, BORDER_REFLECT);
		}
	}
}


void bilinear_16(Mat& U, Mat& V, Mat& Ufinal, Mat& Vfinal)
{
	Mat tempU, tempV;

	U.copyTo(tempU);
	V.copyTo(tempV);

	copyMakeBorder(tempU, tempU, 1, 1, 1, 1, BORDER_REFLECT);
	copyMakeBorder(tempV, tempV, 1, 1, 1, 1, BORDER_REFLECT);

	Ufinal = Mat::zeros(U.rows * 2, U.cols * 2, CV_64FC1);
	Vfinal = Mat::zeros(V.rows * 2, V.cols * 2, CV_64FC1);
	int tempU_rows = 1;
	int tempU_cols = 1;

	for (int i = 0; i < Ufinal.rows; i += 2) {
		tempU_cols = 1;
		for (int j = 0; j < Ufinal.cols; j += 2) {
			Ufinal.at<double>(i, j) = (0.0625 * tempU.at<double>(tempU_rows - 1, tempU_cols - 1)) + (0.1875 * tempU.at<double>(tempU_rows, tempU_cols - 1)) + (0.1875 * tempU.at<double>(tempU_rows - 1, tempU_cols)) + (0.5625 * tempU.at<double>(tempU_rows, tempU_cols));
			Ufinal.at<double>(i, j + 1) = (0.0625 * tempU.at<double>(tempU_rows - 1, tempU_cols + 1)) + (0.1875 * tempU.at<double>(tempU_rows, tempU_cols + 1)) + (0.1875 * tempU.at<double>(tempU_rows - 1, tempU_cols)) + (0.5625 * tempU.at<double>(tempU_rows, tempU_cols));
			Ufinal.at<double>(i + 1, j) = (0.0625 * tempU.at<double>(tempU_rows + 1, tempU_cols - 1)) + (0.1875 * tempU.at<double>(tempU_rows, tempU_cols - 1)) + (0.1875 * tempU.at<double>(tempU_rows + 1, tempU_cols)) + (0.5625 * tempU.at<double>(tempU_rows, tempU_cols));
			Ufinal.at<double>(i + 1, j + 1) = (0.0625 * tempU.at<double>(tempU_rows + 1, tempU_cols + 1)) + (0.1875 * tempU.at<double>(tempU_rows, tempU_cols + 1)) + (0.1875 * tempU.at<double>(tempU_rows + 1, tempU_cols)) + (0.5625 * tempU.at<double>(tempU_rows, tempU_cols));
			tempU_cols++;
		}
		tempU_rows++;
	}

	int tempV_rows = 1;
	int	tempV_cols = 1;

	for (int i = 0; i < Vfinal.rows; i += 2) {
		tempV_cols = 1;
		for (int j = 0; j < Vfinal.cols; j += 2) {
			Vfinal.at<double>(i, j) = (0.0625 * tempV.at<double>(tempV_rows - 1, tempV_cols - 1)) + (0.1875 * tempV.at<double>(tempV_rows, tempV_cols - 1)) + (0.1875 * tempV.at<double>(tempV_rows - 1, tempV_cols)) + (0.5625 * tempV.at<double>(tempV_rows, tempV_cols));
			Vfinal.at<double>(i, j + 1) = (0.0625 * tempV.at<double>(tempV_rows - 1, tempV_cols + 1)) + (0.1875 * tempV.at<double>(tempV_rows, tempV_cols + 1)) + (0.1875 * tempV.at<double>(tempV_rows - 1, tempV_cols)) + (0.5625 * tempV.at<double>(tempV_rows, tempV_cols));
			Vfinal.at<double>(i + 1, j) = (0.0625 * tempV.at<double>(tempV_rows + 1, tempV_cols - 1)) + (0.1875 * tempV.at<double>(tempV_rows, tempV_cols - 1)) + (0.1875 * tempV.at<double>(tempV_rows + 1, tempV_cols)) + (0.5625 * tempV.at<double>(tempV_rows, tempV_cols));
			Vfinal.at<double>(i + 1, j + 1) = (0.0625 * tempV.at<double>(tempV_rows + 1, tempV_cols + 1)) + (0.1875 * tempV.at<double>(tempV_rows, tempV_cols + 1)) + (0.1875 * tempV.at<double>(tempV_rows + 1, tempV_cols)) + (0.5625 * tempV.at<double>(tempV_rows, tempV_cols));
			tempV_cols++;
		}
		tempV_rows++;
	}
}

void UpSample_Lin_2019(Mat& DS_U_plane, Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane) {

	Mat DS_U, DS_V;
	DS_U_plane.copyTo(DS_U);
	DS_V_plane.copyTo(DS_V);

	int height = DS_U_plane.rows * 2;
	int width = DS_U_plane.cols * 2;
	double U_beta_a, U_beta_b, U_beta_c, U_beta_d, U_beta_total;
	double V_beta_a, V_beta_b, V_beta_c, V_beta_d, V_beta_total;
	double U_beta_aa, U_beta_bb, U_beta_cc, U_beta_dd;
	double V_beta_aa, V_beta_bb, V_beta_cc, V_beta_dd;
	double contribute = 0.73;

	resize(DS_U, US_U_plane, Size(0, 0), 2, 2, INTER_NEAREST);
	resize(DS_V, US_V_plane, Size(0, 0), 2, 2, INTER_NEAREST);
	//cout << US_U_plane.at<double>(100, 100) << " " << US_U_plane.at<double>(100, 101) << " " << US_U_plane.at<double>(101, 100) << " " << US_U_plane.at<double>(101, 101) << endl;
	copyMakeBorder(DS_U, DS_U, 1, 1, 1, 1, BORDER_REFLECT);
	copyMakeBorder(DS_V, DS_V, 1, 1, 1, 1, BORDER_REFLECT);
	int xx;
	int yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			U_beta_a = DS_U.at<double>(yy, xx - 1) + DS_U.at<double>(yy - 1, xx) + (DS_U.at<double>(yy - 1, xx - 1)/sqrt(2));
			U_beta_b = DS_U.at<double>(yy, xx + 1) + DS_U.at<double>(yy - 1, xx) + (DS_U.at<double>(yy - 1, xx + 1) / sqrt(2));
			U_beta_c = DS_U.at<double>(yy, xx - 1) + DS_U.at<double>(yy + 1, xx)  + (DS_U.at<double>(yy + 1, xx - 1) / sqrt(2));
			U_beta_d = DS_U.at<double>(yy, xx + 1) + DS_U.at<double>(yy + 1, xx) + (DS_U.at<double>(yy + 1, xx + 1) / sqrt(2));
			U_beta_aa = U_beta_a * (1 - contribute) + DS_U.at<double>(yy, xx) * contribute;
			U_beta_bb = U_beta_b * (1 - contribute) + DS_U.at<double>(yy, xx) * contribute;
			U_beta_cc = U_beta_c * (1 - contribute) + DS_U.at<double>(yy, xx) * contribute;
			U_beta_dd = U_beta_d * (1 - contribute) + DS_U.at<double>(yy, xx) * contribute;
			U_beta_total = U_beta_aa + U_beta_bb + U_beta_cc + U_beta_dd;

			V_beta_a = DS_V.at<double>(yy, xx - 1) + DS_V.at<double>(yy - 1, xx) + (DS_V.at<double>(yy - 1, xx - 1) / sqrt(2));
			V_beta_b = DS_V.at<double>(yy, xx + 1) + DS_V.at<double>(yy - 1, xx) + (DS_V.at<double>(yy - 1, xx + 1) / sqrt(2));
			V_beta_c = DS_V.at<double>(yy, xx - 1) + DS_V.at<double>(yy + 1, xx) + (DS_V.at<double>(yy + 1, xx - 1) / sqrt(2));
			V_beta_d = DS_V.at<double>(yy, xx + 1) + DS_V.at<double>(yy + 1, xx) + (DS_V.at<double>(yy + 1, xx + 1) / sqrt(2));
			V_beta_aa = V_beta_a * (1 - contribute) + DS_V.at<double>(yy, xx) * contribute;
			V_beta_bb = V_beta_b * (1 - contribute) + DS_V.at<double>(yy, xx) * contribute;
			V_beta_cc = V_beta_c * (1 - contribute) + DS_V.at<double>(yy, xx) * contribute;
			V_beta_dd = V_beta_d * (1 - contribute) + DS_V.at<double>(yy, xx) * contribute;
			V_beta_total = V_beta_aa + V_beta_bb + V_beta_cc + V_beta_dd;



			US_U_plane.at<double>(y, x) = 4 * DS_U.at<double>(yy, xx) * U_beta_aa / U_beta_total;
			US_U_plane.at<double>(y, x + 1) = 4 * DS_U.at<double>(yy, xx) * U_beta_bb / U_beta_total;
			US_U_plane.at<double>(y + 1, x) = 4 * DS_U.at<double>(yy, xx) * U_beta_cc / U_beta_total;
			US_U_plane.at<double>(y + 1, x + 1) = 4 * DS_U.at<double>(yy, xx) * U_beta_dd / U_beta_total;

			US_V_plane.at<double>(y, x) = 4 * DS_V.at<double>(yy, xx) * V_beta_aa / V_beta_total;
			US_V_plane.at<double>(y, x + 1) = 4 * DS_V.at<double>(yy, xx) * V_beta_bb / V_beta_total;
			US_V_plane.at<double>(y + 1, x) = 4 * DS_V.at<double>(yy, xx) * V_beta_cc / V_beta_total;
			US_V_plane.at<double>(y + 1, x + 1) = 4 * DS_V.at<double>(yy, xx) * V_beta_dd / V_beta_total;


			xx++;
		}
		yy++;
	}
}

void UpSample_Lin_2017(Mat& DS_U_plane, Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane) {

	Mat DS_U, DS_V;
	DS_U_plane.copyTo(DS_U);
	DS_V_plane.copyTo(DS_V);

	int height = DS_U_plane.rows * 2;
	int width = DS_U_plane.cols * 2;
	double U_alpha_a, U_alpha_b, U_alpha_c, U_alpha_d, U_alpha_total;
	double V_alpha_a, V_alpha_b, V_alpha_c, V_alpha_d, V_alpha_total;

	resize(DS_U, US_U_plane, Size(0, 0), 2, 2, INTER_NEAREST);
	resize(DS_V, US_V_plane, Size(0, 0), 2, 2, INTER_NEAREST);

	copyMakeBorder(DS_U, DS_U, 1, 1, 1, 1, BORDER_REFLECT);
	copyMakeBorder(DS_V, DS_V, 1, 1, 1, 1, BORDER_REFLECT);

	int xx;
	int yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			U_alpha_a = DS_U.at<double>(yy, xx - 1) + DS_U.at<double>(yy - 1, xx);
			U_alpha_b = DS_U.at<double>(yy, xx + 1) + DS_U.at<double>(yy - 1, xx);
			U_alpha_c = DS_U.at<double>(yy, xx - 1) + DS_U.at<double>(yy + 1, xx);
			U_alpha_d = DS_U.at<double>(yy, xx + 1) + DS_U.at<double>(yy + 1, xx);
			U_alpha_total = U_alpha_a + U_alpha_b + U_alpha_c + U_alpha_d;
			V_alpha_a = DS_V.at<double>(yy, xx - 1) + DS_V.at<double>(yy - 1, xx);
			V_alpha_b = DS_V.at<double>(yy, xx + 1) + DS_V.at<double>(yy - 1, xx);
			V_alpha_c = DS_V.at<double>(yy, xx - 1) + DS_V.at<double>(yy + 1, xx);
			V_alpha_d = DS_V.at<double>(yy, xx + 1) + DS_V.at<double>(yy + 1, xx);
			V_alpha_total = V_alpha_a + V_alpha_b + V_alpha_c + V_alpha_d;

			US_U_plane.at<double>(y, x) = 4 * DS_U.at<double>(yy, xx) * U_alpha_a / U_alpha_total;
			US_U_plane.at<double>(y, x + 1) = 4 * DS_U.at<double>(yy, xx) * U_alpha_b / U_alpha_total;
			US_U_plane.at<double>(y + 1, x) = 4 * DS_U.at<double>(yy, xx) * U_alpha_c / U_alpha_total;
			US_U_plane.at<double>(y + 1, x + 1) = 4 * DS_U.at<double>(yy, xx) * U_alpha_d / U_alpha_total;

			US_V_plane.at<double>(y, x) = 4 * DS_V.at<double>(yy, xx) * V_alpha_a / V_alpha_total;
			US_V_plane.at<double>(y, x + 1) = 4 * DS_V.at<double>(yy, xx) * V_alpha_b / V_alpha_total;
			US_V_plane.at<double>(y + 1, x) = 4 * DS_V.at<double>(yy, xx) * V_alpha_c / V_alpha_total;
			US_V_plane.at<double>(y + 1, x + 1) = 4 * DS_V.at<double>(yy, xx) * V_alpha_d / V_alpha_total;


			xx++;
		}
		yy++;
	}

}

void UpSample_Chen_Bilinear(const Mat& DS_U_plane, const Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane)
{
	int height = DS_U_plane.rows * 2;
	int width = DS_U_plane.cols * 2;
	US_U_plane = Mat::zeros(height, width, CV_64FC1);
	US_V_plane = Mat::zeros(height, width, CV_64FC1);
	Mat U_plane, V_plane;
	copyMakeBorder(DS_U_plane, U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, V_plane, 1, 1, 1, 1, BORDER_REPLICATE);

	int xx;
	int yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			US_U_plane.at<double>(y, x) = (U_plane.at<double>(yy - 1, xx) + U_plane.at<double>(yy, xx)) / 2;
			US_U_plane.at<double>(y, x + 1) = (U_plane.at<double>(yy - 1, xx) + U_plane.at<double>(yy - 1, xx + 1) + U_plane.at<double>(yy, xx) + U_plane.at<double>(yy, xx + 1)) / 4;
			US_U_plane.at<double>(y + 1, x) = U_plane.at<double>(yy, xx);
			US_U_plane.at<double>(y + 1, x + 1) = (U_plane.at<double>(yy, xx) + U_plane.at<double>(yy, xx + 1)) / 2;

			US_V_plane.at<double>(y, x) = (V_plane.at<double>(yy, xx - 1) + V_plane.at<double>(yy, xx)) / 2;
			US_V_plane.at<double>(y, x + 1) = V_plane.at<double>(yy, xx);
			US_V_plane.at<double>(y + 1, x) = (V_plane.at<double>(yy, xx - 1) + V_plane.at<double>(yy, xx) + V_plane.at<double>(yy + 1, xx - 1) + V_plane.at<double>(yy + 1, xx)) / 4;
			US_V_plane.at<double>(y + 1, x + 1) = (V_plane.at<double>(yy, xx) + V_plane.at<double>(yy + 1, xx)) / 2;

			xx++;
		}
		yy++;
	}

}

void UpSample_CS_Bilinear(int height, int width, const Mat& DS_plane, Mat& US_plane)
{
	US_plane = Mat::zeros(height, width, CV_64FC1);
	Mat plane;
	copyMakeBorder(DS_plane, plane, 0, 1, 0, 1, BORDER_REPLICATE);
	int xx;
	int yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			US_plane.at<double>(y, x) = plane.at<double>(yy, xx);
			US_plane.at<double>(y + 1, x) = (plane.at<double>(yy, xx) + plane.at<double>(yy + 1, xx)) / 2;
			US_plane.at<double>(y, x + 1) = (plane.at<double>(yy, xx) + plane.at<double>(yy, xx + 1)) / 2;
			US_plane.at<double>(y + 1, x + 1) = (plane.at<double>(yy, xx) + plane.at<double>(yy + 1, xx) + plane.at<double>(yy, xx + 1) + plane.at<double>(yy + 1, xx + 1)) / 4;

			xx++;
		}
		yy++;
	}

}

double BC_funtion(double fx_value)
{
	double fCof = -0.5; //可以代-0.5 or -0.75

	if (fx_value <= 1 && fx_value >= -1)
	{
		return 1 - (3 + fCof)*fabs(fx_value)*fabs(fx_value) + (2 + fCof)*fabs(fx_value)*fabs(fx_value)*fabs(fx_value);
	}
	else if ((fx_value < 2 && fx_value >1) || (fx_value < -1 && fx_value >-2))
	{
		return (-4)*fCof + 8 * fCof*fabs(fx_value) - 5 * fCof*fabs(fx_value)*fabs(fx_value) + fCof * fabs(fx_value)*fabs(fx_value)*fabs(fx_value);
	}
	else
		return 0.0;
}



void UpSample_CS_Bicubic(const Mat& DS_plane, Mat& US_plane)
{
	int height = DS_plane.rows * 2;
	int width = DS_plane.cols * 2;

	US_plane = Mat::zeros(height, width, CV_64FC1);

	for (int y = 0; y < DS_plane.rows; y++)
		for (int x = 0; x < DS_plane.cols; x++)
			US_plane.at<double>(y * 2, x * 2) = DS_plane.at<double>(y, x);

	//先做U平面
	double fWieghtSum = 0.0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (y % 2 == 0 && x % 2 == 0) continue;

			int iOffset_Y;
			int iOffset_X;
			double fSumTemp = 0.0;
			fWieghtSum = 0.0;

			if (y % 2 == 1 && x % 2 == 1) {
				for (int iInitY = -5; iInitY < 6; iInitY += 2)
				{
					for (int iInitX = -5; iInitX < 6; iInitX += 2)
					{
						if (y + iInitY >= 0 && y + iInitY < height)
							iOffset_Y = y + iInitY;
						else continue;

						if (x + iInitX >= 0 && x + iInitX < width)
							iOffset_X = x + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += US_plane.at<double>(mir(iOffset_Y, height), mir(iOffset_X, width)) * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX)); //權重加總值，正規化
					}
				}
			}
			else if (y % 2 == 1 && x % 2 == 0) {
				for (int iInitY = -5; iInitY < 6; iInitY += 2)
				{
					for (int iInitX = -4; iInitX < 7; iInitX += 2)
					{
						if (y + iInitY >= 0 && y + iInitY < height)
							iOffset_Y = y + iInitY;
						else continue;

						if (x + iInitX >= 0 && x + iInitX < width)
							iOffset_X = x + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += US_plane.at<double>(mir(iOffset_Y, height), mir(iOffset_X, width)) * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX));
					}
				}
			}
			else if (y % 2 == 0 && x % 2 == 1) {
				for (int iInitY = -4; iInitY < 7; iInitY += 2)
				{
					for (int iInitX = -5; iInitX < 6; iInitX += 2)
					{
						if (y + iInitY >= 0 && y + iInitY < height)
							iOffset_Y = y + iInitY;
						else continue;

						if (x + iInitX >= 0 && x + iInitX < width)
							iOffset_X = x + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += US_plane.at<double>(mir(iOffset_Y, height), mir(iOffset_X, width)) * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX));
					}
				}
			}
			fSumTemp /= fWieghtSum;
			//if (fSumTemp > 255) fSumTemp = 255; //超出像素值範圍
			//else if (fSumTemp < 0) fSumTemp = 0; //超出像素值範圍
			//mTUPSaRViewBC.at<double>(y, x) = fSumTemp;
			US_plane.at<double>(y, x) = fSumTemp;
			//		cout << "(" << y << ", " << x << ")" << endl;
		}
	}

}



/*20190213*/
void UpSample_420TestOne_Bilinear(int height, int width, const Mat& new_Y_plane, const Mat& DS_U_plane, const Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane) {
	US_U_plane = Mat::zeros(height, width, CV_64FC1);
	US_V_plane = Mat::zeros(height, width, CV_64FC1);

	//double r_TH = 0.9;

	Mat U_plane, V_plane;
	resize(DS_U_plane, U_plane, Size(0, 0), 2, 2, INTER_LINEAR);
	resize(DS_V_plane, V_plane, Size(0, 0), 2, 2, INTER_LINEAR);

	Mat DS_A_Y_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);
	Mat DS_A_U_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);
	Mat DS_A_V_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);

	double u_alpha, u_beta, v_alpha, v_beta;
	double r;
	double r_TH = 0.9;

	int yy;
	int xx;

	/*420A*/
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_A_Y_plane.at<double>(yy, xx) = Clip((new_Y_plane.at<double>(mirror(height, y), mirror(width, x)) + new_Y_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ new_Y_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + new_Y_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
			DS_A_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(mirror(height, y), mirror(width, x)) + U_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ U_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + U_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
			DS_A_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(mirror(height, y), mirror(width, x)) + V_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ V_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);

			xx++;
		}
		yy++;

	}

	copyMakeBorder(DS_A_Y_plane, DS_A_Y_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_A_U_plane, DS_A_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_A_V_plane, DS_A_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);

	Mat Y, A, X;
	double U_LG1, U_LG2, U_LG3, U_LG4;
	double V_LG1, V_LG2, V_LG3, V_LG4;
	Mat Y_m, Y_sd, A_m, A_sd;
	Mat A_tmp, Y_tmp;
	/*LG*/
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			/*U*/
			Y = (Mat_<double>(9, 1) <<
				DS_A_U_plane.at<double>(yy - 1, xx - 1),
				DS_A_U_plane.at<double>(yy - 1, xx),
				DS_A_U_plane.at<double>(yy - 1, xx + 1),
				DS_A_U_plane.at<double>(yy, xx - 1),
				DS_A_U_plane.at<double>(yy, xx),
				DS_A_U_plane.at<double>(yy, xx + 1),
				DS_A_U_plane.at<double>(yy + 1, xx - 1),
				DS_A_U_plane.at<double>(yy + 1, xx),
				DS_A_U_plane.at<double>(yy + 1, xx + 1));

			A = (Mat_<double>(9, 1) <<
				DS_A_Y_plane.at<double>(yy - 1, xx - 1),
				DS_A_Y_plane.at<double>(yy - 1, xx),
				DS_A_Y_plane.at<double>(yy - 1, xx + 1),
				DS_A_Y_plane.at<double>(yy, xx - 1),
				DS_A_Y_plane.at<double>(yy, xx),
				DS_A_Y_plane.at<double>(yy, xx + 1),
				DS_A_Y_plane.at<double>(yy + 1, xx - 1),
				DS_A_Y_plane.at<double>(yy + 1, xx),
				DS_A_Y_plane.at<double>(yy + 1, xx + 1));



			meanStdDev(A, A_m, A_sd);
			meanStdDev(Y, Y_m, Y_sd);

			A_tmp = (A - A_m.at<double>(0, 0)) / A_sd.at<double>(0, 0);
			Y_tmp = (Y - Y_m.at<double>(0, 0)) / Y_sd.at<double>(0, 0);

			r = cv::sum(Y_tmp.mul(A_tmp))[0] / 9;

			if (abs(r) >= r_TH) {
				u_alpha = abs(r) - r_TH;
			}
			else
			{
				u_alpha = 0;

			}
			u_beta = 1 - u_alpha;

			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
			X = (A.t()*A).inv()*A.t()*Y;

			US_U_plane.at<double>(y, x) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y, x);
			US_U_plane.at<double>(y, x + 1) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y, x + 1);
			US_U_plane.at<double>(y + 1, x) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y + 1, x);
			US_U_plane.at<double>(y + 1, x + 1) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y + 1, x + 1);


			/*V*/
			Y = (Mat_<double>(9, 1) <<
				DS_A_V_plane.at<double>(yy - 1, xx - 1),
				DS_A_V_plane.at<double>(yy - 1, xx),
				DS_A_V_plane.at<double>(yy - 1, xx + 1),
				DS_A_V_plane.at<double>(yy, xx - 1),
				DS_A_V_plane.at<double>(yy, xx),
				DS_A_V_plane.at<double>(yy, xx + 1),
				DS_A_V_plane.at<double>(yy + 1, xx - 1),
				DS_A_V_plane.at<double>(yy + 1, xx),
				DS_A_V_plane.at<double>(yy + 1, xx + 1));

			A = (Mat_<double>(9, 1) <<
				DS_A_Y_plane.at<double>(yy - 1, xx - 1),
				DS_A_Y_plane.at<double>(yy - 1, xx),
				DS_A_Y_plane.at<double>(yy - 1, xx + 1),
				DS_A_Y_plane.at<double>(yy, xx - 1),
				DS_A_Y_plane.at<double>(yy, xx),
				DS_A_Y_plane.at<double>(yy, xx + 1),
				DS_A_Y_plane.at<double>(yy + 1, xx - 1),
				DS_A_Y_plane.at<double>(yy + 1, xx),
				DS_A_Y_plane.at<double>(yy + 1, xx + 1));

			meanStdDev(A, A_m, A_sd);
			meanStdDev(Y, Y_m, Y_sd);

			A_tmp = (A - A_m.at<double>(0, 0)) / A_sd.at<double>(0, 0);
			Y_tmp = (Y - Y_m.at<double>(0, 0)) / Y_sd.at<double>(0, 0);

			r = cv::sum(Y_tmp.mul(A_tmp))[0] / 9;

			if (abs(r) >= r_TH) {
				v_alpha = abs(r) - r_TH;
			}
			else
			{
				v_alpha = 0;

			}
			v_beta = 1 - v_alpha;
			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
			X = (A.t()*A).inv()*A.t()*Y;

			US_V_plane.at<double>(y, x) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y, x);
			US_V_plane.at<double>(y, x + 1) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y, x + 1);
			US_V_plane.at<double>(y + 1, x) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y + 1, x);
			US_V_plane.at<double>(y + 1, x + 1) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y + 1, x + 1);


			xx++;
		}
		yy++;
	}



}


//void UpSample_420TestTwo_Bilinear(int height, int width, const Mat& new_Y_plane, const Mat& DS_U_plane, const Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane) {
//US_U_plane = Mat::zeros(height, width, CV_64FC1);
//US_V_plane = Mat::zeros(height, width, CV_64FC1);

//	//double r_TH = 0.9;
//
//	Mat U_plane, V_plane;
//	resize(DS_U_plane, U_plane, Size(0, 0), 2, 2, INTER_LINEAR);
//	resize(DS_V_plane, V_plane, Size(0, 0), 2, 2, INTER_LINEAR);
//
//	/*Mat DS_A_Y_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);
//	Mat DS_A_U_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);
//	Mat DS_A_V_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);*/
//
//	double u_alpha, u_beta, v_alpha, v_beta;
//	double r;
//	double r_TH = 0.9;
//
//	int yy;
//	int xx;
//
//	/*420A*/
//	//yy = 0;
//	//for (int y = 0; y < height; y += 2)
//	//{
//	//	xx = 0;
//	//	for (int x = 0; x < width; x += 2)
//	//	{
//	//		DS_A_Y_plane.at<double>(yy, xx) = Clip((new_Y_plane.at<double>(mirror(height, y), mirror(width, x)) + new_Y_plane.at<double>(mirror(height, y), mirror(width, x + 1))
//	//			+ new_Y_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + new_Y_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
//	//		DS_A_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(mirror(height, y), mirror(width, x)) + U_plane.at<double>(mirror(height, y), mirror(width, x + 1))
//	//			+ U_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + U_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
//	//		DS_A_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(mirror(height, y), mirror(width, x)) + V_plane.at<double>(mirror(height, y), mirror(width, x + 1))
//	//			+ V_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
//
//	//		xx++;
//	//	}
//	//	yy++;
//
//	//}
//
//	/*copyMakeBorder(DS_A_Y_plane, DS_A_Y_plane, 1, 1, 1, 1, BORDER_REPLICATE);
//	copyMakeBorder(DS_A_U_plane, DS_A_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
//	copyMakeBorder(DS_A_V_plane, DS_A_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);*/
//
//	Mat Y, A, X;
//	double U_LG1, U_LG2, U_LG3, U_LG4;
//	double V_LG1, V_LG2, V_LG3, V_LG4;
//	Mat Y_m, Y_sd, A_m, A_sd;
//	Mat A_tmp, Y_tmp;
//	/*LG*/
//	yy = 0;
//	for (int y = 0; y < height; y += 2)
//	{
//		xx = 0;
//		for (int x = 0; x < width; x += 2)
//		{
//			/*U*/
//			Y = (Mat_<double>(4, 1) <<
//				U_plane.at<double>(y, x),
//				U_plane.at<double>(y, x + 1),
//				U_plane.at<double>(y + 1, x),
//				U_plane.at<double>(y + 1, x + 1));
//
//			A = (Mat_<double>(4, 1) <<
//				new_Y_plane.at<double>(y, x),
//				new_Y_plane.at<double>(y, x + 1),
//				new_Y_plane.at<double>(y + 1, x),
//				new_Y_plane.at<double>(y + 1, x + 1));
//
//
//
//			meanStdDev(A, A_m, A_sd);
//			meanStdDev(Y, Y_m, Y_sd);
//
//			A_tmp = (A - A_m.at<double>(0, 0)) / A_sd.at<double>(0, 0);
//			Y_tmp = (Y - Y_m.at<double>(0, 0)) / Y_sd.at<double>(0, 0);
//
//			r = cv::sum(Y_tmp.mul(A_tmp))[0] / 4;
//
//			if (abs(r) >= r_TH) {
//				u_alpha = abs(r) - r_TH;
//			}
//			else
//			{
//				u_alpha = 0;
//
//			}
//			u_beta = 1 - u_alpha;
//
//			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
//			X = (A.t()*A).inv()*A.t()*Y;
//
//			US_U_plane.at<double>(y, x) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y, x);
//			US_U_plane.at<double>(y, x + 1) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y, x + 1);
//			US_U_plane.at<double>(y + 1, x) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y + 1, x);
//			US_U_plane.at<double>(y + 1, x + 1) = u_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y + 1, x + 1);
//
//
//			/*V*/
//			Y = (Mat_<double>(4, 1) <<
//				V_plane.at<double>(y, x),
//				V_plane.at<double>(y, x + 1),
//				V_plane.at<double>(y + 1, x),
//				V_plane.at<double>(y + 1, x + 1));
//
//			A = (Mat_<double>(4, 1) <<
//				new_Y_plane.at<double>(y, x),
//				new_Y_plane.at<double>(y, x + 1),
//				new_Y_plane.at<double>(y + 1, x),
//				new_Y_plane.at<double>(y + 1, x + 1));
//
//
//
//			meanStdDev(A, A_m, A_sd);
//			meanStdDev(Y, Y_m, Y_sd);
//
//			A_tmp = (A - A_m.at<double>(0, 0)) / A_sd.at<double>(0, 0);
//			Y_tmp = (Y - Y_m.at<double>(0, 0)) / Y_sd.at<double>(0, 0);
//
//			r = cv::sum(Y_tmp.mul(A_tmp))[0] / 4;
//
//			if (abs(r) >= r_TH) {
//				v_alpha = abs(r) - r_TH;
//			}
//			else
//			{
//				v_alpha = 0;
//
//			}
//			v_beta = 1 - v_alpha;
//			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
//			X = (A.t()*A).inv()*A.t()*Y;
//
//			US_V_plane.at<double>(y, x) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y, x);
//			US_V_plane.at<double>(y, x + 1) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y, x + 1);
//			US_V_plane.at<double>(y + 1, x) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y + 1, x);
//			US_V_plane.at<double>(y + 1, x + 1) = v_alpha * (X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y + 1, x + 1);
//
//
//			xx++;
//		}
//		yy++;
//	}
//
//
//
//}
//


void UpSample_420TestThree_Bilinear(int height, int width, const Mat& new_Y_plane, const Mat& DS_U_plane, const Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane)
{
	US_U_plane = Mat::zeros(height, width, CV_64FC1);
	US_V_plane = Mat::zeros(height, width, CV_64FC1);

	Mat Y_plane, U_plane, V_plane;

	copyMakeBorder(DS_U_plane, U_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, V_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	Mat Y, A, X;
	Mat Y_sd, Y_m, Y_tmp, A_sd, A_m, A_tmp;
	double u_alpha, u_beta, v_alpha, v_beta;
	double r;
	double r_TH = 0.8;
	int xx;
	int yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{

			US_U_plane.at<double>(y, x) = U_plane.at<double>(yy, xx);
			US_U_plane.at<double>(y + 1, x) = (U_plane.at<double>(yy, xx) + U_plane.at<double>(yy + 1, xx)) / 2;
			US_U_plane.at<double>(y, x + 1) = (U_plane.at<double>(yy, xx) + U_plane.at<double>(yy, xx + 1)) / 2;
			US_U_plane.at<double>(y + 1, x + 1) = (U_plane.at<double>(yy, xx) + U_plane.at<double>(yy + 1, xx) + U_plane.at<double>(yy, xx + 1) + U_plane.at<double>(yy + 1, xx + 1)) / 4;

			US_V_plane.at<double>(y, x) = V_plane.at<double>(yy, xx);
			US_V_plane.at<double>(y + 1, x) = (V_plane.at<double>(yy, xx) + V_plane.at<double>(yy + 1, xx)) / 2;
			US_V_plane.at<double>(y, x + 1) = (V_plane.at<double>(yy, xx) + V_plane.at<double>(yy, xx + 1)) / 2;
			US_V_plane.at<double>(y + 1, x + 1) = (V_plane.at<double>(yy, xx) + V_plane.at<double>(yy + 1, xx) + V_plane.at<double>(yy, xx + 1) + V_plane.at<double>(yy + 1, xx + 1)) / 4;

			xx++;
		}
		yy++;
	}
	copyMakeBorder(new_Y_plane, Y_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	copyMakeBorder(US_U_plane, U_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	copyMakeBorder(US_V_plane, V_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			/*U*/
			Y = (Mat_<double>(4, 1) <<
				U_plane.at<double>(y, x),
				U_plane.at<double>(y, x + 1),
				U_plane.at<double>(y + 1, x),
				U_plane.at<double>(y + 1, x + 1));

			A = (Mat_<double>(4, 1) <<
				Y_plane.at<double>(y, x),
				Y_plane.at<double>(y, x + 1),
				Y_plane.at<double>(y + 1, x),
				Y_plane.at<double>(y + 1, x + 1));



			meanStdDev(A, A_m, A_sd);
			meanStdDev(Y, Y_m, Y_sd);

			A_tmp = (A - A_m.at<double>(0, 0)) / A_sd.at<double>(0, 0);
			Y_tmp = (Y - Y_m.at<double>(0, 0)) / Y_sd.at<double>(0, 0);

			r = cv::sum(Y_tmp.mul(A_tmp))[0] / 4;

			if (abs(r) >= r_TH) {
				u_alpha = abs(r) - r_TH;

			}
			else
			{
				u_alpha = 0;

			}
			u_beta = 1 - u_alpha;

			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
			X = (A.t()*A).inv()*A.t()*Y;

			US_U_plane.at<double>(y, x) = u_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y, x) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y, x);
			US_U_plane.at<double>(y, x + 1) = u_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y, x + 1);
			US_U_plane.at<double>(y + 1, x) = u_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y + 1, x);
			US_U_plane.at<double>(y + 1, x + 1) = u_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0)) + u_beta * U_plane.at<double>(y + 1, x + 1);


			/*V*/
			Y = (Mat_<double>(4, 1) <<
				V_plane.at<double>(y, x),
				V_plane.at<double>(y, x + 1),
				V_plane.at<double>(y + 1, x),
				V_plane.at<double>(y + 1, x + 1));

			A = (Mat_<double>(4, 1) <<
				Y_plane.at<double>(y, x),
				Y_plane.at<double>(y, x + 1),
				Y_plane.at<double>(y + 1, x),
				Y_plane.at<double>(y + 1, x + 1));



			meanStdDev(A, A_m, A_sd);
			meanStdDev(Y, Y_m, Y_sd);

			A_tmp = (A - A_m.at<double>(0, 0)) / A_sd.at<double>(0, 0);
			Y_tmp = (Y - Y_m.at<double>(0, 0)) / Y_sd.at<double>(0, 0);

			r = cv::sum(Y_tmp.mul(A_tmp))[0] / 4;

			if (abs(r) >= r_TH) {
				v_alpha = abs(r) - r_TH;
			}
			else
			{
				v_alpha = 0;

			}
			v_beta = 1 - v_alpha;
			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
			X = (A.t()*A).inv()*A.t()*Y;

			US_V_plane.at<double>(y, x) = v_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y, x) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y, x);
			US_V_plane.at<double>(y, x + 1) = v_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y, x + 1);
			US_V_plane.at<double>(y + 1, x) = v_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y + 1, x);
			US_V_plane.at<double>(y + 1, x + 1) = v_alpha * (X.at<double>(0, 0)*Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0)) + v_beta * V_plane.at<double>(y + 1, x + 1);

			xx++;
		}
		yy++;
	}

}


/*LACDU*/
//void bilinear_weight(Mat& expand444, Mat& Ufinal, Mat& Vfinal) {
//	Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//
//	for (int i = 0; i < expand444.rows; i++)
//		for (int j = 0; j < expand444.cols; j++) {
//			double U[4], V[4], d[4];
//			if (i % 2 == 0 && j % 2 == 0) {
//				d[0] = 0.0625;//1/16 
//				d[1] = 0.1875;//3/16 
//				d[2] = 0.1875;//3/16 
//				d[3] = 0.5625;//9/16 
//				U[0] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 2, expand444.cols))[1];
//				U[1] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j, expand444.cols))[1];
//				U[2] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[1];
//				U[3] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[1];
//				V[0] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 2, expand444.cols))[2]; V[1] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j, expand444.cols))[2];
//				V[2] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[2]; V[3] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[2];
//			}
//			else if (i % 2 == 0 && j % 2 == 1) {
//				d[0] = 0.1875; d[1] = 0.0625; d[2] = 0.5625; d[3] = 0.1875;
//				U[0] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 1, expand444.cols))[1];
//				U[1] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j + 1, expand444.cols))[1];
//				U[2] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[1];
//				U[3] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[1];
//				V[0] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 1, expand444.cols))[2]; V[1] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j + 1, expand444.cols))[2];
//				V[2] = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[2]; V[3] = expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j + 1, expand444.cols))[2];
//			}
//			else if (i % 2 == 1 && j % 2 == 0) {
//				d[0] = 0.1875; d[1] = 0.5625; d[2] = 0.0625; d[3] = 0.1875;
//				U[0] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[1]; U[1] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[1];
//				U[2] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 2, expand444.cols))[1]; U[3] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[1];
//				V[0] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[2]; V[1] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[2];
//				V[2] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 2, expand444.cols))[2]; V[3] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[2];
//			}
//			else if (i % 2 == 1 && j % 2 == 1) {
//				d[0] = 0.5625; d[1] = 0.1875; d[2] = 0.1875; d[3] = 0.0625;
//				U[0] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[1]; U[1] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[1];
//				U[2] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[1]; U[3] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[1];
//				V[0] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[2]; V[1] = expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[2];
//				V[2] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[2]; V[3] = expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[2];
//			}
//			for (int k = 0; k < 4; k++) {
//				Ufinal.at<double>(i, j) += U[k] * d[k];
//				Vfinal.at<double>(i, j) += V[k] * d[k];
//			}
//		}
//}

//void KW(Mat& Ufinal, Mat& Vfinal, const Mat& new_Y_plane, const  Mat& DS_U_plane, const  Mat& DS_V_plane, const  Mat&  YUV) {
//	//int test_i = 400;
//	//int test_j = 912;
//
//	int height = DS_U_plane.rows * 2;
//	int width = DS_U_plane.cols * 2;
//	Mat expand444 = Mat::zeros(height, width, CV_64FC3);
//
//
//	Ufinal = Mat::zeros(height, width, CV_64FC1);
//	Vfinal = Mat::zeros(height, width, CV_64FC1);
//
//	for (int y = 0; y < height; y++)
//		for (int x = 0; x < width; x++)
//		{
//			expand444.at<Vec3d>(y, x)[0] = new_Y_plane.at<double>(y, x);
//
//		}
//	for (int y = 0; y < DS_U_plane.rows; y++)
//		for (int x = 0; x < DS_U_plane.cols; x++)
//		{
//			expand444.at<Vec3d>(y * 2, x * 2)[1] = DS_U_plane.at<double>(y, x);
//			expand444.at<Vec3d>(y * 2, x * 2)[2] = DS_V_plane.at<double>(y, x);
//		}
//	//int test_i = 400;
//	//int test_j = 912;
//	Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Mat U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Mat V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//
//	Mat Mark_Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Mat Mark_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Mat Mark_V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//
//	Mat Out = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	bilinear_weight(expand444, U, V);
//	//Show(U);
//	//Show(V);
//	//----------------------------block-wise content analysis------------------------------------
//	int h = 16, w = 16;
//	int Ns;
//	for (int i = 0; i < expand444.rows; i += 16)
//	for (int j = 0; j < expand444.cols; j += 16){
//		Ns = 0;
//		for (int k = i; k < i + 16; k++)
//		for (int l = j; l < j + 16; l++){
//			if (abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k - 1, expand444.rows), mir(l, expand444.cols))[0])>32 ||
//				abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k, expand444.rows), mir(l - 1, expand444.cols))[0])>32){
//				Ns++;
//			}
//		}
//		if (Ns > 3){
//			Out.at<double>(i, j) = 255;
//			//-------------------------------------做BCIM(Y)-----------------------------------------
//			Mat src = Mat::zeros(16, 16, CV_8UC1);
//			Mat all = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
//			int kk = 0, ll = 0;
//			for (int k = i; k < i + 16; k++){
//				for (int l = j; l < j + 16; l++){
//					src.at<uchar>(kk, ll) = (int)expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//					all.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//					ll++;
//				}
//				ll = 0;
//				kk++;
//			}
//			//histogram
//			float range[] = { 0, 255 };
//			const float* histRange = { range };
//			int histSize = 256;
//			Mat histImg;
//			calcHist(&src, 1, 0, Mat(), histImg, 1, &histSize, &histRange);
//			//Mat showHistImg(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
//			//drawHistImg(histImg, showHistImg);
//			//			imshow("all", all);
//			//			imshow("window1", src);
//			//			imshow("window2", showHistImg);
//			//if (i == test_i && j == test_j)
//			//	imwrite("21_window1.bmp", src);
//
//			//save bsase color
//			int bimrange = 2;//左右各2pixel區間
//			double MinValue;
//			double MaxValue;
//			Point MinLocation;
//			Point MaxLocation;
//			double Y_basecolor[4] = { 0 };
//
//			//把數量最多的顏色(背景色)剔除
//			minMaxLoc(histImg, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
//			for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
//				histImg.at<float>(mir(z, 256)) = 0;
//
//			for (int x = 0; x < 4; x++){
//				minMaxLoc(histImg, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
//				//cout << "The Max number is : " << MaxValue << endl;
//				//cout << "The position is :  " << MaxLocation.x << ", " << MaxLocation.y << endl;
//				Y_basecolor[x] = MaxLocation.y;
//				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
//					histImg.at<float>(mir(z, 256)) = 0;
//				//cout << "======================================" << endl;
//			}
//
//			////if (i == test_i && j == test_j){
//			//	cout << endl << "Base Color of Y : " << endl;
//			//	for (int x = 0; x < 4; x++)
//			//		cout << Y_basecolor[x] << endl;
//			//	//waitKey(0);
//			//}
//			//-----------------------------------------Y標記---------------------------------------------
//			int low1 = Y_basecolor[0] - bimrange;
//			int high1 = Y_basecolor[0] + bimrange;
//			int low2 = Y_basecolor[1] - bimrange;
//			int high2 = Y_basecolor[1] + bimrange;
//			int low3 = Y_basecolor[2] - bimrange;
//			int high3 = Y_basecolor[2] + bimrange;
//			int low4 = Y_basecolor[3] - bimrange;
//			int high4 = Y_basecolor[3] + bimrange;
//			/*if (i == test_i && j == test_j){
//				cout << "low 2 : " << low2 << endl;
//				cout << "high 2: " << high2 << endl;
//			}*/
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low1 &&
//					expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high1)
//					Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;
//
//				else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low2 &&
//					expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high2)
//					Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;
//
//				else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low3 &&
//					expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high3)
//					Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
//
//				else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low4 &&
//					expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high4)
//					Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
//			}
//			//if (i == test_i && j == test_j){
//			//	int count = 0;
//			//	cout << "Set of Y : " << endl;
//			//	for (int x = i; x < i + 16; x++){
//			//		for (int y = j; y < j + 16; y++){
//			//			cout << Mark_Y.at<double>(x, y) << " ";
//			//		}
//			//		cout << endl;
//			//	}
//			//	cout << endl;
//			//	//system("Pause");
//			//}
//			//-------------------------------------做BCIM(U)-----------------------------------------
//			Mat src_U = Mat::zeros(16, 16, CV_8UC1);
//			Mat all_U = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
//			kk = 0;
//			ll = 0;
//			for (int k = i; k < i + 16; k++){
//				for (int l = j; l < j + 16; l++){
//					src_U.at<uchar>(kk, ll) = (int)U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//					all_U.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//					ll++;
//				}
//				ll = 0;
//				kk++;
//			}
//			//histogram
//			float range_U[] = { 0, 255 };
//			const float* histRange_U = { range };
//			//histSize = 256;
//			Mat histImg_U;
//			calcHist(&src_U, 1, 0, Mat(), histImg_U, 1, &histSize, &histRange_U);
//			//Mat showHistImg_U(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
//			//drawHistImg(histImg_U, showHistImg_U);
//			//			imshow("all", all);
//			//			imshow("window1", src_U);
//			//			imshow("window2", showHistImg_U);
//			//if (i == test_i && j == test_j)
//				//imwrite("21_window1_U.bmp", src_U);
//
//			//save bsase color
//			//int bimrange = 10;//左右各10pixel區間
//			//double MinValue;
//			//double MaxValue;
//			//Point MinLocation;
//			//Point MaxLocation;
//			double U_basecolor[4] = { 0 };
//
//			//把數量最多的顏色(背景色)剔除
//			minMaxLoc(histImg_U, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
//			for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
//				histImg_U.at<float>(mir(z, 256)) = 0;
//
//			for (int x = 0; x < 4; x++){
//				minMaxLoc(histImg_U, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
//				//cout << "The Max number is : " << MaxValue << endl;
//				//cout << "The position is :  " << MaxLocation.x << ", " << MaxLocation.y << endl;
//				U_basecolor[x] = MaxLocation.y;
//				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
//					histImg_U.at<float>(mir(z, 256)) = 0;
//				//cout << "======================================" << endl;
//			}
//
//			//if (i == test_i && j == test_j){
//			//	cout << endl << "Base Color of U : " << endl;
//			//	for (int x = 0; x < 4; x++)
//			//		cout << U_basecolor[x] << endl;
//			//	//waitKey(0);
//			//}
//			//-----------------------------------------U標記---------------------------------------------
//			low1 = U_basecolor[0] - bimrange;
//			high1 = U_basecolor[0] + bimrange;
//			low2 = U_basecolor[1] - bimrange;
//			high2 = U_basecolor[1] + bimrange;
//			low3 = U_basecolor[2] - bimrange;
//			high3 = U_basecolor[2] + bimrange;
//			low4 = U_basecolor[3] - bimrange;
//			high4 = U_basecolor[3] + bimrange;
//			/*if (i == test_i && j == test_j){
//				cout << "low 2 : " << low2 << endl;
//				cout << "high 2: " << high2 << endl;
//			}*/
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low1 &&
//					U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high1)
//					Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;
//
//				else if (U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low2 &&
//					U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high2)
//					Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;
//
//				else if (U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low3 &&
//					U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high3)
//					Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
//
//				else if (U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low4 &&
//					U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high4)
//					Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
//			}
//			//if (i == test_i && j == test_j){
//			//	int count = 0;
//			//	cout << "Set of U : " << endl;
//			//	for (int x = i; x < i + 16; x++){
//			//		for (int y = j; y < j + 16; y++){
//			//			cout << Mark_U.at<double>(x, y) << " ";
//			//		}
//			//		cout << endl;
//			//	}
//			//	cout << endl;
//			//	//system("Pause");
//			//}
//			//-------------------------------------做BCIM(V)-----------------------------------------
//			Mat src_V = Mat::zeros(16, 16, CV_8UC1);
//			Mat all_V = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
//			kk = 0;
//			ll = 0;
//			for (int k = i; k < i + 16; k++){
//				for (int l = j; l < j + 16; l++){
//					src_V.at<uchar>(kk, ll) = (int)V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//					all_V.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//					ll++;
//				}
//				ll = 0;
//				kk++;
//			}
//			//histogram
//			float range_V[] = { 0, 255 };
//			const float* histRange_V = { range };
//			Mat histImg_V;
//			calcHist(&src_V, 1, 0, Mat(), histImg_V, 1, &histSize, &histRange_V);
//			//Mat showHistImg_V(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
//			//drawHistImg(histImg_V, showHistImg_V);
//			//		imshow("all", all);
//			//		imshow("window1", src_V);
//			//		imshow("window2", showHistImg_V);
//			//if (i == test_i && j == test_j)
//				//imwrite("21_window1_V.bmp", src_V);
//
//			double V_basecolor[4] = { 0 };
//
//			//把數量最多的顏色(背景色)剔除
//			minMaxLoc(histImg_V, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
//			for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
//				histImg_V.at<float>(mir(z, 256)) = 0;
//
//			for (int x = 0; x < 4; x++){
//				minMaxLoc(histImg_V, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
//				V_basecolor[x] = MaxLocation.y;
//				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
//					histImg_V.at<float>(mir(z, 256)) = 0;
//				//cout << "======================================" << endl;
//			}
//
//			//if (i == test_i && j == test_j){
//			//	cout << endl << "Base Color of V : " << endl;
//			//	for (int x = 0; x < 4; x++)
//			//		cout << V_basecolor[x] << endl;
//			//	//waitKey(0);
//			//}
//			//-----------------------------------------V標記---------------------------------------------
//
//			low1 = V_basecolor[0] - bimrange;
//			high1 = V_basecolor[0] + bimrange;
//			low2 = V_basecolor[1] - bimrange;
//			high2 = V_basecolor[1] + bimrange;
//			low3 = V_basecolor[2] - bimrange;
//			high3 = V_basecolor[2] + bimrange;
//			low4 = V_basecolor[3] - bimrange;
//			high4 = V_basecolor[3] + bimrange;
//			/*if (i == test_i && j == test_j){
//				cout << "low 2 : " << low2 << endl;
//				cout << "high 2: " << high2 << endl;
//			}*/
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low1 &&
//					V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high1)
//					Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;
//
//				else if (V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low2 &&
//					V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high2)
//					Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;
//
//				else if (V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low3 &&
//					V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols))<= high3)
//					Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
//
//				else if (V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low4 &&
//					V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high4)
//					Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
//			}
//			//if (i == test_i && j == test_j){
//			//	int count = 0;
//			//	cout << "Set of V : " << endl;
//			//	for (int x = i; x < i + 16; x++){
//			//		for (int y = j; y < j + 16; y++){
//			//			cout << Mark_V.at<double>(x, y) << " ";
//			//		}
//			//		cout << endl;
//			//	}
//			//	cout << endl;
//			//	//system("Pause");
//			//}
//			//-------------------------------------做線性回歸(U)-----------------------------------------
//			double Y_group_a[1000] = { 0 };
//			double Y_group_b[1000] = { 0 };
//			double U_group_a[1000] = { 0 };
//			double U_group_b[1000] = { 0 };
//			double suma_U = 0, sumb_U = 0;
//			double a_U_a = 0, b_U_a = 0, bot_a = 0, a_U_b = 0, b_U_b = 0, bot_b = 0;
//			double Y_avg_a = 0, U_avg_a = 0, Y_avg_b = 0, U_avg_b = 0;
//			int count_a = 0, count_b = 0;
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1 && Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1){
//					Y_group_a[count_a] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//					U_group_a[count_a] = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//
//					Y_avg_a += Y_group_a[count_a];
//					U_avg_a += U_group_a[count_a];
//					count_a++;
//				}
//				else if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2 && Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2){
//					Y_group_b[count_b] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//					U_group_b[count_b] = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//
//					Y_avg_b += Y_group_b[count_b];
//					U_avg_b += U_group_b[count_b];
//					count_b++;
//				}
//			}
//			Y_avg_a /= count_a;
//			U_avg_a /= count_a;
//			Y_avg_b /= count_b;
//			U_avg_b /= count_b;
//			//if (i == test_i && j == test_j){
//			//	cout << "count_a : " << count_a << endl;
//			//	cout << "Y_avg_a : " << Y_avg_a << endl;
//			//	cout << "U_avg_a : " << U_avg_a << endl;
//			//	//system("Pause");
//			//}
//
//			for (int z = 0; z < count_a; z++){
//				a_U_a += (Y_group_a[z] - Y_avg_a)*(U_group_a[z] - U_avg_a);
//				bot_a += pow(Y_group_a[z] - Y_avg_a, 2);
//			}
//
//			a_U_a /= (bot_a + 0.00001);
//			b_U_a = U_avg_a - a_U_a*Y_avg_a;
//
//			for (int z = 0; z < count_b; z++){
//				a_U_b += (Y_group_b[z] - Y_avg_b)*(U_group_b[z] - U_avg_b);
//				bot_b += pow(Y_group_b[z] - Y_avg_b, 2);
//			}
//
//			a_U_b /= (bot_b + 0.00001);
//			b_U_b = U_avg_b - a_U_b*Y_avg_b;
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) != Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)))
//					Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//				else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 0){
//					Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//				}
//				else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1){
//					Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_U_a*expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_U_a;
//				}
//				else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2){
//					Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_U_b*expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_U_b;
//				}
//			}
//
//
//			//-------------------------------------做線性回歸(V)-----------------------------------------
//			double V_group_a[1000] = { 0 };
//			double V_group_b[1000] = { 0 };
//			sumb_U = 0;
//			double suma_V = 0, sumb_V = 0;
//			bot_a = 0; bot_b = 0;
//			double a_V_a = 0, b_V_a = 0, a_V_b = 0, b_V_b = 0;
//			Y_avg_a = 0;
//			Y_avg_b = 0;
//			double  V_avg_a = 0, V_avg_b = 0;
//			count_a = 0;
//			count_b = 0;
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1 && Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1){
//					Y_group_a[count_a] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//					V_group_a[count_a] = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//
//					Y_avg_a += Y_group_a[count_a];
//					V_avg_a += V_group_a[count_a];
//					count_a++;
//				}
//				else if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2 && Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2){
//					Y_group_b[count_b] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//					V_group_b[count_b] = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//
//					Y_avg_b += Y_group_b[count_b];
//					V_avg_b += V_group_b[count_b];
//					count_b++;
//				}
//			}
//
//
//			Y_avg_a /= count_a;
//			V_avg_a /= count_a;
//			Y_avg_b /= count_b;
//			V_avg_b /= count_b;
//			//if (i == test_i && j == test_j){
//			//	cout << "Y_avg_b : " << Y_avg_b << endl;
//			//	cout << "V_avg_b : " << V_avg_b << endl;
//			//	//system("Pause");
//			//}
//
//			for (int z = 0; z < count_a; z++){
//				a_V_a += (Y_group_a[z] - Y_avg_a)*(V_group_a[z] - V_avg_a);
//				bot_a += pow(Y_group_a[z] - Y_avg_a, 2);
//			}
//
//			a_V_a /= (bot_a + 0.00001);
//			b_V_a = V_avg_a - a_V_a*Y_avg_a;
//
//
//			for (int z = 0; z < count_b; z++){
//				a_V_b += (Y_group_b[z] - Y_avg_b)*(V_group_b[z] - V_avg_b);
//				bot_b += pow(Y_group_b[z] - Y_avg_b, 2);
//			}
//
//			a_V_b /= (bot_b + 0.00001);
//			b_V_b = V_avg_b - a_V_b*Y_avg_b;
//
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) != Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)))
//					Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//				else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 0){
//					Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//				}
//				else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1){
//					Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_V_a*expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_V_a;
//				}
//				else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2){
//					Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_V_b*expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_V_b;
//				}
//			}
//		}//end Ns>=3
//		else{
//			for (int k = i; k < i + h; k++)
//			for (int l = j; l < j + w; l++){
//				Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//				Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
//			}
//		}
//	}
////	Show(Ufinal);
////	imwrite("21_Ufinal.bmp", Ufinal);
////	imwrite("21_Vfinal.bmp", Vfinal);
////	Show(Out);
//}
void bilinear(Mat& expand444, Mat& Ufinal, Mat& Vfinal) {
	/*抽樣UV在2x2左上位置的Bilinear放大*/
	Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	for (int i = 0; i < expand444.rows; i++)
		for (int j = 0; j < expand444.cols; j++) {
			if (i % 2 == 0 && j % 2 == 0) {
				Ufinal.at<double>(i, j) = expand444.at<Vec3d>(i, j)[1];
				Vfinal.at<double>(i, j) = expand444.at<Vec3d>(i, j)[2];
			}
			else if (i % 2 == 0 && j % 2 == 1) {
				Ufinal.at<double>(i, j) = (expand444.at<Vec3d>(i, mir(j - 1, expand444.cols))[1] + expand444.at<Vec3d>(i, mir(j + 1, expand444.cols))[1]) / 2;
				Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(i, mir(j - 1, expand444.cols))[2] + expand444.at<Vec3d>(i, mir(j + 1, expand444.cols))[2]) / 2;
			}
			else if (i % 2 == 1 && j % 2 == 0) {
				Ufinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), j)[1] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), j)[1]) / 2;
				Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), j)[2] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), j)[2]) / 2;
			}
			else if (i % 2 == 1 && j % 2 == 1) {
				Ufinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[1] + expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[1]
					+ expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[1] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[1]) / 4;
				Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[2] + expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[2]
					+ expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[2] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[2]) / 4;
			}
		}
}

void bicubic(Mat& expand444, Mat& Ufinal, Mat& Vfinal) {
	Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	//將已知點像素值複製到mTUPSaRViewBC
	for (int iForIndexH = 0; iForIndexH < expand444.rows; iForIndexH++)
	{
		for (int iForIndexW = 0; iForIndexW < expand444.cols; iForIndexW++)
		{
			if (iForIndexH % 2 == 0 && iForIndexW % 2 == 0) {
				Ufinal.at<double>(iForIndexH, iForIndexW) = expand444.at<Vec3d>(iForIndexH, iForIndexW)[1];
				Vfinal.at<double>(iForIndexH, iForIndexW) = expand444.at<Vec3d>(iForIndexH, iForIndexW)[2];
			}
		}
	}

	//先做U平面
	double fWieghtSum = 0.0;
	for (int iForIndexH = 0; iForIndexH < expand444.rows; iForIndexH++) {
		for (int iForIndexW = 0; iForIndexW < expand444.cols; iForIndexW++) {
			if (iForIndexH % 2 == 0 && iForIndexW % 2 == 0) continue;

			int iOffset_Y;
			int iOffset_X;
			double fSumTemp = 0.0;
			fWieghtSum = 0.0;

			if (iForIndexH % 2 == 1 && iForIndexW % 2 == 1) {
				for (int iInitY = -5; iInitY < 6; iInitY += 2)
				{
					for (int iInitX = -5; iInitX < 6; iInitX += 2)
					{
						if (iForIndexH + iInitY >= 0 && iForIndexH + iInitY < expand444.rows)
							iOffset_Y = iForIndexH + iInitY;
						else continue;

						if (iForIndexW + iInitX >= 0 && iForIndexW + iInitX < expand444.cols)
							iOffset_X = iForIndexW + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += expand444.at<Vec3d>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols))[1] * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX)); //權重加總值，正規化
					}
				}
			}
			else if (iForIndexH % 2 == 1 && iForIndexW % 2 == 0) {
				for (int iInitY = -5; iInitY < 6; iInitY += 2)
				{
					for (int iInitX = -4; iInitX < 7; iInitX += 2)
					{
						if (iForIndexH + iInitY >= 0 && iForIndexH + iInitY < expand444.rows)
							iOffset_Y = iForIndexH + iInitY;
						else continue;

						if (iForIndexW + iInitX >= 0 && iForIndexW + iInitX < expand444.cols)
							iOffset_X = iForIndexW + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += expand444.at<Vec3d>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols))[1] * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX));
					}
				}
			}
			else if (iForIndexH % 2 == 0 && iForIndexW % 2 == 1) {
				for (int iInitY = -4; iInitY < 7; iInitY += 2)
				{
					for (int iInitX = -5; iInitX < 6; iInitX += 2)
					{
						if (iForIndexH + iInitY >= 0 && iForIndexH + iInitY < expand444.rows)
							iOffset_Y = iForIndexH + iInitY;
						else continue;

						if (iForIndexW + iInitX >= 0 && iForIndexW + iInitX < expand444.cols)
							iOffset_X = iForIndexW + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += expand444.at<Vec3d>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols))[1] * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX));
					}
				}
			}
			fSumTemp /= fWieghtSum;
			//if (fSumTemp > 255) fSumTemp = 255; //超出像素值範圍
			//else if (fSumTemp < 0) fSumTemp = 0; //超出像素值範圍
			//mTUPSaRViewBC.at<double>(iForIndexH, iForIndexW) = fSumTemp;
			Ufinal.at<double>(iForIndexH, iForIndexW) = fSumTemp;
			//		cout << "(" << iForIndexH << ", " << iForIndexW << ")" << endl;
		}
	}
	//	Show(Ufinal);
	//	imwrite("U(0,0).bmp", Ufinal);
		//再做V平面
	fWieghtSum = 0.0;
	for (int iForIndexH = 0; iForIndexH < expand444.rows; iForIndexH++) {
		for (int iForIndexW = 0; iForIndexW < expand444.cols; iForIndexW++) {
			if (iForIndexH % 2 == 0 && iForIndexW % 2 == 0) continue;

			int iOffset_Y;
			int iOffset_X;
			double fSumTemp = 0.0;
			fWieghtSum = 0.0;

			if (iForIndexH % 2 == 1 && iForIndexW % 2 == 1) { // 右下
				for (int iInitY = -5; iInitY < 6; iInitY += 2)
				{
					for (int iInitX = -5; iInitX < 6; iInitX += 2)
					{
						if (iForIndexH + iInitY >= 0 && iForIndexH + iInitY < expand444.rows)
							iOffset_Y = iForIndexH + iInitY;
						else continue;

						if (iForIndexW + iInitX >= 0 && iForIndexW + iInitX < expand444.cols)
							iOffset_X = iForIndexW + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += expand444.at<Vec3d>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols))[2] * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX)); //權重加總值，正規化
					}
				}
			}
			else if (iForIndexH % 2 == 1 && iForIndexW % 2 == 0) { //左下
				for (int iInitY = -5; iInitY < 6; iInitY += 2)
				{
					for (int iInitX = -4; iInitX < 7; iInitX += 2)
					{
						if (iForIndexH + iInitY >= 0 && iForIndexH + iInitY < expand444.rows)
							iOffset_Y = iForIndexH + iInitY;
						else continue;

						if (iForIndexW + iInitX >= 0 && iForIndexW + iInitX < expand444.cols)
							iOffset_X = iForIndexW + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += expand444.at<Vec3d>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols))[2] * BC_funtion(fDY)	*BC_funtion(fDX);

						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX));
					}
				}
			}
			else if (iForIndexH % 2 == 0 && iForIndexW % 2 == 1) { // 右上
				for (int iInitY = -4; iInitY < 7; iInitY += 2)
				{
					for (int iInitX = -5; iInitX < 6; iInitX += 2)
					{
						if (iForIndexH + iInitY >= 0 && iForIndexH + iInitY < expand444.rows)
							iOffset_Y = iForIndexH + iInitY;
						else continue;

						if (iForIndexW + iInitX >= 0 && iForIndexW + iInitX < expand444.cols)
							iOffset_X = iForIndexW + iInitX;
						else continue;

						double fDY = (double)iInitY / 2.0;
						double fDX = (double)iInitX / 2.0;

						fSumTemp += expand444.at<Vec3d>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols))[2] * BC_funtion(fDY)*BC_funtion(fDX);
						fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX));
					}
				}
			}
			fSumTemp /= fWieghtSum;
			//if (fSumTemp > 255) fSumTemp = 255; //超出像素值範圍
			//else if (fSumTemp < 0) fSumTemp = 0; //超出像素值範圍
			//mTUPSaRViewBC.at<double>(iForIndexH, iForIndexW) = fSumTemp;
			Vfinal.at<double>(iForIndexH, iForIndexW) = fSumTemp;
		}
	}
}



void bicubic2(const Mat& DS_U_plane, const Mat& DS_V_plane, Mat& US_U_plane, Mat& US_V_plane) {

	int height = DS_U_plane.rows * 2;
	int width = DS_U_plane.cols * 2;
	US_U_plane = Mat::zeros(height, width, CV_64FC1);
	US_V_plane = Mat::zeros(height, width, CV_64FC1);
	double a = -0.5;
	double dx = 0.5;
	double dy = 0.5;
	double x, y;
	int X, Y;
	double u, v;
	double w_x[4], w_y[4], s_x[4], s_y[4];
	double tmp;
	Mat U_plane, V_plane;
	/*copyMakeBorder(US_U_plane, US_U_plane, 0, 0, 1, 0, BORDER_REPLICATE);
	copyMakeBorder(US_V_plane, US_V_plane, 0, 0, 1, 0, BORDER_REPLICATE);*/
	copyMakeBorder(DS_U_plane, U_plane, 1, 3, 1, 3, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, V_plane, 1, 3, 1, 3, BORDER_REPLICATE);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			x = i * dx;
			y = j * dy;
			//算權重
			X = (int)x;
			Y = (int)y;
			u = x - X;
			v = y - Y;
			s_x[0] = 1 + u;
			s_x[1] = u;
			s_x[2] = 1 - u;
			s_x[3] = 2 - u;
			w_x[0] = a * abs(s_x[0] * s_x[0] * s_x[0]) - 5 * a*s_x[0] * s_x[0] + 8 * a*abs(s_x[0]) - 4 * a;
			w_x[1] = (a + 2)*abs(s_x[1] * s_x[1] * s_x[1]) - (a + 3)*s_x[1] * s_x[1] + 1;
			w_x[2] = (a + 2)*abs(s_x[2] * s_x[2] * s_x[2]) - (a + 3)*s_x[2] * s_x[2] + 1;
			w_x[3] = a * abs(s_x[3] * s_x[3] * s_x[3]) - 5 * a*s_x[3] * s_x[3] + 8 * a*abs(s_x[3]) - 4 * a;

			s_y[0] = 1 + v;
			s_y[1] = v;
			s_y[2] = 1 - v;
			s_y[3] = 2 - v;
			w_y[0] = a * abs(s_y[0] * s_y[0] * s_y[0]) - 5 * a*s_y[0] * s_y[0] + 8 * a*abs(s_y[0]) - 4 * a;
			w_y[1] = (a + 2)*abs(s_y[1] * s_y[1] * s_y[1]) - (a + 3)*s_y[1] * s_y[1] + 1;
			w_y[2] = (a + 2)*abs(s_y[2] * s_y[2] * s_y[2]) - (a + 3)*s_y[2] * s_y[2] + 1;
			w_y[3] = a * abs(s_y[3] * s_y[3] * s_y[3]) - 5 * a*s_y[3] * s_y[3] + 8 * a*abs(s_y[3]) - 4 * a;

			for (int s = 0; s <= 3; s++) {
				for (int t = 0; t <= 3; t++) {
					US_U_plane.at<double>(i, j) += U_plane.at<double>(X + s, Y + t)*w_x[s] * w_y[t];
					US_V_plane.at<double>(i, j) += V_plane.at<double>(X + s, Y + t)*w_x[s] * w_y[t];
				}
			}

		}

	}

	//US_U_plane = US_U_plane(Rect(1, 0, width, height));
	//US_V_plane = US_V_plane(Rect(1, 0, width, height));
}

//void Chroma_Left_Shift(Mat &U, Mat &V) {
//
//	/*將放置於 2x2 block 左上位置的抽樣Chroma所重建的Chroma平面下移至左中的位置*/
//	Mat tmp_U, tmp_V;
//	copyMakeBorder(U, tmp_U, 0, 0, 0, 1, BORDER_REFLECT);
//	copyMakeBorder(V, tmp_V, 0, 0, 0, 1, BORDER_REFLECT);
//
//	for (int i = 0; i < U.rows; i++)
//		for (int j = 0; j < U.cols; j++) {
//			U.at<double>(i, j) = (tmp_U.at<double>(i, j) + tmp_U.at<double>(i, j + 1)) / 2;
//			V.at<double>(i, j) = (tmp_V.at<double>(i, j) + tmp_V.at<double>(i, j + 1)) / 2;
//		}
//}


void Chroma_Shift_420A(Mat &U, Mat &V) {

	/*將放置於 2x2 block 左上位置的抽樣Chroma所重建的Chroma平面移至中間*/
	Mat tmp_U, tmp_V;
	copyMakeBorder(U, tmp_U, 1, 0, 1, 0, BORDER_REFLECT);
	copyMakeBorder(V, tmp_V, 1, 0, 1, 0, BORDER_REFLECT);


	for (int i = 0; i < U.rows; i++)
		for (int j = 0; j < U.cols; j++) {
			U.at<double>(i, j) = (tmp_U.at<double>(i, j) + tmp_U.at<double>(i + 1, j) + tmp_U.at<double>(i, j + 1) + tmp_U.at<double>(i + 1, j + 1)) / 4;
			V.at<double>(i, j) = (tmp_V.at<double>(i, j) + tmp_V.at<double>(i + 1, j) + tmp_V.at<double>(i, j + 1) + tmp_V.at<double>(i + 1, j + 1)) / 4;
		}
}

void Chroma_Shift_420L(Mat &U, Mat &V) {

	/*將放置於 2x2 block 左上位置的抽樣Chroma所重建的Chroma平面下移至左中*/
	Mat tmp_U, tmp_V;
	copyMakeBorder(U, tmp_U, 1, 0, 0, 0, BORDER_REFLECT);
	copyMakeBorder(V, tmp_V, 1, 0, 0, 0, BORDER_REFLECT);

	for (int i = 0; i < U.rows; i++)
		for (int j = 0; j < U.cols; j++) {
			U.at<double>(i, j) = (tmp_U.at<double>(i, j) + tmp_U.at<double>(i + 1, j)) / 2;
			V.at<double>(i, j) = (tmp_V.at<double>(i, j) + tmp_V.at<double>(i + 1, j)) / 2;
		}
}

void Chroma_Shift_420R(Mat &U, Mat &V) {

	/*將放置於 2x2 block 左上位置的抽樣Chroma所重建的Chroma平面下移至右中*/
	Mat tmp_U, tmp_V;
	copyMakeBorder(U, tmp_U, 1, 0, 1, 0, BORDER_REFLECT);
	copyMakeBorder(V, tmp_V, 1, 0, 1, 0, BORDER_REFLECT);

	for (int i = 0; i < U.rows; i++)
		for (int j = 0; j < U.cols; j++) {
			U.at<double>(i, j) = (tmp_U.at<double>(i, j) + tmp_U.at<double>(i + 1, j)) / 2;
			V.at<double>(i, j) = (tmp_V.at<double>(i, j) + tmp_V.at<double>(i + 1, j)) / 2;
		}
}

void Upsample_LeftCenter_Bilinear(const Mat& DS_plane, Mat& US_plane) {

	US_plane = Mat::zeros(DS_plane.rows * 2, DS_plane.cols * 2, CV_64FC1);
	Mat plane;
	copyMakeBorder(DS_plane, plane, 1, 1, 0, 1, BORDER_REPLICATE);
	int xx;
	int yy = 1;
	for (int y = 0; y < US_plane.rows; y += 2)
	{
		xx = 0;
		for (int x = 0; x < US_plane.cols; x += 2)
		{
			US_plane.at<double>(y, x) = (plane.at<double>(yy, xx) * 3 + plane.at<double>(yy - 1, xx)) / 4;
			US_plane.at<double>(y, x + 1) = (plane.at<double>(yy, xx) * 3 + plane.at<double>(yy - 1, xx) + plane.at<double>(yy, xx + 1) * 3 + plane.at<double>(yy - 1, xx + 1)) / 8;
			US_plane.at<double>(y + 1, x) = (plane.at<double>(yy, xx) * 3 + plane.at<double>(yy + 1, xx)) / 4;
			US_plane.at<double>(y + 1, x + 1) = (plane.at<double>(yy, xx) * 3 + plane.at<double>(yy + 1, xx) + plane.at<double>(yy, xx + 1) * 3 + plane.at<double>(yy + 1, xx + 1)) / 8;

			xx++;
		}
		yy++;
	}
}

void KW(Mat& Ufinal, Mat& Vfinal, const Mat& new_Y_plane, const  Mat& DS_U_plane, const  Mat& DS_V_plane, const  Mat&  YUV) {
	//int test_i = 400;
	//int test_j = 912;

	int height = DS_U_plane.rows * 2;
	int width = DS_U_plane.cols * 2;
	Mat expand444 = Mat::zeros(height, width, CV_64FC3);


	Ufinal = Mat::zeros(height, width, CV_64FC1);
	Vfinal = Mat::zeros(height, width, CV_64FC1);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			expand444.at<Vec3d>(y, x)[0] = new_Y_plane.at<double>(y, x);

		}
	for (int y = 0; y < DS_U_plane.rows; y++)
		for (int x = 0; x < DS_U_plane.cols; x++)
		{
			expand444.at<Vec3d>(y * 2, x * 2)[1] = DS_U_plane.at<double>(y, x);
			expand444.at<Vec3d>(y * 2, x * 2)[2] = DS_V_plane.at<double>(y, x);
		}

	Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat GT_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat GT_V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	Mat U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	Mat Mark_Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat Mark_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat Mark_V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	for (int i = 0; i < expand444.rows; i++)
		for (int j = 0; j < expand444.cols; j++) {
			GT_U.at<double>(i, j) = YUV.at<Vec3d>(i, j)[1];
			GT_V.at<double>(i, j) = YUV.at<Vec3d>(i, j)[2];
		}
	//Show(GT_U);
	//Show(GT_V);

	Mat Out = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	//bilinear_weight(expand444, U, V);//詭異的程式...
	bilinear(expand444, U, V);//這才正常
	//Show(U);
	//Show(V);
	//----------------------------block-wise content analysis------------------------------------
	int h = 16, w = 16;
	int Ns;
	for (int i = 0; i < expand444.rows; i += 16)
		for (int j = 0; j < expand444.cols; j += 16) {
			Ns = 0;
			for (int k = i; k < i + 16; k++)
				for (int l = j; l < j + 16; l++) {
					if (abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k - 1, expand444.rows), mir(l, expand444.cols))[0]) > 32 ||
						abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k, expand444.rows), mir(l - 1, expand444.cols))[0]) > 32) {
						Ns++;
					}
				}
			if (Ns > 3) {
				Out.at<double>(i, j) = 255;
				//-------------------------------------做BCIM(Y)-----------------------------------------
				Mat src = Mat::zeros(16, 16, CV_8UC1);
				Mat all = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
				int kk = 0, ll = 0;
				for (int k = i; k < i + 16; k++) {
					for (int l = j; l < j + 16; l++) {
						src.at<uchar>(kk, ll) = (int)expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
						all.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
						ll++;
					}
					ll = 0;
					kk++;
				}
				//histogram
				float range[] = { 0, 255 };
				const float* histRange = { range };
				int histSize = 256;
				Mat histImg;
				calcHist(&src, 1, 0, Mat(), histImg, 1, &histSize, &histRange);
				//Mat showHistImg(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
				//drawHistImg(histImg, showHistImg);
				//			imshow("all", all);
				//			imshow("window1", src);
				//			imshow("window2", showHistImg);
				//if (i == test_i && j == test_j)
				//	imwrite("21_window1.bmp", src);

				//save bsase color
				int bimrange = 2;//左右各2pixel區間
				double MinValue;
				double MaxValue;
				Point MinLocation;
				Point MaxLocation;
				double Y_basecolor[4] = { 0 };

				//把數量最多的顏色(背景色)剔除
				minMaxLoc(histImg, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
					histImg.at<float>(mir(z, 256)) = 0;

				for (int x = 0; x < 4; x++) {
					minMaxLoc(histImg, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
					//cout << "The Max number is : " << MaxValue << endl;
					//cout << "The position is :  " << MaxLocation.x << ", " << MaxLocation.y << endl;
					Y_basecolor[x] = MaxLocation.y;
					for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
						histImg.at<float>(mir(z, 256)) = 0;
					//cout << "======================================" << endl;
				}

				/*if (i == test_i && j == test_j){
					cout << endl << "Base Color of Y : " << endl;
					for (int x = 0; x < 4; x++)
						cout << Y_basecolor[x] << endl;
					waitKey(0);
				}*/
				//-----------------------------------------Y標記---------------------------------------------
				int low1 = Y_basecolor[0] - bimrange;
				int high1 = Y_basecolor[0] + bimrange;
				int low2 = Y_basecolor[1] - bimrange;
				int high2 = Y_basecolor[1] + bimrange;
				int low3 = Y_basecolor[2] - bimrange;
				int high3 = Y_basecolor[2] + bimrange;
				int low4 = Y_basecolor[3] - bimrange;
				int high4 = Y_basecolor[3] + bimrange;
				/*if (i == test_i && j == test_j){
					cout << "low 2 : " << low2 << endl;
					cout << "high 2: " << high2 << endl;
				}*/

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low1 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high1)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low2 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high2)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low3 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high3)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;

						else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low4 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high4)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
					}
				/*if (i == test_i && j == test_j){
					int count = 0;
					cout << "Set of Y : " << endl;
					for (int x = i; x < i + 16; x++){
						for (int y = j; y < j + 16; y++){
							cout << Mark_Y.at<double>(x, y) << " ";
						}
						cout << endl;
					}
					cout << endl;
					system("Pause");
				}*/
				//-------------------------------------做BCIM(U)-----------------------------------------
				Mat src_U = Mat::zeros(16, 16, CV_8UC1);
				Mat all_U = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
				kk = 0;
				ll = 0;
				for (int k = i; k < i + 16; k++) {
					for (int l = j; l < j + 16; l++) {
						src_U.at<uchar>(kk, ll) = (int)GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						all_U.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						ll++;
					}
					ll = 0;
					kk++;
				}
				//histogram
				float range_U[] = { 0, 255 };
				const float* histRange_U = { range };
				//histSize = 256;
				Mat histImg_U;
				calcHist(&src_U, 1, 0, Mat(), histImg_U, 1, &histSize, &histRange_U);
				//Mat showHistImg_U(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
				//drawHistImg(histImg_U, showHistImg_U);
				//			imshow("all", all);
				//			imshow("window1", src_U);
				//			imshow("window2", showHistImg_U);
				//if (i == test_i && j == test_j)
				//	imwrite("21_window1_U.bmp", src_U);

				//save bsase color
				//int bimrange = 10;//左右各10pixel區間
				//double MinValue;
				//double MaxValue;
				//Point MinLocation;
				//Point MaxLocation;
				double U_basecolor[4] = { 0 };

				//把數量最多的顏色(背景色)剔除
				minMaxLoc(histImg_U, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
					histImg_U.at<float>(mir(z, 256)) = 0;

				for (int x = 0; x < 4; x++) {
					minMaxLoc(histImg_U, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
					//cout << "The Max number is : " << MaxValue << endl;
					//cout << "The position is :  " << MaxLocation.x << ", " << MaxLocation.y << endl;
					U_basecolor[x] = MaxLocation.y;
					for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
						histImg_U.at<float>(mir(z, 256)) = 0;
					//cout << "======================================" << endl;
				}

				//if (i == test_i && j == test_j){
				//	cout << endl << "Base Color of U : " << endl;
				//	for (int x = 0; x < 4; x++)
				//		cout << U_basecolor[x] << endl;
				////	waitKey(0);
				//}
				//-----------------------------------------U標記---------------------------------------------
				low1 = U_basecolor[0] - bimrange;
				high1 = U_basecolor[0] + bimrange;
				low2 = U_basecolor[1] - bimrange;
				high2 = U_basecolor[1] + bimrange;
				low3 = U_basecolor[2] - bimrange;
				high3 = U_basecolor[2] + bimrange;
				low4 = U_basecolor[3] - bimrange;
				high4 = U_basecolor[3] + bimrange;
				/*if (i == test_i && j == test_j){
					cout << "low 2 : " << low2 << endl;
					cout << "high 2: " << high2 << endl;
				}*/

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low1 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high1)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low2 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high2)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low3 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high3)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;

						else if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low4 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high4)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
					}
				/*if (i == test_i && j == test_j){
					int count = 0;
					cout << "Set of U : " << endl;
					for (int x = i; x < i + 16; x++){
						for (int y = j; y < j + 16; y++){
							cout << Mark_U.at<double>(x, y) << " ";
						}
						cout << endl;
					}
					cout << endl;
					system("Pause");
				}*/
				//-------------------------------------做BCIM(V)-----------------------------------------
				Mat src_V = Mat::zeros(16, 16, CV_8UC1);
				Mat all_V = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
				kk = 0;
				ll = 0;
				for (int k = i; k < i + 16; k++) {
					for (int l = j; l < j + 16; l++) {
						src_V.at<uchar>(kk, ll) = (int)GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						all_V.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						ll++;
					}
					ll = 0;
					kk++;
				}
				//histogram
				float range_V[] = { 0, 255 };
				const float* histRange_V = { range };
				Mat histImg_V;
				calcHist(&src_V, 1, 0, Mat(), histImg_V, 1, &histSize, &histRange_V);
				//Mat showHistImg_V(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
				//drawHistImg(histImg_V, showHistImg_V);
				//		imshow("all", all);
				//		imshow("window1", src_V);
				//		imshow("window2", showHistImg_V);
				//if (i == test_i && j == test_j)
				//	imwrite("21_window1_V.bmp", src_V);

				double V_basecolor[4] = { 0 };

				//把數量最多的顏色(背景色)剔除
				minMaxLoc(histImg_V, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
					histImg_V.at<float>(mir(z, 256)) = 0;

				for (int x = 0; x < 4; x++) {
					minMaxLoc(histImg_V, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
					V_basecolor[x] = MaxLocation.y;
					for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
						histImg_V.at<float>(mir(z, 256)) = 0;
					//cout << "======================================" << endl;
				}

				/*if (i == test_i && j == test_j){
					cout << endl << "Base Color of V : " << endl;
					for (int x = 0; x < 4; x++)
						cout << V_basecolor[x] << endl;
					waitKey(0);
				}*/
				//-----------------------------------------V標記---------------------------------------------

				low1 = V_basecolor[0] - bimrange;
				high1 = V_basecolor[0] + bimrange;
				low2 = V_basecolor[1] - bimrange;
				high2 = V_basecolor[1] + bimrange;
				low3 = V_basecolor[2] - bimrange;
				high3 = V_basecolor[2] + bimrange;
				low4 = V_basecolor[3] - bimrange;
				high4 = V_basecolor[3] + bimrange;
				/*if (i == test_i && j == test_j){
					cout << "low 2 : " << low2 << endl;
					cout << "high 2: " << high2 << endl;
				}*/

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low1 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high1)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low2 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high2)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low3 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high3)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;

						else if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low4 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high4)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
					}
				/*if (i == test_i && j == test_j){
					int count = 0;
					cout << "Set of V : " << endl;
					for (int x = i; x < i + 16; x++){
						for (int y = j; y < j + 16; y++){
							cout << Mark_V.at<double>(x, y) << " ";
						}
						cout << endl;
					}
					cout << endl;
					system("Pause");
				}*/
				//-------------------------------------做線性回歸(U)-----------------------------------------
				double Y_group_a[1000] = { 0 };
				double Y_group_b[1000] = { 0 };
				double U_group_a[1000] = { 0 };
				double U_group_b[1000] = { 0 };
				double suma_U = 0, sumb_U = 0;
				double a_U_a = 0, b_U_a = 0, bot_a = 0, a_U_b = 0, b_U_b = 0, bot_b = 0;
				double Y_avg_a = 0, U_avg_a = 0, Y_avg_b = 0, U_avg_b = 0;
				int count_a = 0, count_b = 0;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1 && Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Y_group_a[count_a] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							U_group_a[count_a] = GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_a += Y_group_a[count_a];
							U_avg_a += U_group_a[count_a];
							count_a++;
						}
						else if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2 && Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Y_group_b[count_b] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							U_group_b[count_b] = GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_b += Y_group_b[count_b];
							U_avg_b += U_group_b[count_b];
							count_b++;
						}
					}
				Y_avg_a /= count_a;
				U_avg_a /= count_a;
				Y_avg_b /= count_b;
				U_avg_b /= count_b;
				/*if (i == test_i && j == test_j){
					cout << "count_a : " << count_a << endl;
					cout << "Y_avg_a : " << Y_avg_a << endl;
					cout << "U_avg_a : " << U_avg_a << endl;
					system("Pause");
				}*/

				for (int z = 0; z < count_a; z++) {
					a_U_a += (Y_group_a[z] - Y_avg_a)*(U_group_a[z] - U_avg_a);
					bot_a += pow(Y_group_a[z] - Y_avg_a, 2);
				}

				a_U_a /= (bot_a + 0.00001);
				b_U_a = U_avg_a - a_U_a * Y_avg_a;

				for (int z = 0; z < count_b; z++) {
					a_U_b += (Y_group_b[z] - Y_avg_b)*(U_group_b[z] - U_avg_b);
					bot_b += pow(Y_group_b[z] - Y_avg_b, 2);
				}

				a_U_b /= (bot_b + 0.00001);
				b_U_b = U_avg_b - a_U_b * Y_avg_b;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) != Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)))
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 0) {
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						}
						else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_U_a * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_U_a;
						}
						else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_U_b * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_U_b;
						}
					}


				//-------------------------------------做線性回歸(V)-----------------------------------------
				double V_group_a[1000] = { 0 };
				double V_group_b[1000] = { 0 };
				sumb_U = 0;
				double suma_V = 0, sumb_V = 0;
				bot_a = 0; bot_b = 0;
				double a_V_a = 0, b_V_a = 0, a_V_b = 0, b_V_b = 0;
				Y_avg_a = 0;
				Y_avg_b = 0;
				double  V_avg_a = 0, V_avg_b = 0;
				count_a = 0;
				count_b = 0;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1 && Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Y_group_a[count_a] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							V_group_a[count_a] = GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_a += Y_group_a[count_a];
							V_avg_a += V_group_a[count_a];
							count_a++;
						}
						else if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2 && Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Y_group_b[count_b] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							V_group_b[count_b] = GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_b += Y_group_b[count_b];
							V_avg_b += V_group_b[count_b];
							count_b++;
						}
					}


				Y_avg_a /= count_a;
				V_avg_a /= count_a;
				Y_avg_b /= count_b;
				V_avg_b /= count_b;
				/*if (i == test_i && j == test_j){
					cout << "Y_avg_b : " << Y_avg_b << endl;
					cout << "V_avg_b : " << V_avg_b << endl;
					system("Pause");
				}*/

				for (int z = 0; z < count_a; z++) {
					a_V_a += (Y_group_a[z] - Y_avg_a)*(V_group_a[z] - V_avg_a);
					bot_a += pow(Y_group_a[z] - Y_avg_a, 2);
				}

				a_V_a /= (bot_a + 0.00001);
				b_V_a = V_avg_a - a_V_a * Y_avg_a;


				for (int z = 0; z < count_b; z++) {
					a_V_b += (Y_group_b[z] - Y_avg_b)*(V_group_b[z] - V_avg_b);
					bot_b += pow(Y_group_b[z] - Y_avg_b, 2);
				}

				a_V_b /= (bot_b + 0.00001);
				b_V_b = V_avg_b - a_V_b * Y_avg_b;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) != Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)))
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 0) {
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						}
						else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_V_a * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_V_a;
						}
						else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_V_b * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_V_b;
						}
					}
			}//end Ns>=3
			else {
				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
					}
			}
		}
}





void KW_BC(Mat& Ufinal, Mat& Vfinal, const Mat& new_Y_plane, const  Mat& DS_U_plane, const  Mat& DS_V_plane, const  Mat&  YUV) {
	//int test_i = 400;
	//int test_j = 912;

	int height = DS_U_plane.rows * 2;
	int width = DS_U_plane.cols * 2;
	Mat expand444 = Mat::zeros(height, width, CV_64FC3);


	Ufinal = Mat::zeros(height, width, CV_64FC1);
	Vfinal = Mat::zeros(height, width, CV_64FC1);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			expand444.at<Vec3d>(y, x)[0] = new_Y_plane.at<double>(y, x);

		}
	for (int y = 0; y < DS_U_plane.rows; y++)
		for (int x = 0; x < DS_U_plane.cols; x++)
		{
			expand444.at<Vec3d>(y * 2, x * 2)[1] = DS_U_plane.at<double>(y, x);
			expand444.at<Vec3d>(y * 2, x * 2)[2] = DS_V_plane.at<double>(y, x);
		}

	Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat GT_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat GT_V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	Mat U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	Mat Mark_Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat Mark_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	Mat Mark_V = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

	for (int i = 0; i < expand444.rows; i++)
		for (int j = 0; j < expand444.cols; j++) {
			GT_U.at<double>(i, j) = YUV.at<Vec3d>(i, j)[1];
			GT_V.at<double>(i, j) = YUV.at<Vec3d>(i, j)[2];
		}
	//Show(GT_U);
	//Show(GT_V);

	Mat Out = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
	//bilinear_weight(expand444, U, V);//詭異的程式...
	bicubic(expand444, U, V);//這才正常
	//Show(U);
	//Show(V);
	//----------------------------block-wise content analysis------------------------------------
	int h = 16, w = 16;
	int Ns;
	for (int i = 0; i < expand444.rows; i += 16)
		for (int j = 0; j < expand444.cols; j += 16) {
			Ns = 0;
			for (int k = i; k < i + 16; k++)
				for (int l = j; l < j + 16; l++) {
					if (abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k - 1, expand444.rows), mir(l, expand444.cols))[0]) > 32 ||
						abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k, expand444.rows), mir(l - 1, expand444.cols))[0]) > 32) {
						Ns++;
					}
				}
			if (Ns > 3) {
				Out.at<double>(i, j) = 255;
				//-------------------------------------做BCIM(Y)-----------------------------------------
				Mat src = Mat::zeros(16, 16, CV_8UC1);
				Mat all = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
				int kk = 0, ll = 0;
				for (int k = i; k < i + 16; k++) {
					for (int l = j; l < j + 16; l++) {
						src.at<uchar>(kk, ll) = (int)expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
						all.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
						ll++;
					}
					ll = 0;
					kk++;
				}
				//histogram
				float range[] = { 0, 255 };
				const float* histRange = { range };
				int histSize = 256;
				Mat histImg;
				calcHist(&src, 1, 0, Mat(), histImg, 1, &histSize, &histRange);
				//Mat showHistImg(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
				//drawHistImg(histImg, showHistImg);
				//			imshow("all", all);
				//			imshow("window1", src);
				//			imshow("window2", showHistImg);
				//if (i == test_i && j == test_j)
				//	imwrite("21_window1.bmp", src);

				//save bsase color
				int bimrange = 2;//左右各2pixel區間
				double MinValue;
				double MaxValue;
				Point MinLocation;
				Point MaxLocation;
				double Y_basecolor[4] = { 0 };

				//把數量最多的顏色(背景色)剔除
				minMaxLoc(histImg, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
					histImg.at<float>(mir(z, 256)) = 0;

				for (int x = 0; x < 4; x++) {
					minMaxLoc(histImg, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
					//cout << "The Max number is : " << MaxValue << endl;
					//cout << "The position is :  " << MaxLocation.x << ", " << MaxLocation.y << endl;
					Y_basecolor[x] = MaxLocation.y;
					for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
						histImg.at<float>(mir(z, 256)) = 0;
					//cout << "======================================" << endl;
				}

				/*if (i == test_i && j == test_j){
					cout << endl << "Base Color of Y : " << endl;
					for (int x = 0; x < 4; x++)
						cout << Y_basecolor[x] << endl;
					waitKey(0);
				}*/
				//-----------------------------------------Y標記---------------------------------------------
				int low1 = Y_basecolor[0] - bimrange;
				int high1 = Y_basecolor[0] + bimrange;
				int low2 = Y_basecolor[1] - bimrange;
				int high2 = Y_basecolor[1] + bimrange;
				int low3 = Y_basecolor[2] - bimrange;
				int high3 = Y_basecolor[2] + bimrange;
				int low4 = Y_basecolor[3] - bimrange;
				int high4 = Y_basecolor[3] + bimrange;
				/*if (i == test_i && j == test_j){
					cout << "low 2 : " << low2 << endl;
					cout << "high 2: " << high2 << endl;
				}*/

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low1 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high1)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low2 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high2)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low3 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high3)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;

						else if (expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] >= low4 &&
							expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] <= high4)
							Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
					}
				/*if (i == test_i && j == test_j){
					int count = 0;
					cout << "Set of Y : " << endl;
					for (int x = i; x < i + 16; x++){
						for (int y = j; y < j + 16; y++){
							cout << Mark_Y.at<double>(x, y) << " ";
						}
						cout << endl;
					}
					cout << endl;
					system("Pause");
				}*/
				//-------------------------------------做BCIM(U)-----------------------------------------
				Mat src_U = Mat::zeros(16, 16, CV_8UC1);
				Mat all_U = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
				kk = 0;
				ll = 0;
				for (int k = i; k < i + 16; k++) {
					for (int l = j; l < j + 16; l++) {
						src_U.at<uchar>(kk, ll) = (int)GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						all_U.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						ll++;
					}
					ll = 0;
					kk++;
				}
				//histogram
				float range_U[] = { 0, 255 };
				const float* histRange_U = { range };
				//histSize = 256;
				Mat histImg_U;
				calcHist(&src_U, 1, 0, Mat(), histImg_U, 1, &histSize, &histRange_U);
				//Mat showHistImg_U(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
				//drawHistImg(histImg_U, showHistImg_U);
				//			imshow("all", all);
				//			imshow("window1", src_U);
				//			imshow("window2", showHistImg_U);
				//if (i == test_i && j == test_j)
				//	imwrite("21_window1_U.bmp", src_U);

				//save bsase color
				//int bimrange = 10;//左右各10pixel區間
				//double MinValue;
				//double MaxValue;
				//Point MinLocation;
				//Point MaxLocation;
				double U_basecolor[4] = { 0 };

				//把數量最多的顏色(背景色)剔除
				minMaxLoc(histImg_U, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
					histImg_U.at<float>(mir(z, 256)) = 0;

				for (int x = 0; x < 4; x++) {
					minMaxLoc(histImg_U, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
					//cout << "The Max number is : " << MaxValue << endl;
					//cout << "The position is :  " << MaxLocation.x << ", " << MaxLocation.y << endl;
					U_basecolor[x] = MaxLocation.y;
					for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
						histImg_U.at<float>(mir(z, 256)) = 0;
					//cout << "======================================" << endl;
				}

				//if (i == test_i && j == test_j){
				//	cout << endl << "Base Color of U : " << endl;
				//	for (int x = 0; x < 4; x++)
				//		cout << U_basecolor[x] << endl;
				////	waitKey(0);
				//}
				//-----------------------------------------U標記---------------------------------------------
				low1 = U_basecolor[0] - bimrange;
				high1 = U_basecolor[0] + bimrange;
				low2 = U_basecolor[1] - bimrange;
				high2 = U_basecolor[1] + bimrange;
				low3 = U_basecolor[2] - bimrange;
				high3 = U_basecolor[2] + bimrange;
				low4 = U_basecolor[3] - bimrange;
				high4 = U_basecolor[3] + bimrange;
				/*if (i == test_i && j == test_j){
					cout << "low 2 : " << low2 << endl;
					cout << "high 2: " << high2 << endl;
				}*/

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low1 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high1)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low2 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high2)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low3 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high3)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;

						else if (GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low4 &&
							GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high4)
							Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
					}
				/*if (i == test_i && j == test_j){
					int count = 0;
					cout << "Set of U : " << endl;
					for (int x = i; x < i + 16; x++){
						for (int y = j; y < j + 16; y++){
							cout << Mark_U.at<double>(x, y) << " ";
						}
						cout << endl;
					}
					cout << endl;
					system("Pause");
				}*/
				//-------------------------------------做BCIM(V)-----------------------------------------
				Mat src_V = Mat::zeros(16, 16, CV_8UC1);
				Mat all_V = Mat::zeros(expand444.rows, expand444.cols, CV_8UC1);
				kk = 0;
				ll = 0;
				for (int k = i; k < i + 16; k++) {
					for (int l = j; l < j + 16; l++) {
						src_V.at<uchar>(kk, ll) = (int)GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						all_V.at<uchar>(mir(k, expand444.rows), mir(l, expand444.cols)) = (int)GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						ll++;
					}
					ll = 0;
					kk++;
				}
				//histogram
				float range_V[] = { 0, 255 };
				const float* histRange_V = { range };
				Mat histImg_V;
				calcHist(&src_V, 1, 0, Mat(), histImg_V, 1, &histSize, &histRange_V);
				//Mat showHistImg_V(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
				//drawHistImg(histImg_V, showHistImg_V);
				//		imshow("all", all);
				//		imshow("window1", src_V);
				//		imshow("window2", showHistImg_V);
				//if (i == test_i && j == test_j)
				//	imwrite("21_window1_V.bmp", src_V);

				double V_basecolor[4] = { 0 };

				//把數量最多的顏色(背景色)剔除
				minMaxLoc(histImg_V, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
				for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
					histImg_V.at<float>(mir(z, 256)) = 0;

				for (int x = 0; x < 4; x++) {
					minMaxLoc(histImg_V, &MinValue, &MaxValue, &MinLocation, &MaxLocation);
					V_basecolor[x] = MaxLocation.y;
					for (int z = MaxLocation.y - bimrange; z <= MaxLocation.y + bimrange; z++)
						histImg_V.at<float>(mir(z, 256)) = 0;
					//cout << "======================================" << endl;
				}

				/*if (i == test_i && j == test_j){
					cout << endl << "Base Color of V : " << endl;
					for (int x = 0; x < 4; x++)
						cout << V_basecolor[x] << endl;
					waitKey(0);
				}*/
				//-----------------------------------------V標記---------------------------------------------

				low1 = V_basecolor[0] - bimrange;
				high1 = V_basecolor[0] + bimrange;
				low2 = V_basecolor[1] - bimrange;
				high2 = V_basecolor[1] + bimrange;
				low3 = V_basecolor[2] - bimrange;
				high3 = V_basecolor[2] + bimrange;
				low4 = V_basecolor[3] - bimrange;
				high4 = V_basecolor[3] + bimrange;
				/*if (i == test_i && j == test_j){
					cout << "low 2 : " << low2 << endl;
					cout << "high 2: " << high2 << endl;
				}*/

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low1 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high1)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low2 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high2)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 1;

						else if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low3 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high3)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;

						else if (GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) >= low4 &&
							GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) <= high4)
							Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = 2;
					}
				/*if (i == test_i && j == test_j){
					int count = 0;
					cout << "Set of V : " << endl;
					for (int x = i; x < i + 16; x++){
						for (int y = j; y < j + 16; y++){
							cout << Mark_V.at<double>(x, y) << " ";
						}
						cout << endl;
					}
					cout << endl;
					system("Pause");
				}*/
				//-------------------------------------做線性回歸(U)-----------------------------------------
				double Y_group_a[1000] = { 0 };
				double Y_group_b[1000] = { 0 };
				double U_group_a[1000] = { 0 };
				double U_group_b[1000] = { 0 };
				double suma_U = 0, sumb_U = 0;
				double a_U_a = 0, b_U_a = 0, bot_a = 0, a_U_b = 0, b_U_b = 0, bot_b = 0;
				double Y_avg_a = 0, U_avg_a = 0, Y_avg_b = 0, U_avg_b = 0;
				int count_a = 0, count_b = 0;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1 && Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Y_group_a[count_a] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							U_group_a[count_a] = GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_a += Y_group_a[count_a];
							U_avg_a += U_group_a[count_a];
							count_a++;
						}
						else if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2 && Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Y_group_b[count_b] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							U_group_b[count_b] = GT_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_b += Y_group_b[count_b];
							U_avg_b += U_group_b[count_b];
							count_b++;
						}
					}
				Y_avg_a /= count_a;
				U_avg_a /= count_a;
				Y_avg_b /= count_b;
				U_avg_b /= count_b;
				/*if (i == test_i && j == test_j){
					cout << "count_a : " << count_a << endl;
					cout << "Y_avg_a : " << Y_avg_a << endl;
					cout << "U_avg_a : " << U_avg_a << endl;
					system("Pause");
				}*/

				for (int z = 0; z < count_a; z++) {
					a_U_a += (Y_group_a[z] - Y_avg_a)*(U_group_a[z] - U_avg_a);
					bot_a += pow(Y_group_a[z] - Y_avg_a, 2);
				}

				a_U_a /= (bot_a + 0.00001);
				b_U_a = U_avg_a - a_U_a * Y_avg_a;

				for (int z = 0; z < count_b; z++) {
					a_U_b += (Y_group_b[z] - Y_avg_b)*(U_group_b[z] - U_avg_b);
					bot_b += pow(Y_group_b[z] - Y_avg_b, 2);
				}

				a_U_b /= (bot_b + 0.00001);
				b_U_b = U_avg_b - a_U_b * Y_avg_b;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) != Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)))
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 0) {
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						}
						else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_U_a * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_U_a;
						}
						else if (Mark_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_U_b * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_U_b;
						}
					}


				//-------------------------------------做線性回歸(V)-----------------------------------------
				double V_group_a[1000] = { 0 };
				double V_group_b[1000] = { 0 };
				sumb_U = 0;
				double suma_V = 0, sumb_V = 0;
				bot_a = 0; bot_b = 0;
				double a_V_a = 0, b_V_a = 0, a_V_b = 0, b_V_b = 0;
				Y_avg_a = 0;
				Y_avg_b = 0;
				double  V_avg_a = 0, V_avg_b = 0;
				count_a = 0;
				count_b = 0;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1 && Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Y_group_a[count_a] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							V_group_a[count_a] = GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_a += Y_group_a[count_a];
							V_avg_a += V_group_a[count_a];
							count_a++;
						}
						else if (Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2 && Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Y_group_b[count_b] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
							V_group_b[count_b] = GT_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));

							Y_avg_b += Y_group_b[count_b];
							V_avg_b += V_group_b[count_b];
							count_b++;
						}
					}


				Y_avg_a /= count_a;
				V_avg_a /= count_a;
				Y_avg_b /= count_b;
				V_avg_b /= count_b;
				/*if (i == test_i && j == test_j){
					cout << "Y_avg_b : " << Y_avg_b << endl;
					cout << "V_avg_b : " << V_avg_b << endl;
					system("Pause");
				}*/

				for (int z = 0; z < count_a; z++) {
					a_V_a += (Y_group_a[z] - Y_avg_a)*(V_group_a[z] - V_avg_a);
					bot_a += pow(Y_group_a[z] - Y_avg_a, 2);
				}

				a_V_a /= (bot_a + 0.00001);
				b_V_a = V_avg_a - a_V_a * Y_avg_a;


				for (int z = 0; z < count_b; z++) {
					a_V_b += (Y_group_b[z] - Y_avg_b)*(V_group_b[z] - V_avg_b);
					bot_b += pow(Y_group_b[z] - Y_avg_b, 2);
				}

				a_V_b /= (bot_b + 0.00001);
				b_V_b = V_avg_b - a_V_b * Y_avg_b;

				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) != Mark_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)))
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 0) {
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						}
						else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 1) {
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_V_a * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_V_a;
						}
						else if (Mark_V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) == 2) {
							Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = a_V_b * expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] + b_V_b;
						}
					}
			}//end Ns>=3
			else {
				for (int k = i; k < i + h; k++)
					for (int l = j; l < j + w; l++) {
						Ufinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
						Vfinal.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = V.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
					}
			}
		}
}

double cal_value(double u, double v, int yy, int xx, Mat& Plane)
{
	double calculate = 0;
	double x_array[] = { -2 + u, -1 + u, u, 1 + u, 2 + u };
	double y_array[] = { 2 + v, 1 + v, v, -1 + v, -2 + v };
	for (int i = 0; i < sizeof(x_array) / sizeof(*x_array); i++)
	{
		for (int j = 0; j < sizeof(y_array) / sizeof(*y_array); j++)
		{
			calculate += BC_funtion(x_array[j]) * BC_funtion(y_array[i]) * Plane.at<double>(yy + i - 2, xx + j - 2);
		}
	}
	return calculate;
}

void bicubic_rewrite2(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, Mat& Ufinal, Mat& Vfinal) {
	Ufinal = Mat::zeros(DS_U_plane.rows * 2, DS_U_plane.cols * 2, CV_64FC1);
	Vfinal = Mat::zeros(DS_V_plane.rows * 2, DS_V_plane.cols * 2, CV_64FC1);
	copyMakeBorder(DS_U_plane, DS_U_plane, 2, 2, 2, 2, BORDER_REFLECT);
	copyMakeBorder(DS_V_plane, DS_V_plane, 2, 2, 2, 2, BORDER_REFLECT);
	int yy = 2;
	for (int y = 0; y < height; y += 2)
	{
		int xx = 2;
		for (int x = 0; x < width; x += 2)
		{
			Ufinal.at<double>(y, x) = cal_value(0.25, -0.25, yy, xx, DS_U_plane);
			Ufinal.at<double>(y, x + 1) = cal_value(-0.25, -0.25, yy, xx, DS_U_plane);
			Ufinal.at<double>(y + 1, x) = cal_value(0.25, 0.25, yy, xx, DS_U_plane);
			Ufinal.at<double>(y + 1, x + 1) = cal_value(-0.25, 0.25, yy, xx, DS_U_plane);

			Vfinal.at<double>(y, x) = cal_value(0.25, -0.25, yy, xx, DS_V_plane);
			Vfinal.at<double>(y, x + 1) = cal_value(-0.25, -0.25, yy, xx, DS_V_plane);
			Vfinal.at<double>(y + 1, x) = cal_value(0.25, 0.25, yy, xx, DS_V_plane);
			Vfinal.at<double>(y + 1, x + 1) = cal_value(-0.25, 0.25, yy, xx, DS_V_plane);
			xx++;

		}
		yy++;
	}

}
