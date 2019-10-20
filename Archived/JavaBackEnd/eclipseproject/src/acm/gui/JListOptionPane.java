/*
 * @author Marty Stepp
 * @version 2017/10/22
 * - added String... items overloads
 * - added static method showInputDialog
 * @version 2017/06/07
 * - added version that accepts a title
 * @version 2015/05/09
 * - initial version
 */

package acm.gui;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import stanford.cs106.gui.GuiUtils;

/**
 * A pop-up dialog box of options as a clickable list.
 * A sort of expansion of the JOptionPane functionality provided in Java Swing.
 */
public class JListOptionPane extends JDialog implements ActionListener {
	private static final int HPADDING = 20;
	private static final int VPADDING = 0;
	
	public static int showInputDialog(Frame frame, String message, String title, String... items) {
		JListOptionPane dialog = new JListOptionPane(frame, message, title, items);
		dialog.setMultipleSelection(false);
		dialog.setVisible(true);
		return dialog.getSelectedIndex();
	}
	
	public static int showInputDialog(Frame frame, String message, String title, Iterable<String> items) {
		JListOptionPane dialog = new JListOptionPane(frame, message, title, items);
		dialog.setMultipleSelection(false);
		dialog.setVisible(true);
		return dialog.getSelectedIndex();
	}
	
	private static final long serialVersionUID = 0;

	@SuppressWarnings("rawtypes")
	private JList list;

	private JButton ok, cancel;

	private boolean pressedOk = false;

	public JListOptionPane(Frame frame, Iterable<String> items) {
		this(frame, /* message */ "", /* title */ "Select an option", items);
	}

	public JListOptionPane(Frame frame, String message, Iterable<String> items) {
		this(frame, message, /* title */ "Select an option", items);
	}

	public JListOptionPane(Frame frame, String message, String title, String... items) {
		this(frame, message, title, Arrays.asList(items));
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public JListOptionPane(Frame frame, String message, String title, Iterable<String> items) {
		super(frame, /* modal */ true);
		
		if (title != null && !title.isEmpty()) {
			setTitle(title);
		}

		List<String> itemList = new ArrayList<String>();
		for (String item : items) {
			itemList.add(item);
		}
		list = new JList(itemList.toArray());
		list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		list.setVisibleRowCount(Math.min(12, itemList.size()));
		list.addListSelectionListener(new ListOptionPaneListSelectionListener());

		JPanel south = new JPanel();
		ok = GuiUtils.createButton("OK", null, 'O', this);
		south.add(ok);
		cancel = GuiUtils.createButton("Cancel", null, 'C', this);
		south.add(cancel);

		if (message != null && !message.isEmpty()) {
			JPanel north = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 5));
			JLabel label = new JLabel(message);
			label.setHorizontalAlignment(SwingConstants.CENTER);
			GuiUtils.pad(label, HPADDING, VPADDING);
			north.add(label);
			add(north, BorderLayout.NORTH);
		}
		GuiUtils.pad(list, HPADDING, VPADDING);
		
		add(new JScrollPane(list));
		add(south, BorderLayout.SOUTH);
		pack();
		
		if (frame != null) {
			// center with respect to frame
			setLocation(frame.getX() + (frame.getWidth() - getWidth()) / 2,
					frame.getY() + (frame.getHeight() - getHeight()) / 2);
		} else {
			GuiUtils.centerWindow(this);
		}
		
		KeyListener keyListener = new ListOptionPaneKeyListener();
		addKeyListener(keyListener);
		
		ok.requestFocus();
		ok.addKeyListener(keyListener);
		cancel.addKeyListener(keyListener);
	}

	public void actionPerformed(ActionEvent e) {
		Object src = e.getSource();
		if (src == ok) {
			pressedOk = true;
		} else if (src == cancel) {
			setVisible(false);
			// dispose();
		}
		setVisible(false);
	}

	public int getSelectedIndex() {
		return list.getSelectedIndex();
	}

	public Object getSelectedValue() {
		return list.getSelectedValue();
	}

	@SuppressWarnings("deprecation")
	public Object[] getSelectedValues() {
		return list.getSelectedValues();
	}

	public boolean hasSelectedValue() {
		return list.getSelectedIndex() >= 0;
	}

	public boolean pressedOk() {
		return pressedOk;
	}
	
	public void setMultipleSelection(boolean enabled) {
		if (enabled) {
			list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		} else {
			list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		}
	}
	
	private class ListOptionPaneListSelectionListener implements ListSelectionListener {
		public void valueChanged(ListSelectionEvent arg0) {
			if (hasSelectedValue()) {
				new Thread(new Runnable() {
					public void run() {
						try { Thread.sleep(300); } catch (InterruptedException e) {
							// empty
						}
						pressedOk = true;
						setVisible(false);
					}
				}).start();
			}
		}
	}
	
	private class ListOptionPaneKeyListener extends KeyAdapter {
		public void keyPressed(KeyEvent event) {
			if (event.getKeyCode() == KeyEvent.VK_ESCAPE) {
				setVisible(false);
			}
		}
	}
}

