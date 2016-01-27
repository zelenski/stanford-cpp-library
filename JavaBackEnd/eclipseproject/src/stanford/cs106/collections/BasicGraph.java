/*
 * @author Marty Stepp
 * @version 2015/11/30
 * - fixed bugs related to in/outDegree and neighbors of vertexes that have none
 *   (should have returned 0 or empty set; instead crashed with NPE)
 * @since 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

/**
 * BasicGraph represents a mostly-complete implementation of the Graph interface.
 * The only functionality missing is the three path-searching algorithms represented
 * by the isReachable, minimumWeightPath, and shortestPath methods.
 */
public class BasicGraph implements Graph {
	// data fields
	private boolean directed;
	private boolean weighted;
	private Table<Vertex, Vertex, Edge> adjacencyMap;   // [source, destination --> edge info]
	private Map<String, Vertex> vertexes;               // [vertex] -> [vertex info]
	
	/**
	 * Constructs a new empty undirected, unweighted graph.
	 */
	public BasicGraph() {
		this(false, false);
	}
	
	/**
	 * Constructs a new empty graph that can be directed or undirected,
	 * weighted or unweighted.
	 */
	public BasicGraph(boolean directed, boolean weighted) {
		this.directed = directed;
		this.weighted = weighted;
		adjacencyMap = TreeBasedTable.create();
		vertexes = new TreeMap<String, Vertex>();
		// edges = HashMultimap.create();
	}

	/** {@inheritDoc} */
	public final void addEdge(String v1, String v2) {
		addEdge(v1, v2, Edge.DEFAULT_WEIGHT);
	}

	/** {@inheritDoc} */
	public final void addEdge(String v1, String v2, double weight) {
		addEdge(vertex(v1), vertex(v2), weight);
	}

	/** {@inheritDoc} */
	public final void addEdge(Vertex v1, Vertex v2) {
		addEdge(v1, v2, Edge.DEFAULT_WEIGHT);
	}

	/** {@inheritDoc} */
	public final void addEdge(Vertex v1, Vertex v2, double weight) {
		checkVertexes(v1, v2);
		checkForNegative(weight);
		if (v1.equals(v2)) {
			throw new IllegalArgumentException("Cannot add a loop (an edge from "
					+ v1 + " back to itself)");
		}
		if (!weighted && weight != Edge.DEFAULT_WEIGHT) {
			throw new IllegalArgumentException("Unweighted graph cannot accept edge weight of "
					+ weight + " (must be " + Edge.DEFAULT_WEIGHT + ")");
		}
		
		Edge edge = null;
		if (containsEdge(v1, v2)) {
			edge = edge(v1, v2);
			edge.setWeight(weight);
		} else {
			if (weighted) {
				edge = new Edge(v1, v2, weight);
			} else {
				edge = new Edge(v1, v2);
			}
			adjacencyMap.put(v1, v2, edge);
			if (!directed) {
				adjacencyMap.put(v2, v1, edge);
			}
			// edges.put(e, edge);
		}
	}
	
	/** {@inheritDoc} */
	public final void addVertex(String v) {
		checkForNull(v);
		if (!containsVertex(v)) {
			vertexes.put(v, new Vertex(v));
		}
	}

	/** {@inheritDoc} */
	public final void clear() {
		vertexes.clear();
		clearEdges();
	}

	/** {@inheritDoc} */
	public final void clearEdges() {
		adjacencyMap.clear();
	}
	
	/** {@inheritDoc} */
	public final boolean containsEdge(String v1, String v2) {
		return containsEdge(vertex(v1), vertex(v2));
	}
	
	/** {@inheritDoc} */
	public final boolean containsEdge(Vertex v1, Vertex v2) {
		return adjacencyMap.contains(v1, v2) ||
				(!directed && adjacencyMap.contains(v2, v1));
	}

	/** {@inheritDoc} */
	public final boolean containsPath(List<String> path) {
		checkForNull(path);
		
		// check edges of each pair of neighbors
		Iterator<String> itr = path.iterator();
		String prev = null;
		while (itr.hasNext()) {
			String next = itr.next();
			if (!containsVertex(next) || 
					(prev != null && !containsEdge(prev, next))) {
				return false;
			}
			prev = next;
		}
		
		return true;
	}

	/** {@inheritDoc} */
	public final boolean containsVertex(String v) {
		return vertexes.containsKey(v);
	}

	/** {@inheritDoc} */
	public final boolean containsVertex(Vertex v) {
		return vertexes.containsKey(v.name());
	}
	
	/** {@inheritDoc} */
	public final double cost(List<String> path) {
		checkForNull(path);
		if (path.isEmpty()) {
			return 0;
		}
		
		int totalCost = 0;
		String prev = null;
		for (String v : path) {
			checkForNull(v);
			checkVertex(v);
			if (prev != null) {
				double weight = edgeWeight(prev, v);
				if (weight < 0) {
					throw new IllegalArgumentException("no edge between " + prev + " and " + v);
				}
				totalCost += weight;
			}
			prev = v;
		}
		
		return totalCost;
	}

	/** {@inheritDoc} */
	public final int degree(String v) {
		return outDegree(v);
	}

	/** {@inheritDoc} */
	public final int degree(Vertex v) {
		return outDegree(v);
	}

	/** {@inheritDoc} */
	public final Edge edge(String v1, String v2) {
		return edge(vertex(v1), vertex(v2));
	}

	/** {@inheritDoc} */
	public final Edge edge(Vertex v1, Vertex v2) {
		checkForNull(v1, v2);
		checkVertexes(v1, v2);
		if (adjacencyMap.contains(v1, v2)) {
			return adjacencyMap.get(v1, v2);
		} else {
			return null;
		}
	}

	/** {@inheritDoc} */
	public final int edgeCount() {
		return edges().size();
	}

	/** {@inheritDoc} */
	public final Collection<Edge> edges() {
		// return new EdgeCollection();
		return adjacencyMap.values();
	}

	/** {@inheritDoc} */
	public final double edgeWeight(String v1, String v2) {
		return edgeWeight(vertex(v1), vertex(v2));
	}

	/** {@inheritDoc} */
	public final double edgeWeight(Vertex v1, Vertex v2) {
		if (containsEdge(v1, v2)) {
			// will work for either order in undirected graph
			return edge(v1, v2).weight();
		} else {
			return -1;
		}
	}
	
	/**
	 * Returns true if o refers to a graph with the same vertices, edges, and other properties
	 * (directed vs. undirected, weighted vs. unweighted, etc.) as this graph.
	 * @param o The object to compare against.
	 */
	public boolean equals(Object o) {
		if (o instanceof BasicGraph) {
			BasicGraph other = (BasicGraph) o;
			return directed == other.directed &&
					weighted == other.weighted &&
					adjacencyMap.equals(other.adjacencyMap) &&
					vertexes.equals(other.vertexes);
		} else {
			return false;
		}
	}
	
	/**
	 * Returns an integer code for placing this graph into a hash-based collection.
	 */
	public int hashCode() {
		return 13 * Boolean.valueOf(directed).hashCode() +
				37 * Boolean.valueOf(weighted).hashCode() +
				51 * adjacencyMap.hashCode() +
				117 * vertexes.hashCode();
	}

	/** {@inheritDoc} */
	public final int inDegree(String v) {
		return inDegree(vertex(v));
	}

	/** {@inheritDoc} */
	public final int inDegree(Vertex v) {
		checkVertex(v);
		if (adjacencyMap.containsColumn(v)) {
			return adjacencyMap.column(v).size();
		} else {
			return 0;
		}
	}

	/** {@inheritDoc} */
	public final boolean isDirected() {
		return directed;
	}

	/** {@inheritDoc} */
	public final boolean isEmpty() {
		return vertexes.isEmpty();
	}
	
	/** {@inheritDoc} */
	public final boolean isWeighted() {
		return weighted;
	}

	/** {@inheritDoc} */
	public final Set<Vertex> neighbors(String v) {
		return neighbors(vertex(v));
	}

	/** {@inheritDoc} */
	public final Set<Vertex> neighbors(Vertex v) {
		checkVertex(v);
		if (adjacencyMap.containsRow(v)) {
			return adjacencyMap.row(v).keySet();
		} else {
			return Collections.emptySet();
		}
	}

	/** {@inheritDoc} */
	public final int outDegree(String v) {
		return outDegree(vertex(v));
	}

	/** {@inheritDoc} */
	public final int outDegree(Vertex v) {
		return neighbors(v).size();
	}

	/** {@inheritDoc} */
	public final void removeEdge(Edge e) {
		removeEdge(e.start().name(), e.end().name());
	}

	/** {@inheritDoc} */
	public final void removeEdge(String v1, String v2) {
		removeEdge(vertex(v1), vertex(v2));
	}

	/** {@inheritDoc} */
	public final void removeEdge(Vertex v1, Vertex v2) {
		if (containsEdge(v1, v2)) {
			adjacencyMap.remove(v1, v2);
			if (!directed) {
				adjacencyMap.remove(v2, v1);
			}
		}
	}

	/** {@inheritDoc} */
	public final void removeVertex(String v) {
		checkForNull(v);
		if (containsVertex(v)) {
			// remove any incoming/outgoing edges from this vertex
			removeVertexHelper(v);
			vertexes.remove(v);
		}
	}

	/** {@inheritDoc} */
	public final void removeVertex(Vertex v) {
		checkForNull(v);
		if (containsVertex(v)) {
			// remove any incoming/outgoing edges from this vertex
			removeVertexHelper(v);
			vertexes.remove(v);
		}
	}

	/** {@inheritDoc} */
	public final void resetData() {
		clearVertexInfo();
	}
	
	/** {@inheritDoc} */
	public final String toString() {
		StringBuilder sb = new StringBuilder(65536);
		sb.append("{V={");
		boolean first = true;
		for (Vertex v : vertexes()) {
			if (!first) {
				sb.append(", ");
			}
			first = false;
			sb.append(v.name());
		}
		sb.append("}");
		
		sb.append(", E={");
		first = true;
		
		for (Edge e : adjacencyMap.values()) {
			if (!first) {
				sb.append(", ");
			}
			first = false;
			sb.append(e);
		}
		sb.append("}");
		
		sb.append("}");
		return sb.toString();
	}

	/** {@inheritDoc} */
	public final String toStringDetailed() {
		StringBuilder sb = new StringBuilder(65536);
		sb.append("Graph{\n").append("\tvertices:\n");
		for (Vertex v : vertexes()) {
			sb.append("\t\t").append(vertexes.get(v)).append(" -> neighbors: ")
					.append(neighbors(v.name())).append('\n');
		}
		sb.append("\tedges:\n");
		for (Edge edge : adjacencyMap.values()) {
			sb.append("\t\t").append(edge).append('\n');
		}
		sb.append('}');
		return sb.toString();
	}
	
	public Vertex vertex(String name) {
		return vertexes.get(name);
	}
	
	/** {@inheritDoc} */
	public final int vertexCount() {
		return vertexes.size();
	}
	
	/** {@inheritDoc} */
	public final Collection<Vertex> vertexes() {
		// return new VertexSet();
		return vertexes.values();
	}
	
	/** 
	 * Tests the given integer to see whether it is negative.
	 * @param value The integer to examine.
	 * @throws IllegalArgumentException If the value is negative.
	 */
	protected static void checkForNegative(double value) {
		if (value < 0) {
			throw new IllegalArgumentException("argument cannot be negative: " + value);
		}
	}
	
	/** 
	 * Tests the given integer to see whether it is negative.
	 * @param value The integer to examine.
	 * @throws IllegalArgumentException If the value is negative.
	 */
	protected static void checkForNegative(int value) {
		if (value < 0) {
			throw new IllegalArgumentException("argument cannot be negative: " + value);
		}
	}
	
	/** 
	 * Tests the given arguments for null.
	 * @param args The arguments to examine.
	 * @throws NullPointerException If any argument is null.
	 */
	protected static void checkForNull(Object... args) {
		for (Object o : args) {
			if (o == null) {
				throw new NullPointerException("argument must not be null");
			}
		}
	}
	
	/** 
	 * Tests the given vertex for null and for membership in the graph.
	 * @param vertex The vertex to examine.
	 * @throws IllegalArgumentException If the vertex is not part of this graph.
	 * @throws NullPointerException If the vertex is null.
	 */
	protected final void checkVertex(String vertex) {
		checkForNull(vertex);
		if (!containsVertex(vertex)) {
			throw new IllegalArgumentException("Vertex not found in graph: " + vertex);
		}
	}
	
	protected final void checkVertex(Vertex vertex) {
		checkForNull(vertex);
		if (!containsVertex(vertex.name()) || vertex(vertex.name()) != vertex) {
			throw new IllegalArgumentException("Vertex not found in graph: " + vertex);
		}
	}
	
	/** 
	 * Tests the given vertices for null and for membership in the graph.
	 * @param v1 The first vertex to examine.
	 * @param v2 The second vertex to examine.
	 * @throws IllegalArgumentException If any vertex is not part of this graph.
	 * @throws NullPointerException If any vertex is null.
	 */
	protected final void checkVertexes(String v1, String v2) {
		checkVertex(v1);
		checkVertex(v2);
	}
	
	/** 
	 * Tests the given vertices for null and for membership in the graph.
	 * @param v1 The first vertex to examine.
	 * @param v2 The second vertex to examine.
	 * @throws IllegalArgumentException If any vertex is not part of this graph.
	 * @throws NullPointerException If any vertex is null.
	 */
	protected final void checkVertexes(Vertex v1, Vertex v2) {
		checkVertex(v1);
		checkVertex(v2);
	}
	
	/**
	 * Resets all distance / previous / visited markings from vertex info
	 * objects in this graph.
	 */
	protected final void clearVertexInfo() {
		for (Vertex vertex : vertexes.values()) {
			vertex.clear();
		}
	}
	
	/**
	 * Sets some of the distance / previous / visited markings from vertex info
	 * objects in this graph to have various random values, to test whether
	 * a student's code properly clears the info between calls.
	 */
	protected final void corruptVertexInfo() {
		Random rand = new Random(42);
		List<Vertex> vertexList = new ArrayList<Vertex>(this.vertexes());
		Collections.shuffle(vertexList, rand);
		int i = 0;
		for (Vertex vertex : this.vertexes.values()) {
			if (i % 2 == 0) {
				vertex.setCost(rand.nextInt(1000));
				vertex.setNumber(rand.nextInt(1000000));
				int randomIndex = rand.nextInt(vertexList.size());
				vertex.setPrevious(vertexList.get(randomIndex));
			} else {
				vertex.clear();
			}
		}
	}
	
	/**
	 * Returns the Vertex object associated with the given vertex.
	 * @param v The vertex to examine.
	 * @throws IllegalArgumentException If any vertex is not part of this graph.
	 * @throws NullPointerException If any vertex is null.
	 */
	protected final Vertex vertexInfo(String v) {
		checkVertex(v);
		return vertexes.get(v);
	}
	
	/**
	 * Returns a read-only view of all internal data about vertex information.
	 */
	protected final Map<String, Vertex> vertexInfos() {
		return Collections.unmodifiableMap(vertexes);
	}
	
	// helper to remove a vertex; used by removeVertex and vertices() iterator remove()
	private void removeVertexHelper(String v) {
		Vertex vertex = vertex(v);
		Map<Vertex, Edge> row = adjacencyMap.row(vertex);
//		for (Edge edge : row.values()) {
//			// edges.remove(edge.edge(), edge);
//		}
		row.clear();
		
		Map<Vertex, Edge> column = adjacencyMap.column(vertex);
		if (!directed) {
//			for (Edge edge : column.values()) {
//				// edges.remove(edge.edge(), edge);
//			}
		}
		column.clear();
		vertexes.remove(v);
		
		// TODO: doesn't remove edges connected to that vertex properly; fix
	}
	
	// helper to remove a vertex; used by removeVertex and vertices() iterator remove()
	private void removeVertexHelper(Vertex v) {
		Map<Vertex, Edge> row = adjacencyMap.row(v);
//		for (Edge edge : row.values()) {
//			// edges.remove(edge.edge(), edge);
//		}
		row.clear();
		
		Map<Vertex, Edge> column = adjacencyMap.column(v);
		if (!directed) {
//			for (Edge edge : column.values()) {
//				// edges.remove(edge.edge(), edge);
//			}
		}
		column.clear();
		vertexes.remove(v.name());
		
		// TODO: doesn't remove edges connected to that vertex properly; fix
	}
	
	
	// private class to represent edges() write-through collection view
	/*
	private class EdgeCollection extends AbstractCollection<Edge> {
		@Override
		public Iterator<Edge> iterator() {
			return new EdgeIterator();
		}
		
		@Override
		public void clear() {
			BasicGraph.this.clearEdges();
		}

		@Override
		public int size() {
			return edgeCount();
		}
		
		private class EdgeIterator implements Iterator<Edge> {
			private Iterator<Edge> innerItr;
			private long itrVersion;
			private Edge last;
			
			public EdgeIterator() {
				// innerItr = edges.values().iterator();
				itrVersion = version;
				last = null;
			}
			
			public boolean hasNext() {
				checkVersion();
				return innerItr.hasNext();
			}

			public Edge next() {
				checkVersion();
				Edge edge = innerItr.next();
				last = edge;
				return edge; // edge.edge();
			}

			public void remove() {
				checkVersion();
				innerItr.remove();   // removes from graph's edges collection
				
				// need to also remove that edge from adjacency map
				// see removeEdge(V, V); don't want to modify edges or version though
				adjacencyMap.remove(last.start(), last.end());
				if (!directed) {
					adjacencyMap.remove(last.end(), last.start());
				}
			}
			
			private void checkVersion() {
				if (version != itrVersion) {
					throw new ConcurrentModificationException();
				}
			}
		}
	}
	*/
	
	// private class to represent vertices() write-through set view
	/*
	private class VertexSet extends AbstractSet<Vertex> {
		@Override
		public boolean add(Vertex v) {
			addVertex(v);
			return true;
		}
		
		@Override
		public void clear() {
			BasicGraph.this.clear();
		}
		
		@Override
		public Iterator<Vertex> iterator() {
			return new VertexIterator();
		}

		@Override
		public int size() {
			return vertexCount();
		}

		private class VertexIterator implements Iterator<Vertex> {
			private Iterator<Vertex<Vertex>> innerItr;
			private long itrVersion;
			private Vertex<Vertex> last;
			
			public VertexIterator() {
				this.innerItr = vertexes.values().iterator();
				itrVersion = version;
				last = null;
			}
			
			public boolean hasNext() {
				checkVersion();
				return innerItr.hasNext();
			}

			public Vertex next() {
				checkVersion();
				Vertex<Vertex> vertex = innerItr.next();
				last = vertex;
				return vertex.vertex();
			}

			public void remove() {
				checkVersion();
				innerItr.remove();   // removes from graph's vertices collection
				
				// need to also remove that edge from adjacency map
				// see removeVertex(V); don't want to modify version though
				removeVertexHelper(last.vertex());
				last = null;
			}
			
			private void checkVersion() {
				if (version != itrVersion) {
					throw new ConcurrentModificationException();
				}
			}
		}
	}
	*/
	
	// private class to represent neighbors() write-through set view
	/*
	private class NeighborSet extends AbstractSet<Vertex> {
		Set<Vertex> row;
		
		public NeighborSet(Vertex v) {
			row = adjacencyMap.row(v).keySet();
		}
		
		@Override
		public void clear() {
			// row.clear();
			super.clear();
		}

		@Override
		public Iterator<Vertex> iterator() {
			return new NeighborIterator();
		}

		@Override
		public int size() {
			return row.size();
		}

		private class NeighborIterator implements Iterator<Vertex> {
			private Iterator<Vertex> innerItr;
			private long itrVersion;
			private Vertex last;
			
			public NeighborIterator() {
				this.innerItr = row.iterator();
				itrVersion = version;
				last = null;
			}
			
			public boolean hasNext() {
				checkVersion();
				return innerItr.hasNext();
			}

			public Vertex next() {
				checkVersion();
				Vertex v = innerItr.next();
				last = v;
				return v;
			}

			public void remove() {
				checkVersion();
				innerItr.remove();   // removes from graph's vertices collection
				
				// need to also remove that edge from adjacency map
				// see removeVertex(V); don't want to modify version though
				removeVertexHelper(last);
				last = null;
			}
			
			private void checkVersion() {
				if (version != itrVersion) {
					throw new ConcurrentModificationException();
				}
			}
		}
	}
	*/
}
