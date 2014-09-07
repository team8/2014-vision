import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class Main {
	public static void main(String[] args) {
		boolean saveImage = false;
		BufferedImage image;
		ImageWindow window = new ImageWindow();

		// Load the image
		try {
			image = ImageIO.read(Main.class.getResourceAsStream("18ft.jpg"));
		} catch (IOException e) {
			e.printStackTrace();
			image = null;
		}
		window.setImage(image);

		// Do all of the actual vision stuff
		Oracle oracle = new Oracle(image);
		oracle.mask();

		// Mask the image for visual aid
		image = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_INT_RGB);
		for (Point point : oracle.candidatePoints) {
			image.setRGB((int) point.getX(), (int) point.getY(), 0x0099FF);
		}
		window.setImage(image);

		// Finish vision stuff
		oracle.groupToParticles();
		oracle.splitAndWeed();
		oracle.pairParticles();
		oracle.findDistance();

		// Draw the bounding boxes of the particles in green
		for (Particle particle : oracle.particles) {
			Graphics g = image.getGraphics();
			g.setColor(Color.GREEN);
			g.drawRect(particle.x - 1, particle.y - 1, particle.width + 2, particle.height + 2);
			if (particle.getRectScore() > 100) {
				g.setColor(Color.RED);
				for (Point point : particle.points) {
					g.fillRect(point.x, point.y, 1, 1);
				}
			}
			g.dispose();
		}

		// Redo the targets' bounding boxes in yellow
		ArrayList<Particle> targets = new ArrayList<>();
		targets.addAll(oracle.verticalTargets);
		targets.addAll(oracle.horizontalTargets);
		for (Particle particle : targets) {
			Graphics g = image.getGraphics();
			g.setColor(Color.YELLOW);
			g.drawRect(particle.x - 1, particle.y - 1, particle.width + 2, particle.height + 2);
			if (particle.getRectScore() > 100) {
				g.setColor(Color.RED);
				for (Point point : particle.points) {
					g.fillRect(point.x, point.y, 1, 1);
				}
			}
			g.dispose();
		}
		window.setImage(image);

		if (oracle.verticalTargets.size() > 0) {
			Target target = oracle.target;

			// Outline the target in red
			Graphics g = image.getGraphics();
			g.setColor(Color.RED);
			g.drawRect(target.verticalParticle.x - 1, target.verticalParticle.y - 1, target.verticalParticle.width + 2, target.verticalParticle.height + 2);
			if (target.horizontalParticle != null)
				g.drawRect(target.horizontalParticle.x - 1, target.horizontalParticle.y - 1, target.horizontalParticle.width + 2, target.horizontalParticle.height + 2);
			g.dispose();
			window.setImage(image);

			if (target.hot) System.out.println("Target is hot");
			else System.out.println("Target isn't hot");

			System.out.println("Distance to wall: " + oracle.distance);
		}

		if (saveImage) {
			try {
				int fileNum = 1;
				File file = new File("/Users/Tyler/Desktop/" + fileNum + ".png");

				while (!file.createNewFile()) file = new File("/Users/Tyler/Desktop/" + (++fileNum) + ".png");

				ImageIO.write(image, "png", file);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private static class ImageWindow extends JPanel {
		JFrame holder = new JFrame();
		BufferedImage image = null;

		public ImageWindow() {
			setPreferredSize(new Dimension(640, 480));
			holder.add(this);
			holder.pack();
			holder.setResizable(false);
			holder.setVisible(true);
			holder.validate();
			holder.repaint();
		}

		void setImage(BufferedImage image) {
			this.image = image;
			repaint();
		}

		@Override
		public void paint(Graphics g) {
			g.drawImage(image, 0, 0, null);
		}
	}
}
