package acm.gui;

import java.util.*;

/**
 * A helper class to represent observable events.
 */
@SuppressWarnings("deprecation")
public class Event<T> extends Observable {
	/** Convenience method to replace notifyObservers. */
	public void fire() {
		notifyObservers(null);
	}

	/** Convenience method to replace notifyObservers. */
	public void fire(T arg) {
		setChanged();
		super.notifyObservers(arg);
	}
}

