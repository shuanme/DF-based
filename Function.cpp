#include "Subsample.h"

using namespace std;
using namespace cv;
#define B_sideLen 16
#define B_EleNum (B_sideLen * B_sideLen)
#define DS_B_sideLen (B_sideLen / 2)
#define DS_B_EleNum (DS_B_sideLen * DS_B_sideLen)

int wsize = 7;

//int mir(int x, int edge) {			// 101
//    if (x < 0)
//        x = -x;
//    if (x >= edge)
//        x = edge - (x - edge + 2);
//    return x;
//}
//int mir1001(int x, int edge) {		// 1001
//    if (x < 0)
//        x = -x - 1;
//    if (x >= edge)
//        x = edge - (x - edge + 1);
//    return x;
//}
//CFA BYE
//void ReadCFA(istream& in, Mat& CFA)
//{
//	int cfa_h, cfa_w, color;
//
//	in >> cfa_h >> cfa_w;
//
//	if (!in || cfa_h <= 0 || cfa_w <= 0)
//	{
//		cerr << "Cannot process the cfa file!" << endl;
//		exit(-1);
//	}
//
//	CFA.create(cfa_h, cfa_w, CV_8U);
//
//	for (int h = 0; h < cfa_h; ++h)
//		for (int w = 0; w < cfa_w; ++w)
//		{
//			if (in >> color)
//			{
//				uchar& val = CFA.at<uchar>(h, w);
//				val = (color % 10) ? CFA_R : CFA_None;
//				val |= ((color / 10) % 10) ? CFA_G : CFA_None;
//				val |= ((color / 100) % 10) ? CFA_B : CFA_None;
//				val |= (color / 1000) ? CFA_W : CFA_None;
//			}
//			else
//			{
//				cerr << "Cannot read the cfa data!" << endl;
//				cerr << "h,w=" << h << "," << w << endl;
//				exit(-1);
//			}
//		}
//}

//int mirror1(int edge, int index)
//{
//    if (index < 0)
//        index = -index;
//    else if (index >= edge)
//        index = edge - (index - edge + 2); // (edge - 1) - (index - (edge - 1))
//    return index;
//}
/*Mat boxfilter(Mat& imSrc, int h, int v)
{
    Mat imDst(imSrc.rows, imSrc.cols, CV_64FC1, Scalar::all(0));
    Mat imCum(imSrc.rows, imSrc.cols, CV_64FC1, Scalar::all(0));

    if (v != 0) {
        /// cumulative sum over Y axis
        imCum = Scalar::all(0);
        for (int i = 0; i < imSrc.rows; i++)
            for (int j = 0; j < imSrc.cols; j++) {
                if (i == 0)
                    imCum.at<double>(i, j) = imSrc.at<double>(i, j);
                else
                    imCum.at<double>(i, j) = imCum.at<double>(i - 1, j) + imSrc.at<double>(i, j);
            }

        /// difference over Y axis
        for (int i = 0; i < v + 1; i++)
            for (int j = 0; j < imDst.cols; j++) {
                int k = i + v;
                imDst.at<double>(i, j) = imCum.at<double>(k, j);
            }
        for (int i = v + 1; i < imDst.rows - v; i++)
            for (int j = 0; j < imDst.cols; j++) {
                int k = i + v;
                int l = i - v - 1;
                imDst.at<double>(i, j) = imCum.at<double>(k, j) - imCum.at<double>(l, j);
            }
        for (int i = imDst.rows - v; i < imDst.rows; i++)
            for (int j = 0; j < imDst.cols; j++) {
                int k = i - v - 1;
                imDst.at<double>(i, j) = imCum.at<double>(imCum.rows - 1, j) - imCum.at<double>(k, j);
            }
    }

    if (h != 0) {
        if (v != 0) {
            /// cumulative sum over X axis
            imCum = Scalar::all(0);
            for (int i = 0; i < imDst.rows; i++)
                for (int j = 0; j < imDst.cols; j++) {
                    if (j == 0)
                        imCum.at<double>(i, j) = imDst.at<double>(i, j);
                    else
                        imCum.at<double>(i, j) = imCum.at<double>(i, j - 1) + imDst.at<double>(i, j);
                }
        }
        else {
            /// cumulative sum over X axis
            imCum = Scalar::all(0);
            for (int i = 0; i < imSrc.rows; i++)
                for (int j = 0; j < imSrc.cols; j++) {
                    if (j == 0)
                        imCum.at<double>(i, j) = imSrc.at<double>(i, j);
                    else
                        imCum.at<double>(i, j) = imCum.at<double>(i, j - 1) + imSrc.at<double>(i, j);
                }
        }

        /// difference over X axis
        for (int i = 0; i < imDst.rows; i++)
            for (int j = 0; j < h + 1; j++) {
                int k = j + h;
                imDst.at<double>(i, j) = imCum.at<double>(i, k);
            }
        for (int i = 0; i < imDst.rows; i++)
            for (int j = h + 1; j < imDst.cols - h; j++) {
                int k = j + h;
                int l = j - h - 1;
                imDst.at<double>(i, j) = imCum.at<double>(i, k) - imCum.at<double>(i, l);
            }
        for (int i = 0; i < imDst.rows; i++)
            for (int j = imDst.cols - h; j < imDst.cols; j++) {
                int k = j - h - 1;
                imDst.at<double>(i, j) = imCum.at<double>(i, imCum.cols - 1) - imCum.at<double>(i, k);
            }
    }

    /// output result
    return imDst;
}*/
void ThreeWaySplit(int color, Mat&imgrgb, Mat&CFA, Mat &dst) {
    dst = Mat::zeros(imgrgb.rows, imgrgb.cols, CV_64FC1);
    if (color == 1)
        for (int i = 0; i < imgrgb.rows; i++)
            for (int j = 0; j < imgrgb.cols; j++)
                dst.at<double>(i, j) = imgrgb.at<Vec3b>(i, j)[2];

    if (color == 2)
        for (int i = 0; i < imgrgb.rows; i++)
            for (int j = 0; j < imgrgb.cols; j++)
                dst.at<double>(i, j) = imgrgb.at<Vec3b>(i, j)[1];

    if (color == 4)
        for (int i = 0; i < imgrgb.rows; i++)
            for (int j = 0; j < imgrgb.cols; j++)
                dst.at<double>(i, j) = imgrgb.at<Vec3b>(i, j)[0];

}
void Show(Mat src) {
    src.convertTo(src, CV_8UC3);
    imshow("src", src);

    waitKey(0);
}
void array(Mat& src) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++)
            cout << src.at<double>(i, j) << ", ";
        cout << endl;
    }
    system("Pause");
}
//double MSD(Mat& expand444, int i, int j, int k) {
//    double MSD;
//
//    if (k == 1) {
//        if (i % 2 == 0 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 0 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 1 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 1, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 1 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 2, expand444.cols))[0], 2)) / 4;
//        }
//    }
//    //-----------------------------------------------------------------------------------------------------------------------------
//    else if (k == 2) {
//        if (i % 2 == 0 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j + 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 0 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 1 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j + 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j + 1, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 1 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 3, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 2, expand444.rows), mir(j, expand444.cols))[0], 2)) / 4;
//        }
//    }
//    //-----------------------------------------------------------------------------------------------------------------------------
//    else if (k == 3) {
//        if (i % 2 == 0 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 0 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 1 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[0], 2)) / 4;
//        }
//        else if (i % 2 == 1 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[0], 2)) / 4;
//        }
//    }
//    //-----------------------------------------------------------------------------------------------------------------------------
//    else if (k == 4) {
//        if (i % 2 == 0 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0], 2)) / 4 / 255;
//        }
//        else if (i % 2 == 0 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0], 2)) / 4 / 255;
//        }
//        else if (i % 2 == 1 && j % 2 == 0) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0], 2)) / 4 / 255;
//        }
//        else if (i % 2 == 1 && j % 2 == 1) {
//            MSD = (pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 2, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 3, expand444.cols))[0], 2)
//                + pow(expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] - expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 2, expand444.cols))[0], 2)) / 4 / 255;
//        }
//    }
//
//    return MSD;
//}
//double BC_funtion(double fx_value)
//{
//    double fCof = -0.5; //可以代-0.5 or -0.75
//
//    if (fx_value <= 1 && fx_value >= -1)
//    {
//        return 1 - (3 + fCof)*fabs(fx_value)*fabs(fx_value) + (2 + fCof)*fabs(fx_value)*fabs(fx_value)*fabs(fx_value);
//    }
//    else if ((fx_value < 2 && fx_value >1) || (fx_value < -1 && fx_value >-2))
//    {
//        return (-4)*fCof + 8 * fCof*fabs(fx_value) - 5 * fCof*fabs(fx_value)*fabs(fx_value) + fCof * fabs(fx_value)*fabs(fx_value)*fabs(fx_value);
//    }
//    else
//        return 0.0;
//}

//double calculate_UV(double u, double v, int yy, int xx, Mat& DS_U_plane, Mat& DS_V_plane, int flag)
//{
//	double calculate = 0;
//	double x_array[] = { -4+u,-3+u,-2 + u, -1 + u, u, 1 + u, 2 + u ,3+u,4+u};
//	double y_array[] = { -4 + u,-3 + u,-2 + u, -1 + u, u, 1 + u, 2 + u ,3 + u,4 + u };
//	double w = 0;
//
//	switch (flag) // 1 for U, 2 for V
//	{
//	case 1:
//		/*calculate = BC_funtion(1 + u)*BC_funtion(1 + v)*DS_U_plane.at<double>(yy - 1, xx - 1) + BC_funtion(1 + u)*BC_funtion(v)*DS_U_plane.at<double>(yy - 1, xx) + BC_funtion(1 + u)*BC_funtion(1 - v)*DS_U_plane.at<double>(yy - 1, xx + 1) + BC_funtion(1 + u)*BC_funtion(2 - v)*DS_U_plane.at<double>(yy - 1, xx + 2) + BC_funtion(u)*BC_funtion(1 + v)*DS_U_plane.at<double>(yy, xx - 1) + BC_funtion(u)*BC_funtion(1 - v)*DS_U_plane.at<double>(yy, xx + 1) + BC_funtion(u)*BC_funtion(2 - v)*DS_U_plane.at<double>(yy, xx + 2) + BC_funtion(1 - u)*BC_funtion(1 + v)*DS_U_plane.at<double>(yy + 1, xx - 1) + BC_funtion(1 - u)*BC_funtion(v)*DS_U_plane.at<double>(yy + 1, xx) + BC_funtion(1 - u)*BC_funtion(1 - v)*DS_U_plane.at<double>(yy + 1, xx + 1) + BC_funtion(1 - u)*BC_funtion(2 - v)*DS_U_plane.at<double>(yy + 1, xx + 2) + BC_funtion(2 - u)*BC_funtion(1 + v)*DS_U_plane.at<double>(yy + 2, xx - 1) + BC_funtion(2 - u)*BC_funtion(v)*DS_U_plane.at<double>(yy + 2, xx) + BC_funtion(2 - u)*BC_funtion(1 - v)*DS_U_plane.at<double>(yy + 2, xx + 1) + BC_funtion(2 - u)*BC_funtion(2 - v)*DS_U_plane.at<double>(yy + 2, xx + 2);*/
//		for (int i = 0; i < sizeof(x_array) / sizeof(*x_array); i++)
//		{
//			for (int j = 0; j < sizeof(y_array) / sizeof(*y_array); j++)
//			{
//				//if(xx+j-2 <= sizeof(x_array) / sizeof(*x_array) && yy + i -2 <= sizeof(y_array) / sizeof(*y_array))
//				calculate += L_funtion(x_array[i]) * L_funtion(y_array[j]) * DS_U_plane.at<double>(yy + i - 4, xx + j - 4);
//				w += L_funtion(x_array[i]) * L_funtion(y_array[j]);
//				//cout << L_funtion(x_array[i]) * L_funtion(y_array[j]) << endl;
//			}
//		}
//		
//		calculate -= L_funtion(u) * L_funtion(v) * DS_U_plane.at<double>(yy, xx);
//		break;
//	case 2:
//		/*calculate = BC_funtion(1 + u)*BC_funtion(1 + v)*DS_V_plane.at<double>(yy - 1, xx - 1) + BC_funtion(1 + u)*BC_funtion(v)*DS_V_plane.at<double>(yy - 1, xx) + BC_funtion(1 + u)*BC_funtion(1 - v)*DS_V_plane.at<double>(yy - 1, xx + 1) + BC_funtion(1 + u)*BC_funtion(2 - v)*DS_V_plane.at<double>(yy - 1, xx + 2) + BC_funtion(u)*BC_funtion(1 + v)*DS_V_plane.at<double>(yy, xx - 1) + BC_funtion(u)*BC_funtion(1 - v)*DS_V_plane.at<double>(yy, xx + 1) + BC_funtion(u)*BC_funtion(2 - v)*DS_V_plane.at<double>(yy, xx + 2) + BC_funtion(1 - u)*BC_funtion(1 + v)*DS_V_plane.at<double>(yy + 1, xx - 1) + BC_funtion(1 - u)*BC_funtion(v)*DS_V_plane.at<double>(yy + 1, xx) + BC_funtion(1 - u)*BC_funtion(1 - v)*DS_V_plane.at<double>(yy + 1, xx + 1) + BC_funtion(1 - u)*BC_funtion(2 - v)*DS_V_plane.at<double>(yy + 1, xx + 2) + BC_funtion(2 - u)*BC_funtion(1 + v)*DS_V_plane.at<double>(yy + 2, xx - 1) + BC_funtion(2 - u)*BC_funtion(v)*DS_V_plane.at<double>(yy + 2, xx) + BC_funtion(2 - u)*BC_funtion(1 - v)*DS_V_plane.at<double>(yy + 2, xx + 1) + BC_funtion(2 - u)*BC_funtion(2 - v)*DS_V_plane.at<double>(yy + 2, xx + 2);*/
//		for (int i = 0; i < sizeof(x_array) / sizeof(*x_array); i++)
//		{
//			for (int j = 0; j < sizeof(y_array) / sizeof(*y_array); j++)
//			{
//				//if (xx + j - 2 <= sizeof(x_array) / sizeof(*x_array) && yy + i - 2 <= sizeof(y_array) / sizeof(*y_array))
//				calculate += L_funtion(x_array[i]) * L_funtion(y_array[j]) * DS_V_plane.at<double>(yy + i - 4, xx + j - 4);
//				w += L_funtion(x_array[i]) * L_funtion(y_array[j]);
//			}
//		}
//		calculate -= L_funtion(u) * L_funtion(v) * DS_V_plane.at<double>(yy, xx);
//		break;
//	default:
//		break;
//	}
//
//	return calculate/w;
//}

double L_funtion(double fx_value)
{
	
	double a = 4;
	if ( 0.0 < abs(fx_value) && abs(fx_value) < a )
	{
		return a * (sin(fx_value * M_PI) / (fx_value * M_PI) * (sin(fx_value*M_PI / a) / (fx_value * M_PI)));	
	}
	else if (fx_value == 0)
		return 1.0;
	else
		return 0;

}

void toYUV(Mat&imgrgb, Mat&dst) {
    dst = Mat::zeros(imgrgb.rows, imgrgb.cols, CV_64FC3);
    for (int i = 0; i < imgrgb.rows; i++)
        for (int j = 0; j < imgrgb.cols; j++) {
            dst.at<Vec3d>(i, j)[0] = 0.257*imgrgb.at<Vec3b>(i, j)[2] + 0.504*imgrgb.at<Vec3b>(i, j)[1] + 0.098*imgrgb.at<Vec3b>(i, j)[0] + 16;
            dst.at<Vec3d>(i, j)[1] = -0.148*imgrgb.at<Vec3b>(i, j)[2] - 0.291*imgrgb.at<Vec3b>(i, j)[1] + 0.439*imgrgb.at<Vec3b>(i, j)[0] + 128;
            dst.at<Vec3d>(i, j)[2] = 0.439*imgrgb.at<Vec3b>(i, j)[2] - 0.368*imgrgb.at<Vec3b>(i, j)[1] - 0.071*imgrgb.at<Vec3b>(i, j)[0] + 128;
        }
}
//CFA BYE
//void Convert_to_Bayer(int height, int width, Mat& bayerImg, const Mat& bgrImg, const Mat& CFA)
//{
//	bayerImg = Mat::zeros(height, width, CV_8UC1);
//	int channel;
//
//
//	for (int y = 0; y < height; y++)
//		for (int x = 0; x < width; x++)
//		{
//			channel = GetColor(CFA, y, x);
//			if (channel == CFA_B)
//				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[0];
//			else if (channel == CFA_G)
//				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[1];
//			else if (channel == CFA_R)
//				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[2];
//		}
//}

void Convert_to_DTDI(int height, int width, Mat& DTDI, const Mat& BGR)
{
	DTDI = Mat::zeros(BGR.rows, BGR.cols, CV_8UC3);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			DTDI.at<Vec3b>(y, x)[1] = BGR.at<Vec3b>(y, x)[1];

			if (x % 2 == 0)
			{
				DTDI.at<Vec3b>(y, x)[0] = BGR.at<Vec3b>(y, x)[0];
				DTDI.at<Vec3b>(y, x)[2] = 0;
			}
			else
			{
				DTDI.at<Vec3b>(y, x)[2] = BGR.at<Vec3b>(y, x)[2];
				DTDI.at<Vec3b>(y, x)[0] = 0;
			}
		}
	}
}

void toYCbCr(Mat&imgrgb, Mat&dst) {
    dst = Mat::zeros(imgrgb.rows, imgrgb.cols, CV_64FC3);
    for (int i = 0; i < imgrgb.rows; i++)
        for (int j = 0; j < imgrgb.cols; j++) {
            dst.at<Vec3d>(i, j)[0] = 0.299*imgrgb.at<Vec3b>(i, j)[2] + 0.587*imgrgb.at<Vec3b>(i, j)[1] + 0.114*imgrgb.at<Vec3b>(i, j)[0];
            dst.at<Vec3d>(i, j)[1] = (imgrgb.at<Vec3b>(i, j)[0] - dst.at<Vec3d>(i, j)[0])*0.564 + 128;
            dst.at<Vec3d>(i, j)[2] = (imgrgb.at<Vec3b>(i, j)[2] - dst.at<Vec3d>(i, j)[0])*0.713 + 128;
        }
}
//void toRGB(Mat&Result, Mat&dst){
//	dst = Mat::zeros(Result.rows, Result.cols, CV_64FC3);
//	for (int i = 0; i < Result.rows; i++)
//	for (int j = 0; j < Result.cols; j++){
//		if (1.164*(Result.at<Vec3d>(i, j)[0] - 16) + 2.018*(Result.at<Vec3d>(i, j)[1] - 128) + 0 * (Result.at<Vec3d>(i, j)[2] - 128)>255)
//			dst.at<Vec3d>(i, j)[0] = 255;
//		else
//			dst.at<Vec3d>(i, j)[0] = abs(1.164*(Result.at<Vec3d>(i, j)[0] - 16) + 2.018*(Result.at<Vec3d>(i, j)[1] - 128) + 0 * (Result.at<Vec3d>(i, j)[2] - 128));
//		if (1.164*(Result.at<Vec3d>(i, j)[0] - 16) - 0.391*(Result.at<Vec3d>(i, j)[1] - 128) - 0.813 * (Result.at<Vec3d>(i, j)[2] - 128)>255)
//			dst.at<Vec3d>(i, j)[1] = 255;
//		else
//			dst.at<Vec3d>(i, j)[1] = abs(1.164*(Result.at<Vec3d>(i, j)[0] - 16) - 0.391*(Result.at<Vec3d>(i, j)[1] - 128) - 0.813 * (Result.at<Vec3d>(i, j)[2] - 128));
//		if (1.164*(Result.at<Vec3d>(i, j)[0] - 16) + 0 * (Result.at<Vec3d>(i, j)[1] - 128) + 1.596 * (Result.at<Vec3d>(i, j)[2] - 128) > 255)
//			dst.at<Vec3d>(i, j)[2] = 255;
//		else
//			dst.at<Vec3d>(i, j)[2] = abs(1.164*(Result.at<Vec3d>(i, j)[0] - 16) + 0 * (Result.at<Vec3d>(i, j)[1] - 128) + 1.596 * (Result.at<Vec3d>(i, j)[2] - 128));
//	}
//}
void toRGB(Mat&Result, Mat&dst) {
    dst = Mat::zeros(Result.rows, Result.cols, CV_64FC3);
    for (int i = 0; i < Result.rows; i++)
        for (int j = 0; j < Result.cols; j++) {
            dst.at<Vec3d>(i, j)[0] = 1.164*(Result.at<Vec3d>(i, j)[0] - 16) + 2.018*(Result.at<Vec3d>(i, j)[1] - 128) + 0 * (Result.at<Vec3d>(i, j)[2] - 128);
            dst.at<Vec3d>(i, j)[1] = 1.164*(Result.at<Vec3d>(i, j)[0] - 16) - 0.391*(Result.at<Vec3d>(i, j)[1] - 128) - 0.813 * (Result.at<Vec3d>(i, j)[2] - 128);
            dst.at<Vec3d>(i, j)[2] = 1.164*(Result.at<Vec3d>(i, j)[0] - 16) + 0 * (Result.at<Vec3d>(i, j)[1] - 128) + 1.596 * (Result.at<Vec3d>(i, j)[2] - 128);
        }
	dst.convertTo(dst, CV_8UC3);
}
void toRGB2(Mat&Result, Mat&dst) {
    dst = Mat::zeros(Result.rows, Result.cols, CV_64FC3);
    for (int i = 0; i < Result.rows; i++)
        for (int j = 0; j < Result.cols; j++) {
            dst.at<Vec3d>(i, j)[0] = Result.at<Vec3d>(i, j)[0] + 1.773*(Result.at<Vec3d>(i, j)[1] - 128);
            dst.at<Vec3d>(i, j)[1] = Result.at<Vec3d>(i, j)[0] - 0.714*(Result.at<Vec3d>(i, j)[2] - 128) - 0.344*(Result.at<Vec3d>(i, j)[1] - 128);
            dst.at<Vec3d>(i, j)[2] = Result.at<Vec3d>(i, j)[0] + (1.403 *(Result.at<Vec3d>(i, j)[2] - 128));
        }
}

//void drawHistImg(const Mat &src, Mat &dst) {
//    int histSize = 256;
//    float histMaxValue = 0;
//    for (int i = 0; i < histSize; i++) {
//        float tempValue = src.at<float>(i);
//        if (histMaxValue < tempValue) {
//            histMaxValue = tempValue;
//        }
//    }
//
//    float scale = (0.9 * 256) / histMaxValue;
//
//    for (int i = 0; i < histSize; i++) {
//        int intensity = static_cast<int>(src.at<float>(i)*scale);
//        line(dst, Point(i, 256), Point(i, 256 - intensity), Scalar(0));
//    }
//}

void YUVSplit( Mat& YUV, int color, Mat &dst) {
    dst = Mat::zeros(YUV.rows, YUV.cols, CV_64FC1);

    if (color == 0)
        for (int i = 0; i < YUV.rows; i++)
			for (int j = 0; j < YUV.cols; j++) 
				dst.at<double>(i, j) = YUV.at<Vec3d>(i, j)[0];


    if (color == 1)
        for (int i = 0; i < YUV.rows; i++)
            for (int j = 0; j < YUV.cols; j++)
                dst.at<double>(i, j) = YUV.at<Vec3d>(i, j)[1];

    if (color == 2)
        for (int i = 0; i < YUV.rows; i++)
            for (int j = 0; j < YUV.cols; j++)
                dst.at<double>(i, j) = YUV.at<Vec3d>(i, j)[2];
}

void channelsSplit_420(Mat& Img, Mat& channel_1, Mat& channel_2, Mat& channel_3) {
	channel_1 = Mat::zeros(Img.rows,Img.cols,CV_64FC1);
	channel_2 = Mat::zeros(Img.rows/2, Img.cols/2, CV_64FC1);
	channel_3 = Mat::zeros(Img.rows/2, Img.cols/2, CV_64FC1);

	for (int i = 0; i < Img.rows; i++)
		for (int j = 0; j < Img.cols; j++)
			channel_1.at<double>(i, j) = Img.at<Vec3d>(i, j)[0];
	for (int i = 0; i < Img.rows / 2; i++) {
		for (int j = 0; j < Img.cols / 2; j++) {
			channel_2.at<double>(i, j) = Img.at<Vec3d>(i * 2, j * 2)[1];
			channel_3.at<double>(i, j) = Img.at<Vec3d>(i * 2, j * 2)[2];
		}
	}
}

void saveYUV(Mat&Y, Mat&U, Mat&V, fstream& yuvFILE)
{ /**
  * Save Y, U, V to YUV
  *
  * @param[in] Y:
  * @param[in] U:
  * @param[in] V:
  *
  * @param[out] yuvFILE: output YUV file
  */
	Mat Y8U, U8U, V8U;
	Y.convertTo(Y8U, CV_8UC1);
	U.convertTo(U8U, CV_8UC1);
	V.convertTo(V8U, CV_8UC1);

	if (!yuvFILE) {
		cerr << "in \"_saveYUV\" funciotn" << endl;
		cerr << "Can't write yuv file" << endl;
		cerr << "Path : " << yuvFILE._Stdstr << endl;

		system("pause");
		exit(-1);
	}

	for (int y = 0; y < Y8U.rows; ++y)
		for (int x = 0; x < Y8U.cols; ++x)
			yuvFILE.write((char *)Y8U.data + y * Y8U.step1(0) + x * Y8U.step1(1) + 0, 1);

	for (int y = 0; y < U8U.rows; ++y) {
		for (int x = 0; x < U8U.cols; ++x) {
			yuvFILE.write((char *)U8U.data + y * U8U.step1(0) + x * U8U.step1(1) + 0, 1);
		}
	}
	//yuvFILE.write("00000",5);

	for (int y = 0; y < V8U.rows; ++y)
		for (int x = 0; x < V8U.cols; ++x)
			yuvFILE.write((char *)V8U.data + y * V8U.step1(0) + x * V8U.step1(1) + 0, 1);
	yuvFILE.close();
}
void readYUV(string yuvPath, int height, int width, int numFrame, vector<Mat>& dst)
{ /**
  * readYUV YUV file
  *
  * @param[in] yuvPath: yuv's path
  * @param[in] height: img's height
  * @param[in] width: img's width
  * @param[in] numFrame: img's frames
  *
  * @param[out] dst: YUV
  */

    ifstream yuvFILE(yuvPath, ios::in | ios::binary);
    if (!yuvFILE) {
        cerr << "in \"_readYUV\" funciotn" << endl;
        cerr << "readYUV : " << yuvPath << endl;
        cerr << "Can't open .yuv File" << endl;
        system("pause");
        exit(-1);
    }

    Mat YUV(height, width, CV_8UC3, Scalar::all(0));

    unsigned char* bufY = new unsigned char[height*width];
    unsigned char* bufU = new unsigned char[(height / 2)*(width / 2)];
    unsigned char* bufV = new unsigned char[(height / 2)*(width / 2)];
    for (int f = 0; f < numFrame - 1; ++f) {
        yuvFILE.read((char*)bufY, height*width);
        yuvFILE.read((char*)bufU, height / 2 * width / 2);
        yuvFILE.read((char*)bufV, height / 2 * width / 2);
    }

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            yuvFILE.read((char *)YUV.data + y * YUV.step1(0) + x * YUV.step1(1) + 0, 1);

	for (int y = 0; y < height / 2; ++y) {
		for (int x = 0; x < width / 2; ++x) {
			yuvFILE.read((char *)YUV.data + y * YUV.step1(0) + x * YUV.step1(1) + 1, 1);
		}
	}

    for (int y = 0; y < height / 2; ++y)
        for (int x = 0; x < width / 2; ++x)
            yuvFILE.read((char *)YUV.data + y * YUV.step1(0) + x * YUV.step1(1) + 2, 1);
    dst.push_back(YUV);
}
void combine420(Mat&Y, Mat&U, Mat&V, Mat&dst)
{ /**
  * combine Y,U,V to YUV mat
  *
  * @param[in] Y:
  * @param[in] U:
  * @param[in] V:
  *
  * @param[out] dst: Mat YUV
  */

    dst = Mat::zeros(Y.rows, Y.cols, CV_64FC3);
    for (int i = 0; i < Y.rows; i++)
        for (int j = 0; j < Y.cols; j++) {
            dst.at<Vec3d>(i, j)[0] = Y.at<uchar>(i, j);
            if (i < Y.rows / 2 && j < Y.cols / 2) {
                dst.at<Vec3d>(i, j)[1] = U.at<uchar>(i, j);
                dst.at<Vec3d>(i, j)[2] = V.at<uchar>(i, j);
            }
        }
}

void combine420_64F(Mat&Y, Mat&U, Mat&V, Mat&dst)
{ /**
  * combine Y,U,V to YUV mat
  *
  * @param[in] Y:
  * @param[in] U:
  * @param[in] V:
  *
  * @param[out] dst: Mat YUV
  */

	dst = Mat::zeros(Y.rows, Y.cols, CV_64FC3);
	for (int i = 0; i < Y.rows; i++)
		for (int j = 0; j < Y.cols; j++) {
			dst.at<Vec3d>(i, j)[0] = Y.at<double>(i, j);
			if (i < Y.rows / 2 && j < Y.cols / 2) {
				dst.at<Vec3d>(i, j)[1] = U.at<double>(i, j);
				dst.at<Vec3d>(i, j)[2] = V.at<double>(i, j);
			}
		}
}

void expand(Mat& src, Mat&dst)
{ /**
  * expande U, V
  *
  * This function expand U, V to correct location
  * i.e U => 1 2 expand to 1 x 2 x
  *          3 4           x x x x
  *                        3 x 4 x
  *                        x x x x
  * @param[in] src: 420 YUV
  *
  * @param[out] dst: 444 YUV
  */
	dst = Mat::zeros(src.rows, src.cols, CV_64FC3);
	
	for (int i = 0; i < src.rows / 2; ++i)
		for (int j = 0; j < src.cols / 2; ++j) {
			dst.at<Vec3d>(i * 2, j * 2)[1] = src.at<Vec3b>(i, j)[1];
			dst.at<Vec3d>(i * 2, j * 2)[2] = src.at<Vec3b>(i, j)[2];
		}
	for (int i = 0; i < src.rows; ++i)
		for (int j = 0; j < src.cols; ++j) {
			dst.at<Vec3d>(i, j)[0] = src.at<Vec3b>(i, j)[0];
		}
}

void expand_64F(Mat& src, Mat&dst)
{ /**
  * expande U, V
  *
  * This function expand U, V to correct location
  * i.e U => 1 2 expand to 1 x 2 x
  *          3 4           x x x x
  *                        3 x 4 x
  *                        x x x x
  * @param[in] src: 420 YUV (8UC3)
  *
  * @param[out] dst: 444 YUV
  */
	dst = Mat::zeros(src.rows, src.cols, CV_64FC3);

	for (int i = 0; i < src.rows / 2; ++i)
		for (int j = 0; j < src.cols / 2; ++j) {
			dst.at<Vec3d>(i * 2, j * 2)[1] = src.at<Vec3d>(i, j)[1];
			dst.at<Vec3d>(i * 2, j * 2)[2] = src.at<Vec3d>(i, j)[2];
		}
	for (int i = 0; i < src.rows; ++i)
		for (int j = 0; j < src.cols; ++j) {
			dst.at<Vec3d>(i, j)[0] = src.at<Vec3d>(i, j)[0];
		}
}

void expand(Mat Y, Mat U, Mat V, Mat &dst) {
    dst = Mat::zeros(Y.rows, Y.cols, CV_64FC3);
    for (int i = 0; i < Y.rows / 2; ++i)
        for (int j = 0; j < Y.cols / 2; ++j) {
            dst.at<Vec3d>(i * 2, j * 2)[1] = U.at<double>(i, j);
            dst.at<Vec3d>(i * 2, j * 2)[2] = V.at<double>(i, j);
        }
    for (int i = 0; i < Y.rows; ++i)
        for (int j = 0; j < Y.cols; ++j) {
            dst.at<Vec3d>(i, j)[0] = Y.at<double>(i, j);
        }
}


void final(Mat& expand444, Mat& Ubar, Mat& Vbar, Mat& Ufinal, Mat& Vfinal) {
    Mat Residual_U = Mat::zeros(Ubar.rows, Ubar.cols, CV_64FC1);
    Mat Residual_V = Mat::zeros(Ubar.rows, Ubar.cols, CV_64FC1);

    Ufinal = Mat::zeros(Ubar.rows, Ubar.cols, CV_64FC1);
    Vfinal = Mat::zeros(Vbar.rows, Vbar.cols, CV_64FC1);

    //Residual
    for (int i = 0; i < Ubar.rows; i++)
        for (int j = 0; j < Ubar.cols; j++) {
            Residual_U.at<double>(i, j) = (double)(expand444.at<Vec3d>(i, j)[1]) - Ubar.at<double>(i, j);
            Residual_V.at<double>(i, j) = (double)(expand444.at<Vec3d>(i, j)[2]) - Vbar.at<double>(i, j);
        }
    //Bilinear Inter
    for (int i = 0; i < Ubar.rows; i++)
        for (int j = 0; j < Ubar.cols; j++) {

            if (i % 2 == 0 && j % 2 == 1) {
                Residual_U.at<double>(i, j) = (Residual_U.at<double>(mir(i, Ubar.rows), mir(j - 1, Ubar.cols)) + Residual_U.at<double>(mir(i, Ubar.rows), mir(j + 1, Ubar.cols))) / 2;
                Residual_V.at<double>(i, j) = (Residual_V.at<double>(mir(i, Vbar.rows), mir(j - 1, Vbar.cols)) + Residual_V.at<double>(mir(i, Vbar.rows), mir(j + 1, Vbar.cols))) / 2;
            }
            else if (i % 2 == 1 && j % 2 == 0) {
                Residual_U.at<double>(i, j) = (Residual_U.at<double>(mir(i - 1, Ubar.rows), mir(j, Ubar.cols)) + Residual_U.at<double>(mir(i + 1, Ubar.rows), mir(j, Ubar.cols))) / 2;
                Residual_V.at<double>(i, j) = (Residual_V.at<double>(mir(i - 1, Vbar.rows), mir(j, Vbar.cols)) + Residual_V.at<double>(mir(i + 1, Vbar.rows), mir(j, Vbar.cols))) / 2;
            }
            else if (i % 2 == 1 && j % 2 == 1) {
                Residual_U.at<double>(i, j) = (Residual_U.at<double>(mir(i - 1, Ubar.rows), mir(j - 1, Ubar.cols)) + Residual_U.at<double>(mir(i - 1, Ubar.rows), mir(j + 1, Ubar.cols)) + Residual_U.at<double>(mir(i + 1, Ubar.rows), mir(j + 1, Ubar.cols)) + Residual_U.at<double>(mir(i + 1, Ubar.rows), mir(j - 1, Ubar.cols))) / 4;
                Residual_V.at<double>(i, j) = (Residual_V.at<double>(mir(i - 1, Vbar.rows), mir(j - 1, Vbar.cols)) + Residual_V.at<double>(mir(i - 1, Vbar.rows), mir(j + 1, Vbar.cols)) + Residual_V.at<double>(mir(i + 1, Vbar.rows), mir(j + 1, Vbar.cols)) + Residual_V.at<double>(mir(i + 1, Vbar.rows), mir(j - 1, Vbar.cols))) / 4;
            }
        }

    //Add
    for (int i = 0; i < Ubar.rows; i++)
        for (int j = 0; j < Ubar.cols; j++)
            Ufinal.at<double>(i, j) = Residual_U.at<double>(i, j) + Ubar.at<double>(i, j);

    for (int i = 0; i < Vbar.rows; i++)
        for (int j = 0; j < Vbar.cols; j++)
            Vfinal.at<double>(i, j) = Residual_V.at<double>(i, j) + Vbar.at<double>(i, j);
}
void UV_enhance(Mat & Ufinal, Mat& Vfinal, Mat& expand444, Mat& Out_Y) {
    int threshold_V = 10;
    int threshold_U = 10;
    Mat Out_pix = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
    ////Horizontal
    //for (int i = 0; i < Vfinal.rows; i++)
    //for (int j = 0; j < Vfinal.cols; j++){
    //	if (abs(Vfinal.at<double>(i, j) - Vfinal.at<double>(i, mir(j + 1, Vfinal.cols)))>threshold_V && abs(Vfinal.at<double>(i, j) - Vfinal.at<double>(i, mir(j - 1, Vfinal.cols)))>threshold_V)
    //		Vfinal.at<double>(i, j) = (Vfinal.at<double>(i, mir(j + 1, Vfinal.cols)) + Vfinal.at<double>(i, mir(j - 1, Vfinal.cols))) / 2;
    //	/*if (abs(Ufinal.at<double>(i, j) - Ufinal.at<double>(i, mir(j + 1, Ufinal.cols)))>threshold_U && abs(Ufinal.at<double>(i, j) - Ufinal.at<double>(i, mir(j - 1, Ufinal.cols)))>threshold_U)
    //		Ufinal.at<double>(i, j) = (Ufinal.at<double>(i, mir(j + 1, Ufinal.cols)) + Ufinal.at<double>(i, mir(j - 1, Ufinal.cols))) / 2;*/
    //}
    ////Vertical
    //for (int i = 0; i < Vfinal.rows; i++)
    //for (int j = 0; j < Vfinal.cols; j++){
    //	if (abs(Vfinal.at<double>(i, j) - Vfinal.at<double>(mir(i - 1, Vfinal.rows), j)) > threshold_V && abs(Vfinal.at<double>(i, j) - Vfinal.at<double>(mir(i + 1, Vfinal.rows), j)) > threshold_V)
    //		Vfinal.at<double>(i, j) = (Vfinal.at<double>(mir(i - 1, Vfinal.rows), j) + Vfinal.at<double>(mir(i + 1, Vfinal.rows), j)) / 2;
    //	/*if (abs(Ufinal.at<double>(i, j) - Ufinal.at<double>(mir(i - 1, Ufinal.rows), j)) > threshold_U && abs(Ufinal.at<double>(i, j) - Ufinal.at<double>(mir(i + 1, Ufinal.rows), j)) > threshold_U)
    //	Ufinal.at<double>(i, j) = (Ufinal.at<double>(mir(i - 1, Ufinal.rows), j) + Ufinal.at<double>(mir(i + 1, Ufinal.rows), j)) / 2;*/
    //}

    int h = 5, w = 5;

    //===V	
    //double avgpix = 0;
    //for (int i = 0; i < expand444.rows; i++)
    //for (int j = 0; j < expand444.cols; j++){
    //	avgpix += Vfinal.at<double>(i, j);
    //}
    //avgpix /= expand444.rows*expand444.cols;
    //cout << endl << "Average Pixel : " << avgpix << endl;
    //for (int i = 0; i < expand444.rows; i++)
    //for (int j = 0; j < expand444.cols; j++){
    //	if (abs(Vfinal.at<double>(i, j) - avgpix) >= 70)
    //Out_pix.at<double>(mir(i, expand444.rows), mir(j, expand444.cols)) = 255;
    //}
    //Show(Out_pix);
    //
    //for (int i = 0; i < Vfinal.rows; i++)
    //for (int j = 0; j < Vfinal.cols; j++){
    //	if (Out_pix.at<double>(mir(i, expand444.rows), mir(j, expand444.cols)) == 255)
    //		Vfinal.at<double>(i, j) = avgpix;
    //}
    //
    //for (int i = 0; i < Vfinal.rows; i++)
    //for (int j = 0; j < Vfinal.cols; j++){
    //	Out_pix.at<double>(mir(i, expand444.rows), mir(j, expand444.cols)) = 0;
    //}
    //avgpix = 0;
    ////U
    //for (int i = 0; i < expand444.rows; i++)
    //for (int j = 0; j < expand444.cols; j++){
    //	avgpix += Ufinal.at<double>(i, j);
    //}
    //avgpix /= expand444.rows*expand444.cols;
    //cout << endl << "Average Pixel : " << avgpix << endl;
    //for (int i = 0; i < expand444.rows; i++)
    //for (int j = 0; j < expand444.cols; j++){
    //	if (abs(Ufinal.at<double>(i, j) - avgpix) >= 70)
    //		Out_pix.at<double>(mir(i, expand444.rows), mir(j, expand444.cols)) = 255;
    //}
    //Show(Out_pix);

    //for (int i = 0; i < Vfinal.rows; i++)
    //for (int j = 0; j < Vfinal.cols; j++){
    //	if (Out_pix.at<double>(mir(i, expand444.rows), mir(j, expand444.cols)) == 255)
    //		Ufinal.at<double>(i, j) = avgpix;
    //}

    for (int i = 0; i < Vfinal.rows; i++)
        for (int j = 0; j < Vfinal.cols; j++) {
            //	if (Out_Y.at<double>(i, j) == 255){
            //--
            double max_V = -100, min_V = 300;
            for (int k = i - (h / 2); k <= i + (h / 2); k++)
                for (int l = j - (w / 2); l <= j + (w / 2); l++) {
                    if (mir(k, Vfinal.rows) % 2 == 0 && mir(l, Vfinal.cols) % 2 == 0) {
                        //				if (Vfinal.at<double>(mir(k, Vfinal.rows), mir(l, Vfinal.cols)) > max_V)
                        if (expand444.at<Vec3d>(mir(k, Ufinal.rows), mir(l, Ufinal.cols))[2] > max_V)
                            max_V = Vfinal.at<double>(mir(k, Vfinal.rows), mir(l, Vfinal.cols));
                        //				if (Vfinal.at<double>(mir(k, Vfinal.rows), mir(l, Vfinal.cols)) < min_V)
                        if (expand444.at<Vec3d>(mir(k, Ufinal.rows), mir(l, Ufinal.cols))[2] < min_V)
                            min_V = Vfinal.at<double>(mir(k, Vfinal.rows), mir(l, Vfinal.cols));
                    }
                }
            //--
            if (Vfinal.at<double>(mir(i, Vfinal.rows), mir(j, Vfinal.cols)) > max_V + threshold_V || Vfinal.at<double>(mir(i, Vfinal.rows), mir(j, Vfinal.cols)) < min_V - threshold_V) {
                Vfinal.at<double>(i, j) = (Vfinal.at<double>(mir(i - 1, Vfinal.rows), mir(j - 1, Vfinal.cols)) + Vfinal.at<double>(mir(i - 1, Vfinal.rows), mir(j, Vfinal.cols)) + Vfinal.at<double>(mir(i - 1, Vfinal.rows), mir(j + 1, Vfinal.cols)) + Vfinal.at<double>(mir(i, Vfinal.rows), mir(j - 1, Vfinal.cols))) / 4;
                //	Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, Vfinal.rows), mir(j - 1, Vfinal.cols))[2] + expand444.at<Vec3d>(mir(i - 1, Vfinal.rows), mir(j, Vfinal.cols))[2] + expand444.at<Vec3d>(mir(i - 1, Vfinal.rows), mir(j + 1, Vfinal.cols))[2] + expand444.at<Vec3d>(mir(i, Vfinal.rows), mir(j - 1, Vfinal.cols))[2]) / 4;
            }
            //	}
        }
    //===U
    for (int i = 0; i < Ufinal.rows; i++)
        for (int j = 0; j < Ufinal.cols; j++) {
            //	if (Out_Y.at<double>(i, j) == 255){
            //--
            double max_a_U = -100, min_a_U = 300;
            for (int k = i - (h / 2); k <= i + (h / 2); k++) {
                for (int l = j - (w / 2); l <= j + (w / 2); l++) {
                    if (mir(k, Ufinal.rows) % 2 == 0 && mir(l, Ufinal.cols) % 2 == 0) {
                        //					if (Ufinal.at<double>(mir(k, Ufinal.rows), mir(l, Ufinal.cols)) > max_a_U)
                        if (expand444.at<Vec3d>(mir(k, Ufinal.rows), mir(l, Ufinal.cols))[1] > max_a_U)
                            max_a_U = Ufinal.at<double>(mir(k, Ufinal.rows), mir(l, Ufinal.cols));
                        //					if (Ufinal.at<double>(mir(k, Ufinal.rows), mir(l, Ufinal.cols)) < min_a_U)
                        if (expand444.at<Vec3d>(mir(k, Ufinal.rows), mir(l, Ufinal.cols))[1] < min_a_U)
                            min_a_U = Ufinal.at<double>(mir(k, Ufinal.rows), mir(l, Ufinal.cols));
                    }
                }
            }
            //if (i == 188 && j == 316){
            //	for (int k = i - (h / 2); k <= i + (h / 2); k++){
            //		for (int l = j - (w / 2); l <= j + (w / 2); l++){
            //			cout << Ufinal.at<double>(mir(k, Ufinal.rows), mir(l, Ufinal.cols)) << "    ";
            //		}
            //		cout << endl;
            //	}
            //	cout << endl;
            //	cout << "min_a_U : " << min_a_U << endl;
            //	cout << "max_a_U : " << max_a_U << endl;
            //	cout << "pixel : " << Ufinal.at<double>(mir(i, Ufinal.rows), mir(j, Ufinal.cols)) << endl << endl;
            //}
            //--
            if (Ufinal.at<double>(mir(i, Ufinal.rows), mir(j, Ufinal.cols)) > max_a_U + threshold_U || Ufinal.at<double>(mir(i, Ufinal.rows), mir(j, Ufinal.cols)) < min_a_U - threshold_U) {
                //if (i == 253 && j == 33){
                //	cout << "min_a_U : " << min_a_U << endl;
                //	cout << "max_a_U : " << max_a_U << endl;
                //	cout << "pixel : " << Ufinal.at<double>(mir(i, Ufinal.rows), mir(j, Ufinal.cols)) << endl << endl;
                //}
                Ufinal.at<double>(i, j) = (Ufinal.at<double>(mir(i - 1, Ufinal.rows), mir(j - 1, Ufinal.cols)) + Ufinal.at<double>(mir(i - 1, Ufinal.rows), mir(j, Ufinal.cols)) + Ufinal.at<double>(mir(i - 1, Ufinal.rows), mir(j + 1, Ufinal.cols)) + Ufinal.at<double>(mir(i, Ufinal.rows), mir(j - 1, Ufinal.cols))) / 4;
                //Ufinal.at<double>(i, j) = Ufinal.at<double>(mir(i - 1, Ufinal.rows), mir(j - 1, Ufinal.cols));
            }
            //if (i == 188 && j == 316){
            //	cout << "pixel : " << Ufinal.at<double>(mir(i, Ufinal.rows), mir(j, Ufinal.cols)) << endl;
            //}
            //	}
        }
    /*for (int i = 0; i < expand444.rows; i++)
    for (int j = 0; j < expand444.cols; j++){
    if (i % 2 == 0 && j % 2 == 0){
    Ufinal.at<double>(i, j) = expand444.at<Vec3d>(i, j)[1];
    Vfinal.at<double>(i, j) = expand444.at<Vec3d>(i, j)[2];
    }
    }*/
}
void bilinear_UV(Mat& expand444, Mat& Ufinal, Mat& Vfinal) {
    Ufinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
    Vfinal = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

    for (int i = 0; i < expand444.rows; i++)
        for (int j = 0; j < expand444.cols; j++) {
            if (i % 2 == 0 && j % 2 == 0) {
                Ufinal.at<double>(i, j) = expand444.at<Vec3d>(i, j)[1];
                Vfinal.at<double>(i, j) = expand444.at<Vec3d>(i, j)[2];
            }
            else if (i % 2 == 0 && j % 2 == 1) {
                Ufinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[1] + expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[1]) / 2;
                Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j - 1, expand444.cols))[2] + expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[2]) / 2;
            }
            else if (i % 2 == 1 && j % 2 == 0) {
                Ufinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[1] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[1]) / 2;
                Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j, expand444.cols))[2] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[2]) / 2;
            }
            else if (i % 2 == 1 && j % 2 == 1) {
                Ufinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[1] + expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[1] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[1] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[1]) / 4;
                Vfinal.at<double>(i, j) = (expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j - 1, expand444.cols))[2] + expand444.at<Vec3d>(mir(i - 1, expand444.rows), mir(j + 1, expand444.cols))[2] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j - 1, expand444.cols))[2] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[2]) / 4;
            }
        }
}
void combine444(Mat&Y, Mat&Ufinal, Mat&Vfinal, Mat&dst) {
    dst = Mat::zeros(Y.rows, Y.cols, CV_64FC3);
    for (int i = 0; i < Y.rows; i++)
        for (int j = 0; j < Y.cols; j++) {
            dst.at<Vec3d>(i, j)[0] = Y.at<double>(i, j);
            dst.at<Vec3d>(i, j)[1] = Ufinal.at<double>(i, j);
            dst.at<Vec3d>(i, j)[2] = Vfinal.at<double>(i, j);
        }
}

double Error_Linear_Regression(Mat & shift_Y, Mat &expand444, int i, int j) {

    //	cout << "eeee" << endl;
    double error = 0;

    double a = 0;
    double b = 0;

    double Y_val[1000] = { 0 };
    double U_val[1000] = { 0 };

    int count = 0;
    double Y_avg = 0;
    double U_avg = 0;
    double bot = 0;
    //-------------------------------------------------------------------------------------------------------------------------------------------
    for (int k = i - 2; k <= i + 2; k++)
        for (int l = j - 2; l <= j + 2; l++) {
            if (mir(k, expand444.rows) % 2 == 0 && mir(l, expand444.cols) % 2 == 0) {
                Y_val[count] = shift_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols));
                U_val[count] = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[1];

                Y_avg += Y_val[count];
                U_avg += U_val[count];
                count++;
            }
        }
    Y_avg /= count;
    U_avg /= count;

    /*for (int z = 0; z < count; z++)
    cout << Y_val[z] <<", ";
    cout << endl;
    for (int z = 0; z < count; z++)
        cout << U_val[z] << ", ";
    cout << endl;*/

    for (int z = 0; z < count; z++) {
        a += (Y_val[z] - Y_avg)*(U_val[z] - U_avg);
        bot += pow(Y_val[z] - Y_avg, 2);
    }

    a /= (bot + 0.01);
    b = U_avg - a * Y_avg;

    //cout << a<<", "<<b << endl;
    //-------------------------------------------------------------------------------------------------------------------------------------------

    for (int z = 0; z < count; z++)
        error += pow((Y_val[z] * a + b) - U_val[z], 2) / (a*a + 1);
    //error += abs((Y_val[z] * a + b) - U_val[z]);

//error /= count;
//cout << error << endl;


    return error;
}

Mat Regression(Mat& expand444, Mat& shift_Y, Mat& shift_U, int h, int w) {
    double th = 0.00001 * 255 * 255;

    Mat real_Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);;
    for (int i = 0; i < expand444.rows; i++)
        for (int j = 0; j < expand444.cols; j++)
            real_Y.at<double>(i, j) = expand444.at<Vec3d>(i, j)[0];

    /*Mat shift_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);;
    for (int i = 0; i < expand444.rows; i++)
    for (int j = 0; j < expand444.cols; j++)
        shift_U.at<double>(i, j) = expand444.at<Vec3d>(i, j)[1];*/

    Mat mask_of_sub = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
    for (int i = 0; i < expand444.rows; i++)
        for (int j = 0; j < expand444.cols; j++)
            if (i % 2 == 0 && j % 2 == 0)
                mask_of_sub.at<double>(i, j) = 1;

    Mat count = boxfilter(mask_of_sub, h, w);
    Mat one = Mat::ones(expand444.rows, expand444.cols, CV_64FC1);
    Mat count2 = boxfilter(one, h, w); //每個local patch的大小

    Mat YxM = shift_Y.mul(mask_of_sub);
    Mat mean_Y = boxfilter(YxM, h, w);
    divide(mean_Y, count, mean_Y);

    Mat mean_U = boxfilter(shift_U, h, w);
    divide(mean_U, count, mean_U);

    Mat YxU = shift_Y.mul(shift_U);
    Mat mean_YU = boxfilter(YxU, h, w); //每個local patch中 被採樣Guide x BeGuide點的平均
    divide(mean_YU, count, mean_YU);

    Mat cov_YU = mean_YU - mean_Y.mul(mean_U);

    Mat YxYxM = shift_Y.mul(shift_Y.mul(mask_of_sub));
    Mat mean_YY = boxfilter(YxYxM, h, w); //每個local patch中 被採樣Guide的平方 的平均
    divide(mean_YY, count, mean_YY);

    Mat var_Y = mean_YY - mean_Y.mul(mean_Y); //變異數 = 被採樣Guide的平方 的平均 - 每點被採樣Guide點平均的平方
    for (int i = 0; i < var_Y.rows; i++)
        for (int j = 0; j < var_Y.cols; j++)
            if (var_Y.at<double>(i, j) < th)
                var_Y.at<double>(i, j) = th;

    /// linear coefficients
    Mat a, b;
    divide(cov_YU, (var_Y + 0.00001), a);
    b = mean_U - a.mul(mean_Y);

    Mat mean_a = boxfilter(a, h, w);  //每個local patch中 平均的a
    divide(mean_a, count2, mean_a);

    Mat mean_b = boxfilter(b, h, w); //每個local patch中 平均的b
    divide(mean_b, count2, mean_b);

    /// output
    Mat Ubar = a.mul(real_Y) + b;
    //Mat Ubar = mean_a.mul(real_Y) + mean_b;

    //Show(Ubar);

    //
    for (int i = 0; i < Ubar.rows; i++)
        for (int j = 0; j < Ubar.cols; j++)
            Ubar.at<double>(i, j) = (Ubar.at<double>(i, j) < 0 ? 0 : Ubar.at<double>(i, j)) > 255 ? 255 : Ubar.at<double>(i, j);

    return Ubar;
}

//int judge(Mat& expand444, stringstream & path, stringstream & downsample, stringstream& QP) {
//    TickMeter time;
//    //time.start();
//
//    int method = 1;
//
//    Mat shift_Y_1 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//A
//    Mat shift_Y_2 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//L
//    Mat shift_Y_3 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//R
//    Mat shift_Y_4 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//DIRECT
//    Mat shift_Y_5 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//MPEG-B
//    Mat shift_Y_6 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//IDID_NEDI
//    Mat shift_Y_7 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//CS_BILINEAR
//    Mat shift_Y_8 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//CS_BICUBIC
//
//    /*Mat Y= Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//    for (int i = 0; i < expand444.rows; i++)
//    for (int j = 0; j < expand444.cols; j++){
//        Y.at <double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//    }
//    Show(Y);*/
//    time.start();
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++) {
//            if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0) {
//                shift_Y_1.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0]
//                    + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0]) / 4;
//            }
//        }
//    time.stop();
//    cout << "A Time : " << time.getTimeSec() << endl;
//    //system("Pause");
//
//    time.start();
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++) {
//            if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0) {
//                shift_Y_2.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0]) / 2;
//            }
//        }
//    time.stop();
//    cout << "L Time : " << time.getTimeSec() << endl;
//    //system("Pause");
//
//    time.start();
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++) {
//            if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0) {
//                shift_Y_3.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0]) / 2;
//            }
//        }
//    time.stop();
//    cout << "R Time : " << time.getTimeSec() << endl;
//    //system("Pause");
//
//    time.start();
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++) {
//            if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0) {
//                shift_Y_4.at <double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//            }
//        }
//    time.stop();
//    cout << "Direct Time : " << time.getTimeSec() << endl;
//    system("Pause");
//
//    Mat coef = (Mat_<double>(1, 13) << 2, 0, -4, -3, 5, 19, 26, 19, 5, -3, -4, 0, 2);
//    //for (int i = 0; i < expand444.rows; i++)
//    //for (int j = 0; j < expand444.cols; j++)
//    //	shift_Y_5.at<double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//    int yy = 0;
//    int xx = 0;
//
//    for (int y = 0; y < expand444.rows; y += 2)
//    {
//        xx = 0;
//        for (int x = 0; x < expand444.cols; x += 2)
//        {
//            double DS_Y_Val = 0;
//            int index = 0;
//
//            for (int offset = -6; offset <= 6; offset++)
//            {
//                DS_Y_Val += expand444.at<Vec3d>(mir(y, expand444.rows), mir(x + offset, expand444.cols))[0] * coef.at<double>(0, index);
//                index++;
//            }
//            DS_Y_Val /= 64;
//
//            shift_Y_5.at<double>(y, x) = DS_Y_Val;
//
//            xx++;
//        }
//        yy++;
//    }
//    //Show(shift_Y_5);
//
//
//    //IDID_NEDI_to_Y(expand444, shift_Y_6, shift_Y_6, shift_Y_6);
//    //Show(shift_Y_6);
//
////======================================================================
//    int h = 16;
//    int w = 16;
//    Mat Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//    Mat imitated_Y = Mat::zeros(expand444.rows / 2, expand444.cols / 2, CV_64FC1);
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++) {
//            Y.at<double>(i, j) = expand444.at<Vec3d>(i, j)[0];
//        }
//    //Show(Y);
//    for (int i = 0; i < expand444.rows; i += 16)
//        for (int j = 0; j < expand444.cols; j += 16) {
//            //-------------------------------------求H-----------------------------------------
//
//            Mat Y_small = Mat::zeros(h*w, 1, CV_64FC1);
//
//            int count = 0;
//            for (int k = i; k < i + h; k++)
//                for (int l = j; l < j + w; l++) {
//                    Y_small.at<double>(count++, 0) = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//                }
//
//            Mat HMatrix(h*w, (h*w) / 4, CV_64F, Scalar::all(0));
//            Decide_HMatrix(HMatrix, 16);
//
//            //-------------------------------------求邊--------------------------------------------
//            Mat Edge_Y(h*w, 1, CV_64F, Scalar::all(0));
//            IDID_edge(Y, Edge_Y, 16, i, j);
//            //-------------------------------------求最終-----------------------------------------
//            Mat Temp_Y;
//
//            Mat temp;
//            invert(Mat(HMatrix.t()*HMatrix), temp, 0);
//            Temp_Y = temp * HMatrix.t()*(Y_small - Edge_Y);
//
//            count = 0;
//            for (int k = i / 2; k < (i + h) / 2; k++)
//                for (int l = j / 2; l < (j + w) / 2; l++) {
//                    imitated_Y.at<double>(mir(k, expand444.rows / 2), mir(l, expand444.cols / 2)) = Temp_Y.at<double>(count++, 0);
//                }
//        }//end for(i, j)
//        //Show(imitated_Y);
//        //imwrite("21_imitated_Y.bmp", imitated_Y);
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++)
//            if (i % 2 == 0 && j % 2 == 0)
//                shift_Y_7.at<double>(i, j) = imitated_Y.at<double>(i / 2, j / 2);
//    //Show(shift_Y_7);
////=========================================================================
//    vector<Mat> decode;
//    readYUV("YUV\\MIDID_BC\\" + path.str() + "_MIDID_BC_smallY.yuv", expand444.rows, expand444.cols, 1, decode);
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++)
//            if (i % 2 == 0 && j % 2 == 0)
//                shift_Y_8.at<double>(i, j) = decode[0].at<Vec3b>(i / 2, j / 2)[1];
//
//    //=========================================================================
//    //=========================================================================
//    int text_count = 0;
//    int right_count_1 = 0;
//    int right_count_2 = 0;
//    int right_count_3 = 0;
//    int right_count_4 = 0;
//    int right_count_5 = 0;
//    int right_count_6 = 0;
//    int right_count_7 = 0;
//    int right_count_8 = 0;
//
//    double sum_error_1 = 0;
//    double sum_error_2 = 0;
//    double sum_error_3 = 0;
//    double sum_error_4 = 0;
//    double sum_error_5 = 0;
//    double sum_error_6 = 0;
//    double sum_error_7 = 0;
//    double sum_error_8 = 0;
//
//    int Ns;
//    for (int i = 0; i < expand444.rows; i++)
//        for (int j = 0; j < expand444.cols; j++) {
//            //----------------------------block-wise content analysis------------------------------------
//            Ns = 0;
//            /*for (int k = i - 2; k <= i + 2; k++)
//            for (int l = j - 2; l <= j + 2; l++){
//                if (abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k - 1, expand444.rows), mir(l, expand444.cols))[0])>50 ||
//                    abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k, expand444.rows), mir(l - 1, expand444.cols))[0])>50){
//                    Ns++;
//                }
//            }*/
//            //---------------------------------------------------------------------------------------------------------
//
//
//            if (Ns >= 0) {
//                /*Mat show = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//                for (int k = i - 2; k <= i + 2; k++)
//                for (int l = j - 2; l <= j + 2; l++){
//                    show.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//                }*/
//                //Show(show);
//
//                text_count++;
//                //cout << "(" << i << " ," << j << ")" << endl;
//
//                //Show(shift_Y_1);
//                //Show(shift_Y_2);
//                //Show(shift_Y_3);
//                //Show(shift_Y_4);
//                //imwrite("shift_Y_1.bmp", shift_Y_1);
//
//                //double error_sum = 0;
//
//            //	int min_error = min(sum_error_1, min(sum_error_2, min(sum_error_3, min(right_count_4, min(right_count_5, min(right_count_6, min(right_count_7, right_count_8)))))));
//
//
//
//                if (sum_error_1 == sum_error_2 && sum_error_1 == sum_error_3 && sum_error_1 == sum_error_4 && sum_error_1 == sum_error_5 && sum_error_1 == sum_error_6 && sum_error_1 == sum_error_7 && sum_error_1 == sum_error_8) {
//                    right_count_1++;
//                    /*right_count_2++;
//                    right_count_3++;
//                    right_count_4++;
//                    right_count_5++;
//                    right_count_6++;
//                    right_count_7++;
//                    right_count_8++;*/
//
//
//                    sum_error_1 += Error_Linear_Regression(shift_Y_1, expand444, i, j);
//                    /*sum_error_2 += Error_Linear_Regression(shift_Y_2, expand444, i, j);
//                    sum_error_3 += Error_Linear_Regression(shift_Y_3, expand444, i, j);
//                    sum_error_4 += Error_Linear_Regression(shift_Y_4, expand444, i, j);
//                    sum_error_5 += Error_Linear_Regression(shift_Y_5, expand444, i, j);
//                    sum_error_6 += Error_Linear_Regression(shift_Y_6, expand444, i, j);
//                    sum_error_7 += Error_Linear_Regression(shift_Y_7, expand444, i, j);
//                    sum_error_8 += Error_Linear_Regression(shift_Y_8, expand444, i, j);*/
//                }
//
//
//                else if (sum_error_1 <= sum_error_2 && sum_error_1 <= sum_error_3 && sum_error_1 <= sum_error_4 && sum_error_1 <= sum_error_5 && sum_error_1 <= sum_error_6 && sum_error_1 <= sum_error_7 && sum_error_1 <= sum_error_8) {
//                    right_count_1++;
//                    sum_error_1 += Error_Linear_Regression(shift_Y_1, expand444, i, j);
//                }
//                else if (sum_error_2 <= sum_error_1 && sum_error_2 <= sum_error_3 && sum_error_2 <= sum_error_4 && sum_error_2 <= sum_error_5 && sum_error_2 <= sum_error_6 && sum_error_2 <= sum_error_7 && sum_error_2 <= sum_error_8) {
//                    right_count_2++;
//                    sum_error_2 += Error_Linear_Regression(shift_Y_2, expand444, i, j);
//                }
//                else if (sum_error_3 <= sum_error_2 && sum_error_3 <= sum_error_1 && sum_error_3 <= sum_error_4 && sum_error_3 <= sum_error_5 && sum_error_3 <= sum_error_6 && sum_error_3 <= sum_error_7 && sum_error_3 <= sum_error_8) {
//                    right_count_3++;
//                    sum_error_3 += Error_Linear_Regression(shift_Y_3, expand444, i, j);
//                }
//                else if (sum_error_4 <= sum_error_2 && sum_error_4 <= sum_error_3 && sum_error_4 <= sum_error_1 && sum_error_4 <= sum_error_5 && sum_error_4 <= sum_error_6 && sum_error_4 <= sum_error_7 && sum_error_4 <= sum_error_8) {
//                    right_count_4++;
//                    sum_error_4 += Error_Linear_Regression(shift_Y_4, expand444, i, j);
//                }
//                else if (sum_error_5 <= sum_error_1 && sum_error_5 <= sum_error_2 && sum_error_5 <= sum_error_3 && sum_error_5 <= sum_error_4 && sum_error_5 <= sum_error_6 && sum_error_5 <= sum_error_7 && sum_error_5 <= sum_error_8) {
//                    right_count_5++;
//                    sum_error_5 += Error_Linear_Regression(shift_Y_5, expand444, i, j);
//                }
//                else if (sum_error_6 <= sum_error_1 && sum_error_6 <= sum_error_2 && sum_error_6 <= sum_error_3 && sum_error_6 <= sum_error_4 && sum_error_6 <= sum_error_5 && sum_error_6 <= sum_error_7 && sum_error_6 <= sum_error_8) {
//                    right_count_6++;
//                    sum_error_6 += Error_Linear_Regression(shift_Y_6, expand444, i, j);
//                }
//                else if (sum_error_7 <= sum_error_1 && sum_error_7 <= sum_error_2 && sum_error_7 <= sum_error_3 && sum_error_7 <= sum_error_4 && sum_error_7 <= sum_error_5 && sum_error_7 <= sum_error_6 && sum_error_7 <= sum_error_8) {
//                    right_count_7++;
//                    sum_error_7 += Error_Linear_Regression(shift_Y_7, expand444, i, j);
//                }
//                else if (sum_error_8 <= sum_error_1 && sum_error_8 <= sum_error_2 && sum_error_8 <= sum_error_3 && sum_error_8 <= sum_error_4 && sum_error_8 <= sum_error_5 && sum_error_8 <= sum_error_6 && sum_error_8 <= sum_error_7) {
//                    right_count_8++;
//                    sum_error_8 += Error_Linear_Regression(shift_Y_8, expand444, i, j);
//                }
//
//
//                //error_sum = error_1 + error_2 + error_3 + error_4 + error_5 + error_7 + 0.01;
//
//                /*sum_error_1 += error_1 / error_sum;
//                sum_error_2 += error_2 / error_sum;
//                sum_error_3 += error_3 / error_sum;
//                sum_error_4 += error_4 / error_sum;
//                sum_error_5 += error_5 / error_sum;*/
//
//
//
//                //cout << "sum_error_1 : " << sum_error_1 << endl;
//                //cout << "sum_error_2 : " << sum_error_2 << endl;
//                //cout << "sum_error_3 : " << sum_error_3 << endl;
//                //cout << "sum_error_4 : " << sum_error_4 << endl;
//                //cout << "sum_error_5 : " << sum_error_5 << endl;
//
//                //system("Pause");
//
//
//                /*if (error_1 < error_2&&error_1 < error_3&&error_1 < error_4&&error_1 < error_5)
//                    right_count_1++;
//                else if (error_2 < error_1&&error_2 < error_3&&error_2 < error_4&&error_2 < error_5)
//                    right_count_2++;
//                else if (error_3 < error_2&&error_3 < error_1&&error_3 < error_4&&error_3 < error_5)
//                    right_count_3++;
//                else if (error_4 < error_2&&error_4 < error_3&&error_4 < error_1&&error_4 < error_5)
//                    right_count_4++;
//                else if (error_5 < error_1&&error_5 < error_2&&error_5 < error_3&&error_5 < error_4)
//                    right_count_5++;*/
//
//                    //i += 5;
//                    //j += 5;
//            }
//            //if (text_count > 10)
//            //	break;
//        }//end (i, j)
//
//    cout << "right_count_1 : " << right_count_1 << endl;
//    cout << "right_count_2 : " << right_count_2 << endl;
//    cout << "right_count_3 : " << right_count_3 << endl;
//    cout << "right_count_4 : " << right_count_4 << endl;
//    cout << "right_count_5 : " << right_count_5 << endl;
//    cout << "right_count_6 : " << right_count_6 << endl;
//    cout << "right_count_7 : " << right_count_7 << endl;
//    cout << "right_count_8 : " << right_count_8 << endl << endl;
//
//
//    cout << "sum_error_1 : " << sum_error_1 << endl;
//    cout << "sum_error_2 : " << sum_error_2 << endl;
//    cout << "sum_error_3 : " << sum_error_3 << endl;
//    cout << "sum_error_4 : " << sum_error_4 << endl;
//    cout << "sum_error_5 : " << sum_error_5 << endl;
//    cout << "sum_error_6 : " << sum_error_6 << endl;
//    cout << "sum_error_7 : " << sum_error_7 << endl;
//    cout << "sum_error_8 : " << sum_error_8 << endl << endl;
//
//    cout << "text_count : " << text_count << endl << endl;
//
//
//    double avg_error_1 = sum_error_1 / text_count;
//    double avg_error_2 = sum_error_2 / text_count;
//    double avg_error_3 = sum_error_3 / text_count;
//    double avg_error_4 = sum_error_4 / text_count;
//    double avg_error_5 = sum_error_5 / text_count;
//    double avg_error_6 = sum_error_6 / text_count;
//    double avg_error_7 = sum_error_7 / text_count;
//    double avg_error_8 = sum_error_8 / text_count;
//
//
//    /*cout << "avg_error_1 : " << avg_error_1 << endl;
//    cout << "avg_error_2 : " << avg_error_2 << endl;
//    cout << "avg_error_3 : " << avg_error_3 << endl;
//    cout << "avg_error_4 : " << avg_error_4 << endl;
//    cout << "avg_error_5 : " << avg_error_5 << endl;
//    cout << "avg_error_6 : " << avg_error_6 << endl;
//    cout << "avg_error_7 : " << avg_error_7 << endl;
//    cout << "avg_error_8 : " << avg_error_8 << endl;*/
//
//    system("Pause");
//
//
//    int right_count = max(right_count_1, max(right_count_2, max(right_count_3, max(right_count_4, max(right_count_5, max(right_count_6, max(right_count_7, right_count_8)))))));
//
//    cout << "text_count : " << text_count << endl;
//
//    if (right_count == right_count_1) {
//        method = 1;
//        cout << "right_count_1 : " << right_count_1 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_1 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_2) {
//        method = 2;
//        cout << "right_count_2 : " << right_count_2 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_2 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_3) {
//        method = 3;
//        cout << "right_count_3 : " << right_count_3 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_3 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_4) {
//        method = 4;
//        cout << "right_count_4 : " << right_count_4 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_4 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_5) {
//        method = 5;
//        cout << "right_count_5 : " << right_count_5 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_5 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_6) {
//        //		method = 5;
//        cout << "right_count_6 : " << right_count_6 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_6 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_7) {
//        method = 9;
//        cout << "right_count_7 : " << right_count_7 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_7 / (double)text_count) * 100 << "%" << endl;
//    }
//    if (right_count == right_count_8) {
//        method = 11;
//        cout << "right_count_8 : " << right_count_8 << endl;
//        cout << "correct persentage : " << (double)((double)right_count_8 / (double)text_count) * 100 << "%" << endl;
//    }
//    //system("Pause");
//
//    return method;
//}


//int judge(Mat& expand444, stringstream & path, stringstream & downsample, stringstream& QP){
//	int method = 1;
//
//	Mat shift_Y_1 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//A
//	Mat shift_Y_2 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//L
//	Mat shift_Y_3 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//R
//	Mat shift_Y_4 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//DIRECT
//	Mat shift_Y_5 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//MPEG-B
//	Mat shift_Y_6 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//IDID_NEDI
//	Mat shift_Y_7 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//CS_BILINEAR
//	Mat shift_Y_8 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//CS_BICUBIC
//
//	/*Mat Y= Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//	Y.at <double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//	}
//	Show(Y);*/
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_1.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0]
//				+ expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0]) / 4;
//		}
//	}
//
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_2.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0]) / 2;
//		}
//	}
//
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_3.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0]) / 2;
//		}
//	}
//
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_4.at <double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//		}
//	}
//
//	Mat coef = (Mat_<double>(1, 13) << 2, 0, -4, -3, 5, 19, 26, 19, 5, -3, -4, 0, 2);
//	//for (int i = 0; i < expand444.rows; i++)
//	//for (int j = 0; j < expand444.cols; j++)
//	//	shift_Y_5.at<double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//	int yy = 0;
//	int xx = 0;
//
//	for (int y = 0; y < expand444.rows; y += 2)
//	{
//		xx = 0;
//		for (int x = 0; x < expand444.cols; x += 2)
//		{
//			double DS_Y_Val = 0;
//			int index = 0;
//
//			for (int offset = -6; offset <= 6; offset++)
//			{
//				DS_Y_Val += expand444.at<Vec3d>(mir(y, expand444.rows), mir(x + offset, expand444.cols))[0] * coef.at<double>(0, index);
//				index++;
//			}
//			DS_Y_Val /= 64;
//
//			shift_Y_5.at<double>(y, x) = DS_Y_Val;
//
//			xx++;
//		}
//		yy++;
//	}
//	//Show(shift_Y_5);
//
//
//	//IDID_NEDI_to_Y(expand444, shift_Y_6, shift_Y_6, shift_Y_6);
//	//Show(shift_Y_6);
//
//	//======================================================================
//	int h = 16;
//	int w = 16;
//	Mat Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Mat imitated_Y = Mat::zeros(expand444.rows / 2, expand444.cols / 2, CV_64FC1);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		Y.at<double>(i, j) = expand444.at<Vec3d>(i, j)[0];
//	}
//	//Show(Y);
//	for (int i = 0; i < expand444.rows; i += 16)
//	for (int j = 0; j < expand444.cols; j += 16){
//		//-------------------------------------求H-----------------------------------------
//
//		Mat Y_small = Mat::zeros(h*w, 1, CV_64FC1);
//
//		int count = 0;
//		for (int k = i; k < i + h; k++)
//		for (int l = j; l < j + w; l++){
//			Y_small.at<double>(count++, 0) = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//		}
//
//		Mat HMatrix(h*w, (h*w) / 4, CV_64F, Scalar::all(0));
//		Decide_HMatrix(HMatrix, 16);
//
//		//-------------------------------------求邊--------------------------------------------
//		Mat Edge_Y(h*w, 1, CV_64F, Scalar::all(0));
//		IDID_edge(Y, Edge_Y, 16, i, j);
//		//-------------------------------------求最終-----------------------------------------
//		Mat Temp_Y;
//
//		Mat temp;
//		invert(Mat(HMatrix.t()*HMatrix), temp, 0);
//		Temp_Y = temp*HMatrix.t()*(Y_small - Edge_Y);
//
//		count = 0;
//		for (int k = i / 2; k < (i + h) / 2; k++)
//		for (int l = j / 2; l < (j + w) / 2; l++){
//			imitated_Y.at<double>(mir(k, expand444.rows / 2), mir(l, expand444.cols / 2)) = Temp_Y.at<double>(count++, 0);
//		}
//	}//end for(i, j)
//	//Show(imitated_Y);
//	//imwrite("21_imitated_Y.bmp", imitated_Y);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++)
//	if (i % 2 == 0 && j % 2 == 0)
//		shift_Y_7.at<double>(i, j) = imitated_Y.at<double>(i / 2, j / 2);
//	//Show(shift_Y_7);
//	//=========================================================================
//	vector<Mat> decode;
//	readYUV("YUV\\MIDID_BC\\" + path.str() + "_MIDID_BC_smallY.yuv", decode, expand444.rows, expand444.cols, 1);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++)
//	if (i % 2 == 0 && j % 2 == 0)
//		shift_Y_8.at<double>(i, j) = decode[0].at<Vec3b>(i / 2, j / 2)[1];
//
//	//=========================================================================
//	//=========================================================================
//	int text_count = 0;
//	int right_count_1 = 0;
//	int right_count_2 = 0;
//	int right_count_3 = 0;
//	int right_count_4 = 0;
//	int right_count_5 = 0;
//	int right_count_6 = 0;
//	int right_count_7 = 0;
//	int right_count_8 = 0;
//
//	double sum_error_1 = 0;
//	double sum_error_2 = 0;
//	double sum_error_3 = 0;
//	double sum_error_4 = 0;
//	double sum_error_5 = 0;
//	double sum_error_6 = 0;
//	double sum_error_7 = 0;
//	double sum_error_8 = 0;
//
//	int Ns;
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		//----------------------------block-wise content analysis------------------------------------
//		Ns = 0;
//		/*for (int k = i - 2; k <= i + 2; k++)
//		for (int l = j - 2; l <= j + 2; l++){
//		if (abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k - 1, expand444.rows), mir(l, expand444.cols))[0])>50 ||
//		abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k, expand444.rows), mir(l - 1, expand444.cols))[0])>50){
//		Ns++;
//		}
//		}*/
//		//---------------------------------------------------------------------------------------------------------
//
//
//		if (Ns >= 0){
//			/*Mat show = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//			for (int k = i - 2; k <= i + 2; k++)
//			for (int l = j - 2; l <= j + 2; l++){
//			show.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//			}*/
//			//Show(show);
//
//			text_count++;
//			//cout << "(" << i << " ," << j << ")" << endl;
//
//			//Show(shift_Y_1);
//			//Show(shift_Y_2);
//			//Show(shift_Y_3);
//			//Show(shift_Y_4);
//			//imwrite("shift_Y_1.bmp", shift_Y_1);
//
//			//double error_sum = 0;
//
//			if (sum_error_1 == sum_error_2&&sum_error_1 == sum_error_3&&sum_error_1 == sum_error_4&&sum_error_1 == sum_error_5&&sum_error_1 == sum_error_6&&sum_error_1 == sum_error_7&&sum_error_1 == sum_error_8){
//				right_count_1++;
//				right_count_2++;
//				right_count_3++;
//				right_count_4++;
//				right_count_5++;
//				right_count_6++;
//				right_count_7++;
//				right_count_8++;
//
//
//				sum_error_1 += Error_Linear_Regression(shift_Y_1, expand444, i, j);
//				sum_error_2 += Error_Linear_Regression(shift_Y_2, expand444, i, j);
//				sum_error_3 += Error_Linear_Regression(shift_Y_3, expand444, i, j);
//				sum_error_4 += Error_Linear_Regression(shift_Y_4, expand444, i, j);
//				sum_error_5 += Error_Linear_Regression(shift_Y_5, expand444, i, j);
//				sum_error_6 += Error_Linear_Regression(shift_Y_6, expand444, i, j);
//				sum_error_7 += Error_Linear_Regression(shift_Y_7, expand444, i, j);
//				sum_error_8 += Error_Linear_Regression(shift_Y_8, expand444, i, j);
//			}
//
//			else{
//				if (sum_error_1 <= sum_error_2&&sum_error_1 <= sum_error_3&&sum_error_1 <= sum_error_4&&sum_error_1 <= sum_error_5&&sum_error_1 <= sum_error_6&&sum_error_1 <= sum_error_7&&sum_error_1 <= sum_error_8){
//					right_count_1++;
//					sum_error_1 += Error_Linear_Regression(shift_Y_1, expand444, i, j);
//				}
//				if (sum_error_2 <= sum_error_1&&sum_error_2 <= sum_error_3&&sum_error_2 <= sum_error_4&&sum_error_2 <= sum_error_5&&sum_error_2 <= sum_error_6&&sum_error_2 <= sum_error_7&&sum_error_2 <= sum_error_8){
//					right_count_2++;
//					sum_error_2 += Error_Linear_Regression(shift_Y_2, expand444, i, j);
//				}
//				if (sum_error_3 <= sum_error_2&&sum_error_3 <= sum_error_1&&sum_error_3 <= sum_error_4&&sum_error_3 <= sum_error_5&&sum_error_3 <= sum_error_6&&sum_error_3 <= sum_error_7&&sum_error_3 <= sum_error_8){
//					right_count_3++;
//					sum_error_3 += Error_Linear_Regression(shift_Y_3, expand444, i, j);
//				}
//				if (sum_error_4 <= sum_error_2&&sum_error_4 <= sum_error_3&&sum_error_4 <= sum_error_1&&sum_error_4 <= sum_error_5&&sum_error_4 <= sum_error_6&&sum_error_4 <= sum_error_7&&sum_error_4 <= sum_error_8){
//					right_count_4++;
//					sum_error_4 += Error_Linear_Regression(shift_Y_4, expand444, i, j);
//				}
//				if (sum_error_5 <= sum_error_1&&sum_error_5 <= sum_error_2&&sum_error_5 <= sum_error_3&&sum_error_5 <= sum_error_4&&sum_error_5 <= sum_error_6&&sum_error_5 <= sum_error_7&&sum_error_5 <= sum_error_8){
//					right_count_5++;
//					sum_error_5 += Error_Linear_Regression(shift_Y_5, expand444, i, j);
//				}
//				if (sum_error_6 <= sum_error_1&&sum_error_6 <= sum_error_2&&sum_error_6 <= sum_error_3&&sum_error_6 <= sum_error_4&&sum_error_6 <= sum_error_5&&sum_error_6 <= sum_error_7&&sum_error_6 <= sum_error_8){
//					right_count_6++;
//					sum_error_6 += Error_Linear_Regression(shift_Y_6, expand444, i, j);
//				}
//				if (sum_error_7 <= sum_error_1&&sum_error_7 <= sum_error_2&&sum_error_7 <= sum_error_3&&sum_error_7 <= sum_error_4&&sum_error_7 <= sum_error_5&&sum_error_7 <= sum_error_6&&sum_error_7 <= sum_error_8){
//					right_count_7++;
//					sum_error_7 += Error_Linear_Regression(shift_Y_7, expand444, i, j);
//				}
//				if (sum_error_8 <= sum_error_1&&sum_error_8 <= sum_error_2&&sum_error_8 <= sum_error_3&&sum_error_8 <= sum_error_4&&sum_error_8 <= sum_error_5&&sum_error_8 <= sum_error_6&&sum_error_8 <= sum_error_7){
//					right_count_8++;
//					sum_error_8 += Error_Linear_Regression(shift_Y_8, expand444, i, j);
//				}
//			}
//
//			//error_sum = error_1 + error_2 + error_3 + error_4 + error_5 + error_7 + 0.01;
//
//			/*sum_error_1 += error_1 / error_sum;
//			sum_error_2 += error_2 / error_sum;
//			sum_error_3 += error_3 / error_sum;
//			sum_error_4 += error_4 / error_sum;
//			sum_error_5 += error_5 / error_sum;*/
//
//
//
//			//cout << "sum_error_1 : " << sum_error_1 << endl;
//			//cout << "sum_error_2 : " << sum_error_2 << endl;
//			//cout << "sum_error_3 : " << sum_error_3 << endl;
//			//cout << "sum_error_4 : " << sum_error_4 << endl;
//			//cout << "sum_error_5 : " << sum_error_5 << endl;
//
//			//system("Pause");
//
//
//			/*if (error_1 < error_2&&error_1 < error_3&&error_1 < error_4&&error_1 < error_5)
//			right_count_1++;
//			else if (error_2 < error_1&&error_2 < error_3&&error_2 < error_4&&error_2 < error_5)
//			right_count_2++;
//			else if (error_3 < error_2&&error_3 < error_1&&error_3 < error_4&&error_3 < error_5)
//			right_count_3++;
//			else if (error_4 < error_2&&error_4 < error_3&&error_4 < error_1&&error_4 < error_5)
//			right_count_4++;
//			else if (error_5 < error_1&&error_5 < error_2&&error_5 < error_3&&error_5 < error_4)
//			right_count_5++;*/
//
//			//i += 5;
//			//j += 5;
//		}
//		//if (text_count > 10)
//		//	break;
//	}//end (i, j)
//
//	cout << "right_count_1 : " << right_count_1 << endl;
//	cout << "right_count_2 : " << right_count_2 << endl;
//	cout << "right_count_3 : " << right_count_3 << endl;
//	cout << "right_count_4 : " << right_count_4 << endl;
//	cout << "right_count_5 : " << right_count_5 << endl;
//	cout << "right_count_6 : " << right_count_6 << endl;
//	cout << "right_count_7 : " << right_count_7 << endl;
//	cout << "right_count_8 : " << right_count_8 << endl << endl;
//
//
//	cout << "sum_error_1 : " << sum_error_1 << endl;
//	cout << "sum_error_2 : " << sum_error_2 << endl;
//	cout << "sum_error_3 : " << sum_error_3 << endl;
//	cout << "sum_error_4 : " << sum_error_4 << endl;
//	cout << "sum_error_5 : " << sum_error_5 << endl;
//	cout << "sum_error_6 : " << sum_error_6 << endl;
//	cout << "sum_error_7 : " << sum_error_7 << endl;
//	cout << "sum_error_8 : " << sum_error_8 << endl << endl;
//
//
//	double avg_error_1 = sum_error_1 / text_count;
//	double avg_error_2 = sum_error_2 / text_count;
//	double avg_error_3 = sum_error_3 / text_count;
//	double avg_error_4 = sum_error_4 / text_count;
//	double avg_error_5 = sum_error_5 / text_count;
//	double avg_error_6 = sum_error_6 / text_count;
//	double avg_error_7 = sum_error_7 / text_count;
//	double avg_error_8 = sum_error_8 / text_count;
//
//
//	/*cout << "avg_error_1 : " << avg_error_1 << endl;
//	cout << "avg_error_2 : " << avg_error_2 << endl;
//	cout << "avg_error_3 : " << avg_error_3 << endl;
//	cout << "avg_error_4 : " << avg_error_4 << endl;
//	cout << "avg_error_5 : " << avg_error_5 << endl;
//	cout << "avg_error_6 : " << avg_error_6 << endl;
//	cout << "avg_error_7 : " << avg_error_7 << endl;
//	cout << "avg_error_8 : " << avg_error_8 << endl;*/
//
//	system("Pause");
//
//
//	int right_count = max(right_count_1, max(right_count_2, max(right_count_3, max(right_count_4, max(right_count_5, max(right_count_6, max(right_count_7, right_count_8)))))));
//
//	cout << "text_count : " << text_count << endl;
//
//	if (right_count == right_count_1){
//		method = 1;
//		cout << "right_count_1 : " << right_count_1 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_1 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_2){
//		method = 2;
//		cout << "right_count_2 : " << right_count_2 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_2 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_3){
//		method = 3;
//		cout << "right_count_3 : " << right_count_3 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_3 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_4){
//		method = 4;
//		cout << "right_count_4 : " << right_count_4 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_4 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_5){
//		method = 5;
//		cout << "right_count_5 : " << right_count_5 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_5 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_6){
//		//		method = 5;
//		cout << "right_count_6 : " << right_count_6 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_6 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_7){
//		method = 9;
//		cout << "right_count_7 : " << right_count_7 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_7 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_8){
//		method = 11;
//		cout << "right_count_8 : " << right_count_8 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_8 / (double)text_count) * 100 << "%" << endl;
//	}
//	//system("Pause");
//
//	return method;
//}


//int judge(Mat& expand444){
//	int method = 1;
//
//	Mat shift_Y_1 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//A
//	Mat shift_Y_2 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//L
//	Mat shift_Y_3 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//R
//	Mat shift_Y_4 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//DIRECT
//	Mat shift_Y_5 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//MPEG-B
//
//	Mat shift_Y_7 = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);//CS_BILINEAR
//
//	/*Mat Y= Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//	Y.at <double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//	}
//	Show(Y);*/
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_1.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0]
//				+ expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0]) / 4;
//		}
//	}
//
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_2.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j, expand444.cols))[0]) / 2;
//		}
//	}
//
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_3.at <double>(i, j) = (expand444.at<Vec3d>(mir(i, expand444.rows), mir(j + 1, expand444.cols))[0] + expand444.at<Vec3d>(mir(i + 1, expand444.rows), mir(j + 1, expand444.cols))[0]) / 2;
//		}
//	}
//
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		if (mir(i, expand444.rows) % 2 == 0 && mir(j, expand444.cols) % 2 == 0){
//			shift_Y_4.at <double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//		}
//	}
//
//	Mat coef = (Mat_<double>(1, 13) << 2, 0, -4, -3, 5, 19, 26, 19, 5, -3, -4, 0, 2);
//	//for (int i = 0; i < expand444.rows; i++)
//	//for (int j = 0; j < expand444.cols; j++)
//	//	shift_Y_5.at<double>(i, j) = expand444.at<Vec3d>(mir(i, expand444.rows), mir(j, expand444.cols))[0];
//	int yy = 0;
//	int xx = 0;
//
//	for (int y = 0; y < expand444.rows; y += 2)
//	{
//		xx = 0;
//		for (int x = 0; x < expand444.cols; x += 2)
//		{
//			double DS_Y_Val = 0;
//			int index = 0;
//
//			for (int offset = -6; offset <= 6; offset++)
//			{
//				DS_Y_Val += expand444.at<Vec3d>(mir(y, expand444.rows), mir(x + offset, expand444.cols))[0] * coef.at<double>(0, index);
//				index++;
//			}
//			DS_Y_Val /= 64;
//
//			shift_Y_5.at<double>(y, x) = DS_Y_Val;
//
//			xx++;
//		}
//		yy++;
//	}
//	//Show(shift_Y_5);
//	//======================================================================
//	int h = 16;
//	int w = 16;
//	Mat Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//	Mat imitated_Y = Mat::zeros(expand444.rows / 2, expand444.cols / 2, CV_64FC1);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		Y.at<double>(i, j) = expand444.at<Vec3d>(i, j)[0];
//	}
//	//Show(Y);
//	for (int i = 0; i < expand444.rows; i += 16)
//	for (int j = 0; j < expand444.cols; j += 16){
//		//-------------------------------------求H-----------------------------------------
//
//		Mat Y_small = Mat::zeros(h*w, 1, CV_64FC1);
//
//		int count = 0;
//		for (int k = i; k < i + h; k++)
//		for (int l = j; l < j + w; l++){
//			Y_small.at<double>(count++, 0) = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//		}
//
//		Mat HMatrix(h*w, (h*w) / 4, CV_64F, Scalar::all(0));
//		Decide_HMatrix(HMatrix, 16);
//
//		//-------------------------------------求邊--------------------------------------------
//		Mat Edge_Y(h*w, 1, CV_64F, Scalar::all(0));
//		IDID_edge(Y, Edge_Y, 16, i, j);
//		//-------------------------------------求最終-----------------------------------------
//		Mat Temp_Y;
//
//		Mat temp;
//		invert(Mat(HMatrix.t()*HMatrix), temp, 0);
//		Temp_Y = temp*HMatrix.t()*(Y_small - Edge_Y);
//
//		count = 0;
//		for (int k = i / 2; k < (i + h) / 2; k++)
//		for (int l = j / 2; l < (j + w) / 2; l++){
//			imitated_Y.at<double>(mir(k, expand444.rows / 2), mir(l, expand444.cols / 2)) = Temp_Y.at<double>(count++, 0);
//		}
//	}//end for(i, j)
//	//Show(imitated_Y);
//	//imwrite("21_imitated_Y.bmp", imitated_Y);
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++)
//	if (i % 2 == 0 && j % 2 == 0)
//		shift_Y_7.at<double>(i, j) = imitated_Y.at<double>(i / 2, j / 2);
//	//	Show(shift_Y_7);
//
//
//	int text_count = 0;
//	int right_count_1 = 0;
//	int right_count_2 = 0;
//	int right_count_3 = 0;
//	int right_count_4 = 0;
//	int right_count_5 = 0;
//
//	int right_count_7 = 0;
//
//	double sum_error_1 = 0;
//	double sum_error_2 = 0;
//	double sum_error_3 = 0;
//	double sum_error_4 = 0;
//	double sum_error_5 = 0;
//
//	double sum_error_7 = 0;
//
//	int Ns;
//	for (int i = 0; i < expand444.rows; i++)
//	for (int j = 0; j < expand444.cols; j++){
//		//----------------------------block-wise content analysis------------------------------------
//		Ns = 0;
//		/*for (int k = i - 2; k <= i + 2; k++)
//		for (int l = j - 2; l <= j + 2; l++){
//		if (abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k - 1, expand444.rows), mir(l, expand444.cols))[0])>50 ||
//		abs(expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0] - expand444.at<Vec3d>(mir(k, expand444.rows), mir(l - 1, expand444.cols))[0])>50){
//		Ns++;
//		}
//		}*/
//		//---------------------------------------------------------------------------------------------------------
//
//
//		if (Ns >= 0){
//			/*Mat show = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//			for (int k = i - 2; k <= i + 2; k++)
//			for (int l = j - 2; l <= j + 2; l++){
//			show.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) = expand444.at<Vec3d>(mir(k, expand444.rows), mir(l, expand444.cols))[0];
//			}*/
//			//Show(show);
//
//			text_count++;
//			//cout << "(" << i << " ," << j << ")" << endl;
//
//			//Show(shift_Y_1);
//			//Show(shift_Y_2);
//			//Show(shift_Y_3);
//			//Show(shift_Y_4);
//			//imwrite("shift_Y_1.bmp", shift_Y_1);
//
//			double error_sum = 0;
//
//			double error_1 = Error_Linear_Regression(shift_Y_1, expand444, i, j);
//			double error_2 = Error_Linear_Regression(shift_Y_2, expand444, i, j);
//			double error_3 = Error_Linear_Regression(shift_Y_3, expand444, i, j);
//			double error_4 = Error_Linear_Regression(shift_Y_4, expand444, i, j);
//			double error_5 = Error_Linear_Regression(shift_Y_5, expand444, i, j);
//
//			double error_7 = Error_Linear_Regression(shift_Y_7, expand444, i, j);
//
//			error_sum = error_1 + error_2 + error_3 + error_4 + error_5 + error_7 + 0.01;
//
//			/*sum_error_1 += error_1 / error_sum;
//			sum_error_2 += error_2 / error_sum;
//			sum_error_3 += error_3 / error_sum;
//			sum_error_4 += error_4 / error_sum;
//			sum_error_5 += error_5 / error_sum;*/
//
//			sum_error_1 += error_1;
//			sum_error_2 += error_2;
//			sum_error_3 += error_3;
//			sum_error_4 += error_4;
//			sum_error_5 += error_5;
//
//			sum_error_7 += error_7;
//
//			//cout << "sum_error_1 : " << sum_error_1 << endl;
//			//cout << "sum_error_2 : " << sum_error_2 << endl;
//			//cout << "sum_error_3 : " << sum_error_3 << endl;
//			//cout << "sum_error_4 : " << sum_error_4 << endl;
//			//cout << "sum_error_5 : " << sum_error_5 << endl;
//
//			//system("Pause");
//
//			if (sum_error_1 <= sum_error_2&&sum_error_1 <= sum_error_3&&sum_error_1 <= sum_error_4&&sum_error_1 <= sum_error_5&&sum_error_1 <= sum_error_7)
//				right_count_1++;
//			if (sum_error_2 <= sum_error_1&&sum_error_2 <= sum_error_3&&sum_error_2 <= sum_error_4&&sum_error_2 <= sum_error_5&&sum_error_2 <= sum_error_7)
//				right_count_2++;
//			if (sum_error_3 <= sum_error_2&&sum_error_3 <= sum_error_1&&sum_error_3 <= sum_error_4&&sum_error_3 <= sum_error_5&&sum_error_3 <= sum_error_7)
//				right_count_3++;
//			if (sum_error_4 <= sum_error_2&&sum_error_4 <= sum_error_3&&sum_error_4 <= sum_error_1&&sum_error_4 <= sum_error_5&&sum_error_4 <= sum_error_7)
//				right_count_4++;
//			if (sum_error_5 <= sum_error_1&&sum_error_5 <= sum_error_2&&sum_error_5 <= sum_error_3&&sum_error_5 <= sum_error_4&&sum_error_5 <= sum_error_7)
//				right_count_5++;
//
//			if (sum_error_7 <= sum_error_1&&sum_error_7 <= sum_error_2&&sum_error_7 <= sum_error_3&&sum_error_7 <= sum_error_4&&sum_error_7 <= sum_error_5)
//				right_count_7++;
//
//			/*if (error_1 < error_2&&error_1 < error_3&&error_1 < error_4&&error_1 < error_5)
//			right_count_1++;
//			else if (error_2 < error_1&&error_2 < error_3&&error_2 < error_4&&error_2 < error_5)
//			right_count_2++;
//			else if (error_3 < error_2&&error_3 < error_1&&error_3 < error_4&&error_3 < error_5)
//			right_count_3++;
//			else if (error_4 < error_2&&error_4 < error_3&&error_4 < error_1&&error_4 < error_5)
//			right_count_4++;
//			else if (error_5 < error_1&&error_5 < error_2&&error_5 < error_3&&error_5 < error_4)
//			right_count_5++;*/
//
//			//i += 5;
//			//j += 5;
//		}
//		//if (text_count > 10)
//		//	break;
//	}//end (i, j)
//
//	cout << "right_count_1 : " << right_count_1 << endl;
//	cout << "right_count_2 : " << right_count_2 << endl;
//	cout << "right_count_3 : " << right_count_3 << endl;
//	cout << "right_count_4 : " << right_count_4 << endl;
//	cout << "right_count_5 : " << right_count_5 << endl;
//
//	cout << "right_count_7 : " << right_count_7 << endl << endl;
//
//
//	cout << "sum_error_1 : " << sum_error_1 << endl;
//	cout << "sum_error_2 : " << sum_error_2 << endl;
//	cout << "sum_error_3 : " << sum_error_3 << endl;
//	cout << "sum_error_4 : " << sum_error_4 << endl;
//	cout << "sum_error_5 : " << sum_error_5 << endl;
//
//	cout << "sum_error_7 : " << sum_error_7 << endl << endl;
//
//
//	double avg_error_1 = sum_error_1 / text_count;
//	double avg_error_2 = sum_error_2 / text_count;
//	double avg_error_3 = sum_error_3 / text_count;
//	double avg_error_4 = sum_error_4 / text_count;
//	double avg_error_5 = sum_error_5 / text_count;
//
//	double avg_error_7 = sum_error_7 / text_count;
//
//
//	cout << "avg_error_1 : " << avg_error_1 << endl;
//	cout << "avg_error_2 : " << avg_error_2 << endl;
//	cout << "avg_error_3 : " << avg_error_3 << endl;
//	cout << "avg_error_4 : " << avg_error_4 << endl;
//	cout << "avg_error_5 : " << avg_error_5 << endl;
//
//	cout << "avg_error_7 : " << avg_error_7 << endl;
//
//	system("Pause");
//
//
//	int right_count = max(right_count_1, max(right_count_2, max(right_count_3, max(right_count_4, max(right_count_5, right_count_7)))));
//
//	cout << "text_count : " << text_count << endl;
//
//	if (right_count == right_count_1){
//		method = 1;
//		cout << "right_count_1 : " << right_count_1 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_1 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_2){
//		method = 2;
//		cout << "right_count_2 : " << right_count_2 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_2 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_3){
//		method = 3;
//		cout << "right_count_3 : " << right_count_3 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_3 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_4){
//		method = 4;
//		cout << "right_count_4 : " << right_count_4 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_4 / (double)text_count) * 100 << "%" << endl;
//	}
//	if (right_count == right_count_5){
//		method = 5;
//		cout << "right_count_5 : " << right_count_5 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_5 / (double)text_count) * 100 << "%" << endl;
//	}
//
//	if (right_count == right_count_7){
//		method = 9;
//		cout << "right_count_7 : " << right_count_7 << endl;
//		cout << "correct persentage : " << (double)((double)right_count_7 / (double)text_count) * 100 << "%" << endl;
//	}
//	system("Pause");
//
//	return method;
//}

double Calculate_SSIM(const Mat& ori_plane, const Mat& prop_plane)
{
    //int height = ori_plane.rows;
    //int width = ori_plane.cols;
    //int imgSize = height * width;
    ////double C1 = 6.5025;		// (0.01 * 255) * (0.01 * 255)
    ////double C2 = 58.5225;	// (0.03 * 255) * (0.03 * 255)
    ////double C3 = C2 / 2;
    //double C1 = 1, C2 = 1, C3 = 1;
    //double ori_prop_SSIM;

    //// *** Luminance *** //
    //long int ori_sigma = 0, prop_sigma = 0;
    //double ori_mean = 0, prop_mean = 0;
    //double ori_prop_luminance = 0;

    //for (int y = 0; y < height; y++)
    //{
    //    for (int x = 0; x < width; x++)
    //    {
    //        ori_sigma += ori_plane.at<uchar>(y, x);
    //        prop_sigma += prop_plane.at<uchar>(y, x);
    //    }
    //}

    //ori_mean = (double)ori_sigma / (double)imgSize;
    //prop_mean = (double)prop_sigma / (double)imgSize;

    //// --- calculate luminance --- //
    //ori_prop_luminance = ((2 * ori_mean * prop_mean) + C1) / ((pow(ori_mean, 2.0) + pow(prop_mean, 2.0)) + C1);

    //////////////////////////////////////////////////////////////////////////////////////

    //// *** Contrast *** //
    //double ori_variance = 0, prop_variance = 0;
    //double ori_sDeviation = 0, prop_sDeviation = 0;
    //double ori_prop_contrast = 0;
    //double ori_diffPow_sum = 0, prop_diffPow_sum = 0;
    //double ori_prop_diffMul_sum = 0;

    //for (int y = 0; y < height; y++)
    //{
    //    for (int x = 0; x < width; x++)
    //    {
    //        double ori_diff = ori_mean - (double)ori_plane.at<uchar>(y, x);
    //        double prop_diff = prop_mean - (double)prop_plane.at<uchar>(y, x);

    //        ori_diffPow_sum += pow(ori_diff, 2.0);
    //        prop_diffPow_sum += pow(prop_diff, 2.0);

    //        ori_prop_diffMul_sum += ori_diff * prop_diff;
    //    }
    //}

    //// --- variance --- //
    //ori_variance = ori_diffPow_sum / imgSize;
    //prop_variance = prop_diffPow_sum / imgSize;

    //// --- standard deviation --- //
    //ori_sDeviation = sqrt(ori_variance);
    //prop_sDeviation = sqrt(prop_variance);

    //// --- calculate contrast --- //
    //ori_prop_contrast = ((2 * ori_sDeviation * prop_sDeviation) + C2) / (pow(ori_sDeviation, 2.0) + pow(prop_sDeviation, 2.0) + C2);

    //////////////////////////////////////////////////////////////////////////////////////

    //// *** Structure *** //
    //double ori_prop_covariance = 0;
    //double ori_prop_structure = 0;

    //// --- calculate convariance --- //
    //ori_prop_covariance = ori_prop_diffMul_sum / imgSize;

    //// --- calculate structure --- //
    //ori_prop_structure = (ori_prop_covariance + C3) / (ori_sDeviation * prop_sDeviation + C3);

    //////////////////////////////////////////////////////////////////////////////////////

    //ori_prop_SSIM = ori_prop_luminance * ori_prop_contrast * ori_prop_structure;

    ////cout << ori_prop_SSIM << "; " << ori_prop_luminance << ", " << ori_prop_contrast << ", " << ori_prop_structure << endl;
    ////imshow("ori", ori_plane);
    ////imshow("prop", prop_plane);
    ////waitKey(0);

    //return ori_prop_SSIM;
	const double C1 = 6.5025, C2 = 58.5225;
	int d = CV_64F;
	Mat I1, I2;
	ori_plane.convertTo(I1, d);
	prop_plane.convertTo(I2, d);
	Mat I1_2 = I1.mul(I1);
	Mat I2_2 = I2.mul(I2);
	Mat I1_I2 = I1.mul(I2);
	Mat mu1, mu2;
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);
	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);
	Mat sigma1_2, sigam2_2, sigam12;
	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigam2_2, Size(11, 11), 1.5);
	sigam2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigam12, Size(11, 11), 1.5);
	sigam12 -= mu1_mu2;
	Mat t1, t2, t3;
	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigam12 + C2;
	t3 = t1.mul(t2);

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigam2_2 + C2;
	t1 = t1.mul(t2);

	Mat ssim_map;
	divide(t3, t1, ssim_map);
	Scalar mssim = mean(ssim_map);

	double ssim = (mssim.val[0] + mssim.val[1] + mssim.val[2]) / 3;
	return ssim;
}

double var(double *s, int N)
{
    double mean = 0, v = 0, temp = 0;

    for (int i = 0; i < N; i++)
    {
        mean += s[i];
        temp += s[i] * s[i];
    }

    mean /= (double)N;
    return (temp - (N*mean*mean)) / (double)(N - 1);
}
//void NEDI_Step1(double **U, double **D, int Width, int Height)
//{
//	//BI(U, D, Width, Height);
//
//	int T = 8, th = 8;
//	MatrixXd C(4 * (wsize + 1)*(wsize + 1), 4);
//	MatrixXd r(4 * (wsize + 1)*(wsize + 1), 1);
//	MatrixXd a(4, 1);
//	double *s = (double*)malloc(4 * sizeof(double));
//
//	//for (int i = T; i < (Width / 2) - T - 1; i++)
//	for (int i = 0; i < Width / 2; i++)
//	{
//		//for (int j = T; j < (Height / 2) - T - 1; j++)
//		for (int j = 0; j < Height / 2; j++)
//		{
//			int count = 0;
//			for (int x = -wsize; x <= wsize + 1; x++)
//			{
//				for (int y = -wsize; y <= wsize + 1; y++)
//				{
//					C(count, 0) = round(D[mirror1(Width / 2, i + x - 1)][mirror1(Height / 2, j + y - 1)]);
//					C(count, 1) = round(D[mirror1(Width / 2, i + x + 1)][mirror1(Height / 2, j + y - 1)]);
//					C(count, 2) = round(D[mirror1(Width / 2, i + x - 1)][mirror1(Height / 2, j + y + 1)]);
//					C(count, 3) = round(D[mirror1(Width / 2, i + x + 1)][mirror1(Height / 2, j + y + 1)]);
//					r(count, 0) = round(D[mirror1(Width / 2, i + x)][mirror1(Height / 2, j + y)]);
//					count++;
//				}
//			}
//
//			s[0] = round(D[mirror1(Width / 2, i)][mirror1(Height / 2, j)]);
//			s[1] = round(D[mirror1(Width / 2, i + 1)][mirror1(Height / 2, j)]);
//			s[2] = round(D[mirror1(Width / 2, i)][mirror1(Height / 2, j + 1)]);
//			s[3] = round(D[mirror1(Width / 2, i + 1)][mirror1(Height / 2, j + 1)]);
//
//			if ((C.transpose()*C).determinant() == 0 || var(s, 4) <th)
//			{
//				for (int k = 0; k<4; k++)
//					a(k, 0) = 0.25;
//			}
//			else
//				a = (C.transpose()*C).inverse()*(C.transpose()*r);
//
//			U[mirror1(Width, 2 * i + 1)][mirror1(Height, 2 * j + 1)] = round(s[0] * a(0, 0) + s[1] * a(1, 0) + s[2] * a(2, 0) + s[3] * a(3, 0));
//
//			if (U[mirror1(Width, 2 * i + 1)][mirror1(Height, 2 * j + 1)] < 0 || U[mirror1(Width, 2 * i + 1)][mirror1(Height, 2 * j + 1)] > 255)
//				U[mirror1(Width, 2 * i + 1)][mirror1(Height, 2 * j + 1)] = round(s[0] * 0.25 + s[1] * 0.25 + s[2] * 0.25 + s[3] * 0.25);
//		}
//	}
//
//	//free
//	free(s);
//	C.resize(0, 0);
//	r.resize(0, 0);
//	a.resize(0, 0);
//}
//void Decide_NEDI_Coef_Step1(int height, int width, int leftTop_hr_y, int leftTop_hr_x, double** US_plane, double** DS_plane, Mat& HMatrix, Mat& E)
//{
//	int th = 8;
//	MatrixXd C(4 * (wsize + 1)*(wsize + 1), 4);
//	MatrixXd r(4 * (wsize + 1)*(wsize + 1), 1);
//	MatrixXd a(4, 1);
//	double *s = (double*)malloc(4 * sizeof(double));
//
//	int LR_height = height / 2;
//	int LR_width = width / 2;
//	int leftTop_lr_y = leftTop_hr_y / 2;
//	int leftTop_lr_x = leftTop_hr_x / 2;
//	int y = 1, x = 1;
//	int HMatrix_y;
//
//	for (int xx = 0; xx < DS_B_sideLen; xx++)
//	{
//		y = 1;
//		for (int yy = 0; yy < DS_B_sideLen; yy++)
//		{
//			int lr_y = leftTop_lr_y + yy;
//			int lr_x = leftTop_lr_x + xx;
//			int hr_y = leftTop_hr_y + y;
//			int hr_x = leftTop_hr_x + x;
//
//			int count = 0;
//			for (int offset_x = -wsize; offset_x <= wsize + 1; offset_x++)
//			{
//				for (int offset_y = -wsize; offset_y <= wsize + 1; offset_y++)
//				{
//					C(count, 0) = DS_plane[mirror1(LR_width, lr_x + offset_x - 1)][mirror1(LR_height, lr_y + offset_y - 1)];
//					C(count, 1) = DS_plane[mirror1(LR_width, lr_x + offset_x + 1)][mirror1(LR_height, lr_y + offset_y - 1)];
//					C(count, 2) = DS_plane[mirror1(LR_width, lr_x + offset_x - 1)][mirror1(LR_height, lr_y + offset_y + 1)];
//					C(count, 3) = DS_plane[mirror1(LR_width, lr_x + offset_x + 1)][mirror1(LR_height, lr_y + offset_y + 1)];
//					r(count, 0) = DS_plane[mirror1(LR_width, lr_x + offset_x)][mirror1(LR_height, lr_y + offset_y)];
//					count++;
//				}
//			}
//
//			s[0] = DS_plane[mirror1(LR_width, lr_x)][mirror1(LR_height, lr_y)];
//			s[1] = DS_plane[mirror1(LR_width, lr_x + 1)][mirror1(LR_height, lr_y)];
//			s[2] = DS_plane[mirror1(LR_width, lr_x)][mirror1(LR_height, lr_y + 1)];
//			s[3] = DS_plane[mirror1(LR_width, lr_x + 1)][mirror1(LR_height, lr_y + 1)];
//
//			if ((C.transpose()*C).determinant() == 0 || var(s, 4) < th)
//			{
//				for (int k = 0; k < 4; k++)
//					a(k, 0) = 0.25;
//			}
//			else
//				a = (C.transpose()*C).inverse()*(C.transpose()*r);
//
//			double tempVal = s[0] * a(0, 0) + s[1] * a(1, 0) + s[2] * a(2, 0) + s[3] * a(3, 0);
//			if (tempVal < 0 || tempVal > 255)
//			{
//				for (int k = 0; k < 4; k++)
//					a(k, 0) = 0.25;
//			}
//
//			HMatrix.at<double>(mirror1(B_EleNum, 2 * yy * B_sideLen + 2 * xx), mirror1(DS_B_EleNum, yy * DS_B_sideLen + xx)) = 1;
//
//			HMatrix_y = y * B_sideLen + x;
//			HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, yy * DS_B_sideLen + xx)) = a(0, 0);
//
//			// 右下
//			if ((xx + 1) % DS_B_sideLen == 0 && (yy + 1) % DS_B_sideLen == 0)
//				E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) = a(1, 0) * s[1] + a(2, 0) * s[2] + a(3, 0) * s[3];
//			// 右邊界
//			else if ((xx + 1) % DS_B_sideLen == 0)
//			{
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, (yy + 1) * DS_B_sideLen + xx)) = a(2, 0);
//				E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) = a(1, 0) * s[1] + a(3, 0) * s[3];
//			}
//			// 下邊界
//			else if ((yy + 1) % DS_B_sideLen == 0)
//			{
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, yy * DS_B_sideLen + (xx + 1))) = a(1, 0);
//				E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) = a(2, 0) * s[2] + a(3, 0) * s[3];
//			}
//			else
//			{
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, yy * DS_B_sideLen + (xx + 1))) = a(1, 0);
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, (yy + 1) * DS_B_sideLen + xx)) = a(2, 0);
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, (yy + 1) * DS_B_sideLen + (xx + 1))) = a(3, 0);
//			}
//
//			y += 2;
//		}
//		x += 2;
//	}
//
//	//free
//	free(s);
//	C.resize(0, 0);
//	r.resize(0, 0);
//	a.resize(0, 0);
//}
//void Decide_NEDI_Coef_Step2(int HR_height, int HR_width, int leftTop_hr_y, int leftTop_hr_x, double **US_plane, Mat& HMatrix, Mat& E)
//{
//	int th = 8;
//	MatrixXd C((wsize + 1)*(wsize + 1), 4);
//	MatrixXd r((wsize + 1)*(wsize + 1), 1);
//	MatrixXd a(4, 1);
//	double *s = (double*)malloc(4 * sizeof(double));
//
//	int HMatrix_y;
//	int yy = 0, xx = 0;
//
//	// Cross-marked Pixels Interpolation
//	for (int x = 1; x < B_sideLen; x += 2)
//	{
//		yy = 0;
//		for (int y = 0; y < B_sideLen; y += 2)
//		{
//			int hr_y = leftTop_hr_y + y;
//			int hr_x = leftTop_hr_x + x;
//
//			int count = 0;
//			for (int offset_x = -wsize; offset_x <= wsize; offset_x++)
//			{
//				for (int offset_y = -wsize; offset_y <= wsize; offset_y++)
//				{
//					if (abs(offset_x) + abs(offset_y) <= wsize && (abs(offset_x) + abs(offset_y)) % 2 == 1)
//					{
//						/* 整數 */
//						C(count, 0) = round(US_plane[mirror1(HR_width, hr_x + offset_x)][mirror1(HR_height, hr_y + offset_y - 2)]);
//						C(count, 1) = round(US_plane[mirror1(HR_width, hr_x + offset_x)][mirror1(HR_height, hr_y + offset_y + 2)]);
//						C(count, 2) = round(US_plane[mirror1(HR_width, hr_x + offset_x - 2)][mirror1(HR_height, hr_y + offset_y)]);
//						C(count, 3) = round(US_plane[mirror1(HR_width, hr_x + offset_x + 2)][mirror1(HR_height, hr_y + offset_y)]);
//						r(count, 0) = round(US_plane[mirror1(HR_width, hr_x + offset_x)][mirror1(HR_height, hr_y + offset_y)]);
//
//						count++;
//					}
//				}
//			}
//
//			/* 整數 */
//			s[0] = round(US_plane[mirror1(HR_width, hr_x)][mirror1(HR_height, hr_y - 1)]);
//			s[1] = round(US_plane[mirror1(HR_width, hr_x)][mirror1(HR_height, hr_y + 1)]);
//			s[2] = round(US_plane[mirror1(HR_width, hr_x - 1)][mirror1(HR_height, hr_y)]);
//			s[3] = round(US_plane[mirror1(HR_width, hr_x + 1)][mirror1(HR_height, hr_y)]);
//
//			if ((C.transpose()*C).determinant() == 0 || var(s, 4) < th)
//			{
//				for (int k = 0; k < 4; k++)
//					a(k, 0) = 0.25;
//			}
//			else
//				a = (C.transpose()*C).inverse()*(C.transpose()*r);
//
//			double tempVal = s[0] * a(0, 0) + s[1] * a(1, 0) + s[2] * a(2, 0) + s[3] * a(3, 0);
//			if (tempVal < 0 || tempVal > 255)
//			{
//				for (int k = 0; k < 4; k++)
//					a(k, 0) = 0.25;
//			}
//
//			HMatrix_y = y * B_sideLen + x;
//			HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, yy * DS_B_sideLen + xx)) = a(2, 0);
//
//			// Circle-marked(Groundtruth) 超過區塊圈選範圍
//			// 右邊界
//			if ((x + 1) % B_sideLen == 0)
//				E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) = a(3, 0) * s[3];
//			else
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, yy * DS_B_sideLen + (xx + 1))) = a(3, 0);
//
//			// Square-marked Pixels Position
//			E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) += a(0, 0) * s[0] + a(1, 0) * s[1];
//
//			yy++;
//		}
//		xx++;
//	}
//
//	yy = 0;
//	xx = 0;
//	// Triangle-marked Piexls Interpolation
//	for (int x = 0; x < B_sideLen; x += 2)
//	{
//		yy = 0;
//		for (int y = 1; y < B_sideLen; y += 2)
//		{
//			int hr_y = leftTop_hr_y + y;
//			int hr_x = leftTop_hr_x + x;
//
//			int count = 0;
//			for (int offset_x = -wsize; offset_x <= wsize; offset_x++)
//			{
//				for (int offset_y = -wsize; offset_y <= wsize; offset_y++)
//				{
//					if (abs(offset_x) + abs(offset_y) <= wsize && (abs(offset_x) + abs(offset_y)) % 2 == 1)
//					{
//						/* 整數 */
//						C(count, 0) = round(US_plane[mirror1(HR_width, hr_x + offset_x)][mirror1(HR_height, hr_y + offset_y - 2)]);
//						C(count, 1) = round(US_plane[mirror1(HR_width, hr_x + offset_x)][mirror1(HR_height, hr_y + offset_y + 2)]);
//						C(count, 2) = round(US_plane[mirror1(HR_width, hr_x + offset_x - 2)][mirror1(HR_height, hr_y + offset_y)]);
//						C(count, 3) = round(US_plane[mirror1(HR_width, hr_x + offset_x + 2)][mirror1(HR_height, hr_y + offset_y)]);
//						r(count, 0) = round(US_plane[mirror1(HR_width, hr_x + offset_x)][mirror1(HR_height, hr_y + offset_y)]);
//
//						count++;
//					}
//				}
//			}
//
//			/* 整數 */
//			s[0] = round(US_plane[mirror1(HR_width, hr_x)][mirror1(HR_height, hr_y - 1)]);
//			s[1] = round(US_plane[mirror1(HR_width, hr_x)][mirror1(HR_height, hr_y + 1)]);
//			s[2] = round(US_plane[mirror1(HR_width, hr_x - 1)][mirror1(HR_height, hr_y)]);
//			s[3] = round(US_plane[mirror1(HR_width, hr_x + 1)][mirror1(HR_height, hr_y)]);
//
//			if ((C.transpose()*C).determinant() == 0 || var(s, 4) < th)
//			{
//				for (int k = 0; k < 4; k++)
//					a(k, 0) = 0.25;
//			}
//			else
//				a = (C.transpose()*C).inverse()*(C.transpose()*r);
//
//			double tempVal = s[0] * a(0, 0) + s[1] * a(1, 0) + s[2] * a(2, 0) + s[3] * a(3, 0);
//			if (tempVal < 0 || tempVal > 255)
//			{
//				for (int k = 0; k < 4; k++)
//					a(k, 0) = 0.25;
//			}
//
//			HMatrix_y = y * B_sideLen + x;
//			HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, yy * DS_B_sideLen + xx)) = a(0, 0);
//
//			// Circle-marked(Groundtruth) 超過區塊圈選範圍
//			// 下邊界
//			if ((y + 1) % B_sideLen == 0)
//				E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) = a(1, 0) * s[1];
//			else
//				HMatrix.at<double>(mirror1(B_EleNum, HMatrix_y), mirror1(DS_B_EleNum, (yy + 1) * DS_B_sideLen + xx)) = a(1, 0);
//
//			// Square-marked Pixels Position
//			E.at<double>(mirror1(B_EleNum, HMatrix_y), 0) += a(2, 0) * s[2] + a(3, 0) * s[3];
//
//			yy++;
//		}
//		xx++;
//	}
//
//	//free
//	free(s);
//	C.resize(0, 0);
//	r.resize(0, 0);
//	a.resize(0, 0);
//}
//void DownSample_by_IDID_NEDI(int height, int width, int leftTop_hr_y, int leftTop_hr_x, const Mat& plane, const Mat& HMatrix, const Mat& E, Mat& DS_plane)
//{
//	Mat block;
//	Mat DS_block;
//	int LR_height = height / 2;
//	int LR_width = width / 2;
//	int leftTop_lr_y = leftTop_hr_y / 2;
//	int leftTop_lr_x = leftTop_hr_x / 2;
//
//	block = Mat::zeros(B_EleNum, 1, CV_64F);
//	int index = 0;
//	for (int y = 0; y < B_sideLen; y++)
//	for (int x = 0; x < B_sideLen; x++)
//	{
//		block.at<double>(index, 0) = (double)plane.at<uchar>(mirror1(height, leftTop_hr_y + y), mirror1(width, leftTop_hr_x + x));
//		index++;
//	}
//
//	DS_block = Mat::zeros(DS_B_EleNum, 1, CV_64F);
//	DS_block = ((HMatrix.t() * HMatrix).inv()) * HMatrix.t() * (block - E);
//
//	for (int i = 0; i < DS_B_EleNum; i++)
//	{
//		if (DS_block.at<double>(i, 0) < 0)
//			DS_block.at<double>(i, 0) = 0;
//		else if (DS_block.at<double>(i, 0) > 255)
//			DS_block.at<double>(i, 0) = 255;
//	}
//
//	index = 0;
//	for (int j = 0; j < DS_B_sideLen; j++)
//	for (int i = 0; i < DS_B_sideLen; i++)
//	{
//		DS_plane.at<uchar>(mirror1(LR_height, leftTop_lr_y + j), mirror1(LR_width, leftTop_lr_x + i)) = (uchar)round(DS_block.at<double>(index, 0));
//		index++;
//	}
//}
//void Decide_IDID_NEDI_HMatrix(int height, int width, int leftTop_hr_y, int leftTop_hr_x, double** US_plane, double** DS_plane, Mat& HMatrix, Mat& E)
//{
//	//Square-marked Piexls Interpolation
//	Decide_NEDI_Coef_Step1(height, width, leftTop_hr_y, leftTop_hr_x, US_plane, DS_plane, HMatrix, E);
//
//	//Cross-marked and Triangle-marked Piexls Interpolation
//	Decide_NEDI_Coef_Step2(height, width, leftTop_hr_y, leftTop_hr_x, US_plane, HMatrix, E);
//}
//void IDID_NEDI_to_Y(Mat& YUV, Mat& Y, Mat& U, Mat &V){
//	{
//		Mat DS_Y_plane;
//		//Mat	DS_U_plane;
//		//Mat	DS_V_plane;
//		Mat HMatrix;
//		Mat E;
//
//		int height = YUV.rows;
//		int width = YUV.cols;
//
//		vector<Mat> vec_yuvPlane = { Mat::zeros(height, width, CV_64FC1), Mat::zeros(height, width, CV_64FC1), Mat::zeros(height, width, CV_64FC1) };
//		split(YUV, vec_yuvPlane);
//
//		vec_yuvPlane[0].convertTo(vec_yuvPlane[0], CV_8UC1);
//		//vec_yuvPlane[1].convertTo(vec_yuvPlane[1], CV_8UC1);
//		//vec_yuvPlane[2].convertTo(vec_yuvPlane[2], CV_8UC1);
//
//		/* Decide NEDI Interpolation Weight */
//		// Allocate Memory
//		double** dArr_DS_Y_plane_by_Direct = new double*[width / 2];
//		//double** dArr_DS_U_plane_by_Direct = new double*[width / 2];
//		//double** dArr_DS_V_plane_by_Direct = new double*[width / 2];
//		for (int w = 0; w < width / 2; w++)
//		{
//			dArr_DS_Y_plane_by_Direct[w] = new double[height / 2];
//			//dArr_DS_U_plane_by_Direct[w] = new double[height / 2];
//			//dArr_DS_V_plane_by_Direct[w] = new double[height / 2];
//		}
//
//		// 用DIRECT抽樣 並將抽樣值放在小張
//		int yy = 0, xx = 0;
//		for (int y = 0; y < height; y += 2)
//		{
//			xx = 0;
//			for (int x = 0; x < width; x += 2)
//			{
//				dArr_DS_Y_plane_by_Direct[xx][yy] = vec_yuvPlane[0].at<uchar>(y, x);
//				//dArr_DS_U_plane_by_Direct[xx][yy] = vec_yuvPlane[1].at<uchar>(y, x);
//				//dArr_DS_V_plane_by_Direct[xx][yy] = vec_yuvPlane[2].at<uchar>(y, x);
//				xx++;
//			}
//			yy++;
//		}
//
//		// 用DIRECT抽樣 並將抽樣值放在大張左上角
//		double** dArr_US_Y_plane = new double*[width];
//		//double** dArr_US_U_plane = new double*[width];
//		//double** dArr_US_V_plane = new double*[width];
//		for (int w = 0; w < width; w++)
//		{
//			dArr_US_Y_plane[w] = new double[height];
//			//dArr_US_U_plane[w] = new double[height];
//			//dArr_US_V_plane[w] = new double[height];
//		}
//		for (int y = 0; y < height; y += 2)
//		for (int x = 0; x < width; x += 2)
//		{
//			dArr_US_Y_plane[x][y] = vec_yuvPlane[0].at<uchar>(y, x);
//			//dArr_US_U_plane[x][y] = vec_yuvPlane[1].at<uchar>(y, x);
//			//dArr_US_V_plane[x][y] = vec_yuvPlane[2].at<uchar>(y, x);
//		}
//
//		// 先將方塊位置(y%2==1 && x%2==1)補滿 方便係數決定
//		NEDI_Step1(dArr_US_Y_plane, dArr_DS_Y_plane_by_Direct, width, height);
//		//NEDI_Step1(dArr_US_U_plane, dArr_DS_U_plane_by_Direct, width, height);
//		//NEDI_Step1(dArr_US_V_plane, dArr_DS_V_plane_by_Direct, width, height);
//
//		/* Decide IDID_NEDI HMatrix(Use to DownSample) and
//		Use IDID_NEDI to DownSample */
//		DS_Y_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);
//		//DS_U_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);
//		//DS_V_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);
//		for (int y = 0; y < height; y += B_sideLen)
//		{
//			for (int x = 0; x < width; x += B_sideLen)
//			{
//				/* Y Plane */
//				HMatrix = Mat::zeros(B_EleNum, DS_B_EleNum, CV_64F);
//				E = Mat::zeros(B_EleNum, 1, CV_64F);
//				Decide_IDID_NEDI_HMatrix(height, width, y, x, dArr_US_Y_plane, dArr_DS_Y_plane_by_Direct, HMatrix, E);
//				DownSample_by_IDID_NEDI(height, width, y, x, vec_yuvPlane[0], HMatrix, E, DS_Y_plane);
//
//				/* U Plane */
//				//HMatrix = Mat::zeros(B_EleNum, DS_B_EleNum, CV_64F);
//				//E = Mat::zeros(B_EleNum, 1, CV_64F);
//				//Decide_IDID_NEDI_HMatrix(height, width, y, x, dArr_US_U_plane, dArr_DS_U_plane_by_Direct, HMatrix, E);
//				//DownSample_by_IDID_NEDI(height, width, y, x, vec_yuvPlane[1], HMatrix, E, DS_U_plane);
//
//				/* V plane */
//				//HMatrix = Mat::zeros(B_EleNum, DS_B_EleNum, CV_64F);
//				//E = Mat::zeros(B_EleNum, 1, CV_64F);
//				//Decide_IDID_NEDI_HMatrix(height, width, y, x, dArr_US_V_plane, dArr_DS_V_plane_by_Direct, HMatrix, E);
//				//DownSample_by_IDID_NEDI(height, width, y, x, vec_yuvPlane[2], HMatrix, E, DS_V_plane);
//
//				/*if (y == 228 && x == 442)
//				{
//				Mat expand_Y(height, width, CV_8UC1, Scalar::all(0));
//				Mat expand_U(height, width, CV_8UC1, Scalar::all(0));
//				Mat expand_V(height, width, CV_8UC1, Scalar::all(0));
//
//				for (int yy = 0; yy < height / 2; yy++)
//				for (int xx = 0; xx < width / 2; xx++)
//				{
//				expand_Y.at<uchar>(yy * 2, xx * 2) = DS_Y_plane.at<uchar>(yy, xx);
//				expand_U.at<uchar>(yy * 2, xx * 2) = DS_U_plane.at<uchar>(yy, xx);
//				expand_V.at<uchar>(yy * 2, xx * 2) = DS_V_plane.at<uchar>(yy, xx);
//				}
//
//				cout << "Y Plane: " << endl;
//				for (int offset_y = -2; offset_y <= 2; offset_y++)
//				{
//				for (int offset_x = -2; offset_x <= 2; offset_x++)
//				{
//				int curr_y = y + offset_y;
//				int curr_x = x + offset_x;
//
//				if (curr_y % 2 == 0 && curr_x % 2 == 0)
//				cout << expand_Y.at<uchar>(y + offset_y, x + offset_x) << "\t";
//				else
//				cout << "X" << "\t";
//				}
//				cout << endl;
//				}
//
//				cout << "U Plane: " << endl;
//				for (int offset_y = -2; offset_y <= 2; offset_y++)
//				{
//				for (int offset_x = -2; offset_x <= 2; offset_x++)
//				{
//				int curr_y = y + offset_y;
//				int curr_x = x + offset_x;
//
//				if (curr_y % 2 == 0 && curr_x % 2 == 0)
//				cout << expand_U.at<uchar>(y + offset_y, x + offset_x) << "\t";
//				else
//				cout << "X" << "\t";
//				}
//				cout << endl;
//				}
//
//				cout << "V Plane: " << endl;
//				for (int offset_y = -2; offset_y <= 2; offset_y++)
//				{
//				for (int offset_x = -2; offset_x <= 2; offset_x++)
//				{
//				int curr_y = y + offset_y;
//				int curr_x = x + offset_x;
//
//				if (curr_y % 2 == 0 && curr_x % 2 == 0)
//				cout << expand_V.at<uchar>(y + offset_y, x + offset_x) << "\t";
//				else
//				cout << "X" << "\t";
//				}
//				cout << endl;
//				}
//				}*/
//			}
//		}
//
//		//Mat shift_Y_6 = Mat::zeros(height, width, CV_64FC1);//IDID_NEDI
//		for (int i = 0; i <height; i++)
//		for (int j = 0; j < width; j++)
//		if (i % 2 == 0 && j % 2 == 0)
//			Y.at<double>(i, j) = DS_Y_plane.at<uchar>(i / 2, j / 2);
//
//		//vec_yuvPlane[0].copyTo(Y);
//		//DS_U_plane.copyTo(U);
//		//DS_V_plane.copyTo(V);
//
//		/* Release Memory */
//		for (int w = 0; w < width / 2; w++)
//		{
//			delete[] dArr_DS_Y_plane_by_Direct[w];
//			//delete[] dArr_DS_U_plane_by_Direct[w];
//			//delete[] dArr_DS_V_plane_by_Direct[w];
//		}
//		delete[] dArr_DS_Y_plane_by_Direct;
//		//delete[] dArr_DS_U_plane_by_Direct;
//		//delete[] dArr_DS_V_plane_by_Direct;
//
//		for (int w = 0; w < width; w++)
//		{
//			delete[] dArr_US_Y_plane[w];
//			//delete[] dArr_US_U_plane[w];
//			//delete[] dArr_US_V_plane[w];
//		}
//		delete[] dArr_US_Y_plane;
//		//delete[] dArr_US_U_plane;
//		//delete[] dArr_US_V_plane;
//	}
//}

void ylRegression(Mat& expand444, Mat& shift_Y, Mat& shift_U, Mat *mat_a, Mat *mat_b, int h, int w) {
    double th = 0.00001 * 255 * 255;

    *mat_a = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
    *mat_b = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);

    Mat real_Y = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);;
    for (int i = 0; i < expand444.rows; i++)
        for (int j = 0; j < expand444.cols; j++)
            real_Y.at<double>(i, j) = expand444.at<Vec3d>(i, j)[0];

    /*Mat shift_U = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);;
    for (int i = 0; i < expand444.rows; i++)
    for (int j = 0; j < expand444.cols; j++)
    shift_U.at<double>(i, j) = expand444.at<Vec3d>(i, j)[1];*/

    Mat mask_of_sub = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
    for (int i = 0; i < expand444.rows; i++)
        for (int j = 0; j < expand444.cols; j++)
            if (i % 2 == 0 && j % 2 == 0)
                mask_of_sub.at<double>(i, j) = 1;

    Mat count = boxfilter(mask_of_sub, h, w);
    Mat one = Mat::ones(expand444.rows, expand444.cols, CV_64FC1);
    Mat count2 = boxfilter(one, h, w); //每個local patch的大小

    Mat YxM = shift_Y.mul(mask_of_sub);
    Mat mean_Y = boxfilter(YxM, h, w);
    divide(mean_Y, count, mean_Y);

    Mat mean_U = boxfilter(shift_U, h, w);
    divide(mean_U, count, mean_U);

    Mat YxU = shift_Y.mul(shift_U);
    Mat mean_YU = boxfilter(YxU, h, w); //每個local patch中 被採樣Guide x BeGuide點的平均
    divide(mean_YU, count, mean_YU);

    Mat cov_YU = mean_YU - mean_Y.mul(mean_U);

    Mat YxYxM = shift_Y.mul(shift_Y.mul(mask_of_sub));
    Mat mean_YY = boxfilter(YxYxM, h, w); //每個local patch中 被採樣Guide的平方 的平均
    divide(mean_YY, count, mean_YY);

    Mat var_Y = mean_YY - mean_Y.mul(mean_Y); //變異數 = 被採樣Guide的平方 的平均 - 每點被採樣Guide點平均的平方
    for (int i = 0; i < var_Y.rows; i++)
        for (int j = 0; j < var_Y.cols; j++)
            if (var_Y.at<double>(i, j) < th)
                var_Y.at<double>(i, j) = th;

    /// linear coefficients
    Mat a, b;
    divide(cov_YU, (var_Y + 0.001), a);
    b = mean_U - a.mul(mean_Y);

    Mat mean_a = boxfilter(a, h, w);  //每個local patch中 平均的a
    divide(mean_a, count2, mean_a);

    Mat mean_b = boxfilter(b, h, w); //每個local patch中 平均的b
    divide(mean_b, count2, mean_b);

    /// output
    Mat Ubar = a.mul(real_Y) + b;
    //Mat Ubar = mean_a.mul(real_Y) + mean_b;

    //Show(Ubar);
    *mat_a = a;
    *mat_b = b;

    //return Ubar;
}
Mat ylCDD(Mat& expand444, Mat& shift_Y, Mat& shift_U, Mat mat_a, Mat mat_b, int h, int w) {

    Mat return_CDD = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
    for (int i = 0; i < expand444.rows; i++)
        for (int j = 0; j < expand444.cols; j++) {

            double CDD = 0;
            for (int k = i - h; k <= i + h; k++)//5x5
                for (int l = j - w; l <= j + w; l++) {
                    if (mir(k, expand444.rows) % 2 == 0 && mir(l, expand444.cols) % 2 == 0) {

                        CDD += (pow(mat_a.at<double>(mir(k, expand444.rows), mir(l, expand444.cols))*shift_Y.at<double>(mir(k, expand444.rows), mir(l, expand444.cols))
                            + mat_b.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)) - shift_U.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)), 2)
                            / (pow(mat_a.at<double>(mir(k, expand444.rows), mir(l, expand444.cols)), 2) + 1));
                    }
                }//5x5

            return_CDD.at<double>(i, j) = CDD;

        }
    return return_CDD;
}
void fusion_error(Mat& Ubar, Mat& Ubc, Mat& Ucc, Mat& Ubc_e, Mat& Ucc_e, int block) {
    Ubar = Mat::zeros(Ubc.rows, Ubc.cols, CV_64FC1);
    Mat output = Mat::zeros(Ubc.rows, Ubc.cols, CV_8UC1);
    int h = block;
    int w = block;
    int count = 0;
    int Y_count = 0, U_count = 0, YU_count = 0, x_count = 0;
    double bc_e = 0, cc_e = 0;
    double weight_bc = 0, weight_cc = 0;
    double e = 0.001;

    for (int iForIndexH = 0; iForIndexH < Ubc.rows; iForIndexH += h) {
        for (int iForIndexW = 0; iForIndexW < Ubc.cols; iForIndexW += w) {
            count = 0;
            Y_count = 0; U_count = 0; YU_count = 0; x_count = 0;
            bc_e = 0;
            cc_e = 0;

            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {
                    if (k % 2 == 0 && l % 2 == 0) {
                        bc_e += abs(Ubc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        cc_e += abs(Ucc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        count++;
                    }
                }
            //cout << YU_count << "\t" << Y_count << "\t" << U_count << "\t" << x_count << "\t" << count << endl;
            bc_e /= count;
            cc_e /= count;

            weight_bc = (1.0 / (bc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)));
            weight_cc = (1.0 / (cc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)));


            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {
                    //int k = iForIndexH;
                    //int l = iForIndexW;
                    Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = weight_bc * Ubc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) + weight_cc * Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));


                    //
                    if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) < 16)
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 16;
                    if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) > 240)
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 240;

                }

        }
    }

}

void fusion_error(Mat& Ubar, Mat& Ubc, Mat& Ucc, Mat& Ubc_e, Mat& Ucc_e, Mat& Udl, Mat& Udl_e, int block, Mat& BandL) {
    Ubar = Mat::zeros(Ubc.rows, Ubc.cols, CV_64FC1);

    Mat output = Mat::zeros(Ubc.rows, Ubc.cols, CV_8UC1);
    int h = block;
    int w = block;
    int count = 0;
    int Y_count = 0, U_count = 0, YU_count = 0, x_count = 0;
    double bc_e = 0, cc_e = 0, dl_e = 0;

    double weight_bc = 0, weight_cc = 0, weight_dl = 0;
    double e = 0.001;

    for (int iForIndexH = 0; iForIndexH < Ubc.rows; iForIndexH += h) {
        for (int iForIndexW = 0; iForIndexW < Ubc.cols; iForIndexW += w) {
            count = 0;
            Y_count = 0; U_count = 0; YU_count = 0; x_count = 0;
            bc_e = 0;
            cc_e = 0;
            dl_e = 0;

            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {
                    if (k % 2 == 0 && l % 2 == 0) {
                        bc_e += abs(Ubc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        cc_e += abs(Ucc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        dl_e += abs(Udl_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        count++;
                    }
                }
            //cout << YU_count << "\t" << Y_count << "\t" << U_count << "\t" << x_count << "\t" << count << endl;
            bc_e /= count;
            cc_e /= count;
            dl_e /= count;

            weight_bc = (1.0 / (bc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (dl_e + e)));
            weight_cc = (1.0 / (cc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (dl_e + e)));
            weight_dl = (1.0 / (dl_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (dl_e + e)));

            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {

                    //int k = iForIndexH;
                    //int l = iForIndexW;
                    if (BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[0] != 255)
                    {//fusion
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) =
                            weight_bc * Ubc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) +
                            weight_cc * Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) +
                            weight_dl * Udl.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));


                        if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) < 0)
                            Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 0;
                        if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) > 255)
                            Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 255;

                        if (weight_bc > weight_cc && weight_bc > weight_dl)
                        {//blue : bic
                            BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[0] = 254;
                            //Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = Ubc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                        }

                        else if (weight_cc > weight_bc && weight_cc > weight_dl)
                        {//green : cc
                            BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[1] = 254;
                            //Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                        }
                        else
                        {//red :dl
                            BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[2] = 254;
                            //Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = Udl.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                        }
                    }
                    else
                    {//copy
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) =
                            Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                    }

                }
        }
    }
}

void fusion_error(Mat& Ubar, Mat& Ubc, Mat& Ucc, Mat& Uul, Mat& Ubc_e, Mat& Ucc_e, Mat& Uul_e, int block) {
    Ubar = Mat::zeros(Ubc.rows, Ubc.cols, CV_64FC1);
    Mat output = Mat::zeros(Ubc.rows, Ubc.cols, CV_8UC1);
    int h = block;
    int w = block;
    int count = 0;
    int Y_count = 0, U_count = 0, YU_count = 0, x_count = 0;
    double bc_e = 0, cc_e = 0, ul_e = 0;
    double weight_bc = 0, weight_cc = 0, weight_ul = 0;
    double e = 0.001;

    for (int iForIndexH = 0; iForIndexH < Ubc.rows; iForIndexH += h) {
        for (int iForIndexW = 0; iForIndexW < Ubc.cols; iForIndexW += w) {
            count = 0;
            Y_count = 0; U_count = 0; YU_count = 0; x_count = 0;
            bc_e = 0;
            cc_e = 0;
            ul_e = 0;

            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {
                    if (k % 2 == 0 && l % 2 == 0) {
                        bc_e += abs(Ubc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        cc_e += abs(Ucc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        ul_e += abs(Uul_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));

                        count++;
                    }
                }
            //cout << YU_count << "\t" << Y_count << "\t" << U_count << "\t" << x_count << "\t" << count << endl;
            bc_e /= count;
            cc_e /= count;
            ul_e /= count;

            weight_bc = (1.0 / (bc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (ul_e + e)));
            weight_cc = (1.0 / (cc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (ul_e + e)));
            weight_ul = (1.0 / (ul_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (ul_e + e)));


            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {
                    //int k = iForIndexH;
                    //int l = iForIndexW;
                    Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) =
                        weight_bc * Ubc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols))
                        + weight_cc * Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols))
                        + weight_ul * Uul.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));


                    //
                    if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) < 16)
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 16;
                    if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) > 240)
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 240;

                }

        }
    }

}

void fusion_error(Mat& Ubar, Mat& Ubc, Mat& Ucc, Mat& Ubc_e, Mat& Ucc_e, int block, Mat& BandL) {
    Ubar = Mat::zeros(Ubc.rows, Ubc.cols, CV_64FC1);

    Mat output = Mat::zeros(Ubc.rows, Ubc.cols, CV_8UC1);
    int h = block;
    int w = block;
    int count = 0;
    int Y_count = 0, U_count = 0, YU_count = 0, x_count = 0;
    double bc_e = 0, cc_e = 0, dl_e = 0;

    double weight_bc = 0, weight_cc = 0, weight_dl = 0;
    double e = 0.001;

    for (int iForIndexH = 0; iForIndexH < Ubc.rows; iForIndexH += h) {
        for (int iForIndexW = 0; iForIndexW < Ubc.cols; iForIndexW += w) {
            count = 0;
            Y_count = 0; U_count = 0; YU_count = 0; x_count = 0;
            bc_e = 0;
            cc_e = 0;
            dl_e = 0;

            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {
                    if (k % 2 == 0 && l % 2 == 0) {
                        bc_e += abs(Ubc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        cc_e += abs(Ucc_e.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)));
                        count++;
                    }
                }
            //cout << YU_count << "\t" << Y_count << "\t" << U_count << "\t" << x_count << "\t" << count << endl;
            bc_e /= count;
            cc_e /= count;

            weight_bc = (1.0 / (bc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (dl_e + e)));
            weight_cc = (1.0 / (cc_e + e)) / ((1.0 / (bc_e + e)) + (1.0 / (cc_e + e)) + (1.0 / (dl_e + e)));

            for (int k = iForIndexH; k < iForIndexH + h; k++)
                for (int l = iForIndexW; l < iForIndexW + w; l++) {

                    //int k = iForIndexH;
                    //int l = iForIndexW;
                    if (BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[0] != 255)
                    {//fusion
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) =
                            weight_bc * Ubc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) +
                            weight_cc * Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));


                        //if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) < 0)
                        //    Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 0;
                        //if (Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) > 255)
                        //    Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = 255;

                        //if (weight_bc > weight_cc && weight_bc > weight_dl)
                        //{//blue : bic
                        //    BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[0] = 254;
                        //    //Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = Ubc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                        //}

                        //else if (weight_cc > weight_bc && weight_cc > weight_dl)
                        //{//green : cc
                        //    BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[1] = 254;
                        //    //Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                        //}
                        //else
                        //{//red :dl
                        //    BandL.at<Vec3b>(mir(k, Ubc.rows), mir(l, Ubc.cols))[2] = 254;
                        //    //Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) = Udl.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                        //}
                    }
                    else
                    {//copy
                        Ubar.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols)) =
                            Ucc.at<double>(mir(k, Ubc.rows), mir(l, Ubc.cols));
                    }

                }
        }
    }
}


void bicubic_all(Mat& expand444, Mat& Ufinal, Mat& Vfinal) {

    //補groundtruth

    //先做U平面
    double fWieghtSum = 0.0;
    for (int iForIndexH = 0; iForIndexH < expand444.rows; iForIndexH++) {
        for (int iForIndexW = 0; iForIndexW < expand444.cols; iForIndexW++) {

            int iOffset_Y;
            int iOffset_X;
            double fSumTemp = 0.0;
            fWieghtSum = 0.0;

            if (iForIndexH % 2 == 0 && iForIndexW % 2 == 0) {
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

                        fSumTemp += Ufinal.at<double>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols)) * BC_funtion(fDY)*BC_funtion(fDX);
                        fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX)); //權重加總值，正規化
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
    }
    //	Show(Ufinal);
    //	imwrite("U(0,0).bmp", Ufinal);

    //再做V平面
    fWieghtSum = 0.0;
    for (int iForIndexH = 0; iForIndexH < expand444.rows; iForIndexH++) {
        for (int iForIndexW = 0; iForIndexW < expand444.cols; iForIndexW++) {

            int iOffset_Y;
            int iOffset_X;
            double fSumTemp = 0.0;
            fWieghtSum = 0.0;

            if (iForIndexH % 2 == 0 && iForIndexW % 2 == 0) {
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

                        fSumTemp += Vfinal.at<double>(mir(iOffset_Y, expand444.rows), mir(iOffset_X, expand444.cols)) * BC_funtion(fDY)*BC_funtion(fDX);
                        fWieghtSum += (BC_funtion(fDY) * BC_funtion(fDX)); //權重加總值，正規化
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
}
//void error_map(Mat& expand444, Mat& U, Mat& V, Mat& U_e, Mat& V_e, int method) {
//    U_e = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//    V_e = Mat::zeros(expand444.rows, expand444.cols, CV_64FC1);
//
//    Mat U_ds, V_ds;
//    ds_method(U, U_ds, method);
//    ds_method(V, V_ds, method);
//
//    for (int iForIndexH = 0; iForIndexH < expand444.rows; iForIndexH++)
//        for (int iForIndexW = 0; iForIndexW < expand444.cols; iForIndexW++) {
//            if (iForIndexH % 2 == 0 && iForIndexW % 2 == 0)
//            {
//                U_e.at<double>(iForIndexH, iForIndexW) = expand444.at<Vec3d>(iForIndexH, iForIndexW)[1] - U_ds.at<double>(iForIndexH / 2, iForIndexW / 2);
//                V_e.at<double>(iForIndexH, iForIndexW) = expand444.at<Vec3d>(iForIndexH, iForIndexW)[2] - V_ds.at<double>(iForIndexH / 2, iForIndexW / 2);
//            }
//        }
//}

void nothing() {}



Mat _imread(String path, int type)
{    /**
    * Read an image
    *
    *  This function reads image with user specified type and avoids reading image error.
    *
    * @param[in] path: Image to load
    * @param[in] type: Image's type
    */

    Mat temp;
    temp = imread(path, type);
    if (temp.empty()) {
        cerr << "in \"_imread\" funciotn" << endl;
        cerr << "Reading image fail\n" << "path is : " << path << endl;
        system("pause");
        exit(-1);
    }
    return temp;
}
Mat _imread(String path)
{    /**
    * Read an image
    *
    *  This function avoids reading image error.
    *
    * @param[in] path: Image to load
    */

    Mat img = imread(path);
    if (img.empty()) {
        cerr << "in \"_imread\" funciotn" << endl;
        cerr << "Reading image fail\n" << "path is : " << path << endl;
        system("pause");
        exit(-1);
    }
    return img;
}


void showUchar(Mat img) {
	img.convertTo(img, CV_8UC1);
	imshow("", img);
	waitKey();
}
double Lumamean(Mat&Y, Mat&oriY) {
	int height = Y.rows;
	int width = Y.cols;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			Y.at<double>(i, k) = int(Y.at<double>(i, k) + 0.5);		
		}
	}
	
	double c = 0;
	double a = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			a = (abs((int)oriY.at<double>(i, j) - (int)Y.at<double>(i, j)));
			//cout << a << " ";
			c += a;
		}
	}
	c/= (height*width);
	return c;
}

//double luma_mean_loss(Mat ori_Y, Mat rec_Y)
//{
//	int height = ori_Y.rows;
//	int width = ori_Y.cols;
//	double sum_ori = 0;
//	double sum_rec = 0;
//	double mean_ori, mean_rec;
//	for (int i = 0; i < height; i++) {
//		for (int j = 0; j < width; j++) {
//			sum_ori += ori_Y.at<double>(i, j);
//			sum_rec += rec_Y.at<double>(i, j);
//		}
//	}
//	mean_ori = sum_ori / (height*width);
//	mean_rec = sum_rec / (height*width);
//	//cout << mean_ori << endl;
//	//cout << mean_rec << endl;
//	return abs(mean_ori - mean_rec);
//}