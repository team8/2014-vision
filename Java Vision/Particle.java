import java.awt.*;
import java.util.ArrayList;

/**
 * A class representing a particle viewed by the visison system
 */
public class Particle {
	/** The array list of points comprising this particle **/
	ArrayList<Point> points = new ArrayList<>();
	/** The x,y coordinates, dimensions of this particle **/
	int x, y, width = 1, height = 1;
	/** The boolean determining the orientation of this particle **/
	boolean isVertical;
	
	/**
	 * Parameterixed constructor, initializes particle to given point
	 * 
	 * @param point	the first point composing this particle
	 */
	public Particle(Point point) {
		this.x = (int) point.getX();
		this.y = (int) point.getY();
		points.add(point);
	}
	
	/**
	 * Adds the given point to this particle
	 * 
	 * @param point	the point to be added
	 */ 
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
	
	/**
	 * Returns the score of this particle
	 * 
	 * @return A measure of the imperfectness of this particle (between 0 - 100)
	 */
	int getRectScore() {
		return (int) Math.round((double) points.size() / (width * height) * 100);
	}

	/**
	 * Returns the aspect ratio score of this particle
	 * 
	 * @return the measure of how close this particle's aspect ratio is to the real one (between 0 and 100)
	 */
	int getAspectRatioScore() {
		int perimeter = 2 * width + 2 * height;
		int area = points.size();
		double equivWidth, equivHeight;
		double score;
		/* Calculates aspect ratio, taking incline into account, and compares with real ratio tp generate score*/
		if (height > width) {
			equivWidth = 0.25 * (perimeter - Math.sqrt(perimeter * perimeter - 16 * area));
			equivHeight = 0.25 * (perimeter + Math.sqrt(perimeter * perimeter - 16 * area));
			score = (equivWidth/equivHeight) / (1.0/8);
		} else {
			equivWidth = 0.25 * (perimeter + Math.sqrt(perimeter * perimeter - 16 * area));
			equivHeight = 0.25 * (perimeter - Math.sqrt(perimeter * perimeter - 16 * area));
			score = (equivWidth/equivHeight) / (23.5/4);
		}
		/* Returns the difference between 100 and the measure of how far from perfect the aspect ratio score is */
		return (int) Math.round(Math.max(0, Math.min(100, (1 - Math.abs(1 - score)) * 100)));
	}
	
	/**
	 * Returns the area of this particle
	 */
	int getArea() {
		return points.size();
	}
	
	/** 
	 * Sets the isVertical boolean depending on the width and height
	 */
	void determineOrientation() {
		isVertical = height > width;
	}
}
