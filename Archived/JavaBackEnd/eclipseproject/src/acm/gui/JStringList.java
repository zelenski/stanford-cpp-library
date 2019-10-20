/*
 * CS 106A
 *
 * This instructor-provided file represents a list of colored strings in a GUI.
 * It is basically a wrapper around the (IMO) poorly designed JList class.
 *
 * Author : Marty Stepp
 * Version: Tue 2015/05/18
 * - added getSelectedValueColor() method
 * Version: Tue 2014/06/05
 * - Added various convenience methods: setItems, setColors, getColor, etc.
 *   to facilitate modification of an existing list's items and colors
 *   for NameSurfer.
 * 
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

package acm.gui;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;
import javax.swing.*;
import javax.swing.event.*;
import stanford.cs106.gui.GuiUtils;

/**
 * Represents a list of strings with optional colors.
 */
@SuppressWarnings("rawtypes")
public class JStringList extends JList {
	private static final long serialVersionUID = 0;
	private List<Color> colorsList;
	private Set<ActionListener> listeners;   // listeners to notify
	private String actionCommand = null;     // current action command
	
	/**
	 * Constructs a new empty list of strings.
	 */
	@SuppressWarnings("unchecked")
	public JStringList() {
		super(new DefaultListModel());
		colorsList = new ArrayList<Color>();
		listeners = new LinkedHashSet<ActionListener>();
		setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		addListSelectionListener(new JStringListSelectionListener());
		// setBackground(new Color(255, 255, 255, 0));   // transparent background
	}
	
	/**
	 * Attaches the given listener to this list of strings so that it will be
	 * notified when the user clicks on items in the list.
	 * @param listener the listener to attach
	 * @throws NullPointerException if the listener is null.
	 */
	public void addActionListener(ActionListener listener) {
		ensureNotNull(listener);
		synchronized (listeners) {
			listeners.add(listener);
		}
	}
	
	/**
	 * Appends the given string to the end of this list, using the default black color.
	 * @param s the string to add
	 * @throws NullPointerException if s is null.
	 */
	public void addItem(String s) {
		addItem(s, Color.BLACK);
	}
	
	/**
	 * Appends the given string to the end of this list, using the given color.
	 * @param s the string to add
	 * @param color the color in which to display the text
	 * @throws NullPointerException if s or color are null.
	 */
	@SuppressWarnings("unchecked")
	public void addItem(String s, Color color) {
		ensureNotNull(s, color);
		getModel().addElement(wrapWithColor(s, color));
		colorsList.add(color);
	}
	
	/**
	 * Adds all of the items from the given array to this list, in black.
	 * @param items The array of items to add.
	 * @throws NullPointerException if the array or any of its elements is null.
	 */
	public void addItems(String[] items) {
		ensureNotNull((Object) items);
		for (String item : items) {
			addItem(item);
		}
	}
	
	/**
	 * Adds all of the items from the given array to this list, in the given colors.
	 * They are treated as parallel arrays; item 0 is added with color 0,
	 * item 1 is added with color 1, and so on. 
	 * If the colors array is shorter, any extra strings are added in black.
	 * If the colors array is longer, any extra colors in it are ignored.
	 * @param items The array of items to add.
	 * @param colors The array of colors to use.
	 * @throws NullPointerException if either array or any of their elements is null.
	 */
	public void addItems(String[] items, Color[] colors) {
		ensureNotNull(items, colors);
		for (int i = 0; i < items.length; i++) {
			String item = items[i];
			Color color = i < colors.length ? colors[i] : Color.BLACK;
			addItem(item, color);
		}
	}
	
	/**
	 * Removes all strings from this list.
	 */
	public void clear() {
		getModel().clear();
		colorsList.clear();
	}
	
	/**
	 * Returns the current action command of this list, which is the string that
	 * will occur when action events are performed on this list.
	 * Initially, the action command is null.
	 * @return the action command; initially null
	 */
	public String getActionCommand() {
		return actionCommand;
	}
	
	/**
	 * Returns an array of all action listeners currently listening to this list.
	 * The array returned should not be modified by the caller.
	 * If there are no listeners on this list, returns an empty array.
	 * @return the array of action listeners
	 */
	public ActionListener[] getActionListeners() {
		return listeners.toArray(new ActionListener[0]);
	}
	
	/**
	 * Returns the color of the item in the list at the given index.
	 * @param index the 0-based index of the item.
	 * @return the color at the given index.
	 * @throws ArrayIndexOutOfBoundsException If the index is outside the range of this list (0 .. size-1).
	 */
	public Color getColor(int index) {
		return colorsList.get(index);
	}
	
	/**
	 * Returns the text of the item in the list at the given index.
	 * @param index the 0-based index of the item.
	 * @return the item at the given index.
	 * @throws ArrayIndexOutOfBoundsException If the index is outside the range of this list (0 .. size-1).
	 */
	public String getItem(int index) {
		String item = (String) getModel().get(index);
		item = stripHTML(item);
		return item;
	}
	
	/**
	 * Returns the number of items in the list.
	 */
	public int getItemCount() {
		return colorsList.size();
	}
	
	/**
	 * Returns a reference to the underlying data model of this list.
	 * To client: Don't modify it.
	 */
	public DefaultListModel getModel() {
		return (DefaultListModel) super.getModel();
	}
	
	/**
	 * Returns the string that is currently selected in this list.
	 * Initially, or if no string is selected, returns null.
	 */
	public String getSelectedValue() {
		// strip out HTML tags/colors
		String selected = (String) super.getSelectedValue();
		if (selected != null) {
			selected = stripHTML(selected);
		}
		return selected;
	}
	
	/**
	 * Returns the color of the string that is currently selected in this list.
	 * Initially, or if no string is selected, returns null.
	 */
	public Color getSelectedValueColor() {
		// strip out HTML tags/colors
		int index = super.getSelectedIndex();
		if (index >= 0 && index < this.getItemCount()) {
			return getColor(index);
		} else {
			return null;
		}
	}
	
	/**
	 * Detaches the given listener so that it will not be notified of future
	 * action events that occur in this list.
	 * @param listener the listener to detach
	 * @throws NullPointerException if the listener is null.
	 */
	public void removeActionListener(ActionListener listener) {
		ensureNotNull(listener);
		synchronized (listeners) {
			listeners.remove(listener);
		}
	}
	
	/**
	 * Removes the element at the given index from this list, shifting others
	 * down an index if necessary.
	 * @throws ArrayIndexOutOfBoundsException If the index is outside the range of this list (0 .. size-1).
	 * @param index the index from which to remove
	 */
	public void removeItem(int index) {
		getModel().removeElementAt(index);
		colorsList.remove(index);
	}
	
	/**
	 * Removes the first occurrence of the given string element at the given index from this list, shifting others
	 * down an index if necessary.
	 * If the given string is not found, the call has no effect.
	 * @param s the string to remove
	 * @throws NullPointerException if the listener is null.
	 */
	public void removeItem(String s) {
		ensureNotNull(s);
		for (int i = 0; i < getModel().getSize(); i++) {
			String item = (String) getModel().get(i);
			if (item.equals(s) || stripHTML(item).equals(s)) {
				getModel().removeElement(item);
				colorsList.remove(i);
				break;
			}
		}
	}
	
	/**
	 * Sets the current action command of this list, which is the string that
	 * will occur when action events are performed on this list.
	 * Initially, the action command is null.
	 * @param actionCommand the new command string to use
	 */
	public void setActionCommand(String actionCommand) {
		this.actionCommand = actionCommand;
	}
	
	/**
	 * Sets the color of the item at the given index to the given color.
	 * The item's text is unchanged.
	 * @param index The index to modify.
	 * @param color The color to put there.
	 * @throws ArrayIndexOutOfBoundsException If the index is outside the range of this list (0 .. size-1).
	 * @throws NullPointerException if the color is null.
	 */
	@SuppressWarnings("unchecked")
	public void setColor(int index, Color color) {
		ensureNotNull(color);
		String text = getItem(index);
		getModel().set(index, wrapWithColor(text, color));
	}
	
	/**
	 * Sets the items of this list to use the colors from the given array.
	 * List item 0 will use array color 0, and so on.
	 * If the array is shorter than the length of this list, items after the
	 * array length are untouched.
	 * If the array is longer than the length of this list, items in the array
	 * past the length of the list are ignored.
	 * @param colors The array of colors to use.
	 * @throws NullPointerException if the array or any of its elements is null.
	 */
	public void setColors(Color[] colors) {
		ensureNotNull((Object) colors);
		int len = Math.min(getItemCount(), colors.length);
		for (int i = 0; i < len; i++) {
			setColor(i, colors[i]);
		}
	}
	
	/**
	 * Sets the item at the given index to store the given text.
	 * The item's color is unchanged.
	 * @param index The index to modify.
	 * @param s The text to put there.
	 * @throws ArrayIndexOutOfBoundsException If the index is outside the range of this list (0 .. size-1).
	 * @throws NullPointerException if s is null.
	 */
	@SuppressWarnings("unchecked")
	public void setItem(int index, String s) {
		ensureNotNull(s);
		Color color = colorsList.get(index);
		getModel().set(index, wrapWithColor(s, color));
	}
	
	/**
	 * Sets the item at the given index to store the given text in the given color.
	 * @param index The index to modify.
	 * @param s The text to put there.
	 * @throws ArrayIndexOutOfBoundsException If the index is outside the range of this list (0 .. size-1).
	 * @throws NullPointerException if s or color is null.
	 */
	@SuppressWarnings("unchecked")
	public void setItem(int index, String s, Color color) {
		ensureNotNull(s, color);
		colorsList.set(index, color);
		getModel().set(index, wrapWithColor(s, color));
	}
	
	/**
	 * Sets the list contents to be the given items in the given colors.
	 * @param items Array of items to add.
	 * @param colors Array of colors to use for each item.
	 * @throws NullPointerException if either array or any of their elements is null.
	 */
	public void setItems(String[] items, Color[] colors) {
		ensureNotNull(items, colors);
		clear();
		addItems(items, colors);
	}
	
	/*
	 * Helper method to check for null.
	 */
	private void ensureNotNull(Object... args) {
		for (Object arg : args) {
			if (arg == null) {
				throw new NullPointerException();
			}
		}
	}
	
	/*
	 * Causes an action event to occur, notifying all current action listeners.
	 */
	private void fireActionPerformed() {
		ActionEvent event = new ActionEvent(this, -1, actionCommand);
		synchronized (listeners) {
			for (ActionListener listener : listeners) {
				listener.actionPerformed(event);
			}
		}
	}
	
	/*
	 * Removes any HTML tags from the given string.
	 * Used to remove coloring.
	 */
	private String stripHTML(String s) {
		return s.replaceAll("<[^>]*>", "");
	}
	
	/*
	 * Converts the given Color into a RGB hex string like "#FF00FF" for purple.
	 */
	private String toRgbString(Color color) {
		return String.format("%02x%02x%02x", color.getRed(), color.getGreen(), color.getBlue());
	}
	
	/*
	 * Returns the given string wrapped with HTML tags to give it the given
	 * color when displayed on the screen.
	 */
	private String wrapWithColor(String s, Color c) {
		String colorString = toRgbString(c);
		return GuiUtils.htmlLabelText(s, Collections.singletonMap("font-color", colorString));
	}
	
	/*
	 * A class for converting list selection events into action events to ease
	 * usage of this class by introductory students.
	 */
	private class JStringListSelectionListener implements ListSelectionListener {
		public void valueChanged(ListSelectionEvent event) {
			if (!event.getValueIsAdjusting()) {
				fireActionPerformed();
			}
		}
	}
}
