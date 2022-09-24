#include "Subsample.h"

Mat IDID_HMatrix;

double luma_mean_preservation(Mat Y_ori, Mat Y_new) {
	int height = Y_ori.rows;
	int width = Y_ori.cols;
	double luma_mean = 0;

	for (int y = 0; y < height; y += 1)
	{
		for (int x = 0; x < width; x += 1)
		{
			luma_mean += (int)(abs(Y_ori.at<double>(y, x) - Y_new.at<double>(y, x)));
		}
	}
	return luma_mean / (height*width);
}

void DownSample_Bayer_LMCD_test(Mat&YUV, Mat&Y, Mat&U, Mat&V, Mat&imgrgbcp, Mat&new_Y_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	int height = YUV.rows;
	int width = YUV.cols;

	//解Y Y Us Vs
	Mat A4 = (Mat_<double>(4, 4) <<
		1.164, 0, -0.1794, -0.3731,
		0, 1.164, 0, 0.7324,
		0, 0, 0.9261, 0,
		0, 0, -0.1794, -0.3731);


	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);


	/*420A*/
	int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y, x + 1)[1] + YUV.at<Vec3d>(y + 1, x)[1] + YUV.at<Vec3d>(y + 1, x + 1)[1]) / 4, 0, 255);
			V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[2] + YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 4, 0, 255);
			xx++;
		}
		yy++;
	}

	copyMakeBorder(U, U, 2, 2, 2, 2, BORDER_REFLECT);
	copyMakeBorder(V, V, 2, 2, 2, 2, BORDER_REFLECT);

	//Mat AA = (A4.t()*A4).inv(DECOMP_SVD)*A4.t();
	Mat AA = A4.inv(DECOMP_SVD);
	//cout << AA << endl;


	Mat X, Y1;

	yy = 2;
	for (int y = 0; y < height; y += 2)
	{
		xx = 2;
		for (int x = 0; x < width; x += 2)
		{
			Up1 = (63*U.at<double>(yy, xx - 2) + 147*U.at<double>(yy, xx - 1) + 21*U.at<double>(yy - 1, xx - 2) + 49*U.at<double>(yy - 1, xx - 1) + 147 * U.at<double>(yy - 1, xx) + 9 * U.at<double>(yy - 2, xx - 2) + 21 * U.at<double>(yy - 2, xx - 1) + 63 * U.at<double>(yy - 2, xx)) / 961;
			Up2 = (63 * U.at<double>(yy, xx + 2) + 147 * U.at<double>(yy, xx + 1) + 21 * U.at<double>(yy - 1, xx + 2) + 49 * U.at<double>(yy - 1, xx + 1) + 147 * U.at<double>(yy - 1, xx) + 9 * U.at<double>(yy - 2, xx + 2) + 21 * U.at<double>(yy - 2, xx + 1) + 63 * U.at<double>(yy - 2, xx)) / 961;
			Up3 = (63 * U.at<double>(yy, xx - 2) + 147 * U.at<double>(yy, xx - 1) + 21 * U.at<double>(yy + 1, xx - 2) + 49 * U.at<double>(yy + 1, xx - 1) + 147 * U.at<double>(yy + 1, xx) + 9 * U.at<double>(yy + 2, xx - 2) + 21 * U.at<double>(yy + 2, xx - 1) + 63 * U.at<double>(yy + 2, xx)) / 961;
			Up4 = (63 * U.at<double>(yy, xx + 2) + 147 * U.at<double>(yy, xx + 1) + 21 * U.at<double>(yy + 1, xx + 2) + 49 * U.at<double>(yy + 1, xx + 1) + 147 * U.at<double>(yy + 1, xx) + 9 * U.at<double>(yy + 2, xx + 2) + 21 * U.at<double>(yy + 2, xx + 1) + 63 * U.at<double>(yy + 2, xx)) / 961;
			Vp1 = (63 * V.at<double>(yy, xx - 2) + 147 * V.at<double>(yy, xx - 1) + 21 * V.at<double>(yy - 1, xx - 2) + 49 * V.at<double>(yy - 1, xx - 1) + 147 * V.at<double>(yy - 1, xx) + 9 * V.at<double>(yy - 2, xx - 2) + 21 * V.at<double>(yy - 2, xx - 1) + 63 * V.at<double>(yy - 2, xx)) / 961;
			Vp2 = (63 * V.at<double>(yy, xx + 2) + 147 * V.at<double>(yy, xx + 1) + 21 * V.at<double>(yy - 1, xx + 2) + 49 * V.at<double>(yy - 1, xx + 1) + 147 * V.at<double>(yy - 1, xx) + 9 * V.at<double>(yy - 2, xx + 2) + 21 * V.at<double>(yy - 2, xx + 1) + 63 * V.at<double>(yy - 2, xx)) / 961;
			Vp3 = (63 * V.at<double>(yy, xx - 2) + 147 * V.at<double>(yy, xx - 1) + 21 * V.at<double>(yy + 1, xx - 2) + 49 * V.at<double>(yy + 1, xx - 1) + 147 * V.at<double>(yy + 1, xx) + 9 * V.at<double>(yy + 2, xx - 2) + 21 * V.at<double>(yy + 2, xx - 1) + 63 * V.at<double>(yy + 2, xx)) / 961;
			Vp4 = (63 * V.at<double>(yy, xx + 2) + 147 * V.at<double>(yy, xx + 1) + 21 * V.at<double>(yy + 1, xx + 2) + 49 * V.at<double>(yy + 1, xx + 1) + 147 * V.at<double>(yy + 1, xx) + 9 * V.at<double>(yy + 2, xx + 2) + 21 * V.at<double>(yy + 2, xx + 1) + 63 * V.at<double>(yy + 2, xx)) / 961;


			//解 Y Y U V
			Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
				(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			X = AA * Y1;
			//X = A4.inv(DECOMP_SVD)*Y1;
			//X = X + E;

			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			U.at<double>(yy, xx) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);

			xx++;
		}
		yy++;

	}

	U = U(Rect(2, 2, width / 2, height / 2));
	V = V(Rect(2, 2, width / 2, height / 2));
}

void DownSample_Bayer_LMCD_Iteration(Mat&YUV, Mat&Y, Mat&U, Mat&V, Mat&imgrgbcp, Mat&new_Y_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	int height = YUV.rows;
	int width = YUV.cols;

	////解Y1Y2Y3Y4UV
	/*Mat A4 = (Mat_<double>(4, 6) <<
		1.164, 0, 0, 0, -0.220, -0.4573,
		0, 1.164, 0, 0, 0, 0.8977,
		0, 0, 1.164, 0, 1.135, 0,
		0, 0, 0, 1.164, -0.220, -0.4573);*/

	//解Y Y Us Vs
	Mat A4 = (Mat_<double>(4, 4) <<
		0, 0, -0.220, -0.4573,
		1.164, 0, 0, 0.8977,
		0, 1.164, 1.135, 0,
		0, 0, -0.220, -0.4573);

	//只改Us Vs
	/*Mat A4 = (Mat_<double>(4, 2) <<
		-0.220, -0.4573,
		0, 0.8977,
		1.135, 0,
		-0.220, -0.4573);*/

	//解Y Us Vs
	/*Mat A4 = (Mat_<double>(4, 3) <<
		0, -0.220, -0.4573,
		0, 0, 0.8977,
		0, 1.135, 0,
		1.164, -0.220, -0.4573);*/



	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);

	Mat AA = (A4.t()*A4).inv(DECOMP_SVD)*A4.t();

	/*420A*/
	int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y, x + 1)[1] + YUV.at<Vec3d>(y + 1, x)[1] + YUV.at<Vec3d>(y + 1, x + 1)[1]) / 4, 0, 255);
			V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[2] + YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 4, 0, 255);
			xx++;
		}
		yy++;
	}

	copyMakeBorder(U, U, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(V, V, 1, 1, 1, 1, BORDER_REPLICATE);


	Mat X, YY;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			Up1 = (U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up2 = (U.at<double>(yy - 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up3 = (U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Up4 = (U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Vp1 = (V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp2 = (V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp3 = (V.at<double>(yy + 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;
			Vp4 = (V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;

			//測試
			//Y1 = (Mat_<double>(6, 1) <<       //G1 R2 G2 G3 B3 G4
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488, 
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2 - 135.488,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3 - 135.488,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);


			//解Y1 Y2 Y3 Y4 Us Vs
			//Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);

			//解 Y Y U V
			YY = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
				(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			//解Us Vs
			//YY = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912 - 1.164*new_Y_plane.at<double>(y, x + 1),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			//解Y Us Vs
			//YY = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912 - 1.164*new_Y_plane.at<double>(y, x + 1),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);


			X = AA*YY;
			//X = A4.inv(DECOMP_SVD)*Y1;
			//X = X + E;

			//new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			U.at<double>(yy, xx) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);

			xx++;
		}
		yy++;

	}

	//cout << "LMCD: " << "Us: " << U.at<double>(100, 100) << " " << "Vs: " << V.at<double>(100, 100) << endl;

	/*Iteration*/
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
	int M0, M1, M2, M3;
	double rec_M0, rec_M1, rec_M2, rec_M3;
	int Y0, Y1, Y2, Y3;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	/*double last_error;
	int same_num;*/
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double simpleArrayY[256];

	for (int arra = 0; arra < 256; arra++)
	{

		simpleArrayY[arra] = 1.164*(arra - 16);
	}
	
	double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
	double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{

			des_U = U.at<double>(yy, xx);
			des_V = V.at<double>(yy, xx);


			/*梯度下降*/


			M0 = (int)imgrgbcp.at<Vec3b>(y, x)[1];
			M1 = (int)imgrgbcp.at<Vec3b>(y, x + 1)[2];
			M2 = (int)imgrgbcp.at<Vec3b>(y + 1, x)[0];
			M3 = (int)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1];


			Y0 = (int)(new_Y_plane.at<double>(y, x) + 0.5);
			Y1 = (int)(new_Y_plane.at<double>(y, x + 1) + 0.5);
			Y2 = (int)(new_Y_plane.at<double>(y + 1, x) + 0.5);
			Y3 = (int)(new_Y_plane.at<double>(y + 1, x + 1) + 0.5);
			simpleYleftTop = simpleArrayY[Y0];
			simpleYr = simpleArrayY[Y1];
			simpleYb = simpleArrayY[Y2];
			simpleYrightDown = simpleArrayY[Y3];



			tmp_U = des_U;
			tmp_V = des_V;

			theLeftTopU = ((double)(tmp_U * 9) + U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			theLeftTopV = ((double)(tmp_V * 9) + V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			theRightTopV = ((double)(tmp_V * 9) + V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			theLeftDownU = ((double)(tmp_U * 9) + U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			theRightDownU = ((double)(tmp_U * 9) + U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			theRightDownV = ((double)(tmp_V * 9) + V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;


			rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
			rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
			rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
			rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
			error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



			if (error == 0) {

				U.at<double>(yy, xx) = des_U;
				V.at<double>(yy, xx) = des_V;

				xx++;
				continue;

			}



			error_TH = INT_MAX;
			for (int i = -1; i <= 1; i++) {
				tmp_V = des_V - i;


				if (0 <= tmp_V && tmp_V <= 255) {
					theLeftTopV = ((double)(tmp_V * 9) + V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
					theRightTopV = ((double)(tmp_V * 9) + V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
					theRightDownV = ((double)(tmp_V * 9) + V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;

					rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R

					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {

							theLeftTopU = ((double)(tmp_U * 9) + U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
							theLeftDownU = ((double)(tmp_U * 9) + U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
							theRightDownU = ((double)(tmp_U * 9) + U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;


							rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
							rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
							rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
							error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);


							if (error < error_TH)
							{
								error_TH = error;
								n_i = i;
								n_j = j;
								can_U = tmp_U;
								can_V = tmp_V;
							}

						}

					}
				}

			}

			des_U = can_U;
			des_V = can_V;


			/*	last_error = error_TH;
			same_num = 0;*/

			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						theLeftTopV = ((double)(tmp_V * 9) + V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
						theRightTopV = ((double)(tmp_V * 9) + V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
						theRightDownV = ((double)(tmp_V * 9) + V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;

						rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {
									theLeftTopU = ((double)(tmp_U * 9) + U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
									theLeftDownU = ((double)(tmp_U * 9) + U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
									theRightDownU = ((double)(tmp_U * 9) + U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;


									rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
									rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
									rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
									error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



									if (error < error_TH)
									{
										error_TH = error;
										tmp_i = i;
										tmp_j = j;
										can_U = tmp_U;
										can_V = tmp_V;
									}
								}


							}
						}
					}

				}
				n_i = tmp_i;
				n_j = tmp_j;
				des_U = can_U;
				des_V = can_V;


			}




			U.at<double>(yy, xx) = des_U;
			V.at<double>(yy, xx) = des_V;




			xx++;
		}

		yy++;


	}


	U = U(Rect(1, 1, width / 2, height / 2));
	V = V(Rect(1, 1, width / 2, height / 2));

	//cout << determinant(A4.inv(DECOMP_SVD)) << endl;
	//cout << "疊代： " << "Us: " << U.at<double>(100, 100) << " " << "Vs: " << V.at<double>(100, 100) << endl;
	/*cout << new_Y_plane.at<double>(100, 100) << " " << new_Y_plane.at<double>(100, 101) << endl;
	cout << new_Y_plane.at<double>(101, 100) << " " << new_Y_plane.at<double>(101, 101) << endl;*/


}

void DownSample_Bayer_LMCD(Mat&YUV, Mat&Y, Mat&U, Mat&V, Mat&imgrgbcp, Mat&new_Y_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	int height = YUV.rows;
	int width = YUV.cols;
	//Y = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);
	//U = Mat::zeros(height / 2, width / 2, CV_64FC1);//420
	//V = Mat::zeros(height / 2, width / 2, CV_64FC1);//420

	/*Mat UL = Mat::zeros(height, width, CV_64FC1);
	Mat VL = Mat::zeros(height, width, CV_64FC1);*/


	///因為沒把Y U V值傳過來所以要從YUV
	/*for (int i = 0; i < YUV.rows; i++)
		for (int j = 0; j < YUV.cols; j++) {
			Y.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
			new_Y_plane.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
		}*/
	

	//測試 G1 R2 G2 G3 B3 G4
	/*Mat A4 = (Mat_<double>(6, 6) << 
		1.164, 0, 0, 0, -0.220, -0.4573,
		0, 1.164, 0, 0, 0, 0.8977,
		0, 1.164, 0, 0, -0.220, -0.4573,
		0, 0, 1.164, 0, -0.220, -0.4573,
		0, 0, 1.164, 0, 1.135, 0,
		0, 0, 0, 1.164, -0.220, -0.4573);*/

	////解Y1Y2Y3Y4UV
	/*Mat A4 = (Mat_<double>(4, 6) <<
		1.164, 0, 0, 0, -0.220, -0.4573,
		0, 1.164, 0, 0, 0, 0.8977,
		0, 0, 1.164, 0, 1.135, 0,
		0, 0, 0, 1.164, -0.220, -0.4573);*/

	////解Y Y Y U V
	/*Mat A4 = (Mat_<double>(4, 5) <<
		1.164, 0, 0, -0.220, -0.4573,
		0, 1.164, 0, 0, 0.8977,
		0, 0, 1.164, 1.135, 0,
		0, 0, 0, -0.220, -0.4573);*/

	//解Y Y Us Vs
	Mat A4 = (Mat_<double>(4, 4) <<
		1.164, 0, -0.220, -0.4573,
		0, 1.164, 0, 0.8977,
		0, 0, 1.135, 0,
		0, 0, -0.220, -0.4573);

	//只改Us Vs
	/*Mat A4 = (Mat_<double>(4, 2) <<
		-0.220, -0.4573,
		0, 0.8977,
		1.135, 0,
		-0.220, -0.4573);*/

	//解Y Us Vs
	/*Mat A4 = (Mat_<double>(4, 3) <<
		0, -0.220, -0.4573,
		1.164, 0, 0.8977,
		0, 1.135, 0,
		0, -0.220, -0.4573);*/



	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);


	/*cout << YUV.at<Vec3d>(0, 0)[1] << "Ua1 ";
	cout << YUV.at<Vec3d>(0, 1)[1] << "Ub1 ";
	cout << YUV.at<Vec3d>(1, 0)[1] << "Uc1 ";
	cout << YUV.at<Vec3d>(1, 1)[1] << "Ud1 "<<endl;
	cout << YUV.at<Vec3d>(0, 2)[1] << "Ua2 ";
	cout << YUV.at<Vec3d>(0, 3)[1] << "Ub2 ";
	cout << YUV.at<Vec3d>(1, 2)[1] << "Uc2 ";
	cout << YUV.at<Vec3d>(1, 3)[1] << "Ud2 ";*/

	/*420A*/
	int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y, x + 1)[1] + YUV.at<Vec3d>(y + 1, x)[1] + YUV.at<Vec3d>(y + 1, x + 1)[1]) / 4, 0, 255);
			V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[2] + YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 4, 0, 255);
			xx++;
		}
		yy++;
	}
	/*cout << "Us: " << U.at<double>(100, 100) << " " << "Vs: " << V.at<double>(100, 100) << endl;
	cout << new_Y_plane.at<double>(100, 100) << " " << new_Y_plane.at<double>(100, 101) << endl;
	cout << new_Y_plane.at<double>(101, 100) << " " << new_Y_plane.at<double>(101, 101) << endl;*/

	//int yy = 0;
	//int xx = 0;
	//for (int y = 0; y < height; y += 2)
	//{
	//	xx = 0;
	//	for (int x = 0; x < width; x += 2)
	//	{
	//		U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y + 1, x)[1]) / 2, 0, 255);			//420L

	//		V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 2, 0, 255);		//420R
	//		xx++;
	//	}
	//	yy++;
	//}

	/*Mat A = (Mat_<double>(3, 3) <<
		1, 2, 3,
		4, 5, 6,
		7, 8, 9);
	Mat B = (Mat_<double>(3, 1) <<
		0,2,6);
	Mat Ans;
	Ans= (A.t()*A).inv()*A.t()*B;
	cout << Ans.at<double>(0, 0) << "HHH "<<endl;
	cout << Ans.at<double>(1, 0) << "HHH " << endl;
	cout << Ans.at<double>(2, 0) << "HHH" << endl;*/

	copyMakeBorder(U, U, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(V, V, 1, 1, 1, 1, BORDER_REPLICATE);

	//Mat AA = (A4.t()*A4).inv(DECOMP_SVD)*A4.t();
	Mat AA = A4.inv(DECOMP_SVD);
	//cout << AA << endl;


	Mat X, Y1;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			Up1 = (U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up2 = (U.at<double>(yy - 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up3 = (U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Up4 = (U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Vp1 = (V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp2 = (V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp3 = (V.at<double>(yy + 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;
			Vp4 = (V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;

			//測試
			//Y1 = (Mat_<double>(6, 1) <<       //G1 R2 G2 G3 B3 G4
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488, 
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2 - 135.488,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3 - 135.488,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);


			//解Y1 Y2 Y3 Y4 Us Vs
			//Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);

			//解Y1 Y2 Y3  Us Vs
			//Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 ,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));


			//解 Y Y U V
			Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
				(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			//解Us Vs
			//Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912 - 1.164*new_Y_plane.at<double>(y, x + 1),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			//解Y Us Vs
			//Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			

			X = AA*Y1;
			//X = A4.inv(DECOMP_SVD)*Y1;
			//X = X + E;

			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			U.at<double>(yy, xx) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);

			xx++;
		}
		yy++;

	}

	U = U(Rect(1, 1, width / 2, height / 2));
	V = V(Rect(1, 1, width / 2, height / 2));

	//cout << determinant(A4.inv(DECOMP_SVD)) << endl;
	/*cout << "Us: " << U.at<double>(100, 100) << " " << "Vs: " << V.at<double>(100, 100) << endl;
	cout << new_Y_plane.at<double>(100, 100) << " " << new_Y_plane.at<double>(100, 101) << endl;
	cout << new_Y_plane.at<double>(101, 100) << " " << new_Y_plane.at<double>(101, 101) << endl;*/



}


void Decide_Bicubic_Coef(Mat& Coef_A, Mat& Coef_B, Mat& Coef_C)
{
	// GT	A
	// B	C

	int coef_y = 0;
	int coef_x = 0;

	/* Type A */
	for (int offset_y = -4; offset_y < 7; offset_y += 2)
	{
		coef_x = 0;
		for (int offset_x = -5; offset_x < 6; offset_x += 2)
		{
			double fDY = (double)offset_y / 2.0;
			double fDX = (double)offset_x / 2.0;
			double coef = BC_funtion(fDY) * BC_funtion(fDX);

			Coef_A.at<double>(coef_y, coef_x) = coef;
			coef_x++;
		}
		coef_y++;
	}

	/* Type B */
	coef_y = 0;
	coef_x = 0;
	for (int offset_y = -5; offset_y < 6; offset_y += 2)
	{
		coef_x = 0;
		for (int offset_x = -4; offset_x < 7; offset_x += 2)
		{
			double fDY = (double)offset_y / 2.0;
			double fDX = (double)offset_x / 2.0;
			double coef = BC_funtion(fDY) * BC_funtion(fDX);

			Coef_B.at<double>(coef_y, coef_x) = coef;
			coef_x++;
		}
		coef_y++;
	}

	/* Type C */
	coef_y = 0;
	coef_x = 0;
	for (int offset_y = -5; offset_y < 6; offset_y += 2)
	{
		coef_x = 0;
		for (int offset_x = -5; offset_x < 6; offset_x += 2)
		{
			double fDY = (double)offset_y / 2.0;
			double fDX = (double)offset_x / 2.0;
			double coef = BC_funtion(fDY) * BC_funtion(fDX);

			Coef_C.at<double>(coef_y, coef_x) = coef;
			coef_x++;
		}
		coef_y++;
	}
}
void Decide_IDID_Bicubic_HMatrix(Mat& Coef_A, Mat& Coef_B, Mat& Coef_C, Mat& HMatrix)
{
	int B_sideLen = 16;
	int B_EleNum = B_sideLen * B_sideLen;
	int DS_B_sideLen = B_sideLen / 2;
	int DS_B_EleNum = DS_B_sideLen * DS_B_sideLen;

	int HMatrix_y, HMatrix_x;
	int yy = 0, xx = 0;

	for (int y = 0; y < B_sideLen; y++)
		for (int x = 0; x < B_sideLen; x++)
		{
			HMatrix_y = B_sideLen * y + x;

			yy = 0;
			xx = 0;
			if (y % 2 == 0 && x % 2 == 0)
			{
				HMatrix_x = DS_B_sideLen * (y / 2) + (x / 2);
				HMatrix.at<double>(mir(HMatrix_y, B_EleNum), mir(HMatrix_x, DS_B_EleNum)) = 1;
			}
			else if (y % 2 == 0 && x % 2 == 1)
			{
				for (int offset_y = -4; offset_y <= 6; offset_y += 2)
				{
					xx = 0;
					for (int offset_x = -5; offset_x <= 5; offset_x += 2)
					{
						int pos_y = (y + offset_y) / 2;
						int pos_x = (x + offset_x) / 2;

						if (pos_y >= 0 && pos_y < DS_B_sideLen && pos_x >= 0 && pos_x < DS_B_sideLen)
						{
							HMatrix_x = DS_B_sideLen * pos_y + pos_x;
							HMatrix.at<double>(mir(HMatrix_y, B_EleNum), mir(HMatrix_x, DS_B_EleNum)) = Coef_A.at<double>(yy, xx);
						}
						xx++;
					}
					yy++;
				}
			}
			else if (y % 2 == 1 && x % 2 == 0)
			{
				for (int offset_y = -5; offset_y <= 5; offset_y += 2)
				{
					xx = 0;
					for (int offset_x = -4; offset_x <= 6; offset_x += 2)
					{
						int pos_y = (y + offset_y) / 2;
						int pos_x = (x + offset_x) / 2;

						if (pos_y >= 0 && pos_y < DS_B_sideLen && pos_x >= 0 && pos_x < DS_B_sideLen)
						{
							HMatrix_x = DS_B_sideLen * pos_y + pos_x;
							HMatrix.at<double>(mir(HMatrix_y, B_EleNum), mir(HMatrix_x, DS_B_EleNum)) = Coef_B.at<double>(yy, xx);
						}
						xx++;
					}
					yy++;
				}
			}
			else
			{
				for (int offset_y = -5; offset_y <= 5; offset_y += 2)
				{
					xx = 0;
					for (int offset_x = -5; offset_x <= 5; offset_x += 2)
					{
						int pos_y = (y + offset_y) / 2;
						int pos_x = (x + offset_x) / 2;

						if (pos_y >= 0 && pos_y < DS_B_sideLen && pos_x >= 0 && pos_x < DS_B_sideLen)
						{
							HMatrix_x = DS_B_sideLen * pos_y + pos_x;
							HMatrix.at<double>(mir(HMatrix_y, B_EleNum), mir(HMatrix_x, DS_B_EleNum)) = Coef_C.at<double>(yy, xx);
						}
						xx++;
					}
					yy++;
				}
			}
		}
}
void Decide_HMatrix() {
	//IDID_HMatrix.create(256, 64, CV_64F);
	IDID_HMatrix = Mat::zeros(256, 64, CV_64F);
	Mat Coef_A(6, 6, CV_64F, Scalar(0));
	Mat Coef_B(6, 6, CV_64F, Scalar(0));
	Mat Coef_C(6, 6, CV_64F, Scalar(0));
	Decide_Bicubic_Coef(Coef_A, Coef_B, Coef_C);
	Decide_IDID_Bicubic_HMatrix(Coef_A, Coef_B, Coef_C, IDID_HMatrix);
}
Mat IDID_Bicubic(Mat& plane)
{
	Mat E;
	Mat block;
	Mat DS_block;
	int height = plane.rows;
	int width = plane.cols;
	int LR_height = height / 2;
	int LR_width = width / 2;
	int lr_y = 0;
	int lr_x = 0;
	int B_sideLen = 16;
	int B_EleNum = B_sideLen * B_sideLen;
	int DS_B_sideLen = B_sideLen / 2;
	int DS_B_EleNum = DS_B_sideLen * DS_B_sideLen;

	Mat DS_plane(height / 2, width / 2, CV_64F, Scalar(0));
	Mat ma = ((IDID_HMatrix.t() * IDID_HMatrix).inv()) * IDID_HMatrix.t();
	

	for (int hr_y = 0; hr_y < height; hr_y += B_sideLen)
	{
		lr_x = 0;
		for (int hr_x = 0; hr_x < width; hr_x += B_sideLen)
		{
			// 決定 E
			E = Mat::zeros(B_EleNum, 1, CV_64F); // compensate for pixel on the edge of block

			for (int offset_y = 0; offset_y < B_sideLen; offset_y++)
				for (int offset_x = 0; offset_x < B_sideLen; offset_x++)
				{
					int E_y = B_sideLen * offset_y + offset_x;

					int center_y = hr_y + offset_y;
					int center_x = hr_x + offset_x;

					if (center_y % 2 == 0 && center_x % 2 == 1)
					{
						for (int offset_yy = -4; offset_yy <= 6; offset_yy += 2)
							for (int offset_xx = -5; offset_xx <= 5; offset_xx += 2)
							{
								// 位置
								int curr_y = center_y + offset_yy;
								int curr_x = center_x + offset_xx;

								int pos_y = (offset_y + offset_yy) / 2;
								int pos_x = (offset_x + offset_xx) / 2;

								if (pos_y < 0 || pos_y >= DS_B_sideLen || pos_x < 0 || pos_x >= DS_B_sideLen)
								{
									// 係數
									double fDY = (double)offset_yy / 2.0;
									double fDX = (double)offset_xx / 2.0;
									double coef = BC_funtion(fDY) * BC_funtion(fDX);

									E.at<double>(E_y, 0) += coef * plane.at<double>(mir(curr_y, height), mir(curr_x, width));
								}
							}
					}
					else if (center_y % 2 == 1 && center_x % 2 == 0)
					{
						for (int offset_yy = -5; offset_yy <= 5; offset_yy += 2)
							for (int offset_xx = -4; offset_xx <= 6; offset_xx += 2)
							{
								// 位置
								int curr_y = center_y + offset_yy;
								int curr_x = center_x + offset_xx;

								int pos_y = (offset_y + offset_yy) / 2;
								int pos_x = (offset_x + offset_xx) / 2;

								if (pos_y < 0 || pos_y >= DS_B_sideLen || pos_x < 0 || pos_x >= DS_B_sideLen)
								{
									// 係數
									double fDY = (double)offset_yy / 2.0;
									double fDX = (double)offset_xx / 2.0;
									double coef = BC_funtion(fDY) * BC_funtion(fDX);

									E.at<double>(E_y, 0) += coef * plane.at<double>(mir(curr_y, height), mir(curr_x, width));
								}
							}
					}
					else if (center_y % 2 == 1 && center_x % 2 == 1)
					{
						for (int offset_yy = -5; offset_yy <= 5; offset_yy += 2)
							for (int offset_xx = -5; offset_xx <= 5; offset_xx += 2)
							{
								// 位置
								int curr_y = center_y + offset_yy;
								int curr_x = center_x + offset_xx;

								int pos_y = (offset_y + offset_yy) / 2;
								int pos_x = (offset_x + offset_xx) / 2;

								if (pos_y < 0 || pos_y >= DS_B_sideLen || pos_x < 0 || pos_x >= DS_B_sideLen)
								{
									// 係數
									double fDY = (double)offset_yy / 2.0;
									double fDX = (double)offset_xx / 2.0;
									double coef = BC_funtion(fDY) * BC_funtion(fDX);

									E.at<double>(E_y, 0) += coef * plane.at<double>(mir(curr_y, height), mir(curr_x, width));
								}
							}
					}
				}

			block = Mat::zeros(B_EleNum, 1, CV_64F);
			int index = 0;
			for (int y = 0; y < B_sideLen; y++)
				for (int x = 0; x < B_sideLen; x++)
				{
					block.at<double>(index, 0) = plane.at<double>(mir(hr_y + y, height), mir(hr_x + x, width));
					index++;
				}

			DS_block = Mat::zeros(DS_B_EleNum, 1, CV_64F);
			//DS_block = ((HMatrix.t() * HMatrix).inv()) * HMatrix.t() * block;//(block - E);
			//DS_block = ((IDID_HMatrix.t() * IDID_HMatrix).inv()) * IDID_HMatrix.t() * (block - E);
			DS_block = ma * (block - E);
			for (int i = 0; i < DS_B_EleNum; i++)
			{
				if (DS_block.at<double>(i, 0) < 0)
					DS_block.at<double>(i, 0) = 0;
				else if (DS_block.at<double>(i, 0) > 255)
					DS_block.at<double>(i, 0) = 255;
			}

			index = 0;
			for (int j = 0; j < DS_B_sideLen; j++)
				for (int i = 0; i < DS_B_sideLen; i++)
				{
					DS_plane.at<double>(mir(lr_y + j, LR_height), mir(lr_x + i, LR_width)) = DS_block.at<double>(index, 0);
					index++;
				}

			lr_x += DS_B_sideLen;
		}
		lr_y += DS_B_sideLen;
	}
	return DS_plane;
}
void MIDID_BC(Mat& YUV, Mat& Y, Mat& U, Mat& V) {
	Y = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);
	U = Mat::zeros(YUV.rows / 2, YUV.cols / 2, CV_64FC1);
	V = Mat::zeros(YUV.rows / 2, YUV.cols / 2, CV_64FC1);
	/*cout << V.at<double>(0, 0) << endl;
	cout << V.at<double>(0, 1) << endl;
	cout << V.at<double>(1, 0) << endl;
	cout << V.at<double>(1, 1) << endl;*/

	for (int i = 0; i < Y.rows; i++)
		for (int j = 0; j < Y.cols; j++)
			Y.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];

	Mat tempU = Mat::zeros(YUV.rows, YUV.cols, CV_64F);
	for (int i = 0; i < tempU.rows; i++)
		for (int j = 0; j < tempU.cols; j++)
			tempU.at<double>(i, j) = YUV.at<Vec3d>(i, j)[1];
	//ds_method(tempU, U, DOWN_MIDID_BC);
	//U = IDID_Bicubic(tempU);

	resize(tempU, U, Size(), 0.5, 0.5, INTER_CUBIC);
	U = IDID_Bicubic(tempU);

	Mat tempV = Mat::zeros(YUV.rows, YUV.cols, CV_64F);
	for (int i = 0; i < tempV.rows; i++)
		for (int j = 0; j < tempV.cols; j++)
			tempV.at<double>(i, j) = YUV.at<Vec3d>(i, j)[2];

	//ds_method(tempV, V, DOWN_MIDID_BC);
	//V = IDID_Bicubic(tempV);
	/*cout << V.at<Vec3d>(0, 0)[2] << endl;
	cout << V.at<Vec3d>(0, 1)[2] << endl;
	cout << V.at<Vec3d>(1, 0)[2] << endl;
	cout << V.at<Vec3d>(1, 1)[2] << endl;*/
	//showUchar(V);
	//

	resize(tempV, V, Size(), 0.5, 0.5, INTER_CUBIC);
	V = IDID_Bicubic(tempV);
}

void DownSample_420JDU(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {

	//420A
	int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(y, x) + U_plane.at<double>(y, x + 1) + U_plane.at<double>(y + 1, x) + U_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(y, x) + V_plane.at<double>(y, x + 1) + V_plane.at<double>(y + 1, x) + V_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			xx++;
		}
		yy++;
	}

	//找4種組合
	double error_TH;
	double error[4],error_total[4];
	double a = -0.391;
	double b = 2.018;
	double c = 1.596;
	double d = -0.813;

	double a_pow_2 = pow(-0.391, 2);
	double b_pow_2 = pow(2.018, 2);
	double c_pow_2 = pow(1.596, 2);
	double d_pow_2 = pow(-0.813, 2);
	double mul_ad = (-0.391 * -0.813);
	double a_plus_b = a_pow_2 + b_pow_2;
	double c_plus_d = c_pow_2 + d_pow_2;
	int DS_U_floor, DS_U_ceil, DS_V_floor, DS_V_ceil;
	int min_index;

	yy = 0;
	xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_floor = floor(DS_U_plane.at<double>(yy, xx));
			DS_V_floor = floor(DS_V_plane.at<double>(yy, xx));
			DS_U_ceil = ceil(DS_U_plane.at<double>(yy, xx));
			DS_V_ceil = ceil(DS_V_plane.at<double>(yy, xx));

			for (int i = 0; i < 4; i++)
				error_total[i] = 0;

			//(floor,floor)
			error[0] = c_pow_2* pow((DS_V_floor - V_plane.at<double>(y, x)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y, x)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y, x)) + d* (DS_V_floor - V_plane.at<double>(y, x)), 2) ;
			error[1] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y, x + 1)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y, x + 1)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y, x + 1)) + d * (DS_V_floor - V_plane.at<double>(y, x + 1)), 2);
			error[2] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y + 1, x)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y + 1, x)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y + 1, x)) + d * (DS_V_floor - V_plane.at<double>(y + 1, x)), 2);
			error[3] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y + 1, x + 1)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y + 1, x + 1)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y + 1, x + 1)) + d * (DS_V_floor - V_plane.at<double>(y + 1, x + 1)), 2);
			error_total[0] = error[0] + error[1] + error[2] + error[3];
			//cout << error[0] << " " << error[1] << " " << error[2] << " " << error[3] << endl;
			//cout << error_total[0] << endl;

			//(floor,ceil)
			error[0] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y, x)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y, x)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y, x)) + d * (DS_V_ceil - V_plane.at<double>(y, x)), 2);
			error[1] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y, x + 1)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y, x + 1)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y, x + 1)) + d * (DS_V_ceil - V_plane.at<double>(y, x + 1)), 2);
			error[2] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y + 1, x)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y + 1, x)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y + 1, x)) + d * (DS_V_ceil - V_plane.at<double>(y + 1, x)), 2);
			error[3] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y + 1, x + 1)), 2) + b_pow_2 * pow((DS_U_floor - U_plane.at<double>(y + 1, x + 1)), 2) + pow(a*(DS_U_floor - U_plane.at<double>(y + 1, x + 1)) + d * (DS_V_ceil - V_plane.at<double>(y + 1, x + 1)), 2);
			error_total[1] = error[0] + error[1] + error[2] + error[3];
			//cout << error[0] << " " << error[1] << " " << error[2] << " " << error[3] << endl;
			//cout << error_total[1] << endl;

			//(ceil,floor)
			error[0] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y, x)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y, x)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y, x)) + d * (DS_V_floor - V_plane.at<double>(y, x)), 2);
			error[1] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y, x + 1)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y, x + 1)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y, x + 1)) + d * (DS_V_floor - V_plane.at<double>(y, x + 1)), 2);
			error[2] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y + 1, x)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y + 1, x)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y + 1, x)) + d * (DS_V_floor - V_plane.at<double>(y + 1, x)), 2);
			error[3] = c_pow_2 * pow((DS_V_floor - V_plane.at<double>(y + 1, x + 1)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y + 1, x + 1)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y + 1, x + 1)) + d * (DS_V_floor - V_plane.at<double>(y + 1, x + 1)), 2);
			error_total[2] = error[0] + error[1] + error[2] + error[3];
			//cout << error[0] << " " << error[1] << " " << error[2] << " " << error[3] << endl;
			//cout << error_total[2] << endl;

			//(ceil,ceil)
			error[0] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y, x)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y, x)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y, x)) + d * (DS_V_ceil - V_plane.at<double>(y, x)), 2);
			error[1] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y, x + 1)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y, x + 1)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y, x + 1)) + d * (DS_V_ceil - V_plane.at<double>(y, x + 1)), 2);
			error[2] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y + 1, x)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y + 1, x)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y + 1, x)) + d * (DS_V_ceil - V_plane.at<double>(y + 1, x)), 2);
			error[3] = c_pow_2 * pow((DS_V_ceil - V_plane.at<double>(y + 1, x + 1)), 2) + b_pow_2 * pow((DS_U_ceil - U_plane.at<double>(y + 1, x + 1)), 2) + pow(a*(DS_U_ceil - U_plane.at<double>(y + 1, x + 1)) + d * (DS_V_ceil - V_plane.at<double>(y + 1, x + 1)), 2);
			error_total[3] = error[0] + error[1] + error[2] + error[3];
			//cout << error[0] << " " << error[1] << " " << error[2] << " " << error[3] << endl;
			//cout << error_total[3] << endl;

			//cout << error_total[0] << " " << error_total[1] << " " << error_total[2] << " " << error_total[3] << endl;

			error_TH = INT_MAX;
			min_index = 0;
			//find_min
			for (int i = 0; i < 4; i++) {
				if (error_total[i] < error_TH) {
					min_index = i;
					error_TH = error_total[i];
				}
			}
			//cout << DS_U_plane.at<double>(yy, xx) << " " << DS_V_plane.at<double>(yy, xx) << endl;
			switch (min_index) {
			case 0:
				DS_U_plane.at<double>(yy, xx) = DS_U_floor;
				DS_V_plane.at<double>(yy, xx) = DS_V_floor;
				break;
			case 1:
				DS_U_plane.at<double>(yy, xx) = DS_U_floor;
				DS_V_plane.at<double>(yy, xx) = DS_V_ceil;
				break;
			case 2:
				DS_U_plane.at<double>(yy, xx) = DS_U_ceil;
				DS_V_plane.at<double>(yy, xx) = DS_V_floor;
				break;
			case 3:
				DS_U_plane.at<double>(yy, xx) = DS_U_ceil;
				DS_V_plane.at<double>(yy, xx) = DS_V_ceil;
				break;
			default:
				cout << "Error!!!" << endl;
				break;
			}
			//cout << DS_U_plane.at<double>(yy, xx) << " " << DS_V_plane.at<double>(yy, xx) << endl;
			//cout << min_index << endl;
			xx++;
		}
		yy++;
	}


}

void DownSample_DTDI_test(Mat&YUV, Mat&Y, Mat&U, Mat&V, Mat&imgrgbcp, Mat&new_Y_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	int height = YUV.rows;
	int width = YUV.cols;
	//Y = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);
	//U = Mat::zeros(height / 2, width / 2, CV_64FC1);//420
	//V = Mat::zeros(height / 2, width / 2, CV_64FC1);//420

	/*Mat UL = Mat::zeros(height, width, CV_64FC1);
	Mat VL = Mat::zeros(height, width, CV_64FC1);*/


	///因為沒把Y U V值傳過來所以要從YUV
	/*for (int i = 0; i < YUV.rows; i++)
		for (int j = 0; j < YUV.cols; j++) {
			Y.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
			new_Y_plane.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
		}*/

	Mat A4 = (Mat_<double>(8, 6) <<
		//1.164, 0, 0, 0, 0, 0.8977,
		1.164, 0, 0, 0, -0.220, -0.4573,
		1.164, 0, 0, 0, 1.135, 0,

		0, 1.164, 0, 0, 0, 0.8977,
		0, 1.164, 0, 0, -0.220, -0.4573,
		//0, 1.164, 0, 0, 1.135, 0,

		//0, 0, 1.164, 0, 0, 0.8977,
		0, 0, 1.164, 0, -0.220, -0.4573,
		0, 0, 1.164, 0, 1.135, 0,

		0, 0, 0, 1.164, 0, 0.8977,
		0, 0, 0, 1.164, -0.220, -0.4573);
		//0, 0, 0, 1.164, 1.135, 0);
	////Mat A4 = (Mat_<double>(8, 6) <<
	//	//1.164, 0, 0, 0, 0, 1.596,
	//	1.164, 0, 0, 0, -0.391, -0.813,
	//	1.164, 0, 0, 0, 2.018, 0,

	//	0, 1.164, 0, 0, 0, 1.596,
	//	0, 1.164, 0, 0, -0.391, -0.813,
	//	//0, 1.164, 0, 0, 2.018, 0,

	//	//0, 0, 1.164, 0, 0, 1.596,
	//	0, 0, 1.164, 0, -0.391, -0.813,
	//	0, 0, 1.164, 0, 2.018, 0,

	//	0, 0, 0, 1.164, 0, 1.596,
	//	0, 0, 0, 1.164, -0.391, -0.813);
	//	//0, 0, 0, 1.164, 2.018, 0);

	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);
	

	/*cout << YUV.at<Vec3d>(0, 0)[1] << "Ua1 ";
	cout << YUV.at<Vec3d>(0, 1)[1] << "Ub1 ";
	cout << YUV.at<Vec3d>(1, 0)[1] << "Uc1 ";
	cout << YUV.at<Vec3d>(1, 1)[1] << "Ud1 "<<endl;
	cout << YUV.at<Vec3d>(0, 2)[1] << "Ua2 ";
	cout << YUV.at<Vec3d>(0, 3)[1] << "Ub2 ";
	cout << YUV.at<Vec3d>(1, 2)[1] << "Uc2 ";
	cout << YUV.at<Vec3d>(1, 3)[1] << "Ud2 ";*/

	/*420A*/
	/*int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y, x + 1)[1] + YUV.at<Vec3d>(y + 1, x)[1] + YUV.at<Vec3d>(y + 1, x + 1)[1]) / 4, 0, 255);
			V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[2] + YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 4, 0, 255);
			xx++;
		}
		yy++;
	}*/


	int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y + 1, x)[1]) / 2, 0, 255);			//420L
			
			V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 2, 0, 255);		//420R
			xx++;
		}
		yy++;
	}
	//cout << U.at<double>(100, 100) << "  " << V.at<double>(100, 100) << endl;
	/*Mat A = (Mat_<double>(3, 3) <<
		1, 2, 3,
		4, 5, 6,
		7, 8, 9);
	Mat B = (Mat_<double>(3, 1) <<
		0,2,6);
	Mat Ans;
	Ans= (A.t()*A).inv()*A.t()*B;
	cout << Ans.at<double>(0, 0) << "HHH "<<endl;
	cout << Ans.at<double>(1, 0) << "HHH " << endl;
	cout << Ans.at<double>(2, 0) << "HHH" << endl;*/

	copyMakeBorder(U, U, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(V, V, 1, 1, 1, 1, BORDER_REPLICATE);


	Mat X, Y1;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			Up1 = (U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up2 = (U.at<double>(yy - 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up3 = (U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Up4 = (U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Vp1 = (V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp2 = (V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp3 = (V.at<double>(yy + 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;
			Vp4 = (V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;



			//Y1 = (Mat_<double>(12, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[0] ,

			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[0] ,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] ,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[0] );


			Y1 = (Mat_<double>(8, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
				//(double)imgrgbcp.at<Vec3b>(y, x)[2] - 1.596*Vp1 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y, x)[0] - 2.018*Up1 + 276.928,

				(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2 - 135.488,
				//(double)imgrgbcp.at<Vec3b>(y, x + 1)[0] - 2.018*Up2 + 276.928,

				//(double)imgrgbcp.at<Vec3b>(y + 1, x)[2] - 1.596*Vp3 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,

				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[2] - 1.596*Vp4 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);
				//(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[0] - 2.018*Up4 + 276.928);

			//Y1 = (Mat_<double>(8, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	//(double)imgrgbcp.at<Vec3b>(y, x)[2] - 1.596*Vp1,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[0] - 2.018*Up1,

			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2,
			//	//(double)imgrgbcp.at<Vec3b>(y, x + 1)[0] - 2.018*Up2,

			//	//(double)imgrgbcp.at<Vec3b>(y + 1, x)[2] - 1.596*Vp3,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[2] - 1.596*Vp4,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4);
			//	//(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[0] - 2.018*Up4);


			X = (A4.t()*A4).inv()*A4.t()*Y1;
			//X = X + E;

			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);
			/*U.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) / 0.5625 + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) / 0.5625 + 0.5), 0, 255);*/
			U.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) + 0.5), 0, 255);

			xx++;
		}
		yy++;

	}

	U = U(Rect(1, 1, width / 2, height / 2));
	V = V(Rect(1, 1, width / 2, height / 2));

	double YM = 0;
	for (int i = 0; i < YUV.rows; i++)        //算有動的Y的平均
		for (int j = 0; j < YUV.cols; j++) {
			YM += new_Y_plane.at<double>(i, j);
		}


}


// 仁舜 全彩
void DownSample_420LACS(Mat&YUV, Mat&Y, Mat&U, Mat&V, Mat&imgrgbcp, Mat&new_Y_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	int height = YUV.rows;
	int width = YUV.cols;
	//Y = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);
	//U = Mat::zeros(height / 2, width / 2, CV_64FC1);//420
	//V = Mat::zeros(height / 2, width / 2, CV_64FC1);//420

	/*Mat UL = Mat::zeros(height, width, CV_64FC1);
	Mat VL = Mat::zeros(height, width, CV_64FC1);*/


	///因為沒把Y U V值傳過來所以要從YUV
	/*for (int i = 0; i < YUV.rows; i++)
		for (int j = 0; j < YUV.cols; j++) {
			Y.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
			new_Y_plane.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
		}*/

	Mat A4 = (Mat_<double>(12, 6) <<
		1.164, 0, 0, 0, 0, 0.8977,
		1.164, 0, 0, 0, -0.220, -0.4573,
		1.164, 0, 0, 0, 1.135, 0,

		0, 1.164, 0, 0, 0, 0.8977,
		0, 1.164, 0, 0, -0.220, -0.4573,
		0, 1.164, 0, 0, 1.135, 0,

		0, 0, 1.164, 0, 0, 0.8977,
		0, 0, 1.164, 0, -0.220, -0.4573,
		0, 0, 1.164, 0, 1.135, 0,

		0, 0, 0, 1.164, 0, 0.8977,
		0, 0, 0, 1.164, -0.220, -0.4573,
		0, 0, 0, 1.164, 1.135, 0);
	/*Mat A4 = (Mat_<double>(12, 6) <<
		1.164, 0, 0, 0, 0, 1.596,
		1.164, 0, 0, 0, -0.391, -0.813,
		1.164, 0, 0, 0, 2.018, 0,

		0, 1.164, 0, 0, 0, 1.596,
		0, 1.164, 0, 0, -0.391, -0.813,
		0, 1.164, 0, 0, 2.018, 0,

		0, 0, 1.164, 0, 0, 1.596,
		0, 0, 1.164, 0, -0.391, -0.813,
		0, 0, 1.164, 0, 2.018, 0,

		0, 0, 0, 1.164, 0, 1.596,
		0, 0, 0, 1.164, -0.391, -0.813,
		0, 0, 0, 1.164, 2.018, 0);*/



	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);
	int yy = 0;
	int xx = 0;

	/*cout << YUV.at<Vec3d>(0, 0)[1] << "Ua1 ";
	cout << YUV.at<Vec3d>(0, 1)[1] << "Ub1 ";
	cout << YUV.at<Vec3d>(1, 0)[1] << "Uc1 ";
	cout << YUV.at<Vec3d>(1, 1)[1] << "Ud1 "<<endl;
	cout << YUV.at<Vec3d>(0, 2)[1] << "Ua2 ";
	cout << YUV.at<Vec3d>(0, 3)[1] << "Ub2 ";
	cout << YUV.at<Vec3d>(1, 2)[1] << "Uc2 ";
	cout << YUV.at<Vec3d>(1, 3)[1] << "Ud2 ";*/

	/*420A*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[1] + YUV.at<Vec3d>(y, x + 1)[1] + YUV.at<Vec3d>(y + 1, x)[1] + YUV.at<Vec3d>(y + 1, x + 1)[1]) / 4, 0, 255);
			V.at<double>(yy, xx) = Clip((YUV.at<Vec3d>(y, x)[2] + YUV.at<Vec3d>(y, x + 1)[2] + YUV.at<Vec3d>(y + 1, x)[2] + YUV.at<Vec3d>(y + 1, x + 1)[2]) / 4, 0, 255);
			xx++;
		}
		yy++;

	}

	/*Mat A = (Mat_<double>(3, 3) <<
		1, 2, 3,
		4, 5, 6,
		7, 8, 9);
	Mat B = (Mat_<double>(3, 1) <<
		0,2,6);
	Mat Ans;
	Ans= (A.t()*A).inv()*A.t()*B;
	cout << Ans.at<double>(0, 0) << "HHH "<<endl;
	cout << Ans.at<double>(1, 0) << "HHH " << endl;
	cout << Ans.at<double>(2, 0) << "HHH" << endl;*/

	copyMakeBorder(U, U, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(V, V, 1, 1, 1, 1, BORDER_REPLICATE);


	Mat X, Y1;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			Up1 = (U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up2 = (U.at<double>(yy - 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up3 = (U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Up4 = (U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Vp1 = (V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp2 = (V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp3 = (V.at<double>(yy + 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;
			Vp4 = (V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;



			//Y1 = (Mat_<double>(12, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[0] ,

			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[0] ,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] ,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[2] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] ,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[0] );

			Y1 = (Mat_<double>(12, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
				(double)imgrgbcp.at<Vec3b>(y, x)[2] - 1.596*Vp1 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y, x)[0] - 2.018*Up1 + 276.928,

				(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[0] - 2.018*Up2 + 276.928,

				(double)imgrgbcp.at<Vec3b>(y + 1, x)[2] - 1.596*Vp3 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,

				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[2] - 1.596*Vp4 + 222.912,
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488,
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[0] - 2.018*Up4 + 276.928);

			//Y1 = (Mat_<double>(12, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[2] - 1.596*Vp1,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1,
			//	(double)imgrgbcp.at<Vec3b>(y, x)[0] - 2.018*Up1,

			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2,
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[0] - 2.018*Up2,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[2] - 1.596*Vp3,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3,

			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[2] - 1.596*Vp4,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[0] - 2.018*Up4);


			X = (A4.t()*A4).inv()*A4.t()*Y1;
			//X = X + E;

			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);
			/*U.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) / 0.5625 + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) / 0.5625 + 0.5), 0, 255);*/
			U.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) + 0.5), 0, 255);

			xx++;
		}
		yy++;

	}

	U = U(Rect(1, 1, width / 2, height / 2));
	V = V(Rect(1, 1, width / 2, height / 2));

	double YM = 0;
	for (int i = 0; i < YUV.rows; i++)        //算有動的Y的平均
		for (int j = 0; j < YUV.cols; j++) {
			YM += new_Y_plane.at<double>(i, j);
		}


}
void DownSample_420BRIGHT(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	/*Y = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);
	U = Mat::zeros(YUV.rows / 2, YUV.cols / 2, CV_64FC1);
	V = Mat::zeros(YUV.rows / 2, YUV.cols / 2, CV_64FC1);
	int height = YUV.rows;
	int width = YUV.cols;*/

	int yy = 0;
	int xx = 0;

	/*for (int i = 0; i < YUV.rows; i++)
		for (int j = 0; j < YUV.cols; j++) {
			Y.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
		}*/

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			int Y_left_top = Y_plane.at<double>(y, x);
			int Y_right_top = Y_plane.at<double>(y, x + 1);
			int Y_left_bottom = Y_plane.at<double>(y + 1, x);
			int Y_right_bottom = Y_plane.at<double>(y + 1, x + 1);

			int choice = 0;
			if ((Y_left_top >= Y_right_top) && (Y_left_top >= Y_left_bottom) && (Y_left_top >= Y_right_bottom))
				choice = 1;
			else if ((Y_right_top >= Y_left_top) && (Y_right_top >= Y_left_bottom) && (Y_right_top >= Y_right_bottom))
				choice = 2;
			else if ((Y_left_bottom >= Y_left_top) && (Y_left_bottom >= Y_right_top) && (Y_left_bottom >= Y_right_bottom))
				choice = 3;
			else
				choice = 4;

			switch (choice)
			{
			case 1:
				DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y, x);
				DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y, x);
				break;
			case 2:
				DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y, x + 1);
				DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y, x + 1);
				break;
			case 3:
				DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y + 1, x);
				DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y + 1, x);
				break;
			case 4:
				DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y + 1, x + 1);
				DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y + 1, x + 1);
				break;
			}
			xx++;
		}
		yy++;
	}
}

void DownSample_420BRIGHT_MEAN(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	/*Y = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);
	U = Mat::zeros(YUV.rows / 2, YUV.cols / 2, CV_64FC1);
	V = Mat::zeros(YUV.rows / 2, YUV.cols / 2, CV_64FC1);

	int height = YUV.rows;
	int width = YUV.cols;*/

	int yy = 0;
	int xx = 0;

	/*for (int i = 0; i < YUV.rows; i++)
		for (int j = 0; j < YUV.cols; j++) {
			Y.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];
		}*/

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			double Y_left_top = Y_plane.at<double>(y, x);
			double Y_right_top = Y_plane.at<double>(y, x + 1);
			double Y_left_bottom = Y_plane.at<double>(y + 1, x);
			double Y_right_bottom = Y_plane.at<double>(y + 1, x + 1);

			/* find the largest luminance */
			double largest = 0;
			int choice = 0;
			if ((Y_left_top >= Y_right_top) && (Y_left_top >= Y_left_bottom) && (Y_left_top >= Y_right_bottom))
			{
				choice = 1;
				largest = Y_left_top;
			}
			else if ((Y_right_top >= Y_left_top) && (Y_right_top >= Y_left_bottom) && (Y_right_top >= Y_right_bottom))
			{
				choice = 2;
				largest = Y_right_top;
			}
			else if ((Y_left_bottom >= Y_left_top) && (Y_left_bottom >= Y_right_top) && (Y_left_bottom >= Y_right_bottom))
			{
				choice = 3;
				largest = Y_left_bottom;
			}
			else
			{
				choice = 4;
				largest = Y_right_bottom;
			}

			/* find the smallest luminance */
			double smallest = Y_left_top;
			if (Y_right_top < smallest)
				smallest = Y_right_top;
			if (Y_left_bottom < smallest)
				smallest = Y_left_bottom;
			if (Y_right_bottom < smallest)
				smallest = Y_right_bottom;

			double ratio = largest / smallest;
			/* BRIGHT method */
			if (ratio > 2)
			{
				switch (choice)
				{
				case 1:
					DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y, x);
					DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y, x);
					break;
				case 2:
					DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y, x + 1);
					DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y, x + 1);
					break;
				case 3:
					DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y + 1, x);
					DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y + 1, x);
					break;
				case 4:
					DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y + 1, x + 1);
					DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y + 1, x + 1);
					break;
				}
			}
			/* MEAN method */
			else
			{
				double left_top = U_plane.at<double>(y, x);
				double right_top = U_plane.at<double>(y, x + 1);
				double left_bottom = U_plane.at<double>(y + 1, x);
				double right_bottom = U_plane.at<double>(y + 1, x + 1);

				double avg = (left_top + right_top + left_bottom + right_bottom) / 4;
				DS_U_plane.at<double>(yy, xx) = avg;

				//

				left_top = V_plane.at<double>(y, x);
				right_top = V_plane.at<double>(y, x + 1);
				left_bottom = V_plane.at<double>(y + 1, x);
				right_bottom = V_plane.at<double>(y + 1, x + 1);

				avg = (left_top + right_top + left_bottom + right_bottom) / 4;
				DS_V_plane.at<double>(yy, xx) = avg;
			}
			xx++;
		}
		yy++;
	}
}



//以下都是舊的
void DownSample_420MMCS_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
	//double mean_range = 0;
	int sortI, sortJ;
	int yy = 0;
	int xx = 0;

	/*char filenameY2[] = "Kodak1Range.txt";
	fstream fr2;
	fr2.open(filenameY2, ios::out);*/

	int R, GleftTop, GrightDown, B, Yb, Yr, YleftTop, YrightDown;
	int y, x;
	double Ub, Vr, Ug_leftTop, Ug_rightDown, Vg_leftTop, Vg_rightDown;
	int UbFloor, UbCeil, VbFloor, VbCeil, Ug_leftTopFloor, Ug_leftTopCeil, Ug_rightDownFloor, Ug_rightDownCeil, Vg_leftTopFloor, Vg_leftTopCeil, Vg_rightDownFloor, Vg_rightDownCeil;
	int rec_B, rec_G1, rec_G2, rec_R;
	int move_U, move_V;

	int error;
	double Ymult = 18.624, Vmult = 104.064, Umult = 50.048;//Ymult(1.164*16) Vmult(0.813*128) Umult(0.391*128)
	double simpleU, simpleV;
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
	double simpleArrayU[2][256], simpleArrayV[2][256], simpleArrayY[1][256];
	double U1, U2, U3, U4, V1, V2, V3, V4;
	double Umin, Umax, Vmin, Vmax;
	int UminInteger, UmaxInteger, VminInteger, VmaxInteger;
	//int simpleError[1][511];//----------------------------------------------0224還未做
	int arra;
	for (arra = 0; arra < 256; arra++)
	{
		simpleArrayU[0][arra] = -0.391*(arra - 128);//G
		simpleArrayU[1][arra] = 2.018*(arra - 128);//B
		simpleArrayV[0][arra] = -0.813*(arra - 128);//G
		simpleArrayV[1][arra] = 1.596*(arra - 128);//R
		simpleArrayY[0][arra] = 1.164*(arra - 16);//Y
												  //cout << "rr:" << simpleArrayY[arra] <<  endl;
												  //simpleError[0][arra] = arra * arra;
	}
	double left_top, right_top, left_bottom, right_bottom;
	for (y = 0; y < height; y += 2)
	{
		xx = 0;
		for (x = 0; x < width; x += 2)
		{
			left_top = U_plane.at<double>(mirror(height, y), mirror(width, x));
			right_top = U_plane.at<double>(mirror(height, y), mirror(width, x + 1));
			left_bottom = U_plane.at<double>(mirror(height, y + 1), mirror(width, x));
			right_bottom = U_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1));
			DS_U_plane.at<double>(yy, xx) = (left_top + right_top + left_bottom + right_bottom) / 4;

			left_top = V_plane.at<double>(mirror(height, y), mirror(width, x));
			right_top = V_plane.at<double>(mirror(height, y), mirror(width, x + 1));
			left_bottom = V_plane.at<double>(mirror(height, y + 1), mirror(width, x));
			right_bottom = V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1));
			DS_V_plane.at<double>(yy, xx) = (left_top + right_top + left_bottom + right_bottom) / 4;
			xx++;
		}
		yy++;
	}


	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);

	yy = 0;
	for (y = 0; y < height; y += 2)
	{
		xx = 0;
		for (x = 0; x < width; x += 2)
		{
			R = (int)BGR.at<Vec3b>(y, x + 1)[2];
			GleftTop = (int)BGR.at<Vec3b>(y, x)[1];
			GrightDown = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
			B = (int)BGR.at<Vec3b>(y + 1, x)[0];

			U1 = U_plane.at<double>(y, x);
			U2 = U_plane.at<double>(y, x + 1);
			U3 = U_plane.at<double>(y + 1, x);
			U4 = U_plane.at<double>(y + 1, x + 1);

			V1 = V_plane.at<double>(y, x);
			V2 = V_plane.at<double>(y, x + 1);
			V3 = V_plane.at<double>(y + 1, x);
			V4 = V_plane.at<double>(y + 1, x + 1);

			Umin = Umax = U1;
			if (U2 < Umin)
				Umin = U2;
			if (U3 < Umin)
				Umin = U3;
			if (U4 < Umin)
				Umin = U4;

			if (U2 > Umax)
				Umax = U2;
			if (U3 > Umax)
				Umax = U3;
			if (U4 > Umax)
				Umax = U4;
			Vmin = Vmax = V1;
			if (V2 < Vmin)
				Vmin = V2;
			if (V3 < Vmin)
				Vmin = V3;
			if (V4 < Vmin)
				Vmin = V4;

			if (V2 > Vmax)
				Vmax = V2;
			if (V3 > Vmax)
				Vmax = V3;
			if (V4 > Vmax)
				Vmax = V4;
			UminInteger = (int)floor(Umin);
			UmaxInteger = (int)ceil(Umax);

			VminInteger = (int)floor(Vmin);
			VmaxInteger = (int)ceil(Vmax);

			Yb = (int)(Y_plane.at<double>(y + 1, x) + 0.5);//B pixel's Y
			Yr = (int)(Y_plane.at<double>(y, x + 1) + 0.5);//R pixel's Y
			YleftTop = (int)(Y_plane.at<double>(y, x) + 0.5);
			YrightDown = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);

			simpleYb = simpleArrayY[0][Yb];
			simpleYr = simpleArrayY[0][Yr];
			simpleYleftTop = simpleArrayY[0][YleftTop];//1.164 * (YleftTop - 16);
			simpleYrightDown = simpleArrayY[0][YrightDown];//1.164 * (YrightDown - 16);

														   //fr2 << "y:" << y << " x:" << x << " U:" << Ub << " V:" << Vr << " " << all[0] << "~" << all[5] << " " << all2[0] << "~" << all2[5] << endl;
			int error_TH = INT_MAX;
			//error = 0;
			//roun = (UmaxInteger - UminInteger + 1)*(VmaxInteger - VminInteger + 1);
			/*if (roun>300 && roun<400)
			cout << y << " " << x << endl;*/
			double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
			double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;
			//fr2 << roun << endl;
			//fr2 << roun<< endl;
			//fr2 << "y:" << y << " x:" << x << " " << (all[5] - all[0] + 1) << " " << (all2[5] - all2[0] + 1) << endl;
			//mean_range += (UmaxInteger - UminInteger + 1)*(VmaxInteger - VminInteger + 1);
			if (UmaxInteger - UminInteger >= VmaxInteger - VminInteger)
			{
				for (move_U = UminInteger; move_U <= UmaxInteger; move_U++)
				{

					theLeftDownU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(height / 2, yy + 1), mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(height / 2, yy + 1), xx)) / 16;
					//fr2 << move_U << ":  ";
					rec_B = (int)(simpleYb + 2.018 *(theLeftDownU - 128) + 0.5);
					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;
					//simpleU = simpleArrayU[0][move_U];//-0.391 * (move_U - 128);
					for (move_V = VminInteger; move_V <= VmaxInteger; move_V++)
					{

						theLeftTopU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(height / 2, yy - 1), mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(height / 2, yy - 1), xx)) / 16;
						theLeftTopV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(height / 2, yy - 1), mirror(width / 2, xx - 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(width / 2, xx - 1)) + 3.0*DS_V_plane.at<double>(mirror(height / 2, yy - 1), xx)) / 16;

						rec_G1 = (int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5);//(int)(simpleYleftTop + simpleU - 0.813*(move_V - 128) + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;

						theRightDownU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(height / 2, yy + 1), mirror(width / 2, xx + 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(width / 2, xx + 1)) + 3.0*DS_U_plane.at<double>(mirror(height / 2, yy + 1), xx)) / 16;
						theRightDownV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(height / 2, yy + 1), mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(height / 2, yy + 1), xx)) / 16;



						rec_G2 = (int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;

						theRightTopV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(height / 2, yy - 1), mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(height / 2, yy - 1), xx)) / 16;


						rec_R = (int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5);//(int)(simpleYr + 1.596*(move_V - 128) + 0.5);
						if (rec_R < 0) rec_R = 0;
						else if (rec_R > 255) rec_R = 255;



						//error = simpleError[0][B - rec_B + 255] + simpleError[0][GleftTop - rec_G1 + 255] + simpleError[0][GrightDown - rec_G2 + 255] + simpleError[0][R - rec_R + 255];
						error = (B - rec_B)*(B - rec_B) + (GleftTop - rec_G1)*(GleftTop - rec_G1) + (GrightDown - rec_G2)*(GrightDown - rec_G2) + (R - rec_R)*(R - rec_R);
						if (error < error_TH)
						{
							error_TH = error;
							DS_U_plane.at<double>(yy, xx) = (uchar)move_U;
							DS_V_plane.at<double>(yy, xx) = (uchar)move_V;
						}

						//	fr2 << "U:" << move_U << " V:" << move_V<<" " << error << endl;
					}
					//if (y == 122 && x == 306)
					//	fr2 << endl;
				}
				/*if (y == chooseY && x == chooseX)
				cout << "U:" << DS_U_plane.at<double>(yy, xx) << " V:" << DS_V_plane.at<double>(yy, xx) << " error:" << error << endl;*/
			}
			else {
				for (move_V = VminInteger; move_V <= VmaxInteger; move_V++)//move_V = all2[0]; move_V <= all2[5]; move_V++
				{
					//fr2 << move_U << ":  ";
					theRightTopV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(height / 2, yy - 1), mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(height / 2, yy - 1), xx)) / 16;


					rec_R = (int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5);//(int)(simpleYr + 1.596*(move_V - 128) + 0.5);
					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;
					simpleV = simpleArrayV[0][move_V];
					for (move_U = UminInteger; move_U <= UmaxInteger; move_U++)
					{
						theLeftDownU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(height / 2, yy + 1), mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(height / 2, yy + 1), xx)) / 16;


						rec_B = (int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5);
						if (rec_B < 0) rec_B = 0;
						else if (rec_B > 255) rec_B = 255;

						theLeftTopU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(height / 2, yy - 1), mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(width / 2, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(height / 2, yy - 1), xx)) / 16;
						theLeftTopV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(height / 2, yy - 1), mirror(width / 2, xx - 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(width / 2, xx - 1)) + 3.0*DS_V_plane.at<double>(mirror(height / 2, yy - 1), xx)) / 16;

						rec_G1 = (int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;

						theRightDownU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(height / 2, yy + 1), mirror(width / 2, xx + 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(width / 2, xx + 1)) + 3.0*DS_U_plane.at<double>(mirror(height / 2, yy + 1), xx)) / 16;
						theRightDownV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(height / 2, yy + 1), mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(width / 2, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(height / 2, yy + 1), xx)) / 16;

						rec_G2 = (int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;


						//error = simpleError[0][B - rec_B + 255] + simpleError[0][GleftTop - rec_G1 + 255] + simpleError[0][GrightDown - rec_G2 + 255] + simpleError[0][R - rec_R + 255];

						error = (B - rec_B)*(B - rec_B) + (GleftTop - rec_G1)*(GleftTop - rec_G1) + (GrightDown - rec_G2)*(GrightDown - rec_G2) + (R - rec_R)*(R - rec_R);
						if (error < error_TH)
						{
							error_TH = error;
							DS_U_plane.at<double>(yy, xx) = (uchar)move_U;
							DS_V_plane.at<double>(yy, xx) = (uchar)move_V;
						}

						//fr2 << "U:" << move_U << " V:" << move_V<<" " << error << endl;
					}
					//fr2 << endl;
				}
			}
			//if (roun>10)
			//	totalRoun += roun;
			//frY << (int)round(DS_U_plane.at<double>(yy, xx)) << endl;
			//frYx << (int)round(DS_V_plane.at<double>(yy, xx)) << endl;
			xx++;
		}
		yy++;
	}
	//cout << (double)mean_range / ((double)(height*width) / (4.0)) << endl;
	//cout << (double)totalRoun / ((double)(height*width) / (4.0)) << endl;
	//frY.close();
	//frYx.close();
	//frY.close();
	//fr2.close();
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));
}

void DownSample_420GDCS_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
	double B_pow_a = pow(2.018, 2);
	double G_pow_a = pow(-0.391, 2);
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = pow(-0.813, 2);
	double R_pow_b = pow(1.596, 2);

	double B_mul_ab = 0;
	double G_mul_ab = (-0.391)*(-0.813);
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a + G_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b + G_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab + G_mul_ab;
	double pow_of_SumofMul_ab = pow(sum_of_mul_ab, 2);

	double C = (sum_of_pow_a*sum_of_pow_b) - pow_of_SumofMul_ab;
	double D = pow_of_SumofMul_ab - (sum_of_pow_a*sum_of_pow_b);

	int M0, M1, M2, M3;
	double rec_M0, rec_M1, rec_M2, rec_M3;
	int Y0, Y1, Y2, Y3;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	/*double last_error;
	int same_num;*/
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double AA, BB;
	//int w = width / 2;
	//int h = height / 2;


	double simpleArrayY[1][256];

	for (int arra = 0; arra < 256; arra++)
	{

		simpleArrayY[0][arra] = 1.164*(arra - 16);
	}


	int xx;
	int yy = 0;
	/*420Lin*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
				+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
				+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
				+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));

			BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
				+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
				+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
				+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b*AA - sum_of_mul_ab * BB) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a*BB - sum_of_mul_ab * AA) / C, 0, 255);
			//DS_V_plane.at<double>(yy, xx) = Clip((((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / D), 0, 255);



			xx++;
		}
		yy++;
	}
	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);

	/*420GDCS*/
	double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
	double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{


			des_U = DS_U_plane.at<double>(yy, xx);
			des_V = DS_V_plane.at<double>(yy, xx);


			/*梯度下降*/


			M0 = (int)BGR.at<Vec3b>(y, x)[1];
			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];


			Y0 = (int)(Y_plane.at<double>(y, x) + 0.5);
			Y1 = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
			Y2 = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
			Y3 = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
			simpleYleftTop = simpleArrayY[0][Y0];
			simpleYr = simpleArrayY[0][Y1];
			simpleYb = simpleArrayY[0][Y2];
			simpleYrightDown = simpleArrayY[0][Y3];



			tmp_U = des_U;
			tmp_V = des_V;

			theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;


			rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
			rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
			rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
			rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
			error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



			if (error == 0) {

				DS_U_plane.at<double>(yy, xx) = des_U;
				DS_V_plane.at<double>(yy, xx) = des_V;

				xx++;
				continue;

			}



			error_TH = INT_MAX;
			for (int i = -1; i <= 1; i++) {
				tmp_V = des_V - i;


				if (0 <= tmp_V && tmp_V <= 255) {
					theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
					theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
					theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;

					rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R

					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {

							theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
							theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
							theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;


							rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
							rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
							rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
							error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);


							if (error < error_TH)
							{
								error_TH = error;
								n_i = i;
								n_j = j;
								can_U = tmp_U;
								can_V = tmp_V;
							}

						}

					}
				}

			}

			des_U = can_U;
			des_V = can_V;


			/*	last_error = error_TH;
			same_num = 0;*/

			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
						theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
						theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;

						rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {
									theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
									theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
									theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;


									rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
									rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
									rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
									error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



									if (error < error_TH)
									{
										error_TH = error;
										tmp_i = i;
										tmp_j = j;
										can_U = tmp_U;
										can_V = tmp_V;
									}
								}


							}
						}
					}

				}
				n_i = tmp_i;
				n_j = tmp_j;
				des_U = can_U;
				des_V = can_V;

				/*if (last_error == error_TH)
				same_num++;
				else
				same_num = 0;
				last_error = error_TH;*/

			}




			DS_U_plane.at<double>(yy, xx) = des_U;
			DS_V_plane.at<double>(yy, xx) = des_V;




			xx++;
		}

		yy++;


	}
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));


}

/*2019/9/18*/
//void DownSample_420GDCS_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
//{
//
//
//	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
//
//
//	double B_pow_a = pow(2.018, 2);
//	double G_pow_a = pow(-0.391, 2);
//	double R_pow_a = 0;
//
//	double B_pow_b = 0;
//	double G_pow_b = pow(-0.813, 2);
//	double R_pow_b = pow(1.596, 2);
//
//	double B_mul_ab = 0;
//	double G_mul_ab = (-0.391)*(-0.813);
//	double R_mul_ab = 0;
//
//	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a + G_pow_a;
//	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b + G_pow_b;
//	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab + G_mul_ab;
//	double pow_of_SumofMul_ab = pow(sum_of_mul_ab, 2);
//
//	double C = (sum_of_pow_a*sum_of_pow_b) - pow_of_SumofMul_ab;
//	double D = pow_of_SumofMul_ab - (sum_of_pow_a*sum_of_pow_b);
//
//	int M0, M1, M2, M3;
//	double rec_M0, rec_M1, rec_M2, rec_M3;
//	int Y0, Y1, Y2, Y3;
//	int des_U, des_V;
//	int tmp_U, tmp_V;
//	double error;
//	/*double last_error;
//	int same_num;*/
//	int n_i, n_j;
//	int old_i, old_j;
//	int tmp_i, tmp_j;
//	double error_TH;
//	int can_U, can_V;
//	double AA, BB;
//	int w = width / 2;
//	int h =height/2;
//
//
//	double simpleArrayY[1][256];
//
//	for (int arra = 0; arra < 256; arra++)
//	{
//
//		simpleArrayY[0][arra] = 1.164*(arra - 16);
//	}
//
//
//	int xx;
//	int yy = 0;
//	/*420Lin*/
//	for (int y = 0; y < height; y += 2)
//	{
//		xx = 0;
//		for (int x = 0; x < width; x += 2)
//		{
//			AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
//				+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
//				+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
//				+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));
//
//			BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
//				+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
//				+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
//				+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));
//
//			DS_U_plane.at<double>(yy, xx) = Clip((int)(((sum_of_pow_b*AA) - (sum_of_mul_ab*BB)) / C + 0.5), 0, 255);
//			DS_V_plane.at<double>(yy, xx) = Clip((int)(((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / D + 0.5), 0, 255);
//
//
//			xx++;
//		}
//		yy++;
//	}
//
//
//	/*420GDCS*/
//	double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
//	double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;
//	yy = 0;
//	for (int y = 0; y < height; y += 2)
//	{
//		xx = 0;
//		for (int x = 0; x < width; x += 2)
//		{
//
//			///*CH方法計算UV*/
//			//AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
//			//	+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
//			//	+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
//			//	+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));
//
//			//BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
//			//	+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
//			//	+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
//			//	+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));
//
//			des_U = DS_U_plane.at<double>(yy, xx);
//			des_V = DS_V_plane.at<double>(yy, xx);
//
//
//			/*梯度下降*/
//
//
//			M0 = (int)BGR.at<Vec3b>(y, x)[1];
//			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
//			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
//			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
//
//
//			Y0 = (int)(Y_plane.at<double>(y, x) + 0.5);
//			Y1 = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
//			Y2 = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
//			Y3 = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
//			simpleYleftTop = simpleArrayY[0][Y0];
//			simpleYr = simpleArrayY[0][Y1];
//			simpleYb = simpleArrayY[0][Y2];
//			simpleYrightDown = simpleArrayY[0][Y3];
//
//
//
//			tmp_U = des_U;
//			tmp_V = des_V;
//
//			theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//			theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//			theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//			theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//			theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//			theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//
//
//			rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
//			rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
//			rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
//			rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
//			error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);
//
//
//
//			if (error == 0) {
//
//				DS_U_plane.at<double>(yy, xx) = des_U;
//				DS_V_plane.at<double>(yy, xx) = des_V;
//
//				xx++;
//				continue;
//
//			}
//
//
//
//			error_TH = INT_MAX;
//			for (int i = -1; i <= 1; i++) {
//				tmp_V = des_V - i;
//
//
//				if (0 <= tmp_V && tmp_V <= 255) {
//					theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//					theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//					theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//
//					rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
//
//					for (int j = -1; j <= 1; j++) {
//
//						tmp_U = des_U + j;
//
//						if (0 <= tmp_U && tmp_U <= 255) {
//							
//							theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//							theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//							theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//							
//
//							rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
//							rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
//							rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
//							error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);
//
//
//							if (error < error_TH)
//							{
//								error_TH = error;
//								n_i = i;
//								n_j = j;
//								can_U = tmp_U;
//								can_V = tmp_V;
//							}
//
//						}
//
//					}
//				}
//
//			}
//
//			des_U = can_U;
//			des_V = can_V;
//
//
//			/*	last_error = error_TH;
//			same_num = 0;*/
//
//			while ((n_i != 0 || n_j != 0) && error_TH != 0) {
//
//
//				tmp_i = 0;
//				tmp_j = 0;
//				for (int i = -1; i <= 1; i++) {
//					tmp_V = des_V - i;
//					if (0 <= tmp_V && tmp_V <= 255) {
//						theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//						theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//						theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//
//						rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
//						for (int j = -1; j <= 1; j++) {
//							old_i = i + n_i;
//							old_j = j + n_j;
//
//							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {
//
//								tmp_U = des_U + j;
//
//								if (0 <= tmp_U && tmp_U <= 255) {
//									theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
//									theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//									theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
//									
//
//									rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
//									rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
//									rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
//									error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);
//
//
//
//									if (error < error_TH)
//									{
//										error_TH = error;
//										tmp_i = i;
//										tmp_j = j;
//										can_U = tmp_U;
//										can_V = tmp_V;
//									}
//								}
//
//
//							}
//						}
//					}
//
//				}
//				n_i = tmp_i;
//				n_j = tmp_j;
//				des_U = can_U;
//				des_V = can_V;
//
//				/*if (last_error == error_TH)
//				same_num++;
//				else
//				same_num = 0;
//				last_error = error_TH;*/
//
//			}
//
//
//
//
//			DS_U_plane.at<double>(yy, xx) = des_U;
//			DS_V_plane.at<double>(yy, xx) = des_V;
//
//
//
//
//			xx++;
//		}
//
//		yy++;
//
//
//	}
//
//}


void DownSample_420GBCS_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


	int sortI, sortJ;
	int yy = 0;
	int xx = 0;

	/*char filenameY2[] = "rangetest.txt";
	fstream fr2;
	fr2.open(filee[0][number], ios::out);*/
	/*char filenameU[] = "Tree_rate0102.txt";
	fstream frYY;
	frYY.open(filenameU, ios::out);*/
	/*char filenameV[] = "Tree_V0102.txt";
	fstream fpZZZ;
	fpZZZ.open(filenameV, ios::out);*/
	double B_pow_a = pow(2.018, 2);
	double G_pow_a = pow(-0.391, 2);
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = pow(-0.813, 2);
	double R_pow_b = pow(1.596, 2);

	double B_mul_ab = 0;
	double G_mul_ab = (-0.391)*(-0.813);
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a + G_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b + G_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab + G_mul_ab;
	double pow_of_SumofMul_ab = pow(sum_of_mul_ab, 2);

	double C = (sum_of_pow_a*sum_of_pow_b) - pow_of_SumofMul_ab;
	double D = pow_of_SumofMul_ab - (sum_of_pow_a*sum_of_pow_b);
	double AA, BB;
	int R, GleftTop, GrightDown, B, Yb, Yr, YleftTop, YrightDown;
	//int G11, G12, B1, R1, G21, G22, B2, R2, G31, G32, B3, R3;//0922
	int y, x;
	double Ub, Vr, Ug_leftTop, Ug_rightDown, Vg_leftTop, Vg_rightDown;
	int UbFloor, UbCeil, VbFloor, VbCeil, Ug_leftTopFloor, Ug_leftTopCeil, Ug_rightDownFloor, Ug_rightDownCeil, Vg_leftTopFloor, Vg_leftTopCeil, Vg_rightDownFloor, Vg_rightDownCeil;
	int rec_B, rec_G1, rec_G2, rec_R;
	int rec_B1, rec_G1_11, rec_G1_12, rec_R1, rec_B2, rec_G2_21, rec_G2_22, rec_R2, rec_B3, rec_G3_31, rec_G3_32, rec_R3;//0922
	//int YG11, YG12, YB1, YR1, YG21, YG22, YB2, YR2, YG31, YG32, YB3, YR3;//0922
	int rec1, rec2, rec3, rec4, rec5, rec6, rec7, rec8;
	int move_U, move_V;
	//int roun, totalRoun = 0;
	double error;
	double Ymult = 18.624, Vmult = 104.064, Umult = 50.048;//Ymult(1.164*16) Vmult(0.813*128) Umult(0.391*128)
	double simpleU, simpleV;
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
	double simpleArrayU[2][256], simpleArrayV[2][256], simpleArrayY[1][256];
	//int simpleError[1][511];//----------------------------------------------0224還未做
	int arra;
	for (arra = 0; arra < 256; arra++)
	{
		simpleArrayU[0][arra] = -0.391*(arra - 128);//G
		simpleArrayU[1][arra] = 2.018*(arra - 128);//B
		simpleArrayV[0][arra] = -0.813*(arra - 128);//G
		simpleArrayV[1][arra] = 1.596*(arra - 128);//R
		simpleArrayY[0][arra] = 1.164*(arra - 16);//Y
		//cout << "rr:" << simpleArrayY[arra] <<  endl;
		//simpleError[0][arra] = arra * arra;
	}
	/*for ( arra = 0; arra < 511; arra++)
	{
	simpleError[0][arra] = (arra-255) * (arra-255);
	}*/

	//cout << "rr:" << simpleArrayY[0] << " " << simpleArrayY[1] << " " << simpleArrayY[2] << endl;
	//double left_top, right_top, left_bottom, right_bottom;
	double LTU, RTU, LDU, RDU, LTV, RTV, LDV, RDV;

	/*420Lin*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
				+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
				+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
				+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));

			BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
				+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
				+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
				+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));

			DS_U_plane.at<double>(yy, xx) = Clip((int)(((sum_of_pow_b*AA) - (sum_of_mul_ab*BB)) / C + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / D + 0.5), 0, 255);


			xx++;
		}
		yy++;
	}
	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	yy = 1;
	for (y = 0; y < height; y += 2)
	{
		xx = 1;
		for (x = 0; x < width; x += 2)
		{
			R = (int)BGR.at<Vec3b>(y, x + 1)[2];
			GleftTop = (int)BGR.at<Vec3b>(y, x)[1];
			GrightDown = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
			B = (int)BGR.at<Vec3b>(y + 1, x)[0];

			Yb = (int)(Y_plane.at<double>(y + 1, x) + 0.5);//B pixel's Y
			Yr = (int)(Y_plane.at<double>(y, x + 1) + 0.5);//R pixel's Y
			YleftTop = (int)(Y_plane.at<double>(y, x) + 0.5);
			YrightDown = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
			//YG11, YG12, YB1, YR1, YG21, YG22, YB2, YR2, YG31, YG32, YB3, YR3;

			simpleYb = simpleArrayY[0][Yb];
			simpleYr = simpleArrayY[0][Yr];
			simpleYleftTop = simpleArrayY[0][YleftTop];//1.164 * (YleftTop - 16);
			simpleYrightDown = simpleArrayY[0][YrightDown];//1.164 * (YrightDown - 16);

			LDU = DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx);
			LTU = DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx);
			RDU = DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx);
			LTV = DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx);
			RDV = DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx);
			RTV = DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx);

			//theLeftDownU = ((double)(move_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
			Ub = ((B - (Yb * 1.164 - Ymult)) / 2.018) + 128;
			Ub = (Ub * 16 - LDU) / 9;
			if (Ub < 0) Ub = 0;
			else if (Ub > 255) Ub = 255;
			UbFloor = (int)floor(Ub);
			UbCeil = (int)ceil(Ub);
			/*---------------------------------------------------------------------------------------*/
			//theRightTopV = ((double)(move_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
			Vr = ((R - (Yr * 1.164 - Ymult)) / 1.596) + 128;
			Vr = (Vr * 16 - RTV) / 9;
			if (Vr < 0) Vr = 0;
			else if (Vr > 255) Vr = 255;
			VbFloor = (int)floor(Vr);
			VbCeil = (int)ceil(Vr);
			/*-------------------------------------*/
			Ug_leftTop = (GleftTop - 1.164*(YleftTop)+Ymult + 0.813*(Vr)-Vmult) / (-0.391) + 128;// 0.813*(VleftTop - 128))
			Ug_leftTop = (Ug_leftTop * 16 - LTU) / 9;
			if (Ug_leftTop < 0) Ug_leftTop = 0;
			else if (Ug_leftTop > 255) Ug_leftTop = 255;
			Ug_leftTopFloor = (int)floor(Ug_leftTop);
			Ug_leftTopCeil = (int)ceil(Ug_leftTop);
			/*-------------------------------------*/
			Ug_rightDown = (GrightDown - 1.164*(YrightDown)+Ymult + 0.813*(Vr)-Vmult) / (-0.391) + 128;
			Ug_rightDown = ((Ug_rightDown * 16) - RDU) / 9;
			if (Ug_rightDown < 0) Ug_rightDown = 0;
			else if (Ug_rightDown > 255) Ug_rightDown = 255;
			Ug_rightDownFloor = (int)floor(Ug_rightDown);
			Ug_rightDownCeil = (int)ceil(Ug_rightDown);

			/*-------------------------------------*/
			Vg_leftTop = (GleftTop - 1.164*(YleftTop)+Ymult + 0.391*(Ub)-Umult) / (-0.813) + 128;
			Vg_leftTop = (Vg_leftTop * 16 - LTV) / 9;
			if (Vg_leftTop < 0) Vg_leftTop = 0;
			else if (Vg_leftTop > 255) Vg_leftTop = 255;
			Vg_leftTopFloor = (int)floor(Vg_leftTop);
			Vg_leftTopCeil = (int)ceil(Vg_leftTop);
			/*-------------------------------------*/
			Vg_rightDown = (GrightDown - 1.164*(YrightDown)+Ymult + 0.391*(Ub)-Umult) / (-0.813) + 128;
			Vg_rightDown = (Vg_rightDown * 16 - RDV) / 9;
			if (Vg_rightDown < 0) Vg_rightDown = 0;
			else if (Vg_rightDown > 255) Vg_rightDown = 255;
			Vg_rightDownFloor = (int)floor(Vg_rightDown);
			Vg_rightDownCeil = (int)ceil(Vg_rightDown);

			int all[6] = { UbFloor, UbCeil, Ug_leftTopFloor, Ug_leftTopCeil, Ug_rightDownFloor, Ug_rightDownCeil };
			//sort(all, all + 6);
			for (sortI = 0; sortI < 5; sortI++)
			{
				for (sortJ = 0; sortJ < 5 - sortI; sortJ++)
				{
					if (all[sortJ + 1] < all[sortJ])
						swap(all[sortJ + 1], all[sortJ]);
				}
			}
			int all2[6] = { VbFloor, VbCeil, Vg_leftTopFloor, Vg_leftTopCeil, Vg_rightDownFloor, Vg_rightDownCeil };
			//sort(all2, all2 + 6);
			for (sortI = 0; sortI < 5; sortI++)
			{
				for (sortJ = 0; sortJ < 5 - sortI; sortJ++)
				{
					if (all2[sortJ + 1] < all2[sortJ])
						swap(all2[sortJ + 1], all2[sortJ]);
				}
			}
			//fr2 << "y:" << y << " x:" << x << " U:" << Ub << " V:" << Vr << " " << all[0] << "~" << all[5] << " " << all2[0] << "~" << all2[5] << endl;
			double error_TH = INT_MAX;
			//error = 0;

			//roun = (all[5] - all[0] + 1)*(all2[5] - all2[0] + 1);
			//cout << "[" << all[5] << ", " << all[0] << "]x[" << all2[5] << ", " << all2[0] << "] = " << roun << endl;
			//fr2 << roun << endl;
			//fr2 << roun<< endl;
			//left top //0922

			double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
			double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;
			if (all[5] - all[0] >= all2[5] - all2[0])//all[5] - all[0] >= all2[5] - all2[0]
			{
				for (move_U = all[0]; move_U <= all[5]; move_U++)
				{
					//fr2 << move_U << ":  ";
					theLeftDownU = ((double)(move_U * 9) + LDU) / 16;

					rec_B = (int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5);
					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;
					//simpleU = simpleArrayU[0][move_U];//-0.391 * (move_U - 128);
					for (move_V = all2[0]; move_V <= all2[5]; move_V++)
					{
						theLeftTopU = ((double)(move_U * 9) + LTU) / 16;
						theLeftTopV = ((double)(move_V * 9) + LTV) / 16;
						rec_G1 = (int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5);//(int)(simpleYleftTop + simpleU - 0.813*(move_V - 128) + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;

						theRightDownU = ((double)(move_U * 9) + RDU) / 16;
						theRightDownV = ((double)(move_V * 9) + RDV) / 16;


						rec_G2 = (int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;

						theRightTopV = ((double)(move_V * 9) + RTV) / 16;

						rec_R = (int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5);//(int)(simpleYr + 1.596*(move_V - 128) + 0.5);
						if (rec_R < 0) rec_R = 0;
						else if (rec_R > 255) rec_R = 255;

						//error = simpleError[0][B - rec_B + 255] + simpleError[0][GleftTop - rec_G1 + 255] + simpleError[0][GrightDown - rec_G2 + 255] + simpleError[0][R - rec_R + 255];

						/*error = (rec1 - (int)BGR.at<Vec3b>(y - 1, x - 1)[1])*(rec1 - (int)BGR.at<Vec3b>(y - 1, x - 1)[1]) + (rec2 - (int)BGR.at<Vec3b>(y - 1, x)[0])*(rec2 - (int)BGR.at<Vec3b>(y - 1, x)[0])
						+ (rec4 - (int)BGR.at<Vec3b>(y, x - 1)[2])*(rec4 - (int)BGR.at<Vec3b>(y, x - 1)[2])
						+ 7*(GleftTop - rec_G1)*(GleftTop - rec_G1);*/
						error = (B - rec_B)*(B - rec_B) + (GleftTop - rec_G1)*(GleftTop - rec_G1) + (GrightDown - rec_G2)*(GrightDown - rec_G2) + (R - rec_R)*(R - rec_R);
						/*error = (rec1 - (int)BGR.at<Vec3b>(y - 1, x - 1)[1])*(rec1 - (int)BGR.at<Vec3b>(y - 1, x - 1)[1]) + (rec2 - (int)BGR.at<Vec3b>(y - 1, x)[0])*(rec2 - (int)BGR.at<Vec3b>(y - 1, x)[0])
						+ (rec3 - (int)BGR.at<Vec3b>(y - 1, x + 1)[1])*(rec3 - (int)BGR.at<Vec3b>(y - 1, x + 1)[1]) + (rec4 - (int)BGR.at<Vec3b>(y, x - 1)[2])*(rec4 - (int)BGR.at<Vec3b>(y, x - 1)[2])
						+ (rec6 - (int)BGR.at<Vec3b>(y + 1, x - 1)[1])*(rec6 - (int)BGR.at<Vec3b>(y + 1, x - 1)[1])
						+ 5 * (B - rec_B)*(B - rec_B) + 7 * (GleftTop - rec_G1)*(GleftTop - rec_G1) + 3 * (GrightDown - rec_G2)*(GrightDown - rec_G2) + 3 * (R - rec_R)*(R - rec_R);*/
						if (error < error_TH)
						{
							error_TH = error;
							DS_U_plane.at<double>(yy, xx) = (uchar)move_U;
							DS_V_plane.at<double>(yy, xx) = (uchar)move_V;
						}

						//fr2 << "U:" << move_U << " V:" << move_V<<" " << error << endl;
					}
					//fr2 << endl;
				}
			}
			else {
				for (move_V = all2[0]; move_V <= all2[5]; move_V++)//move_V = all2[0]; move_V <= all2[5]; move_V++
				{
					//fr2 << move_U << ":  ";
					theRightTopV = ((double)(move_V * 9) + RTV) / 16;

					rec_R = (int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5);//(int)(simpleYr + 1.596*(move_V - 128) + 0.5);
					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;

					for (move_U = all[0]; move_U <= all[5]; move_U++)
					{
						theLeftDownU = ((double)(move_U * 9) + LDU) / 16;

						rec_B = (int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5);
						if (rec_B < 0) rec_B = 0;
						else if (rec_B > 255) rec_B = 255;

						theLeftTopU = ((double)(move_U * 9) + LTU) / 16;
						theLeftTopV = ((double)(move_V * 9) + LTV) / 16;
						rec_G1 = (int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5);//(int)(simpleYleftTop + simpleU - 0.813*(move_V - 128) + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;

						theRightDownU = ((double)(move_U * 9) + RDU) / 16;
						theRightDownV = ((double)(move_V * 9) + RDV) / 16;

						rec_G2 = (int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;
						//error = simpleError[0][B - rec_B + 255] + simpleError[0][GleftTop - rec_G1 + 255] + simpleError[0][GrightDown - rec_G2 + 255] + simpleError[0][R - rec_R + 255];

						error = (B - rec_B)*(B - rec_B) + (GleftTop - rec_G1)*(GleftTop - rec_G1) + (GrightDown - rec_G2)*(GrightDown - rec_G2) + (R - rec_R)*(R - rec_R);
						if (error < error_TH)
						{
							error_TH = error;
							DS_U_plane.at<double>(yy, xx) = (uchar)move_U;
							DS_V_plane.at<double>(yy, xx) = (uchar)move_V;
						}

						//fr2 << "U:" << move_U << " V:" << move_V<<" " << error << endl;
					}
					//fr2 << endl;
				}
			}
			//frYY << error_TH << endl;
			//fpZZZ << (int)round(DS_V_plane.at<double>(yy, xx)) << endl;
			xx++;
			//totalRoun += roun;
		}
		yy++;
	}
	//cout << (double)totalRoun / ((double)(height*width) / (4.0)) << endl;//計算面積
	//frY.close();
	//frYx.close();
	//frY.close();
	//fr2.close();
	//frYY.close();
	//fpZZZ.close();

	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));
}





void DownSample_420ES_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;


	double B_pow_a = pow(2.018, 2);
	double G_pow_a = pow(-0.391, 2);
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = pow(-0.813, 2);
	double R_pow_b = pow(1.596, 2);

	double B_mul_ab = 0;
	double G_mul_ab = (-0.391)*(-0.813);
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a + G_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b + G_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab + G_mul_ab;
	double pow_of_SumofMul_ab = pow(sum_of_mul_ab, 2);

	double C = (sum_of_pow_a*sum_of_pow_b) - pow_of_SumofMul_ab;
	double D = pow_of_SumofMul_ab - (sum_of_pow_a*sum_of_pow_b);

	int M0, M1, M2, M3;
	double rec_M0, rec_M1, rec_M2, rec_M3;
	int Y0, Y1, Y2, Y3;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	/*double last_error;
	int same_num;*/
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double AA, BB;
	int w = width / 2;
	int h = height / 2;


	double simpleArrayY[1][256];

	for (int arra = 0; arra < 256; arra++)
	{

		simpleArrayY[0][arra] = 1.164*(arra - 16);
	}


	int xx;
	int yy = 0;
	/*420Lin*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
				+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
				+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
				+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));

			BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
				+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
				+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
				+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));

			DS_U_plane.at<double>(yy, xx) = Clip((int)(((sum_of_pow_b*AA) - (sum_of_mul_ab*BB)) / C + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / D + 0.5), 0, 255);



			xx++;
		}
		yy++;
	}
	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	/*420GDCS*/
	double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
	double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			/*CH方法計算UV*/
			AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
				+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
				+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
				+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));

			BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
				+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
				+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
				+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));

			des_U = (int)(((sum_of_pow_b*AA) - (sum_of_mul_ab*BB)) / C + 0.5);
			des_V = (int)(((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / D + 0.5);


			/*梯度下降*/


			M0 = (int)BGR.at<Vec3b>(y, x)[1];
			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];


			Y0 = (int)(Y_plane.at<double>(y, x) + 0.5);
			Y1 = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
			Y2 = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
			Y3 = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
			simpleYleftTop = simpleArrayY[0][Y0];
			simpleYr = simpleArrayY[0][Y1];
			simpleYb = simpleArrayY[0][Y2];
			simpleYrightDown = simpleArrayY[0][Y3];
			error_TH = INT_MAX;
			for (int u = 0; u <= 255; u++) {
				for (int v = 0; v <= 255; v++) {
					tmp_U = u;
					tmp_V = v;

					theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
					theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
					theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy - 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy - 1), xx)) / 16;
					theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx - 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
					theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_U_plane.at<double>(mirror(h, yy + 1), xx)) / 16;
					theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(mirror(h, yy + 1), mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(yy, mirror(w, xx + 1)) + 3.0*DS_V_plane.at<double>(mirror(h, yy + 1), xx)) / 16;


					rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
					rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
					rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
					rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
					error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);

					if (error < error_TH) {
						error_TH = error;
						can_U = tmp_U;
						can_V = tmp_V;
					}
				}
			}




			DS_U_plane.at<double>(yy, xx) = can_U;
			DS_V_plane.at<double>(yy, xx) = can_V;




			xx++;
		}

		yy++;


	}
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));


}




void DownSample_Bayer_420IJCSU_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


	double R_pow_a = 0;
	double G_pow_a = 0.391*0.391;
	double B_pow_a = 2.018*2.018;

	double R_pow_b = 1.596*1.596;
	double G_pow_b = 0.813*0.813;
	double B_pow_b = 0;

	double B_mul_ab = 0;
	double G_mul_ab = 0.391*0.813;
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a + G_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b + G_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab + G_mul_ab;
	double C = sum_of_mul_ab * sum_of_mul_ab - (sum_of_pow_a*sum_of_pow_b);
	double AA, BB;
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
	int M0, M1, M2, M3;
	double rec_M0, rec_M1, rec_M2, rec_M3;
	int Y0, Y1, Y2, Y3;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	/*double last_error;
	int same_num;*/
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double simpleArrayY[256];

	for (int arra = 0; arra < 256; arra++)
	{

		simpleArrayY[arra] = 1.164*(arra - 16);
	}

	double det;
	det = 4 * pow(0.5625, 4) * ((sum_of_pow_a * sum_of_pow_b) - pow(sum_of_mul_ab, 2));
	//cout << "det: " << det << endl;

	int xx;
	int yy = 0;
	/*420Lin*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			AA = (G_pow_a*U_plane.at<double>(y, x) + G_mul_ab * V_plane.at<double>(y, x))
				+ (R_pow_a*U_plane.at<double>(y, x + 1) + R_mul_ab * V_plane.at<double>(y, x + 1))
				+ (B_pow_a*U_plane.at<double>(y + 1, x) + B_mul_ab * V_plane.at<double>(y + 1, x))
				+ (G_pow_a*U_plane.at<double>(y + 1, x + 1) + G_mul_ab * V_plane.at<double>(y + 1, x + 1));

			BB = (G_mul_ab*U_plane.at<double>(y, x) + G_pow_b * V_plane.at<double>(y, x))
				+ (R_mul_ab*U_plane.at<double>(y, x + 1) + R_pow_b * V_plane.at<double>(y, x + 1))
				+ (B_mul_ab*U_plane.at<double>(y + 1, x) + B_pow_b * V_plane.at<double>(y + 1, x))
				+ (G_mul_ab*U_plane.at<double>(y + 1, x + 1) + G_pow_b * V_plane.at<double>(y + 1, x + 1));

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_mul_ab * BB - sum_of_pow_b * AA) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_mul_ab * AA - sum_of_pow_a * BB) / C, 0, 255);



			xx++;
		}
		yy++;
	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);

	C = 0.5625 * C;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{

			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x);
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x + 1);
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x);
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x + 1);

			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x);
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x + 1);
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x);
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x + 1);

			AA = G_pow_a * Up1 + G_mul_ab * Vp1
				+ R_pow_a * Up2 + R_mul_ab * Vp2
				+ B_pow_a * Up3 + B_mul_ab * Vp3
				+ G_pow_a * Up4 + G_mul_ab * Vp4;

			BB = G_pow_b * Vp1 + G_mul_ab * Up1
				+ R_pow_b * Vp2 + R_mul_ab * Up2
				+ B_pow_b * Vp3 + B_mul_ab * Up3
				+ G_pow_b * Vp4 + G_mul_ab * Up4;

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b * AA - sum_of_mul_ab * BB) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a * BB - sum_of_mul_ab * AA) / C, 0, 255);

			xx++;
		}
		yy++;
	}
	/*420GDCS*/
	double theLeftTopU, theLeftDownU, theRightTopU, theRightDownU;
	double theLeftTopV, theLeftDownV, theRightTopV, theRightDownV;
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{

			des_U = DS_U_plane.at<double>(yy, xx);
			des_V = DS_V_plane.at<double>(yy, xx);


			/*梯度下降*/


			M0 = (int)BGR.at<Vec3b>(y, x)[1];
			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];


			Y0 = (int)(Y_plane.at<double>(y, x) + 0.5);
			Y1 = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
			Y2 = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
			Y3 = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
			simpleYleftTop = simpleArrayY[Y0];
			simpleYr = simpleArrayY[Y1];
			simpleYb = simpleArrayY[Y2];
			simpleYrightDown = simpleArrayY[Y3];



			tmp_U = des_U;
			tmp_V = des_V;

			theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;


			rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
			rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
			rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
			rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
			error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



			if (error == 0) {

				DS_U_plane.at<double>(yy, xx) = des_U;
				DS_V_plane.at<double>(yy, xx) = des_V;

				xx++;
				continue;

			}



			error_TH = INT_MAX;
			for (int i = -1; i <= 1; i++) {
				tmp_V = des_V - i;


				if (0 <= tmp_V && tmp_V <= 255) {
					theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
					theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
					theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;

					rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R

					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {

							theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
							theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
							theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;


							rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
							rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
							rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
							error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);


							if (error < error_TH)
							{
								error_TH = error;
								n_i = i;
								n_j = j;
								can_U = tmp_U;
								can_V = tmp_V;
							}

						}

					}
				}

			}

			des_U = can_U;
			des_V = can_V;


			/*	last_error = error_TH;
			same_num = 0;*/

			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						theLeftTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
						theRightTopV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
						theRightDownV = ((double)(tmp_V * 9) + DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;

						rec_M1 = Clip((int)(simpleYr + 1.596*(theRightTopV - 128) + 0.5), 0, 255);//R
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {
									theLeftTopU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
									theLeftDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
									theRightDownU = ((double)(tmp_U * 9) + DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;


									rec_M0 = Clip((int)(simpleYleftTop - 0.391 * (theLeftTopU - 128) - 0.813*(theLeftTopV - 128) + 0.5), 0, 255);//G
									rec_M2 = Clip((int)(simpleYb + 2.018* (theLeftDownU - 128) + 0.5), 0, 255);//B
									rec_M3 = Clip((int)(simpleYrightDown - 0.391 * (theRightDownU - 128) - 0.813 * (theRightDownV - 128) + 0.5), 0, 255);//G
									error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



									if (error < error_TH)
									{
										error_TH = error;
										tmp_i = i;
										tmp_j = j;
										can_U = tmp_U;
										can_V = tmp_V;
									}
								}


							}
						}
					}

				}
				n_i = tmp_i;
				n_j = tmp_j;
				des_U = can_U;
				des_V = can_V;


			}




			DS_U_plane.at<double>(yy, xx) = des_U;
			DS_V_plane.at<double>(yy, xx) = des_V;




			xx++;
		}

		yy++;


	}
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));



}

void DownSample_RGB_420JCSU_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	double B_pow_a = 4.072324;//2.018^2
	double G_pow_a = 0.152881;//(-0.391)^2
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = 0.660969;//(-0.813)^2
	double R_pow_b = 2.547216;//(1.596)^2

	double B_mul_ab = 0;
	double G_mul_ab = 0.317883;//(-0.391)*(-0.813);
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab;
	double pow_of_SumofMul_ab = sum_of_mul_ab * sum_of_mul_ab;
	double C = 2.25*(pow_of_SumofMul_ab - sum_of_pow_a * sum_of_pow_b);//2.25 = 4*(9/16)
	double AA, BB;

	double U1, U2, U3, U4;
	double V1, V2, V3, V4;

	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	double sum_of_U, sum_of_V;



	double sY1, sY2, sY3, sY4;
	int R1, R2, R3, R4, G1, G2, G3, G4, B1, B2, B3, B4;
	int diff_R1, diff_R2, diff_R3, diff_R4, diff_G1, diff_G2, diff_G3, diff_G4, diff_B1, diff_B2, diff_B3, diff_B4;
	int Y1, Y2, Y3, Y4;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	double mul_U, mul_V;
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double Ybgr[256];
	for (int arra = 0; arra < 256; arra++)
	{

		Ybgr[arra] = 1.164*(arra - 16);//Y

	}

	int yy = 0;
	int xx = 0;

	/*420A*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(mirror(height, y), mirror(width, x)) + U_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ U_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + U_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(mirror(height, y), mirror(width, x)) + V_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ V_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);


			xx++;
		}
		yy++;

	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);



	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{

			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x);
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x + 1);
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x);
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x + 1);
			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x);
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x + 1);
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x);
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x + 1);
			sum_of_U = Up1 + Up2 + Up3 + Up4;
			sum_of_V = Vp1 + Vp2 + Vp3 + Vp4;

			AA = sum_of_pow_a * sum_of_U + sum_of_mul_ab * sum_of_V;
			BB = sum_of_pow_b * sum_of_V + sum_of_mul_ab * sum_of_U;

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b * AA - sum_of_mul_ab * BB) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a * BB - sum_of_mul_ab * AA) / C, 0, 255);

			xx++;
		}
		yy++;
	}
	
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));



}

void DownSample_RGB_420IJCSU_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	double B_pow_a = 4.072324;//2.018^2
	double G_pow_a = 0.152881;//(-0.391)^2
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = 0.660969;//(-0.813)^2
	double R_pow_b = 2.547216;//(1.596)^2

	double B_mul_ab = 0;
	double G_mul_ab = 0.317883;//(-0.391)*(-0.813);
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab;
	double pow_of_SumofMul_ab = sum_of_mul_ab * sum_of_mul_ab;
	double C = 2.25*(pow_of_SumofMul_ab - sum_of_pow_a * sum_of_pow_b);//2.25 = 4*(9/16)
	double AA, BB;


	double U1, U2, U3, U4;
	double V1, V2, V3, V4;

	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	double sum_of_U, sum_of_V;



	double sY1, sY2, sY3, sY4;
	int R1, R2, R3, R4, G1, G2, G3, G4, B1, B2, B3, B4;
	int diff_R1, diff_R2, diff_R3, diff_R4, diff_G1, diff_G2, diff_G3, diff_G4, diff_B1, diff_B2, diff_B3, diff_B4;
	int Y1, Y2, Y3, Y4;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	double mul_U, mul_V;
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double Ybgr[256];
	for (int arra = 0; arra < 256; arra++)
	{

		Ybgr[arra] = 1.164*(arra - 16);//Y

	}

	double det;
	det = 4 * pow(0.5625, 4) * ((sum_of_pow_a * sum_of_pow_b) - pow(sum_of_mul_ab, 2));
	cout << "det: " << det << endl;

	int yy = 0;
	int xx = 0;

	/*420A*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(mirror(height, y), mirror(width, x)) + U_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ U_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + U_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(mirror(height, y), mirror(width, x)) + V_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ V_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);


			xx++;
		}
		yy++;

	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);



	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{

			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x);
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x + 1);
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x);
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x + 1);
			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x);
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x + 1);
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x);
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x + 1);
			sum_of_U = Up1 + Up2 + Up3 + Up4;
			sum_of_V = Vp1 + Vp2 + Vp3 + Vp4;

			AA = sum_of_pow_a * sum_of_U + sum_of_mul_ab * sum_of_V;
			BB = sum_of_pow_b * sum_of_V + sum_of_mul_ab * sum_of_U;

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b * AA - sum_of_mul_ab * BB) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a * BB - sum_of_mul_ab * AA) / C, 0, 255);

			xx++;
		}
		yy++;
	}
	/*GD*/
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			//sum = 0;
			des_U = DS_U_plane.at<double>(yy, xx);
			des_V = DS_V_plane.at<double>(yy, xx);


			/*梯度下降*/


			B1 = (int)BGR.at<Vec3b>(y, x)[0];
			G1 = (int)BGR.at<Vec3b>(y, x)[1];
			R1 = (int)BGR.at<Vec3b>(y, x)[2];

			B2 = (int)BGR.at<Vec3b>(y, x + 1)[0];
			G2 = (int)BGR.at<Vec3b>(y, x + 1)[1];
			R2 = (int)BGR.at<Vec3b>(y, x + 1)[2];


			B3 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			G3 = (int)BGR.at<Vec3b>(y + 1, x)[1];
			R3 = (int)BGR.at<Vec3b>(y + 1, x)[2];

			B4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[0];
			G4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
			R4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[2];

			sY1 = Ybgr[(int)(Y_plane.at<double>(y, x) + 0.5)];
			sY2 = Ybgr[(int)(Y_plane.at<double>(y, x + 1) + 0.5)];
			sY3 = Ybgr[(int)(Y_plane.at<double>(y + 1, x) + 0.5)];
			sY4 = Ybgr[(int)(Y_plane.at<double>(y + 1, x + 1) + 0.5)];

			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;


			tmp_U = des_U;
			tmp_V = des_V;
			mul_U = tmp_U * 0.5625;
			mul_V = tmp_V * 0.5625;
			U1 = Up1 + mul_U;
			U2 = Up2 + mul_U;
			U3 = Up3 + mul_U;
			U4 = Up4 + mul_U;
			V1 = Vp1 + mul_V;
			V2 = Vp2 + mul_V;
			V3 = Vp3 + mul_V;
			V4 = Vp4 + mul_V;

			diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
			diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
			diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);

			diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
			diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
			diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);

			diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
			diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
			diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);

			diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);
			diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
			diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
			error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


			if (error == 0) {

				DS_U_plane.at<double>(yy, xx) = des_U;
				DS_V_plane.at<double>(yy, xx) = des_V;

				xx++;
				continue;

			}



			error_TH = INT_MAX;
			for (int i = -1; i <= 1; i++) {
				tmp_V = des_V - i;


				if (0 <= tmp_V && tmp_V <= 255) {
					mul_V = tmp_V * 0.5625;
					V1 = Vp1 + mul_V;
					V2 = Vp2 + mul_V;
					V3 = Vp3 + mul_V;
					V4 = Vp4 + mul_V;
					diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
					diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
					diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
					diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);

					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {

							mul_U = tmp_U * 0.5625;
							U1 = Up1 + mul_U;
							U2 = Up2 + mul_U;
							U3 = Up3 + mul_U;
							U4 = Up4 + mul_U;

							diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
							diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
							diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
							diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

							diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
							diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
							diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
							diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
							error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


							if (error < error_TH)
							{

								error_TH = error;
								n_i = i;
								n_j = j;
								can_U = tmp_U;
								can_V = tmp_V;
							}

						}

					}
				}

			}

			des_U = can_U;
			des_V = can_V;


			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						mul_V = tmp_V * 0.5625;
						V1 = Vp1 + mul_V;
						V2 = Vp2 + mul_V;
						V3 = Vp3 + mul_V;
						V4 = Vp4 + mul_V;
						diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
						diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
						diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
						diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {
									mul_U = tmp_U * 0.5625;
									U1 = Up1 + mul_U;
									U2 = Up2 + mul_U;
									U3 = Up3 + mul_U;
									U4 = Up4 + mul_U;

									diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
									diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
									diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
									diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

									diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
									diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
									diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
									diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
									error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;
									if (error < error_TH)
									{

										error_TH = error;
										tmp_i = i;
										tmp_j = j;
										can_U = tmp_U;
										can_V = tmp_V;
									}
								}


							}
						}
					}

				}
				n_i = tmp_i;
				n_j = tmp_j;
				des_U = can_U;
				des_V = can_V;

			}




			DS_U_plane.at<double>(yy, xx) = des_U;
			DS_V_plane.at<double>(yy, xx) = des_V;


			xx++;
		}

		yy++;


	}

	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));



}



void DownSample_DTDI_420IJCSU_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	double B_pow_a = 4.072324;
	double G_pow_a = 0.152881;
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = 0.660969;
	double R_pow_b = 2.547216;

	double B_mul_ab = 0;
	double G_mul_ab = 0.317883;
	double R_mul_ab = 0;

	double sum_of_pow_a1 = G_pow_a + B_pow_a;// + R_pow_a;
	double sum_of_pow_a2 = G_pow_a + R_pow_a;// + B_pow_a;
	double sum_of_pow_a3 = G_pow_a + B_pow_a;// + R_pow_a;
	double sum_of_pow_a4 = G_pow_a + R_pow_a;// + B_pow_a;
	;//
	double sum_of_pow_b1 = G_pow_b + B_pow_b;// + R_pow_b;
	double sum_of_pow_b2 = G_pow_b + R_pow_b;// + B_pow_b;
	double sum_of_pow_b3 = G_pow_b + B_pow_b;// + R_pow_b;
	double sum_of_pow_b4 = G_pow_b + R_pow_b;// + B_pow_b;

	double sum_of_pow_ab1 = G_mul_ab + B_mul_ab;// + R_mul_ab;
	double sum_of_pow_ab2 = G_mul_ab + R_mul_ab;// + B_mul_ab;
	double sum_of_pow_ab3 = G_mul_ab + B_mul_ab;// + R_mul_ab;
	double sum_of_pow_ab4 = G_mul_ab + R_mul_ab;// + B_mul_ab;

	double sum_of_pow_a = 2 * (2 * G_pow_a + R_pow_a + B_pow_a);
	double sum_of_pow_b = 2 * (2 * G_pow_b + R_pow_b + B_pow_b);
	double sum_of_mul_ab = 2 * (2 * G_mul_ab + R_mul_ab + B_mul_ab);
	double pow_of_SumofMul_ab = sum_of_mul_ab * sum_of_mul_ab;
	/*double sum_of_pow_a = 4 * (G_pow_a + R_pow_a + B_pow_a);
	double sum_of_pow_b = 4 * (G_pow_b + R_pow_b + B_pow_b);
	double sum_of_mul_ab = 4 * (G_mul_ab + R_mul_ab + B_mul_ab);
	double pow_of_SumofMul_ab = sum_of_mul_ab * sum_of_mul_ab;*/
	double C = 0.5625 * (pow_of_SumofMul_ab - sum_of_pow_a * sum_of_pow_b);//0.5625 = 9/16
	double AA, BB;

	double U1, U2, U3, U4;
	double V1, V2, V3, V4;

	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;




	double sY1, sY2, sY3, sY4;
	int R1, R2, R3, R4, G1, G2, G3, G4, B1, B2, B3, B4;
	int diff_R1, diff_R2, diff_R3, diff_R4, diff_G1, diff_G2, diff_G3, diff_G4, diff_B1, diff_B2, diff_B3, diff_B4;
	int Y1, Y2, Y3, Y4;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;
	double mul_U, mul_V;
	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;

	double Ybgr[256];
	//int sum;
	for (int arra = 0; arra < 256; arra++)
	{

		Ybgr[arra] = 1.164*(arra - 16);//Y

	}

	int yy = 0;
	int xx = 0;

	/*420A*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(mirror(height, y), mirror(width, x)) + U_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ U_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + U_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(mirror(height, y), mirror(width, x)) + V_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ V_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);


			xx++;
		}
		yy++;

	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);



	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{


			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x);
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x + 1);
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x);
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x + 1);
			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x);
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x + 1);
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x);
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x + 1);

			AA = (sum_of_pow_a1*Up1 + sum_of_pow_a2 * Up2 + sum_of_pow_a3 * Up3 + sum_of_pow_a4 * Up4) + (sum_of_pow_ab1*Vp1 + sum_of_pow_ab2 * Vp2 + sum_of_pow_ab3 * Vp3 + sum_of_pow_ab4 * Vp4);
			BB = (sum_of_pow_b1*Vp1 + sum_of_pow_b2 * Vp2 + sum_of_pow_b3 * Vp3 + sum_of_pow_b4 * Vp4) + (sum_of_pow_ab1*Up1 + sum_of_pow_ab2 * Up2 + sum_of_pow_ab3 * Up3 + sum_of_pow_ab4 * Up4);


			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b * AA - sum_of_mul_ab * BB) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a * BB - sum_of_mul_ab * AA) / C, 0, 255);



			xx++;
		}
		yy++;
	}
	/*GD*/
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{

			des_U = DS_U_plane.at<double>(yy, xx);
			des_V = DS_V_plane.at<double>(yy, xx);


			/*梯度下降*/


			B1 = (int)BGR.at<Vec3b>(y, x)[0];
			G1 = (int)BGR.at<Vec3b>(y, x)[1];
			//R1 = (int)BGR.at<Vec3b>(y, x)[2];

			//B2 = (int)BGR.at<Vec3b>(y, x + 1)[0];
			G2 = (int)BGR.at<Vec3b>(y, x + 1)[1];
			R2 = (int)BGR.at<Vec3b>(y, x + 1)[2];


			B3 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			G3 = (int)BGR.at<Vec3b>(y + 1, x)[1];
			//R3 = (int)BGR.at<Vec3b>(y + 1, x)[2];

			//B4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[0];
			G4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
			R4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[2];

			sY1 = Ybgr[(int)(Y_plane.at<double>(y, x) + 0.5)];
			sY2 = Ybgr[(int)(Y_plane.at<double>(y, x + 1) + 0.5)];
			sY3 = Ybgr[(int)(Y_plane.at<double>(y + 1, x) + 0.5)];
			sY4 = Ybgr[(int)(Y_plane.at<double>(y + 1, x + 1) + 0.5)];

			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;


			tmp_U = des_U;
			tmp_V = des_V;
			mul_U = tmp_U * 0.5625;
			mul_V = tmp_V * 0.5625;
			U1 = Up1 + mul_U;
			U2 = Up2 + mul_U;
			U3 = Up3 + mul_U;
			U4 = Up4 + mul_U;
			V1 = Vp1 + mul_V;
			V2 = Vp2 + mul_V;
			V3 = Vp3 + mul_V;
			V4 = Vp4 + mul_V;

			diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
			diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
			//diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);

			//diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
			diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
			diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);

			diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
			diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
			//diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);

			//diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);
			diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
			diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
			//error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;
			error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


			if (error == 0) {

				DS_U_plane.at<double>(yy, xx) = des_U;
				DS_V_plane.at<double>(yy, xx) = des_V;

				xx++;
				continue;

			}



			error_TH = INT_MAX;
			for (int i = -1; i <= 1; i++) {
				tmp_V = des_V - i;


				if (0 <= tmp_V && tmp_V <= 255) {
					mul_V = tmp_V * 0.5625;
					V1 = Vp1 + mul_V;
					V2 = Vp2 + mul_V;
					V3 = Vp3 + mul_V;
					V4 = Vp4 + mul_V;
					//diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
					diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
					//diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
					diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);

					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {

							mul_U = tmp_U * 0.5625;
							U1 = Up1 + mul_U;
							U2 = Up2 + mul_U;
							U3 = Up3 + mul_U;
							U4 = Up4 + mul_U;

							diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
							//diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
							diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
							//diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

							diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
							diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
							diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
							diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
							//error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;
							error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


							if (error < error_TH)
							{

								error_TH = error;
								n_i = i;
								n_j = j;
								can_U = tmp_U;
								can_V = tmp_V;
							}

						}

					}
				}

			}

			des_U = can_U;
			des_V = can_V;


			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						mul_V = tmp_V * 0.5625;
						V1 = Vp1 + mul_V;
						V2 = Vp2 + mul_V;
						V3 = Vp3 + mul_V;
						V4 = Vp4 + mul_V;
						//diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
						diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
						//diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
						diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {
									mul_U = tmp_U * 0.5625;
									U1 = Up1 + mul_U;
									U2 = Up2 + mul_U;
									U3 = Up3 + mul_U;
									U4 = Up4 + mul_U;

									diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
									//diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
									diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
									//diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

									diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
									diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
									diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
									diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
									//error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;
									error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_G4 * diff_G4 + diff_R4 * diff_R4;

									if (error < error_TH)
									{

										error_TH = error;
										tmp_i = i;
										tmp_j = j;
										can_U = tmp_U;
										can_V = tmp_V;
									}
								}


							}
						}
					}

				}
				n_i = tmp_i;
				n_j = tmp_j;
				des_U = can_U;
				des_V = can_V;

			}



			DS_U_plane.at<double>(yy, xx) = des_U;
			DS_V_plane.at<double>(yy, xx) = des_V;


			xx++;
		}

		yy++;


	}

	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));



}



/*20190213*/
void DownSample_420TestOne_Bilinear(int height, int width, Mat& new_Y_plane, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	/*Bilinear solution 變數*/
	double B_pow_a = 4.072324;//2.018^2
	double G_pow_a = 0.152881;//(-0.391)^2
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = 0.660969;//(-0.813)^2
	double R_pow_b = 2.547216;//(1.596)^2

	double B_mul_ab = 0;
	double G_mul_ab = 0.317883;//(-0.391)*(-0.813);
	double R_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab;
	double pow_of_SumofMul_ab = sum_of_mul_ab * sum_of_mul_ab;

	double AA, BB;

	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	double sum_of_U, sum_of_V;


	/*420A變數*/
	Mat DS_A_Y_plane, DS_A_U_plane, DS_A_V_plane;
	DS_A_Y_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);
	DS_A_U_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);
	DS_A_V_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);

	/*Linear Regression變數*/
	Mat Y, A, X;
	Mat Y_m, Y_sd, A_m, A_sd;

	double U_LG1, U_LG2, U_LG3, U_LG4;
	double V_LG1, V_LG2, V_LG3, V_LG4;
	double u_alpha, u_beta, v_alpha, v_beta;
	double r;
	double r_TH = 0.9;
	double C = 2.25*(pow_of_SumofMul_ab - sum_of_pow_a * sum_of_pow_b);//2.25 = 4*(9/16)

	Mat A_tmp, Y_tmp;

	int yy;
	int xx;


	/*420A*/
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_A_Y_plane.at<double>(yy, xx) = Clip((Y_plane.at<double>(mirror(height, y), mirror(width, x)) + Y_plane.at<double>(mirror(height, y), mirror(width, x + 1))
				+ Y_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + Y_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 4, 0, 255);

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

	DS_U_plane = DS_A_U_plane.clone();
	DS_V_plane = DS_A_V_plane.clone();



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

			U_LG1 = X.at<double>(0, 0)*new_Y_plane.at<double>(y, x) + X.at<double>(1, 0);
			U_LG2 = X.at<double>(0, 0)*new_Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0);
			U_LG3 = X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0);
			U_LG4 = X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0);

			Up1 = u_beta * (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x);
			Up2 = u_beta * (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16 - U_plane.at<double>(y, x + 1);
			Up3 = u_beta * (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x);
			Up4 = u_beta * (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16 - U_plane.at<double>(y + 1, x + 1);

			sum_of_U = u_alpha * (U_LG1 + U_LG2 + U_LG3 + U_LG4) + (Up1 + Up2 + Up3 + Up4);

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
				//cout << abs(r) << " " << r_TH<<" "<<v_alpha << endl;
			}
			else
			{
				v_alpha = 0;

			}
			v_beta = 1 - v_alpha;

			copyMakeBorder(A, A, 0, 0, 0, 1, BORDER_CONSTANT, 1);
			X = (A.t()*A).inv()*A.t()*Y;
			V_LG1 = X.at<double>(0, 0)*new_Y_plane.at<double>(y, x) + X.at<double>(1, 0);
			V_LG2 = X.at<double>(0, 0)*new_Y_plane.at<double>(y, x + 1) + X.at<double>(1, 0);
			V_LG3 = X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x) + X.at<double>(1, 0);
			V_LG4 = X.at<double>(0, 0)*new_Y_plane.at<double>(y + 1, x + 1) + X.at<double>(1, 0);




			Vp1 = v_beta * (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x);
			Vp2 = v_beta * (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16 - V_plane.at<double>(y, x + 1);
			Vp3 = v_beta * (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x);
			Vp4 = v_beta * (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16 - V_plane.at<double>(y + 1, x + 1);

			sum_of_V = v_alpha * (V_LG1 + V_LG2 + V_LG3 + V_LG4) + (Vp1 + Vp2 + Vp3 + Vp4);


			AA = sum_of_pow_a * sum_of_U + sum_of_mul_ab * sum_of_V;
			BB = sum_of_pow_b * sum_of_V + sum_of_mul_ab * sum_of_U;

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b * AA - sum_of_mul_ab * BB) / (u_beta * C), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a * BB - sum_of_mul_ab * AA) / (v_beta * C), 0, 255);


			xx++;
		}
		yy++;
	}


	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));

}


/*YUV*/  
void DownSample_420TestTwo_Bilinear(int height, int width, Mat& new_Y_plane, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;



	Mat A4 = (Mat_<double>(12, 6) <<
		1.164, 0, 0, 0, 0, 1.596,
		1.164, 0, 0, 0, -0.391, -0.813,
		1.164, 0, 0, 0, 2.018, 0,

		0, 1.164, 0, 0, 0, 1.596,
		0, 1.164, 0, 0, -0.391, -0.813,
		0, 1.164, 0, 0, 2.018, 0,

		0, 0, 1.164, 0, 0, 1.596,
		0, 0, 1.164, 0, -0.391, -0.813,
		0, 0, 1.164, 0, 2.018, 0,

		0, 0, 0, 1.164, 0, 1.596,
		0, 0, 0, 1.164, -0.391, -0.813,
		0, 0, 0, 1.164, 2.018, 0);


	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);



	int yy = 0;
	int xx = 0;
	
	/*420A*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(y, x) + U_plane.at<double>(y, x + 1) + U_plane.at<double>(y + 1, x) + U_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(y, x) + V_plane.at<double>(y, x + 1) + V_plane.at<double>(y + 1, x) + V_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);

			xx++;
		}
		yy++;

	}
	

	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);

	/*cout << DS_U_plane.at<double>(50, 50) << "Uu2 ";
	cout << DS_V_plane.at<double>(50, 50) << "Vv2 ";*/

	Mat X, Y;
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			Up1 = (DS_U_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			Up2 = (DS_U_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy - 1, xx)) / 16;
			Up3 = (DS_U_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_U_plane.at<double>(yy, xx - 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			Up4 = (DS_U_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_U_plane.at<double>(yy, xx + 1) + 3.0*DS_U_plane.at<double>(yy + 1, xx)) / 16;
			Vp1 = (DS_V_plane.at<double>(yy - 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			Vp2 = (DS_V_plane.at<double>(yy - 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy - 1, xx)) / 16;
			Vp3 = (DS_V_plane.at<double>(yy + 1, xx - 1) + 3.0*DS_V_plane.at<double>(yy, xx - 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;
			Vp4 = (DS_V_plane.at<double>(yy + 1, xx + 1) + 3.0*DS_V_plane.at<double>(yy, xx + 1) + 3.0*DS_V_plane.at<double>(yy + 1, xx)) / 16;

			

			Y = (Mat_<double>(12, 1) <<
				(double)BGR.at<Vec3b>(y, x)[2] - 1.596*Vp1,
				(double)BGR.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1,
				(double)BGR.at<Vec3b>(y, x)[0] - 2.018*Up1,

				(double)BGR.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[1] + 0.391*Up2 + 0.813*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[0] - 2.018*Up2,

				(double)BGR.at<Vec3b>(y + 1, x)[2] - 1.596*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[1] + 0.391*Up3 + 0.813*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[0] - 2.018*Up3,

				(double)BGR.at<Vec3b>(y + 1, x + 1)[2] - 1.596*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[0] - 2.018*Up4);

			//Y = Y / 255;
			X = (A4.t()*A4).inv()*A4.t()*Y;
			X = X + E;

			//X = X * 255;
			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);
			DS_U_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) / 0.5625 + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) / 0.5625 + 0.5), 0, 255);
			xx++;
		}

		yy++;


	}
	/*cout << DS_U_plane.at<double>(50, 50) << "U2 ";
	cout << DS_V_plane.at<double>(50, 50) << "V2 ";*/
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));

}


void DownSample_420TestThree_Bilinear(int height, int width, Mat& new_Y_plane, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;

	Mat A4 = (Mat_<double>(12, 6) <<
		1.164, 0, 0, 0, 0, 1.596,
		1.164, 0, 0, 0, -0.391, -0.813,
		1.164, 0, 0, 0, 2.018, 0,

		0, 1.164, 0, 0, 0, 1.596*0.5,
		0, 1.164, 0, 0, -0.391*0.5, -0.813*0.5,
		0, 1.164, 0, 0, 2.018*0.5, 0,

		0, 0, 1.164, 0, 0, 1.596*0.5,
		0, 0, 1.164, 0, -0.391*0.5, -0.813*0.5,
		0, 0, 1.164, 0, 2.018*0.5, 0,

		0, 0, 0, 1.164, 0, 1.596*0.25,
		0, 0, 0, 1.164, -0.391*0.25, -0.813*0.25,
		0, 0, 0, 1.164, 2.018*0.25, 0);


	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);


	Mat tmp_U_plane, tmp_V_plane;
	copyMakeBorder(U_plane, tmp_U_plane, 0, 1, 0, 1, BORDER_REFLECT_101);//ABC|BA
	copyMakeBorder(V_plane, tmp_V_plane, 0, 1, 0, 1, BORDER_REFLECT_101);//ABC|BA

	Mat X, Y;
	int yy;
	int xx;
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{



			Up2 = 0.5*tmp_U_plane.at<double>(y, x + 2);
			Up3 = 0.5*tmp_U_plane.at<double>(y + 2, x);
			Up4 = 0.25*(tmp_U_plane.at<double>(y, x + 2) + tmp_U_plane.at<double>(y + 2, x) + tmp_U_plane.at<double>(y + 2, x + 2));

			Vp2 = 0.5*tmp_V_plane.at<double>(y, x + 2);
			Vp3 = 0.5*tmp_V_plane.at<double>(y + 2, x);
			Vp4 = 0.25*(tmp_V_plane.at<double>(y, x + 2) + tmp_V_plane.at<double>(y + 2, x) + tmp_V_plane.at<double>(y + 2, x + 2));



			Y = (Mat_<double>(12, 1) <<
				(double)BGR.at<Vec3b>(y, x)[2] + 222.912,
				(double)BGR.at<Vec3b>(y, x)[1] - 135.488,
				(double)BGR.at<Vec3b>(y, x)[0] + 276.928,

				(double)BGR.at<Vec3b>(y, x + 1)[2] + 222.912 - 1.596*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[1] - 135.488 + 0.391*Up2 + 0.813*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[0] + 276.928 - 2.018*Up2,

				(double)BGR.at<Vec3b>(y + 1, x)[2] + 222.912 - 1.596*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[1] - 135.488 + 0.391*Up3 + 0.813*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[0] + 276.928 - 2.018*Up3,

				(double)BGR.at<Vec3b>(y + 1, x + 1)[2] + 222.912 - 1.596*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[1] - 135.488 + 0.391*Up4 + 0.813*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[0] + 276.928 - 2.018*Up4);


			X = (A4.t()*A4).inv()*A4.t()*Y;

			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);
			DS_U_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) + 0.5), 0, 255);


			xx++;
		}

		yy++;


	}

}


void DownSample_420TestFour_Bilinear(int height, int width, Mat& new_Y_plane, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;



	Mat A4 = (Mat_<double>(12, 2) <<
		0, 1.596,
		-0.391, -0.813,
		2.018, 0,

		0, 1.596*0.5,
		-0.391*0.5, -0.813*0.5,
		2.018*0.5, 0,

		0, 1.596*0.5,
		-0.391*0.5, -0.813*0.5,
		2.018*0.5, 0,

		0, 1.596*0.25,
		-0.391*0.25, -0.813*0.25,
		2.018*0.25, 0);


	Mat E = (Mat_<double>(2, 1) <<
		128, 128);


	Mat tmp_U_plane, tmp_V_plane;
	copyMakeBorder(U_plane, tmp_U_plane, 0, 1, 0, 1, BORDER_REFLECT_101);//ABC|BA
	copyMakeBorder(V_plane, tmp_V_plane, 0, 1, 0, 1, BORDER_REFLECT_101);//ABC|BA

	Mat X, Y;
	int yy;
	int xx;
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{



			Up2 = 0.5*tmp_U_plane.at<double>(y, x + 2);
			Up3 = 0.5*tmp_U_plane.at<double>(y + 2, x);
			Up4 = 0.25*(tmp_U_plane.at<double>(y, x + 2) + tmp_U_plane.at<double>(y + 2, x) + tmp_U_plane.at<double>(y + 2, x + 2));

			Vp2 = 0.5*tmp_V_plane.at<double>(y, x + 2);
			Vp3 = 0.5*tmp_V_plane.at<double>(y + 2, x);
			Vp4 = 0.25*(tmp_V_plane.at<double>(y, x + 2) + tmp_V_plane.at<double>(y + 2, x) + tmp_V_plane.at<double>(y + 2, x + 2));



			Y = (Mat_<double>(12, 1) <<
				(double)BGR.at<Vec3b>(y, x)[2] - 1.164*new_Y_plane.at<double>(y, x) + 222.912,
				(double)BGR.at<Vec3b>(y, x)[1] - 1.164*new_Y_plane.at<double>(y, x) - 135.488,
				(double)BGR.at<Vec3b>(y, x)[0] - 1.164*new_Y_plane.at<double>(y, x) + 276.928,

				(double)BGR.at<Vec3b>(y, x + 1)[2] - 1.164*new_Y_plane.at<double>(y, x + 1) + 222.912 - 1.596*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[1] - 1.164*new_Y_plane.at<double>(y, x + 1) - 135.488 + 0.391*Up2 + 0.813*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[0] - 1.164*new_Y_plane.at<double>(y, x + 1) + 276.928 - 2.018*Up2,

				(double)BGR.at<Vec3b>(y + 1, x)[2] - 1.164*new_Y_plane.at<double>(y + 1, x) + 222.912 - 1.596*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[1] - 1.164*new_Y_plane.at<double>(y + 1, x) - 135.488 + 0.391*Up3 + 0.813*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[0] - 1.164*new_Y_plane.at<double>(y + 1, x) + 276.928 - 2.018*Up3,

				(double)BGR.at<Vec3b>(y + 1, x + 1)[2] - 1.164*new_Y_plane.at<double>(y + 1, x + 1) + 222.912 - 1.596*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[1] - 1.164*new_Y_plane.at<double>(y + 1, x + 1) - 135.488 + 0.391*Up4 + 0.813*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[0] - 1.164*new_Y_plane.at<double>(y + 1, x + 1) + 276.928 - 2.018*Up4);


			X = (A4.t()*A4).inv()*A4.t()*Y;

			
			DS_U_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);


			xx++;
		}

		yy++;


	}




}



void DownSample_420TestFive_Bilinear(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	double B_pow_a = 4.072324;//2.018^2
	double G_pow_a = 0.152881;//(-0.391)^2
	double R_pow_a = 0;

	double B_pow_b = 0;
	double G_pow_b = 0.660969;//(-0.813)^2
	double R_pow_b = 2.547216;//(1.596)^2

	double B_mul_ab = 0;
	double G_mul_ab = 0.317883;//(-0.391)*(-0.813);
	double R_mul_ab = 0;

	double pow_w1 = 1;
	double pow_w2 = 0.25;
	double pow_w3 = 0.25;
	double pow_w4 = 0.0625;
	double sum_of_pow_w = pow_w1 + pow_w2 + pow_w3 + pow_w4;


	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab;
	double pow_of_SumofMul_ab = sum_of_mul_ab * sum_of_mul_ab;
	double C = sum_of_pow_w * (pow_of_SumofMul_ab - sum_of_pow_a * sum_of_pow_b);//2.25 = 4*(9/16)
	double AA, BB;

	double U1, U2, U3, U4;
	double V1, V2, V3, V4;

	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;
	double sum_of_U, sum_of_V;



	double sY1, sY2, sY3, sY4;
	int R1, R2, R3, R4, G1, G2, G3, G4, B1, B2, B3, B4;
	int diff_R1, diff_R2, diff_R3, diff_R4, diff_G1, diff_G2, diff_G3, diff_G4, diff_B1, diff_B2, diff_B3, diff_B4;
	int Y1, Y2, Y3, Y4;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;

	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double Ybgr[256];
	for (int arra = 0; arra < 256; arra++)
	{

		Ybgr[arra] = 1.164*(arra - 16);//Y

	}

	int yy = 0;
	int xx = 0;

	/*420D*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) = V_plane.at<double>(y, x);

			xx++;
		}
		yy++;
	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 0, 1, 0, 1, BORDER_REPLICATE);



	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{

			Up1 = -U_plane.at<double>(y, x);
			Up2 = 0.5 * DS_U_plane.at<double>(yy, xx + 1) - U_plane.at<double>(y, x + 1);
			Up3 = 0.5 * DS_U_plane.at<double>(yy, xx - 1) - U_plane.at<double>(y + 1, x);
			Up4 = 0.25 * (DS_U_plane.at<double>(yy + 1, xx + 1) + DS_U_plane.at<double>(yy, xx + 1) + DS_U_plane.at<double>(yy + 1, xx)) - U_plane.at<double>(y + 1, x + 1);

			Vp1 = -V_plane.at<double>(y, x);
			Vp2 = 0.5 * DS_V_plane.at<double>(yy, xx + 1) - V_plane.at<double>(y, x + 1);
			Vp3 = 0.5 * DS_V_plane.at<double>(yy, xx - 1) - V_plane.at<double>(y + 1, x);
			Vp4 = 0.25 * (DS_V_plane.at<double>(yy + 1, xx + 1) + DS_V_plane.at<double>(yy, xx + 1) + DS_V_plane.at<double>(yy + 1, xx)) - V_plane.at<double>(y + 1, x + 1);

			sum_of_U = Up1 + 0.5*Up2 + 0.5*Up3 + 0.25 * Up4;
			sum_of_V = Vp1 + 0.5*Vp2 + 0.5*Vp3 + 0.25 * Vp4;

			AA = sum_of_pow_a * sum_of_U + sum_of_mul_ab * sum_of_V;
			BB = sum_of_pow_b * sum_of_V + sum_of_mul_ab * sum_of_U;

			DS_U_plane.at<double>(yy, xx) = Clip((int)((sum_of_pow_b * AA - sum_of_mul_ab * BB) / C+0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)((sum_of_pow_a * BB - sum_of_mul_ab * AA) / C+0.5), 0, 255);

			xx++;
		}
		yy++;
	}
	///*GD*/
	//yy = 1;
	//for (int y = 0; y < height; y += 2)
	//{
	//	xx = 1;
	//	for (int x = 0; x < width; x += 2)
	//	{
	//		//sum = 0;
	//		des_U = DS_U_plane.at<double>(yy, xx);
	//		des_V = DS_V_plane.at<double>(yy, xx);


	//		/*梯度下降*/


	//		B1 = (int)BGR.at<Vec3b>(y, x)[0];
	//		G1 = (int)BGR.at<Vec3b>(y, x)[1];
	//		R1 = (int)BGR.at<Vec3b>(y, x)[2];

	//		B2 = (int)BGR.at<Vec3b>(y, x + 1)[0];
	//		G2 = (int)BGR.at<Vec3b>(y, x + 1)[1];
	//		R2 = (int)BGR.at<Vec3b>(y, x + 1)[2];


	//		B3 = (int)BGR.at<Vec3b>(y + 1, x)[0];
	//		G3 = (int)BGR.at<Vec3b>(y + 1, x)[1];
	//		R3 = (int)BGR.at<Vec3b>(y + 1, x)[2];

	//		B4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[0];
	//		G4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
	//		R4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[2];

	//		sY1 = Ybgr[(int)(Y_plane.at<double>(y, x) + 0.5)];
	//		sY2 = Ybgr[(int)(Y_plane.at<double>(y, x + 1) + 0.5)];
	//		sY3 = Ybgr[(int)(Y_plane.at<double>(y + 1, x) + 0.5)];
	//		sY4 = Ybgr[(int)(Y_plane.at<double>(y + 1, x + 1) + 0.5)];

	//		Up1 = 0;
	//		Up2 = 0.5 * DS_U_plane.at<double>(yy, xx + 1);
	//		Up3 = 0.5 * DS_U_plane.at<double>(yy, xx - 1);
	//		Up4 = 0.25 * (DS_U_plane.at<double>(yy + 1, xx + 1) + DS_U_plane.at<double>(yy, xx + 1) + DS_U_plane.at<double>(yy + 1, xx));

	//		Vp1 = 0;
	//		Vp2 = 0.5 * DS_V_plane.at<double>(yy, xx + 1);
	//		Vp3 = 0.5 * DS_V_plane.at<double>(yy, xx - 1);
	//		Vp4 = 0.25 * (DS_V_plane.at<double>(yy + 1, xx + 1) + DS_V_plane.at<double>(yy, xx + 1) + DS_V_plane.at<double>(yy + 1, xx));


	//		tmp_U = des_U;
	//		tmp_V = des_V;
	//		
	//		U1 = Up1 + tmp_U;
	//		U2 = Up2 + tmp_U * 0.5;
	//		U3 = Up3 + tmp_U * 0.5;
	//		U4 = Up4 + tmp_U * 0.25;
	//		V1 = Vp1 + tmp_V;
	//		V2 = Vp2 + tmp_V * 0.5;
	//		V3 = Vp3 + tmp_V * 0.5;
	//		V4 = Vp4 + tmp_V * 0.25;

	//		diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
	//		diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
	//		diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);

	//		diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
	//		diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
	//		diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);

	//		diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
	//		diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
	//		diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);

	//		diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);
	//		diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
	//		diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
	//		error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


	//		if (error == 0) {

	//			DS_U_plane.at<double>(yy, xx) = des_U;
	//			DS_V_plane.at<double>(yy, xx) = des_V;

	//			xx++;
	//			continue;

	//		}



	//		error_TH = INT_MAX;
	//		for (int i = -1; i <= 1; i++) {
	//			tmp_V = des_V - i;


	//			if (0 <= tmp_V && tmp_V <= 255) {
	//				
	//				V1 = Vp1 + tmp_V;
	//				V2 = Vp2 + tmp_V * 0.5;
	//				V3 = Vp3 + tmp_V * 0.5;
	//				V4 = Vp4 + tmp_V * 0.25;
	//				diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
	//				diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
	//				diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
	//				diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);

	//				for (int j = -1; j <= 1; j++) {

	//					tmp_U = des_U + j;

	//					if (0 <= tmp_U && tmp_U <= 255) {

	//						U1 = Up1 + tmp_U;
	//						U2 = Up2 + tmp_U * 0.5;
	//						U3 = Up3 + tmp_U * 0.5;
	//						U4 = Up4 + tmp_U * 0.25;

	//						diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
	//						diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
	//						diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
	//						diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

	//						diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
	//						diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
	//						diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
	//						diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
	//						error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


	//						if (error < error_TH)
	//						{

	//							error_TH = error;
	//							n_i = i;
	//							n_j = j;
	//							can_U = tmp_U;
	//							can_V = tmp_V;
	//						}

	//					}

	//				}
	//			}

	//		}

	//		des_U = can_U;
	//		des_V = can_V;


	//		while ((n_i != 0 || n_j != 0) && error_TH != 0) {


	//			tmp_i = 0;
	//			tmp_j = 0;
	//			for (int i = -1; i <= 1; i++) {
	//				tmp_V = des_V - i;
	//				if (0 <= tmp_V && tmp_V <= 255) {
	//					V1 = Vp1 + tmp_V;
	//					V2 = Vp2 + tmp_V * 0.5;
	//					V3 = Vp3 + tmp_V * 0.5;
	//					V4 = Vp4 + tmp_V * 0.25;
	//					diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
	//					diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
	//					diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
	//					diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
	//					for (int j = -1; j <= 1; j++) {
	//						old_i = i + n_i;
	//						old_j = j + n_j;

	//						if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

	//							tmp_U = des_U + j;

	//							if (0 <= tmp_U && tmp_U <= 255) {
	//								U1 = Up1 + tmp_U;
	//								U2 = Up2 + tmp_U * 0.5;
	//								U3 = Up3 + tmp_U * 0.5;
	//								U4 = Up4 + tmp_U * 0.25;

	//								diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
	//								diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
	//								diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
	//								diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

	//								diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
	//								diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
	//								diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
	//								diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
	//								error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;
	//								if (error < error_TH)
	//								{

	//									error_TH = error;
	//									tmp_i = i;
	//									tmp_j = j;
	//									can_U = tmp_U;
	//									can_V = tmp_V;
	//								}
	//							}


	//						}
	//					}
	//				}

	//			}
	//			n_i = tmp_i;
	//			n_j = tmp_j;
	//			des_U = can_U;
	//			des_V = can_V;

	//		}




	//		DS_U_plane.at<double>(yy, xx) = des_U;
	//		DS_V_plane.at<double>(yy, xx) = des_V;


	//		xx++;
	//	}

	//	yy++;


	//}

	DS_U_plane = DS_U_plane(Rect(0, 0, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(0, 0, width / 2, height / 2));



}


void DownSample_420TestSix_Bilinear(int height, int width, Mat& new_Y_plane, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;



	Mat A4 = (Mat_<double>(12, 6) <<
		1.164, 0, 0, 0, 0, 1.596,
		1.164, 0, 0, 0, -0.391, -0.813,
		1.164, 0, 0, 0, 2.018, 0,

		0, 1.164, 0, 0, 0, 1.596*0.5,
		0, 1.164, 0, 0, -0.391*0.5, -0.813*0.5,
		0, 1.164, 0, 0, 2.018*0.5, 0,

		0, 0, 1.164, 0, 0, 1.596*0.5,
		0, 0, 1.164, 0, -0.391*0.5, -0.813*0.5,
		0, 0, 1.164, 0, 2.018*0.5, 0,

		0, 0, 0, 1.164, 0, 1.596*0.25,
		0, 0, 0, 1.164, -0.391*0.25, -0.813*0.25,
		0, 0, 0, 1.164, 2.018*0.25, 0);


	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);


	Mat tmp_U_plane, tmp_V_plane;
	copyMakeBorder(U_plane, tmp_U_plane, 0, 1, 0, 1, BORDER_REFLECT_101);//ABC|BA
	copyMakeBorder(V_plane, tmp_V_plane, 0, 1, 0, 1, BORDER_REFLECT_101);//ABC|BA

	Mat X, Y;
	int yy;
	int xx;
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{



			Up2 = 0.5*tmp_U_plane.at<double>(y, x + 2);
			Up3 = 0.5*tmp_U_plane.at<double>(y + 2, x);
			Up4 = 0.25*(tmp_U_plane.at<double>(y, x + 2) + tmp_U_plane.at<double>(y + 2, x) + tmp_U_plane.at<double>(y + 2, x + 2));

			Vp2 = 0.5*tmp_V_plane.at<double>(y, x + 2);
			Vp3 = 0.5*tmp_V_plane.at<double>(y + 2, x);
			Vp4 = 0.25*(tmp_V_plane.at<double>(y, x + 2) + tmp_V_plane.at<double>(y + 2, x) + tmp_V_plane.at<double>(y + 2, x + 2));



			Y = (Mat_<double>(12, 1) <<
				(double)BGR.at<Vec3b>(y, x)[2] + 222.912,
				(double)BGR.at<Vec3b>(y, x)[1] - 135.488,
				(double)BGR.at<Vec3b>(y, x)[0] + 276.928,

				(double)BGR.at<Vec3b>(y, x + 1)[2] + 222.912 - 1.596*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[1] - 135.488 + 0.391*Up2 + 0.813*Vp2,
				(double)BGR.at<Vec3b>(y, x + 1)[0] + 276.928 - 2.018*Up2,

				(double)BGR.at<Vec3b>(y + 1, x)[2] + 222.912 - 1.596*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[1] - 135.488 + 0.391*Up3 + 0.813*Vp3,
				(double)BGR.at<Vec3b>(y + 1, x)[0] + 276.928 - 2.018*Up3,

				(double)BGR.at<Vec3b>(y + 1, x + 1)[2] + 222.912 - 1.596*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[1] - 135.488 + 0.391*Up4 + 0.813*Vp4,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[0] + 276.928 - 2.018*Up4);


			X = (A4.t()*A4).inv()*A4.t()*Y;

			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);
			DS_U_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) + 0.5), 0, 255);


			xx++;
		}

		yy++;


	}

	double U1, U2, U3, U4;
	double V1, V2, V3, V4;
	double sY1, sY2, sY3, sY4;
	int R1, R2, R3, R4, G1, G2, G3, G4, B1, B2, B3, B4;
	int diff_R1, diff_R2, diff_R3, diff_R4, diff_G1, diff_G2, diff_G3, diff_G4, diff_B1, diff_B2, diff_B3, diff_B4;
	int Y1, Y2, Y3, Y4;
	int des_U, des_V;
	int tmp_U, tmp_V;
	double error;

	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;
	int can_U, can_V;
	double Ybgr[256];
	for (int arra = 0; arra < 256; arra++)
	{

		Ybgr[arra] = 1.164*(arra - 16);//Y

	}
	copyMakeBorder(DS_U_plane, DS_U_plane, 0, 1, 0, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 0, 1, 0, 1, BORDER_REPLICATE);

	/*GD*/
	yy = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			//sum = 0;
			des_U = DS_U_plane.at<double>(yy, xx);
			des_V = DS_V_plane.at<double>(yy, xx);


			/*梯度下降*/


			B1 = (int)BGR.at<Vec3b>(y, x)[0];
			G1 = (int)BGR.at<Vec3b>(y, x)[1];
			R1 = (int)BGR.at<Vec3b>(y, x)[2];

			B2 = (int)BGR.at<Vec3b>(y, x + 1)[0];
			G2 = (int)BGR.at<Vec3b>(y, x + 1)[1];
			R2 = (int)BGR.at<Vec3b>(y, x + 1)[2];


			B3 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			G3 = (int)BGR.at<Vec3b>(y + 1, x)[1];
			R3 = (int)BGR.at<Vec3b>(y + 1, x)[2];

			B4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[0];
			G4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];
			R4 = (int)BGR.at<Vec3b>(y + 1, x + 1)[2];

			sY1 = Ybgr[(int)(new_Y_plane.at<double>(y, x))];
			sY2 = Ybgr[(int)(new_Y_plane.at<double>(y, x + 1))];
			sY3 = Ybgr[(int)(new_Y_plane.at<double>(y + 1, x))];
			sY4 = Ybgr[(int)(new_Y_plane.at<double>(y + 1, x + 1))];

			Up1 = 0;
			Up2 = 0.5 * DS_U_plane.at<double>(yy, xx + 1);
			Up3 = 0.5 * DS_U_plane.at<double>(yy, xx - 1);
			Up4 = 0.25 * (DS_U_plane.at<double>(yy + 1, xx + 1) + DS_U_plane.at<double>(yy, xx + 1) + DS_U_plane.at<double>(yy + 1, xx));

			Vp1 = 0;
			Vp2 = 0.5 * DS_V_plane.at<double>(yy, xx + 1);
			Vp3 = 0.5 * DS_V_plane.at<double>(yy, xx - 1);
			Vp4 = 0.25 * (DS_V_plane.at<double>(yy + 1, xx + 1) + DS_V_plane.at<double>(yy, xx + 1) + DS_V_plane.at<double>(yy + 1, xx));


			tmp_U = des_U;
			tmp_V = des_V;

			U1 = Up1 + tmp_U;
			U2 = Up2 + tmp_U * 0.5;
			U3 = Up3 + tmp_U * 0.5;
			U4 = Up4 + tmp_U * 0.25;

			V1 = Vp1 + tmp_V;
			V2 = Vp2 + tmp_V * 0.5;
			V3 = Vp3 + tmp_V * 0.5;
			V4 = Vp4 + tmp_V * 0.25;

			diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
			diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
			diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);

			diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
			diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
			diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);

			diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
			diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
			diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);

			diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);
			diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
			diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
			error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


			if (error == 0) {

				DS_U_plane.at<double>(yy, xx) = des_U;
				DS_V_plane.at<double>(yy, xx) = des_V;

				xx++;
				continue;

			}



			error_TH = INT_MAX;
			for (int i = -1; i <= 1; i++) {
				tmp_V = des_V - i;


				if (0 <= tmp_V && tmp_V <= 255) {

					V1 = Vp1 + tmp_V;
					V2 = Vp2 + tmp_V * 0.5;
					V3 = Vp3 + tmp_V * 0.5;
					V4 = Vp4 + tmp_V * 0.25;
					diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
					diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
					diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
					diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);

					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {

							U1 = Up1 + tmp_U;
							U2 = Up2 + tmp_U * 0.5;
							U3 = Up3 + tmp_U * 0.5;
							U4 = Up4 + tmp_U * 0.25;

							diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
							diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
							diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
							diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

							diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
							diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
							diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
							diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
							error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;


							if (error < error_TH)
							{

								error_TH = error;
								n_i = i;
								n_j = j;
								can_U = tmp_U;
								can_V = tmp_V;
							}

						}

					}
				}

			}

			des_U = can_U;
			des_V = can_V;


			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						V1 = Vp1 + tmp_V;
						V2 = Vp2 + tmp_V * 0.5;
						V3 = Vp3 + tmp_V * 0.5;
						V4 = Vp4 + tmp_V * 0.25;
						diff_R1 = R1 - Clip((int)(sY1 + 1.596*(V1 - 128) + 0.5), 0, 255);
						diff_R2 = R2 - Clip((int)(sY2 + 1.596*(V2 - 128) + 0.5), 0, 255);
						diff_R3 = R3 - Clip((int)(sY3 + 1.596*(V3 - 128) + 0.5), 0, 255);
						diff_R4 = R4 - Clip((int)(sY4 + 1.596*(V4 - 128) + 0.5), 0, 255);
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {
									U1 = Up1 + tmp_U;
									U2 = Up2 + tmp_U * 0.5;
									U3 = Up3 + tmp_U * 0.5;
									U4 = Up4 + tmp_U * 0.25;

									diff_B1 = B1 - Clip((int)(sY1 + 2.018*(U1 - 128) + 0.5), 0, 255);
									diff_B2 = B2 - Clip((int)(sY2 + 2.018*(U2 - 128) + 0.5), 0, 255);
									diff_B3 = B3 - Clip((int)(sY3 + 2.018*(U3 - 128) + 0.5), 0, 255);
									diff_B4 = B4 - Clip((int)(sY4 + 2.018*(U4 - 128) + 0.5), 0, 255);

									diff_G1 = G1 - Clip((int)(sY1 - 0.391*(U1 - 128) - 0.813*(V1 - 128) + 0.5), 0, 255);
									diff_G2 = G2 - Clip((int)(sY2 - 0.391*(U2 - 128) - 0.813*(V2 - 128) + 0.5), 0, 255);
									diff_G3 = G3 - Clip((int)(sY3 - 0.391*(U3 - 128) - 0.813*(V3 - 128) + 0.5), 0, 255);
									diff_G4 = G4 - Clip((int)(sY4 - 0.391*(U4 - 128) - 0.813*(V4 - 128) + 0.5), 0, 255);
									error = diff_B1 * diff_B1 + diff_G1 * diff_G1 + diff_R1 * diff_R1 + diff_B2 * diff_B2 + diff_G2 * diff_G2 + diff_R2 * diff_R2 + diff_B3 * diff_B3 + diff_G3 * diff_G3 + diff_R3 * diff_R3 + diff_B4 * diff_B4 + diff_G4 * diff_G4 + diff_R4 * diff_R4;
									if (error < error_TH)
									{

										error_TH = error;
										tmp_i = i;
										tmp_j = j;
										can_U = tmp_U;
										can_V = tmp_V;
									}
								}


							}
						}
					}

				}
				n_i = tmp_i;
				n_j = tmp_j;
				des_U = can_U;
				des_V = can_V;

			}




			DS_U_plane.at<double>(yy, xx) = des_U;
			DS_V_plane.at<double>(yy, xx) = des_V;


			xx++;
		}

		yy++;


	}

	DS_U_plane = DS_U_plane(Rect(0, 0, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(0, 0, width / 2, height / 2));


}



void DownSample_420TestSeven_Bilinear(int height, int width, Mat& new_Y_plane, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;



	Mat A4 = (Mat_<double>(12, 6) <<
		1.164, 0, 0, 0, 0, 1.596,
		1.164, 0, 0, 0, -0.391, -0.813,
		1.164, 0, 0, 0, 2.018, 0,

		0, 1.164, 0, 0, 0, 1.596,
		0, 1.164, 0, 0, -0.391, -0.813,
		0, 1.164, 0, 0, 2.018, 0,

		0, 0, 1.164, 0, 0, 1.596,
		0, 0, 1.164, 0, -0.391, -0.813,
		0, 0, 1.164, 0, 2.018, 0,

		0, 0, 0, 1.164, 0, 1.596,
		0, 0, 0, 1.164, -0.391, -0.813,
		0, 0, 0, 1.164, 2.018, 0);

	Mat E = (Mat_<double>(6, 1) <<
		16, 16, 16, 16, 128, 128);

	int yy = 0;
	int xx = 0;
	/*420A*/
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(y, x) + U_plane.at<double>(y, x + 1) + U_plane.at<double>(y + 1, x) + U_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(y, x) + V_plane.at<double>(y, x + 1) + V_plane.at<double>(y + 1, x) + V_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);

			xx++;
		}
		yy++;

	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 1, 1, 1, 1, BORDER_REPLICATE);
	Mat X, Y;
	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			
			Y = (Mat_<double>(12, 1) <<
				(double)BGR.at<Vec3b>(y, x)[2] ,
				(double)BGR.at<Vec3b>(y, x)[1] ,
				(double)BGR.at<Vec3b>(y, x)[0] ,

				(double)BGR.at<Vec3b>(y, x + 1)[2] ,
				(double)BGR.at<Vec3b>(y, x + 1)[1] ,
				(double)BGR.at<Vec3b>(y, x + 1)[0] ,

				(double)BGR.at<Vec3b>(y + 1, x)[2] ,
				(double)BGR.at<Vec3b>(y + 1, x)[1] ,
				(double)BGR.at<Vec3b>(y + 1, x)[0] ,

				(double)BGR.at<Vec3b>(y + 1, x + 1)[2] ,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[1] ,
				(double)BGR.at<Vec3b>(y + 1, x + 1)[0] );

			//Y = Y / 255;
			X = (A4.t()*A4).inv()*A4.t()*Y;
			X = X + E;

			//X = X * 255;
			new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(3, 0) + 0.5), 0, 255);
			DS_U_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(4, 0) / 0.5625 + 0.5), 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((int)(X.at<double>(5, 0) / 0.5625 + 0.5), 0, 255);
			xx++;
		}
		yy++;
	}
	DS_U_plane = DS_U_plane(Rect(1, 1, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(1, 1, width / 2, height / 2));

}

double calculate_UV(double u, double v, int yy, int xx, Mat& DS_U_plane, Mat& DS_V_plane, int flag)
{
	double calculate = 0;
	double x_array[] = { -2 + u, -1 + u, u, 1 + u, 2 + u };
	double y_array[] = { 2 + v, 1 + v, v, -1 + v, -2 + v };

	switch (flag) // 1 for U, 2 for V
	{
	case 1:
		for (int i = 0; i < sizeof(x_array) / sizeof(*x_array); i++)
		{
			for (int j = 0; j < sizeof(y_array) / sizeof(*y_array); j++)
			{
				calculate += BC_funtion(x_array[i]) * BC_funtion(y_array[j]) * DS_U_plane.at<double>(yy + i - 2, xx + j - 2);
			}
		}
		calculate -= BC_funtion(u) * BC_funtion(v) * DS_U_plane.at<double>(yy, xx);
		break;
	case 2:
		for (int i = 0; i < sizeof(x_array) / sizeof(*x_array); i++)
		{
			for (int j = 0; j < sizeof(y_array) / sizeof(*y_array); j++)
			{
				calculate += BC_funtion(x_array[i]) * BC_funtion(y_array[j]) * DS_V_plane.at<double>(yy + i - 2, xx + j - 2);
			}
		}
		calculate -= BC_funtion(u) * BC_funtion(v) * DS_V_plane.at<double>(yy, xx);
		break;
	default:
		break;
	}

	return calculate;
}

struct pixel {
	double alpha;
	double beta;
};

void cal_cbcr(Mat& DS_U_plane, Mat& DS_V_plane, int yy, int xx, vector<pixel>& Block, vector<double>& diff_cb, vector<double>& diff_cr, double& w) {
	double item1_1 = 0; double item1_2 = 0;
	double item2_1 = 0; double item2_2 = 0;
	double item3_1 = 0; double item3_2; double item3_3 = 0;
	int i = 0;

	for (pixel p : Block) {
		item1_1 += p.beta * p.beta;
		item1_2 += (p.alpha * p.alpha * diff_cb[i]) + (p.alpha * p.beta * diff_cr[i]);
		item2_1 += p.alpha * p.beta;
		item2_2 += (p.alpha * p.beta * diff_cb[i]) + (p.beta * p.beta * diff_cr[i]);
		item3_1 += p.alpha * p.alpha;
		i++;
	}
	item3_2 = item1_1;
	item3_3 = item2_1 * item2_1;
	DS_U_plane.at<double>(yy, xx) = Clip((int)(((item1_1 * item1_2) - (item2_1 * item2_2)) / (w * ((item3_1 * item1_1) - item3_3)) + 0.5), 16, 240);
	DS_V_plane.at<double>(yy, xx) = Clip((int)(((-item2_1 * item1_2) + (item3_1 * item2_2)) / (w * ((item3_1 * item1_1) - item3_3)) + 0.5), 16, 240);

}

double up_val(const double& u, const double& v, const int& yy, const int& xx, const double& sups, Mat& Plane)
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
	calculate -= BC_funtion(u) * BC_funtion(v) * Plane.at<double>(yy, xx);
	calculate += BC_funtion(u) * BC_funtion(v) * sups;
	//calculate = Clip((int)(calculate+0.5),16,240);
	//cout << calculate << endl;
	return calculate;
}

void BAYER_iteration_3X3(int yy, int xx, double M0, double M1, double M2, double M3, double& error_T, double Y0, double Y1, double Y2, double Y3, Mat& DS_Cb_plane, Mat& DS_Cr_plane, double& des_Cb, double& des_Cr, int y, int x, Mat& U, Mat& V)
{
	//cout << "Bayer_iteration_3X3" << endl;
	double rec_M0, rec_M1, rec_M2, rec_M3;
	double tmp_Cb, tmp_Cr;
	double can_Cb, can_Cr;
	double error = 0;


	double theLeftTopCb, theLeftDownCb, theRightTopCb, theRightDownCb;
	double theLeftTopCr, theLeftDownCr, theRightTopCr, theRightDownCr;

	double rec_R0, rec_G0, rec_B0, rec_W0;
	double rec_R1, rec_G1, rec_B1, rec_W1;
	double rec_R2, rec_G2, rec_B2, rec_W2;
	double rec_R3, rec_G3, rec_B3, rec_W3;
	double new_dis = 0;
	bool iter = true;
	double big_cb1, big_cr1, big_cb2, big_cr2, big_cb3, big_cr3, big_cb4, big_cr4;
	while (iter) {
		iter = false;
		can_Cb = des_Cb;
		can_Cr = des_Cr;
		//cout << error_T << endl;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if ((i != 0) | (j != 0)) {
					tmp_Cb = can_Cb + j;
					tmp_Cr = can_Cr - i;

					if ((16 <= tmp_Cb && tmp_Cb <= 240) && (16 <= tmp_Cr && tmp_Cr <= 240)) {

						big_cb1 = up_val(0.25, -0.25, yy, xx, tmp_Cb, DS_Cb_plane);
						big_cr1 = up_val(0.25, -0.25, yy, xx, tmp_Cr, DS_Cr_plane);
						big_cb2 = up_val(-0.25, -0.25, yy, xx, tmp_Cb, DS_Cb_plane);
						big_cr2 = up_val(-0.25, -0.25, yy, xx, tmp_Cr, DS_Cr_plane);
						big_cb3 = up_val(0.25, 0.25, yy, xx, tmp_Cb, DS_Cb_plane);
						big_cr3 = up_val(0.25, 0.25, yy, xx, tmp_Cr, DS_Cr_plane);
						big_cb4 = up_val(-0.25, 0.25, yy, xx, tmp_Cb, DS_Cb_plane);
						big_cr4 = up_val(-0.25, 0.25, yy, xx, tmp_Cr, DS_Cr_plane);



						//Bayer
						rec_G0 = Clip((int)(Y0 - 0.213 * (big_cb1 - 128) - 0.534 * (big_cr1 - 128) + 0.5), 0, 255);//G
						rec_R1 = Clip((int)(Y1 + 1.793 * (big_cr2 - 128) + 0.5), 0, 255);//R				
						rec_B2 = Clip((int)(Y2 + 2.115 * (big_cb3 - 128) + 0.5), 0, 255);//B
						rec_G3 = Clip((int)(Y3 - 0.213 * (big_cb4 - 128) - 0.534 * (big_cr4 - 128) + 0.5), 0, 255);//G

						error = (M0 - rec_G0) * (M0 - rec_G0) + (M1 - rec_R1) * (M1 - rec_R1) + (M2 - rec_B2) * (M2 - rec_B2) + (M3 - rec_G3) * (M3 - rec_G3);
						//cout << error << endl;



						if (error < error_T)
						{
							des_Cb = tmp_Cb;
							des_Cr = tmp_Cr;
							error_T = error;
							iter = true;
						}
					}
				}
			}
		}
	}
}


bool BAYER_iteration_5X5(int yy, int xx, double M0, double M1, double M2, double M3, double& error_T, double Y0, double Y1, double Y2, double Y3, Mat& DS_Cb_plane, Mat& DS_Cr_plane, double& des_Cb, double& des_Cr, int y, int x, Mat& U, Mat& V)
{
	//cout << "=====Bayer_iteration_5X5=====" << endl;
	double tmp_Cb = 0, tmp_Cr = 0;
	double error = 0;
	int can_Cb = des_Cb;
	int can_Cr = des_Cr;
	double rec_R0, rec_G0, rec_B0, rec_W0;
	double rec_R1, rec_G1, rec_B1, rec_W1;
	double rec_R2, rec_G2, rec_B2, rec_W2;
	double rec_R3, rec_G3, rec_B3, rec_W3;
	double error_TH = error_T;
	bool iter = false;
	double big_cb1, big_cr1, big_cb2, big_cr2, big_cb3, big_cr3, big_cb4, big_cr4;

	double gradient[16][2] = { -2, -2, -2,-1,-2,0,-2,1,-2,2,
	-1,-2,-1,2,0,-2,0,2,1,-2,1,2,
	2,-2,2,-1,2,0,2,1,2,2 };

	for (int i = 0; i < sizeof(gradient) / sizeof(*gradient); i++)
	{
		tmp_Cb = can_Cb + gradient[i][0];
		tmp_Cr = can_Cr + gradient[i][1];

		if ((16 <= tmp_Cb && tmp_Cb <= 240) && (16 <= tmp_Cr && tmp_Cr <= 240)) {

			big_cb1 = up_val(0.25, -0.25, yy, xx, tmp_Cb, DS_Cb_plane);
			big_cr1 = up_val(0.25, -0.25, yy, xx, tmp_Cr, DS_Cr_plane);
			big_cb2 = up_val(-0.25, -0.25, yy, xx, tmp_Cb, DS_Cb_plane);
			big_cr2 = up_val(-0.25, -0.25, yy, xx, tmp_Cr, DS_Cr_plane);
			big_cb3 = up_val(0.25, 0.25, yy, xx, tmp_Cb, DS_Cb_plane);
			big_cr3 = up_val(0.25, 0.25, yy, xx, tmp_Cr, DS_Cr_plane);
			big_cb4 = up_val(-0.25, 0.25, yy, xx, tmp_Cb, DS_Cb_plane);
			big_cr4 = up_val(-0.25, 0.25, yy, xx, tmp_Cr, DS_Cr_plane);



			//Bayer
			rec_G0 = Clip((int)(Y0 - 0.213 * (big_cb1 - 128) - 0.534 * (big_cr1 - 128) + 0.5), 0, 255);//G
			rec_R1 = Clip((int)(Y1 + 1.793 * (big_cr2 - 128) + 0.5), 0, 255);//R				
			rec_B2 = Clip((int)(Y2 + 2.115 * (big_cb3 - 128) + 0.5), 0, 255);//B
			rec_G3 = Clip((int)(Y3 - 0.213 * (big_cb4 - 128) - 0.534 * (big_cr4 - 128) + 0.5), 0, 255);//G

			error = (M0 - rec_G0) * (M0 - rec_G0) + (M1 - rec_R1) * (M1 - rec_R1) + (M2 - rec_B2) * (M2 - rec_B2) + (M3 - rec_G3) * (M3 - rec_G3);


			if (error < error_T)
			{
				//cout << error << " " << error_T << endl;
				des_Cb = tmp_Cb;
				des_Cr = tmp_Cr;
				error_T = error;
				iter = true;
			}

		}
	}
	return iter;
}

void DownSample420_YCbCr_BIC_BAYER_iter2(int height, int width, Mat U, Mat V, Mat& DS_U_plane, Mat& DS_V_plane, Mat& imgrgbcp, Mat& rgbw, Mat& new_Y_plane)
{
	double Up1, Up2, Up3, Up4;
	double Vp1, Vp2, Vp3, Vp4;

	double B_cb = 2.115;
	double B_cr = 0;
	double G_cb = 0.213;
	double G_cr = 0.534;
	double R_cb = 0;
	double R_cr = 1.793;
	double W_cb = 0.6343;
	double W_cr = 0.4197;

	double item1_1 = 0; double item1_2 = 0;
	double item2_1 = 0; double item2_2 = 0;
	double item3_1 = 0; double item3_2; double item3_3 = 0;

	int yy = 0;
	int xx = 0;

	double R_pow_a = 0;
	double G_pow_a = 0.213 * 0.213;
	double B_pow_a = 2.115 * 2.115;

	double R_pow_b = 1.793 * 1.793;
	double G_pow_b = 0.534 * 0.534;
	double B_pow_b = 0;

	double R_mul_ab = 0;
	double G_mul_ab = 0.213 * 0.534;
	double B_mul_ab = 0;

	double sum_of_pow_a = G_pow_a + R_pow_a + B_pow_a + G_pow_a;
	double sum_of_pow_b = G_pow_b + R_pow_b + B_pow_b + G_pow_b;
	double sum_of_mul_ab = G_mul_ab + R_mul_ab + B_mul_ab + G_mul_ab;
	double C = sum_of_mul_ab * sum_of_mul_ab - (sum_of_pow_a * sum_of_pow_b);

	double AA, BB;
	double Cb_p1, Cb_p2, Cb_p3, Cb_p4;
	double Cr_p1, Cr_p2, Cr_p3, Cr_p4;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			AA = (G_pow_a * U.at<double>(y, x) + G_mul_ab * V.at<double>(y, x))
				+ (R_pow_a * U.at<double>(y, x + 1) + R_mul_ab * V.at<double>(y, x + 1))
				+ (B_pow_a * U.at<double>(y + 1, x) + B_mul_ab * V.at<double>(y + 1, x))
				+ (G_pow_a * U.at<double>(y + 1, x + 1) + G_mul_ab * V.at<double>(y + 1, x + 1));

			BB = (G_mul_ab * U.at<double>(y, x) + G_pow_b * V.at<double>(y, x))
				+ (R_mul_ab * U.at<double>(y, x + 1) + R_pow_b * V.at<double>(y, x + 1))
				+ (B_mul_ab * U.at<double>(y + 1, x) + B_pow_b * V.at<double>(y + 1, x))
				+ (G_mul_ab * U.at<double>(y + 1, x + 1) + G_pow_b * V.at<double>(y + 1, x + 1));

			DS_U_plane.at<double>(yy, xx) = Clip((sum_of_mul_ab * BB - sum_of_pow_b * AA) / C, 16, 240);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_mul_ab * AA - sum_of_pow_a * BB) / C, 16, 240);

			xx++;
		}
		yy++;
	}

	copyMakeBorder(DS_U_plane, DS_U_plane, 2, 2, 2, 2, BORDER_REPLICATE);
	copyMakeBorder(DS_V_plane, DS_V_plane, 2, 2, 2, 2, BORDER_REPLICATE);
	double M0, M1, M2, M3;
	yy = 2;
	for (int y = 0; y < height; y += 2)
	{
		xx = 2;
		for (int x = 0; x < width; x += 2)
		{

			Up1 = calculate_UV(0.25, -0.25, yy, xx, DS_U_plane, DS_V_plane, 1);
			Up2 = calculate_UV(0.25, 0.25, yy, xx, DS_U_plane, DS_V_plane, 1);
			Up3 = calculate_UV(-0.25, -0.25, yy, xx, DS_U_plane, DS_V_plane, 1);
			Up4 = calculate_UV(-0.25, 0.25, yy, xx, DS_U_plane, DS_V_plane, 1);
			Vp1 = calculate_UV(0.25, -0.25, yy, xx, DS_U_plane, DS_V_plane, 2);
			Vp2 = calculate_UV(0.25, 0.25, yy, xx, DS_U_plane, DS_V_plane, 2);
			Vp3 = calculate_UV(-0.25, -0.25, yy, xx, DS_U_plane, DS_V_plane, 2);
			Vp4 = calculate_UV(-0.25, 0.25, yy, xx, DS_U_plane, DS_V_plane, 2);

			//cout << weightSum(0, 0) << "" << weightSum(0, 0.5) << "" << weightSum(0.5, 0) << "" << weightSum(0.5, 0.5) << endl;
			vector<double> diff_cb = { U.at<double>(y, x) - Up1,U.at<double>(y, x + 1) - Up2,U.at<double>(y + 1, x) - Up3,U.at<double>(y + 1, x + 1) - Up4 };
			vector<double> diff_cr = { V.at<double>(y, x) - Vp1,V.at<double>(y, x + 1) - Vp2,V.at<double>(y + 1, x) - Vp3,V.at<double>(y + 1, x + 1) - Vp4 };
			double w = BC_funtion(0.25) * BC_funtion(0.25);

			//--------解 U V--------

			//Bayer
			vector<pixel> B;
			B = { pixel{ G_cb,G_cr},pixel{ R_cb,R_cr },pixel{ B_cb,B_cr },pixel{ G_cb,G_cr} };
			cal_cbcr(DS_U_plane, DS_V_plane, yy, xx, B, diff_cb, diff_cr, w);

			xx++;
			//cout << DS_U_plane.at<double>(yy, xx) << " " << DS_V_plane.at<double>(yy, xx) << endl;
		}
		yy++;
	}

	double theLeftTopCb, theLeftDownCb, theRightTopCb, theRightDownCb;
	double theLeftTopCr, theLeftDownCr, theRightTopCr, theRightDownCr;
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;


	double rec_R0, rec_G0, rec_B0, rec_W0;
	double rec_R1, rec_G1, rec_B1, rec_W1;
	double rec_R2, rec_G2, rec_B2, rec_W2;
	double rec_R3, rec_G3, rec_B3, rec_W3;

	double Y0, Y1, Y2, Y3;
	double tmp_cb, tmp_cr, error = 0;
	double simpleArrayY[256];

	for (int arra = 16; arra < 236; arra++)
	{
		simpleArrayY[arra] = 1.164 * (arra - 16);
	}
	/*迭代*/
	yy = 2;
	for (int y = 0; y < height; y += 2)
	{
		xx = 2;
		for (int x = 0; x < width; x += 2)
		{
			tmp_cb = DS_U_plane.at<double>(yy, xx);
			tmp_cr = DS_V_plane.at<double>(yy, xx);

			/*梯度下降*/
			/*算錯誤率*/
			M0 = (int)rgbw.at<double>(y, x);
			M1 = (int)rgbw.at<double>(y, x + 1);
			M2 = (int)rgbw.at<double>(y + 1, x);
			M3 = (int)rgbw.at<double>(y + 1, x + 1);

			//cout << M0 << " " << M1 << " " << M2 << " " << M3 << endl;

			Y0 = simpleArrayY[(int)new_Y_plane.at<double>(y, x)];
			Y1 = simpleArrayY[(int)new_Y_plane.at<double>(y, x + 1)];
			Y2 = simpleArrayY[(int)new_Y_plane.at<double>(y + 1, x)];
			Y3 = simpleArrayY[(int)new_Y_plane.at<double>(y + 1, x + 1)];

			double big_cb1, big_cr1, big_cb2, big_cr2, big_cb3, big_cr3, big_cb4, big_cr4;

			big_cb1 = up_val(0.25, -0.25, yy, xx, tmp_cb, DS_U_plane);
			big_cr1 = up_val(0.25, -0.25, yy, xx, tmp_cr, DS_V_plane);
			big_cb2 = up_val(-0.25, -0.25, yy, xx, tmp_cb, DS_U_plane);
			big_cr2 = up_val(-0.25, -0.25, yy, xx, tmp_cr, DS_V_plane);
			big_cb3 = up_val(0.25, 0.25, yy, xx, tmp_cb, DS_U_plane);
			big_cr3 = up_val(0.25, 0.25, yy, xx, tmp_cr, DS_V_plane);
			big_cb4 = up_val(-0.25, 0.25, yy, xx, tmp_cb, DS_U_plane);
			big_cr4 = up_val(-0.25, 0.25, yy, xx, tmp_cr, DS_V_plane);


			//Bayer
			rec_G0 = Clip((int)(Y0 - 0.213 * (big_cb1 - 128) - 0.534 * (big_cr1 - 128) + 0.5), 0, 255);//G
			rec_R1 = Clip((int)(Y1 + 1.793 * (big_cr2 - 128) + 0.5), 0, 255);//R				
			rec_B2 = Clip((int)(Y2 + 2.115 * (big_cb3 - 128) + 0.5), 0, 255);//B
			rec_G3 = Clip((int)(Y3 - 0.213 * (big_cb4 - 128) - 0.534 * (big_cr4 - 128) + 0.5), 0, 255);//G

			//cout << Y1 << " " << 1.164 * (new_Y_plane.at<double>(y, x+1) - 16) << endl;
			error = (M0 - rec_G0) * (M0 - rec_G0) + (M1 - rec_R1) * (M1 - rec_R1) + (M2 - rec_B2) * (M2 - rec_B2) + (M3 - rec_G3) * (M3 - rec_G3);


			if (error == 0)
			{
				DS_U_plane.at<double>(yy, xx) = tmp_cb;
				DS_V_plane.at<double>(yy, xx) = tmp_cr;

				xx++;
				continue;
			}

			//BAYER_iteration_3X3(yy, xx, M0, M1, M2, M3, error, Y0, Y1, Y2, Y3, DS_U_plane, DS_V_plane, tmp_cb, tmp_cr, y, x, U, V);
			bool iter = true;
			while (iter)
			{
				//cout << "==================" << U.at<double>(y, x) << "==================" << endl;
				BAYER_iteration_3X3(yy, xx, M0, M1, M2, M3, error, Y0, Y1, Y2, Y3, DS_U_plane, DS_V_plane, tmp_cb, tmp_cr, y, x, U, V);
				//還沒看5x5
				iter = BAYER_iteration_5X5(yy, xx, M0, M1, M2, M3, error, Y0, Y1, Y2, Y3, DS_U_plane, DS_V_plane, tmp_cb, tmp_cr, y, x, U, V);
				//cout << error << endl;
			}

			DS_U_plane.at<double>(yy, xx) = tmp_cb;
			DS_V_plane.at<double>(yy, xx) = tmp_cr;

			xx++;
		}
		yy++;
	}

	DS_U_plane = DS_U_plane(Rect(2, 2, width / 2, height / 2));
	DS_V_plane = DS_V_plane(Rect(2, 2, width / 2, height / 2));
}





