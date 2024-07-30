/*h file for tracing the iit logo*/

int32_t pos_x_gcode[365] PROGMEM = {
-5226, 4545, -5226, 4545, 4545, 4777, 4942, 4545, 
4777, 4942, 4942, 4985, 4997, 4942, 4985, 4997, 
4997, 4996, 4997, 4996, 4996, 4882, 4613, 4996, 
4882, 4613, 4613, 4372, 4129, 4613, 4372, 4129, 
4129, 3448, 2768, 4129, 3448, 2768, 2768, -5682, 
2768, -5682, -5682, -5682, -5682, -5682, -5682, -5677, 
-5645, -5682, -5677, -5645, -5645, -5478, -5226, -5645, 
-5478, -5226, -5056, -5213, -5355, -5056, -5213, -5355, 
-5355, -5497, -5556, -5355, -5497, -5556, -5556, -5556, 
-5556, -5556, -5556, -4123, -2689, -5556, -4123, -2689, 
-2689, -1612, -534, -2689, -1612, -534, -534, 4355, 
-534, 4355, 4355, 4567, 4747, 4355, 4567, 4747, 
4747, 4834, 4870, 4747, 4834, 4870, 4870, 4870, 
4870, 4870, 4870, 4832, 4740, 4870, 4832, 4740, 
4740, 4569, 4369, 4740, 4569, 4369, 4369, -5056, 
4369, -5056, -3717, -3466, -3227, -3717, -3466, -3227, 
-3227, -3001, -2882, -3227, -3001, -2882, -2882, -2880, 
-2978, -2882, -2880, -2978, -2978, -3148, -3374, -2978, 
-3148, -3374, -3374, -3715, -4024, -3374, -3715, -4024, 
-4024, -4222, -4283, -4024, -4222, -4283, -4283, -4242, 
-4121, -3940, -3717, -4283, -4242, -4121, -3940, -3717, 
-4250, -3580, -2909, -4250, -3580, -2909, -2909, -2909, 
-2909, -2909, -2909, -3580, -4250, -2909, -3580, -4250, 
-4250, -4250, -4250, -4250, -1688, -1018, -347, -1688, 
-1018, -347, -347, -347, -347, -347, -347, -1018, 
-1688, -347, -1018, -1688, -1688, -1688, -1688, -1688, 
-1362, -1576, -1700, -1362, -1576, -1700, -1700, -1717, 
-1629, -1700, -1717, -1629, -1629, -1418, -1127, -1629, 
-1418, -1127, -1127, -893, -671, -1127, -893, -671, 
-671, -446, -323, -671, -446, -323, -323, -315, 
-404, -323, -315, -404, -404, -593, -852, -404, 
-593, -852, -852, -1113, -1362, -852, -1113, -1362, 
405, 718, 1030, 405, 718, 1030, 1030, 1254, 
1479, 1030, 1254, 1479, 1479, 1900, 2322, 1479, 
1900, 2322, 2322, 2321, 2321, 2322, 2321, 2321, 
2321, 2958, 3596, 2321, 2958, 3596, 3596, 3596, 
3596, 3596, 3596, 3596, 3596, 2959, 2321, 3596, 
2959, 2321, 2321, 2321, 2321, 2321, 2321, 2321, 
2321, 2335, 2430, 2321, 2335, 2430, 2430, 2587, 
2788, 2430, 2587, 2788, 2788, 3179, 3562, 2788, 
3179, 3562, 3562, 3562, 3562, 3562, 3562, 3562, 
3562, 3129, 2684, 3562, 3129, 2684, 2684, 2248, 
1822, 2684, 2248, 1822, 1822, 1436, 1166, 1822, 
1436, 1166, 1166, 1025, 979, 1166, 1025, 979, 
979, 979, 979, 979, 979, 405, 979, 405, 
405, 405, 405, 405, -1142};

int32_t pos_y_gcode[365] PROGMEM = {
3961, 3961, 3961, 3961, 3961, 3843, 3641, 3961, 
3843, 3641, 3641, 3514, 3381, 3641, 3514, 3381, 
3381, -4372, 3381, -4372, -4372, -4688, -4886, -4372, 
-4688, -4886, -4886, -4934, -4935, -4886, -4934, -4935, 
-4935, -4935, -4931, -4935, -4935, -4931, -4931, -4931, 
-4931, -4931, -4931, 3352, -4931, 3352, 3352, 3479, 
3602, 3352, 3479, 3602, 3602, 3832, 3961, 3602, 
3832, 3961, 3859, 3834, 3765, 3859, 3834, 3765, 
3765, 3618, 3422, 3765, 3618, 3422, 3422, -4805, 
3422, -4805, -4805, -4805, -4807, -4805, -4805, -4807, 
-4807, -4809, -4809, -4807, -4809, -4809, -4809, -4809, 
-4809, -4809, -4809, -4769, -4649, -4809, -4769, -4649, 
-4649, -4523, -4373, -4649, -4523, -4373, -4373, 3422, 
-4373, 3422, 3422, 3578, 3708, 3422, 3578, 3708, 
3708, 3819, 3859, 3708, 3819, 3859, 3859, 3859, 
3859, 3859, 2862, 2867, 2785, 2862, 2867, 2785, 
2785, 2582, 2302, 2785, 2582, 2302, 2302, 2028, 
1772, 2302, 2028, 1772, 1772, 1586, 1472, 1772, 
1586, 1472, 1472, 1456, 1604, 1472, 1456, 1604, 
1604, 1859, 2176, 1604, 1859, 2176, 2176, 2412, 
2618, 2775, 2862, 2176, 2412, 2618, 2775, 2862, 
1084, 1084, 1084, 1084, 1084, 1084, 1084, -3780, 
1084, -3780, -3780, -3780, -3780, -3780, -3780, -3780, 
-3780, 1084, -3780, 1084, 1084, 1084, 1084, 1084, 
1084, 1084, 1084, -3780, 1084, -3780, -3780, -3780, 
-3780, -3780, -3780, -3780, -3780, 1084, -3780, 1084, 
1533, 1721, 1978, 1533, 1721, 1978, 1978, 2256, 
2520, 1978, 2256, 2520, 2520, 2750, 2867, 2520, 
2750, 2867, 2867, 2865, 2789, 2867, 2865, 2789, 
2789, 2591, 2317, 2789, 2591, 2317, 2317, 2048, 
1793, 2317, 2048, 1793, 1793, 1580, 1461, 1793, 
1580, 1461, 1461, 1450, 1533, 1461, 1450, 1533, 
945, 961, 978, 945, 961, 978, 978, 1787, 
2597, 978, 1787, 2597, 2597, 2597, 2595, 2597, 
2597, 2595, 2595, 1792, 988, 2595, 1792, 988, 
988, 988, 988, 988, 988, 988, 988, 615, 
242, 988, 615, 242, 242, 242, 242, 242, 
242, 242, 242, -967, -2175, 242, -967, -2175, 
-2175, -2484, -2780, -2175, -2484, -2780, -2780, -2921, 
-2976, -2780, -2921, -2976, -2976, -2969, -2886, -2976, 
-2969, -2886, -2886, -3268, -3649, -2886, -3268, -3649, 
-3649, -3766, -3815, -3649, -3766, -3815, -3815, -3816, 
-3717, -3815, -3816, -3717, -3717, -3469, -3098, -3717, 
-3469, -3098, -3098, -2675, -2233, -3098, -2675, -2233, 
-2233, 248, -2233, 248, 248, 248, 248, 248, 
248, 945, 248, 945, -991};

int32_t speed_gcode[365] PROGMEM = {
1, 0, 1, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 1, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 1, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 0, 0, 1, 0, 0, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
1, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 1, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 1, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 1, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 0, 1, 0, 0, 1, 0, 
0, 1, 0, 0, 1, 0, 0, 1, 
0, 0, 1, 0, 0, 1, 0, 0, 
1, 0, 1, 0, 1, 0, 1, 0, 
1, 0, 1, 0, 1};