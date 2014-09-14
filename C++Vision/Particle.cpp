#include "Particle.h"
#include <math.h>
/*
 * cpp file for particle class
 * see header file for method and field descriptions
 */

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

Particle::getAspectRatioScore() {
	int perimeter = 2 * width + 2 * height;
	int area = points.size();
	double equivWidth;
	double equivHeight;
	double score;
	/* Calculates aspect ratio, taking incline into account, and compares with real ratio tp generate score*/
	if (height > width) {
		equivWidth = 0.25 * (perimeter - sqrt(perimeter * perimeter - 16 * area));
		equivHeight = 0.25 * (perimeter + sqrt(perimeter * perimeter - 16 * area));
		score = (equivWidth/equivHeight) / (1.0/8);
	} 
	else {
		equivWidth = 0.25 * (perimeter + sqrt(perimeter * perimeter - 16 * area));
		equivHeight = 0.25 * (perimeter - sqrt(perimeter * perimeter - 16 * area));
		score = (equivWidth/equivHeight) / (23.5/4);
	}
	/* Returns the difference between 100 and the measure of how far from perfect the aspect ratio score is */
	return (int) round(std::max(0, std::min(100, (1 - abs(1 - score)) * 100)));
}

Particle::getArea() {
	return points.size();
}

Particle::determineOrientation() {
	orientation = height > width ? Orientation.Vertical : Orientation.Horizontal;
}
