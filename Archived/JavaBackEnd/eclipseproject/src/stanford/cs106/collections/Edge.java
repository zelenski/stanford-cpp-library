/*
 * @author Marty Stepp
 * @version 2016/11/29
 * - added template parameters <V, E>
 * @version 2015/06/08
 */

package stanford.cs106.collections;

/**
 * An Edge object represents information stored in a graph edge,
 * including its vertices, weight, and any extra edge info.
 */
public class Edge<V, E> implements Cloneable {
	/** Default weight of an edge in an undirected graph (1). */
	public static final int DEFAULT_WEIGHT = 1;
	
	private Vertex<V> start;        // starting vertex for this edge
	private Vertex<V> end;          // ending vertex for this edge
	private double weight;       // The edge's weight; 1 in an unweighted graph
	private boolean weighted;
	public E extraData;     // extra information, if any stored in this edge (null if none)
	
	/** 
	 * Constructs information about the given edge with the default edge weight.
	 * @param start The starting vertex of the edge.
	 * @param end The ending vertex of the edge.
	 * @param edge The extra information (if any) to store in this edge; null if none.
	 * @throws NullPointerException If the start or end vertex is null.
	 */
	public Edge(Vertex<V> start, Vertex<V> end) {
		this(start, end, DEFAULT_WEIGHT, false);
	}
	
	/** 
	 * Constructs information about the given edge with the default edge weight.
	 * @param start The starting vertex of the edge.
	 * @param end The ending vertex of the edge.
	 * @param edge The extra information (if any) to store in this edge; null if none.
	 * @throws NullPointerException If the start or end vertex is null.
	 */
	public Edge(Vertex<V> start, Vertex<V> end, double weight) {
		this(start, end, weight, true);
	}
	
	/**
	 * Constructs information about the given edge with the given weight.
	 * @param start The starting vertex of the edge.
	 * @param end The ending vertex of the edge.
	 * @param weight Edge's weight; cannot be negative.  Set to 1 in an unweighted graph.
	 * @param weighted true if this is used in a weighted graph, else false.
	 * @throws IllegalArgumentException If the weight is negative.
	 * @throws NullPointerException If the start or end vertex is null.
	 */
	private Edge(Vertex<V> start, Vertex<V> end, double weight, boolean weighted) {
		if (start == null) {
			throw new NullPointerException("null start vertex");
		}
		if (end == null) {
			throw new NullPointerException("null end vertex");
		}

		this.start = start;
		this.end = end;
		setWeight(weight);
		this.weighted = weighted;
	}
	
	/**
	 * Returns a copy of this Edge as per the contract of the Object.clone method.
	 */
	public Edge<V, E> clone() {
		try {
			@SuppressWarnings("unchecked")
			Edge<V, E> copy = (Edge<V, E>) super.clone();
			return copy;
		} catch (CloneNotSupportedException e) {
			return null;  // will not happen
		}
	}
	
	/**
	 * Returns true if this edge touches the given vertex, that is,
	 * if it is the start or end of this edge.
	 * @throws NullPointerException If the vertex is null.
	 */
	public boolean contains(Vertex<V> vertex) {
		if (vertex == null) {
			throw new NullPointerException("null vertex");
		}
		return start.equals(vertex) || end.equals(vertex);
	}
	
	/**
	 * Returns the weight of this edge.
	 */
	public double cost() {
		return weight();
	}
	
	/**
	 * Returns the ending vertex of this edge.
	 * In undirected graphs, the notion of "start" and "end" are ambiguous
	 * for an edge; this method always returns the second vertex that was passed
	 * to the edge's constructor.
	 */
	public Vertex<V> end() {
		return end;
	}
	
	/**
	 * Returns true if o refers to an Edge object with identical state to this Edge object.
	 * @param o the object to examine
	 */
	public boolean equals(Object o) {
		if (o instanceof Edge) {
			@SuppressWarnings("unchecked")
			Edge<V, E> other = (Edge<V, E>) o;
			return start.equals(other.start) &&
					end.equals(other.end) &&
					weight == other.weight;
		} else {
			return false;
		}
	}
	
	/**
	 * Returns the ending vertex of this edge.
	 * In undirected graphs, the notion of "start" and "end" are ambiguous
	 * for an edge; this method always returns the second vertex that was passed
	 * to the edge's constructor.
	 */
	public Vertex<V> finish() {
		return end;
	}
	
	/**
	 * Returns an integer code for placing this edge into a hash-based collection.
	 */
	public int hashCode() {
		return 13 * start.hashCode() +
				37 * end.hashCode();
				// 137 * weight +
				// (edge == null ? 42 : edge.hashCode());
	}
	
	/**
	 * Modifies this edge's weight to be the given value.
	 * @param weight The new weight to use.
	 * @throws IllegalArgumentException If the weight is negative.
	 */
	public void setWeight(double weight) {
		if (weight < 0) {
			throw new IllegalArgumentException("negative edge weight: " + weight);
		}
		this.weight = weight;
	}
	
	/**
	 * Returns the starting vertex of this edge.
	 * In undirected graphs, the notion of "start" and "end" are ambiguous
	 * for an edge; this method always returns the first vertex that was passed
	 * to the edge's constructor.
	 */
	public Vertex<V> start() {
		return start;
	}
	
	/**
	 * Returns the weight of this edge.
	 */
	public double weight() {
		return weight;
	}
	
	/** Returns a string representation of the information about this edge. */
	public String toString() {
		return "(" + start.name() + "," + end.name()
				+ (weighted ? ",weight=" + weight : "")
				+ (extraData != null ? ",extraData=" + extraData : "")
				+ ")";
	}
}
