/*
   Draws a line


   Outputs:
   - x,y

   Inputs:
   - angle: angle in degrees
   - period: cycle period in ms
   - offset: absolute position if the center of the line.
   - scale: Length of the line
   - t_millis: current time

*/
void line(int32_t *x, int32_t *y, float angle, uint32_t period, int32_t offset_x, int32_t offset_y, float scale, unsigned long t_millis)
{
  if (period <= 0)
    Serial1.print("Line period = 0");

  uint32_t t;
  double x_temp;
  double y_temp;

  if (t_millis % period < period / 2.0)
  {
    t = t_millis % period;
  }
  else
  {
    t = period - (t_millis % period);
  }
  x_temp = (t - period / 4.0) / (period / 2.0) * cos(angle * M_PI / 180.0);
  y_temp = (t - period / 4.0) / (period / 2.0) * sin(angle * M_PI / 180.0);

  *x = x_temp * scale + offset_x;
  *y = y_temp * scale + offset_y;
}
