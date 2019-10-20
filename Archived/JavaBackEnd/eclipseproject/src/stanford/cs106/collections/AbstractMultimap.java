/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public abstract class AbstractMultimap<K, V> implements Multimap<K, V> {
	private Map<K, Collection<V>> map;
	private boolean isSet;
	
	public AbstractMultimap() {
		this(false);
	}
	
	public AbstractMultimap(boolean isSet) {
		this.map = createMap();
		this.isSet = isSet;
	}
			
	
	public void clear() {
		map.clear();
	}

	protected abstract Map<K, Collection<V>> createMap();
	protected abstract Collection<V> createSet();
	
	protected Collection<V> createList() {
		return new ArrayList<V>();
	}
	
	public boolean containsEntry(Object key, Object value) {
		return containsKey(key) && map.get(key).contains(value);
	}

	public boolean containsKey(Object key) {
		return map.containsKey(key);
	}

	public boolean containsValue(Object value) {
		for (Collection<V> collection : map.values()) {
			if (collection.contains(value)) {
				return true;
			}
		}
		return false;
	}

	public Collection<V> get(K key) {
		return map.get(key);
	}

	public boolean isEmpty() {
		return map.isEmpty();
	}

	public Set<K> keySet() {
		return map.keySet();
	}

	public boolean put(K key, V value) {
		Collection<V> coll;
		if (map.containsKey(key)) {
			coll = map.get(key);
		} else {
			coll = isSet ? createSet() : createList();
			map.put(key, coll);
		}
		return coll.add(value);
	}

	public boolean remove(Object key, Object value) {
		if (containsKey(key)) {
			Collection<V> coll = map.get(key);
			boolean result = coll.remove(value);
			if (coll.isEmpty()) {
				map.remove(key);
			}
			return result;
		} else {
			return false;
		}
	}

	public Collection<V> removeAll(Object key) {
		return map.remove(key);
	}

	public int size() {
		int result = 0;
		for (Collection<V> coll : map.values()) {
			result += coll.size();
		}
		return result;
	}
	
	public String toString() {
		return map.toString();
	}
	
	public Collection<V> values() {
		Collection<V> result = createList();
		for (Collection<V> coll : map.values()) {
			result.addAll(coll);
		}
		return Collections.unmodifiableCollection(result);
	}
}
