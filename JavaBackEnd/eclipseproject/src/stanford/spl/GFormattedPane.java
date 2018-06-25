/*
 * @version 2018/06/24
 * - added get/setContentType, get/setFont
 * @version 2018/06/23
 * - initial version
 */

package stanford.spl;

import java.awt.*;
import java.io.*;
import java.util.*;
import java.util.List;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;

import acm.util.JTFTools;

public class GFormattedPane extends GInteractor {
	private JEditorPane editorPane;
	private JScrollPane scrollPane;
	private List<DocumentListener> documentListeners;
	
	public GFormattedPane() {
		super(new JScrollPane());
		editorPane = new JEditorPane();
		editorPane.setEditorKit(JEditorPane.createEditorKitForContentType("text/html"));
		editorPane.setEditable(false);
		scrollPane = (JScrollPane) getInteractor();
		scrollPane.setViewportView(editorPane);
	}
	
	public void addDocumentListener(DocumentListener listener) {
		editorPane.getDocument().addDocumentListener(listener);
		if (documentListeners == null) {
			documentListeners = new ArrayList<DocumentListener>();
		}
		documentListeners.add(listener);
	}
	
	public boolean hasDocumentListener() {
		return documentListeners != null && !documentListeners.isEmpty();
	}
	
	public void removeDocumentListener(DocumentListener listener) {
		editorPane.getDocument().removeDocumentListener(listener);
		if (documentListeners != null) {
			documentListeners.remove(listener);
		}
	}

	public void removeDocumentListeners() {
		if (documentListeners != null) {
			for (DocumentListener listener : documentListeners) {
				editorPane.getDocument().removeDocumentListener(listener);
			}
		}
	}
	
	public void addHyperlinkListener(HyperlinkListener listener) {
		editorPane.addHyperlinkListener(listener);
	}
	
	public String getContentType() {
		return editorPane.getContentType();
	}
	
	@Override
	public Font getFont() {
		return editorPane.getFont();
	}

	public String getPage() {
		return editorPane.getPage().toString();
	}
	
	public String getText() {
		return editorPane.getText();
	}
	
	public void setContentType(String contentType) {
		editorPane.setContentType(contentType);
	}
	
	@Override
	public void setFont(Font font) {
		super.setFont(font);
		editorPane.setFont(font);
	}
	
	@Override
	public void setFont(String font) {
		super.setFont(font);
		editorPane.setFont(JTFTools.decodeFont(font));
	}
	
	public void setPage(String url) throws IOException {
		Document oldDocument = editorPane.getDocument();
		editorPane.setPage(url);
		Document newDocument = editorPane.getDocument();
		
		// reattach listeners if document changed
		if (newDocument != oldDocument && documentListeners != null) {
			for (DocumentListener listener : documentListeners) {
				editorPane.getDocument().addDocumentListener(listener);
			}
		}
	}
	
	public void setText(String text) {
		editorPane.setText(text);
	}
}
