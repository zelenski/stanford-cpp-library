package stanford.cs106.collections;

import java.util.*;

public interface Table<R, C, V> {
	void clear();
	Map<R, V> column(C columnKey);
	boolean	contains(Object rowKey, Object columnKey);
	boolean	containsColumn(Object columnKey);
	boolean	containsRow(Object rowKey);
	boolean	containsValue(Object value);
	V get(Object rowKey, Object columnKey);
	boolean	isEmpty();
	V put(R rowKey, C columnKey, V value);
	V remove(Object rowKey, Object columnKey);
	Map<C, V> row(R rowKey);
	Set<R> rowKeySet();
	Map<R, Map<C, V>> rowMap();
	int	size();
	Collection<V> values();
}
