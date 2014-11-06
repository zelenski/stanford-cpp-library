package stanford.spl;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.*;
import javax.swing.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/10/22
 */
public class GBufferedImage extends GInteractor {
	private static final long serialVersionUID = 1L;
	
	private BufferedImage bufferedImage;
	private int imageWidth;
	private int imageHeight;
	private int backgroundColor;
	private JLabel label;
	
	public GBufferedImage(int width, int height) {
		this(width, height, 0);   // black
	}
	
	public GBufferedImage(int width, int height, int backgroundColor) {
		super(new JLabel("HOORAY"));
		this.imageWidth = width;
		this.imageHeight = height;
		this.backgroundColor = backgroundColor;
		bufferedImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		label = (JLabel) this.getInteractor();
		if (backgroundColor != 0) {
			fill(backgroundColor);
		}
		repaintImage();
	}
	
	public void clear() {
		fill(backgroundColor);
	}
	
	public void fill(int rgb) {
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				bufferedImage.setRGB(x, y, rgb);
			}
		}
		repaintImage();
	}
	
	public void fillRegion(int x, int y, int width, int height, int rgb) {
		for (int yy = y; yy < y + height; yy++) {
			for (int xx = x; xx < x + width; xx++) {
				bufferedImage.setRGB(xx, yy, rgb);
			}
		}
		repaintImage();
	}
	
	public BufferedImage getBufferedImage() {
		return bufferedImage;
	}
	
	public int getImageHeight() {
		return imageHeight;
	}
	
	public int getImageWidth() {
		return imageWidth;
	}
	
	public void load(String filename) {
		try {
			bufferedImage = ImageIO.read(new File(filename));
			imageWidth = bufferedImage.getWidth();
			imageHeight = bufferedImage.getHeight();
			repaintImage();
			SplPipeDecoder.writeResult(toStringBase64());   // this is a LONG string
		} catch (Exception ex) {
			SplPipeDecoder.writeResult("error:" + ex.getClass().getSimpleName() + ": " + ex.getMessage().replace('\n', ' '));
		}
	}
	
	public void resize(int w, int h, boolean retain) {
		BufferedImage oldImage = bufferedImage;
		bufferedImage = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		imageWidth = w;
		imageHeight = h;
		fill(backgroundColor);
		if (retain) {
			Graphics g = bufferedImage.getGraphics();
			g.drawImage(oldImage, 0, 0, label);
		}
		repaintImage();
	}
	
	public void save(String filename) {
		String extension = "png";
		int dot = filename.lastIndexOf('.');
		if (dot >= 0) {
			extension = filename.substring(dot + 1).toLowerCase();
		}
		
		try {
			ImageIO.write(bufferedImage, extension, new File(filename));
			SplPipeDecoder.writeResult("ok");
		} catch (Exception ex) {
			SplPipeDecoder.writeResult("error:" + ex.getClass().getSimpleName() + ": " + ex.getMessage().replace('\n', ' '));
		}
	}
	
	public void setRGB(int x, int y, int rgb) {
		bufferedImage.setRGB(x, y, rgb);
		repaintImage();
	}
	
	public String toStringBase64() {
		StringBuilder sb = new StringBuilder(1024 * 256);
		sb.append((int) imageWidth);
		sb.append('\n');
		sb.append((int) imageHeight);
		sb.append('\n');
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				// trim off top two hex digits (alpha) because C++ lib expects 24-bit ints
				int px = bufferedImage.getRGB(x, y);
				sb.append(String.format("#%06x\n", px & 0x00ffffff));
			}
		}
		String base64 = Base64.encodeBytes(sb.toString().getBytes());
		return base64;
	}
	
	private void repaintImage() {
		label.setIcon(new ImageIcon(bufferedImage));
		Dimension size = new Dimension(imageWidth, imageHeight);
		label.setPreferredSize(size);
		label.setSize(size);
	}
}
