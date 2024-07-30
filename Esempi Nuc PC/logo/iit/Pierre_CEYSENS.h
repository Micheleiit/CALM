/*h file for tracing*/

int32_t pos_x_gcode[305] PROGMEM = {
203, 200, 195, 188, 183, 180, 180, 182, 
183, 187, 197, 200, 202, 203, 203, 200, 
195, 188, 183, 180, 203, 200, 195, 188, 
183, 180, 180, 182, 183, 187, 197, 200, 
202, 203, 203, 200, 195, 188, 183, 180, 
168, 168, 168, 168, 145, 145, 145, 145, 
113, 135, 113, 135, 113, 127, 113, 127, 
113, 113, 113, 113, 102, 98, 93, 87, 
82, 78, 78, 80, 82, 85, 95, 98, 
100, 102, 102, 98, 93, 87, 82, 78, 
102, 98, 93, 87, 82, 78, 78, 80, 
82, 85, 95, 98, 100, 102, 102, 98, 
93, 87, 82, 78, 72, 58, 72, 58, 
45, 58, 58, 45, 58, 58, 18, 40, 
18, 40, 18, 32, 18, 32, 18, 18, 
18, 18, 7, 5, 2, -2, -8, -12, 
-15, -17, -18, -18, -17, -15, -12, -8, 
-2, 2, 5, 7, 7, 5, 2, -2, 
-8, -12, -15, -17, -18, -18, -17, -15, 
-12, -8, -2, 2, 5, 7, 18, 40, 
18, 40, 113, 135, 113, 135, 145, 168, 
145, 168, -68, -48, -48, -50, -52, -55, 
-60, -63, -67, -68, -68, -67, -63, -60, 
-55, -52, -48, -68, -48, -48, -50, -52, 
-55, -60, -63, -67, -68, -68, -67, -63, 
-60, -55, -52, -48, -88, -87, -83, -80, 
-75, -88, -87, -83, -80, -75, -88, -88, 
-88, -88, -110, -108, -105, -102, -97, -110, 
-108, -105, -102, -97, -110, -110, -110, -110, 
-142, -122, -122, -123, -125, -128, -133, -137, 
-140, -142, -142, -140, -137, -133, -128, -125, 
-122, -142, -122, -122, -123, -125, -128, -133, 
-137, -140, -142, -142, -140, -137, -133, -128, 
-125, -122, -153, -153, -153, -153, -155, -153, 
-152, -153, -155, -155, -153, -152, -153, -155, 
-188, -188, -188, -188, -188, -173, -168, -167, 
-165, -165, -167, -168, -173, -188, -188, -173, 
-168, -167, -165, -165, -167, -168, -173, -188, 
-99};

int32_t pos_y_gcode[305] PROGMEM = {
16, 19, 21, 21, 19, 16, 12, 9, 
7, 6, 2, 1, -1, -4, -9, -13, 
-14, -14, -13, -9, 16, 19, 21, 21, 
19, 16, 12, 9, 7, 6, 2, 1, 
-1, -4, -9, -13, -14, -14, -13, -9, 
21, -14, 21, -14, 21, -14, 21, -14, 
-14, -14, -14, -14, 4, 4, 4, 4, 
21, -14, 21, -14, 16, 19, 21, 21, 
19, 16, 12, 9, 7, 6, 2, 1, 
-1, -4, -9, -13, -14, -14, -13, -9, 
16, 19, 21, 21, 19, 16, 12, 9, 
7, 6, 2, 1, -1, -4, -9, -13, 
-14, -14, -13, -9, 21, 4, 21, 4, 
21, 4, -14, 21, 4, -14, -14, -14, 
-14, -14, 4, 4, 4, 4, 21, -14, 
21, -14, 12, 16, 19, 21, 21, 19, 
16, 12, 7, -1, -6, -9, -13, -14, 
-14, -13, -9, -6, 12, 16, 19, 21, 
21, 19, 16, 12, 7, -1, -6, -9, 
-13, -14, -14, -13, -9, -6, 21, 21, 
21, 21, 21, 21, 21, 21, 21, -14, 
21, -14, -1, -1, 2, 6, 7, 9, 
9, 7, 4, -1, -4, -9, -13, -14, 
-14, -13, -9, -1, -1, 2, 6, 7, 
9, 9, 7, 4, -1, -4, -9, -13, 
-14, -14, -13, -9, -1, 4, 7, 9, 
9, -1, 4, 7, 9, 9, 9, -14, 
9, -14, -1, 4, 7, 9, 9, -1, 
4, 7, 9, 9, 9, -14, 9, -14, 
-1, -1, 2, 6, 7, 9, 9, 7, 
4, -1, -4, -9, -13, -14, -14, -13, 
-9, -1, -1, 2, 6, 7, 9, 9, 
7, 4, -1, -4, -9, -13, -14, -14, 
-13, -9, 9, -14, 9, -14, 21, 19, 
21, 22, 21, 21, 19, 21, 22, 21, 
21, -14, 21, -14, 21, 21, 19, 17, 
14, 9, 6, 4, 2, 2, 21, 21, 
19, 17, 14, 9, 6, 4, 2, 2, 
-223};

int32_t speed_gcode[305] PROGMEM = {
1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 1, 0, 1, 0, 1, 0, 
1, 0, 1, 0, 1, 0, 1, 0, 
1, 0, 1, 0, 1, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 1, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 1, 0, 1, 0, 
1, 0, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 1, 0, 
1, 0, 1, 0, 1, 0, 1, 0, 
1, 0, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 0, 0, 
0, 1, 0, 0, 0, 0, 1, 0, 
1, 0, 1, 0, 0, 0, 0, 1, 
0, 0, 0, 0, 1, 0, 1, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 1, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 1, 0, 1, 0, 1, 0, 
0, 0, 0, 1, 0, 0, 0, 0, 
1, 0, 1, 0, 1, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 1, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
1};