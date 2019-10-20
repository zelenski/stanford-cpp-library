/*
 * Code related to interacting with (getting and setting, invoking, etc.)
 * fields, constants, methods, and classes using reflection.
 * 
 * author: Marty Stepp
 * version: 2014/05/05
 */

package stanford.cs106.reflect;

import java.lang.reflect.*;
import java.util.*;

public class ReflectionUtils {
	public static class FieldNameComparator implements Comparator<Field> {
		public int compare(Field arg0, Field arg1) {
			return arg0.getName().compareTo(arg1.getName());
		}
	}

	public static class MethodNameComparator implements Comparator<Method> {
		public int compare(Method arg0, Method arg1) {
			return arg0.getName().compareTo(arg1.getName());
		}
	}
	
	public static String getClassNameWithoutPackage(String className) {
		int space = className.lastIndexOf('.');
		if (space >= 0) {
			className = className.substring(space + 1);
		}
		return className;
	}
	
	public static String getClassNameWithoutPackage(Class<?> clazz) {
		return getClassNameWithoutPackage(clazz, null);
	}
	
	public static String getClassNameWithoutPackage(Class<?> clazz, Type genericType) {
		String className = clazz.getName();
		if (clazz.isArray()) {
			className = clazz.getComponentType().getName() + "[]";
		} else if (genericType instanceof ParameterizedType) {
			ParameterizedType paramGenericType = (ParameterizedType) genericType;
			Type[] paramArgs = paramGenericType.getActualTypeArguments();
			if (paramArgs.length > 0) {
				className += "<";
				for (int i = 0; i < paramArgs.length; i++) {
					Class<?> paramType = (Class<?>) paramArgs[i];
					if (i > 0) {
						className += ", ";
					}
					className += getClassNameWithoutPackage(paramType);
				}
				className += ">";
			}
		}
		int space = className.lastIndexOf('.');
		if (space >= 0) {
			className = className.substring(space + 1);
		}
		return className;
	}
	
	// Returns true if the given constant exists
	public static boolean constantExists(Class<?> clazz, String constantNameRegex) {
		Field constantField = getFieldToMatchRegex(clazz, constantNameRegex);
		return constantField != null;
	}

	// Returns true if the given constant exists
	public static boolean constantExists(String className, String constantNameRegex) {
		Class<?> clazz = classForName(className);
		return constantExists(clazz, constantNameRegex);
	}

	// Returns the value of a private static final constant from a given class.
	public static Object getConstantValue(Class<?> clazz, String constantNameRegex) {
		return getConstantValue(clazz, null, constantNameRegex);
	}
	
	public static Object getConstantValue(Class<?> clazz, Object obj, String constantNameRegex) {
		try {
			Field constantField = getFieldToMatchRegex(clazz, constantNameRegex);
			if (constantField != null) {
				if (Modifier.isStatic(constantField.getModifiers())) {
					return constantField.get(null);
				} else {
					return constantField.get(obj);
				}
			}
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}

		throw new ReflectionRuntimeException("Unable to get constant in class " + clazz.getName()
				+ " with approximate name \"" + constantNameRegex
				+ "\"; no matching constant field found");
	}

	// Returns the value of a private static final constant from a given class.
	// Take that, private access!
	public static Object getConstantValue(String className, String constantNameRegex) {
		Class<?> clazz = classForName(className);
		return getConstantValue(clazz, constantNameRegex);
	}

	public static double getConstantValueDouble(Class<?> clazz, Object obj, String constantNameRegex) {
		Object value = getConstantValue(clazz, obj, constantNameRegex);
		return Double.parseDouble(String.valueOf(value));
	}
	
	public static double getConstantValueDouble(Class<?> clazz, String constantNameRegex) {
		Object value = getConstantValue(clazz, constantNameRegex);
		return Double.parseDouble(String.valueOf(value));
	}

	public static double getConstantValueDouble(String className, String constantNameRegex) {
		Object value = getConstantValue(className, constantNameRegex);
		return Double.parseDouble(String.valueOf(value));
	}

	public static int getConstantValueInt(Class<?> clazz, Object obj, String constantNameRegex) {
		return (int) getConstantValueDouble(clazz, obj, constantNameRegex);
	}

	public static int getConstantValueInt(Class<?> clazz, String constantNameRegex) {
		return (int) getConstantValueDouble(clazz, constantNameRegex);
	}

	public static int getConstantValueInt(String className, String constantNameRegex) {
		return (int) getConstantValueDouble(className, constantNameRegex);
	}

	public static boolean getConstantValueBoolean(Class<?> clazz, Object obj, String constantNameRegex) {
		Object value = getConstantValue(clazz, obj, constantNameRegex);
		return Boolean.parseBoolean(String.valueOf(value));
	}

	public static boolean getConstantValueBoolean(Class<?> clazz, String constantNameRegex) {
		Object value = getConstantValue(clazz, constantNameRegex);
		return Boolean.parseBoolean(String.valueOf(value));
	}

	public static boolean getConstantValueBoolean(String className, String constantNameRegex) {
		Object value = getConstantValue(className, constantNameRegex);
		return Boolean.parseBoolean(String.valueOf(value));
	}
	
	public static Field getFieldToMatchRegex(Class<?> clazz, String constantNameRegex) {
		return getFieldToMatchRegex(clazz, constantNameRegex, true);
	}
	
	public static Field getFieldToMatchRegex(Class<?> clazz, String constantNameRegex, boolean includeSuperclasses) {
		try {
			while (clazz != null) {
				for (Field field : clazz.getDeclaredFields()) {
					String constantName = field.getName();
					if (constantNameRegex.isEmpty()
							|| constantName.equalsIgnoreCase(constantNameRegex)
							|| constantName.toUpperCase().matches(
									".*" + constantNameRegex.toUpperCase() + ".*")) {
						field.setAccessible(true); // lolol not private any
															// more
						Field modifiersField = getField(Field.class, "modifiers");
						modifiersField.setInt(field, field.getModifiers() & ~Modifier.FINAL); // lolol not final any more
						return field;
					}
				}
				
				if (includeSuperclasses) {
					clazz = clazz.getSuperclass();
				} else {
					clazz = null;   // will end loop
				}
			}
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}
		
		throw new IllegalArgumentException("No field with approximate name \"" + constantNameRegex + "\"");
	}
	
	// Sets the value of a private static constant from a given class.
	public static void setConstantValue(Class<?> clazz, String constantNameRegex, Object value) {
		setConstantValue(clazz, null, constantNameRegex, value);
	}
	
	// Sets the value of a private static constant from a given class.
	public static void setConstantValue(Class<?> clazz, Object obj, String constantNameRegex, Object value) {
		boolean set = false;
		try {
			Field constantField = getFieldToMatchRegex(clazz, constantNameRegex);
			if (Modifier.isStatic(constantField.getModifiers())) {
				constantField.set(null, value);
			} else {
				constantField.set(obj, value);
			}

			// check to make sure the field was set
			Object newValue = getConstantValue(clazz, obj, constantField.getName());
			if (!newValue.equals(value)) {
				throw new ReflectionRuntimeException("constant \"" + constantField.getName() + "\" is "
						+ newValue + ", not properly set to " + value);
			}

			set = true;
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}

		if (!set) {
			throw new ReflectionRuntimeException("Unable to set constant in class " + clazz.getName()
					+ " with approximate name \"" + constantNameRegex
					+ "\"; no matching constant field found");
		}
	}
	
	public static void setConstantValue(String className, String constantNameRegex, Object value) {
		Class<?> clazz = classForName(className);
		setConstantValue(clazz, null, constantNameRegex, value);
	}
	
	public static Object[] getDefaultArgs(Method method) {
		Class<?>[] paramTypes = method.getParameterTypes();
		Object[] paramValues = new Object[paramTypes.length];
		for (int i = 0; i < paramTypes.length; i++) {
			paramValues[i] = getDefaultValue(paramTypes[i]);
		}
		return paramValues;
	}
	
	public static Object getDefaultValue(Class<?> type) {
		if (type == Integer.TYPE || type == Integer.class) {
			return 0;
		} else if (type == Long.TYPE || type == Long.class) {
			return 0L;
		} else if (type == Short.TYPE || type == Short.class) {
			return 0L;
		} else if (type == Byte.TYPE || type == Byte.class) {
			return (byte) 0;
		} else if (type == Double.TYPE || type == Double.class) {
			return 0.0;
		} else if (type == Float.TYPE || type == Float.class) {
			return 0.0f;
		} else if (type == Boolean.TYPE || type == Boolean.class) {
			return false;
		} else if (type == Character.TYPE || type == Character.class) {
			return '\0';
		} else if (type == String.class) {
			return "";
		} else {
			return null;
		}
	}
	
	public static Object getFirstFieldValueOfType(Class<?> clazz, Object obj, Class<?> fieldType) {
		for (Field field : clazz.getDeclaredFields()) {
			if (fieldType.isAssignableFrom(field.getType())) {
				return getFieldValue(obj, field);
			}
		}
		return null;
	}
	
	// Sets the value of a private field from a given class.
	public static Object getFieldValue(Object obj, Field field) {
		try {
			Field modifiersField = getField(Field.class, "modifiers");
			field.setAccessible(true); // lolol not private
			modifiersField.setInt(field, field.getModifiers()
					& ~Modifier.FINAL); // lolol not final any more
			return field.get(obj);
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}
	}

	// Sets the value of a private field from a given class.
	public static Object getFieldValue(Object obj, String name) {
		Class<?> clazz = obj.getClass();
		try {
			Field modifiersField = getField(Field.class, "modifiers");
			modifiersField.setAccessible(true);
			Field field = getField(clazz, name);
			String fieldName = field.getName();
			if (fieldName.equals(name)) {
				field.setAccessible(true); // lolol not private
				modifiersField.setInt(field, field.getModifiers()
						& ~Modifier.FINAL); // lolol not final any more
				return field.get(obj);
			}
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}
		throw new IllegalArgumentException("Fields.get: unable to read field \"" + name + "\" from class " + clazz.getName());
	}

	public static Object parseValue(Class<?> type, String value) {
		return parseValue(type, null, value);
	}
	
	public static Object parseValue(Class<?> type, Type genericType, String value) {
		if (type == Integer.TYPE || type == Integer.class) {
			return Integer.parseInt(value);
		} else if (type == Long.TYPE || type == Long.class) {
			return Long.parseLong(value);
		} else if (type == Short.TYPE || type == Short.class) {
			return Short.parseShort(value);
		} else if (type == Byte.TYPE || type == Byte.class) {
			return Byte.parseByte(value);
		} else if (type == Float.TYPE || type == Float.class) {
			return Float.parseFloat(value);
		} else if (type == Double.TYPE || type == Double.class) {
			return Double.parseDouble(value);
		} else if (type == Boolean.TYPE || type == Boolean.class) {
			return Boolean.parseBoolean(value);
		} else if (type == Character.TYPE || type == Character.class) {
			if (value.length() > 2 && value.charAt(0) == '\'' && value.charAt(value.length() - 1) == '\'') {
				value = value.substring(1, value.length() - 1);
			}
			return value.charAt(0);
		} else if (type == String.class) {
			if (value.length() > 2 && value.charAt(0) == '"' && value.charAt(value.length() - 1) == '"') {
				value = value.substring(1, value.length() - 1);
			}
			return value;
		} else if (type == List.class || type == ArrayList.class || type.isArray()) {
			value = value.trim();
			if (value.startsWith("[") || value.startsWith("{") || value.startsWith("(")) {
				value = value.substring(1);
			}
			if (value.endsWith("]") || value.endsWith("}") || value.endsWith(")")) {
				value = value.substring(0, value.length() - 1);
			}
			String[] elements = value.split("[ \t]*,[ \t]*");
			ArrayList<Object> list = new ArrayList<Object>();
			Class<?> paramType = Object.class;
			ParameterizedType paramGenericType = (ParameterizedType) genericType;
			if (paramGenericType.getActualTypeArguments().length > 0) {
				paramType = (Class<?>) paramGenericType.getActualTypeArguments()[0];
			}
			for (int i = 0; i < elements.length; i++) {
				if (paramType == null || paramType == Object.class || paramType == String.class) {
					list.add(elements[i]);
				} else {
					list.add(parseValue(paramType, elements[i]));
				}
			}
			
			if (type.isArray()) {
				// convert ArrayList into array
				Object[] array = (Object[]) Array.newInstance(type.getComponentType(), 0);
				return list.toArray(array);
			} else {
				return list;
			}
		} else if (type == Map.class || type == HashMap.class) {
			value = value.trim();
			if (value.startsWith("[") || value.startsWith("{") || value.startsWith("(")) {
				value = value.substring(1);
			}
			if (value.endsWith("]") || value.endsWith("}") || value.endsWith(")")) {
				value = value.substring(0, value.length() - 1);
			}
			String[] elements = value.split("[ \t]*,[ \t]*");
			Map<Object, Object> map = new HashMap<Object, Object>();
			Class<?> keyType = Object.class;
			Class<?> valueType = Object.class;
			ParameterizedType paramGenericType = (ParameterizedType) genericType;
			if (paramGenericType.getActualTypeArguments().length >= 2) {
				keyType = (Class<?>) paramGenericType.getActualTypeArguments()[0];
				valueType = (Class<?>) paramGenericType.getActualTypeArguments()[1];
			}
			for (int i = 0; i < elements.length; i++) {
				String[] keyValue = elements[i].split("[ \t]*=[ \t]*");
				if (keyValue.length < 2) {
					continue;
				}
				Object kv_key = keyValue[0];
				Object kv_value = keyValue[1];
						
				if (keyType != null && keyType != Object.class && keyType != String.class) {
					kv_key = parseValue(keyType, keyValue[0]);
				}
				if (valueType != null && valueType != Object.class && valueType != String.class) {
					kv_value = parseValue(valueType, keyValue[1]);
				}
				map.put(kv_key, kv_value);
			}
			
			return map;
		} else {
			throw new IllegalArgumentException("Fields.parseValue: unable to parse value \"" + value + "\" of type " + type.getName());
		}
	}

	// Sets the value of a private field from a given class.
	public static void setFieldValue(Class<?> clazz, String name, Object value) {
		try {
			Field modifiersField = getField(Field.class, "modifiers");
			Field field = getField(clazz, name);
			String fieldName = field.getName();
			if (fieldName.equals(name)) {
				modifiersField.setInt(field, field.getModifiers()
						& ~Modifier.FINAL); // lolol not final any more
				field.set(null, value);
			}
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}
	}

	// Sets the value of a private field from a given class.
	public static void setFieldValue(Object obj, Field field, Object value) {
		try {
			Field modifiersField = getField(Field.class, "modifiers");
			field.setAccessible(true); // lolol not private
			modifiersField.setInt(field, field.getModifiers()
					& ~Modifier.FINAL); // lolol not final any more
			field.set(obj, value);
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		}
	}
	
	public static Class<?> classForName(String className) {
		try {
			return Class.forName(className);
		} catch (ClassNotFoundException cnfe) {
			throw new ReflectionRuntimeException(cnfe);
		}
	}
	
	public static Object staticMethodInvoke(String className, String methodName, Object... params) {
		return staticMethodInvoke(classForName(className), methodName, params);
	}
	
	public static Object staticMethodInvoke(Class<?> clazz, String methodName, Object... params) {
		return methodInvoke(clazz, null, methodName, params);
	}
	
	public static Object methodInvoke(String className, Object obj, String methodName, Object... params) {
		return methodInvoke(classForName(className), obj, methodName, params);
	}
	
	public static Object methodInvoke(Class<?> clazz, Object obj, String methodName, Object... params) {
		try {
			Method method = getMethod(clazz, methodName);
			if (method != null) {
				if (!Modifier.isPublic(method.getModifiers())) {
					method.setAccessible(true);
				}
				return method.invoke(obj, params);
			} else {
				return null;
			}
		} catch (IllegalAccessException ite) {
			throw new ReflectionRuntimeException(ite);
		} catch (InvocationTargetException ite) {
			throw new ReflectionRuntimeException(ite);
		}
	}
	
	public static Method getMethod(Class<?> clazz, String methodName) {
		for (Method method : clazz.getDeclaredMethods()) {
			if (method.getName().equals(methodName)) {
				return method;
			}
		}
		for (Method method : clazz.getMethods()) {
			if (method.getName().equals(methodName)) {
				return method;
			}
		}
		return null;
	}
	
	public static Field getField(Class<?> clazz, String fieldName) {
		return getField(clazz, fieldName, true);
	}

	public static Field getField(Class<?> clazz, String fieldName, boolean checkSuperclasses) {
		Class<?> currentClazz = clazz;
		while (currentClazz != null) {
			for (Field field : currentClazz.getDeclaredFields()) {
				if (field.getName().equals(fieldName)) {
					field.setAccessible(true);
					return field;
				}
			}
			if (checkSuperclasses) {
				currentClazz = currentClazz.getSuperclass();
			} else {
				break;
			}
		}
		
		throw new ReflectionRuntimeException("no field named \"" + fieldName + "\" found in " + clazz);
	}
	
	public static Class<?> primitiveFor(Class<?> clazz) {
		if (clazz == Integer.class) {
			return Integer.TYPE;
		} else if (clazz == Double.class) {
			return Double.TYPE;
		} else if (clazz == Boolean.class) {
			return Boolean.TYPE;
		} else if (clazz == Long.class) {
			return Long.TYPE;
		} else if (clazz == Short.class) {
			return Short.TYPE;
		} else if (clazz == Byte.class) {
			return Byte.TYPE;
		} else if (clazz == Character.class) {
			return Character.TYPE;
		} else if (clazz == Float.class) {
			return Float.TYPE;
		} else {
			return clazz;
		}
	}

	public static String toStringViaReflection(Object o) {
	    if (o == null) {
	        return "null";
	    }
	    StringBuilder sb = new StringBuilder();
	    Class<?> clazz = o.getClass();
	    sb.append(clazz.getName() + "{");
	    boolean first = true;
	    for (Field field : clazz.getDeclaredFields()) {
	        if (!first) {
	            sb.append(",");
	        }
	        String fieldName = field.getName();
	        sb.append(fieldName + "=");
	        try {
	            sb.append(field.get(o));
	        } catch (IllegalAccessException e) {
	            // private field; see if there's an accessor for this field
	            String capitalized = fieldName.substring(0, 1).toUpperCase() + fieldName.substring(1);
                try {
                    Method method = clazz.getMethod("get" + capitalized);
                    sb.append(method.invoke(o));
                } catch (Exception ex) {
                	// empty
                }
                try {
                    Method method = clazz.getMethod("is" + capitalized);
                    sb.append(method.invoke(o));
                } catch (Exception ex) {
                	// empty
                }
	        } catch (Exception e) {
	            sb.append(e.getClass().getName());
	        }
	        first = false;
	    }
	    sb.append("}");
	    return sb.toString();
	}
}
