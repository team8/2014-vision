package JavaVision;

import java.awt.*;
import java.util.ArrayList;

public class Particle {
	ArrayList<Point> points = new ArrayList<>();
	int x, y, width = 1, height = 1;
	boolean isVertical;

	public Particle(Point point) {
		this.x = (int) point.getX();
		this.y = (int) point.getY();
		points.add(point);
	}

	void addPoint(Point point) {
		points.add(point);

		if (point.getX() > x + width) width += point.getX() - x - width;
		else if (point.getX() < x) {
			double offset = x - point.getX();
			x -= offset;
			width += offset;
		}
		if (point.getY() > y + height) height += point.getY() - y - height;
		else if (point.getY() < y) {
			double offset = y - point.getY();
			y -= offset;
			height += offset;
		}
	}

	int getRectScore() {
		return (int) Math.round((double) points.size() / (width * height) * 100);
	}

	int getAspectRatio() {
		int perimeter = 2 * width + 2 * height;
		int area = points.size();
		double equivWidth, equivHeight;
		double score;

		if (height > width) {
			equivWidth = 0.25 * (perimeter - Math.sqrt(perimeter * perimeter - 16 * area));
			equivHeight = 0.25 * (perimeter + Math.sqrt(perimeter * perimeter - 16 * area));
			score = (equivWidth/equivHeight) / (1.0/8);
		} else {
			equivWidth = 0.25 * (perimeter + Math.sqrt(perimeter * perimeter - 16 * area));
			equivHeight = 0.25 * (perimeter - Math.sqrt(perimeter * perimeter - 16 * area));
			score = (equivWidth/equivHeight) / (23.5/4);
		}
		return (int) Math.round(Math.max(0, Math.min(100, (1 - Math.abs(1 - score)) * 100)));
	}

	int getArea() {
		return points.size();
	}

	void determineOrientation() {
		isVertical = height > width;
	}
}
