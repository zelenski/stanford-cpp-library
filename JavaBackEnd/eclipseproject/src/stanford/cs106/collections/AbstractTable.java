/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public abstract class AbstractTable<R, C, V> implements Table<R, C, V> {
	private Map<R, Map<C, V>> map;
	
	public AbstractTable() {
		this.map = createTableMap();
	}
	
	protected abstract Map<R, Map<C, V>> createTableMap();
	protected abstract Map<C, V> createRowMap();
	protected abstract Map<R, V> createColumnMap();
	
	public void clear() {
		map.clear();
	}
	
	public Map<R, V> column(C columnKey) {
		Map<R, V> result = createColumnMap();
		for (R rowKey : map.keySet()) {
			Map<C, V> row = map.get(rowKey);
			if (row.containsKey(columnKey)) {
				result.put(rowKey, row.get(columnKey));
			}
		}
		return Collections.unmodifiableMap(result);
	}

	public boolean contains(Object rowKey, Object columnKey) {
		return map.containsKey(rowKey) && map.get(rowKey).containsKey(columnKey);
	}

	public boolean containsColumn(Object columnKey) {
		// TODO
		return false;
	}

	public boolean containsRow(Object rowKey) {
		return map.containsKey(rowKey);
	}

	public boolean containsValue(Object value) {
		// TODO
		return false;
	}

	public V get(Object rowKey, Object columnKey) {
		if (contains(rowKey, columnKey)) {
			return map.get(rowKey).get(columnKey);
		} else {
			return null;
		}
	}

	public boolean isEmpty() {
		return size() == 0;
	}

	public V put(R rowKey, C columnKey, V value) {
		Map<C, V> row = null;
		if (containsRow(rowKey)) {
			row = map.get(rowKey);
		} else {
			row = createRowMap();
			map.put(rowKey, row);
		}
		return row.put(columnKey, value);
	}

	public V remove(Object rowKey, Object columnKey) {
		if (containsRow(rowKey)) {
			Map<C, V> row = map.get(rowKey);
			V result = row.remove(columnKey);
			if (row.isEmpty()) {
				map.remove(rowKey);
			}
			return result;
		} else {
			return null;
		}
	}

	public Map<C, V> row(R rowKey) {
		return map.get(rowKey);
	}

	public Set<R> rowKeySet() {
		return map.keySet();
	}

	public Map<R, Map<C, V>> rowMap() {
		return map;
	}

	public int size() {
		int result = 0;
		for (Map<C, V> row : map.values()) {
			result += row.size();
		}
		return result;
	}
	
	public String toString() {
		return map.toString();
	}

	public Collection<V> values() {
		// TODO: this is probably really inefficient
		Collection<V> result = new ArrayList<V>();
		for (Map<C, V> column : map.values()) {
			result.addAll(column.values());
		}
		return result;
	}
}
