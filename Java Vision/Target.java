package JavaVision;

public class Target {
	final static int TAPE_WIDTH_LIMIT = 50;
	final static int VERTICAL_SCORE_LIMIT = 50;
	final static int LR_SCORE_LIMIT = 50;

	Particle horizontalParticle = null, verticalParticle;
	int totalScore = 0, leftScore = 0, rightScore = 0, tapeWidthScore = 0, verticalScore = 0;
	boolean hot = false;

	void checkIfHot() {
		hot = tapeWidthScore >= TAPE_WIDTH_LIMIT & verticalScore >= VERTICAL_SCORE_LIMIT & ((leftScore > LR_SCORE_LIMIT) | (rightScore > LR_SCORE_LIMIT));
	}
}
