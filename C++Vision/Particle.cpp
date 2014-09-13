#include "Particle.h"

Particle::Particle(POINT point) {
    this.x = point.x;
    this.y = point.y
    
    this.width = 1;
    this.height = 1;
    
    points = new Vector<POINT>();
}
