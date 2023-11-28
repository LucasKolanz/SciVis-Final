#include "icVector.H"

bool fieldHeightOn = false;
bool contHeightOn = false;

void drawContourLine(double v, icVector3 rgb,int saddle_index);
void findMinMax(double& min, double& max);
// void contHeight();
void fieldHeight();
void drawAllLines();