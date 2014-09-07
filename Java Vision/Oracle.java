import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.ArrayList;

public class Oracle {
	final double HALF_CAMERA_ANGLE = 56 / 360.0 * Math.PI; // Axis M1013 is about 67 degrees, but this might be correct
	final int ERROR_MARGIN = 3;
	final int RECTANGULARITY_LIMIT = 40;
	final int ASPECT_RATIO_LIMIT = 55;
	final int MINIMUM_AREA = 150;
	BufferedImage image;
	ArrayList<Point> candidatePoints = new ArrayList<>();
	ArrayList<Particle> particles = new ArrayList<>();
	ArrayList<Particle> verticalTargets = new ArrayList<>();
	ArrayList<Particle> horizontalTargets = new ArrayList<>();
	Target target = new Target();
	double distance;

	public Oracle(BufferedImage image) {
		this.image = image;
	}

	/**
	 * Takes a value between 0 and 2 and returns a score between 0 and 100 on how close to 1 the value is.
	 *
	 * @param value The value to score
	 * @return A score between 0 and 100 of how close to 1 the value is
	 */
	int valueToScore(double value) {
		return (int) Math.round(Math.max(0, Math.min(100, (1 - Math.abs(1 - value)) * 100)));
	}

	void mask() {
		long startTime = System.nanoTime();
		for (int x = 0; x < image.getWidth(); x++) {
			for (int y = 0; y < image.getHeight(); y++) {
				HSVColor color = new HSVColor(image.getRGB(x, y));
				if (color.getH() >= 150 && color.getH() < 195 && color.getV() > 40) {
					candidatePoints.add(new Point(x, y));
				}
			}
		}
		System.out.println("Time to mask: " + (System.nanoTime() - startTime) / 1_000_000_000.0);
	}

	void groupToParticles() {
		// Group the pixels into particles.
		long startTime = System.nanoTime();
		while (!candidatePoints.isEmpty()) {
			Particle particle = new Particle(candidatePoints.get(0));
			ArrayList<Point> pointsToSearch = new ArrayList<>();
			pointsToSearch.add(candidatePoints.remove(0));
			while (getNeighbors(pointsToSearch, particle));
			particle.determineOrientation();
			particles.add(particle);
		}
		System.out.println("Time to group: " + (System.nanoTime() - startTime) / 1_000_000_000.0);
	}

	boolean getNeighbors(ArrayList<Point> pointsToSearch, Particle particle) {
		ArrayList<Point> newPoints = new ArrayList<>();
		for (Point parentPoint : pointsToSearch) {
			for (Point point : candidatePoints) {
				boolean xInRange = point.getX() <= parentPoint.getX() + ERROR_MARGIN && point.getX() >= parentPoint.getX() - ERROR_MARGIN;
				boolean yInRange = point.getY() <= parentPoint.getY() + ERROR_MARGIN && point.getY() >= parentPoint.getY() - ERROR_MARGIN;
				if (xInRange && yInRange) {
					newPoints.add(point);
				}
			}

			candidatePoints.removeAll(newPoints);
		}

		for (Point point : pointsToSearch) particle.addPoint(point);
		pointsToSearch.clear();
		pointsToSearch.addAll(newPoints);

		return pointsToSearch.size() > 0;
	}

	void splitAndWeed() {
		long startTime = System.nanoTime();
		for (Particle particle : particles) {
			if (particle.getRectScore() >= RECTANGULARITY_LIMIT && particle.getAspectRatioScore() > ASPECT_RATIO_LIMIT && particle.getArea() > MINIMUM_AREA) {
				if (particle.isVertical) verticalTargets.add(particle);
				else horizontalTargets.add(particle);
			}
		}
		System.out.println("Time to split and weed: " + (System.nanoTime() - startTime) / 1_000_000_000.0);
	}

	void pairParticles() {
		long startTime = System.nanoTime();
		if (verticalTargets.size() > 0) {
			target.verticalParticle = verticalTargets.get(0);
			for (Particle verticalParticle : verticalTargets) {
				for (Particle horizontalParticle : horizontalTargets) {
					int leftScore = valueToScore(1.2 * (verticalParticle.x - (horizontalParticle.x + horizontalParticle.width / 2)) / horizontalParticle.width);
					int rightScore = valueToScore(1.2 * ((horizontalParticle.x + horizontalParticle.width / 2) - verticalParticle.x - verticalParticle.width) / horizontalParticle.width);
					int tapeWidthScore = valueToScore(verticalParticle.width / horizontalParticle.height);
					int verticalScore = valueToScore(1 - (verticalParticle.y - (horizontalParticle.y + horizontalParticle.height / 2)) / (4 * horizontalParticle.height));

					int total = leftScore > rightScore ? leftScore : rightScore;
					total += tapeWidthScore + verticalScore;

					if (total > target.totalScore) {
						target.horizontalParticle = horizontalParticle;
						target.verticalParticle = verticalParticle;
						target.totalScore = total;
						target.leftScore = leftScore;
						target.rightScore = rightScore;
						target.tapeWidthScore = tapeWidthScore;
						target.verticalScore = verticalScore;
					}
				}

				target.checkIfHot();
			}
		}
		System.out.println("Time to pair: " + (System.nanoTime() - startTime) / 1_000_000_000.0);
	}

	void findDistance() {
		double widthFt = (8.0/3.0) * image.getWidth() / (2 * target.verticalParticle.height * Math.tan(HALF_CAMERA_ANGLE)) / 2.0;
		distance = widthFt/Math.tan(HALF_CAMERA_ANGLE);
	}

	Point getAimCoordinate(int x, int y) {
		int aimX = (x - image.getWidth()/2) / (image.getWidth()/2);
		int aimY = (y - image.getHeight()/2) / (image.getHeight()/2);
		return new Point(x, y);
	}
}
