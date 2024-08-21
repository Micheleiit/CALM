/*
* pen_zoom.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/
#include "pen_zoom.h"

void initZoomStruct(ZoomStruct* zoomst){
  zoomst->zoomAmount = 0;
  zoomst->zoomFactor = 0;

 zoomst->zoom_state = NORMAL_ZOOM;
 
}