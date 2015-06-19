package stanford.cs106.util;

import java.util.*;

public class Stanford {
	private Stanford() {
		// empty
	}
	
	/**
	 * Returns a quarter string such as "15sp" for Spring 2015.
	 * @return a quarter string such as "15sp" for Spring 2015.
	 */
	public static String getCurrentQuarter() {
		Calendar today = GregorianCalendar.getInstance();
		int year = today.get(Calendar.YEAR) % 100;
		int month = today.get(Calendar.MONTH) + 1;   // horrible design
		// int day = today.get(Calendar.DAY_OF_MONTH);
		String quarter;
		if (month >= 1 && month <= 3) {
			quarter = "wi";
		} else if (month > 3 && month <= 6) {
			quarter = "sp";
		} else if (month > 6 && month <= 8) {
			quarter = "su";
		} else { // if (month > 8 && month <= 12)
			quarter = "au";
		}
		return year + quarter;
	}
}
