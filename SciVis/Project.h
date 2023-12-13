#include "icVector.H"
#include "Polyline.h"

bool fieldHeightOn = true;
bool contHeightOn = false;
bool rainbowMapOn = false;
bool greyscaleMapOn = false;
bool bicolorMapOn = false;

enum colorMaps {rainbow,bicolor,greyscale};
bool* colorMapBools[] = {&rainbowMapOn,&bicolorMapOn,&greyscaleMapOn};

void drawContourLine(double v, icVector3 rgb,int saddle_index);
inline icVector3 rainbowColorFromScalar(double scalar,double max, double min);

// void contHeight();
void fieldHeight();
void drawAllLines(int num_conts);

void rainbowMap();
void greyscaleMap();
void bicolorMap();

void toggleMapBool(int map);

void HSVtoRGB(icVector3& hsv, icVector3& rgb);
void RGBtoHSV(icVector3& rgb, icVector3& hsv);

void findMinMax(double& min, double& max);
double max(double m1,double m2,double m3);
double min(double m1, double m2,double m3);


void cpheight(std::vector<POLYLINE> polylines);
double fyx(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22);
double fxy(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22);
double fyy(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22);
double fxx(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22);
double fofxy(double x, double y, double x1, double y1,
	double x2, double y2,double f11, double f21, double f12, double f22);
double findCriticalPoint(Quad* quad, double& x0, double& y0, bool& test, bool &saddle);

// void display_crit_points();
void plotCriticalPoints();
void criticalContours();
void Ncontours(int num_conts);
