/*
 * CS 106A Image Algorithms
 *
 * This instructor-provided file implements the underlying audio system used
 * by your Melody Maker program's Note class.
 *
 * @author see below; modifications made by Marty Stepp
 * @version Tue 2014/05/25
 * - added stuff to facilitate autograders
 * @version Tue 2014/05/11
 * - initial version for 14sp
 *
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

package stanford.cs106.audio;

/*************************************************************************
 * A simple library for reading, writing, and manipulating .wav files
 * and playing raw audio data.
 *
 * Limitations:
 *   - Does not seem to work properly when reading .wav files from a .jar file.
 *   - Assumes the audio is monaural, with sampling rate of 44,100.
 *************************************************************************/

import java.applet.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.sound.sampled.*;

/**
 * <i>Standard audio</i>. This class provides a basic capability for creating,
 * reading, and saving audio.
 * <p>
 * The audio format uses a sampling rate of 44,100 (CD quality audio), 16-bit,
 * monaural.
 * 
 * <p>
 * For additional documentation, see <a
 * href="http://introcs.cs.princeton.edu/15inout">Section 1.5</a> of
 * <i>Introduction to Programming in Java: An Interdisciplinary Approach</i> by
 * Robert Sedgewick and Kevin Wayne.
 */
public final class StdAudio {
	/**
	 * The sample rate - 44,100 Hz for CD quality audio.
	 */
	public static final int SAMPLE_RATE = 44100;

	private static final int BYTES_PER_SAMPLE = 2; // 16-bit audio
	private static final int BITS_PER_SAMPLE = 16; // 16-bit audio
	private static final double MAX_16_BIT = Short.MAX_VALUE; // 32,767
	private static final int SAMPLE_BUFFER_SIZE = 4096;

	private static SourceDataLine line; // to play the sound
	private static byte[] buffer; // our internal buffer
	private static int bufferSize = 0; // number of samples currently in
										// internal buffer
	private static boolean muted = false;
	private static boolean paused = false;
	private static Set<AudioEventListener> listeners;

	// static initializer
	static {
		init();
	}
	
	public static class AudioEvent {
		public static enum Type { PLAY, LOOP, PAUSE, UNPAUSE, STOP, MUTE, UNMUTE }
		
		private Type type;
		private Note note;
		private double duration;
		
		public AudioEvent(Type type) {
			this(type, 0.0);
		}
		
		public AudioEvent(Type type, double duration) {
			this.type = type;
			this.duration = duration;
		}
		
		public AudioEvent(Type type, Note note, double duration) {
			this.type = type;
			this.note = note;
			this.duration = duration;
		}
		
		public double getDuration() {
			return duration;
		}
		
		public Note getNote() {
			return note;
		}
		
		public Type getType() {
			return type;
		}
		
		public String toString() {
			return "AudioEvent{Type=" + type
					+ (note == null ? "" : (", note=" + note))
					+ (duration == 0.0 ? "" : (", duration=" + duration)) + "}";
		}
	}
	
	public static interface AudioEventListener {
		void onAudioEvent(AudioEvent event);
	}
	
	public static void addAudioEventListener(AudioEventListener listener) {
		listeners.add(listener);
	}
	
	// open up an audio stream
	private static void init() {
		try {
			// 44,100 samples per second, 16-bit audio, mono, signed PCM, little
			// Endian
			AudioFormat format = new AudioFormat((float) SAMPLE_RATE, BITS_PER_SAMPLE, 1, true,
					false);
			DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);

			line = (SourceDataLine) AudioSystem.getLine(info);
			line.open(format, SAMPLE_BUFFER_SIZE * BYTES_PER_SAMPLE);

			// the internal buffer is a fraction of the actual buffer size, this
			// choice is arbitrary
			// it gets divided because we can't expect the buffered data to line
			// up exactly with when
			// the sound card decides to push out its samples.
			buffer = new byte[SAMPLE_BUFFER_SIZE * BYTES_PER_SAMPLE / 3];
			listeners = new HashSet<AudioEventListener>();
		} catch (Exception e) {
			System.err.println("Error initializing StdAudio audio system:");
			e.printStackTrace();
			System.exit(1);
		}

		// no sound gets made before this call
		line.start();
	}
	
	/**
	 * Removes all audio event listeners from being notified of future
	 * audio events, if any were present.  If none were present, has no effect.
	 */
	public static void clearAudioEventListeners() {
		listeners.clear();
	}
	
	/**
	 * Close standard audio.
	 */
	public static void close() {
		line.drain();
		line.stop();
	}

	/**
	 * Returns whether the audio system is currently muted.
	 */
	public static boolean isMuted() {
		return muted;
	}

	/**
	 * Returns whether the audio system is currently paused.
	 */
	public static boolean isPaused() {
		return paused;
	}

	/**
	 * Loop a sound file (in .wav or .au format) in a background thread.
	 */
	public static void loop(String filename) {
		if (muted) {
			return;
		}
		URL url = null;
		try {
			File file = new File(filename);
			if (file.canRead())
				url = file.toURI().toURL();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
		// URL url = StdAudio.class.getResource(filename);
		if (url == null)
			throw new RuntimeException("audio " + filename + " not found");
		AudioClip clip = Applet.newAudioClip(url);
		clip.loop();
		notifyListeners(new AudioEvent(AudioEvent.Type.LOOP));
	}

	/**
	 * Create a note (sine wave) of the given frequency (Hz), for the given
	 * duration (seconds) scaled to the given volume (amplitude).
	 */
	public static double[] note(double hz, double duration, double amplitude) {
		int N = (int) (StdAudio.SAMPLE_RATE * duration);
		double[] a = new double[N + 1];
		for (int i = 0; i <= N; i++)
			a[i] = amplitude * Math.sin(2 * Math.PI * i * hz / StdAudio.SAMPLE_RATE);
		return a;
	}

	/**
	 * Write one sample (between -1.0 and +1.0) to standard audio. If the sample
	 * is outside the range, it will be clipped.
	 */
	public static void play(double in) {
		if (muted) {
			return;
		}

		// clip if outside [-1, +1]
		if (in < -1.0) {
			in = -1.0;
		}
		if (in > +1.0) {
			in = +1.0;
		}

		// convert to bytes
		short s = (short) (MAX_16_BIT * in);
		buffer[bufferSize++] = (byte) s;
		buffer[bufferSize++] = (byte) (s >> 8); // little Endian

		// send to sound card if buffer is full
		if (bufferSize >= buffer.length) {
			line.write(buffer, 0, buffer.length);
			bufferSize = 0;
		}
	}

	/**
	 * Write an array of samples (between -1.0 and +1.0) to standard audio. If a
	 * sample is outside the range, it will be clipped.
	 */
	public static void play(double[] input) {
		prePlay();
		for (int i = 0; i < input.length; i++) {
			play(input[i]);
		}
	}

	/**
	 * Write an array of samples (between -1.0 and +1.0) to standard audio. If a
	 * sample is outside the range, it will be clipped.
	 */
	public static void play(double[] input, double duration) {
		play(input);
		notifyListeners(new AudioEvent(AudioEvent.Type.PLAY, duration));
	}

	/**
	 * Write an array of samples (between -1.0 and +1.0) to standard audio. If a
	 * sample is outside the range, it will be clipped.
	 */
	public static void play(Note note, double[] input, double duration) {
		play(input);
		notifyListeners(new AudioEvent(AudioEvent.Type.PLAY, note, duration));
	}

	/**
	 * Play a sound file (in .wav or .au format) in a background thread.
	 */
	public static void play(String filename) {
		prePlay();
		URL url = null;
		try {
			File file = new File(filename);
			if (file.canRead())
				url = file.toURI().toURL();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
		// URL url = StdAudio.class.getResource(filename);
		if (url == null)
			throw new RuntimeException("audio " + filename + " not found");
		AudioClip clip = Applet.newAudioClip(url);
		clip.play();
	}

	/**
	 * Read audio samples from a file (in .wav or .au format) and return them as
	 * a double array with values between -1.0 and +1.0.
	 */
	public static double[] read(String filename) {
		byte[] data = readByte(filename);
		int N = data.length;
		double[] d = new double[N / 2];
		for (int i = 0; i < N / 2; i++) {
			d[i] = ((short) (((data[2 * i + 1] & 0xFF) << 8) + (data[2 * i] & 0xFF)))
					/ ((double) MAX_16_BIT);
		}
		return d;
	}

	/**
	 * Removes the given audio event listener from being notified of future
	 * audio events, if it was present.  If not present, has no effect.
	 */
	public static void removeAudioEventListener(AudioEventListener listener) {
		listeners.remove(listener);
	}
	
	/**
	 * Save the double array as a sound file (using .wav or .au format).
	 */
	public static void save(String filename, double[] input) {
		// assumes 44,100 samples per second
		// use 16-bit audio, mono, signed PCM, little Endian
		AudioFormat format = new AudioFormat(SAMPLE_RATE, 16, 1, true, false);
		byte[] data = new byte[2 * input.length];
		for (int i = 0; i < input.length; i++) {
			int temp = (short) (input[i] * MAX_16_BIT);
			data[2 * i + 0] = (byte) temp;
			data[2 * i + 1] = (byte) (temp >> 8);
		}

		// now save the file
		try {
			ByteArrayInputStream bais = new ByteArrayInputStream(data);
			AudioInputStream ais = new AudioInputStream(bais, format, input.length);
			if (filename.endsWith(".wav") || filename.endsWith(".WAV")) {
				AudioSystem.write(ais, AudioFileFormat.Type.WAVE, new File(filename));
			} else if (filename.endsWith(".au") || filename.endsWith(".AU")) {
				AudioSystem.write(ais, AudioFileFormat.Type.AU, new File(filename));
			} else {
				throw new RuntimeException("File format not supported: " + filename);
			}
		} catch (Exception e) {
			System.out.println(e);
			System.exit(1);
		}
	}

	/**
	 * Sets whether the audio system is muted.
	 * If audio is muted, notes do not play and playing methods return immediately.
	 */
	public static void setMute(boolean mute) {
		muted = mute;
		notifyListeners(new AudioEvent(mute ? AudioEvent.Type.MUTE : AudioEvent.Type.UNMUTE));
	}

	/**
	 * Sets whether the audio system is paused.
	 * If audio is paused, playing methods "block" in an infinite while loop.
	 */
	public static void setPaused(boolean pause) {
		paused = pause;
		notifyListeners(new AudioEvent(pause ? AudioEvent.Type.PAUSE : AudioEvent.Type.UNPAUSE));
	}

	/*
	 * Informs all added audio event listeners of the given event.
	 */
	private static void notifyListeners(AudioEvent event) {
		for (AudioEventListener listener : listeners) {
			listener.onAudioEvent(event);
		}
	}

	/*
	 * Maintenance to be done before playing; pause/mute management.
	 */
	private static void prePlay() {
		if (muted) {
			return;
		}
		while (paused) {
			try {
				Thread.sleep(50);
			} catch (InterruptedException ie) {
				// empty
			}
		}
	}

	/*
	 * Return data as a byte array.
	 */
	private static byte[] readByte(String filename) {
		byte[] data = null;
		AudioInputStream ais = null;
		try {

			// try to read from file
			File file = new File(filename);
			if (file.exists()) {
				ais = AudioSystem.getAudioInputStream(file);
				data = new byte[ais.available()];
				ais.read(data);
			}

			// try to read from URL
			else {
				URL url = StdAudio.class.getResource(filename);
				ais = AudioSystem.getAudioInputStream(url);
				data = new byte[ais.available()];
				ais.read(data);
			}
		} catch (Exception e) {
			System.out.println(e.getMessage());
			throw new RuntimeException("Could not read " + filename);
		}

		return data;
	}

	/**
	 * Test client - play an A major scale to standard audio.
	 */
	public static void main(String[] args) {
		// 440 Hz for 1 sec
		double freq = 440.0;
		for (int i = 0; i <= StdAudio.SAMPLE_RATE; i++) {
			StdAudio.play(0.5 * Math.sin(2 * Math.PI * freq * i / StdAudio.SAMPLE_RATE));
		}

		// scale increments
		int[] steps = { 0, 2, 4, 5, 7, 9, 11, 12 };
		for (int i = 0; i < steps.length; i++) {
			double hz = 440.0 * Math.pow(2, steps[i] / 12.0);
			StdAudio.play(note(hz, 1.0, 0.5));
		}

		// need to call this in non-interactive stuff so the program doesn't
		// terminate
		// until all the sound leaves the speaker.
		StdAudio.close();

		// need to terminate a Java program with sound
		System.exit(0);
	}

	/*
	 * This private constructor ensures that StdAudio is non-instantiable.
	 */
	private StdAudio() {
		// empty
	}
}
