#include "Subsample.h"
#include<math.h>
#include <filesystem>
using namespace std;


void BGR_to_YUV(int height, int width, Mat& YUV, const Mat& BGR)
{
	YUV = Mat::zeros(height, width, CV_64FC3);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			YUV.at<Vec3d>(y, x)[0] = 0.257 * BGR.at<Vec3b>(y, x)[2] + 0.504 * BGR.at<Vec3b>(y, x)[1] + 0.098 * BGR.at<Vec3b>(y, x)[0] + 16;
			YUV.at<Vec3d>(y, x)[1] = -0.148 * BGR.at<Vec3b>(y, x)[2] - 0.291 * BGR.at<Vec3b>(y, x)[1] + 0.439 * BGR.at<Vec3b>(y, x)[0] + 128;
			YUV.at<Vec3d>(y, x)[2] = 0.439 * BGR.at<Vec3b>(y, x)[2] - 0.368 * BGR.at<Vec3b>(y, x)[1] - 0.071 * BGR.at<Vec3b>(y, x)[0] + 128;

			/*YUV.at<Vec3d>(y, x)[0] = Y;
			YUV.at<Vec3d>(y, x)[1] = U;
			YUV.at<Vec3d>(y, x)[2] = V;*/
		}
	}
}

void YUV_to_BGR(int height, int width, Mat& BGR, const Mat& YUV)
{
	BGR = Mat::zeros(height, width, CV_64FC3);
	for (int i = 0; i < YUV.rows; i++)
		for (int j = 0; j < YUV.cols; j++) {
			BGR.at<Vec3d>(i, j)[0] = 1.164 * (YUV.at<Vec3d>(i, j)[0] - 16) + 2.018 * (YUV.at<Vec3d>(i, j)[1] - 128) + 0 * (YUV.at<Vec3d>(i, j)[2] - 128);
			BGR.at<Vec3d>(i, j)[1] = 1.164 * (YUV.at<Vec3d>(i, j)[0] - 16) - 0.391 * (YUV.at<Vec3d>(i, j)[1] - 128) - 0.813 * (YUV.at<Vec3d>(i, j)[2] - 128);
			BGR.at<Vec3d>(i, j)[2] = 1.164 * (YUV.at<Vec3d>(i, j)[0] - 16) + 0 * (YUV.at<Vec3d>(i, j)[1] - 128) + 1.596 * (YUV.at<Vec3d>(i, j)[2] - 128);
		}

	BGR.convertTo(BGR, CV_8UC3);

}

void copyRound(Mat& dst, const Mat src) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<double>(i, j) = (int)(src.at<double>(i, j) + 0.5);
		}
	}
}
void ReadCFA(istream& in, Mat& CFA)
{

	int cfa_h, cfa_w, color;

	in >> cfa_h >> cfa_w;

	if (!in || cfa_h <= 0 || cfa_w <= 0)
	{
		cerr << "Cannot process the cfa file!" << endl;
		exit(-1);
	}

	CFA.create(cfa_h, cfa_w, CV_8U);

	for (int h = 0; h < cfa_h; ++h)
		for (int w = 0; w < cfa_w; ++w)
		{
			if (in >> color)
			{
				uchar& val = CFA.at<uchar>(h, w);
				val = (color % 10) ? CFA_R : CFA_None;
				val |= ((color / 10) % 10) ? CFA_G : CFA_None;
				val |= ((color / 100) % 10) ? CFA_B : CFA_None;
				val |= (color / 1000) ? CFA_W : CFA_None;
			}
			else
			{
				cerr << "Cannot read the cfa data!" << endl;
				cerr << "h,w=" << h << "," << w << endl;
				exit(-1);
			}
		}

}

void Convert_to_Bayer(int height, int width, Mat& bayerImg, const Mat& bgrImg, const Mat& CFA)
{
	bayerImg = Mat::zeros(height, width, CV_8UC1);
	int channel;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			channel = GetColor(CFA, y, x);

			if (channel == CFA_B)
				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[0];
			else if (channel == CFA_G)
				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[1];
			else if (channel == CFA_R)
				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[2];
		}
}



/*Bayer*/
void BayerProcess()
{
	double timeStart = 0, timeEnd = 0;
	string img_set[] = {

		//"IMAX",
		//"Kodak_big",
		//"Video",
		//"SCI",
		"classical"
	};//dataset

	int img_num[] = {

		//18,
		//24,
		//200,
		//20,
		8
	};//picture numbers

	string DS[] =
	{
		//"Lin",	  // CDM method
		"IJCSU",  // BIDM method
		//"JDU",	  // modified 420A
		//"Bayer" // Baseline
	};//down sample methods

	string US = "Resize_BIL";
	//upsample methods: Resize_COPY, Resize_BIL

	Mat CFA;
	fstream cfa;
	cfa.open("GRBG.cfa", ios::in);
	ReadCFA(cfa, CFA);
	cfa.close();

	//string qparray[] = { "1" , "7.5", "15", "22.5", "30", "37.5", "45", "52.5", "60", "67.5" };//CR
	string qparray[] = { "0", "4", "8", "12", "16", "20", "24", "28", "32", "36", "40", "44", "48", "51" };//QP
	int yuv_write = false;
	int luma_modification = true;
	int skip = false;//skip server side
	int J2K = false;//true:	J2K;	false:	VVC
	double downsample_time = 0;
	double luma_modification_time = 0;
	double upsample_time = 0;
	double shift_time = 0;
	double psnr_sum = 0;
	double Bayer_ssim_sum = 0;
	double cpsnr_sum = 0;
	double cssim_sum = 0;
	double time1_sum = 0, time2_sum = 0, time3_sum = 0;
	double luma_mean_sum = 0;
	double demosaick_time = 0;
	double bitrate_sum = 0;






	for (int d = 0; d < sizeof(DS) / sizeof(*DS); d++) {

		cout << "<" << DS[d] << ">" << endl;


		cout << "<" << US << ">" << endl;


		for (int s = 0; s < sizeof(img_set) / sizeof(*img_set); s++) {
			int start = 0;
			int end = img_num[s];

			cout << "------------" << img_set[s] << "------------" << endl;




			for (int qp = 0; qp < sizeof(qparray) / sizeof(*qparray); qp++) {
				cout << "------------QP " << qparray[qp] << "------------" << endl;

				for (int i = start; i < end; i++) {
					
					Mat img;//原始圖檔
					Mat bayerImg;//原始Bayer
					Mat bgrImg;//原始RBG
					Mat yuvImg;//原始YUV
					Mat DS_U_plane, DS_V_plane;//抽樣UV
					Mat US_U_plane, US_V_plane;//重建UV
					Mat new_Y_plane;//新Y
					int nh, nw;
					Mat new_yuvImg;
					Mat tmp_bgrImg;
					Mat new_bayerImg;
					Mat new_bgrImg;

					if (img_set[s] == "SCI" && (i +1 == 7 || i + 1 == 9)) 
						continue;

					/*---------------------------------------讀檔--------------------------------------*/

					stringstream imgpath;
					imgpath.str("");
					imgpath.clear();
					imgpath << img_set[s] << "/" << i + 1;

					if (img_set[s].compare("Kodak") == 0)
						imgpath << ".png";//Kodak 
					else
						imgpath << ".bmp";
					img = imread(imgpath.str());
					int height = img.rows;
					int width = img.cols;

					/*--------------------------------Original Bayer---------------------------------*/
					Convert_to_Bayer(height, width, bayerImg, img, CFA);
					timeStart = clock();
					Kiku(height, width, bgrImg, bayerImg, CFA);
					timeEnd = clock();
					demosaick_time += ((timeEnd - timeStart) / CLOCKS_PER_SEC);


					/*--------------------------------Write Bayer Ground Truth---------------------------------*/
					//stringstream file_path;
					//file_path << "Bayer_original/" << img_set[s] << "/"  << i + 1 << ".bmp"; //Bayer Ground Truth Path
					//imwrite(file_path.str(), bayerImg);
					/*--------------------------------Write RGB Ground truth---------------------------------*/
					//stringstream file_path;
					/*file_path.str("");
					file_path.clear();
					file_path << "Bayer_Color/" << img_set[s] << "/"  << i + 1 << ".tif";//RGB Ground Truth Path
					imwrite(file_path.str(), bgrImg);*/


					/*-------------------------------------BGR2YUV-------------------------------------*/
					BGR_to_YUV(height, width, yuvImg, bgrImg);
					vector<Mat> yuvPlane;
					split(yuvImg, yuvPlane);//YUV->Y,U,V
					new_Y_plane = Mat::zeros(height, width, CV_64FC1);
					DS_U_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);//420
					DS_V_plane = Mat::zeros(height / 2, width / 2, CV_64FC1);//420
					nh = height + (8 - height % 8) % 8;
					nw = width + (8 - width % 8) % 8;
					/*-----------------------------------Down Sample--------------------------------*/
					if (!skip) {
						if (qparray[qp] == "0") {
							if (DS[d].compare("Lin") == 0) {
								timeStart = clock();
								DownSample_420Lin_Copy(height, width, DS_U_plane, DS_V_plane, bgrImg, yuvPlane[0], yuvPlane[1], yuvPlane[2]);
								copyRound(new_Y_plane, yuvPlane[0]);
								timeEnd = clock();
							}
							else if (DS[d].compare("IJCSU") == 0) {
								timeStart = clock();
								DownSample_Bayer_420IJCSU_Bilinear(height, width, DS_U_plane, DS_V_plane, bgrImg, yuvPlane[0], yuvPlane[1], yuvPlane[2]);
								copyRound(new_Y_plane, yuvPlane[0]);
								timeEnd = clock();
							}
							else if (DS[d].compare("JDU") == 0) {
								timeStart = clock();
								DownSample_420JDU(height, width, DS_U_plane, DS_V_plane, bgrImg, yuvPlane[0], yuvPlane[1], yuvPlane[2]);
								copyRound(new_Y_plane, yuvPlane[0]);
								timeEnd = clock();
							}
							else {
								cout << "Error: " << DS[d] << endl;
							}
							downsample_time = ((timeEnd - timeStart) / CLOCKS_PER_SEC);
						}

						//cout << downsample_time << endl;

						/*-----------------------------------Luma Modifiaction--------------------------------*/
						Mat imgrgbcp = Mat::zeros(height, width, CV_64FC1);
						bgrImg.copyTo(imgrgbcp);
						double mystart = 0, myend = 0;
						if (luma_modification) {
							Mat ori420;
							combine420_64F(new_Y_plane, DS_U_plane, DS_V_plane, ori420);

							Mat exp444;
							expand_64F(ori420, exp444);

							Mat dstU;
							Mat dstV;

							if (US.compare("Resize_BIL") == 0) {
								resize(DS_U_plane, dstU, Size(), 2, 2, INTER_LINEAR);
								resize(DS_V_plane, dstV, Size(), 2, 2, INTER_LINEAR);
							}
							else if (US.compare("Resize_BIC") == 0) {
								resize(DS_U_plane, dstU, Size(), 2, 2, INTER_CUBIC);
								resize(DS_V_plane, dstV, Size(), 2, 2, INTER_CUBIC);
							}
							else if (US.compare("Resize_COPY") == 0) {
								resize(DS_U_plane, dstU, Size(0, 0), 2, 2, INTER_NEAREST);
								resize(DS_V_plane, dstV, Size(0, 0), 2, 2, INTER_NEAREST);
							}



							Convert_Y(5, imgrgbcp.rows, imgrgbcp.cols, new_Y_plane, imgrgbcp, dstU, dstV, mystart, myend);

							luma_modification_time = (myend - mystart) / CLOCKS_PER_SEC;
						}
						 


						//延伸為8的倍數

						nh = height + (8 - height % 8) % 8;
						nw = width + (8 - width % 8) % 8;
						copyMakeBorder(new_Y_plane, new_Y_plane, 0, nh - height, 0, nw - width, BORDER_REPLICATE);
						copyMakeBorder(DS_U_plane, DS_U_plane, 0, (nh - height) / 2, 0, (nw - width) / 2, BORDER_REPLICATE);
						copyMakeBorder(DS_V_plane, DS_V_plane, 0, (nh - height) / 2, 0, (nw - width) / 2, BORDER_REPLICATE);
					}



					//else if (DS[d].compare("Bayer") == 0) {
					//	new_Y_plane = bayerImg.clone();
					//	DS_U_plane = Mat::zeros(height, width, CV_64FC1);//444
					//	DS_V_plane = Mat::zeros(height, width, CV_64FC1);//444
					//	copyMakeBorder(new_Y_plane, new_Y_plane, 0, nh - height, 0, nw - width, BORDER_REPLICATE);
					//	copyMakeBorder(DS_U_plane, DS_U_plane, 0, nh - height, 0, nw - width, BORDER_REPLICATE);
					//	copyMakeBorder(DS_V_plane, DS_V_plane, 0, nh - height, 0, nw - width, BORDER_REPLICATE);
					//}
					// 
					//-----------------------------------Write Encoded File-----------------------------------
					fstream yuvBegin;
					stringstream ssim_yuvImgNameOctober;
					if (yuv_write == true) {
						ssim_yuvImgNameOctober.str("");
						ssim_yuvImgNameOctober.clear();
						ssim_yuvImgNameOctober << "Bayer YUV/" << img_set[s] << "/source/" << DS[d] << "_" << i + 1 << "L.yuv";

						yuvBegin.open(ssim_yuvImgNameOctober.str(), ios::out | ios::binary);//in or out
						saveYUV(new_Y_plane, DS_U_plane, DS_V_plane, yuvBegin);
						yuvBegin.close();
					}
					//cout << "stop" << endl;

					//-----------------------------------VVC (J2K) Proccess------------------------------------
					//-----------------------------------Read Decoded File-----------------------------------
					//if (qparray[qp] != "0") {
					if (1) {
						new_Y_plane = Mat::zeros(nh, nw, CV_8UC1);
						DS_U_plane = Mat::zeros(nh / 2, nw / 2, CV_8UC1);//420
						DS_V_plane = Mat::zeros(nh / 2, nw / 2, CV_8UC1);//420
						ssim_yuvImgNameOctober.str("");
						ssim_yuvImgNameOctober.clear();

						//string J2K_file = "D:\\SY_JP2000\\Bayer_" + img_set[s] + "\\result\\" + DS[d] + "\\" + qparray[qp] + "\\" + DS[d] + "_" + to_string(i + 1) + ".jp2";
						////cout << "File size: " << filesystem::file_size(J2K_file) << " bytes" << endl;//C++ 17以上才有
						//float f_size_b = 8 * filesystem::file_size(J2K_file);
						////cout << "File size: " << f_size_b << " bits" << endl;
						//bitrate_sum += f_size_b / (height * width);
						////cout << f_size_b / (height * width) << endl;
						//continue;


						if (J2K) {
							string yuvpath = "D:\\SY_JP2000\\Bayer_"; //J2K Decoded File Path
							yuvpath += img_set[s] + "\\result\\" + DS[d] + "\\" + qparray[qp] + "\\" + DS[d] + "_" + to_string(i + 1) + ".bmp";
							Mat J2Kyuv = imread(yuvpath, CV_LOAD_IMAGE_GRAYSCALE);
							new_Y_plane = J2Kyuv(Rect(0, 0, width, height));
							DS_U_plane = J2Kyuv(Rect(0, height, width / 2, height / 2));
							DS_V_plane = J2Kyuv(Rect(width / 2, height, width / 2, height / 2));
						}
						else {
							ssim_yuvImgNameOctober << "Bayer YUV/" << img_set[s] << "/result/" << qparray[qp] << "/" << DS[d] << "_" << i + 1 << "_L_QP" << qparray[qp] << ".yuv";//VVC Decoded File Path
							fstream yuvEnd;
							yuvEnd.open(ssim_yuvImgNameOctober.str(), ios::in | ios::binary);
							if (DS[d].compare("Bayer") == 0) {
								DS_U_plane = Mat::zeros(nh, nw, CV_8UC1);//444
								DS_V_plane = Mat::zeros(nh, nw, CV_8UC1);//444

								Read_YUV444(nh, nw, yuvEnd, new_Y_plane, DS_U_plane, DS_V_plane);
								new_bayerImg = new_Y_plane.clone();
								new_bayerImg = new_bayerImg(Rect(0, 0, width, height));
								new_bayerImg.convertTo(new_bayerImg, CV_8UC1);
							}
							else {
								Read_YUV420(nh, nw, yuvEnd, new_Y_plane, DS_U_plane, DS_V_plane);
							}
							yuvEnd.close();
						}


					}
					if (DS[d].compare("Bayer") != 0) {

						new_Y_plane = new_Y_plane(Rect(0, 0, width, height));
						DS_U_plane = DS_U_plane(Rect(0, 0, width / 2, height / 2));
						DS_V_plane = DS_V_plane(Rect(0, 0, width / 2, height / 2));

						new_Y_plane.convertTo(new_Y_plane, CV_64FC1);
						DS_U_plane.convertTo(DS_U_plane, CV_64FC1);
						DS_V_plane.convertTo(DS_V_plane, CV_64FC1);


						/*----------------------------------Upsample----------------------------------*/


						if (US.compare("Resize_BIL") == 0) {

							timeStart = clock();
							resize(DS_U_plane, US_U_plane, Size(0, 0), 2, 2, INTER_LINEAR);
							resize(DS_V_plane, US_V_plane, Size(0, 0), 2, 2, INTER_LINEAR);
							timeEnd = clock();
						}
						else if (US.compare("Resize_COPY") == 0) {
							timeStart = clock();
							resize(DS_U_plane, US_U_plane, Size(0, 0), 2, 2, INTER_NEAREST);
							resize(DS_V_plane, US_V_plane, Size(0, 0), 2, 2, INTER_NEAREST);
							timeEnd = clock();
						}


						upsample_time = ((timeEnd - timeStart) / CLOCKS_PER_SEC);

						/*---------------------------------合併YUV-----------------------------------*/
						new_yuvImg = Mat::zeros(height, width, CV_64FC3);
						vector<Mat> new_yuvPlane = { new_Y_plane, US_U_plane, US_V_plane };
						merge(new_yuvPlane, new_yuvImg);
						/*---------------------------------Reconstruct BGR-----------------------------------*/
						YUV_to_BGR(height, width, tmp_bgrImg, new_yuvImg);
						/*---------------------------------New Bayer---------------------------------*/
						Convert_to_Bayer(height, width, new_bayerImg, tmp_bgrImg, CFA);
					}
					Kiku(height, width, new_bgrImg, new_bayerImg, CFA);

					///*---------------------------------Write Bayer Result---------------------------------*/
					//stringstream file_path2;
					//file_path2.str("");
					//file_path2.clear();
					//if (J2K)
					//	file_path2 << "J2K_result/Bayer/" << img_set[s] << "/" << DS[d] << "/" << qparray[qp] << "/" << i + 1 << ".bmp";
					//else
					//	file_path2 << "Bayer_image(new)/" << img_set[s] << "/" << DS[d] << "/" << qparray[qp] << "/" << i + 1 << ".bmp";
					//imwrite(file_path2.str(), new_bayerImg);
					///*---------------------------------Write RGB Result---------------------------------*/
					//file_path2.str("");
					//file_path2.clear();
					//if (J2K)
					//	file_path2 << "J2K_result/Color/" << img_set[s] << "/" << DS[d] << "/" << qparray[qp] << "/" << i + 1 << ".bmp";
					//else
					//	file_path2 << "Color_image(new)/" << img_set[s] << "/" << DS[d] << "/" << qparray[qp] << "/" << i + 1 << ".bmp";
					//imwrite(file_path2.str(), new_bgrImg);
					
					/*----------------------------------Split----------------------------------*/
					//vector<Mat> bgrNewPlane = { Mat::zeros(height, width, CV_8UC1), Mat::zeros(height, width, CV_8UC1), Mat::zeros(height, width, CV_8UC1) };
					//split(new_bgrImg, bgrNewPlane);
					/*---------------------------------PSNR----------------------------------*/

					double psnr;
					double cssim;

					double cpsnr;
					double tmp_cpsnr;
					double Bayer_ssim;

					psnr = PSNR(bayerImg, new_bayerImg);
					Bayer_ssim = Bayer_SSIM(bayerImg, new_bayerImg);

					cpsnr = PSNR(bgrImg, new_bgrImg);//RGB
					cssim = SSIM(bgrImg, new_bgrImg);//RGB




					cout << "PSNR: " << psnr << " Bayer_SSIM: " << Bayer_ssim << " CPSNR: " << cpsnr << " CSSIM: " << cssim << endl;



					psnr_sum += psnr;
					Bayer_ssim_sum += Bayer_ssim;
					cpsnr_sum += cpsnr;
					cssim_sum += cssim;

					if (!luma_modification)
						luma_modification_time = 0;

					time1_sum += downsample_time;
					time2_sum += luma_modification_time;
					time3_sum += upsample_time;

					downsample_time = 0;
					luma_modification_time = 0;
					upsample_time = 0;
				}
				demosaick_time = 0;


				if (img_set[s] == "SCI") {
					cout << psnr_sum / (img_num[s]-2) << endl << Bayer_ssim_sum / (img_num[s] - 2) << endl << endl << cpsnr_sum / (img_num[s] - 2) << endl << cssim_sum / (img_num[s] - 2) << endl;
				}
				else {
					cout << psnr_sum / img_num[s] << endl << Bayer_ssim_sum / img_num[s] << endl << endl << cpsnr_sum / img_num[s] << endl << cssim_sum / img_num[s] << endl;
				}
				
				cout << "Avg.PSNR: " << psnr_sum / img_num[s] << " Avg.SSIM: " << Bayer_ssim_sum / img_num[s] << endl;
				cout << "Avg.CPSNR: " << cpsnr_sum / img_num[s] << " Avg.CSSIM: " << cssim_sum / img_num[s] << endl;//RGB
				//cout << "Avg.Bitrate: " << bitrate_sum / img_num[s] << endl;
				//cout << "Avg.DStime: " << time1_sum / img_num[s] << " Avg.Ltime: " << time2_sum / img_num[s] << " Avg.UPtime: " << time3_sum / img_num[s] << endl;

				psnr_sum = 0;
				Bayer_ssim_sum = 0;
				cpsnr_sum = 0;
				cssim_sum = 0;
				bitrate_sum = 0;
				time1_sum = 0;
				time2_sum = 0;
				time3_sum = 0;



			}


		}


	}

}



int main() {

	BayerProcess();

	system("PAUSE");
	return 0;
}



