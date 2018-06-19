package stanford.karel;

import java.util.Observable;
import java.util.Observer;

/**
 * An event is an improved version of a java.util.Observable.
 */
public class Event<T> extends Observable {
	private String name = "Event";

	public Event() {
	}

	public Event(String name) {
		this.name = name;
	}

	public void add(Observer o) {
		this.addObserver(o);
	}

	public void remove(Observer o) {
		this.deleteObserver(o);
	}

	public void fire() {
		this.fire(null);
	}

	public void fire(T arg) {
		this.setChanged();
		this.notifyObservers(arg);
	}

	public String getName() {
		return this.name;
	}

	public String toString() {
		return this.name;
	}
}
