#include "Subsample.h"
void Read_YUV420(int height, int width, fstream& yuv_reader, Mat& Y_plane, Mat& sub_U_plane, Mat& sub_V_plane)
{
	int y, x;
	// Y
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_reader.read((char*)Y_plane.data + y * Y_plane.step1(0) + x * Y_plane.step1(1) + 0, 1);
	}

	// U
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_reader.read((char*)sub_U_plane.data + y * sub_U_plane.step1(0) + x * sub_U_plane.step1(1) + 0, 1);
	}

	// V
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_reader.read((char*)sub_V_plane.data + y * sub_V_plane.step1(0) + x * sub_V_plane.step1(1) + 0, 1);
	}
}

void Read_YUV444(int height, int width, fstream& yuv_reader, Mat& Y_plane, Mat& sub_U_plane, Mat& sub_V_plane)
{
	int y, x;
	// Y
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_reader.read((char*)Y_plane.data + y * Y_plane.step1(0) + x * Y_plane.step1(1) + 0, 1);
	}

	// U
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_reader.read((char*)sub_U_plane.data + y * sub_U_plane.step1(0) + x * sub_U_plane.step1(1) + 0, 1);
	}

	// V
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_reader.read((char*)sub_V_plane.data + y * sub_V_plane.step1(0) + x * sub_V_plane.step1(1) + 0, 1);
	}
}

void Write_YUV420(int height, int width, fstream& yuv_writer, const Mat& Y_plane, const Mat& sub_U_plane, const Mat& sub_V_plane)
{
	
	int y, x;
		
	// Y
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_writer.write((char*)Y_plane.data + y * Y_plane.step1(0) + x * Y_plane.step1(1) + 0, 1);
	}

	// U
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_writer.write((char*)sub_U_plane.data + y * sub_U_plane.step1(0) + x * sub_U_plane.step1(1) + 0, 1);
	}

	// V
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_writer.write((char*)sub_V_plane.data + y * sub_V_plane.step1(0) + x * sub_V_plane.step1(1) + 0, 1);
	}
	//cout << "end" << endl;
}