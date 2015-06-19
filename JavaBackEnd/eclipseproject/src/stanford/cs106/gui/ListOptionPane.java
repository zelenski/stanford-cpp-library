package stanford.cs106.gui;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

/**
 * A pop-up dialog box of options as a clickable list.
 * A sort of expansion of the JOptionPane functionality provided in Java Swing.
 * @author Marty Stepp
 * @version 2015/05/09
 */
@SuppressWarnings({"rawtypes", "unchecked", "deprecation"})
public class ListOptionPane extends JDialog implements ActionListener {
	private static final long serialVersionUID = 0;

	private JList list;

	private JButton ok, cancel;

	private boolean pressedOk = false;

	public ListOptionPane(JFrame frame, Collection<String> items) {
		super(frame, true);
		setTitle("Load...");

		list = new JList(items.toArray());
		list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		list.setVisibleRowCount(Math.min(12, items.size()));
		list.addListSelectionListener(new ListOptionPaneListSelectionListener());

		JPanel south = new JPanel();
		ok = GuiUtils.createButton("OK", null, 'O', this);
		south.add(ok);
		cancel = GuiUtils.createButton("Cancel", null, 'C', this);
		south.add(cancel);

		add(new JScrollPane(list));
		add(south, BorderLayout.SOUTH);
		pack();
		if (frame != null) {
			setLocation(frame.getX() + (frame.getWidth() - getWidth()) / 2, frame
					.getY()
					+ (frame.getHeight() - getHeight()) / 2);
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

