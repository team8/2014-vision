#ifindef PARTICLE_H
#define PARTICLE_H

#include "WPILib.h"

/*
 * A class representing a particle viewed by the visison system
 */

class Particle {
private:
  
  // The x-coordinate of this particle
  int x;
  
  // The y-coordinate of this particle
  int y;
  
  // The width of this particle
  int width;
  
  // The height of this particle
  int height;
  
  // The orientation of this particle
  Orientation orientation;
  
  // The vector of points making up this particle
  std::vector<POINT> points;

public:
  
  // The struct representing a point 
  struct POINT {
    int x;
    int y;
  }
  
  enum Orientation {
    Vertical,
    Horizontal
  }
  
  //Parameterized Constructor, initializes particle to given point
  Particle(POINT point);
  
  //Sets the orientation of this instance
  void determineOrientation();
  
  //Adds the given point to this particle
  void addPoint(POINT point);
  
  // Gets a score demonstrating the rectangularity of this particle from 0 - 100
  int getRectScore();
  
  // Gets a score demonstrating the measure of closeness of this to the actual aspect ratio from 0 - 100
  int getAspectRatioScore();
  
  //Returns the area of this particle
  int getArea()
}

#endif
