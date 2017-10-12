
// DetectIterface.h : header file
//

#pragma once
#include "Config.h"

typedef struct SRect{
	SRect(Point start, Point end){
		 point_start = start;
		 point_end = end;
		 width = end.x - start.x;
		 height = end.y - start.y;
	}
	SRect(){
		width = 0;
		height = 0;
	}
	Point point_start;
	Point point_end;
	int width;
	int height;
}SRect;

class DetectIterface
{
public:
	DetectIterface();
	~DetectIterface();
private:
	string s_dir_path;
	Mat src_root;
	//vector<vector<Point>>hull;
	vector<SRect> rect_contour;
	vector<String> list_win;
public:
	////Method
	void setListIcon(vector<SRect> list_contur);
	vector<SRect> getListIcon(void);
	void setImagePath(string path);
	String getImagePath(void);
	void onLoad(void);
	Mat getCvImageRoot(void);
	Mat filterBinary(Mat src_input, double thresh = 128, double maxvalue = 255, int type = 0);
	vector<vector<Point>> findContour(Mat src_binary);
	vector<SRect> DrawRectContour(vector<vector<Point>>contour);
	bool showWindow(Mat src, string win_title);
	bool saveImage(string name_file, int index);
};
