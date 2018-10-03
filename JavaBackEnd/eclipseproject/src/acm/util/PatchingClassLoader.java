package acm.util;

import java.io.*;
import java.lang.reflect.*;
import java.util.*;

/**
 * A ClassLoader that allows you to patch what the superclass is for an existing class
 * as it is loaded.  This is primarily used to hot-patch ConsolePrograms to be CommandLinePrograms
 * when run in a non-graphical setting.
 * @author Marty Stepp, based on Eric Roberts code
 *
 */
public class PatchingClassLoader extends ClassLoader {
	private static final int CONSTANT_Utf8 =                1;
	private static final int CONSTANT_Integer =             3;
	private static final int CONSTANT_Float =               4;
	private static final int CONSTANT_Long =                5;
	private static final int CONSTANT_Double =              6;
	private static final int CONSTANT_Class =               7;
	private static final int CONSTANT_String =              8;
	private static final int CONSTANT_Fieldref =            9;
	private static final int CONSTANT_Methodref =          10;
	private static final int CONSTANT_InterfaceMethodref = 11;
	private static final int CONSTANT_NameAndType =        12;

	private HashMap<Integer,Integer> classTable;
	private ClassLoader realLoader;
	private String targetName;
	private String newSuperClassName;
	private int superclassOffset;

	public PatchingClassLoader(String targetClassName, Class<?> newSuperClass) {
		this(targetClassName, newSuperClass.getName());
	}
	
	public PatchingClassLoader(String targetClassName, String newSuperClassName) {
		this.targetName = targetClassName;
		this.newSuperClassName = newSuperClassName.replace(".", "/");   // "acm/program/CommandLineProgram"
		try {
			Class<?> classLoader = java.lang.ClassLoader.class;
			Method getSystemClassLoader = classLoader.getMethod("getSystemClassLoader", new Class[0]);
			realLoader = (ClassLoader) getSystemClassLoader.invoke(null, new Object[0]);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

	public Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
		if (name.equals(targetName)) {
			InputStream in = getResourceAsStream(name + ".class");
			superclassOffset = findSuperclassOffset(in);
			in = getResourceAsStream(name + ".class");
			byte[] code = patchClassData(in);
			return defineClass(name, code, 0, code.length);
		} else {
			return realLoader.loadClass(name);
		}
	}

	public InputStream getResourceAsStream(String name) {
		return realLoader.getResourceAsStream(name);
	}

	public java.net.URL getResource(String name) {
		return realLoader.getResource(name);
	}

	private byte[] patchClassData(InputStream in) {
		try {
			ByteArrayOutputStream out = new ByteArrayOutputStream();
			JTFTools.copyBytes(in, out, 8);
			int nConstants = in.read() << 8 | in.read();
			out.write(nConstants >> 8);
			out.write(nConstants & 0xFF);
			for (int index = 1; index < nConstants; index++) {
				int type = in.read();
				out.write(type);
				if (JTFTools.testDebugOption("constants")) {
					System.out.println(index + ": " + getConstantTypeName(type));
				}
				switch (type) {
				  case CONSTANT_Integer: JTFTools.copyBytes(in, out, 4); break;
				  case CONSTANT_Float: JTFTools.copyBytes(in, out, 4); break;
				  case CONSTANT_Long: JTFTools.copyBytes(in, out, 8); index++; break;
				  case CONSTANT_Double: JTFTools.copyBytes(in, out, 8); index++; break;
				  case CONSTANT_Class: JTFTools.copyBytes(in, out, 2); break;
				  case CONSTANT_String: JTFTools.copyBytes(in, out, 2); break;
				  case CONSTANT_Fieldref: JTFTools.copyBytes(in, out, 4); break;
				  case CONSTANT_Methodref: JTFTools.copyBytes(in, out, 4); break;
				  case CONSTANT_InterfaceMethodref: JTFTools.copyBytes(in, out, 4); break;
				  case CONSTANT_NameAndType: JTFTools.copyBytes(in, out, 4); break;
				  case CONSTANT_Utf8:
					if (index == superclassOffset) {
						int nChars = in.read() << 8 | in.read();
						in.skip(nChars);
						nChars = newSuperClassName.length();
						out.write(nChars >> 8);
						out.write(nChars & 0xFF);
						for (int j = 0; j < nChars; j++) {
							out.write((byte) newSuperClassName.charAt(j));
						}
					} else {
						int nChars = in.read() << 8 | in.read();
						out.write(nChars >> 8);
						out.write(nChars & 0xFF);
						JTFTools.copyBytes(in, out, nChars);
					}
					break;
				}
			}
			while (true) {
				int ch = in.read();
				if (ch == -1) break;
				out.write(ch);
			}
			return out.toByteArray();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

	private int findSuperclassOffset(InputStream in) {
		classTable = new HashMap<Integer,Integer>();
		try {
			in.skip(8);
			int nConstants = in.read() << 8 | in.read();
			nConstants += 2;
			for (int i = 1; i < nConstants - 2; i++) {
				int type = in.read();
				switch (type) {
				  case CONSTANT_Integer: in.skip(4); break;
				  case CONSTANT_Float: in.skip(4); break;
				  case CONSTANT_Long: in.skip(8); i++; break;
				  case CONSTANT_Double: in.skip(8); i++; break;
				  case CONSTANT_String: in.skip(2); break;
				  case CONSTANT_Fieldref: in.skip(4); break;
				  case CONSTANT_Methodref: in.skip(4); break;
				  case CONSTANT_InterfaceMethodref: in.skip(4); break;
				  case CONSTANT_NameAndType: in.skip(4); break;
				  case CONSTANT_Class:
					int offset = in.read() << 8 | in.read();
					classTable.put(Integer.valueOf(i), Integer.valueOf(offset));
					break;
				  case CONSTANT_Utf8:
					int nChars = in.read() << 8 | in.read();
					in.skip(nChars);
					break;
				}
			}
			in.skip(4);
			return classTable.get(Integer.valueOf(in.read() << 8 | in.read())).intValue();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

	private static String getConstantTypeName(int id) {
		switch (id) {
		  case CONSTANT_Utf8: return "Utf8";
		  case CONSTANT_Integer: return "Integer";
		  case CONSTANT_Float: return "Float";
		  case CONSTANT_Long: return "Long";
		  case CONSTANT_Double: return "Double";
		  case CONSTANT_Class: return "Class";
		  case CONSTANT_String: return "String";
		  case CONSTANT_Fieldref: return "Fieldref";
		  case CONSTANT_Methodref: return "Methodref";
		  case CONSTANT_InterfaceMethodref: return "InterfaceMethodref";
		  case CONSTANT_NameAndType: return "NameAndType";
		  default: return "Type[" + id + "]";
		}
	}
}
