/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public class HashMultimap<K, V> extends AbstractMultimap<K, V> {
	public static <K, V> HashMultimap<K, V> create() {
		return new HashMultimap<K, V>();
	}
	
	public static <K, V> HashMultimap<K, V> create(boolean isSet) {
		return new HashMultimap<K, V>(isSet);
	}
	
	public HashMultimap() {
		super();
	}
	
	public HashMultimap(boolean isSet) {
		super(isSet);
	}
	
	@Override
	protected Map<K, Collection<V>> createMap() {
		return new HashMap<K, Collection<V>>();
	}

	@Override
	protected Collection<V> createSet() {
		return new HashSet<V>();
	}

}
