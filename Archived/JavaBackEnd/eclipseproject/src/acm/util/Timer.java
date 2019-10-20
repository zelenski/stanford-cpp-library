/*
 * @version 2017/10/22
 * - initial version
 */

package acm.util;

/**
 * This class exports is useful for measuring the elapsed
 * time of a program in milliseconds over a given interval.
 */
public class Timer {
	// instance variables
	private long _startMS;
	private long _stopMS;
	private boolean _isStarted;

	/**
	 * Constructs a new timer.
	 * Does not auto-start.
	 */
	public Timer() {
		this(/* autostart */ false);
	}

	/**
	 * Constructs a new timer.
	 * If an optional bool parameter of true is passed, also starts the timer.
	 */
	public Timer(boolean autostart) {
		_startMS = 0;
		_stopMS = 0;
		_isStarted = false;
		if (autostart) {
			start();
		}
	}

	/**
	 * Returns the number of milliseconds that have elapsed since this timer
	 * was started.
	 * Returns 0 if the timer was never started.
	 */
	public long elapsed() {
		return _stopMS - _startMS;
	}

	/**
	 * Returns true if the timer has been started.
	 */
	public boolean isStarted() {
		return _isStarted;
	}

	/**
	 * Starts the timer.
	 * If the timer was already started, restarts it such that its 'elapsed'
	 * time will be 0 at the moment of the call.
	 */
	public void start() {
		_startMS = currentTimeMS();
		_isStarted = true;
	}

	/**
	 * Stops the timer and returns the number of elapsed milliseconds.
	 * If the timer was never started, 0 ms will be considered to have elapsed.
	 */
	public long stop() {
		_stopMS = currentTimeMS();
		if (!_isStarted) {
			_startMS = _stopMS;
		}
		_isStarted = false;
		return elapsed();
	}

	/**
	 * A static utility function for getting the current time as a Unix
	 * timestamp of milliseconds since the epoch.
	 */
	public static long currentTimeMS() {
		return System.currentTimeMillis();
	}
}
