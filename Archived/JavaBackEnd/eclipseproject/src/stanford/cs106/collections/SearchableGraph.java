///*
// * @author Marty Stepp
// * @version 2015/06/06
// */
//
//package stanford.cs106.collections;
//
//import java.util.*;
//// import com.google.common.collect.*;
//
///**
// * A SearchableGraph implements the Graph interface by writing three path-finding
// * methods: isReachable (DFS), shortestPath (BFS), and minimumWeightPath (Dijkstra's
// * algorithm).  It builds on the partial implementation found in the AbstractGraph class.
// */
//public class SearchableGraph extends AbstractGraph {
//	/** 
//	 * Constructs a new empty undirected, unweighted graph.
//	 */
//	public SearchableGraph() {
//		super();
//	}
//	
//	/** 
//	 * Constructs a new empty graph that can be directed or undirected,
//	 * weighted or unweighted.
//	 */
//	public SearchableGraph(boolean directed, boolean weighted) {
//		super(directed, weighted);
//	}
//	
//	
//	/**
//	 * Returns true if there is any path in this graph that 
//	 * leads from the given starting vertex v1 to the given ending vertex v2. 
//	 * Uses the Depth-First Search (DFS) algorithm to find the path.
//	 * If there is no path to v2 from v1, the method returns false. 
//	 * @param v1 The starting vertex.
//	 * @param v2 The ending vertex.
//	 * @throws IllegalArgumentException If either vertex is not part of the graph.
//	 * @throws NullPointerException If any object parameter is null.
//	 */
//	public boolean isReachable(V v1, V v2) {
//		checkVertexes(v1, v2);
//		clearVertexInfo();
//		return isReachableHelper(v1, v2);
//	}
//	
//	// private recursive DFS helper for isReachable
//	private boolean isReachableHelper(V v1, V v2) {
//		if (v1.equals(v2)) {
//			return true;
//		}
//		
//		vertexInfo(v1).setVisited();
//		for (V neighbor : neighbors(v1)) {
//			if (!vertexInfo(neighbor).visited() && isReachableHelper(neighbor, v2)) {
//				return true;
//			}
//		}
//		
//		return false;
//	}
//	
//	/**
//	 * Returns true if there is any path in this graph that 
//	 * leads from the given starting vertex v1 to the given ending vertex v2. 
//	 * Uses the Depth-First Search (DFS) algorithm to find the path.
//	 * If there is no path to v2 from v1, the method returns false. 
//	 * @param v1 The starting vertex.
//	 * @param v2 The ending vertex.
//	 * @throws IllegalArgumentException If either vertex is not part of the graph.
//	 * @throws NullPointerException If any object parameter is null.
//	 */
//	public List<V> dfs(V v1, V v2) {
//		checkVertexes(v1, v2);
//		clearVertexInfo();
//		List<V> path = new ArrayList<V>();
//		dfsHelper(v1, v2, path);
//		return path;
//	}
//	
//	// private recursive DFS helper for isReachable
//	private boolean dfsHelper(V v1, V v2, List<V> path) {
//		vertexInfo(v1).setVisited();
//		path.add(v1);
//		if (v1.equals(v2)) {
//			return true;
//		}
//		
//		for (V neighbor : neighbors(v1)) {
//			if (!vertexInfo(neighbor).visited() && isReachableHelper(neighbor, v2)) {
//				return true;
//			}
//		}
//		
//		// vertexInfo(v1).setVisited(false);
//		path.remove(path.size() - 1);
//		return false;
//	}
//	
//	/**
//	 * Returns a list of vertices representing a minimum-weight path from vertex
//	 * v1 to vertex v2.
//	 * If v2 is not reachable from v1, returns null.
//	 * Uses Dijkstra's Algorithm to find a minimum-weight path.
//	 * @param v1 The starting vertex.
//	 * @param v2 The ending vertex.
//	 * @throws IllegalArgumentException If either vertex is not part of the graph.
//	 * @throws NullPointerException If any object parameter is null.
//	 */
//	public List<V> dijkstrasAlgorithm(V v1, V v2) {
//		checkVertexes(v1, v2);
//		clearVertexInfo();
//		
//		// optional optimization: if v1 and v2 are same, return path immediately
//		if (v1.equals(v2)) {
//			List<V> path = new LinkedList<V>();
//			path.add(v1);
//			return path;
//		}
//		
//		vertexInfo(v1).setCost(0);
//		Collection<V> vertices = this.vertexes();
//		Queue<V> pq = new PriorityQueue<V>(vertices.size() + 10, new VertexCostComparator());
//		pq.addAll(vertices);
//
//		while (!pq.isEmpty()) {
//			V minVertex = pq.remove();
//			Vertex<V> minVertexInfo = vertexInfo(minVertex);
//			minVertexInfo.setVisited();
//			if (minVertexInfo.cost() == Vertex.MAX_COST) {
//				// all remaining vertices have infinite cost; abort
//				pq.clear();
//				continue;
//			}
//			
//			for (V neighbor : neighbors(minVertex)) {
//				Vertex<V> neighborInfo = vertexInfo(neighbor);
//				if (!neighborInfo.visited()) {
//					double newCost = minVertexInfo.cost() + edgeWeight(minVertex, neighbor);
//					if (newCost < neighborInfo.cost()) {
//						pq.remove(neighbor);
//						neighborInfo.setCost(newCost);
//						neighborInfo.setPrevious(minVertex);
//						pq.add(neighbor);  // have to remove/re-add because ordering changes
//					}
//				}
//			}
//		}
//		
//		if (Math.abs(vertexInfo(v2).cost()) >= Vertex.MAX_COST) {
//			return null;   // no path was found
//		} else {
//			return reconstructPath(v1, v2);
//		}
//	}
//
//	/**
//	 * Returns the path in this graph with the least number of vertices that 
//	 * leads from the given starting vertex v1 to the given ending vertex v2. 
//	 * Uses the Breadth-First Search (BFS) algorithm to find the path.
//	 * If v2 is not reachable from v1, the method returns null. 
//	 * @param v1 The starting vertex.
//	 * @param v2 The ending vertex.
//	 * @throws IllegalArgumentException If either vertex is not part of the graph.
//	 * @throws NullPointerException If any object parameter is null.
//	 */
//	public List<V> bfs(V v1, V v2) {
//		checkVertexes(v1, v2);
//		clearVertexInfo();
//		List<V> path = null;
//		
//		Queue<V> queue = new LinkedList<V>();
//		queue.add(v1);
//		vertexInfo(v1).setVisited();
//		
//		while (!queue.isEmpty()) {
//			V v = queue.remove();
//			if (v.equals(v2)) {
//				path = reconstructPath(v1, v);
//				break;
//			} else {
//				for (V n : neighbors(v)) {
//					Vertex<V> vi = vertexInfo(n);
//					if (!vi.visited()) {
//						vi.setVisited(true);
//						vi.setPrevious(v);
//						queue.add(n);
//					}
//				}
//			}
//		}
//		
//		return path;
//	}
//	
//	public List<V> topologicalSort() {
//		clearVertexInfo();
//		Map<V, Integer> degree = new HashMap<V, Integer>();
//		Queue<V> queue = new LinkedList<V>();
//		for (V v : vertexes()) {
//			int indeg = inDegree(v);
//			degree.put(v, indeg);
//			if (indeg == 0) {
//				queue.add(v);
//			}
//		}
//		
//		List<V> out = new ArrayList<V>();
//		while (!queue.isEmpty()) {
//			V v = queue.remove();
//			out.add(v);
//			for (V n : neighbors(v)) {
//				int nIndeg = degree.get(n);
//				if (nIndeg == 1) {
//					degree.remove(n);
//					queue.add(n);
//				} else {
//					degree.put(n, nIndeg - 1);
//				}
//			}
//		}
//		
//		if (out.size() == vertexCount()) {
//			return out;
//		} else {
//			return null;
//		}
//	}
//	
////	public void topologicalSortAll() {
////		clearVertexInfo();
////		Multiset<V> degree = HashMultiset.create();
////		for (V v : vertices()) {
////			int indeg = inDegree(v);
////			degree.setCount(v, indeg);
////		}
////		List<V> out = new ArrayList<V>();
////		topologicalSortAll(degree, out);
////	}
////	
////	private void topologicalSortAll(Multiset<V> degree, List<V> out) {
////		// System.out.println("  topoAll(" + out + ", deg=" + degree + ")");
////		
////		if (out.size() == vertexCount()) {
////			System.out.println(out);
////			return;
////		}
////		
////		for (V v : vertices()) {
////			if (!vertexInfo(v).visited() && !degree.contains(v)) {
////				// choose
////				vertexInfo(v).setVisited();
////				out.add(v);
////				for (V n : neighbors(v)) {
////					degree.remove(n);   // decrease count by 1
////				}
////				
////				// explore
////				topologicalSortAll(degree, out);
////
////				// un-choose
////				for (V n : neighbors(v)) {
////					degree.add(n);   // increase count by 1
////				}
////				vertexInfo(v).setVisited(false);
////				out.remove(out.size() - 1);
////			}
////		}
////	}
//	
//	// rebuilds path back from v1 to v2 by walking backwards from v2 to v1 through
//	// the 'previous' markers in each vertex
//	private List<V> reconstructPath(V v1, V v2) {
//		List<V> path = new LinkedList<V>();
//		while (!v2.equals(v1)) {
//			path.add(0, v2);
//			v2 = vertexInfo(v2).previous();
//		}
//		path.add(0, v1);
//		return path;
//	}
//
//	private class VertexCostComparator implements Comparator<V> {
//		/**
//		 * Compares vertex 'V' objects based on their related cost to be reached.
//		 * Used to order the priority queue in Dijkstra's algorithm.
//		 */
//		public int compare(V vertex1, V vertex2) {
//			return Double.compare(vertexInfo(vertex1).cost(), vertexInfo(vertex2).cost());
//		}
//	}
//	
//	
//	////////////// STUDENTS DON'T NEED TO WRITE THE METHODS BELOW //////////////
//	
//	/**
//	 * A Guava-like static factory method that returns a new searchable
//	 * undirected, unweighted graph.
//	 */
//	public static <V, E> SearchableGraph<V, E> create() {
//		return create(false, false);
//	}
//	
//	/**
//	 * A Guava-like static factory method that returns a new empty unweighted 
//	 * searchable graph that can be directed or undirected.
//	 */
//	public static <V, E> SearchableGraph<V, E> create(boolean directed) {
//		return create(directed, false);
//	}
//	
//	/**
//	 * A Guava-like static factory method that returns a new empty searchable graph
//	 * that can be directed or undirected, weighted or unweighted.
//	 */
//	public static <V, E> SearchableGraph<V, E> create(boolean directed, boolean weighted) {
//		return new SearchableGraph<V, E>(directed, weighted);
//	}
//}
