package stanford.spl;

import java.awt.*;
import java.awt.image.*;
import java.io.*;

import javax.imageio.*;
import javax.swing.*;

/**
 * 
 * @author Marty Stepp
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
	
	public void load(String filename) {
		try {
			bufferedImage = ImageIO.read(new File(filename));
			imageWidth = bufferedImage.getWidth();
			imageHeight = bufferedImage.getHeight();
			repaintImage();
			// SplPipeDecoder.writeResult(toStringCompressed());   // this is a LONG string
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
//		StringBuilder sb = new StringBuilder(1024 * 256);
//		sb.append((int) imageWidth);
//		sb.append('\n');
//		sb.append((int) imageHeight);
//		sb.append('\n');
//		for (int y = 0; y < imageHeight; y++) {
//			for (int x = 0; x < imageWidth; x++) {
//				// trim off top two hex digits (alpha) because C++ lib expects 24-bit ints
//				int px = bufferedImage.getRGB(x, y);
//				sb.append(String.format("#%06x\n", px & 0x00ffffff));
//			}
//		}
		
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
				int red = (px & 0xff0000) >> 16;
				int green = (px & 0x00ff00) >> 8;
				int blue = px & 0x0000ff;
				out.write(red);
				out.write(green);
				out.write(blue);
			}
		}
		String base64 = Base64.encodeBytes(out.toByteArray());
		return base64;
	}
	
	public String toStringCompressed() {
		StringBuilder sb = new StringBuilder(imageWidth * imageHeight * 7 + 32);
		sb.append((int) imageWidth);
		sb.append('#');
		sb.append((int) imageHeight);
		sb.append('#');
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				// trim off top two hex digits (alpha) because C++ lib expects 24-bit ints
				int px = bufferedImage.getRGB(x, y);
				sb.append(String.format("#%06x", px & 0x00ffffff));
			}
		}
		return sb.toString();
	}
	
	public void fromStringBase64(String base64) throws IOException {
		byte[] bytes = Base64.decode(base64);
		int w = (((bytes[0] & 0x000000ff) << 8) & 0x0000ff00) | (bytes[1] & 0x000000ff);
		int h = (((bytes[2] & 0x000000ff) << 8) & 0x0000ff00) | (bytes[3] & 0x000000ff);
		if (w != imageWidth || h != imageHeight) {
			this.resize(w, h, /* retain */ false);
		}
		
		int[] pixelArray = new int[w * h];
		int byteIndex = 4;
		int index = 0;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int rgb =
						0xff000000   // alpha
						| ((bytes[byteIndex] << 16) & 0x00ff0000)
						| ((bytes[byteIndex + 1] << 8) & 0x0000ff00)
						| ((bytes[byteIndex + 2]) & 0x000000ff);
				pixelArray[index] = rgb;
				index++;
				byteIndex += 3;
			}
		}
		bufferedImage.setRGB(0, 0, w, h, pixelArray, 0, w);
		// bufferedImage.flush();
		repaintImage();
	}
	
	public void fromStringCompressed(String data) throws IOException {
		BufferedReader reader = new BufferedReader(new StringReader(data));
		int w = 0;
		int ch;
		while ((ch = reader.read()) != '#') {
			w = 10 * w + (ch - '0');
		}
		int h = 0;
		while ((ch = reader.read()) != '#') {
			h = 10 * h + (ch - '0');
		}
		if (w != imageWidth || h != imageHeight) {
			this.resize(w, h, /* retain */ true);
		}
		
		int[] pixelArray = new int[w * h];
		int index = 0;
		// StringBuilder sb = new StringBuilder(16);
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				reader.read();   // throw away '#'
//				for (int i = 0; i < 6; i++) {
//					sb.append((char) reader.read());
//				}
//				int rgb = Integer.valueOf(sb.toString(), 16) | 0xff000000;   // restore alpha channel
//				sb.delete(0, 6);
				
				int rgb = 0;
				for (int i = 0; i < 6; i++) {
					int oneByte = reader.read();
					oneByte = ((oneByte >= '0' && oneByte <= '9') ? (oneByte - '0') : (oneByte - 'a' + 10));
					rgb = rgb << 4 | oneByte;
				}
				pixelArray[index] = rgb;
				// bufferedImage.setRGB(x, y, rgb);
				index++;
			}
		}
		reader.close();
		
		bufferedImage.setRGB(0, 0, w, h, pixelArray, 0, w);
		// bufferedImage.flush();
		repaintImage();
	}
	
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
