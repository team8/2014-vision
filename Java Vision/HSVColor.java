package JavaVision;

import java.awt.*;

public class HSVColor {
	private int h, s, v;

	public HSVColor(int rgb) {
		float[] HSV = new float[3];
		Color color = new Color(rgb);
		Color.RGBtoHSB(color.getRed(), color.getGreen(), color.getBlue(), HSV);
		setH(Math.round(HSV[0] * 359));
		setS(Math.round(HSV[1] * 100));
		setV(Math.round(HSV[2] * 100));
	}


	void setH(int h) {
		this.h = Math.max(0, Math.min(359, h));
	}

	int getH() {
		return h;
	}

	void setS(int s) {
		this.s = Math.max(0, Math.min(100, s));
	}

	int getS() {
		return s;
	}

	void setV(int v) {
		this.v = Math.max(0, Math.min(100, v));
	}

	int getV() {
		return v;
	}

	Color getColor() {
		return new Color(Color.HSBtoRGB(h, s, v));
	}
}
