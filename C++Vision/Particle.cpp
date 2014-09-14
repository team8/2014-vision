#include "Particle.h"
#include <math.h>

Particle::Particle(POINT point) {
    this.x = point.x;
    this.y = point.y
    
    this.width = 1;
    this.height = 1;
    
    points = new Vector<POINT>();
    points.push_back(point);
}

Particle::addPoint(POINT point) {
    	points.push_back(point);

		if (point.x > x + width) width += point.x - x - width;
		else if (point.x < x) {
			double offset = x - point.x;
			x -= offset;
			width += offset;
		}
		if (point.y > y + height) height += point.y - y - height;
		else if (point.y < y) {
			double offset = y - point.y;
			y -= offset;
			height += offset;
		}
}

Particle::getScore() {
    		return (int) round((double) points.size() / (width * height) * 100);
}
