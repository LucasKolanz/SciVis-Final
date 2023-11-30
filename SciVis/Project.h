#include "icVector.H"

bool fieldHeightOn = false;
bool contHeightOn = false;
bool rainbowMapOn = false;
bool greyscaleMapOn = false;
bool bicolorMapOn = false;

enum colorMaps {rainbow,bicolor,greyscale};
bool* colorMapBools[] = {&rainbowMapOn,&bicolorMapOn,&greyscaleMapOn};

void drawContourLine(double v, icVector3 rgb,int saddle_index);
icVector3 rainbowColorFromScalar(double scalar);

// void contHeight();
void fieldHeight();
void drawAllLines();

void rainbowMap();
void greyscaleMap();
void bicolorMap();

void toggleMapBool(int map);

void HSVtoRGB(icVector3& hsv, icVector3& rgb);
void RGBtoHSV(icVector3& rgb, icVector3& hsv);

void findMinMax(double& min, double& max);
double max(double m1,double m2,double m3);
double min(double m1, double m2,double m3);