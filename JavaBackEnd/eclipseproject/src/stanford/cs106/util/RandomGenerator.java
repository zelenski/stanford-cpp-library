package stanford.cs106.util;

public class RandomGenerator extends acm.util.RandomGenerator {
	private static final long serialVersionUID = 0L;
	private static Long allSeed = null;
	private static Boolean rigBooleanValue;
	private static Double rigDoubleValue;
	private static Integer rigIntValue;
	
	public static void setAllSeed(Long seed) {
		allSeed = seed;
		if (seed != null) {
			acm.util.RandomGenerator.getInstance().setSeed(seed);
		}
	}
	
	private static final RandomGenerator instance = new RandomGenerator();
	
	public static RandomGenerator getInstance() {
		return instance;
	}
	
	public static void rigBoolean(Boolean value) {
		rigBooleanValue = value;
	}
	
	public static void rigDouble(Double value) {
		rigDoubleValue = value;
	}
	
	public static void rigInt(Integer value) {
		rigIntValue = value;
	}
	
	public RandomGenerator() {
		super();
		if (allSeed != null) {
			setSeed(allSeed);
		}
	}
	
	public RandomGenerator(long seed) {
		super();
		setSeed(seed);
	}
	
	@Override
	public boolean nextBoolean() {
		if (rigBooleanValue != null) {
			return rigBooleanValue;
		} else {
			return super.nextBoolean();
		}
	}
	
	@Override
	public boolean nextBoolean(double probability) {
		if (rigBooleanValue != null) {
			return rigBooleanValue;
		} else {
			return super.nextBoolean(probability);
		}
	}
	
	@Override
	public double nextDouble() {
		if (rigDoubleValue != null) {
			return rigDoubleValue;
		} else {
			return super.nextDouble();
		}
	}
	
	@Override
	public double nextDouble(double min, double max) {
		if (rigDoubleValue != null) {
			return rigDoubleValue;
		} else {
			return super.nextDouble(min, max);
		}
	}
	
	@Override
	public int nextInt() {
		if (rigIntValue != null) {
			return rigIntValue;
		} else {
			return super.nextInt();
		}
	}
	
	@Override
	public int nextInt(int max) {
		if (rigIntValue != null) {
			return rigIntValue;
		} else {
			return super.nextInt(max);
		}
	}
	
	@Override
	public int nextInt(int min, int max) {
		if (rigIntValue != null) {
			return rigIntValue;
		} else {
			return super.nextInt(min, max);
		}
	}
}
