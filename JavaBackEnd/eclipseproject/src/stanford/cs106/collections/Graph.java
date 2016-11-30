/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

/**
 * A Graph is a collection of vertices and edges between vertices.
 * A graph can be directed or undirected (indicating whether edges are one-directional
 * or bidirectional, respectively), and weighted or unweighted (indicating whether
 * edges have different or uniform cost, respectively).
 * 
 * <p>
 * A Graph does not allow null vertices, but it does allow null edges,
 * which corresponds to an edge with no extra information stored in it.
 * 
 * <p>
 * A Graph can have at most one edge with the same start and end vertex;
 * in other words, it is not permitted to have two edges between the same vertices,
 * except in a directed graph it is allowed to have an edge A,B and an edge B,A.
 * Loops (edges from a vertex directly back to itself) are prohibited; an
 * IllegalArgumentException will be thrown on an attempt to add a loop.
 * 
 * <p>
 * Implementations of the Graph interface do not allow edges with negative weights.
 * If a negative edge weight is passed, an IllegalArgumentException is thrown.
 * A weight of 0 is allowed.
 */
public interface Graph<V, E> {
	/**
	 * Adds an edge between the given two vertices, if none already exists.
	 * The edge will be given a default weight of 1.
	 * No extra information will be stored in the edge.
	 * If an edge already exists between these vertices, its extra information
	 * will be cleared and its weight is updated to 1.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * 
	 * @param v1 Starting vertex.
	 * @param v2 Ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph,
	 *                                  or if v1 and v2 are the same (a loop).
	 * @throws NullPointerException If any object parameter is null.
	 */
	void addEdge(String v1, String v2);

	/**
	 * Adds an edge between the given two vertices, if none already exists.
	 * The edge will be given a default weight of 1.
	 * No extra information will be stored in the edge.
	 * If an edge already exists between these vertices, its extra information
	 * will be cleared and its weight is updated to 1.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * 
	 * @param v1 Starting vertex.
	 * @param v2 Ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph,
	 *                                  or if v1 and v2 are the same (a loop).
	 * @throws NullPointerException If any object parameter is null.
	 */
	void addEdge(Vertex<V> v1, Vertex<V> v2);
	
	/**
	 * Adds an edge between the given two vertices, if none already exists.
	 * The edge will be given a default weight of 1.
	 * No extra information will be stored in the edge.
	 * If an edge already exists between these vertices, its extra information
	 * will be cleared and its weight is updated to 1.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * 
	 * @param v1 Starting vertex.
	 * @param v2 Ending vertex.
	 * @param weight Edge's weight.
	 * @throws IllegalArgumentException If either vertex is not part of the graph,
	 *                                  if edge weight is negative,
	 *                                  if v1 and v2 are the same (a loop),
	 *                                  or if this is an unweighted graph and an edge
	 *                                  weight other than 1 is passed.
	 * @throws NullPointerException If any object parameter is null.
	 */
	void addEdge(String v1, String v2, double weight);
	
	/**
	 * Adds an edge between the given two vertices, if none already exists.
	 * The edge will be given a default weight of 1.
	 * No extra information will be stored in the edge.
	 * If an edge already exists between these vertices, its extra information
	 * will be cleared and its weight is updated to 1.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * 
	 * @param v1 Starting vertex.
	 * @param v2 Ending vertex.
	 * @param weight Edge's weight.
	 * @throws IllegalArgumentException If either vertex is not part of the graph,
	 *                                  if edge weight is negative,
	 *                                  if v1 and v2 are the same (a loop),
	 *                                  or if this is an unweighted graph and an edge
	 *                                  weight other than 1 is passed.
	 * @throws NullPointerException If any object parameter is null.
	 */
	void addEdge(Vertex<V> v1, Vertex<V> v2, double weight);
	
	/**
	 * Adds the given element as a vertex in this graph.
	 * If the given element is already a vertex in this graph, no change is made.
	 * @param v The vertex to add.
	 * @throws NullPointerException If the vertex is null.
	 */
	void addVertex(String v);
	
	/**
	 * Removes all vertices and edges from this graph.
	 */
	void clear();
	
	/**
	 * Removes all edges from this graph.
	 */
	void clearEdges();
	
	/**
	 * Returns whether this graph contains an edge between the given vertices.
	 * If either vertex is not part of this graph, returns false.
	 */
	boolean containsEdge(String v1, String v2);
	
	/**
	 * Returns whether this graph contains an edge between the given vertices.
	 * If either vertex is not part of this graph, returns false.
	 */
	boolean containsEdge(Vertex<V> v1, Vertex<V> v2);
	
	/**
	 * Returns true if the given list of vertices represents a path that can
	 * be formed by walking edges between those vertices in this graph,
	 * walking from the start to the end of the list.
	 * If any vertex in the path is not found in this graph, or if there is
	 * not an edge between any pair of neighboring vertices in the list, returns false.
	 * Returns true for an empty list, and for a one-element list where the one
	 * element is a vertex that is found in this graph.
	 * @param path The list of vertices representing the path to check.
	 * @throws NullPointerException If the list or any of its elements is null.
	 */
	boolean containsPath(List<String> path);
	
	/**
	 * Returns whether this graph contains the given vertex.
	 * If the vertex passed is null, returns false.
	 * @param v The vertex.
	 */
	boolean containsVertex(String v);
	
	/**
	 * Returns whether this graph contains the given vertex.
	 * If the vertex passed is null, returns false.
	 * @param v The vertex.
	 */
	boolean containsVertex(Vertex<V> v);
	
	/**
	 * Returns the total cost of following the given path of vertices in this graph,
	 * which is the sum of the edge weights between the path's vertices from start to end.
	 * @param path The list of vertices in the path to examine.
	 * @throws IllegalArgumentException If any vertex in the path is not part of this graph,
	 *                                  or if there is not an edge between any two neighboring
	 *                                  vertices in the path.
	 * @throws NullPointerException If the path or any vertex in it is null.
	 */
	public double cost(List<String> path);
	
	/**
	 * Returns the number of outgoing edges from the given vertex.
	 * This method is an alias for outDegree; it is provided because
	 * undirected graphs generally just think of vertices as having one
	 * overall degree and not a separate in- and out- degree.
	 * @param v The vertex.
	 * @throws IllegalArgumentException If the vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	int degree(String v);
	
	/**
	 * Returns the number of outgoing edges from the given vertex.
	 * This method is an alias for outDegree; it is provided because
	 * undirected graphs generally just think of vertices as having one
	 * overall degree and not a separate in- and out- degree.
	 * @param v The vertex.
	 * @throws IllegalArgumentException If the vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	int degree(Vertex<V> v);
	
	/**
	 * Returns the extra information stored in the edge between vertices v1 and v2, if one exists.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * Returns null if there is no edge between the given vertices or if the edge between
	 * the given vertices contains no extra information.
	 * @param v1 The starting vertex.
	 * @param v2 The ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	Edge<V, E> edge(String v1, String v2);
	
	/**
	 * Returns the extra information stored in the edge between vertices v1 and v2, if one exists.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * Returns null if there is no edge between the given vertices or if the edge between
	 * the given vertices contains no extra information.
	 * @param v1 The starting vertex.
	 * @param v2 The ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	Edge<V, E> edge(Vertex<V> v1, Vertex<V> v2);
	
	/**
	 * Returns the total number of edges in this graph.
	 * Note that this does not necessarily equal edges().size() because edges()
	 * returns a set of unique edge information values stored in each edge,
	 * excluding nulls (edges without any extra information stored in them),
	 * while this method returns a count of all edges including ones without
	 * any information stored in them and ones with information that is a duplicate
	 * of that stored in some other edge.
	 */
	int edgeCount();
	
	/**
	 * Returns a collection of all edges in this graph.
	 * The collection is a shallow 'view' of the edges.
	 * Any changes made to it are written back to the graph.
	 * New edges cannot be added directly to the collection returned by this method,
	 * because it would not know which vertices to associate the edge with.
	 * But edges can be removed, cleared, etc.
	 */
	Collection<Edge<V, E>> edges();
	
	/**
	 * Returns the weight of the edge between vertices v1 and v2, if one exists.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * Returns -1 if there is no edge between the given vertices.
	 * @param v1 The starting vertex.
	 * @param v2 The ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	double edgeWeight(String v1, String v2);
	
	/**
	 * Returns the weight of the edge between vertices v1 and v2, if one exists.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * Returns -1 if there is no edge between the given vertices.
	 * @param v1 The starting vertex.
	 * @param v2 The ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	double edgeWeight(Vertex<V> v1, Vertex<V> v2);
	
	/**
	 * Returns the number of incoming edges to the given vertex.
	 * @param v The vertex to examine.
	 * @throws IllegalArgumentException If the vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	int inDegree(String v);
	
	/**
	 * Returns the number of incoming edges to the given vertex.
	 * @param v The vertex to examine.
	 * @throws IllegalArgumentException If the vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	int inDegree(Vertex<V> v);
	
	/**
	 * Returns true if this is a directed graph, and false if it is undirected.
	 */
	boolean isDirected();
	
	/**
	 * Returns true if this graph does not contain any vertices or edges.
	 */
	boolean isEmpty();
	
	/**
	 * Returns true if there is any path in this graph that 
	 * leads from the given starting vertex v1 to the given ending vertex v2. 
	 * Uses the Depth-First Search (DFS) algorithm to find the path.
	 * If there is no path to v2 from v1, the method returns false. 
	 * @param v1 The starting vertex.
	 * @param v2 The ending vertex.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	// boolean isReachable(V v1, V v2);
	
	/**
	 * Returns true if this is a weighted graph and false if it is unweighted.
	 */
	boolean isWeighted();
	
	/** 
	 * Returns a collection containing all neighbors, that is, all vertices that are
	 * directly connected to the given vertex by an outgoing edge.
	 * The collection is a shallow 'view' of the neighboring vertices.
	 * Any changes made to it are written back to the graph.
	 * New vertices cannot be added directly to the collection returned by this method.
	 * But vertices can be removed, cleared, etc.
	 * @param v The vertex to examine.
	 * @throws IllegalArgumentException If the vertex is not part of this graph.
	 * @throws NullPointerException If the vertex is null.
	 */
	Set<Vertex<V>> neighbors(String v);
	
	/** 
	 * Returns a collection containing all neighbors, that is, all vertices that are
	 * directly connected to the given vertex by an outgoing edge.
	 * The collection is a shallow 'view' of the neighboring vertices.
	 * Any changes made to it are written back to the graph.
	 * New vertices cannot be added directly to the collection returned by this method.
	 * But vertices can be removed, cleared, etc.
	 * @param v The vertex to examine.
	 * @throws IllegalArgumentException If the vertex is not part of this graph.
	 * @throws NullPointerException If the vertex is null.
	 */
	Set<Vertex<V>> neighbors(Vertex<V> v);
	
	/**
	 * Returns the number of outgoing edges from the given vertex.
	 * @param v The vertex to examine.
	 */
	int outDegree(String v);
	
	/**
	 * Returns the number of outgoing edges from the given vertex.
	 * @param v The vertex to examine.
	 */
	int outDegree(Vertex<V> v);
	
	/**
	 * Removes any edge(s) that exist with the given extra info stored in it/them.
	 * If multiple edges have the given extra info, all are removed.
	 * If null is passed, removes any edges that have no extra info stored in them.
	 * If no edge exists in this graph with the given extra info, has no effect.
	 * This implementation must examine all edges in the graph to find the
	 * specified edge, so it is inefficient.
	 * @param e The edge extra info of the edge to remove.
	 */
	void removeEdge(Edge<V, E> e);
	
	/**
	 * Removes any edge that exists from vertex v1 to vertex v2.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * If no edge exists in this graph between the two given vertices, has no effect.
	 * @param v1 The starting vertex of the edge.
	 * @param v2 The ending vertex of the edge.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	void removeEdge(String v1, String v2);
	
	/**
	 * Removes any edge that exists from vertex v1 to vertex v2.
	 * If this graph is undirected, the two vertices can be passed in either order.
	 * If no edge exists in this graph between the two given vertices, has no effect.
	 * @param v1 The starting vertex of the edge.
	 * @param v2 The ending vertex of the edge.
	 * @throws IllegalArgumentException If either vertex is not part of the graph.
	 * @throws NullPointerException If any object parameter is null.
	 */
	void removeEdge(Vertex<V> v1, Vertex<V> v2);
	
	/**
	 * Removes the given vertex from this graph, along with all edges that were
	 * touching it.
	 * If the given vertex is not part of this graph, has no effect.
	 * @param v The vertex to remove.
	 * @throws NullPointerException If the vertex is null.
	 */
	void removeVertex(String v);
	
	/**
	 * Removes the given vertex from this graph, along with all edges that were
	 * touching it.
	 * If the given vertex is not part of this graph, has no effect.
	 * @param v The vertex to remove.
	 * @throws NullPointerException If the vertex is null.
	 */
	void removeVertex(Vertex<V> v);
	
	/**
	 * Clears out any data stored in each vertex.
	 */
	void resetData();
	
	/** 
	 * Returns a string representation of this graph and its vertices and edges, such as
	 * "{V={A, B, C}, E={(A, B), (A, C)}}".
	 */
	String toString();
	
	/** 
	 * Returns a detailed multi-line string representation of this graph and its vertices, such as
	 * "Graph{
	 *     vertices:
	 *         A -> neighbors: [B, C]
	 *         B -> neighbors: [A]
	 *         C -> neighbors: [A]
	 *     edges:
	 *         (A,B,weight=2)
	 *         (A,C)
	 * }"
	 */
	String toStringDetailed();
	
	/**
	 * Returns the structure of information about the vertex with the given name.
	 */
	Vertex<V> vertex(String name);
	
	/**
	 * Returns the number of vertices in this graph.
	 */
	int vertexCount();

	/** 
	 * Returns a collection of the vertices in this graph.
	 * The collection is a shallow 'view' of the vertices.
	 * Any changes made to it are written back to the graph.
	 * Vertices can be added, removed, cleared, etc. on this collection.
	 */
	Collection<Vertex<V>> vertexes();
}
