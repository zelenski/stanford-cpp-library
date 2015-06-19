package stanford.cs106.audio;

import java.applet.*;
import java.io.*;
import java.util.*;

import javax.sound.sampled.*;

import javazoom.jlgui.basicplayer.*;

public class SplClip implements AudioClip, BasicPlayerListener, Runnable {
	private static final Set<String> SUPPORTED_EXTENSIONS = new TreeSet<String>(
			Arrays.asList(
			// "aif", "aifc", "aiff", "au", "snd", 
			"mp3", "ogg", "ogs",
			// "wma", 
			"wav", "wave"));
	
	public static boolean supportsExtension(String extension) {
		extension = String.valueOf(extension).toLowerCase();
		return SUPPORTED_EXTENSIONS.contains(extension)
				|| SUPPORTED_EXTENSIONS.contains("." + extension);
	}
	
	public static boolean supportsFile(String name) {
		return supportsExtension(getExtension(name));
	}
	
	public static String getExtension(String name) {
		String extension = (name.lastIndexOf(".") >= 0 ? name.substring(name.lastIndexOf(".") + 1) : name).toLowerCase();
		return extension;
	}
	
	private String filename;
	private BasicController control;
	private Clip clip;
	private Thread thread;
	private boolean loop;
	
	public SplClip(String filename) {
		this.filename = filename;
	}
	
	public void loop() {
		// TODO make it support multiple plays (rewind and play again)
		play(/* loop */ true);
	}

	public void play() {
		play(/* loop */ false);
	}
	
	private void play(boolean loop) {
		this.loop = loop;
		String extension = getExtension(filename);
			if (extension.equals("mp3")) {
				// special case for playing MP3 files
				thread = new Thread(this);
				thread.start();
			} else {
				try {
					// play au/wav files using built-in Java audio libraries
					File soundFile = new File(filename);
					AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(soundFile);
					AudioFormat format = audioInputStream.getFormat();
					DataLine.Info info = new DataLine.Info(Clip.class, format);
					clip = (Clip) AudioSystem.getLine(info);
					clip.open(audioInputStream);
					if (loop) {
						clip.loop(Clip.LOOP_CONTINUOUSLY);
					}
					clip.start();
				} catch (IOException ioe) {
					throw new IllegalArgumentException(ioe);
				} catch (LineUnavailableException lue) {
					throw new IllegalArgumentException(lue);
				} catch (UnsupportedAudioFileException uafe) {
					throw new IllegalArgumentException(uafe);
				}
			}
	}
	
	public void run() {
		String extension = getExtension(filename);
		try {
			if (extension.equals("mp3")) {
				do {
					// special case for playing MP3 files
					AudioInputStream in = AudioSystem.getAudioInputStream(new File(filename));
					AudioInputStream din = null;
					AudioFormat baseFormat = in.getFormat();
					AudioFormat decodedFormat = new AudioFormat(
							AudioFormat.Encoding.PCM_SIGNED,
							baseFormat.getSampleRate(), 16, baseFormat.getChannels(),
							baseFormat.getChannels() * 2, baseFormat.getSampleRate(),
							false);
					din = AudioSystem.getAudioInputStream(decodedFormat, in);
					// Play now.
					rawplay(decodedFormat, din);
					in.close();
				} while (loop);
			} else {
				// play au/wav files using built-in Java audio libraries
				File soundFile = new File(filename);
				AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(soundFile);
				AudioFormat format = audioInputStream.getFormat();
				DataLine.Info info = new DataLine.Info(Clip.class, format);
				clip = (Clip) AudioSystem.getLine(info);
				clip.open(audioInputStream);
				if (loop) {
					clip.loop(Clip.LOOP_CONTINUOUSLY);
				}
				clip.start();
			}
		} catch (IOException ioe) {
			throw new IllegalArgumentException(ioe);
		} catch (LineUnavailableException lue) {
			throw new IllegalArgumentException(lue);
		} catch (UnsupportedAudioFileException uafe) {
			throw new IllegalArgumentException(uafe);
		}
	}

	@SuppressWarnings("deprecation")
	public void stop() {
		if (control != null) {
			control.stop();
			control = null;
		}
		if (clip != null) {
			clip.stop();
		}
		if (thread != null) {
			thread.stop();
			thread = null;
		}
	}
	
	public void waitUntilDonePlaying() {
		if (clip != null) {
			if (!clip.isRunning()) {
				while (!clip.isRunning()) {
					try {
						Thread.sleep(50);
					} catch (InterruptedException ie) {
						// empty
					}
				}
			}
			while (clip.isRunning()) {
				try {
					Thread.sleep(50);
				} catch (InterruptedException ie) {
					// empty
				}
			}
		}
	}

	@SuppressWarnings("rawtypes")
	public void opened(Object stream, Map properties) {
		// TODO Auto-generated method stub
		
	}

	@SuppressWarnings("rawtypes")
	public void progress(int bytesread, long microseconds, byte[] pcmdata,
			Map properties) {
		// TODO Auto-generated method stub
		
	}

	public void stateUpdated(BasicPlayerEvent event) {
		// TODO Auto-generated method stub
		
	}

	public void setController(BasicController controller) {
		// TODO Auto-generated method stub
		
	}

	private void rawplay(AudioFormat targetFormat, AudioInputStream din) throws LineUnavailableException, IOException {
		byte[] data = new byte[65536];
		SourceDataLine line = getLine(targetFormat);
		if (line != null) {
			// Start
			line.start();
			int nBytesRead = 0;
			while (nBytesRead != -1) {
				nBytesRead = din.read(data, 0, data.length);
				if (nBytesRead != -1)
					line.write(data, 0, nBytesRead);
			}
			// Stop
			line.drain();
			line.stop();
			line.close();
			din.close();
		}
	}

	private SourceDataLine getLine(AudioFormat audioFormat)
			throws LineUnavailableException {
		SourceDataLine res = null;
		DataLine.Info info = new DataLine.Info(SourceDataLine.class,
				audioFormat);
		res = (SourceDataLine) AudioSystem.getLine(info);
		res.open(audioFormat);
		return res;
	}
}
