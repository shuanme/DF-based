#include "Subsample.h"
#define ck_showtxt 0
int mirror(int edge, int index)
{
	if (index < 0)
		index = -index;
	else if (index >= edge)
		index = edge - (index - edge + 2); // (edge - 1) - (index - (edge - 1))
	return index;
}
int mir(int x, int edge) {
	if (x < 0)
		x = -x;
	if (x >= edge)
		x = edge - (x - edge + 2);
	return x;
}
void DownSample_420Yang(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& U_plane, const Mat& V_plane)
{
	int yy = 0;
	int xx = 0;

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			//DS_U_plane.at<double>(yy, xx) = (uchar)round((U_plane.at<double>(mirror(height, y), mirror(width, x)) + U_plane.at<double>(mirror(height, y + 1), mirror(width, x))) / 2);
			//DS_V_plane.at<double>(yy, xx) = (uchar)round((V_plane.at<double>(mirror(height, y), mirror(width, x + 1)) + V_plane.at<double>(mirror(height, y + 1), mirror(width, x + 1))) / 2);

			DS_U_plane.at<double>(yy, xx) = (int)(U_plane.at<double>(mirror(height, y + 1), mirror(width, x)) + 0.5);
			DS_V_plane.at<double>(yy, xx) = (int)(V_plane.at<double>(mirror(height, y), mirror(width, x + 1)) + 0.5);


			xx++;
		}
		yy++;
	}
}
void DownSample_420A(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
	int hei = height / 2;
	int wid = width / 2;
	int x;
	int y = 0;
	for (int yy = 0; yy < hei; yy++)
	{
		x = 0;
		for (int xx = 0; xx < wid; xx++)
		{

			DS_U_plane.at<double>(yy, xx) += U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) += V_plane.at<double>(y, x);
			x++;
			DS_U_plane.at<double>(yy, xx) += U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) += V_plane.at<double>(y, x);
			x++;
		}
		y++;
		x = 0;
		for (int xx = 0; xx < wid; xx++)
		{

			DS_U_plane.at<double>(yy, xx) += U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) += V_plane.at<double>(y, x);
			x++;
			DS_U_plane.at<double>(yy, xx) = (int)((DS_U_plane.at<double>(yy, xx) + U_plane.at<double>(y, x)) / 4 + 0.5);
			DS_V_plane.at<double>(yy, xx) = (int)((DS_V_plane.at<double>(yy, xx) + V_plane.at<double>(y, x)) / 4 + 0.5);
			x++;
		}
		y++;

	}

	//都取floor
	/*int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(y, x) + U_plane.at<double>(y, x + 1) + U_plane.at<double>(y + 1, x) + U_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(y, x) + V_plane.at<double>(y, x + 1) + V_plane.at<double>(y + 1, x) + V_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_U_plane.at<double>(yy, xx) = (int)DS_U_plane.at<double>(yy, xx);
			DS_V_plane.at<double>(yy, xx) = (int)DS_V_plane.at<double>(yy, xx);
			xx++;
		}
		yy++;
	}*/


}
void DownSample_420L(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	int yy = 0;
	int xx = 0;

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = ((U_plane.at<double>(y, x) + U_plane.at<double>(y + 1, x)) / 2 );
			DS_V_plane.at<double>(yy, xx) = ((V_plane.at<double>(y, x) + V_plane.at<double>(y + 1, x)) / 2 );



			xx++;
		}
		yy++;
	}
}
void DownSample_420R(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
	int yy = 0;
	int xx = 0;

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = (int)((U_plane.at<double>(y, x + 1) + U_plane.at<double>(y + 1, x + 1)) / 2 + 0.5);
			DS_V_plane.at<double>(yy, xx) = (int)((V_plane.at<double>(y, x + 1) + V_plane.at<double>(y + 1, x + 1)) / 2 + 0.5);

			xx++;
		}
		yy++;
	}

}

void DownSample_420Direct(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
	int yy = 0;
	int xx = 0;

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = (int)(U_plane.at<double>(y, x) + 0.5);
			DS_V_plane.at<double>(yy, xx) = (int)(V_plane.at<double>(y, x) + 0.5);

			xx++;
		}
		yy++;
	}

	/*int a = 0, b = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (i % 2 == 0 && j % 2 == 0) {
				DS_U_plane.at<double>(a, b) = U_plane.at<double>(i, j);
				DS_V_plane.at<double>(a, b) = V_plane.at<double>(i, j);
				if (b == (width / 2) - 1) {
					a++;
					b = 0;
				}
				else
					b++;
			}
		}*/
	//cout<<"d";

}
void DownSample_420MPEG_B(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


	Mat coef = (Mat_<double>(1, 13) << 2, 0, -4, -3, 5, 19, 26, 19, 5, -3, -4, 0, 2);

	int yy = 0;
	int xx = 0;
	double U_DS_Val;
	double V_DS_Val;
	int index;

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			U_DS_Val = 0;
			V_DS_Val = 0;
			index = 0;

			for (int offset = -6; offset <= 6; offset++)
			{
				U_DS_Val += U_plane.at<double>(mirror(height, y), mirror(width, x + offset)) * coef.at<double>(0, index);
				V_DS_Val += V_plane.at<double>(mirror(height, y), mirror(width, x + offset)) * coef.at<double>(0, index);
				index++;
			}
			U_DS_Val /= 64;
			V_DS_Val /= 64;

			DS_U_plane.at<double>(yy, xx) = (int)(U_DS_Val + 0.5);
			DS_V_plane.at<double>(yy, xx) = (int)(V_DS_Val + 0.5);

			xx++;
		}
		yy++;
	}
}


void DownSample_420Chen(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


	int yy = 0;
	int xx;
	int y, x;

	for (y = 0; y < height; y += 2)
	{
		xx = 0;
		for (x = 0; x < width; x += 2)
		{

			DS_U_plane.at<double>(yy, xx) = (int)(U_plane.at<double>(y + 1, x) + 0.5);

			DS_V_plane.at<double>(yy, xx) = (int)(V_plane.at<double>(y, x + 1) + 0.5);
			xx++;
		}
		yy++;
	}

}


void DownSample_420Lin_Copy(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{


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
	double ds_u, ds_v;
	int yy = 0;
	int xx = 0;
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

			ds_u = Clip((int)(((sum_of_pow_b*AA) - (sum_of_mul_ab*BB)) / C + 0.5), 0, 255);
			ds_v = Clip((int)(((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / D + 0.5), 0, 255);
			//ds_v = Clip((int)(((sum_of_mul_ab*AA) - (sum_of_pow_a*BB)) / C + 0.5), 0, 255);

			/*DS_U_plane.at<double>(yy, xx) = Clip((sum_of_pow_b*AA - sum_of_mul_ab * BB) / C, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((sum_of_pow_a*BB - sum_of_mul_ab * AA) / C, 0, 255);*/

			DS_U_plane.at<double>(yy, xx) = ds_u;
			DS_V_plane.at<double>(yy, xx) = ds_v;


			xx++;
		}
		yy++;
	}


}
void DownSample_420GBCS_Copy(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
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
	//int roun, totalRoun = 0;
	int error;
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
	for (y = 0; y < height; y += 2)
	{
		xx = 0;
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

			simpleYb = simpleArrayY[0][Yb];
			simpleYr = simpleArrayY[0][Yr];
			simpleYleftTop = simpleArrayY[0][YleftTop];//1.164 * (YleftTop - 16);
			simpleYrightDown = simpleArrayY[0][YrightDown];//1.164 * (YrightDown - 16);

			Ub = ((B - (Yb * 1.164 - Ymult)) / 2.018) + 128;
			if (Ub < 0) Ub = 0;
			else if (Ub > 255) Ub = 255;
			UbFloor = (int)floor(Ub);
			UbCeil = (int)ceil(Ub);
			/*---------------------------------------------------------------------------------------*/
			Vr = ((R - (Yr * 1.164 - Ymult)) / 1.596) + 128;
			if (Vr < 0) Vr = 0;
			else if (Vr > 255) Vr = 255;
			VbFloor = (int)floor(Vr);
			VbCeil = (int)ceil(Vr);
			/*-------------------------------------*/
			Ug_leftTop = (GleftTop - 1.164*(YleftTop)+Ymult + 0.813*(Vr)-Vmult) / (-0.391) + 128;// 0.813*(VleftTop - 128))
			if (Ug_leftTop < 0) Ug_leftTop = 0;
			else if (Ug_leftTop > 255) Ug_leftTop = 255;
			Ug_leftTopFloor = (int)floor(Ug_leftTop);
			Ug_leftTopCeil = (int)ceil(Ug_leftTop);
			/*-------------------------------------*/
			Ug_rightDown = (GrightDown - 1.164*(YrightDown)+Ymult + 0.813*(Vr)-Vmult) / (-0.391) + 128;
			if (Ug_rightDown < 0) Ug_rightDown = 0;
			else if (Ug_rightDown > 255) Ug_rightDown = 255;
			Ug_rightDownFloor = (int)floor(Ug_rightDown);
			Ug_rightDownCeil = (int)ceil(Ug_rightDown);

			/*-------------------------------------*/
			Vg_leftTop = (GleftTop - 1.164*(YleftTop)+Ymult + 0.391*(Ub)-Umult) / (-0.813) + 128;
			if (Vg_leftTop < 0) Vg_leftTop = 0;
			else if (Vg_leftTop > 255) Vg_leftTop = 255;
			Vg_leftTopFloor = (int)floor(Vg_leftTop);
			Vg_leftTopCeil = (int)ceil(Vg_leftTop);
			/*-------------------------------------*/
			Vg_rightDown = (GrightDown - 1.164*(YrightDown)+Ymult + 0.391*(Ub)-Umult) / (-0.813) + 128;
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
			int error_TH = INT_MAX;
			//error = 0;
			//roun = (all[5] - all[0] + 1)*(all2[5] - all2[0] + 1);
			/*if (roun>300 && roun<400)
				cout << y << " " << x << endl;*/
				/*if (y == chooseY && x == chooseX)//122 306
				{
					cout << (int)BGR.at<Vec3b>(y, x)[0] << " " << (int)BGR.at<Vec3b>(y, x)[1] << " " << (int)BGR.at<Vec3b>(y, x)[2]<<endl;
					cout << (int)BGR.at<Vec3b>(y, x+1)[0] << " " << (int)BGR.at<Vec3b>(y, x+1)[1] << " " << (int)BGR.at<Vec3b>(y, x+1)[2] << endl;
					cout << (int)BGR.at<Vec3b>(y+1, x)[0] << " " << (int)BGR.at<Vec3b>(y+1, x)[1] << " " << (int)BGR.at<Vec3b>(y+1, x)[2] << endl;
					cout << (int)BGR.at<Vec3b>(y+1, x+1)[0] << " " << (int)BGR.at<Vec3b>(y+1, x+1)[1] << " " << (int)BGR.at<Vec3b>(y+1, x+1)[2] << endl;
					cout <<"YUV:"<< endl;
					cout << Y_plane.at<double>(y, x) << " " << U_plane.at<double>(y, x) << " " << V_plane.at<double>(y, x) << endl;
					cout << Y_plane.at<double>(y, x+1) << " " << U_plane.at<double>(y, x+1) << " " << V_plane.at<double>(y, x+1) << endl;
					cout << Y_plane.at<double>(y+1, x) << " " << U_plane.at<double>(y+1, x) << " " << V_plane.at<double>(y+1, x) << endl;
					cout << Y_plane.at<double>(y+1, x+1) << " " << U_plane.at<double>(y+1, x+1) << " " << V_plane.at<double>(y+1, x+1) << endl;
					cout << "------------" << endl;
					cout << all[0] << " " << all[5] << " " << all2[0] << " " << all2[5] << endl;
					cout << Ub << " " << Vr << "\nUlt" << Ug_leftTop << " Urd" << Ug_rightDown << "\nVlt" << Vg_leftTop << " Vrd" << Vg_rightDown << endl;
				}*/
				//fr2 << roun << endl;
				//fr2 << roun<< endl;
				//fr2 << "y:" << y << " x:" << x << " " << (all[5] - all[0] + 1) << " " << (all2[5] - all2[0] + 1) << endl;
			if (all[5] - all[0] >= all2[5] - all2[0])
			{
				for (move_U = all[0]; move_U <= all[5]; move_U++)
				{
					//fr2 << move_U << ":  ";
					rec_B = (int)(simpleYb + simpleArrayU[1][move_U] + 0.5);
					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;
					simpleU = simpleArrayU[0][move_U];//-0.391 * (move_U - 128);
					for (move_V = all2[0]; move_V <= all2[5]; move_V++)
					{

						rec_G1 = (int)(simpleYleftTop + simpleU + simpleArrayV[0][move_V] + 0.5);//(int)(simpleYleftTop + simpleU - 0.813*(move_V - 128) + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;
						rec_G2 = (int)(simpleYrightDown + simpleU + simpleArrayV[0][move_V] + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;
						rec_R = (int)(simpleYr + simpleArrayV[1][move_V] + 0.5);//(int)(simpleYr + 1.596*(move_V - 128) + 0.5);
						if (rec_R < 0) rec_R = 0;
						else if (rec_R > 255) rec_R = 255;



						/*error += ((B - rec_B >= 0) ? simpleArray[B - rec_B] : simpleArray[rec_B - B]);
						error += ((GleftTop - rec_G1 >= 0) ? simpleArray[GleftTop - rec_G1] : simpleArray[rec_G1 - GleftTop]);
						error += ((GrightDown - rec_G2 >= 0) ? simpleArray[GrightDown - rec_G2] : simpleArray[rec_G2 - GrightDown]);
						error += ((R - rec_R >= 0) ? simpleArray[R - rec_R] : simpleArray[rec_R - R]);*/
						/*error = ((B - rec_B >= 0) ? simpleError[0][B - rec_B] : simpleError[0][rec_B - B]) + ((GleftTop - rec_G1 >= 0) ? simpleError[0][GleftTop - rec_G1] : simpleError[0][rec_G1 - GleftTop])
						+ ((GrightDown - rec_G2 >= 0) ? simpleError[0][GrightDown - rec_G2] : simpleError[0][rec_G2 - GrightDown]) + ((R - rec_R >= 0) ? simpleError[0][R - rec_R] : simpleError[0][rec_R - R])*/;
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
				for (move_V = all2[0]; move_V <= all2[5]; move_V++)//move_V = all2[0]; move_V <= all2[5]; move_V++
				{
					//fr2 << move_U << ":  ";
					rec_R = (int)(simpleYr + simpleArrayV[1][move_V] + 0.5);//(int)(simpleYr + 1.596*(move_V - 128) + 0.5);
					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;
					simpleV = simpleArrayV[0][move_V];
					for (move_U = all[0]; move_U <= all[5]; move_U++)
					{
						rec_B = (int)(simpleYb + simpleArrayU[1][move_U] + 0.5);
						if (rec_B < 0) rec_B = 0;
						else if (rec_B > 255) rec_B = 255;
						rec_G1 = (int)(simpleYleftTop + simpleArrayU[0][move_U] + simpleV + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;
						rec_G2 = (int)(simpleYrightDown + simpleArrayU[0][move_U] + simpleV + 0.5);
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
			//frY << (int)round(DS_U_plane.at<double>(yy, xx)) << endl;
			//frYx << (int)round(DS_V_plane.at<double>(yy, xx)) << endl;
			/*if (roun>10)
			totalRoun += roun;*/
			//totalRoun += roun;

			if (xx == 128)
				cout << DS_U_plane.at<double>(yy, xx) << " " << DS_V_plane.at<double>(yy, xx) << " " << error_TH << endl;
			xx++;
		}
		yy++;
	}
	//cout << (double)totalRoun / ((double)(height*width) / (4.0)) << endl;
	//frY.close();
	//frYx.close();
	//frY.close();
	//fr2.close();
}
void DownSample_420GDCS_Copy(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{

	double simpleArrayU[2][256], simpleArrayV[2][256], simpleArrayY[1][256];
	int arra;
	for (arra = 0; arra < 256; arra++)
	{
		simpleArrayU[0][arra] = -0.391*(arra - 128);//G
		simpleArrayU[1][arra] = 2.018*(arra - 128);//B
		simpleArrayV[0][arra] = -0.813*(arra - 128);//G
		simpleArrayV[1][arra] = 1.596*(arra - 128);//R
		simpleArrayY[0][arra] = 1.164*(arra - 16);//Y

	}
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;

	int R, GleftTop, GrightDown, B, Yb, Yr, YleftTop, YrightDown;

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

	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;

	double last_error;
	int same_num;

	int can_U, can_V;
	double AA, BB;
	int yy = 0;
	int xx = 0;
	int y, x;
	for (y = 0; y < height; y += 2)
	{

		xx = 0;
		for (x = 0; x < width; x += 2)
		{



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




			M0 = (int)BGR.at<Vec3b>(y, x)[1];
			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];


			YleftTop = Y0 = (int)(Y_plane.at<double>(y, x) + 0.5);
			Yr = Y1 = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
			Yb = Y2 = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
			YrightDown = Y3 = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
			simpleYb = simpleArrayY[0][Yb];
			simpleYr = simpleArrayY[0][Yr];
			simpleYleftTop = simpleArrayY[0][YleftTop];
			simpleYrightDown = simpleArrayY[0][YrightDown];



			tmp_U = des_U;
			tmp_V = des_V;
			rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
			rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
			rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
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
					rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
					for (int j = -1; j <= 1; j++) {

						tmp_U = des_U + j;

						if (0 <= tmp_U && tmp_U <= 255) {
							rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
							rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
							rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
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



			/*last_error = error_TH;
			same_num = 0;*/

			while ((n_i != 0 || n_j != 0) && error_TH != 0) {


				tmp_i = 0;
				tmp_j = 0;
				for (int i = -1; i <= 1; i++) {
					tmp_V = des_V - i;
					if (0 <= tmp_V && tmp_V <= 255) {
						rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
						for (int j = -1; j <= 1; j++) {
							old_i = i + n_i;
							old_j = j + n_j;

							if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

								tmp_U = des_U + j;

								if (0 <= tmp_U && tmp_U <= 255) {

									rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
									rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
									rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
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




}

void DownSample_420MMCS_Copy(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane, int number)
{
	//int s_y = 418;
	//int s_x = 1744;

	int sortI, sortJ;
	int yy = 0;
	int xx = 0;


	//char filenameE[] = "UbUs_E.txt";
	//fstream fpE;
	//fpE.open(filenameE, ios::out);
	/*char filenameU[] = "UbUs_U.txt";
	fstream fpU;
	fpU.open(filenameU, ios::out);*/
	//char filenameV[] = "UbUs_V.txt";
	//fstream fpV;
	//fpV.open(filenameV, ios::out);

	int R, GleftTop, GrightDown, B, Yb, Yr, YleftTop, YrightDown;
	int y, x;
	double Ub, Vr, Ug_leftTop, Ug_rightDown, Vg_leftTop, Vg_rightDown;
	int UbFloor, UbCeil, VbFloor, VbCeil, Ug_leftTopFloor, Ug_leftTopCeil, Ug_rightDownFloor, Ug_rightDownCeil, Vg_leftTopFloor, Vg_leftTopCeil, Vg_rightDownFloor, Vg_rightDownCeil;
	int rec_B, rec_G1, rec_G2, rec_R;
	int move_U, move_V;
	int roun;
	//double mean_range = 0;
	int error;
	double simpleU, simpleV;
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;
	double simpleArrayU[2][256], simpleArrayV[2][256], simpleArrayY[1][256];
	double U1, U2, U3, U4, V1, V2, V3, V4;
	double Umin, Umax, Vmin, Vmax;
	int UminInteger, UmaxInteger, VminInteger, VmaxInteger;

	int arra;
	for (arra = 0; arra < 256; arra++)
	{
		simpleArrayU[0][arra] = -0.391*(arra - 128);//G
		simpleArrayU[1][arra] = 2.018*(arra - 128);//B
		simpleArrayV[0][arra] = -0.813*(arra - 128);//G
		simpleArrayV[1][arra] = 1.596*(arra - 128);//R
		simpleArrayY[0][arra] = 1.164*(arra - 16);//Y

	}


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

			Yb = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
			Yr = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
			YleftTop = (int)(Y_plane.at<double>(y, x) + 0.5);
			YrightDown = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);

			simpleYb = simpleArrayY[0][Yb];
			simpleYr = simpleArrayY[0][Yr];
			simpleYleftTop = simpleArrayY[0][YleftTop];
			simpleYrightDown = simpleArrayY[0][YrightDown];


			int error_TH = INT_MAX;

			//mean_range += (UmaxInteger - UminInteger + 1)*(VmaxInteger - VminInteger + 1);

			if (UmaxInteger - UminInteger >= VmaxInteger - VminInteger)
			{
				for (move_U = UminInteger; move_U <= UmaxInteger; move_U++)
				{

					rec_B = (int)(simpleYb + simpleArrayU[1][move_U] + 0.5);
					if (rec_B < 0) rec_B = 0;
					else if (rec_B > 255) rec_B = 255;
					simpleU = simpleArrayU[0][move_U];
					for (move_V = VminInteger; move_V <= VmaxInteger; move_V++)
					{

						rec_G1 = (int)(simpleYleftTop + simpleU + simpleArrayV[0][move_V] + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;
						rec_G2 = (int)(simpleYrightDown + simpleU + simpleArrayV[0][move_V] + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;
						rec_R = (int)(simpleYr + simpleArrayV[1][move_V] + 0.5);
						if (rec_R < 0) rec_R = 0;
						else if (rec_R > 255) rec_R = 255;



						error = (B - rec_B)*(B - rec_B) + (GleftTop - rec_G1)*(GleftTop - rec_G1) + (GrightDown - rec_G2)*(GrightDown - rec_G2) + (R - rec_R)*(R - rec_R);
						if (error < error_TH)
						{
							error_TH = error;
							DS_U_plane.at<double>(yy, xx) = (uchar)move_U;
							DS_V_plane.at<double>(yy, xx) = (uchar)move_V;
						}


					}

				}

			}
			else {
				for (move_V = VminInteger; move_V <= VmaxInteger; move_V++)
				{

					rec_R = (int)(simpleYr + simpleArrayV[1][move_V] + 0.5);
					if (rec_R < 0) rec_R = 0;
					else if (rec_R > 255) rec_R = 255;
					simpleV = simpleArrayV[0][move_V];
					for (move_U = UminInteger; move_U <= UmaxInteger; move_U++)
					{
						rec_B = (int)(simpleYb + simpleArrayU[1][move_U] + 0.5);
						if (rec_B < 0) rec_B = 0;
						else if (rec_B > 255) rec_B = 255;
						rec_G1 = (int)(simpleYleftTop + simpleArrayU[0][move_U] + simpleV + 0.5);
						if (rec_G1 < 0) rec_G1 = 0;
						else if (rec_G1 > 255) rec_G1 = 255;
						rec_G2 = (int)(simpleYrightDown + simpleArrayU[0][move_U] + simpleV + 0.5);
						if (rec_G2 < 0) rec_G2 = 0;
						else if (rec_G2 > 255) rec_G2 = 255;




						error = (B - rec_B)*(B - rec_B) + (GleftTop - rec_G1)*(GleftTop - rec_G1) + (GrightDown - rec_G2)*(GrightDown - rec_G2) + (R - rec_R)*(R - rec_R);
						if (error < error_TH)
						{
							error_TH = error;
							DS_U_plane.at<double>(yy, xx) = (uchar)move_U;
							DS_V_plane.at<double>(yy, xx) = (uchar)move_V;
						}

					}

				}
			}

			xx++;
		}
		yy++;

	}
	//cout << (double)mean_range / ((double)(height*width) / (4.0)) << endl;

}



void DownSample_420ES_Copy(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane) {


	double simpleArrayU[2][256], simpleArrayV[2][256], simpleArrayY[1][256];
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
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;

	int R, GleftTop, GrightDown, B, Yb, Yr, YleftTop, YrightDown;
	int M0, M1, M2, M3;
	double rec_M0, rec_M1, rec_M2, rec_M3;
	int Y0, Y1, Y2, Y3;

	int tmp_U, tmp_V;
	int can_U, can_V;
	double error, error_TH;



	int xx;
	int yy = 0;

	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2) {

			M0 = (int)BGR.at<Vec3b>(y, x)[1];
			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];


			YleftTop = Y0 = (int)(Y_plane.at<double>(y, x) + 0.5);
			Yr = Y1 = (int)(Y_plane.at<double>(y, x + 1) + 0.5);
			Yb = Y2 = (int)(Y_plane.at<double>(y + 1, x) + 0.5);
			YrightDown = Y3 = (int)(Y_plane.at<double>(y + 1, x + 1) + 0.5);
			simpleYb = simpleArrayY[0][Yb];
			simpleYr = simpleArrayY[0][Yr];
			simpleYleftTop = simpleArrayY[0][YleftTop];
			simpleYrightDown = simpleArrayY[0][YrightDown];




			error_TH = INT_MAX;
			for (int u = 0; u <= 255; u++) {
				for (int v = 0; v <= 255; v++) {
					tmp_U = u;
					tmp_V = v;
					rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
					rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
					rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
					rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
					error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);
					if (error < error_TH)
					{

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

}
void DownSample_420MPCS_Copy(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
	double sY[256], sU[2][256], sV[2][256];


	for (int arra = 0; arra < 256; arra++)
	{
		sU[0][arra] = -0.391*(arra - 128);//G
		sV[1][arra] = 1.596*(arra - 128);//R
		sU[1][arra] = 2.018*(arra - 128);//B
		sV[0][arra] = -0.813*(arra - 128);//G

		sY[arra] = 1.164*arra;//Y

	}


	Mat X, b;
	//Mat A = (Mat_<double>(4, 2) << -0.391, -0.813,//G
	//								0,  1.596,//R
	//							2.018,      0,//B
	//						   -0.391, -0.813);//G

	Mat A_inv = (Mat_<double>(2, 4) << -0.06023247, -0.06136466, 0.47219931, -0.06023247,
		-0.20022626, 0.4225765, -0.07759016, -0.20022626);
	//cout << A_inv.at<double>(0, 0) << " " << A_inv.at<double>(0, 1) << " " << A_inv.at<double>(0, 2) << " " << A_inv.at<double>(0, 3) << endl;
	//cout << A_inv.at<double>(1, 0) << " " << A_inv.at<double>(1, 1) << " " << A_inv.at<double>(1, 2) << " " << A_inv.at<double>(1, 3) << endl;


	double sY0, sY1, sY2, sY3;
	int M0, M1, M2, M3;
	int rec_M0, rec_M1, rec_M2, rec_M3;
	int xx, yy;
	int error, error_TH;
	int u, v;
	int u_tmp, v_tmp;
	int lu, lv;
	yy = 0;
	for (int y = 0; y < height; y += 2) {
		xx = 0;
		for (int x = 0; x < width; x += 2) {
			M0 = (int)BGR.at<Vec3b>(y, x)[1];
			M1 = (int)BGR.at<Vec3b>(y, x + 1)[2];
			M2 = (int)BGR.at<Vec3b>(y + 1, x)[0];
			M3 = (int)BGR.at<Vec3b>(y + 1, x + 1)[1];


			sY0 = sY[(int)(Y_plane.at<double>(y, x) + 0.5)];
			sY1 = sY[(int)(Y_plane.at<double>(y, x + 1) + 0.5)];
			sY2 = sY[(int)(Y_plane.at<double>(y + 1, x) + 0.5)];
			sY3 = sY[(int)(Y_plane.at<double>(y + 1, x + 1) + 0.5)];
			b = (Mat_<double>(4, 1) << M0 - sY0 - 135.488, M1 - sY1 + 222.912, M2 - sY2 + 276.928, M3 - sY3 - 135.488);//G
			X = A_inv * b;
			//DS_U_plane.at<double>(yy, xx) = (int)(X.at<double>(0, 0) + 0.5);
			//DS_V_plane.at<double>(yy, xx) = (int)(X.at<double>(1, 0) + 0.5);
			/*	if ((xx == 15 && yy == 15) || (xx == 13 && yy == 18) || (xx == 13 && yy == 18) || (xx == 53 && yy == 18) || (xx == 33 && yy == 40) || (xx == 153 && yy == 186) || (xx == 113 && yy == 158) || (xx == 313 && yy == 148) || (xx == 113 && yy == 186) || (xx == 36 && yy == 148)) {
					cout << DS_U_plane.at<double>(yy, xx) << " " << DS_V_plane.at<double>(yy, xx) << endl;
				}*/
			sY0 = sY0 - 18.624;
			sY1 = sY1 - 18.624;
			sY2 = sY2 - 18.624;
			sY3 = sY3 - 18.624;

			lu = (int)X.at<double>(0, 0);
			lv = (int)X.at<double>(1, 0);
			error_TH = INT_MAX;
			for (u_tmp = lu - 2; u_tmp <= lu + 3; u_tmp++) {
				for (v_tmp = lv - 2; v_tmp <= lv + 3; v_tmp++) {
					rec_M0 = (int)(sY0 + sU[0][u_tmp] + sV[0][v_tmp] + 0.5);
					rec_M1 = (int)(sY1 + sV[1][v_tmp] + 0.5);//R
					rec_M2 = (int)(sY2 + sU[1][u_tmp] + 0.5);//B
					rec_M3 = (int)(sY3 + sU[0][u_tmp] + sV[0][v_tmp] + 0.5);//G
					error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);
					if (error < error_TH) {
						u = u_tmp;
						v = v_tmp;
						error_TH = error;
					}
					/*if (xx == 128 && yy == 27)
						cout << u << " " << v << " " << error << endl;*/
				}
			}
			DS_U_plane.at<double>(yy, xx) = u;
			DS_V_plane.at<double>(yy, xx) = v;
			//if(xx==128 && yy==27)
			//	cout << DS_U_plane.at<double>(yy, xx) << " " << DS_V_plane.at<double>(yy, xx) << " " << error_TH << endl;

			xx++;
		}
		yy++;
	}
}

void DownSample_Bayer_LMCD_Copy(Mat&YUV, Mat&Y, Mat&U, Mat&V, Mat&imgrgbcp, Mat&new_Y_plane) {
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
		0, 0, 0, 1.135, 0,
		0, 0, 1.164, -0.220, -0.4573);*/

	//解Y Y Us Vs
	/*Mat A4 = (Mat_<double>(4, 4) <<
		0, 0, -0.391, -0.813,
		0, 0, 0, 1.596,
		1.164, 0, 2.018, 0,
		0, 1.164, -0.391, -0.813);*/

	//只改Us Vs
	/*Mat A4 = (Mat_<double>(4, 2) <<
		-0.391, -0.813,
		0, 1.596,
		2.018, 0,
		-0.391, -0.813);*/

	//解Y Us Vs
	Mat A4 = (Mat_<double>(4, 3) <<
		0, -0.391, -0.813,
		0, 0, 1.596,
		0, 2.018, 0,
		1.164, -0.391, -0.813);



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

	Mat A_inv = (A4.t()*A4).inv(DECOMP_SVD)*A4.t();
	//Mat A_inv = A4.inv(DECOMP_SVD);

	double simpleArrayU[2][256], simpleArrayV[2][256], simpleArrayY[1][256];
	int arra;
	for (arra = 0; arra < 256; arra++)
	{
		simpleArrayU[0][arra] = -0.391*(arra - 128);//G
		simpleArrayU[1][arra] = 2.018*(arra - 128);//B
		simpleArrayV[0][arra] = -0.813*(arra - 128);//G
		simpleArrayV[1][arra] = 1.596*(arra - 128);//R
		simpleArrayY[0][arra] = 1.164*(arra - 16);//Y

	}
	double simpleYb, simpleYr, simpleYleftTop, simpleYrightDown;

	int R, GleftTop, GrightDown, B, Yb, Yr, YleftTop, YrightDown;

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

	int n_i, n_j;
	int old_i, old_j;
	int tmp_i, tmp_j;
	double error_TH;

	double last_error;
	int same_num;

	int can_U, can_V;
	double AA, BB;
	//int yy = 0;
	//int xx = 0;
	//int y, x;


	Mat X, YY;

	yy = 1;
	for (int y = 0; y < height; y += 2)
	{
		xx = 1;
		for (int x = 0; x < width; x += 2)
		{
			/*Up1 = (U.at<double>(yy - 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up2 = (U.at<double>(yy - 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy - 1, xx)) / 16;
			Up3 = (U.at<double>(yy + 1, xx - 1) + 3.0*U.at<double>(yy, xx - 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Up4 = (U.at<double>(yy + 1, xx + 1) + 3.0*U.at<double>(yy, xx + 1) + 3.0*U.at<double>(yy + 1, xx)) / 16;
			Vp1 = (V.at<double>(yy - 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp2 = (V.at<double>(yy - 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy - 1, xx)) / 16;
			Vp3 = (V.at<double>(yy + 1, xx - 1) + 3.0*V.at<double>(yy, xx - 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;
			Vp4 = (V.at<double>(yy + 1, xx + 1) + 3.0*V.at<double>(yy, xx + 1) + 3.0*V.at<double>(yy + 1, xx)) / 16;*/

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

			//解Y1 Y2 Y3 Y4 Us Vs
			//Y1 = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] + 0.391*Up1 + 0.813*Vp1 - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] - 1.596*Vp2 + 222.912,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] - 2.018*Up3 + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] + 0.391*Up4 + 0.813*Vp4 - 135.488);


			//解 Y Y U V
			//YY = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] + 222.912 - 1.164*new_Y_plane.at<double>(y, x + 1),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] + 276.928,
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] - 135.488  );

			//解Us Vs
			//YY = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
			//	(double)imgrgbcp.at<Vec3b>(y, x)[1] - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
			//	(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] + 222.912 - 1.164*new_Y_plane.at<double>(y, x + 1),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
			//	(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] - 135.488 - 1.164*new_Y_plane.at<double>(y + 1, x + 1));

			//解Y Us Vs
			YY = (Mat_<double>(4, 1) <<       //RaGaBa RbGbBb RcGcBc RdGdBd
				(double)imgrgbcp.at<Vec3b>(y, x)[1] - 135.488 - 1.164*new_Y_plane.at<double>(y, x),
				(double)imgrgbcp.at<Vec3b>(y, x + 1)[2] + 222.912 - 1.164*new_Y_plane.at<double>(y, x + 1),
				(double)imgrgbcp.at<Vec3b>(y + 1, x)[0] + 276.928 - 1.164*new_Y_plane.at<double>(y + 1, x),
				(double)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1] - 135.488);



			X = A_inv * YY;
			//X = A4.inv(DECOMP_SVD)*YY;
			//X = X + E;

			//new_Y_plane.at<double>(y, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y, x + 1) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			//new_Y_plane.at<double>(y + 1, x) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			new_Y_plane.at<double>(y + 1, x + 1) = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			U.at<double>(yy, xx) = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			V.at<double>(yy, xx) = Clip((int)(X.at<double>(2, 0) + 0.5), 0, 255);

			


			//Iteration

			//des_U = Clip((int)(X.at<double>(0, 0) + 0.5), 0, 255);
			//des_V = Clip((int)(X.at<double>(1, 0) + 0.5), 0, 255);
			//M0 = (int)imgrgbcp.at<Vec3b>(y, x)[1];
			//M1 = (int)imgrgbcp.at<Vec3b>(y, x + 1)[2];
			//M2 = (int)imgrgbcp.at<Vec3b>(y + 1, x)[0];
			//M3 = (int)imgrgbcp.at<Vec3b>(y + 1, x + 1)[1];


			//YleftTop = Y0 = (int)(new_Y_plane.at<double>(y, x) + 0.5);
			//Yr = Y1 = (int)(new_Y_plane.at<double>(y, x + 1) + 0.5);
			//Yb = Y2 = (int)(new_Y_plane.at<double>(y + 1, x) + 0.5);
			//YrightDown = Y3 = (int)(new_Y_plane.at<double>(y + 1, x + 1) + 0.5);
			//simpleYb = simpleArrayY[0][Yb];
			//simpleYr = simpleArrayY[0][Yr];
			//simpleYleftTop = simpleArrayY[0][YleftTop];
			//simpleYrightDown = simpleArrayY[0][YrightDown];



			//tmp_U = des_U;
			//tmp_V = des_V;
			//rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			//rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
			//rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
			//rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			//error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);


			//if (error == 0) {

			//	U.at<double>(yy, xx) = des_U;
			//	V.at<double>(yy, xx) = des_V;

			//	xx++;
			//	continue;

			//}



			//error_TH = INT_MAX;
			//for (int i = -1; i <= 1; i++) {
			//	tmp_V = des_V - i;
			//	if (0 <= tmp_V && tmp_V <= 255) {
			//		rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
			//		for (int j = -1; j <= 1; j++) {

			//			tmp_U = des_U + j;

			//			if (0 <= tmp_U && tmp_U <= 255) {
			//				rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			//				rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
			//				rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			//				error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);



			//				if (error < error_TH)
			//				{
			//					error_TH = error;
			//					n_i = i;
			//					n_j = j;
			//					can_U = tmp_U;
			//					can_V = tmp_V;
			//				}

			//			}

			//		}
			//	}

			//}

			//des_U = can_U;
			//des_V = can_V;



			///*last_error = error_TH;
			//same_num = 0;*/

			//while ((n_i != 0 || n_j != 0) && error_TH != 0) {


			//	tmp_i = 0;
			//	tmp_j = 0;
			//	for (int i = -1; i <= 1; i++) {
			//		tmp_V = des_V - i;
			//		if (0 <= tmp_V && tmp_V <= 255) {
			//			rec_M1 = Clip((int)(simpleYr + simpleArrayV[1][tmp_V] + 0.5), 0, 255);//R
			//			for (int j = -1; j <= 1; j++) {
			//				old_i = i + n_i;
			//				old_j = j + n_j;

			//				if (old_i < -1 || 1 < old_i || old_j < -1 || 1 < old_j) {

			//					tmp_U = des_U + j;

			//					if (0 <= tmp_U && tmp_U <= 255) {

			//						rec_M0 = Clip((int)(simpleYleftTop + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			//						rec_M2 = Clip((int)(simpleYb + simpleArrayU[1][tmp_U] + 0.5), 0, 255);//B
			//						rec_M3 = Clip((int)(simpleYrightDown + simpleArrayU[0][tmp_U] + simpleArrayV[0][tmp_V] + 0.5), 0, 255);//G
			//						error = (M0 - rec_M0)*(M0 - rec_M0) + (M1 - rec_M1) * (M1 - rec_M1) + (M2 - rec_M2) * (M2 - rec_M2) + (M3 - rec_M3) * (M3 - rec_M3);


			//						if (error < error_TH)
			//						{
			//							error_TH = error;
			//							tmp_i = i;
			//							tmp_j = j;
			//							can_U = tmp_U;
			//							can_V = tmp_V;
			//						}
			//					}


			//				}
			//			}
			//		}

			//	}
			//	n_i = tmp_i;
			//	n_j = tmp_j;
			//	des_U = can_U;
			//	des_V = can_V;

			//	/*if (last_error == error_TH)
			//		same_num++;
			//	else
			//		same_num = 0;
			//	last_error = error_TH;*/

			//}


			//U.at<double>(yy, xx) = des_U;
			//V.at<double>(yy, xx) = des_V;

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

	double YM = 0;
	for (int i = 0; i < YUV.rows; i++)        //算有動的Y的平均
		for (int j = 0; j < YUV.cols; j++) {
			YM += new_Y_plane.at<double>(i, j);
		}


	
}
