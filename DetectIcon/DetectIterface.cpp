#include "stdafx.h"
#include "DetectIterface.h"

//----------------------------------------------------------------------------------------
DetectIterface::DetectIterface()
{
	s_dir_path.clear();
	rect_contour.clear();
}

//----------------------------------------------------------------------------------------
DetectIterface::~DetectIterface()
{
}

//----------------------------------------------------------------------------------------
void DetectIterface::setImagePath(string path)
{
	s_dir_path = path;
}

//----------------------------------------------------------------------------------------
String DetectIterface::getImagePath(void)
{
	return s_dir_path;
}

//----------------------------------------------------------------------------------------
void DetectIterface::onLoad(void)
{
	if (!s_dir_path.empty()){
		src_root = imread(s_dir_path);
		Mat sr_binary = filterBinary(src_root, 220, 255, 1);
		vector<vector<Point>> hull = findContour(sr_binary);
		rect_contour = DrawRectContour(hull);
		//for (int i = 0; i < rect_contour.size(); i++){
		//	Rect  rc;
		//	rc = cv::Rect(Point(rect_contour[i].point_start.x, rect_contour[i].point_start.y),
		//					Point(rect_contour[i].point_end.x, rect_contour[i].point_end.y));
		//	Mat dst = src_root(rc);
		//	char buff[20];
		//	sprintf_s(buff, "image line %d.jpg", i);
		//	showWindow(dst, buff);
		//}
	}
}

//----------------------------------------------------------------------------------------
Mat DetectIterface::getCvImageRoot(void)
{
	return src_root;
}

//----------------------------------------------------------------------------------------
Mat DetectIterface::filterBinary(Mat src_input, double thresh, double maxvalue, int type)
{
	Mat src_binary = src_input.clone();
	//chuyen sang anh xam
	cvtColor(src_binary, src_binary, COLOR_BGR2GRAY);
	//imshow("anh nhi phan", src_binary);
	//Chuyen sang anh nhi phan lay nguong
	if (type == 1){
		threshold(src_binary, src_binary, thresh, maxvalue, CV_THRESH_BINARY_INV);
	}
	else{
		threshold(src_binary, src_binary, thresh, maxvalue, CV_THRESH_BINARY);
	}
	//imshow("anh nhi phan", src_binary);
	return src_binary;
}

//----------------------------------------------------------------------------------------
vector<vector<Point>> DetectIterface::findContour(Mat src_binary)
{
	Mat src_contour = Mat::zeros(src_binary.size(), CV_8UC3);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RNG rng;
	//Tim vien bao quanh
	findContours(src_binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
		Point(0, 0));
	vector<vector<Point>> hull_tmp(contours.size());
	//Noi vien thanh nhung doan thang
	for (int i = 0; i < contours.size(); i++){
		convexHull(Mat(contours[i]), hull_tmp[i], false);
	}
	//Ve vien
	for (int i = 0; i < hull_tmp.size(); i++){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(src_contour, hull_tmp, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}
	//imshow("xem anh vien", src_contour);
	return hull_tmp;
}

//----------------------------------------------------------------------------------------
vector<SRect> DetectIterface::DrawRectContour(vector<vector<Point>>contour)
{
	vector<SRect> rect_icon;
	int x_min, y_min, x_max, y_max, i_width, i_height;
	for (int count = 0; count < contour.size(); count++){
		for (int i = 0; i < contour[count].size(); i++){
			if (i == 0){
				x_min = x_max = contour[count][i].x;
				y_min = y_max = contour[count][i].y;
			}
			else{
				if (contour[count][i].x > x_max){
					x_max = contour[count][i].x;
				}
				if (contour[count][i].x < x_min){
					x_min = contour[count][i].x;
				}
				if (contour[count][i].y > y_max){
					y_max = contour[count][i].y;
				}
				if (contour[count][i].y < y_min){
					y_min = contour[count][i].y;
				}
			}
		}
		i_width = x_max - x_min;
		i_height = y_max - y_min;
		if (i_width > 9 && i_height > 9){	//lay nhung icon co kich thuoc width va height lon hon 9
			rect_icon.push_back(SRect(Point(x_min, y_min), Point(x_max, y_max)));
			//ShowImage(x_min, y_min, x_max, y_max);
		}
	}
	//Loai bo nhung vien nam phia trong vien khac
	for (int i = 0; i < rect_icon.size() - 1; i++){
		for (int j = i + 1; j < rect_icon.size(); j++){
			if ((rect_icon[i].point_start.x <= rect_icon[j].point_start.x) &&
				(rect_icon[i].point_start.y <= rect_icon[j].point_start.y) &&
				(rect_icon[i].point_end.x >= rect_icon[j].point_end.x) &&
				(rect_icon[i].point_end.y >= rect_icon[j].point_end.y))
			{
				rect_icon.erase(rect_icon.begin() + j);
				j--;
			}
		}
	}
	return rect_icon;
}

//----------------------------------------------------------------------------------------
bool DetectIterface::showWindow(Mat src, string win_title)
{
	for (int i = 0; i < list_win.size(); i++){
		if (win_title == list_win[i])
			return 0;
	}
	imshow(win_title, src);
	return 1;
}

//----------------------------------------------------------------------------------------
void DetectIterface::setListIcon(vector<SRect> list_contour)
{
	rect_contour = list_contour;
}
//----------------------------------------------------------------------------------------
vector<SRect> DetectIterface::getListIcon(void)
{
	return rect_contour;
}

//----------------------------------------------------------------------------------------
bool DetectIterface::saveImage(string name_file, int index)
{
	if (index < 0 || index >= rect_contour.size())
		return 0;
	Rect  rc;
	rc = cv::Rect(Point(rect_contour[index].point_start.x, rect_contour[index].point_start.y),
		Point(rect_contour[index].point_end.x, rect_contour[index].point_end.y));
	Mat dst = src_root(rc);
	imwrite(name_file, dst);
	return 1;
}
