/*
 * A quick reimplementation of Stanford C++ library's Grid collection in Java.
 * Basically a thin wrapper around a 2-D array, but useful for porting C++
 * programs over to Java with minimal code changes.
 * 
 * @author Marty Stepp
 * @version 2015/05/28
 */

package stanford.cs106.collections;

import java.util.Arrays;

public class Grid<E> {
	private int rows, cols;
	private E[][] array;
	
	public Grid(int rows, int cols) {
		resize(rows, cols);
	}
	
	public void fill(E value) {
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				array[row][col] = value;
			}
		}
	}
	
	public E get(int row, int col) {
		checkIndex(row, col);
		return array[row][col];
	}
	
	public int height() {
		return rows;
	}
	
	public boolean inBounds(int row, int col) {
		return row >= 0 && row < rows && col >= 0 && col < cols;
	}
	
	public boolean isEmpty() {
		return rows == 0 || cols == 0;
	}
	
	public int numCols() {
		return cols;
	}
	
	public int numRows() {
		return rows;
	}
	
	@SuppressWarnings("unchecked")
	public void resize(int rows, int cols) {
		if (rows < 0 || cols < 0) {
			throw new IllegalArgumentException(rows + "," + cols);
		}
		this.rows = rows;
		this.cols = cols;
		this.array = (E[][]) new Object[rows][cols];
	}
	
	public void set(int row, int col, E value) {
		checkIndex(row, col);
		array[row][col] = value;
	}
	
	public int size() {
		return rows * cols;
	}
	
	public String toString() {
		return Arrays.deepToString(array);
	}
	
	public int width() {
		return cols;
	}
	
	private void checkIndex(int row, int col) {
		if (!inBounds(row, col)) {
			throw new IndexOutOfBoundsException(row + "," + col);
		}
	}
}
