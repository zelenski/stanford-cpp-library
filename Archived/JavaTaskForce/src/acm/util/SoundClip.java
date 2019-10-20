/*
 * @(#)SoundClip.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY
//
// Class introduced in V1.1

package acm.util;

import java.applet.*;
import java.io.*;
import java.net.*;
import java.text.*;
import javax.sound.sampled.*;

/* Class: SoundClip */
/**
 * This class represents a sound, which can be created from a URL, sound file,
 * resource stream, or digital samples.
 */

public class SoundClip implements AudioClip {

/* Constant: MONO */
/**
 * Number of channels in a mono sound.
 */
	public static final int MONO = 1;

/* Constant: STEREO */
/**
 * Number of channels in a stereo sound.
 */
	public static final int STEREO = 2;

/* Constant: LEFT */
/**
 * Identifies the left channel in a stereo sound.
 */
	public static final int LEFT = 0;

/* Constant: RIGHT */
/**
 * Identifies the right channel in a stereo sound.
 */
	public static final int RIGHT = 1;

/* Constant: STANDARD_FRAME_RATE */
/**
 * Specifies the standard frame rate in a sampled sound.
 */
	public static final double STANDARD_FRAME_RATE = 22050.0;

/* Constant: MAX_SAMPLE */
/**
 * Specifies the maximum value of a sample in a sound.  Samples are signed integers
 * between <code>-MAX_SAMPLE</code> and <code>MAX_SAMPLE</code>.
 */
	public static final int MAX_SAMPLE = 32767;

/* Constructor: SoundClip() */
/**
 * Creates an empty single-channel sound to which samples can be added.
 *
 * @usage SoundClip sound = new SoundClip();
 */
	public SoundClip() {
		this(MONO);
	}

/* Constructor: SoundClip(channels) */
/**
 * Creates an empty sound with the specified number of channels, which
 * must be 1 for mono and 2 for stereo.
 *
 * @usage SoundClip sound = new SoundClip(channels);
 * @param channels The desired number of channels (1 for mono, 2 for stereo)
 */
	public SoundClip(int channels) {
		if (channels < 1 || channels > 2) {
			throw new ErrorException("SoundClip: Illegal number of channels");
		}
		nChannels = channels;
		frameRate = (float) STANDARD_FRAME_RATE;
		soundName = "Untitled";
		frameCount = 0;
		clipVolume = 1.0;
		data = new byte[0];
		format = new AudioFormat(frameRate, 16, channels, true, true);
	}

/* Constructor: SoundClip(name) */
/**
 * Creates a new sound by reading the data from the specified file.
 *
 * @usage SoundClip sound = new SoundClip(name);
 * @param name The file from which the sound is read
 */
	public SoundClip(String name) {
		if (name.startsWith("http:")) {
			try {
				URL url = new URL(name);
				soundName = JTFTools.getURLSuffix(url.getPath());
				readSound(convertToPCM(getAudioInputStream(url.openStream())));
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		} else {
			soundName = name;
			readSound(convertToPCM(getAudioInputStream(new File(name))));
		}
	}

/* Constructor: SoundClip(file) */
/**
 * Creates a new sound by reading the data from the specified file.
 *
 * @usage SoundClip sound = new SoundClip(file);
 * @param file A <code>File</code> object from which the sound is read
 */
	public SoundClip(File file) {
		try {
			soundName = file.getName();
			readSound(convertToPCM(getAudioInputStream(file)));
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Constructor: SoundClip(url) */
/**
 * Creates a new sound by reading the data from the specified URL.
 *
 * @usage SoundClip sound = new SoundClip(url);
 * @param url A network URL containing the sound
 */
	public SoundClip(URL url) {
		try {
			soundName = JTFTools.getURLSuffix(url.getPath());
			readSound(convertToPCM(getAudioInputStream(url.openStream())));
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Constructor: SoundClip(in) */
/**
 * Creates a new sound by reading the data from the specified input stream.
 *
 * @usage SoundClip sound = new SoundClip(in);
 * @param in An <code>InputStream</code> from which the sound is read
 */
	public SoundClip(InputStream in) {
		soundName = "Untitled";
		readSound(convertToPCM(getAudioInputStream(in)));
	}

/* Constructor: SoundClip(samples) */
/**
 * Creates a new mono sound from the sample array.
 *
 * @usage SoundClip sound = new SoundClip(samples);
 * @param samples An array of integers containing the sampled sound
 */
	public SoundClip(int[] samples) {
		this(MONO);
		addSampleData(samples);
	}

/* Constructor: SoundClip(left, right) */
/**
 * Creates a new stereo sound from the two sample arrays.
 *
 * @usage SoundClip sound = new SoundClip(samples);
 * @param left An array of integers containing the samples for the left channel
 * @param right An array of integers containing the samples for the right channel
 */
	public SoundClip(int[] left, int[] right) {
		this(STEREO);
		addSampleData(left, right);
	}

/* Method: play() */
/**
 * Plays the sound through the computer's audio system.
 *
 * @usage sound.play();
 */
	public synchronized void play() {
		if (player == null) player = new SoundPlayer(this);
		player.play();
	}

/* Method: loop() */
/**
 * Plays the sound in a continuous audio loop.
 *
 * @usage sound.loop();
 */
	public void loop() {
		if (player == null) player = new SoundPlayer(this);
		player.loop();
	}

/* Method: stop() */
/**
 * Stops the playback of the sound.  Calling <code>stop</code> saves the
 * current frame index so that calling <code>start</code> will resume from
 * the point at which it stopped.
 *
 * @usage sound.stop();
 */
	public synchronized void stop() {
		if (player != null) player.stop();
	}

/* Method: save(filename) */
/**
 * Writes a data file containing the specified sound.  The format of the sound
 * is determined by the file extension, as follows:
 *
 * <center>
 * <table border=1>
 * <tr><td align=center><code>.au</code></td><td>Sun audio file with ALAW encoding</td></tr>
 * <tr><td align=center><code>.wav</code></td><td>Waveform audio format</td></tr>
 * <tr><td align=center><code>.aif</code> or <code>.aiff</code></td><td>Audio Interchange File Format</td></tr>
 * </table>
 * </center>
 *
 * @usage sound.save(filename);
 * @param filename The name of the file
 */
	public void save(String filename) {
		save(new File(System.getProperty("user.dir"), filename));
	}

/* Method: save(file) */
/**
 * Writes the sound to the specified <code>File</code> object.
 *
 * @usage sound.save(file);
 * @param file The <code>File</code> object to which the sound is written
 */
	public void save(File file) {
		try {
			AudioSystem.write(getAudioInputStream(), getFormatForFile(file.getName()), file);
		} catch (IOException ex) {
			throw new ErrorException("save: I/O error - " + ex.getMessage());
		}
	}

/* Method: getName() */
/**
 * Returns the name of the sound, which is typically the file name from which it
 * was read.
 *
 * @usage String name = sound.getName();
 * @return The name of the sound
 */
	public String getName() {
		return soundName;
	}

/* Method: setName(name) */
/**
 * Sets a name to identify the sound.
 *
 * @usage sound.setName(name);
 * @param name The name to use for the sound
 */
	public void setName(String name) {
		soundName = name;
	}

/* Method: getChannelCount() */
/**
 * Returns the number of channels.
 *
 * @usage int channels = sound.getChannelCount();
 * @return The number of channels (1 for mono, 2 for stereo)
 */
	public int getChannelCount() {
		return nChannels;
	}

/* Method: isStereo() */
/**
 * Returns <code>true</code> if the sound is recorded in stereo.
 *
 * @usage if (sound.isStereo()) . . .
 * @return <code>true</code> if the sound is recorded in stereo
 */
	public boolean isStereo() {
		return nChannels == STEREO;
	}

/* Method: getFrameCount() */
/**
 * Returns the number of frames in a sound.
 *
 * @usage int nFrames = sound.getFrameCount();
 * @return The number of frames in a sound
 */
	public int getFrameCount() {
		return frameCount;
	}

/* Method: getFrameRate() */
/**
 * Returns the frame rate of the sound.
 *
 * @usage double frameRate = sound.getFrameRate();
 * @return The frame rate of the sound (in frames/second)
 */
	public double getFrameRate() {
		return frameRate;
	}

/* Method: getDuration() */
/**
 * Returns the duration of a sound (in seconds).
 *
 * @usage double duration = sound.getDuration();
 * @return The duration of a sound (in seconds)
 */
	public double getDuration() {
		return frameCount / frameRate;
	}

/* Method: getFrameIndex() */
/**
 * Returns the current frame index in the sound.
 *
 * @usage int frameIndex = sound.getFrameIndex();
 * @return The current frame index in the sound
 */
	public int getFrameIndex() {
		return currentFrame;
	}

/* Method: setFrameIndex(frame) */
/**
 * Sets the current frame index.
 *
 * @usage sound.setFrameIndex(frameIndex);
 * @param frameIndex The current frame index in the sound
 */
	public void setFrameIndex(int frameIndex) {
		currentFrame = frameIndex;
	}

/* Method: rewind() */
/**
 * Rewinds the sound to the beginning.  This method is useful after you have
 * stopped a sound and want to replay it from the beginning.
 *
 * @usage sound.rewind();
 */
	public void rewind() {
		setFrameIndex(0);
	}

/* Method: getVolume() */
/**
 * Returns the overall volume setting for the sound, which is a number
 * between 0 (silent) and 1 (maximum volume).
 *
 * @usage double volume = sound.getVolume();
 * @return The overall volume setting for the sound
 */
	public double getVolume() {
		return clipVolume;
	}

/* Method: setVolume(volume) */
/**
 * Sets the overall volume setting for the sound, which is a number
 * between 0 (silent) and 1 (maximum volume).
 *
 * @usage sound.setVolume(volume);
 * @param volume The new overall volume setting for the sound
 */
	public void setVolume(double volume) {
		clipVolume = volume;
	}

/* Method: getSampleData() */
/**
 * Returns an array of integers corresponding to the samples in the primary
 * channel of the sound.  For mono sounds, this channel is the only channel;
 * for stereo sounds, it is defined to be the left channel.
 *
 * @usage int[] samples = sound.getSampleData();
 * @return An array of the samples in the sound
 */
	public int[] getSampleData() {
		return getSampleData(0);
	}

/* Method: getSampleData(channel) */
/**
 * Returns an array of integers corresponding to the samples in the specified
 * channel, which is typically represented by <code>LEFT</code> or <code>RIGHT</code>.
 *
 * @usage int[] samples = sound.getSampleData(channel);
 * @param channel The channel for which samples are requested
 * @return An array of the samples in the sound
 */
	public int[] getSampleData(int channel) {
		standardize();
		if (channel < 0 || channel >= nChannels) {
			throw new ErrorException("getSamples: Channel number out of range");
		}
		int[] samples = new int[frameCount];
		int ix = channel * BYTES_PER_SAMPLE;
		for (int i = 0; i < frameCount; i++) {
			samples[i] = data[ix] << 8 | (data[ix + 1] & 0xFF);
			ix += nChannels * BYTES_PER_SAMPLE;
		}
		return samples;
	}

/* Method: addSampleData(sample) */
/**
 * Adds a single sound sample to the end of the sound.  If the sound is stereo,
 * the new sample is added to both the left and right channel.
 *
 * @usage sound.addSampleData(sample);
 * @param sample An integers representing a new sound samples
 */
	public void addSampleData(int sample) {
		standardize();
		if (nChannels == 2) {
			addSampleData(sample, sample);
		} else {
			int ix = frameCount * BYTES_PER_SAMPLE;
			frameCount++;
			if (ix >= data.length) {
				byte[] newData = new byte[ix + BUFFER_INCREMENT];
				if (ix > 0) {
					System.arraycopy(data, 0, newData, 0, ix);
				}
				data = newData;
			}
			data[ix++] = (byte) (sample >> 8);
			data[ix++] = (byte) (sample & 0xFF);
		}
	}

/* Method: addSampleData(left, right) */
/**
 * Adds the specified samples to the end of the appropriate channels of
 * the stereo sound.
 *
 * @usage sound.addSampleData(left, right);
 * @param left An integer containing a single sample for the left channel
 * @param right An integer containing a single sample for the right channel
 */
	public void addSampleData(int left, int right) {
		standardize();
		if (nChannels != 2) {
			throw new ErrorException("addSampleData: Sound is not stereo");
		} else {
			int ix = 2 * frameCount * BYTES_PER_SAMPLE;
			frameCount++;
			if (ix >= data.length) {
				byte[] newData = new byte[ix + BUFFER_INCREMENT];
				if (ix > 0) {
					System.arraycopy(data, 0, newData, 0, ix);
				}
				data = newData;
			}
			data[ix++] = (byte) (left >> 8);
			data[ix++] = (byte) (left & 0xFF);
			data[ix++] = (byte) (right >> 8);
			data[ix++] = (byte) (right & 0xFF);
		}
	}

/* Method: addSampleData(samples) */
/**
 * Adds the specified samples to the end of the sound.  If the sound is stereo,
 * the new samples are added to both the left and right channel.
 *
 * @usage sound.addSampleData(samples);
 * @param samples An array of integers containing the new sound samples
 */
	public void addSampleData(int[] samples) {
		standardize();
		if (nChannels == 2) {
			addSampleData(samples, samples);
		} else {
			int ix = frameCount * BYTES_PER_SAMPLE;
			frameCount += samples.length;
			byte[] newData = new byte[frameCount * BYTES_PER_SAMPLE];
			if (ix > 0) {
				System.arraycopy(data, 0, newData, 0, ix);
			}
			data = newData;
			for (int i = 0; i < samples.length; i++) {
				data[ix++] = (byte) (samples[i] >> 8);
				data[ix++] = (byte) (samples[i] & 0xFF);
			}
		}
	}

/* Method: addSampleData(left, right) */
/**
 * Adds the specified samples to the end of the appropriate channels of
 * the stereo sound.
 *
 * @usage sound.addSampleData(left, right);
 * @param left An array of integers containing the samples for the left channel
 * @param right An array of integers containing the samples for the right channel
 */
	public void addSampleData(int[] left, int[] right) {
		standardize();
		if (nChannels != 2) {
			throw new ErrorException("addSampleData: Sound is not stereo");
		} else if (left.length != right.length) {
			throw new ErrorException("addSampleData: Channels have unequal length");
		} else {
			int ix = 2 * frameCount * BYTES_PER_SAMPLE;
			frameCount += left.length;
			byte[] newData = new byte[2 * frameCount * BYTES_PER_SAMPLE];
			if (ix > 0) {
				System.arraycopy(data, 0, newData, 0, ix);
			}
			data = newData;
			for (int i = 0; i < left.length; i++) {
				data[ix++] = (byte) (left[i] >> 8);
				data[ix++] = (byte) (left[i] & 0xFF);
				data[ix++] = (byte) (right[i] >> 8);
				data[ix++] = (byte) (right[i] & 0xFF);
			}
		}
	}

/* Method: toString() */
/**
 * Converts a sound to a string.
 *
 * @usage String str = sound.toString();
 * @return A string representation of the sound
 */
	public String toString() {
		String str = soundName;
		str += " (";
		str += (nChannels == 1) ? "mono" : "stereo";
		str += ", ";
		str += new DecimalFormat("#0.00").format(getDuration());
		str += " sec)";
		return str;
	}

/**********************************************************************/
/* Static methods                                                     */
/**********************************************************************/

/* Static method: sampleToIntensity(sample) */
/**
 * Converts an integer sample in the range -32767 and +32767 into a
 * <code>double</code> between -1.0 and +1.0.
 *
 * @usage double intensity = SoundClip.sampleToIntensity(sample)
 * @param sample An integer sample from a digital sound
 * @return The corresponding intensity value between -1.0 and +1.0
 */
	public static double sampleToIntensity(int sample) {
		return Math.max(-1.0, Math.min(+1.0, (double) sample / MAX_SAMPLE));
	}

/* Static method: intensityToSample(intensity) */
/**
 * Converts an intensity value between -1.0 and +1.0 into the corresponding
 * integer sample in the range -32767 and +32767.
 *
 * @usage int sample = SoundClip.intensityToSample(intensity)
 * @param intensity An intensity value between -1.0 and +1.0
 * @return The corresponding integer sample between -32767 and +32767
 */
	public static int intensityToSample(double intensity) {
		return (int) Math.round(Math.max(-1.0, Math.min(+1.0, intensity)) * MAX_SAMPLE);
	}

/**********************************************************************/
/* Protected methods                                                  */
/**********************************************************************/

/* Protected method: getData() */
/**
 * Returns the byte array containing the sound data.
 */
	protected byte[] getData() {
		return data;
	}

/* Protected method: getFormat() */
/**
 * Returns the format in which the sound is stored.
 */
	protected AudioFormat getFormat() {
		return format;
	}

/* Protected method: getAudioInputStream() */
/**
 * Returns an <code>AudioInputStream</code> for the sound.
 */
	protected AudioInputStream getAudioInputStream() {
		return new AudioInputStream(new ByteArrayInputStream(data), format, frameCount);
	}

/**********************************************************************/
/* Private methods                                                    */
/**********************************************************************/

/* Private method: standardize() */
/**
 * Converts the data for this sound so that it has the standard sampling rate.
 */
	private void standardize() {
		if (frameRate == STANDARD_FRAME_RATE) return;
		double sf = STANDARD_FRAME_RATE / frameRate;
		int newFrameCount = (int) (frameCount * sf);
		byte[] newData = new byte[nChannels * newFrameCount * BYTES_PER_SAMPLE];
		int ix = 0;
		for (int i = 0; i < newFrameCount; i++) {
			// Add averaging at some point in the future
			for (int channel = 0; channel < nChannels; channel++) {
				int sx = (int) (i / sf) * BYTES_PER_SAMPLE * nChannels + BYTES_PER_SAMPLE * channel;
				int sample = (data[sx] << 8) | (data[sx + 1] & 0xFF);
				newData[ix++] = (byte) (sample >> 8);
				newData[ix++] = (byte) (sample & 0xFF);
			}
		}
		data = newData;
		frameCount = newFrameCount;
		format = new AudioFormat(frameRate, 16, nChannels, true, true);
	}

/* Private method: getFormatForFile(name) */
/**
 * Returns an audio format appropriate to the file name.
 */
	private AudioFileFormat.Type getFormatForFile(String name) {
		name = name.toLowerCase();
		if (name.endsWith(".wav")) return AudioFileFormat.Type.WAVE;
		if (name.endsWith(".aif")) return AudioFileFormat.Type.AIFF;
		if (name.endsWith(".aiff")) return AudioFileFormat.Type.AIFF;
		return AudioFileFormat.Type.AU;
	}

/* Private method: readSound(in) */
/**
 * Reads in the data for this sound from the specified <code>AudioInputStream</code>.
 */
	private void readSound(AudioInputStream in) {
		format = in.getFormat();
		nChannels = format.getChannels();
		frameRate = format.getFrameRate();
		long frameLength = in.getFrameLength();
		if (frameLength > Integer.MAX_VALUE) {
			throw new ErrorException("SoundClip: Sound file is too large");
		}
		frameCount = (int) frameLength;
		if (frameCount < 0) {
			data = new byte[BUFFER_INCREMENT];
		} else {
			data = new byte[frameCount * nChannels * 2];
		}
		int offset = 0;
		boolean eof = false;
		while (!eof) {
			int bytesRemaining = data.length - offset;
			while (bytesRemaining > 0) {
				try {
					int nBytes = in.read(data, offset, bytesRemaining);
					if (nBytes == 0) {
						eof = true;
						break;
					}
					offset += nBytes;
					bytesRemaining -= nBytes;
				} catch (IOException ex) {
					throw new ErrorException(ex);
				}
			}
			if (frameCount >= 0) eof = true;
			if (!eof) {
				byte[] ndata = new byte[2 * data.length];
				System.arraycopy(data, 0, ndata, 0, data.length);
				data = ndata;
			}
		}
		if (offset < data.length && frameCount < 0) {
			byte[] ndata = new byte[offset];
			System.arraycopy(data, 0, ndata, 0, offset);
			data = ndata;
		}
		if (frameCount < 0) frameCount = offset / (2 * nChannels);
	}

/* Private method: convertToPCM(in) */
/**
 * Converts an <code>AudioInputStream</code> to a 16-bit linear PCM format.
 */
	private AudioInputStream convertToPCM(AudioInputStream in) {
		AudioFormat newFormat = in.getFormat();
		newFormat = new AudioFormat(newFormat.getSampleRate(), 16, newFormat.getChannels(), true, true);
		return AudioSystem.getAudioInputStream(newFormat, in);
	}

/* Private method: getAudioInputStream(in) */
/**
 * Creates an <code>AudioInputStream</code> from an input stream.
 */
	private AudioInputStream getAudioInputStream(InputStream in) {
		try {
			if (!(in instanceof BufferedInputStream)) {
				in = new BufferedInputStream(in);
			}
			return AudioSystem.getAudioInputStream(in);
		} catch (UnsupportedAudioFileException ex) {
			throw new ErrorException(createUnsupportedFormatMessage());
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: getAudioInputStream(file) */
/**
 * Creates an <code>AudioInputStream</code> from a file.
 */
	private AudioInputStream getAudioInputStream(File file) {
		try {
			return AudioSystem.getAudioInputStream(file);
		} catch (UnsupportedAudioFileException ex) {
			throw new ErrorException(createUnsupportedFormatMessage());
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: createUnsupportedFormatMessage() */
/**
 * Generates a more useful error message.
 */
	private String createUnsupportedFormatMessage() {
		int dot = soundName.lastIndexOf('.');
		if (dot == -1) return "Unsupported audio file format";
		return "The " + soundName.substring(dot + 1) + " format is not supported by JavaSound";
	}

/* Private constants */
	private static final int BYTES_PER_SAMPLE = 2;
	private static final int BUFFER_INCREMENT = 10000;

/* Private instance variables */
	private AudioFormat format;
	private SoundPlayer player;
	private String soundName;
	private double clipVolume;
	private float frameRate;
	private int nChannels;
	private int frameCount;
	private int currentFrame;
	private byte[] data;

}

/* Package class: SoundPlayer */
/**
 * This class implements the actual playing of a sound.  This implementation
 * uses the <code>SourceDataLine</code> model rather than the <code>Clip</code>
 * model to avoid problems with noise when a sound is stopped.
 */
class SoundPlayer implements Runnable {

/* Constructor: SoundPlayer(sound) */
/**
 * Creates a new player for the specified sound.
 */
	public SoundPlayer(SoundClip sound) {
		soundClip = sound;
		oldVolume = 1.0;
	}

/* Method: play() */
/**
 * Plays the sound once.
 */
	public void play() {
		if (thread != null) return;
		looping = false;
		thread = new Thread(this);
		thread.start();
	}

/* Method: loop() */
/**
 * Plays the sound in a continuous loop.
 */
	public void loop() {
		if (thread != null) return;
		looping = true;
		thread = new Thread(this);
		thread.start();
	}

/* Method: stop() */
/**
 * Stops the sound at the end of the next buffer of data.  This implementation
 * avoid the use of the <code>stop</code> method on the <code>SourceDataLine</code>,
 * which generates unacceptable noise on certain sound engines.
 */
	public void stop() {
		if (thread == null) return;
		looping = false;
		thread = null;
	}

/* Method: run() */
/**
 * This method is executed when the <code>SoundPlayer</code> thread is started.
 * It writes data to the <code>SourceDataLine</code> in units of a relatively
 * small buffer, checking between each write to ensure that the thread is not
 * <code>null</code>, which is used as the signal to stop playing.  At the end
 * of the sound, the implementation writes <code>PADDING</code> bytes of zero
 * data to the mixer before stopping.  This strategy eliminates the noise
 * otherwise associated with stopping on certain sound engines.
 */
	public void run() {
		byte[] data = soundClip.getData();
		int nChannels = soundClip.getChannelCount();
		int offset = soundClip.getFrameIndex() * nChannels * BYTES_PER_SAMPLE;
		int length = soundClip.getFrameCount() * nChannels * BYTES_PER_SAMPLE;
		SourceDataLine line = openSourceDataLine(Math.min(MAX_BUFFER_SIZE, length));
		line.start();
		while (thread != null) {
			int bytesRemaining = length - offset;
			while (thread != null && bytesRemaining > 0) {
				setLineVolume(line, soundClip.getVolume());
				int chunkSize = Math.min(MAX_BUFFER_SIZE, bytesRemaining);
				int nBytes = line.write(data, offset, chunkSize);
				offset += nBytes;
				bytesRemaining -= nBytes;
			}
			if (!looping) break;
			offset = 0;
		}
		if (thread == null) {
			soundClip.setFrameIndex(offset / nChannels / BYTES_PER_SAMPLE);
		}
		data = new byte[PADDING];
		line.write(data, 0, data.length);
		line.drain();
		line.stop();
		line.close();
		thread = null;
	}

/* Private method: openSourceDataLine(bufferSize) */
/**
 * Opens a new <code>SourceDataLine</code> for this sound.
 */
	private SourceDataLine openSourceDataLine(int bufferSize) {
		try {
			AudioFormat format = soundClip.getFormat();
			Class<?> sourceDataLineClass = Class.forName("javax.sound.sampled.SourceDataLine");
			DataLine.Info info = new DataLine.Info(sourceDataLineClass, format);
			if (!AudioSystem.isLineSupported(info)) {
				throw new ErrorException("SoundClip: Unsupported data line format");
			}
			SourceDataLine line = (SourceDataLine) AudioSystem.getLine(info);
			line.open(format, bufferSize);
			return line;
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

	private void setLineVolume(SourceDataLine line, double volume) {
		if (oldVolume == volume) return;
		oldVolume = volume;
		if (line.isControlSupported(FloatControl.Type.VOLUME)) {
			FloatControl volumeControl = (FloatControl) line.getControl(FloatControl.Type.VOLUME);
			volumeControl.setValue((float) volume);
		} else if (line.isControlSupported(FloatControl.Type.MASTER_GAIN)) {
			FloatControl gainControl = (FloatControl) line.getControl(FloatControl.Type.MASTER_GAIN);
			double gain = 20 * Math.log(Math.max(volume, EPSILON)) / Math.log(10.0);
			gainControl.setValue((float) gain);
		}
	}

/* Private constants */
	private static final int PADDING = 400;
	private static final int BYTES_PER_SAMPLE = 2;
	private static final int MAX_BUFFER_SIZE = 4096;
	private static final double EPSILON = 0.000001;

/* Private instance variables */
	private SoundClip soundClip;
	private Thread thread;
	private boolean looping;
	private double oldVolume;

}
