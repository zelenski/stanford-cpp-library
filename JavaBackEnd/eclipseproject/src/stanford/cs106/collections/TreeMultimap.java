/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public class TreeMultimap<K extends Comparable<K>, V> extends AbstractMultimap<K, V> {
	public static <K extends Comparable<K>, V> TreeMultimap<K, V> create() {
		return new TreeMultimap<K, V>();
	}
	
	public static <K extends Comparable<K>, V> TreeMultimap<K, V> create(boolean isSet) {
		return new TreeMultimap<K, V>(isSet);
	}
	
	public TreeMultimap() {
		super();
	}
	
	public TreeMultimap(boolean isSet) {
		super(isSet);
	}
	
	@Override
	protected Map<K, Collection<V>> createMap() {
		return new TreeMap<K, Collection<V>>();
	}

	@Override
	protected Collection<V> createSet() {
		return new TreeSet<V>();
	}

}
