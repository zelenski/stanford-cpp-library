// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   TableLayout.java

package acm.gui;

import acm.util.ErrorException;
import acm.util.OptionTable;
import java.awt.*;
import java.io.Serializable;
import java.lang.reflect.Method;
import java.util.*;
import javax.swing.JScrollBar;
import javax.swing.JSlider;

// Referenced classes of package acm.gui:
//            TableConstraints

public class TableLayout
    implements LayoutManager2, Serializable
{

    public TableLayout()
    {
        this(0, 0);
    }

    public TableLayout(int i, int j)
    {
        this(i, j, 0, 0);
    }

    public TableLayout(int i, int j, int k, int l)
    {
        nRows = i;
        nColumns = j;
        hGap = k;
        vGap = l;
        horizontalAlignment = 10;
        verticalAlignment = 10;
        defaultFill = 1;
        constraintTable = new HashMap();
        propertyTable = new HashMap();
        layoutTable = null;
    }

    public void setColumnCount(int i)
    {
        nColumns = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getColumnCount()
    {
        return nColumns;
    }

    public void setRowCount(int i)
    {
        nRows = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getRowCount()
    {
        return nRows;
    }

    public void setHorizontalAlignment(int i)
    {
        horizontalAlignment = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getHorizontalAlignment()
    {
        return horizontalAlignment;
    }

    public void setVerticalAlignment(int i)
    {
        verticalAlignment = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getVerticalAlignment()
    {
        return verticalAlignment;
    }

    public void setDefaultFill(int i)
    {
        defaultFill = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getDefaultFill()
    {
        return defaultFill;
    }

    public void setHgap(int i)
    {
        hGap = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getHgap()
    {
        return hGap;
    }

    public void setVgap(int i)
    {
        vGap = i;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public int getVgap()
    {
        return vGap;
    }

    public void setStrictGridBagModel(boolean flag)
    {
        useStrictGridBagModel = flag;
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public boolean isStrictGridBagModel()
    {
        return useStrictGridBagModel;
    }

    public void setConstraints(Component component, GridBagConstraints gridbagconstraints)
    {
        constraintTable.put(component, (GridBagConstraints)gridbagconstraints.clone());
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public void setConstraints(Component component, String s)
    {
        setConstraints(component, ((GridBagConstraints) (new TableConstraints(s))));
    }

    public TableConstraints getConstraints(Component component)
    {
        GridBagConstraints gridbagconstraints = lookupConstraints(component);
        return gridbagconstraints != null ? new TableConstraints(gridbagconstraints) : null;
    }

    public void addLayoutComponent(String s, Component component)
    {
        addLayoutComponent(component, s);
    }

    public void removeLayoutComponent(Component component)
    {
        constraintTable.remove(component);
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public Dimension preferredLayoutSize(Container container)
    {
        if(container.getComponentCount() == 0)
            return new Dimension(0, 0);
        else
            return processLayout(container, 2);
    }

    public Dimension minimumLayoutSize(Container container)
    {
        if(container.getComponentCount() == 0)
            return new Dimension(0, 0);
        else
            return processLayout(container, 1);
    }

    public void layoutContainer(Container container)
    {
        targetContainer = container;
        processLayout(container, 0);
    }

    public void addLayoutComponent(Component component, Object obj)
    {
        if(obj == null)
        {
            obj = new TableConstraints("");
            ((TableConstraints)obj).fill = defaultFill;
        } else
        if(obj instanceof String)
        {
            OptionTable optiontable = new OptionTable(((String)obj).toLowerCase(), TableConstraints.LEGAL_KEYS);
            obj = new TableConstraints(optiontable.getMap());
            if(!optiontable.isSpecified("fill"))
                ((TableConstraints)obj).fill = optiontable.isSpecified("anchor") ? 0 : defaultFill;
        } else
        if(!(obj instanceof GridBagConstraints))
            throw new ErrorException("TableLayout: Illegal constraints");
        constraintTable.put(component, (GridBagConstraints)obj);
        if(targetContainer != null)
            targetContainer.invalidate();
    }

    public Dimension maximumLayoutSize(Container container)
    {
        return new Dimension(0x7fffffff, 0x7fffffff);
    }

    public float getLayoutAlignmentX(Container container)
    {
        return 0.5F;
    }

    public float getLayoutAlignmentY(Container container)
    {
        return 0.5F;
    }

    public synchronized void invalidateLayout(Container container)
    {
        layoutTable = null;
        Iterator iterator = propertyTable.keySet().iterator();
        do
        {
            if(!iterator.hasNext())
                break;
            String s = (String)iterator.next();
            if(s.startsWith("width") && !s.equals("width"))
                propertyTable.put(s, new Integer(0));
            if(s.startsWith("height") && !s.equals("height"))
                propertyTable.put(s, new Integer(0));
            if(s.startsWith("weightx") && !s.equals("weightx"))
                propertyTable.put(s, new Double(0.0D));
            if(s.startsWith("weighty") && !s.equals("weighty"))
                propertyTable.put(s, new Double(0.0D));
        } while(true);
    }

    public String toString()
    {
        String s = getClass().getName();
        s = (new StringBuilder()).append(s).append("[rows=").append(nRows).append(",columns=").append(nColumns).toString();
        if(hGap != 0)
            s = (new StringBuilder()).append(s).append(",hgap=").append(hGap).toString();
        if(vGap != 0)
            s = (new StringBuilder()).append(s).append(",vgap=").append(vGap).toString();
        s = (new StringBuilder()).append(s).append("]").toString();
        return s;
    }

    protected GridBagConstraints lookupConstraints(Component component)
    {
        return (GridBagConstraints)constraintTable.get(component);
    }

    protected TableConstraints lookupConstraints(Component component, Container container)
    {
        Object obj = container.getTreeLock();
        JVM INSTR monitorenter ;
        if(layoutTable == null)
            computeLayoutTable(container);
        return (TableConstraints)layoutTable.get(component);
        Exception exception;
        exception;
        throw exception;
    }

    private Dimension getMinimumComponentSize(Component component)
    {
        if(isScrollbar(component))
            return getMinimumScrollbarSize(component);
        else
            return component.getMinimumSize();
    }

    private Dimension getPreferredComponentSize(Component component)
    {
        if(isScrollbar(component))
            return getMinimumScrollbarSize(component);
        else
            return component.getPreferredSize();
    }

    private boolean isScrollbar(Component component)
    {
        return (component instanceof Scrollbar) || (component instanceof JScrollBar) || (component instanceof JSlider);
    }

    private Dimension getMinimumScrollbarSize(Component component)
    {
        Dimension dimension = component.getMinimumSize();
        try
        {
            Class class1 = component.getClass();
            Method method = class1.getMethod("getOrientation", new Class[0]);
            int i = ((Integer)method.invoke(component, new Object[0])).intValue();
            if(i == 0)
                dimension.width = Math.max(dimension.width, 100);
            else
                dimension.height = Math.max(dimension.height, 100);
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
        return dimension;
    }

    private Dimension processLayout(Container container, int i)
    {
        Object obj = container.getTreeLock();
        JVM INSTR monitorenter ;
        return lockedProcessLayout(container, i);
        Exception exception;
        exception;
        throw exception;
    }

    private synchronized Dimension lockedProcessLayout(Container container, int i)
    {
        Dimension dimension = null;
        if(layoutTable == null)
            computeLayoutTable(container);
        int j = getIntProperty("nRows");
        int k = getIntProperty("nColumns");
        int ai[] = new int[j];
        int ai1[] = new int[k];
        int l = container.getComponentCount();
        for(int i1 = 0; i1 < l; i1++)
        {
            Component component = container.getComponent(i1);
            TableConstraints tableconstraints = (TableConstraints)layoutTable.get(component);
            int l1 = tableconstraints.gridx;
            int i2 = tableconstraints.gridy;
            int j2 = getIntProperty((new StringBuilder()).append("width").append(l1).toString());
            int k2 = getIntProperty((new StringBuilder()).append("height").append(i2).toString());
            if(j2 == 0)
            {
                if(i == 1 || useStrictGridBagModel)
                    j2 = getMinimumComponentSize(component).width;
                else
                    j2 = getPreferredComponentSize(component).width;
                j2 += 2 * tableconstraints.ipadx + tableconstraints.insets.left + tableconstraints.insets.right;
            }
            if(k2 == 0)
            {
                if(i == 1 || useStrictGridBagModel)
                    k2 = getMinimumComponentSize(component).height;
                else
                    k2 = getPreferredComponentSize(component).height;
                k2 += 2 * tableconstraints.ipady + tableconstraints.insets.top + tableconstraints.insets.bottom;
            }
            if(tableconstraints.gridwidth <= 1)
                ai1[l1] = Math.max(ai1[l1], j2);
            if(tableconstraints.gridheight <= 1)
                ai[i2] = Math.max(ai[i2], k2);
        }

        int j1 = hGap;
        int k1 = vGap;
        double ad[] = new double[k];
        double ad1[] = new double[j];
        double d = 0.0D;
        double d1 = 0.0D;
        for(int l2 = 0; l2 < k; l2++)
        {
            j1 += ai1[l2] + hGap;
            ad[l2] = getDoubleProperty((new StringBuilder()).append("weightx").append(l2).toString());
            d += ad[l2];
        }

        for(int i3 = 0; i3 < j; i3++)
        {
            k1 += ai[i3] + vGap;
            ad1[i3] = getDoubleProperty((new StringBuilder()).append("weighty").append(i3).toString());
            d1 += ad1[i3];
        }

        if(i == 0)
        {
            Dimension dimension1 = container.getSize();
            Insets insets = container.getInsets();
            dimension1.width -= insets.left + insets.right;
            dimension1.height -= insets.top + insets.bottom;
            int j3 = dimension1.width - j1;
            int k3 = dimension1.height - k1;
            int l3 = insets.left;
            int i4 = insets.top;
            if(d == 0.0D)
                switch(horizontalAlignment)
                {
                default:
                    break;

                case 11: // '\013'
                    j3 = 0;
                    break;

                case 10: // '\n'
                    l3 += j3 / 2;
                    j3 = 0;
                    break;

                case 12: // '\f'
                    l3 += j3;
                    j3 = 0;
                    break;

                case 1: // '\001'
                    d = k;
                    for(int j4 = 0; j4 < k; j4++)
                        ad[j4] = 1.0D;

                    break;
                }
            if(d1 == 0.0D)
                switch(verticalAlignment)
                {
                default:
                    break;

                case 13: // '\r'
                    k3 = 0;
                    break;

                case 10: // '\n'
                    i4 += k3 / 2;
                    k3 = 0;
                    break;

                case 14: // '\016'
                    i4 += k3;
                    k3 = 0;
                    break;

                case 1: // '\001'
                    d1 = j;
                    for(int k4 = 0; k4 < j; k4++)
                        ad1[k4] = 1.0D;

                    break;
                }
            int ai2[] = new int[k];
            int l4 = hGap + l3;
            for(int i5 = 0; i5 < k; i5++)
            {
                ai2[i5] = l4;
                if(j3 > 0)
                {
                    int j5 = (int)Math.round(((double)j3 * ad[i5]) / d);
                    ai1[i5] += j5;
                    j3 -= j5;
                    d -= ad[i5];
                }
                l4 += ai1[i5] + hGap;
            }

            int ai3[] = new int[j];
            int k5 = vGap + i4;
            for(int l5 = 0; l5 < j; l5++)
            {
                ai3[l5] = k5;
                if(k3 > 0)
                {
                    int j6 = (int)Math.round(((double)k3 * ad1[l5]) / d1);
                    ai[l5] += j6;
                    k3 -= j6;
                    d1 -= ad1[l5];
                }
                k5 += ai[l5] + vGap;
            }

            for(int i6 = 0; i6 < l; i6++)
            {
                Component component1 = container.getComponent(i6);
                TableConstraints tableconstraints1 = (TableConstraints)layoutTable.get(component1);
                int k6 = tableconstraints1.gridx;
                int l6 = tableconstraints1.gridy;
                int i7 = ai2[k6] + tableconstraints1.insets.left;
                int j7 = ai3[l6] + tableconstraints1.insets.top;
                int k7 = ai1[k6];
                for(int l7 = 1; l7 < tableconstraints1.gridwidth && k6 + l7 < k; l7++)
                    k7 += ai1[k6 + l7] + hGap;

                k7 -= tableconstraints1.insets.left + tableconstraints1.insets.right;
                int i8 = ai[l6];
                for(int j8 = 1; j8 < tableconstraints1.gridheight && l6 + j8 < j; j8++)
                    i8 += ai[l6 + j8] + vGap;

                i8 -= tableconstraints1.insets.top + tableconstraints1.insets.bottom;
                Dimension dimension2 = useStrictGridBagModel ? getMinimumComponentSize(component1) : getPreferredComponentSize(component1);
                Rectangle rectangle = computeCellBounds(new Rectangle(i7, j7, k7, i8), dimension2, tableconstraints1);
                component1.setBounds(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
            }

        } else
        {
            dimension = new Dimension(j1, k1);
        }
        return dimension;
    }

    private void computeLayoutTable(Container container)
    {
        int ai[] = null;
        int i = 0;
        int j = 0;
        int k = nColumns;
        int l = container.getComponentCount();
        boolean flag = false;
        layoutTable = new HashMap();
        for(int i1 = 0; i1 < l; i1++)
        {
            Component component = container.getComponent(i1);
            TableConstraints tableconstraints = getConstraints(component);
            if(tableconstraints.gridx != -1)
                j = tableconstraints.gridx;
            if(tableconstraints.gridy != -1)
                i = tableconstraints.gridy;
            if(nRows > 0 && i >= nRows)
                throw new ErrorException("TableLayout: Too many rows specified");
            do
            {
                if(ai == null || j >= ai.length || ai[j] <= 0)
                    break;
                j++;
                if(k > 0 && j >= k)
                {
                    for(int j1 = 0; j1 < ai.length; j1++)
                        if(ai[j1] > 0)
                            ai[j1]--;

                    i++;
                    j = getFirstAvailableColumn(ai);
                }
            } while(true);
            tableconstraints.gridx = j;
            tableconstraints.gridy = i;
            boolean flag1 = flag;
            setMaxProperty((new StringBuilder()).append("width").append(j).toString(), tableconstraints.width);
            setMaxProperty((new StringBuilder()).append("height").append(i).toString(), tableconstraints.height);
            setMaxProperty((new StringBuilder()).append("weightx").append(j).toString(), tableconstraints.weightx);
            setMaxProperty((new StringBuilder()).append("weighty").append(i).toString(), tableconstraints.weighty);
            int k1 = 1;
            switch(tableconstraints.gridwidth)
            {
            case 0: // '\0'
                flag1 = true;
                if(k > 0)
                    k1 = k - j;
                break;

            case -1: 
                if(k <= 0)
                    throw new ErrorException("TableLayout: Illegal to use gridwidth=RELATIVE in first row");
                k1 = k - j - 1;
                flag = true;
                break;

            default:
                k1 = tableconstraints.gridwidth;
                flag1 = nColumns > 0 && j + tableconstraints.gridwidth >= nColumns;
                break;
            }
            if(k1 > 1 && ai != null)
            {
                for(int l1 = j; l1 < Math.min(ai.length, j + k1); l1++)
                    if(ai[l1] != 0)
                        throw new ErrorException("TableLayout: Overlapping cells");

            }
            int i2 = 1;
            switch(tableconstraints.gridheight)
            {
            case 0: // '\0'
                i2 = 0x7fffffff;
                break;

            case -1: 
                throw new ErrorException("TableLayout: Illegal to use gridheight=RELATIVE");

            default:
                i2 = tableconstraints.gridheight;
                break;
            }
            if(i2 > 1)
            {
                if(ai == null)
                    ai = new int[j + k1];
                else
                if(ai.length < j + k1)
                {
                    int ai1[] = ai;
                    ai = new int[j + k1];
                    System.arraycopy(ai1, 0, ai, 0, ai1.length);
                }
                for(int j2 = j; j2 < j + k1; j2++)
                    ai[j2] = i2;

            }
            tableconstraints.gridwidth = k1;
            tableconstraints.gridheight = i2;
            layoutTable.put(component, tableconstraints);
            for(j += k1; flag1 || k > 0 && j >= k; flag1 = false)
            {
                if(k <= 0)
                    k = j;
                if(ai != null)
                {
                    for(int k2 = 0; k2 < ai.length; k2++)
                        if(ai[k2] > 0)
                            ai[k2]--;

                }
                i++;
                j = getFirstAvailableColumn(ai);
            }

        }

        if(k <= 0)
            k = j;
        setIntProperty("nColumns", k);
        setIntProperty("nRows", i + 1);
    }

    private Rectangle computeCellBounds(Rectangle rectangle, Dimension dimension, TableConstraints tableconstraints)
    {
        int i = rectangle.x;
        int j = rectangle.y;
        int k = rectangle.width;
        int l = rectangle.height;
        if(tableconstraints.fill == 0 || tableconstraints.fill == 3)
            k = dimension.width;
        if(tableconstraints.fill == 0 || tableconstraints.fill == 2)
            l = dimension.height;
        if(k != rectangle.width)
            switch(tableconstraints.anchor)
            {
            case 10: // '\n'
            case 11: // '\013'
            case 15: // '\017'
                i += (rectangle.width - k) / 2;
                break;

            case 12: // '\f'
            case 13: // '\r'
            case 14: // '\016'
                i += rectangle.width - k;
                break;
            }
        if(l != rectangle.height)
            switch(tableconstraints.anchor)
            {
            case 10: // '\n'
            case 13: // '\r'
            case 17: // '\021'
                j += (rectangle.height - l) / 2;
                break;

            case 14: // '\016'
            case 15: // '\017'
            case 16: // '\020'
                j += rectangle.height - l;
                break;
            }
        return new Rectangle(i, j, k, l);
    }

    private void setMaxProperty(String s, int i)
    {
        setIntProperty(s, Math.max(i, getIntProperty(s)));
    }

    private void setMaxProperty(String s, double d)
    {
        setDoubleProperty(s, Math.max(d, getDoubleProperty(s)));
    }

    private void setIntProperty(String s, int i)
    {
        propertyTable.put(s, new Integer(i));
    }

    private int getIntProperty(String s)
    {
        Object obj = propertyTable.get(s);
        if(obj == null)
            return 0;
        else
            return ((Integer)obj).intValue();
    }

    private void setDoubleProperty(String s, double d)
    {
        propertyTable.put(s, new Double(d));
    }

    private double getDoubleProperty(String s)
    {
        Object obj = propertyTable.get(s);
        if(obj == null)
            return 0.0D;
        else
            return ((Double)obj).doubleValue();
    }

    private int getFirstAvailableColumn(int ai[])
    {
        if(useStrictGridBagModel && ai != null)
        {
            for(int i = ai.length; i > 0; i--)
                if(ai[i - 1] > 0)
                    return i;

        }
        return 0;
    }

    public static final int NONE = 0;
    public static final int HORIZONTAL = 2;
    public static final int VERTICAL = 3;
    public static final int BOTH = 1;
    public static final int CENTER = 10;
    public static final int LEFT = 11;
    public static final int RIGHT = 12;
    public static final int TOP = 13;
    public static final int BOTTOM = 14;
    public static final int FILL = 1;
    private static final int LAYOUT_CONTAINER_MODE = 0;
    private static final int MINIMUM_LAYOUT_SIZE_MODE = 1;
    private static final int PREFERRED_LAYOUT_SIZE_MODE = 2;
    private static final int MINIMUM_SCROLLBAR_SIZE = 100;
    private transient Container targetContainer;
    private HashMap constraintTable;
    private HashMap layoutTable;
    private HashMap propertyTable;
    private boolean useStrictGridBagModel;
    private int nRows;
    private int nColumns;
    private int horizontalAlignment;
    private int verticalAlignment;
    private int defaultFill;
    private int hGap;
    private int vGap;
    static final long serialVersionUID = 1L;
}
