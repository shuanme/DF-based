#include "Subsample.h"

void Convert_Y(int way, int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane ,  double& timestart, double& timeend)
{
	double t_start, t_end;

	double iretime = 0;
	timestart = clock();
	switch (way)
	{
	case 1:		/* Local Optimal */
		Local_Optimal(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane);
		break;
	case 2:		/* Global Optimal 組俊*/
		Global_Optimal(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane);
		break;
	case 3:		/*  Exhausted Search */
		ExhaustedSearch(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane);
		break;
	case 4:
		Global_Optimal_Similar(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane, iretime);
		break;
	case 5:
		Global_Optimal_ForBayer(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane);
		break;
	case 6:
		Global_Optimal_ForDTDI(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane);
		break;
	case 7:		//北科大O
		Global_Optimal_Similar2(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane, iretime);
		break;
	case 8:		//北科大
		Global_Optimal_Similar3(height, width, new_Y_plane, BGR, US_U_plane, US_V_plane, iretime);
		break;
	default: break;
	}
	timeend = clock();

	//cout << "iretime : " << iretime << endl;
}

void lumaModification(int lumaModificationMethod, int curUpsampleMethod, int curDownsampleMethod, int move, Mat& Y, Mat& U, Mat& V, Mat& RGB, double& timestart, double& timeend) {
	cout << "luma modification!" << endl;

	Mat ori420;
	combine420_64F(Y, U, V, ori420);

	Mat exp444;
	expand_64F(ori420, exp444);
	
	//============================== upsample to 444 ==============================
	////upsample後的U V
	Mat dstU;
	Mat dstV;
	//int temp = curUpsampleMethod;
	if (lumaModificationMethod == 2 || lumaModificationMethod == 7 || lumaModificationMethod == 8)
	{
		curUpsampleMethod = UP_COPY;	//主俊
	}

	/*if (lumaModificationMethod == 4 )
	{
		curUpsampleMethod = UP_OPBIC;	
	}*/
	
	if (curUpsampleMethod == UP_ASBLG) { //C.C method ABSLG
		//Regression_Filtering_LS_Adaptive(exp444, curDownsampleMethod, dstU, dstV);
	}
	else if (curUpsampleMethod == UP_COPY /*|| lumaModificationMethod == 2*/ ) {
		//copy(exp444, dstU, dstV);
		resize(U, dstU, Size(0, 0), 2, 2, INTER_NEAREST);
		resize(V, dstV, Size(0, 0), 2, 2, INTER_NEAREST);
	}
	else if (curUpsampleMethod == UP_BIL) {
		bilinear(exp444, dstU, dstV);
		//resize(U, dstU, Size(), 2, 2, INTER_LINEAR);
		//resize(V, dstV, Size(), 2, 2, INTER_LINEAR);
	}
	else if (curUpsampleMethod == UP_BIC) {
		bicubic(exp444, dstU, dstV);
		//resize(U, dstU, Size(), 2, 2, INTER_CUBIC);
		//resize(V, dstV, Size(), 2, 2, INTER_CUBIC);
	}
	else if (curUpsampleMethod == UP_OPBIC) {
		//bicubic(exp444, dstU, dstV);
		resize(U, dstU, Size(), 2, 2, INTER_CUBIC);
		resize(V, dstV, Size(), 2, 2, INTER_CUBIC);
	}
	//else if (curUpsampleMethod == UP_ftap) {
	//	ftap(exp444, dstU, dstV);
	//	//resize(U, dstU, Size(), 2, 2, INTER_CUBIC);
	//	//resize(V, dstV, Size(), 2, 2, INTER_CUBIC);
	//}
	else if (curUpsampleMethod == UP_Lanczos4) {
		//resize(compressed_U, dstU, Size(), 2, 2, INTER_CUBIC);
		//resize(compressed_V, dstV, Size(), 2, 2, INTER_CUBIC);
		//resize(U, dstU, Size(), 2, 2, INTER_LANCZOS4);
		//resize(V, dstV, Size(), 2, 2, INTER_LANCZOS4);
		//LANCZOS4(exp444, dstU, dstV);
		//bicubic(exp444, dstU, dstV);
	}
	//else if (curUpsampleMethod == UP_BIL_16) {
	//	//bilinear_16(U, V, dstU, dstV);
	//	bilinear_16_temp(curDownsampleMethod, exp444, dstU, dstV);
	//	//resize(U, dstU, Size(), 2, 2, INTER_LINEAR);
	//	//resize(V, dstV, Size(), 2, 2, INTER_LINEAR);
	//}
	//else if (curUpsampleMethod == UP_CACR) { //Y.C CACR

	//	Mat bicU, bicV; // bic interpolation
	//	Mat asblgU, asblgV; // ASBLG, i.e. C.C.'s method
	//	Mat bicU_e, bicV_e, asblgU_e, asblgV_e; // error map

	//	// bic interpolation
	//	bicubic_correction(exp444, bicU, bicV, curDownsampleMethod);

	//	// 4.5 groundtruth沒有error ->改用會有error的bc
	//	Mat temp1 = bicU.clone();
	//	Mat temp2 = bicV.clone();

	//	if (curDownsampleMethod != DOWN_A && curDownsampleMethod != DOWN_L && curDownsampleMethod != DOWN_R)
	//		bicubic_all(exp444, temp1, temp2);

	//	// ASBLG
	//	Regression_Filtering_LS_Adaptive(exp444, curDownsampleMethod, asblgU, asblgV);

	//	// create error map
	//	error_map(exp444, temp1, temp2, bicU_e, bicV_e, curDownsampleMethod);//bic
	//	error_map(exp444, asblgU, asblgV, asblgU_e, asblgV_e, curDownsampleMethod);//ASBLG

	//																			// fusion bic and ASBLG
	//	fusion_error(dstU, bicU, asblgU, bicU_e, asblgU_e, 5);//U
	//	fusion_error(dstV, bicV, asblgV, bicV_e, asblgV_e, 5);//V

	//   //Compensation(exp444, dstU, dstV, method, path, video);
	//}
	/*else if (curUpsampleMethod == UP_NEDI) {
		double** ds_U = new double*[exp444.cols / 2];
		double** ds_V = new double*[exp444.cols / 2];
		for (int w = 0; w < exp444.cols / 2; w++) {
			ds_U[w] = new double[exp444.rows / 2];
			ds_V[w] = new double[exp444.rows / 2];
		}

		double** us_U = new double*[exp444.cols];
		double** us_V = new double*[exp444.cols];
		for (int w = 0; w < exp444.cols; w++) {
			us_U[w] = new double[exp444.rows];
			us_V[w] = new double[exp444.rows];
		}
		for (int y = 0; y < exp444.rows / 2; y++)
			for (int x = 0; x < exp444.cols / 2; x++) {
				ds_U[x][y] = exp444.at<Vec3d>(y * 2, x * 2)[1];
				ds_V[x][y] = exp444.at<Vec3d>(y * 2, x * 2)[2];
			}
		NEDI(us_U, ds_U, exp444.cols, exp444.rows, 7);
		NEDI(us_V, ds_V, exp444.cols, exp444.rows, 7);
		dstU.create(exp444.rows, exp444.cols, CV_64F);
		dstV.create(exp444.rows, exp444.cols, CV_64F);
		for (int y = 0; y < exp444.rows; y++)
			for (int x = 0; x < exp444.cols; x++) {
				dstU.at<double>(y, x) = us_U[x][y];
				dstV.at<double>(y, x) = us_V[x][y];
			}

		for (int w = 0; w < exp444.cols / 2; w++) {
			delete[] ds_U[w];
			delete[] ds_V[w];
		}
		delete[] ds_U;
		delete[] ds_V;

		for (int w = 0; w < exp444.cols; w++) {
			delete[] us_U[w];
			delete[] us_V[w];
		}
		delete[] us_U;
		delete[] us_V;
	}*/
	
	////////////////位移
	if (move == true && (curUpsampleMethod == UP_BIL || curUpsampleMethod == UP_BIC || curUpsampleMethod == UP_NEDI)) {
		if (curDownsampleMethod == YL_BIN || curDownsampleMethod == LACS/* || curDownsampleMethod == CB_REG1*/  ) {
			Mat map_x(RGB.rows, RGB.cols, CV_32F);
			Mat map_y(RGB.rows, RGB.cols, CV_32F);

			for (int i = 0; i < RGB.rows; i++)
				for (int j = 0; j < RGB.cols; j++) {
					map_x.at<float>(i, j) = j - 0.5;
					map_y.at<float>(i, j) = i - 0.5;
				}
			remap(dstU, dstU, map_x, map_y, CV_INTER_LINEAR, BORDER_REFLECT);
			remap(dstV, dstV, map_x, map_y, CV_INTER_LINEAR, BORDER_REFLECT);
		}
		else if (curDownsampleMethod == t2)
		{
			Mat map_x(RGB.rows, RGB.cols, CV_32F);
			Mat map_y(RGB.rows, RGB.cols, CV_32F);

			for (int i = 0; i < RGB.rows; i++)
				for (int j = 0; j < RGB.cols; j++) {
					map_x.at<float>(i, j) = j - 0.5;
					map_y.at<float>(i, j) = i - 0.5;
				}
			remap(dstU, dstU, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT);
			remap(dstV, dstV, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT);
		}
		
		else if (curDownsampleMethod == DOWN_A || curDownsampleMethod == DOWN_L || curDownsampleMethod == DOWN_R || curDownsampleMethod == Anchor) {
			Mat map_x(RGB.rows, RGB.cols, CV_32F);
			Mat map_y(RGB.rows, RGB.cols, CV_32F);

			if (curDownsampleMethod == DOWN_A) {

				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j - 0.5;
						map_y.at<float>(i, j) = i - 0.5;
					}
				//Chroma_Shift_420A(US_U_plane, US_V_plane);

			}
			else if (curDownsampleMethod == Anchor) {
				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j;
						map_y.at<float>(i, j) = i - 0.5;
					}
				//Chroma_Shift_420L(US_U_plane, US_V_plane);

			}
			else if (curDownsampleMethod == DOWN_L) {
				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j;
						map_y.at<float>(i, j) = i - 0.5;
					}
				//Chroma_Shift_420L(US_U_plane, US_V_plane);

			}
			else if (curDownsampleMethod == DOWN_R) {
				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j - 1;
						map_y.at<float>(i, j) = i - 0.5;
					}
				//Chroma_Shift_420A(US_U_plane, US_V_plane);
			}
			remap(dstU, dstU, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT); // 把(map_x,map_y)座標點給新的dstU，-是往右或下，+是往左或上
			remap(dstV, dstV, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT);
			cout << "alignment of LM!" << endl;

		}
	}
	else if (move == true && curUpsampleMethod == UP_Lanczos4)
	{

	 if (curDownsampleMethod == t4)
		{
			Mat map_x(RGB.rows, RGB.cols, CV_32F);
			Mat map_y(RGB.rows, RGB.cols, CV_32F);

			for (int i = 0; i < RGB.rows; i++)
				for (int j = 0; j < RGB.cols; j++) {
					map_x.at<float>(i, j) = j - 0.5;
					map_y.at<float>(i, j) = i - 0.5;
				}
			remap(dstU, dstU, map_x, map_y, CV_INTER_LANCZOS4, BORDER_REFLECT);
			remap(dstV, dstV, map_x, map_y, CV_INTER_LANCZOS4, BORDER_REFLECT);
		}
	 else if (curDownsampleMethod == DOWN_A || curDownsampleMethod == DOWN_L || curDownsampleMethod == DOWN_R) {
		 Mat map_x(RGB.rows, RGB.cols, CV_32F);
		 Mat map_y(RGB.rows, RGB.cols, CV_32F);

		 if (curDownsampleMethod == DOWN_A) {

			 for (int i = 0; i < RGB.rows; i++)
				 for (int j = 0; j < RGB.cols; j++) {
					 map_x.at<float>(i, j) = j - 0.5;
					 map_y.at<float>(i, j) = i - 0.5;
				 }
			 //Chroma_Shift_420A(US_U_plane, US_V_plane);

		 }
		 else if (curDownsampleMethod == DOWN_L) {
			 for (int i = 0; i < RGB.rows; i++)
				 for (int j = 0; j < RGB.cols; j++) {
					 map_x.at<float>(i, j) = j;
					 map_y.at<float>(i, j) = i - 0.5;
				 }
			 //Chroma_Shift_420L(US_U_plane, US_V_plane);

		 }
		 else if (curDownsampleMethod == DOWN_R) {
			 for (int i = 0; i < RGB.rows; i++)
				 for (int j = 0; j < RGB.cols; j++) {
					 map_x.at<float>(i, j) = j - 1;
					 map_y.at<float>(i, j) = i - 0.5;
				 }
			 //Chroma_Shift_420A(US_U_plane, US_V_plane);
		 }
		 remap(dstU, dstU, map_x, map_y, CV_INTER_LANCZOS4, BORDER_REFLECT); // 把(map_x,map_y)座標點給新的dstU，-是往右或下，+是往左或上
		 remap(dstV, dstV, map_x, map_y, CV_INTER_LANCZOS4, BORDER_REFLECT);
		 cout << "alignment of LM!" << endl;
	 }
	}
	else if (move == true && ( curUpsampleMethod == UP_OPBIC )) {
		 if ( curDownsampleMethod == DOWN_L || curDownsampleMethod == DOWN_R || curDownsampleMethod == Anchor || curDownsampleMethod == DOWN_D) {
			Mat map_x(RGB.rows, RGB.cols, CV_32F);
			Mat map_y(RGB.rows, RGB.cols, CV_32F);
			 if (curDownsampleMethod == Anchor) {
				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j + 0.5 ;
						map_y.at<float>(i, j) = i ;
					}
				//Chroma_Shift_420L(US_U_plane, US_V_plane);

			}
			else if (curDownsampleMethod == DOWN_L) {
				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j + 0.5;
						map_y.at<float>(i, j) = i ;
					}
				//Chroma_Shift_420L(US_U_plane, US_V_plane);

			}
			else if (curDownsampleMethod == DOWN_R) {
				for (int i = 0; i < RGB.rows; i++)
					for (int j = 0; j < RGB.cols; j++) {
						map_x.at<float>(i, j) = j - 0.5;
						map_y.at<float>(i, j) = i ;
					}
				//Chroma_Shift_420A(US_U_plane, US_V_plane);
			}
			else if (curDownsampleMethod == DOWN_D) {
				 for (int i = 0; i < RGB.rows; i++)
					 for (int j = 0; j < RGB.cols; j++) {
						 map_x.at<float>(i, j) = j + 0.5;
						 map_y.at<float>(i, j) = i + 0.5;
					 }
				 //Chroma_Shift_420A(US_U_plane, US_V_plane);
			 }
			remap(dstU, dstU, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT); // 把(map_x,map_y)座標點給新的dstU，-是往右或下，+是往左或上
			remap(dstV, dstV, map_x, map_y, CV_INTER_CUBIC, BORDER_REFLECT);
			cout << "alignment of LM!" << endl;

		}
	}
	
	Convert_Y(lumaModificationMethod, RGB.rows, RGB.cols, Y, RGB, dstU, dstV, timestart, timeend);
	//curUpsampleMethod = temp;
}

/* Distortion �G�����u�̧C�I */
void Local_Optimal(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			double U = US_U_plane.at<double>(y, x);
			double V = US_V_plane.at<double>(y, x);

			double C1 = R - 1.596 * (V - 128);
			double C2 = G + 0.391 * (U - 128) + 0.813 * (V - 128);
			double C3 = B - 2.018 * (U - 128);
			double Yopt = ((C1 + C2 + C3) / (3 * 1.164)) + 16;
			//double Yopt = (R + G + B) / 3.492 + 104.339 - 0.466*U - 0.224*V;

			if (Yopt < 0)
				Yopt = 0;
			else if (Yopt > 255)
				Yopt = 255;

			new_Y_plane.at<double>(y, x) = (double)round(Yopt);
			//cout << (int)new_Y_plane.at<double>(y, x) << " " << (int)(double)round(Yopt) << endl;
		}
	}
}

/* min(Yr,Yg,Yb) <= Y_best <= max(Yr,Yg,Yb) �b�϶����M��̨Ϊ�Y */
void Global_Optimal(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane) {
	int count = 0;
	int p = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			double U = US_U_plane.at<double>(y, x);
			double V = US_V_plane.at<double>(y, x);

			//U = (int)round(U);
			//V = (int)round(V);

			double Yr = (R - 1.596 * (V - 128)) / 1.164 + 16;
			double Yg = (G + 0.391 * (U - 128) + 0.813 * (V - 128)) / 1.164 + 16;
			double Yb = (B - 2.018 * (U - 128)) / 1.164 + 16;

			if (Yr < 0) Yr = 0;
			else if (Yr > 255) Yr = 255;

			if (Yg < 0) Yg = 0;
			else if (Yg > 255) Yg = 255;

			if (Yb < 0) Yb = 0;
			else if (Yb > 255) Yb = 255;


			double minVal = min(Yr, Yg);
			minVal = min(minVal, Yb);
			double maxVal = max(Yr, Yg);
			maxVal = max(maxVal, Yb);

			/*cout << "Before: " << new_Y_plane.at<double>(y, x) << " Yr: " << U << " Yg: " << V << " Yb: " << B << " ";*/

			//int start = (int)floor(minVal) ;
			//int end = (int)ceil(maxVal) ;

			int start = (int)floor(minVal)  ;
			int end = (int)ceil(maxVal) ;
			if (end < 0)
				cout << "end < 0" << endl;
			if (start > 255)
				cout << " start > 255" << endl;
			double error_TH = DBL_MAX;
			/*double c_1 = 1.596 *V - 222.912,
				c_2 = 0.391 * U + 0.813 * V - 135.488,
				c_4 = 2.018 * U - 276.928;*/
			//cout << end - start << " : " << end << endl;
			for (int move_Y = start; move_Y <= end; move_Y++)
			{  
				int rec_R = (int)round(1.164 * (move_Y - 16) + 1.596 * (V - 128));
				int rec_G = (int)round(1.164 * (move_Y - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
				int rec_B = (int)round(1.164 * (move_Y - 16) + 2.018 * (U - 128));


				count++;
				/*int rec_R = (int)round(1.164 * (move_Y)+c_1);
				int rec_G = (int)round(1.164 * (move_Y)-c_2);
				int rec_B = (int)round(1.164 * (move_Y)+c_4);*/

				if (rec_R < 0) rec_R = 0;
				else if (rec_R > 255) rec_R = 255;

				if (rec_G < 0) rec_G = 0;
				else if (rec_G > 255) rec_G = 255;

				if (rec_B < 0) rec_B = 0;
				else if (rec_B > 255) rec_B = 255;

				double error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�

				/* Weight 1 */
				//double error = 0.3 * (R - rec_R)*(R - rec_R) + 0.6 * (G - rec_G)*(G - rec_G) + 0.1 * (B - rec_B)*(B - rec_B);
				/* Weight 2 */
				//double error = 0.25 * (R - rec_R)*(R - rec_R) + 0.5 * (G - rec_G)*(G - rec_G) + 0.25 * (B - rec_B)*(B - rec_B);

				/* Weight 3 */
				//double error = 0.4 * (R - rec_R)*(R - rec_R) + 0.4 * (G - rec_G)*(G - rec_G) + 0.2 * (B - rec_B)*(B - rec_B);
				/* Weight 4 */
				//double error = 0.4 * (R - rec_R)*(R - rec_R) + 0.5 * (G - rec_G)*(G - rec_G) + 0.1 * (B - rec_B)*(B - rec_B);

				if (error < error_TH)
				{
					//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

					error_TH = error;
					new_Y_plane.at<double>(y, x) = (double)move_Y;
					//cout << (double)move_Y << endl;

				}

				/*if (move_Y == 107) cout << move_Y << ", " << error << endl;
				if (move_Y == 113) cout << move_Y << ", " << error << endl;*/
			}

			if (new_Y_plane.at<double>(y, x) == (int)floor(minVal) + 1 || new_Y_plane.at<double>(y, x) == (int)ceil(maxVal) - 1)
				p++;

			/*if (y == 0 && x == 0)
			{
				int rec_R = (int)round(1.164 * (new_Y_plane.at<double>(y, x) - 16) + 1.596 * (V - 128));
				int rec_G = (int)round(1.164 * (new_Y_plane.at<double>(y, x) - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
				int rec_B = (int)round(1.164 * (new_Y_plane.at<double>(y, x) - 16) + 2.018 * (U - 128));

				if (rec_R < 0) rec_R = 0;
				else if (rec_R > 255) rec_R = 255;

				if (rec_G < 0) rec_G = 0;
				else if (rec_G > 255) rec_G = 255;

				if (rec_B < 0) rec_B = 0;
				else if (rec_B > 255) rec_B = 255;

				int error = (R - rec_R)*(R - rec_R) + (G - rec_G)*(G - rec_G) + (B - rec_B)*(B - rec_B); // �קKcall function �`�ٮɶ�

				cout << start << "   " << end << endl;
				cout << (int)new_Y_plane.at<double>(y, x) << endl;
				cout << rec_R << ", " << rec_G << ", " << rec_B << endl; system("pause");
			}*/
			//cout << "After: " << new_Y_plane.at<double>(y, x) << endl;
		}
	}
	//cout << "1 : " << new_Y_plane.at<double>(39, 21) << endl;
	//cout << "count : " << count / height / width << endl;
	//cout << "this is problem " << p << endl;
}



void Global_Optimal_Similar(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane, double & iretime)
{
	/*Mat A = (Mat_<double>(3, 1) <<
		1.164,
		1.164,
		1.164);
	Mat At = A.t();
	Mat invA = (Mat_<double>(1, 1) <<
		0.2460213);
	Mat T = invA * At;*/
	double c = 0;
	int f = 0;
	int a = 1;
	int R, G, B;
	double U, V;
	// double c_1 = 0, c_2 = 0, c_4 = 0;
	double c_1, c_2, c_3, c_4;
	//int est_R, est_G, est_B;
	
	double error;
	int y,x,i;
	int count = 0;


	for ( y = 0; y < height; y++) {
		for ( x = 0; x < width; x++) {
			//iretime++;
			int error_TH = INT_MAX;
			 int R = (int)BGR.at<Vec3b>(y, x)[2];
			 int G = (int)BGR.at<Vec3b>(y, x)[1];
			 int B = (int)BGR.at<Vec3b>(y, x)[0];

			 U = US_U_plane.at<double>(y, x);
			 V = US_V_plane.at<double>(y, x);

			 //U = (int)round(U);
			 //V = (int)round(V);

			


			 c_1 = 1.596 *V - 222.912;
			 c_2 = 0.391 * U + 0.813 * V - 135.488;
			 c_4 = 2.018 * U - 276.928;
			/*Mat result = (Mat_<double>(3, 1) <<
				(R - 1.596 * V + 222.912),
				(G + 0.391 * U + 0.813 * V - 135.488),
				(B - 2.018 * U + 276.928));

			Mat C = T * result;*/

			/*c = 0.2863687932*(R - 1.596 * V + 222.912)
				+ 0.2863687932*(G + 0.391 * U + 0.813 * V - 135.488)
				+ 0.2863687932*(B - 2.018 * U + 276.928);*/

			c = 0.2863687932*R - 0.22422676507*V + 104.33904254
				+ 0.2863687932*G - 0.46592202653*U  
				+ 0.2863687932*B;

			//c = ((R + G + B) + (-c_1 + c_2 - c_3))/3/1.164;

			//new_Y_plane.at<double>(y, x) = C.at<double>(0,0);
			//new_Y_plane.at<double>(y, x) = c;

			//int choice_1 = (int)floor(C.at<double>(0, 0));
			//int choice_2 = (int)ceil(C.at<double>(0, 0));
			int choice_1 = (int)floor(c);
			int choice_2 = (int)ceil(c);
			//int choice_1 = (int)c;
			//int choice_2 = (int)c + 1;
			//double error_TH = DBL_MAX;
			
			/*if (choice_1 > 0)
				choice_1 -= 1;
			if (choice_2 < 255)
				choice_2 += 1;*/
			int t;
			int e1 = 0;
			for ( i = choice_1; i <= choice_2; i++) {
				int est_R = (int)round(1.164 * (i) + c_1);
				int est_G = (int)round(1.164 * (i) - c_2 );
				int est_B = (int)round(1.164 * (i) + c_4);
				count++;
				//int est_R = (int)(1.164 * (i - 16) + 1.596 * (V - 128));
				//int est_R = (int)(1.164 * i + 1.596 *V - 222.912);
				//int est_G = (int)(1.164 * (i - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
				//int est_G = (int)(1.164 * i - 0.391 * U - 0.813 * V + 135.488);
				//int est_B = (int)(1.164 * (i - 16) + 2.018 * (U - 128));
				//int est_B = (int)(1.164 * i + 2.018 * U - 276.928);

				if (est_R < 0) est_R = 0;
				else if (est_R > 255) est_R = 255;

				if (est_G < 0) est_G = 0;
				else if (est_G > 255) est_G = 255;

				if (est_B < 0) est_B = 0;
				else if (est_B > 255) est_B = 255;

				int tr = (R - est_R);
				int tg = (G - est_G);
				int tb = (B - est_B);
				//error = (R - est_R) * (R - est_R) + (G - est_G) * (G - est_G) + (B - est_B) * (B - est_B);
				error = tr * tr + tb * tb + tg * tg;
				//error = pow((R - est_R),2) + pow((G - est_G),2)  + pow((B - est_B),2) ;
				//error = (R - (1.164 * (i)+c_1)) * (R - (1.164 * (i)+c_1)) + (G - (1.164 * (i)-c_2)) * (G - (1.164 * (i)-c_2)) + (B - (1.164 * (i)+c_4)) * (B - (1.164 * (i)+c_4));
				 e1 = error;
				if (error < error_TH) {
					error_TH = error;
					new_Y_plane.at<double>(y, x) = (double)i;
					t = i;
				}
			}
			//--------------------------       迭代       -----------------------------------------------------------------
			if (choice_1 == t)	//-
			{
				
				 f = t;
				 a = 1;
				for (a; a > 0; a)
				{
					iretime++;
					f--;
					int est_R = (int)round(1.164 * f + c_1);
					int est_G = (int)round(1.164 * f - c_2 );
					int est_B = (int)round(1.164 * f + c_4);
					count++;

					if (est_R < 0) est_R = 0;
					else if (est_R > 255) est_R = 255;

					if (est_G < 0) est_G = 0;
					else if (est_G > 255) est_G = 255;

					if (est_B < 0) est_B = 0;
					else if (est_B > 255) est_B = 255;

					 error = (R - est_R) * (R - est_R) + (G - est_G) * (G - est_G) + (B - est_B) * (B - est_B);
					if (error < error_TH) {
						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)f;
					}
					else
					{
						a = 0;
						
					}
				}
			}
			else  //+
			{
				f = t;
				//int num = 0;

				//int a = 2;
				a = 1;
				for (a; a > 0; a)
				{
					iretime++;
					f++;
					int est_R = (int)round(1.164 * f + c_1);
					int est_G = (int)round(1.164 * f - c_2 );
					int est_B = (int)round(1.164 * f + c_4);
					count++;

					if (est_R < 0) est_R = 0;
					else if (est_R > 255) est_R = 255;

					if (est_G < 0) est_G = 0;
					else if (est_G > 255) est_G = 255;

					if (est_B < 0) est_B = 0;
					else if (est_B > 255) est_B = 255;

					 error = (R - est_R) * (R - est_R) + (G - est_G) * (G - est_G) + (B - est_B) * (B - est_B);
					if (error < error_TH) {
						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)f;
						//a = 2;
						//a = 1;
					}
					else
					{
						a = 0;			
					}
				}
			}
			

			
		}
	}
	//iretime = iretime/height / width;
	//cout << "count : "<< count / height / width << endl;
}

void Global_Optimal_Similar2(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane, double& iretime)
{

	float c = 0;
	int f = 0;
	int a = 1;
	int R, G, B;
	double U, V;

	double c_1, c_2, c_3, c_4;

	int error;
	int y, x, i;
	int pd1, pd2, pd3, pd4, pd5, pd6, pd7 = 0;
	double interval[8] = {0};

	
	double count = 0, count2 = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			iretime++;
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			 U = US_U_plane.at<double>(y, x);
			 V = US_V_plane.at<double>(y, x);

			double Yr = (R - 1.596 * (V - 128)) / 1.164 + 16;
			double Yg = (G + 0.391 * (U - 128) + 0.813 * (V - 128)) / 1.164 + 16;
			double Yb = (B - 2.018 * (U - 128)) / 1.164 + 16;

			if (Yr < 0) Yr = 0;
			else if (Yr > 255) Yr = 255;

			if (Yg < 0) Yg = 0;
			else if (Yg > 255) Yg = 255;

			if (Yb < 0) Yb = 0;
			else if (Yb > 255) Yb = 255;


			double minVal = min(Yr, Yg);
			minVal = min(minVal, Yb);
			double maxVal = max(Yr, Yg);
			maxVal = max(maxVal, Yb);
			int error_TH = INT_MAX;
			double sr1 = minVal, sr2 = maxVal;
			int check = 1;
			int check1 = 1;

			 //[a b c d e] = [1.164		1.596		-0.391		-0.813		2.018] 
			double aa = 1.164, bb = 1.596, cc = -0.391, dd = -0.813, ee = 2.018;
			
			

			double yrl = (bb*(V - 128 )/aa) + 16;
			double yru = ((255 + bb*(V - 128 ))/aa) + 16;
			double ygl = (cc*(128-U) - dd*(128 - V)) / aa + 16;
			double ygu = (255 + cc*(128-U) + dd*(128 - V))/aa + 16;
			double ybl = ee*(128 - U) /aa + 16;
			double ybu = (255 + ee*(128 - U))/aa + 16;
			interval[0] = sr1;
			interval[1] = sr2;
			interval[2] = yrl;
			interval[3] = yru;
			interval[4] = ygl;
			interval[5] = ygu;
			interval[6] = ybl;
			interval[7] = ybu;
			//if (sr1 == sr2)
				//cout << "sr1 = sr2" << endl;

			//sort interval
			for (int i = 1; i < 8; i++)
			{
				double min = interval[i - 1];
				int mini = i - 1;
				double temp = 0;
				for (int j = i; j < 8; j++)
				{
					if (interval[mini] > interval[j])
					{
						temp = interval[mini];
						interval[mini] = interval[j];
						interval[j] = temp;

					}
				}

			}
			//cout << "iinterval : " << interval[0] << " , " << interval[1] << " , " << interval[2] << " , " << interval[3] << " , " << interval[4] << " , " << interval[5] << " , " << interval[6] << " , " << interval[7]  << endl;
			
			
			int mystar = 0, myend = 0;
			for (int i = 0; i < 8; i++)
			{
				if (interval[i] < sr1)
					mystar++;
				if (interval[i] < sr2)
					myend++;
			}
			//if (sr1 == sr2)
			//cout << "123 : " << mystar << " : " << myend << endl;
			
			int a = 0;
			for (int a1 = 31; a1 < 31; a1++)
				cout << a1;

			if (mystar == myend)
				count++;
				//cout << mystar << " : " << myend << endl;
			//count++;
			count2 = count2 + myend - mystar;
			//cout << myend - mystar << endl;
			//cout << sr1 << ":2:2:" << sr2 << endl;
			 //cout << interval[myend] << ":::" <<interval[mystar] << endl;
			for (int i = mystar; i < myend; i++)
			{
				a++;
				int id = 0;
				double ty = 0;
				//if(mystar == myend)
				//cout << mystar << " : " << myend << endl;
				//count++;
				if ((interval[i] + interval[i + 1]) / 2 <= yru && (interval[i] + interval[i + 1]) / 2 >= yrl)
				{
					id = id + 1;
				}
				if ((interval[i] + interval[i + 1]) / 2 <= ygu && (interval[i] + interval[i + 1]) / 2 >= ygl)
				{
					id = id + 10;
				}
				if ((interval[i] + interval[i + 1]) / 2 <= ybu && (interval[i] + interval[i + 1]) / 2 >= ybl)
				{
					id = id + 100;
				}

				if (id == 1)
				{
					ty = Yr;
					count++;
				}
				else if (id == 10)
				{
					ty = Yg;
					count++;
				}
				else if (id == 11)
				{
					ty = (Yr + Yg) / 2;
					count++;
				}
				else if (id == 100)
				{
					ty = Yb;
					count++;
				}
				else if (id == 101)
				{
					ty = (Yr + Yb) / 2;
					count++;
				}
				else if (id == 110)
				{
					ty = (Yg + Yb) / 2;
					count++;
				}
				else if (id == 111)
				{
					ty = (Yr + Yg + Yb) / 3;
					count++;
				}
				else
				{
					//cout << (interval[i] + interval[i + 1]) / 2 << endl;
					//cout << yul
					//cout << mystar << ": 123 :" << myend << endl;
					//cout << "id : " << id << endl;
					//ty = interval[mystar];
					ty = (Yr + Yg + Yb) / 3;
					count++;
				}

				//ty = (int)ty;
				//ty = (int)round(ty);
				
					int tyyy = (int)round(ty);




					int rec_R = (int)round(1.164 * (ty - 16) + 1.596 * (V - 128));
					int rec_G = (int)round(1.164 * (ty - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
					int rec_B = (int)round(1.164 * (ty - 16) + 2.018 * (U - 128));
					/*if (ty == 0)
						cout << "ty == 0" << endl;*/

				if (check1 == 1)
				{

					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;

					double error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�


					if (error < error_TH)
					{
						//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)ty;
						//cout << tyyy << endl;
						//cout << new_Y_plane.at<double>(y, x) << endl;
						//count2++;
					}
				}


				//-----------------------------------------
				if(check == 1){
				int tyy = (int)floor(ty);
				rec_R = (int)round(1.164 * (tyy - 16) + 1.596 * (V - 128));
				rec_G = (int)round(1.164 * (tyy - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
				rec_B = (int)round(1.164 * (tyy - 16) + 2.018 * (U - 128));


				
				if (rec_R < 0) rec_R = 0;
				else if (rec_R > 255) rec_R = 255;

				if (rec_G < 0) rec_G = 0;
				else if (rec_G > 255) rec_G = 255;

				if (rec_B < 0) rec_B = 0;
				else if (rec_B > 255) rec_B = 255;

				if ((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)) {
					error = (G - rec_G) * (G - rec_G);
				}
				else if (y % 2 == 0 && x % 2 == 1){
					error = (R - rec_R) * (R - rec_R);
				}
				else {
					error = (B - rec_B) * (B - rec_B);
				}

				//error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�


				if (error < error_TH)
				{
					//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

					error_TH = error;
					new_Y_plane.at<double>(y, x) = (double)tyy;
					//count2++;
				}

				//-----------------------------------------
				tyy = (int)ceil(ty);
				rec_R = (int)round(1.164 * (tyy - 16) + 1.596 * (V - 128));
				rec_G = (int)round(1.164 * (tyy - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
				rec_B = (int)round(1.164 * (tyy - 16) + 2.018 * (U - 128));



				if (rec_R < 0) rec_R = 0;
				else if (rec_R > 255) rec_R = 255;

				if (rec_G < 0) rec_G = 0;
				else if (rec_G > 255) rec_G = 255;

				if (rec_B < 0) rec_B = 0;
				else if (rec_B > 255) rec_B = 255;


				if ((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)) {
					error = (G - rec_G) * (G - rec_G);
				}
				else if (y % 2 == 0 && x % 2 == 1) {
					error = (R - rec_R) * (R - rec_R);
				}
				else {
					error = (B - rec_B) * (B - rec_B);
				}
				//error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�


				if (error < error_TH)
				{
					//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

					error_TH = error;
					new_Y_plane.at<double>(y, x) = (double)tyy;
					//count2++;
				}
			}
				
			}

			
			if (a > 0)
				count2++;


			

		

		}
	}
	iretime = iretime / height / width;
	cout << "count : " << count / height / width << endl;
	cout << "count2 : " << count2 / height / width << endl;
}

void Global_Optimal_Similar3(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane, double& iretime)
{

	float c = 0;
	int f = 0;
	int a = 1;
	int R, G, B;
	double U, V;

	double c_1, c_2, c_3, c_4;

	int error;
	int y, x, i;
	int pd1, pd2, pd3, pd4, pd5, pd6, pd7 = 0;
	double interval[8] = { 0 };


	double count = 0, count2 = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			iretime++;
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			U = US_U_plane.at<double>(y, x);
			V = US_V_plane.at<double>(y, x);

			//U = (int)round(U);
			//V = (int)round(V);

			double Yr = (R - 1.596 * (V - 128)) / 1.164 + 16;
			double Yg = (G + 0.391 * (U - 128) + 0.813 * (V - 128)) / 1.164 + 16;
			double Yb = (B - 2.018 * (U - 128)) / 1.164 + 16;

			if (Yr < 0) Yr = 0;
			else if (Yr > 255) Yr = 255;

			if (Yg < 0) Yg = 0;
			else if (Yg > 255) Yg = 255;

			if (Yb < 0) Yb = 0;
			else if (Yb > 255) Yb = 255;


			double minVal = min(Yr, Yg);
			minVal = min(minVal, Yb);
			double maxVal = max(Yr, Yg);
			maxVal = max(maxVal, Yb);
			int error_TH = INT_MAX;
			double sr1 = minVal, sr2 = maxVal;

			//[a b c d e] = [1.164		1.596		-0.391		-0.813		2.018] 
			double aa = 1.164, bb = 1.596, cc = -0.391, dd = -0.813, ee = 2.018;



			double yrl = (bb * (V - 128) / aa) + 16;
			double yru = ((255 + bb * (V - 128)) / aa) + 16;
			double ygl = (cc * (128 - U) - dd * (128 - V)) / aa + 16;
			double ygu = (255 + cc * (128 - U) + dd * (128 - V)) / aa + 16;
			double ybl = ee * (128 - U) / aa + 16;
			double ybu = (255 + ee * (128 - U)) / aa + 16;
			interval[0] = sr1;
			interval[1] = sr2;
			interval[2] = yrl;
			interval[3] = yru;
			interval[4] = ygl;
			interval[5] = ygu;
			interval[6] = ybl;
			interval[7] = ybu;
			//if (sr1 == sr2)
				//cout << "sr1 = sr2" << endl;

			//sort interval
			for (int i = 1; i < 8; i++)
			{
				double min = interval[i - 1];
				int mini = i - 1;
				double temp = 0;
				for (int j = i; j < 8; j++)
				{
					if (interval[mini] > interval[j])
					{
						temp = interval[mini];
						interval[mini] = interval[j];
						interval[j] = temp;

					}
				}

			}
			//cout << "iinterval : " << interval[0] << " , " << interval[1] << " , " << interval[2] << " , " << interval[3] << " , " << interval[4] << " , " << interval[5] << " , " << interval[6] << " , " << interval[7]  << endl;


			int mystar = 0, myend = 0;
			for (int i = 0; i < 8; i++)
			{
				if (interval[i] < sr1)
					mystar++;
				if (interval[i] < sr2)
					myend++;
			}
			//if (sr1 == sr2)
			//cout << "123 : " << mystar << " : " << myend << endl;

			int a = 0;

			
				a++;
				int id = 0;
				double ty = 0;
				//if(mystar == myend)
				//cout << mystar << " : " << myend << endl;
				count++;
				
				for (id = 0; id < 7; id++)
				{
					if (id == 0)
					{
						ty = Yr;
						count++;
					}
					else if (id == 1)
					{
						ty = Yg;
						count++;
					}
					else if (id == 2)
					{
						ty = (Yr + Yg) / 2;
						count++;
					}
					else if (id == 3)
					{
						ty = Yb;
						count++;
					}
					else if (id == 4)
					{
						ty = (Yr + Yb) / 2;
						count++;
					}
					else if (id == 5)
					{
						ty = (Yg + Yb) / 2;
						count++;
					}
					else if (id == 6)
					{
						ty = (Yr + Yg + Yb) / 3;
						count++;
					}
					else
					{
						//cout << (interval[i] + interval[i + 1]) / 2 << endl;
						//cout << yul
						//cout << mystar << ": 123 :" << myend << endl;
						//cout << "id : " << id << endl;
						//ty = interval[mystar];
						ty = (Yr + Yg + Yb) / 3;
						count++;
					}

					int tyyy = (int)round(ty);




					int rec_R = (int)round(1.164 * (tyyy - 16) + 1.596 * (V - 128));
					int rec_G = (int)round(1.164 * (tyyy - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
					int rec_B = (int)round(1.164 * (tyyy - 16) + 2.018 * (U - 128));
					/*if (ty == 0)
						cout << "ty == 0" << endl;*/



					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;

					double error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�


					if (error < error_TH)
					{
						//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)ty;
						//cout << tyyy << endl;
						//cout << new_Y_plane.at<double>(y, x) << endl;

					}


					//-----------------------------------------
					int tyy = (int)floor(ty);
					rec_R = (int)round(1.164 * (tyy - 16) + 1.596 * (V - 128));
					rec_G = (int)round(1.164 * (tyy - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
					rec_B = (int)round(1.164 * (tyy - 16) + 2.018 * (U - 128));



					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;

					error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�


					if (error < error_TH)
					{
						//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)tyy;

					}

					//-----------------------------------------
					tyy = (int)ceil(ty);
					rec_R = (int)round(1.164 * (tyy - 16) + 1.596 * (V - 128));
					rec_G = (int)round(1.164 * (tyy - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
					rec_B = (int)round(1.164 * (tyy - 16) + 2.018 * (U - 128));



					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;

					error = (R - rec_R) * (R - rec_R) + (G - rec_G) * (G - rec_G) + (B - rec_B) * (B - rec_B); // �קKcall function �`�ٮɶ�


					if (error < error_TH)
					{
						//cout << "@(" << y << ", " << x << ")" << " original Y: " << new_Y_plane.at<double>(y, x) << " new Y: " << (double)move_Y << endl;

						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)tyy;

					}


				}

				
				

				//ty = (int)ty;
				//ty = (int)round(ty);

				


			if (a > 0)
				count2++;






		}
	}
	iretime = iretime / height / width;
	//cout << "count : " << count / height / width << endl;
	cout << "count2 : " << count2 / height / width << endl;
}

void Global_Optimal_ForBayer(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane)
{
	int minVal;
	int maxVal;
	int f = 0;
	int a = 1;
	int count = 0;
	double iretime;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			double U = US_U_plane.at<double>(y, x);
			double V = US_V_plane.at<double>(y, x);

			// G position
			if ((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1))
			{
				double Yg = (G + 0.391 * (U - 128) + 0.813 * (V - 128)) / 1.164 + 16;
				if (Yg < 0) Yg = 0;
				else if (Yg > 255) Yg = 255;

				minVal = (int)floor(Yg);
				maxVal = (int)ceil(Yg);
				//minVal = 0;
				//maxVal = 255;
				int t;
				int error_TH = INT_MAX;
				for (int move_Y = minVal; move_Y <= maxVal; move_Y++)
				{
					int rec_G = (int)round(1.164 * (move_Y - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					int error = (G - rec_G)*(G - rec_G);
					if (error < error_TH)
					{
						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)move_Y;
						t = move_Y;
					}
				}

				//if (minVal == t)	//-
				//{

				//	f = t;
				//	a = 1;
				//	for (a; a > 0; a)
				//	{
				//		iretime++;
				//		f--;
				//		count++;

				//		int rec_G = (int)round(1.164 * (f - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));

				//		if (rec_G < 0) rec_G = 0;
				//		else if (rec_G > 255) rec_G = 255;

				//		int error = (G - rec_G) * (G - rec_G);
				//		if (error < error_TH)
				//		{
				//			error_TH = error;
				//			new_Y_plane.at<double>(y, x) = (double)f;
				//		}
				//		else
				//		{
				//			a = 0;

				//		}
				//	}
				//}
				//else  //+
				//{
				//	f = t;
				//	//int num = 0;

				//	//int a = 2;
				//	a = 1;
				//	for (a; a > 0; a)
				//	{
				//		iretime++;
				//		f++;
				//		count++;

				//		int rec_G = (int)round(1.164 * (f - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));

				//		if (rec_G < 0) rec_G = 0;
				//		else if (rec_G > 255) rec_G = 255;

				//		int error = (G - rec_G) * (G - rec_G);
				//		if (error < error_TH)
				//		{
				//			error_TH = error;
				//			new_Y_plane.at<double>(y, x) = (double)f;
				//		}
				//		else
				//		{
				//			a = 0;
				//		}
				//	}
				//}

			}
			// R position
			else if (y % 2 == 0 && x % 2 == 1)
			{
				double Yr = (R - 1.596 * (V - 128)) / 1.164 + 16;
				if (Yr < 0) Yr = 0;
				else if (Yr > 255) Yr = 255;

				minVal = (int)floor(Yr);
				maxVal = (int)ceil(Yr);
				//minVal = 0;
				//maxVal = 255;
				int t;
				int error_TH = INT_MAX;
				for (int move_Y = minVal; move_Y <= maxVal; move_Y++)
				{
					int rec_R = (int)round(1.164 * (move_Y - 16) + 1.596 * (V - 128));

					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					int error = (R - rec_R)*(R - rec_R);
					if (error < error_TH)
					{
						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)move_Y;
						t = move_Y;
					}
				}

				//if (minVal == t)	//-
				//{

				//	f = t;
				//	a = 1;
				//	for (a; a > 0; a)
				//	{
				//		iretime++;
				//		f--;
				//		count++;

				//		int rec_R = (int)round(1.164 * (f - 16) + 1.596 * (V - 128));

				//		if (rec_R < 0) rec_R = 0;
				//		else if (rec_R > 255) rec_R = 255;

				//		int error = (R - rec_R) * (R - rec_R);
				//		if (error < error_TH)
				//		{
				//			error_TH = error;
				//			new_Y_plane.at<double>(y, x) = (double)f;
				//		}
				//		else
				//		{
				//			a = 0;

				//		}
				//	}
				//}
				//else  //+
				//{
				//	f = t;
				//	//int num = 0;

				//	//int a = 2;
				//	a = 1;
				//	for (a; a > 0; a)
				//	{
				//		iretime++;
				//		f++;
				//		count++;

				//		int rec_R = (int)round(1.164 * (f - 16) + 1.596 * (V - 128));

				//		if (rec_R < 0) rec_R = 0;
				//		else if (rec_R > 255) rec_R = 255;

				//		int error = (R - rec_R) * (R - rec_R);
				//		if (error < error_TH)
				//		{
				//			error_TH = error;
				//			new_Y_plane.at<double>(y, x) = (double)f;
				//		}
				//		else
				//		{
				//			a = 0;
				//		}
				//	}
				//}
			}
			// B poistion
			else
			{
				double Yb = (B - 2.018 * (U - 128)) / 1.164 + 16;
				if (Yb < 0) Yb = 0;
				else if (Yb > 255) Yb = 255;

				minVal = (int)floor(Yb);
				maxVal = (int)ceil(Yb);
				//minVal = 0;
				//maxVal = 255;
				int t;
				int error_TH = INT_MAX;
				for (int move_Y = minVal; move_Y <= maxVal; move_Y++)
				{
					int rec_B = (int)round(1.164 * (move_Y - 16) + 2.018 * (U - 128));

					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;

					int error = (B - rec_B)*(B - rec_B);
					if (error < error_TH)
					{
						error_TH = error;
						new_Y_plane.at<double>(y, x) = (double)move_Y;
						t = move_Y;
					}
				}

				//if (minVal == t)	//-
				//{

				//	f = t;
				//	a = 1;
				//	for (a; a > 0; a)
				//	{
				//		iretime++;
				//		f--;
				//		count++;

				//		int rec_B = (int)round(1.164 * (f - 16) + 1.596 * (V - 128));

				//		if (rec_B < 0) rec_B = 0;
				//		else if (rec_B > 255) rec_B = 255;

				//		int error = (B - rec_B) * (B - rec_B);
				//		if (error < error_TH)
				//		{
				//			error_TH = error;
				//			new_Y_plane.at<double>(y, x) = (double)f;
				//		}
				//		else
				//		{
				//			a = 0;

				//		}
				//	}
				//}
				//else  //+
				//{
				//	f = t;
				//	//int num = 0;

				//	//int a = 2;
				//	a = 1;
				//	for (a; a > 0; a)
				//	{
				//		iretime++;
				//		f++;
				//		count++;

				//		int rec_B = (int)round(1.164 * (f - 16) + 1.596 * (V - 128));

				//		if (rec_B < 0) rec_B = 0;
				//		else if (rec_B > 255) rec_B = 255;

				//		int error = (B - rec_B) * (B - rec_B);
				//		if (error < error_TH)
				//		{
				//			error_TH = error;
				//			new_Y_plane.at<double>(y, x) = (double)f;
				//		}
				//		else
				//		{
				//			a = 0;
				//		}
				//	}
				//}
			}
		}
	}
}

void Global_Optimal_ForDTDI(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane)
{
	// GB GR
	// GB GR

	int minVal, maxVal;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			double U = US_U_plane.at<double>(y, x);
			double V = US_V_plane.at<double>(y, x);

			double Yg = (G + 0.391 * (U - 128) + 0.813 * (V - 128)) / 1.164 + 16;

			if (Yg < 0) Yg = 0;
			else if (Yg > 255) Yg = 255;

			if (x % 2 == 0)
			{
				double Yb = (B - 2.018 * (U - 128)) / 1.164 + 16;

				if (Yb < 0) Yb = 0;
				else if (Yb > 255) Yb = 255;

				if (Yg <= Yb)
				{
					minVal = (int)floor(Yg);
					maxVal = (int)ceil(Yb);
				}
				else
				{
					minVal = (int)floor(Yb);
					maxVal = (int)ceil(Yg);
				}

				int error_TH = INT_MAX;
				for (int move_Y = minVal; move_Y <= maxVal; move_Y++)
				{
					int rec_G = (int)round(1.164 * (move_Y - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
					int rec_B = (int)round(1.164 * (move_Y - 16) + 2.018 * (U - 128));

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;

					int error = (G - rec_G)*(G - rec_G) + (B - rec_B)*(B - rec_B); // 避免call function 節省時間
					if (error < error_TH)
					{
						error_TH = error;
						new_Y_plane.at<uchar>(y, x) = (uchar)move_Y;
					}
				}
			}
			else
			{
				double Yr = (R - 1.596 * (V - 128)) / 1.164 + 16;

				if (Yr < 0) Yr = 0;
				else if (Yr > 255) Yr = 255;

				if (Yg <= Yr)
				{
					minVal = (int)floor(Yg);
					maxVal = (int)ceil(Yr);
				}
				else
				{
					minVal = (int)floor(Yr);
					maxVal = (int)ceil(Yg);
				}

				int error_TH = INT_MAX;
				for (int move_Y = minVal; move_Y <= maxVal; move_Y++)
				{
					int rec_G = (int)round(1.164 * (move_Y - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
					int rec_R = (int)round(1.164 * (move_Y - 16) + 1.596 * (V - 128));

					if (rec_G < 0) rec_G = 0;
					else if (rec_G > 255) rec_G = 255;

					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					int error = (G - rec_G)*(G - rec_G) + (R - rec_R)*(R - rec_R); // 避免call function 節省時間
					if (error < error_TH)
					{
						error_TH = error;
						new_Y_plane.at<uchar>(y, x) = (uchar)move_Y;
					}
				}
			}
		}
	}
}

/* Exhausted Search (�b 0~255 ���A��X�@�ӳ̨Ϊ� Y */
void ExhaustedSearch(int height, int width, Mat& new_Y_plane, const Mat& BGR, const Mat& US_U_plane, const Mat& US_V_plane)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int R = (int)BGR.at<Vec3b>(y, x)[2];
			int G = (int)BGR.at<Vec3b>(y, x)[1];
			int B = (int)BGR.at<Vec3b>(y, x)[0];

			double U = US_U_plane.at<double>(y, x);
			//cout << U << endl;
			U = (int)round(U);
			double V = US_V_plane.at<double>(y, x); 
			V = (int)round(V);
			double error_TH = DBL_MAX;

			for (int move_Y = 0; move_Y <= 255; move_Y++)
			{
				int rec_R = (int)round(1.164 * (move_Y - 16) + 1.596 * (V - 128));
				int rec_G = (int)round(1.164 * (move_Y - 16) - 0.391 * (U - 128) - 0.813 * (V - 128));
				int rec_B = (int)round(1.164 * (move_Y - 16) + 2.018 * (U - 128));

				if (rec_R < 0) rec_R = 0;
				else if (rec_R > 255) rec_R = 255;

				if (rec_G < 0) rec_G = 0;
				else if (rec_G > 255) rec_G = 255;

				if (rec_B < 0) rec_B = 0;
				else if (rec_B > 255) rec_B = 255;

				int error = (R - rec_R)*(R - rec_R) + (G - rec_G)*(G - rec_G) + (B - rec_B)*(B - rec_B); // �קKcall function �`�ٮɶ�
				//double error = 0.3 * (R - rec_R)*(R - rec_R) + 0.6 * (G - rec_G)*(G - rec_G) + 0.1 * (B - rec_B)*(B - rec_B);
				//int error = 0.25 * (R - rec_R)*(R - rec_R) + 0.5 * (G - rec_G)*(G - rec_G) + 0.25 * (B - rec_B)*(B - rec_B);
				if (error < error_TH)
				{
					error_TH = error;
					new_Y_plane.at<double>(y, x) = (double)move_Y;
				}
			}
		}
	}
}