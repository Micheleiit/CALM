/*
* pen_zoom.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/
#include "pen_zoom.h"


void initZoomStruct(ZoomStruct* zoomStruct){
  zoomStruct->zoomAmount = 0;
  zoomStruct->zoomFactor = 0;

 zoomStruct->zoom_state = NORMAL_ZOOM;
 
}