package stanford.spl;

public interface GTableListener {
	public void tableCopy(GTableEvent event);
	public void tableCut(GTableEvent event);
	public void tableEditBegin(GTableEvent event);
	public void tablePaste(GTableEvent event);
	public void tableReplaceBegin(GTableEvent event);
	public void tableSelected(GTableEvent event);
	public void tableUpdated(GTableEvent event);
}
