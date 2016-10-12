package stanford.spl;

import java.awt.*;
import java.awt.image.*;
import java.io.*;

import javax.imageio.*;
import javax.swing.*;

/**
 * 
 * @author Marty Stepp
 * @version 2015/10/08
 * - bug fixes in to/fromGrid support
 * @version 2015/08/12
 * - added to/fromGrid support, much faster per-pixel operations
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
	
	public void load(String filename) throws IOException {
		bufferedImage = ImageIO.read(new File(filename));
		imageWidth = bufferedImage.getWidth();
		imageHeight = bufferedImage.getHeight();
		repaintImage();
	}
	
	public void resize(int w, int h, boolean retain) {
		if (retain && w == imageWidth && h == imageHeight) {
			// no-op
			return;
		}
		
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
	
	public void save(String filename) throws IOException {
		String extension = "png";
		int dot = filename.lastIndexOf('.');
		if (dot >= 0) {
			extension = filename.substring(dot + 1).toLowerCase();
		}
		ImageIO.write(bufferedImage, extension, new File(filename));
	}
	
	public void setRGB(int x, int y, int rgb) {
		bufferedImage.setRGB(x, y, rgb);
		repaintImage();
	}
	
	public String toStringBase64() {
		ByteArrayOutputStream out = new ByteArrayOutputStream(imageWidth * imageHeight * 3 + 512);
		
		// width and height each as 2-byte integers (0-65535)
		out.write(((imageWidth & 0x0000ff00) >> 8) & 0x000000ff);
		out.write(imageWidth & 0x000000ff);
		out.write(((imageHeight & 0x0000ff00) >> 8) & 0x000000ff);
		out.write(imageHeight & 0x000000ff);
		
		// each byte of image as a 3-byte integer (rgb)
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int px = bufferedImage.getRGB(x, y);
				int red = ((px & 0x00ff0000) >> 16) & 0x000000ff;
				int green = ((px & 0x0000ff00) >> 8) & 0x000000ff;
				int blue = px & 0x000000ff;
				out.write(red);
				out.write(green);
				out.write(blue);
			}
		}
		byte[] bytes = out.toByteArray();
		String base64raw = Base64.encodeBytes(bytes);
		String base64 = base64raw.replace("\r", "");
		base64 = base64raw.replace("\n", "");
		// JOptionPane.showMessageDialog(null, "GBufferedImage.toStringBase64: \n w=" + imageWidth + ", h=" + imageHeight + ", \n bytes=" + bytes.length + ", base64len=" + base64.length() + " (was " + base64raw.length() + ")");
		return base64;
	}
	
	public void fromStringBase64(String base64) throws IOException {
		byte[] bytes = Base64.decode(base64);
		int w = (((bytes[0] & 0x000000ff) << 8) & 0x0000ff00) | (bytes[1] & 0x000000ff);
		int h = (((bytes[2] & 0x000000ff) << 8) & 0x0000ff00) | (bytes[3] & 0x000000ff);
		if (w != imageWidth || h != imageHeight) {
			this.resize(w, h, /* retain */ false);
		}
		
		// JOptionPane.showMessageDialog(null, "GBufferedImage.fromStringBase64: w=" + imageWidth + ", h=" + imageHeight + ", bytes=" + bytes.length + ", base64len=" + base64.length());
		
		int[] pixelArray = new int[imageWidth * imageHeight];
		int byteIndex = 4;
		int index = 0;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				// OOB shouldn't happen, but let's just make sure not to walk past end of array
				if (byteIndex + 2 < bytes.length && index < pixelArray.length) {
					int rgb =
							0xff000000   // alpha
							| (((bytes[byteIndex] & 0x000000ff) << 16) & 0x00ff0000)
							| (((bytes[byteIndex + 1] & 0x000000ff) << 8) & 0x0000ff00)
							| ((bytes[byteIndex + 2]) & 0x000000ff);
					pixelArray[index] = rgb;
					index++;
					byteIndex += 3;
				}
			}
		}
		bufferedImage.setRGB(0, 0, imageWidth, imageHeight, pixelArray, 0, imageWidth);
		bufferedImage.flush();
		repaintImage();
	}
	
	// JL: SwingUtilities.invokeLater
	private void repaintImage() {
		Dimension oldSize = label.getPreferredSize();
		label.setIcon(new ImageIcon(bufferedImage));
		Dimension size = new Dimension(imageWidth, imageHeight);
		if (!size.equals(oldSize)) {
			label.setPreferredSize(size);
			label.setSize(size);
		}
		// label.repaint();
		// label.paintImmediately(0, 0, imageWidth, imageHeight);
	}
}
