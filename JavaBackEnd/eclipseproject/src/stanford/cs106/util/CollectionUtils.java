package stanford.cs106.util;

import java.util.*;

public class CollectionUtils {
	/** So that an object of this class cannot be constructed. */
	private CollectionUtils() {}
	
	/**
	 * Converts the elements passed into a list.
	 * Almost the same as Java's Collections.asList, but returns an ArrayList.
	 */
	// @SafeVarargs
	public static <T> ArrayList<T> asList(T... strings) {
		ArrayList<T> list = new ArrayList<T>(strings.length + 1);
		for (T s : strings) {
			list.add(s);
		}
		return list;
	}
	
	/**
	 * Converts the elements found in the given Enumeration into a list.
	 */
	public static <T> List<T> asList(Enumeration<T> enu) {
		List<T> list = new ArrayList<T>();
		while (enu != null && enu.hasMoreElements()) {
			list.add(enu.nextElement());
		}
		return list;
	}

	/**
	 * Converts the elements found in the given Iterator into a list.
	 */
	public static <T> List<T> asList(Iterator<T> itr) {
		List<T> list = new ArrayList<T>();
		while (itr != null && itr.hasNext()) {
			list.add(itr.next());
		}
		return list;
	}

	/**
	 * Converts the given pair of arrays into a map that maps each keys[i] to
	 * the corresponding values[i].
	 * @return the map (empty map if keys == null or values == null)
	 */
	public static <K, V> Map<K, V> asMap(K[] keys, V[] values) {
		Map<K, V> map = new LinkedHashMap<K, V>();
		if (keys == null || values == null) {
			return map;
		}
		
		for (int i = 0, len = Math.min(keys.length, values.length); i < len; i++) {
			map.put(keys[i], values[i]);
		}
		return map;
	}

	/**
	 * Converts the given pair of lists into a map that maps each keys[i] to
	 * the corresponding values[i].
	 * @return the map (empty map if keys == null or values == null)
	 */
	public static <K, V> Map<K, V> asMap(List<K> keys, List<V> values) {
		Map<K, V> map = new LinkedHashMap<K, V>();
		if (keys == null || values == null) {
			return map;
		}
		
		for (int i = 0, len = Math.min(keys.size(), values.size()); i < len; i++) {
			map.put(keys.get(i), values.get(i));
		}
		return map;
	}

	/**
	 * Converts the given array of object pairs into a map with each pair as key, value.
	 * Must have an even number of items.
	 * If any key or value is null, that pair is skipped.
	 * @param pairs	The pairs for the map as {..., key, value, ...} 
	 * @return the map
	 * @throws ClassCastException if an even index element is not a K or an odd element is not a V
	 */
	@SuppressWarnings("unchecked")
	public static <K, V> Map<K, V> asMap(Object... pairs) {
		if (pairs.length % 2 != 0) {
			throw new IllegalArgumentException("must pass an even number of parameters");
		}

		Map<K, V> map = new LinkedHashMap<K, V>();
		for (int i = 0; i < pairs.length - 1; i += 2) {
			if (pairs[i] != null && pairs[i + 1] != null) {
				map.put((K) pairs[i], (V) pairs[i + 1]);
			}
		}
		return map;
	}

	/**
	 * Converts the given pair of arrays into a sorted map that maps each
	 * keys[i] to the corresponding values[i].
	 * @return the map (empty map if keys == null or values == null)
	 */
	public static <K extends Comparable<? super K>, V> Map<K, V> asMapSorted(K[] keys, V[] values) {
		Map<K, V> map = new TreeMap<K, V>();
		if (keys == null || values == null) {
			return map;
		}
		
		for (int i = 0, len = Math.min(keys.length, values.length); i < len; i++) {
			map.put(keys[i], values[i]);
		}
		return map;
	}

	/**
	 * Converts the given pair of arrays into a map that maps each keys[i] to
	 * the corresponding values[i].
	 * @return the map (empty map if keys == null or values == null)
	 */
	public static <K extends Comparable<? super K>, V> Map<K, V> asMapSorted(List<K> keys, List<V> values) {
		Map<K, V> map = new TreeMap<K, V>();
		if (keys == null || values == null) {
			return map;
		}
		
		for (int i = 0, len = Math.min(keys.size(), values.size()); i < len; i++) {
			map.put(keys.get(i), values.get(i));
		}
		return map;
	}

	/**
	 * Converts the given values into a set.
	 * @return the set, or empty set if values == null
	 */
	public static <K> Set<K> asSet(K value) {
		Set<K> set = new LinkedHashSet<K>();
		if (value != null) {
			set.add(value);
		}
		return set;
	}

	/**
	 * Converts the given values into a set.
	 * @return the set, or empty set if values == null
	 */
	// @SafeVarargs
	public static <K> Set<K> asSet(K... values) {
		Set<K> set = new LinkedHashSet<K>();
		if (values == null) {
			return set;
		}
		
		for (K value : values) {
			set.add(value);
		}
		return set;
	}

	/**
	 * Converts the given values into a sorted set.
	 * @return the set, or empty set if values == null
	 */
	// @SafeVarargs
	public static <K extends Comparable<K>> Set<K> asSetSorted(K... values) {
		Set<K> set = new TreeSet<K>();
		for (K value : values) {
			set.add(value);
		}
		return set;
	}
	
	/**
	 * Returns the index of the given value in the given list,
	 * using == to compare rather than .equals.
	 */
	public static <T> int indexOfSafe(List<T> list, T value) {
		for (int i = 0; i < list.size(); i++) {
			if (list.get(i) == value) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * Combines the elements of the given collection into a string separated
	 * by commas.
	 */
	public static String join(Collection<?> c) {
		return join(c, ", ");
	}
	
	/**
	 * Combines the elements of the given collection into a string separated
	 * by the given separator.
	 */
	public static String join(Collection<?> c, String separator) {
		if (c == null) {
			return null;
		}
		
		StringBuilder result = new StringBuilder();
		int i = 0;
		for (Object o : c) {
			if (i > 0) {
				result.append(separator);
			}
			result.append(o);
			i++;
		}
		return result.toString();
	}
	
	/**
	 * Combines the elements of the given map into a string separated
	 * by an equals sign between keys and their values, and commas between each entry.
	 */
	public static String join(Map<?, ?> map) {
		return join(map, "=", ", ");
	}
	
	/**
	 * Combines the elements of the given collection into a string separated
	 * by the given kv separator between keys and their values, and
	 * the given element separator commas between each entry.
	 */
	public static String join(Map<?, ?> map, String kvSeparator, String elementSeparator) {
		if (map == null) {
			return null;
		}
		
		StringBuilder result = new StringBuilder();
		int i = 0;
		for (Map.Entry<?, ?> entry : map.entrySet()) {
			if (i > 0) {
				result.append(elementSeparator);
			}
			result.append(entry.getKey());
			result.append(kvSeparator);
			result.append(entry.getValue());
			i++;
		}
		return result.toString();
	}
}
