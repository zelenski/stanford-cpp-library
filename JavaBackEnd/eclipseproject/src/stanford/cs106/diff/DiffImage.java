/*
 * CS 106A Image Algorithms
 *
 * This instructor-provided file helps you view the difference between two images.
 *
 * Author : Marty Stepp
 * Version: Tue 2014/05/11
 *
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

package stanford.cs106.diff;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.net.*;

import javax.imageio.*;
import javax.swing.*;
import javax.swing.event.*;

import stanford.cs106.gui.GuiUtils;

// Reports the differences between two images.
public class DiffImage extends JPanel implements ActionListener, ChangeListener {
    private static final long serialVersionUID = 0;
    
    private BufferedImage image1;
    private BufferedImage image2;
    private String image1name;
    private int numDiffPixels;
    private int opacity = 50;
    private String label1Text = "Expected";
    private String label2Text = "Actual";
    private boolean highlightDiffs = false;
    
    private Color highlightColor = new Color(224, 0, 224);
    private JLabel image1Label;
    private JLabel image2Label;
    private JLabel diffPixelsLabel;
    private JSlider slider;
    private JCheckBox box;
    private JMenuItem saveAsItem;
    private JMenuItem setImage1Item;
    private JMenuItem setImage2Item;
    private JFrame frame;
    private JButton colorButton;
    
    /*
     * ...
     */
    public DiffImage(Image image1, Image image2) throws IOException {
        setImage1(toBufferedImage(image1));
        setImage2(toBufferedImage(image2));
        display();
    }
    
    /*
     * ...
     */
    private BufferedImage toBufferedImage(Image img) {
    	BufferedImage bimage = new BufferedImage(img.getWidth(this), img.getHeight(this), BufferedImage.TYPE_INT_RGB);
    	bimage.getGraphics().drawImage(img, 0, 0, this);
    	return bimage;
    }
    
    /*
     * ...
     */
    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == box) {
            highlightDiffs = box.isSelected();
            slider.setEnabled(!highlightDiffs);
            repaint();
        } else if (source == colorButton) {
            Color color = JColorChooser.showDialog(frame,
                                                   "Choose highlight color", highlightColor);
            if (color != null) {
                highlightColor = color;
                colorButton.setBackground(color);
                colorButton.setForeground(color);
                repaint();
            }
        } else if (source == saveAsItem) {
            saveAs();
        } else if (source == setImage1Item) {
            setImage1();
        } else if (source == setImage2Item) {
            setImage2();
        }
    }
    
    // Counts number of pixels that differ between the two images.
    public void countDiffPixels() {
        if (image1 == null || image2 == null) {
            return;
        }
        
        int w1 = image1.getWidth();
        int h1 = image1.getHeight();
        int w2 = image2.getWidth();
        int h2 = image2.getHeight();
        int wmax = Math.max(w1, w2);
        int hmax = Math.max(h1, h2);
        
        // check each pair of pixels
        numDiffPixels = 0;
        for (int y = 0; y < hmax; y++) {
            for (int x = 0; x < wmax; x++) {
                int pixel1 = (x < w1 && y < h1) ? image1.getRGB(x, y) : 0;
                int pixel2 = (x < w2 && y < h2) ? image2.getRGB(x, y) : 0;
                if (pixel1 != pixel2) {
                    numDiffPixels++;
                }
            }
        }
    }
    
    // initializes diffimage panel
    public void display() {
        countDiffPixels();
        
        setupComponents();
        setupEvents();
        setupLayout();
        
        frame.pack();
        GuiUtils.centerWindow(frame);
        
        frame.setVisible(true);
        frame.toFront();
    }
    
    // draws the given image onto the given graphics context
    public void drawImageFull(Graphics2D g2, BufferedImage image) {
        int iw = image.getWidth();
        int ih = image.getHeight();
        int w = getWidth();
        int h = getHeight();
        int dw = w - iw;
        int dh = h - ih;
        
        if (dw > 0) {
            g2.fillRect(iw, 0, dw, ih);
        }
        if (dh > 0) {
            g2.fillRect(0, ih, iw, dh);
        }
        if (dw > 0 && dh > 0) {
            g2.fillRect(iw, ih, dw, dh);
        }
        g2.drawImage(image, 0, 0, this);
    }
    
    // paints the DiffImage panel
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        
        // draw the expected output (image 1)
        if (image1 != null) {
            drawImageFull(g2, image1);
        }
        
        // draw the actual output (image 2)
        if (image2 != null) {
            Composite oldComposite = g2.getComposite();
            g2.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_ATOP, ((float) opacity) / 100));
            drawImageFull(g2, image2);
            g2.setComposite(oldComposite);
        }
        g2.setColor(Color.BLACK);
        
        // draw the highlighted diffs (if so desired)
        if (highlightDiffs && image1 != null && image2 != null) {
            int w1 = image1.getWidth();
            int h1 = image1.getHeight();
            int w2 = image2.getWidth();
            int h2 = image2.getHeight();
            
            int wmax = Math.max(w1, w2);
            int hmax = Math.max(h1, h2);
            
            // check each pair of pixels
            g2.setColor(highlightColor);
            for (int y = 0; y < hmax; y++) {
                for (int x = 0; x < wmax; x++) {
                    int pixel1 = (x < w1 && y < h1) ? image1.getRGB(x, y) : 0;
                    int pixel2 = (x < w2 && y < h2) ? image2.getRGB(x, y) : 0;
                    if (pixel1 != pixel2) {
                        g2.fillRect(x, y, 1, 1);
                    }
                }
            }
        }
    }
    
    /*
     * ...
     */
    public void save(File file) throws IOException {
        // String extension = filename.substring(filename.lastIndexOf(".") + 1);
        // ImageIO.write(diffImage, extension, new File(filename));
        String filename = file.getName();
        String extension = filename.substring(filename.lastIndexOf(".") + 1);
        BufferedImage img = new BufferedImage(getPreferredSize().width, getPreferredSize().height, BufferedImage.TYPE_INT_ARGB);
        img.getGraphics().setColor(getBackground());
        img.getGraphics().fillRect(0, 0, img.getWidth(), img.getHeight());
        paintComponent(img.getGraphics());
        ImageIO.write(img, extension, file);
    }
    
    /*
     * ...
     */
    public void save(String filename) throws IOException {
        save(new File(filename));
    }
    
    // Called when "Save As" menu item is clicked
    public void saveAs() {
//        checkChooser();
//        if (chooser.showSaveDialog(frame) != JFileChooser.APPROVE_OPTION) {
//            return;
//        }
//        
//        File selectedFile = chooser.getSelectedFile();
//        try {
//            save(selectedFile.toString());
//        } catch (IOException ex) {
//            JOptionPane.showMessageDialog(frame, "Unable to save image:\n" + ex);
//        }
    }
    
    // called when "Set Image 1" menu item is clicked
    public void setImage1() {
//        checkChooser();
//        if (chooser.showSaveDialog(frame) != JFileChooser.APPROVE_OPTION) {
//            return;
//        }
//        
//        File selectedFile = chooser.getSelectedFile();
//        try {
//            setImage1(selectedFile.toString());
//            countDiffPixels();
//            diffPixelsLabel.setText("(" + numDiffPixels + " pixels differ)");
//            image1Label.setText(selectedFile.getName());
//            frame.pack();
//        } catch (IOException ex) {
//            JOptionPane.showMessageDialog(frame, "Unable to set image 1:\n" + ex);
//        }
    }
    
    // sets image 1 to be the given image
    public void setImage1(BufferedImage image) {
        if (image == null) {
            throw new NullPointerException();
        }
        
        image1 = image;
        setPreferredSize(new Dimension(
                                       Math.max(getPreferredSize().width, image.getWidth()),
                                       Math.max(getPreferredSize().height, image.getHeight()))
                             );
        if (frame != null) {
            frame.pack();
        }
        repaint();
    }
    
    // loads image 1 from the given filename or URL
    public void setImage1(String filename) throws IOException {
        image1name = new File(filename).getName();
        if (filename.startsWith("http")) {
            setImage1(ImageIO.read(new URL(filename)));
        } else {
            setImage1(ImageIO.read(new File(filename)));
        }
    }
    
    // called when "Set Image 2" menu item is clicked
    public void setImage2() {
//        checkChooser();
//        if (chooser.showSaveDialog(frame) != JFileChooser.APPROVE_OPTION) {
//            return;
//        }
//        
//        File selectedFile = chooser.getSelectedFile();
//        try {
//            setImage2(selectedFile.toString());
//            countDiffPixels();
//            diffPixelsLabel.setText("(" + numDiffPixels + " pixels differ)");
//            image2Label.setText(selectedFile.getName());
//            frame.pack();
//        } catch (IOException ex) {
//            JOptionPane.showMessageDialog(frame, "Unable to set image 2:\n" + ex);
//        }
    }
    
    // sets image 2 to be the given image
    public void setImage2(BufferedImage image) {
        if (image == null) {
            throw new NullPointerException();
        }
        
        image2 = image;
        setPreferredSize(new Dimension(
                                       Math.max(getPreferredSize().width, image.getWidth()),
                                       Math.max(getPreferredSize().height, image.getHeight()))
                             );
        if (frame != null) {
            frame.pack();
        }
        repaint();
    }
    
    // loads image 2 from the given filename
    public void setImage2(String filename) throws IOException {
        if (filename.startsWith("http")) {
            setImage2(ImageIO.read(new URL(filename)));
        } else {
            setImage2(ImageIO.read(new File(filename)));
        }

    }
    
    private void setupComponents() {
        String title = "DiffImage";
        if (image1name != null) {
            title = "Compare to " + image1name;
        }
        frame = new JFrame(title);
        frame.setResizable(false);
        // frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        slider = new JSlider();
        slider.setPaintLabels(false);
        slider.setPaintTicks(true);
        slider.setSnapToTicks(true);
        slider.setMajorTickSpacing(25);
        slider.setMinorTickSpacing(5);
        
        box = new JCheckBox("Highlight diffs in color: ", highlightDiffs);
        
        colorButton = new JButton("X");
        colorButton.setBackground(highlightColor);
        colorButton.setForeground(highlightColor);
        colorButton.setPreferredSize(new Dimension(24, 24));
        
        diffPixelsLabel = new JLabel("(" + numDiffPixels + " pixels differ)");
        diffPixelsLabel.setFont(diffPixelsLabel.getFont().deriveFont(Font.BOLD));
        image1Label = new JLabel(label1Text);
        image2Label = new JLabel(label2Text);
        
        setupMenuBar();
    }
    
    // initializes layout of components
    private void setupLayout() {
        JPanel southPanel1 = new JPanel();
        southPanel1.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
        southPanel1.add(image1Label);
        southPanel1.add(slider);
        southPanel1.add(image2Label);
        southPanel1.add(Box.createHorizontalStrut(20));
        
        JPanel southPanel2 = new JPanel();
        southPanel2.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
        southPanel2.add(diffPixelsLabel);
        southPanel2.add(Box.createHorizontalStrut(20));
        southPanel2.add(box);
        southPanel2.add(colorButton);
        
        Container southPanel = javax.swing.Box.createVerticalBox();
        southPanel.add(southPanel1);
        southPanel.add(southPanel2);
        
        frame.add(this, BorderLayout.CENTER);
        frame.add(southPanel, BorderLayout.SOUTH);
    }
    
    // initializes main menu bar
    private void setupMenuBar() {
        saveAsItem = new JMenuItem("Save As...", 'A');
        saveAsItem.setAccelerator(KeyStroke.getKeyStroke("ctrl S"));
        setImage1Item = new JMenuItem("Set Image 1...", '1');
        setImage1Item.setAccelerator(KeyStroke.getKeyStroke("ctrl 1"));
        setImage2Item = new JMenuItem("Set Image 2...", '2');
        setImage2Item.setAccelerator(KeyStroke.getKeyStroke("ctrl 2"));
        
        JMenu file = new JMenu("File");
        file.setMnemonic('F');
        file.add(setImage1Item);
        file.add(setImage2Item);
        file.addSeparator();
        file.add(saveAsItem);
        
        JMenuBar bar = new JMenuBar();
        bar.add(file);
        
        // disabling menu bar to simplify code
        // frame.setJMenuBar(bar);
    }
    
    // method of ChangeListener interface
    public void stateChanged(ChangeEvent e) {
        opacity = slider.getValue();
        repaint();
    }
    
    // adds event listeners to various components
    private void setupEvents() {
        slider.addChangeListener(this);
        box.addActionListener(this);
        colorButton.addActionListener(this);
        saveAsItem.addActionListener(this);
        this.setImage1Item.addActionListener(this);
        this.setImage2Item.addActionListener(this);
    }
}
