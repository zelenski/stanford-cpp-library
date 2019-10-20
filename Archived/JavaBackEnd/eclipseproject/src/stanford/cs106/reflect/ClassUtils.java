package stanford.cs106.reflect;

// Author: Marty Stepp
// A bunch of methods used to dynamically examine and modify Java classes.

import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.security.*;
import java.util.*;

import javax.tools.JavaCompiler;
import javax.tools.ToolProvider;

import stanford.cs106.io.*;
import stanford.cs106.util.*;

public final class ClassUtils {
	// class constants
	public static final String CLASS_EXTENSION = ".class";
	public static final String JAVA_EXTENSION = ".java";
	private static final FileFilter CLASS_FILTER = new ExtensionFilter(CLASS_EXTENSION);

	public static boolean hasMain(Class<?> clazz) {
		try {
			Method main = clazz.getMethod("main", new String[0].getClass());
			int mod = main.getModifiers();
			return Modifier.isStatic(mod) && Modifier.isPublic(mod) && main.getReturnType() == Void.TYPE;
		} catch (NoSuchMethodException e) {
			return false;
		}
	}
	
	public static void runMain(Class<?> clazz) throws SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
		runMain(clazz, null);
	}
	
	public static void runMain(Class<?> clazz, String[] args) throws SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
		Method main = clazz.getMethod("main", new String[0].getClass());
		int mod = main.getModifiers();
		if (Modifier.isStatic(mod) && Modifier.isPublic(mod) && main.getReturnType() == Void.TYPE) {
			main.invoke(null, (Object) args);
		}
	}
	
	// Returns a list of all folders in the system Java class path.
	public static List<String> getClassPathFolders() {
		try {
			String classPath = System.getProperty("java.class.path").trim();
			if (classPath.length() == 0) {
				classPath = ".";
			}
			String[] tokens = classPath.split(File.pathSeparator);
			Set<String> absPaths = new HashSet<String>();
			List<String> pruned = new ArrayList<String>();
			for (String token : tokens) {
				File tokenFile = new File(token);
				if (tokenFile.isDirectory() && !absPaths.contains(tokenFile.getAbsolutePath())) {
					pruned.add(token);
					absPaths.add(tokenFile.getAbsolutePath());
				}
			}
			return pruned;
		} catch (AccessControlException e) {
			return Arrays.asList(".");
		}
	}
	
	// Returns all fields in the given class that have the given type.
	// Assumes the type is not generic such as List<String> or that generics should be ignored.
	public static Set<Field> getFields(Class<?> clazz, Class<?> type) {
		return getFields(clazz, type, null);
	}

	// Returns all fields in the given class that have the given type and parameterized type.
	// Works for checking generics such as List<String>.
	public static Set<Field> getFields(Class<?> clazz, Class<?> type, Class<?> parameterizedType) {
		return getFields(clazz, type, parameterizedType, false);
	}
	
	public static Set<Field> getFields(Class<?> clazz, Class<?> type, Class<?> parameterizedType, boolean allowSubtype) {
		return getFields(clazz, new LinkedHashSet<Class<?>>(CollectionUtils.asSet(type)), parameterizedType, allowSubtype);
	}
	
	public static Set<Field> getFields(Class<?> clazz, Set<Class<?>> types, Class<?> parameterizedType, boolean allowSubtype) {
		Set<Field> resultFields = new LinkedHashSet<Field>();
		List<String> fieldNames = new ArrayList<String>(ClassUtils.getFieldNames(clazz));
		List<Class<?>> fieldTypes = new ArrayList<Class<?>>(ClassUtils.getFieldTypes(clazz));

		for (int i = 0; i < fieldNames.size(); i++) {
			String fieldName = fieldNames.get(i);
			Class<?> fieldType = fieldTypes.get(i);

			for (Class<?> type : types) {
				if (fieldType == type || (allowSubtype && type.isAssignableFrom(fieldType))) {
					if (parameterizedType == null) {
						// not generic; standard type
						try {
							resultFields.add(clazz.getDeclaredField(fieldName));
						} catch (Exception e) {
							e.printStackTrace();
							continue;
						}
					} else {
						// generics; try to match
						try {
							Field field = clazz.getDeclaredField(fieldName);
							Type genericType = field.getGenericType();
							if (genericType != null && genericType instanceof ParameterizedType) {
								ParameterizedType paramType = (ParameterizedType) genericType;
								Type paramRealType = paramType.getActualTypeArguments()[0];
								if (paramRealType == parameterizedType) {
									resultFields.add(field);
								}
							}
						} catch (Exception e) {
							continue;
						}
					}
				}
			}
		}
		return resultFields;
	}
	
	/**
	 * Returns a set of names of all fields in the given class.
	 * If the class is not found, returns an empty set.
	 */
	public static Set<String> getFieldNames(Class<?> clazz) {
		Set<String> fields = new LinkedHashSet<String>();
		for (Field field : clazz.getDeclaredFields()) {
			// add all fields except static final constants
			int mod = field.getModifiers();
			if (!(Modifier.isStatic(mod) && Modifier.isFinal(mod))) {
				fields.add(field.getName());
			}
		}
		return fields;
	}
	
	/**
	 * Returns a set of types of all fields in the given class.
	 * If the class is not found, returns an empty set.
	 */
	public static List<Class<?>> getFieldTypes(Class<?> clazz) {
		List<Class<?>> fields = new ArrayList<Class<?>>();
		for (Field field : clazz.getDeclaredFields()) {
			// add all fields except static final constants
			int mod = field.getModifiers();
			if (!(Modifier.isStatic(mod) && Modifier.isFinal(mod))) {
				fields.add(field.getType());
			}
		}
		return fields;
	}

	// Returns a list of all folders in the system Java class path.
	public static String getFirstClassPathFolder() {
		List<String> folders = getClassPathFolders();
		if (folders.size() == 0) {
			return ".";
		}
		String folder = folders.get(0).trim();
		if (folder.length() == 0) {
			return ".";
		}
		return folder;
	}
	
	/**
	 * Returns a set of names of all fields in class with the given name that are not private.
	 * If the class name is not found, returns an empty set.
	 * If class name is null, throws a NullPointerException.
	 */
    public static Set<String> getNonPrivateFieldNames(String className) {
        return getNonPrivateFieldNames(className, false);
    }

	public static Set<String> getNonPrivateFieldNames(String className, boolean allowProtected) {
		try {
			return getNonPrivateFieldNames(Class.forName(className), allowProtected);
		} catch (ClassNotFoundException e) {
			return Collections.emptySet();
		}
	}
	
	/**
	 * Returns a set of names of all fields in the given class that are not private.
	 * If class is null, throws a NullPointerException.
	 */
    public static Set<String> getNonPrivateFieldNames(Class<?> clazz) {
        return getNonPrivateFieldNames(clazz, false);
    }

	public static Set<String> getNonPrivateFieldNames(Class<?> clazz, boolean allowProtected) {
		Set<String> fields = new LinkedHashSet<String>();
		for (Field field : clazz.getDeclaredFields()) {
			// the only allowed public fields are ones that are 'static final' constants
			int mod = field.getModifiers();
			if (!Modifier.isPrivate(mod) && (!allowProtected || !Modifier.isProtected(mod)) 
			        && !(Modifier.isStatic(mod) && Modifier.isFinal(mod))) {
				fields.add(field.getName());
			}
		}
		return fields;
	}

	public static Set<String> getNonPrivateMethodNames(Class<?> clazz) {
	    return getNonPrivateMethodNames(clazz, false);
	}

	/**
	 * Returns a set of names of all methods in the given class that are not private.
	 * If class is null, throws a NullPointerException.
	 */
	public static Set<String> getNonPrivateMethodNames(Class<?> clazz, boolean allowProtected) {
		Set<String> methods = new LinkedHashSet<String>();
		for (Method method : clazz.getDeclaredMethods()) {
			// the only allowed public fields are ones that are 'static final' constants
			int mod = method.getModifiers();
			if (!Modifier.isPrivate(mod) && (!allowProtected || !Modifier.isProtected(mod))) {
				methods.add(method.getName());
			}
		}
		return methods;
	}
	
	/**
	 * Returns a set of names of all methods in the given class that are not private.
	 * If class is null, throws a NullPointerException.
	 */
	public static Set<Method> getNonPrivateMethods(Class<?> clazz) {
		Set<Method> methods = new LinkedHashSet<Method>();
		for (Method method : clazz.getDeclaredMethods()) {
			// the only allowed public fields are ones that are 'static final' constants
			int mod = method.getModifiers();
			if (!Modifier.isPrivate(mod)) {
				methods.add(method);
			}
		}
		return methods;
	}
	
	/**
	 * Returns a set of names of all methods in the given class that are not private.
	 * If class is null, throws a NullPointerException.
	 */
	public static Set<Constructor<?>> getNonPrivateConstructors(Class<?> clazz) {
		Set<Constructor<?>> constructors = new LinkedHashSet<Constructor<?>>();
		for (Constructor<?> ctor : clazz.getConstructors()) {
			// the only allowed public fields are ones that are 'static final' constants
			int mod = ctor.getModifiers();
			if (!Modifier.isPrivate(mod)) {
				constructors.add(ctor);
			}
		}
		return constructors;
	}
	
	/**
	 * Returns a set of names of all methods in the given class.
	 * If class is null, throws a NullPointerException.
	 */
	public static Set<String> getMethodNames(Class<?> clazz) {
		Set<String> methods = new LinkedHashSet<String>();
		for (Method method : clazz.getDeclaredMethods()) {
			methods.add(method.getName());
		}
		return methods;
	}

	// Returns whether the given name represents an inner class (has a $ sign).
	public static boolean isInnerClass(Class<?> clazz) {
		return isInnerClass(clazz.getName());
	}

	// Returns whether the given name represents an inner class (has a $ sign).
	public static boolean isInnerClass(String className) {
		return className.indexOf('$') >= 0;
	}

	public static String writeAndCompile(String fileText, String className, boolean useTempFolder)
			throws IOException, ClassNotFoundException, NoSuchMethodException,
			IllegalAccessException, InvocationTargetException {
		// write the modified text to a new file (possibly in temp dir)
		String javaFileName = className + JAVA_EXTENSION;
		if (useTempFolder) {
			javaFileName = System.getProperty("java.io.tmpdir")
					+ File.separatorChar + javaFileName;
		}
		IOUtils.writeEntireFile(fileText, javaFileName);

		String classFileName = compile(javaFileName);
		new File(javaFileName).delete();

		return classFileName;
	}

	public static Class<?> writeAndLoadClass(String fileText, String className, boolean useTempFolder)
			throws IOException, ClassNotFoundException, IllegalAccessException,
			InvocationTargetException, NoSuchMethodException {
		// write the modified text to a new file (possibly in temp dir)
		String classFileName = writeAndCompile(fileText, className, useTempFolder);

		// move class to current directory
		new File(classFileName).renameTo(new File("." + File.separatorChar
				+ className + CLASS_EXTENSION));
		return loadClass(classFileName);
	}

	/**
	 * Compiles the .java source file with the given file name,
	 * and returns the file name of the newly compiled .class file.
	 * Throws a CompilerErrorException if the compilation fails.
	 */
	public static String compile(String fileName)
			throws ClassNotFoundException, NoSuchMethodException,
			IllegalAccessException, InvocationTargetException {
		
		String folderName = IOUtils.getFolder(fileName);
		String classPath = System.getProperty("java.class.path")
				+ File.pathSeparator + "." + File.pathSeparator + folderName;
		String[] args = { "-classpath", classPath, fileName };
		
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
		int result = -1;
		if (compiler == null) {
			// fall back to JDK <= 1.6 com.sun.tools.javac.Main method
			try {
				Class<?> compilerClass = Class.forName("com.sun.tools.javac.Main");
				Method compileMethod = compilerClass.getMethod("compile", new String[0]
						.getClass());
				result = (Integer) compileMethod.invoke(null, (Object) args);
			} catch (ClassNotFoundException cnfe) {
				throw new CompilerErrorException(
						"ERROR: Cannot dynamically compile code on this version of JDK. \n"
						+ "This feature requires JDK 1.7 or later, \n"
						+ "and the latest version of your editing environment. \n"
						+ "The editor must also be set up to properly use your JDK. \n"
						+ "Please update your Java and/or Eclipse installation or settings.");
			}
		} else {
			// use good JDK 1.7+ JavaCompiler system
			result = compiler.run(System.in, out, out, args);
		}
		
		if (result != 0) {
			throw new CompilerErrorException("Compilation failed with error code "
					+ result + ":\n" + out.toString());
		}

		return IOUtils.removeExtension(fileName) + CLASS_EXTENSION;
	}

	// Loads all classes that extend the given class from the given folder.
	@SuppressWarnings("unchecked")
	public static <T> List<Class<? extends T>> getClasses(Class<T> superClass, String folderName) {
	    try {
			List<Class<? extends T>> list = new ArrayList<Class<? extends T>>();
			java.io.File folder = new java.io.File(folderName);
			if (!folder.exists() || !folder.canRead()) {
				return list;
			}

			for (java.io.File file : folder.listFiles(CLASS_FILTER)) {
				String fileName = file.getName();
				if (file.canRead() && !file.isDirectory() && fileName.endsWith(CLASS_EXTENSION)) {
					try {
						Class<?> existingClass = Class.forName(IOUtils.removeExtension(fileName));
						if (existingClass != null && !existingClass.isInterface()
								&& !Modifier.isAbstract(existingClass.getModifiers())
								&& superClass.isAssignableFrom(existingClass)) {
							// then this is a concrete class that implements the interface
							list.add((Class<? extends T>) existingClass);
						}
					} catch (IncompatibleClassChangeError icce) {
						icce.printStackTrace();
					} catch (Throwable t) {
						t.printStackTrace();
					}
				}
			}
			
			Collections.sort(list, new ClassComparator());
			return list;
		} catch (SecurityException e) {
			// probably running as an applet
            return Collections.emptyList();
		}
	}
	
	// Returns whether the given class implements the given interface.
	public static boolean classImplements(Class<?> clazz, Class<?> interfaceType) {
		for (Class<?> c : clazz.getInterfaces()) {
			if (c == interfaceType) {
				return true;
			}
		}
		return false;
	}

	// Dynamically loads the compiled .class file with the given file name
	// into our JVM and returns its Class object.
	// Throws various reflectiony exceptions if the file is bad.
	public static Class<?> loadClass(String fileName)
			throws ClassNotFoundException {
		String folderName = IOUtils.getFolder(fileName);
		File folder = new File(folderName);
		ClassLoader loader = ClassLoader.getSystemClassLoader();

		ClassLoader urlLoader = loader;
		try {
			URL fileUrl = new URL("file:" + System.getProperty("user.dir")
					+ File.separator + fileName);

			File currentDir = new File(System.getProperty("user.dir"));
			urlLoader = URLClassLoader.newInstance(
					new URL[] { folder.toURI().toURL(),
							currentDir.toURI().toURL(), fileUrl }, loader);
		} catch (MalformedURLException mfurle) {
			mfurle.printStackTrace(); // this will never happen
		}

		String className = IOUtils.removeExtension(IOUtils.removeFolder(fileName));
		try {
			Class<?> clazz = urlLoader.loadClass(className);
			return clazz;
		} catch (IncompatibleClassChangeError icce) {
			throw new RuntimeException("Unable to load the class: " + icce);
		}
	}

	public static String readAndRename(String oldClassName, String newClassName)
			throws IOException {
		String fileName = oldClassName + JAVA_EXTENSION;
		return readAndRename(fileName, oldClassName, newClassName);
	}

	public static String readAndRename(String fileName, String oldClassName, String newClassName)
			throws IOException {
		String fileText = IOUtils.readEntireFile(fileName);

		// replace the class name in the code
		fileText = fileText.replaceAll(oldClassName, newClassName);
		return fileText;
	}

	// Treats fileText as the text of a Java source file, and
	// replaces occurrences of its class name with the given new class name,
	// then writes it to a new .java file with that name.
	// Returns the new file name.
	public static String renameAndWriteJavaFile(String fileText,
			String oldClassName, String newClassName, boolean useTempFolder) {
		// replace the class name in the code
		fileText = fileText.replaceAll(oldClassName, newClassName);

		// write the modified text to a new file
		String newFileName = newClassName + JAVA_EXTENSION;
		if (useTempFolder) {
			newFileName = System.getProperty("java.io.tmpdir") + newFileName;
		}
		IOUtils.writeEntireFile(fileText, newFileName);
		return newFileName;
	}

	// Removes any characters from given text that wouldn't be acceptable
	// in a Java class name.
	// Not perfect (e.g., doesn't prevent names that start with a number).
	public static String sanitizeClassName(String text) {
		text = text.replaceAll("[^A-Za-z0-9_$]+", "_");
		return text;
	}

    // Removes any characters from given text that wouldn't be acceptable
    // in a Java class name.
    // Not perfect (e.g., doesn't prevent names that start with a number).
    public static String stripPackages(Class<?> clazz) {
        return stripPackages(clazz.getName());
    }

    // Removes any characters from given text that wouldn't be acceptable
    // in a Java class name.
    // Not perfect (e.g., doesn't prevent names that start with a number).
    public static String stripPackages(String className) {
        return className.replaceAll(".*\\.", "");
    }

	public static boolean reflectionEquals(Object o1, Object o2) {
		if (o1 == null) {
			return o2 == null;
		}
		if (o2 == null) {
			return o1 == null;
		}
		if (o1 == o2) {
			return true;
		}
		
		Class<?> clazz1 = o1.getClass();
		Class<?> clazz2 = o2.getClass();
		if (clazz1 != clazz2) {
			return false;
		}
		
		for (Field field : clazz1.getDeclaredFields()) {
			try {
				field.setAccessible(true);
				Object f1 = field.get(o1);
				Object f2 = field.get(o2);
				if ((f1 == null && f2 != null) || (f2 == null && f1 != null)) {
					return false;
				}
				if (f1 != null && f2 != null) {
					Class<?> fieldType = field.getType();
					if (fieldType.isArray()) {
//						if (!Arrays.deepEquals((Object[]) f1, (Object[]) f2)) {
//							return false;
//						}
						throw new UnsupportedOperationException("array fields not supported right now");
					} else {
						if (!f1.equals(f2)) {
							return false;
						}
					}
				}
			} catch (IllegalAccessException iae) {
				throw new RuntimeException(iae);
			}
		}
		
		return true;
	}

	// For sorting class reflection objects by name.
	public static class ClassComparator implements Comparator<Class<?>> {
		public int compare(Class<?> c1, Class<?> c2) {
			return c1.getName().compareTo(c2.getName());
		}
	}

	// inner class to filter files by extension
	public static class ExtensionFilter implements FileFilter {
		private String extension;

		public ExtensionFilter(String extension) {
			this.extension = extension;
		}

		public boolean accept(java.io.File f) {
			return f != null && f.exists() && f.canRead()
					&& f.getName().endsWith(extension);
		}
	}

	/**
	 * Loads classes from files on the server's hard disk.
	 * I shouldn't need to write this class, but Java's built-in class loaders
	 * (particularly URLClassLoader) are actually quite shitty.  You have to pass
	 * them arrays of java.net.URL objects, and they throw a million exceptions, and
	 * they don't really work very well.  So I have to make my own wrapper.  Sigh.
	 * 
	 * @author Marty Stepp
	 *
	 */
	public static class BetterClassLoader extends URLClassLoader {
		/**
		 * Private internal constructor; clients should use newInstance instead.
		 */
		private BetterClassLoader(URL[] urls, ClassLoader parent) {
			super(urls, parent);
		}
		
		/**
		 * Creates a new loader that loads classes from the given array of file names.
		 * If a class can't be found in any of those files, falls back to the system
		 * class loader.
		 */
		public static ClassLoader newInstance(String... filenames) {
			return newInstance(ClassLoader.getSystemClassLoader(), filenames);
		}
		
		/**
		 * Creates a new loader that loads classes from the given array of file names.
		 * If a class can't be found in any of those files, falls back to the given
		 * "parent" class loader.
		 */
		@SuppressWarnings("deprecation")
		public static ClassLoader newInstance(ClassLoader parent, String... filenames) {
			try {
				Set<URL> urls = new LinkedHashSet<URL>();
				for (String filename : filenames) {
					urls.add(new File(filename).toURL());
					urls.add(new File(new File(filename).getParent()).toURL());
				}
				return new BetterClassLoader(urls.toArray(new URL[0]), parent);
			} catch (MalformedURLException mfurle) {
				mfurle.printStackTrace();   // this will probably never happen
				throw new RuntimeException(mfurle);
			}
		}
		
		/**
		 * I don't think this method is needed any more; used to help load classes.
		 */
		/*
		protected Class<?> findClass(String name) throws ClassNotFoundException {
			/ *
			if (name.contains("sandbox")) {
				String s = ":-(";
				try {
					s = "" + super.findClass(name);
				} catch (Throwable t) {}
				if (s.equals(":-(")) {
					s = ":'( :'(";
					try {
						s += "" + sandbox.Scanner.class;
					} catch (Throwable t) {
						s += "(couldn't load scanner)";
					}
					try {
						s = "(load) " + ClassLoader.getSystemClassLoader().loadClass(name);
					} catch (Throwable t) {}
				}
				throw new RuntimeException("findClass: " + name + ", " + s);
			}
			* /
			return super.findClass(name);
		}
		*/

		/**
		 * Tries to load the class from the default system class loader, but if
		 * it isn't found, loads from the internal list of URLs.
		 * 
		 * The main exception is that classes from the 'Sandbox' package are
		 * forcibly loaded from Practice-It's own notions of the sandbox.* classes.
		 * JUnit classes are also treated as a special case.
		 * 
		 * This is to make it so that dynamically loaded DumpingGround classes
		 * can see sandbox.* classes that they interact with.
		 */
		public Class<?> loadClass(String name) throws ClassNotFoundException {
			try {
				Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass(name);
				if (clazz != null) {
					return clazz;
				}
			} catch (ClassNotFoundException e) {
				// empty
			}
			return super.loadClass(name);
		}
	}
}
