/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public interface Multimap<K, V> {
	void clear();
	boolean	containsEntry(Object key, Object value);
	boolean	containsKey(Object key);
	boolean	containsValue(Object value);
	Collection<V> get(K key);
	boolean	isEmpty();
	Set<K> keySet();
	boolean	put(K key, V value);
	boolean	remove(Object key, Object value);
	Collection<V> removeAll(Object key);
	int	size();
	Collection<V> values();
}
