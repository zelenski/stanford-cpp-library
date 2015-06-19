/*
 * @author Marty Stepp
 * @version 2015/06/06
 */

package stanford.cs106.collections;

import java.util.*;

public class TreeBasedTable
	<R extends Comparable<? super R>, C extends Comparable<? super C>, V>
		extends AbstractTable<R, C, V> {
	public static <R extends Comparable<? super R>, C extends Comparable<? super C>, V>
			TreeBasedTable<R, C, V> create() {
		return new TreeBasedTable<R, C, V>();
	}
	
	@Override
	protected Map<R, Map<C, V>> createTableMap() {
		return new TreeMap<R, Map<C, V>>();
	}

	@Override
	protected Map<R, V> createColumnMap() {
		return new TreeMap<R, V>();
	}

	@Override
	protected Map<C, V> createRowMap() {
		return new TreeMap<C, V>();
	}
}
