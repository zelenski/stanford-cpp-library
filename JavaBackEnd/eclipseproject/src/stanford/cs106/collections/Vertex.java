/*
 * @author Marty Stepp
 * @version 2016/11/29
 * - added template parameter <V>
 * @version 2015/06/08
 */

package stanford.cs106.collections;

import java.awt.Color;
import java.util.*;

/**
 * A Vertex object represents extra information associated with each vertex.
 * Each Vertex object for some vertex value v is accessed internally in a
 * Graph subclass by writing:
 * 
 * <pre>
 * {@code
 * vertexInfo(v)
 * }
 * </pre>
 */
@SuppressWarnings("deprecation")
public class Vertex<V> extends Observable implements Comparable<Vertex<V>>, Cloneable {
	/** Maximum possible cost to reach a vertex from another; treat this as "infinity". */
	public static final double MAX_COST = Double.POSITIVE_INFINITY;
	
	private String name;
	private int number;         // a mark giving a number or index to this vertex (or -1)
	private Color color;        // for some algorithms that like to color vertexes
	private Vertex<V> previous;    // a reference to a 'previous' vertex; useful for path searching
	private boolean visited;    // a mark for whether this vertex has been visited
	private double cost;        // a mark for the minimum cost to reach this vertex
	public V extraData;    // anything else you want to store in here
	
	/**
	 * Constructs information for the given vertex.
	 * @param v The vertex this information is about.
	 * @throws NullPointerException If the vertex is null.
	 */
	public Vertex(String name) {
		if (name == null) {
			throw new NullPointerException("null name");
		}
		this.name = name;
		this.clear();
	}
	
	/**
	 * Resets the previous, visited, cost, and number data fields to their original values.
	 */
	public void clear() {
		clear(false);
	}
	
	/**
	 * Resets the previous, visited, cost, and number data fields to their original values.
	 */
	public void clear(boolean clearExtraData) {
		this.previous = null;
		this.visited = false;
		this.cost = MAX_COST;
		this.number = -1;
		this.color = null;
		if (clearExtraData) {
			this.extraData = null;
		}
	}
	
	/**
	 * Returns a copy of this Vertex as per the contract of the Object.clone method.
	 */
	public Vertex<V> clone() {
		try {
			@SuppressWarnings("unchecked")
			Vertex<V> copy = (Vertex<V>) super.clone();
			copy.clear();
			return copy;
		} catch (CloneNotSupportedException e) {
			return null;  // will not happen
		}
	}
	
	/**
	 * Returns color of vertex
	 */
	public Color color() {
		return color;
	}

	public int compareTo(Vertex<V> o) {
		return name.compareTo(o.name);
	}
	
	/**
	 * Returns the value of the internal 'cost' to reach this vertex.
	 * Returns Integer.MAX_VALUE if the cost was not set previously.
	 * Used internally in various path-finding algorithms.
	 */
	public double cost() {
		return cost;
	}
	
	/**
	 * Returns true if o refers to a Vertex object correspending to the same
	 * underlying 'V' object as this one.
	 */
	public boolean equals(Object o) {
		if (o instanceof Vertex) {
			@SuppressWarnings("unchecked")
			Vertex<V> other = (Vertex<V>) o;
			return name.equals(other.name);
		} else {
			return false;
		}
	}
	
	/**
	 * Returns an integer code for placing this vertex into a hash-based collection.
	 */
	public int hashCode() {
		return name.hashCode();
	}
	
	/**
	 * Returns the vertex's name.
	 */
	public String name() {
		return name;
	}
	
	public void notifyObservers() {
		setChanged();
		super.notifyObservers();
	}
	
	public void notifyObservers(Object arg) {
		setChanged();
		super.notifyObservers(arg);
	}
	
	/**
	 * Returns the value of the internal 'number' vertex marker for this vertex.
	 * Returns -1 if the number was not set previously.
	 * Used internally in various algorithms such as topological sort.
	 */
	public int number() {
		return number;
	}
	
	/**
	 * Returns the value of the internal 'previous' vertex marker for this vertex.
	 * Used internally in various path-finding algorithms.
	 * If no previous vertex has been set, returns null.
	 */
	public Vertex<V> previous() {
		return previous;
	}
	
	/**
	 * Sets the vertex's color
	 */
	public void setColor(Color color) {
		this.color = color;
		notifyObservers(Event.COLOR_CHANGED);
	}
	
	/**
	 * Sets the value of the internal 'cost' to reach this vertex.
	 * Used internally in various path-finding algorithms.
	 * @param cost the new cost to use
	 */
	public void setCost(double cost) {
		this.cost = cost;
	}
	
	/**
	 * Sets the value of the internal 'number' vertex marker for this vertex.
	 * Used internally in various algorithms such as topological sort.
	 * @param number the new number to use
	 */
	public void setNumber(int number) {
		this.number = number;
	}
	
	/**
	 * Sets the value of the internal 'previous' vertex marker for this vertex.
	 * Used internally in various path-finding algorithms.
	 * If null is passed, the previous vertex is cleared.
	 */
	public void setPrevious(Vertex<V> previous) {
		this.previous = previous;
	}
	
	/**
	 * Sets the value of the internal 'previous' vertex marker to true for this vertex.
	 * Used internally in various path-finding algorithms.
	 */
	public void setVisited() {
		setVisited(true);
	}
	
	/**
	 * Sets the value of the internal 'previous' vertex marker for this vertex.
	 * Used internally in various path-finding algorithms.
	 */
	public void setVisited(boolean visited) {
		this.visited = visited;
	}
	
	/**
	 * Returns the value of the internal 'visited' vertex marker for this vertex.
	 * Used internally in various path-finding algorithms.
	 */
	public boolean visited() {
		return visited;
	}
	
	/**
	 * Returns a string representation of the information about this vertex.
	 */
	public String toString() {
		String result = "(" + this.name;
		if (this.number != -1) {
			result += ",#" + this.number;
		}
		if (this.previous != null) {
			result += ",previous=" + this.previous.name;
		}
		if (this.cost < MAX_COST) {
			result += ",cost=" + this.cost;
		}
		if (this.visited) {
			result += ",visited";
		}
		if (this.extraData != null) {
			result += ",extraData=" + extraData;
		}
		return result + ")";
	}
	
	public static enum Event {
		COLOR_CHANGED
	}
}
