/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public class HashBasedTable<R, C, V> extends AbstractTable<R, C, V> {
	public static <R, C, V> HashBasedTable<R, C, V> create() {
		return new HashBasedTable<R, C, V>();
	}
	
	@Override
	protected Map<R, Map<C, V>> createTableMap() {
		return new HashMap<R, Map<C, V>>();
	}

	@Override
	protected Map<R, V> createColumnMap() {
		return new HashMap<R, V>();
	}

	@Override
	protected Map<C, V> createRowMap() {
		return new HashMap<C, V>();
	}
}
