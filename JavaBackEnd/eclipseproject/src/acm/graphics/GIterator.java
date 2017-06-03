/*
 * @version 2017/04/27
 * - moved out from GContainer.java into its own file
 */

package acm.graphics;

import java.util.*;
import acm.util.*;

/* Package class: GIterator */
/**
 * Implements an iterator class for any object that implements
 * <code>GContainer</code> (i.e., <a href="GCanvas.html"><code>GCanvas</code></a>
 * and <a href="GCompound.html"><code>GCompound</code></a>).  The usual method
 * for using this class is to write something like</p>
 *
 * <p><pre><code>
 * &nbsp;    for (Iterator&lt;GObject&gt; i = gc.iterator(direction); i.hasNext(); )
 * </code></pre>
 *
 * where <code>gc</code> is the graphic container.  The enumeration supports
 * traversal in two directions.  By default, it starts with the front
 * element and works toward the back (as would be appropriate, for
 * example, when trying to find the topmost component for a mouse click).
 * You can, however, also process the elements of the container from back
 * to front (as would be useful when drawing elements of the container,
 * when the front objects should be drawn last).  To specify the direction
 * of the traversal, specify either <code>GContainer.FRONT_TO_BACK</code> or
 * <code>GContainer.BACK_TO_FRONT</code> in the <code>iterator</code> call.
 */
class GIterator implements Iterator<GObject> {
	/* Private instance variables */
	private GContainer cont;
	private int dir;
	private int index;
	private int nElements;

	/* Constructor: GIterator(container, direction) */
	/**
	 * Creates a new <code>GIterator</code> that runs through the
	 * container in the specified direction (<code>GContainer.FRONT_TO_BACK</code>
	 * or <code>GContainer.BACK_TO_FRONT</code>).
	 *
	 * @usage Iterator<GObject> i = new GIterator(container, direction);
	 * @param container The <code>GContainer</code> whose elements the iterator should return
	 * @param direction The direction in which to process the elements
	 */
	public GIterator(GContainer container, int direction) {
		switch (direction) {
		case GContainer.FRONT_TO_BACK: case GContainer.BACK_TO_FRONT:
			dir = direction;
			break;
		default:
			throw new ErrorException("Illegal direction for iterator");
		}
		cont = container;
		index = 0;
		nElements = container.getElementCount();
	}

	/* Method: hasNext() */
	/**
	 * Returns <code>true</code> if the iterator has more elements.  Implements
	 * the <code>hasNext</code> method for the <code>Iterator</code> interface.
	 *
	 * @usage while (i.hasNext()) . . .
	 * @return <code>true</code> if the iterator has more elements, <code>false</code> otherwise
	 */
	public boolean hasNext() {
		return index < nElements;
	}

	/* Method: next() */
	/**
	 * Returns the next element from the iterator.  Implements the <code>next</code>
	 * method for the <code>Iterator</code> interface.
	 *
	 * @usage Object element = i.next();
	 * @return The next element from the iterator
	 */
	public GObject next() {
		if (dir == GContainer.FRONT_TO_BACK) {
			return cont.getElement(nElements - index++ - 1);
		} else {
			return cont.getElement(index++);
		}
	}

	/* Method: nextElement() */
	/**
	 * Returns the next element from the iterator as a <code>GObject</code>.  This
	 * method is callable only if the iterator is declared as a <code>GIterator</code>.
	 *
	 * @usage GObject element = i.nextElement();
	 * @return The next element from the iterator as a <code>GObject</code>
	 */
	public GObject nextElement() {
		return next();
	}

	/* Method: remove() */
	/**
	 * Removes the current element from its container.  Implements the <code>remove</code>
	 * method for the <code>Iterator</code> interface.
	 *
	 * @usage i.remove();
	 */
	public void remove() {
		if (dir == GContainer.FRONT_TO_BACK) {
			cont.remove(cont.getElement(nElements - --index - 1));
		} else {
			cont.remove(cont.getElement(--index));
		}
		nElements--;
	}
}
