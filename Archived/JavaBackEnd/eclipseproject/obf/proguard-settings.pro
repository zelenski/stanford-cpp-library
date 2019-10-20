#
# This ProGuard configuration file illustrates how to process ProGuard
# (including its main application, its GUI, its Ant task, and its WTK plugin),
# and the ReTrace tool, all in one go.
# Configuration files for typical applications will be very similar.
# Usage:
#     java -jar proguard.jar @proguardall.pro
#

# Specify the input jars, output jars, and library jars.
# We'll read all jars from the lib directory, process them, and write the
# processed jars to a new out directory.

-injars  in
-outjars out

# You may have to adapt the paths below.

#-libraryjars <java.home>/lib/rt.jar
#-libraryjars /usr/lib/jvm/java-6-sun-1.6.0.24/jre/lib/rt.jar
#-libraryjars /usr/share/java/ant.jar
#-libraryjars /usr/local/java/wtk2.5.2/wtklib/kenv.zip
#-libraryjars /cygdrive/c/Program Files (x86)/Java/jre7/lib/rt.jar
-libraryjars jars/rt.jar

# Allow methods with the same signature, except for the return type,
# to get the same obfuscation name.

-overloadaggressively

# Put all obfuscated classes into the nameless root package.

-repackageclasses ''

# Adapt the names and contents of the resource files.

-adaptresourcefilenames    **.properties,**.gif,**.jpg,**.txt
#-adaptresourcefilecontents proguard/ant/task.properties

# The main entry points.

-keep public class Main {
    public static void main(java.lang.String[]);
}

-keep public class CommandLineMain {
    public static void main(java.lang.String[]);
}

# -keepclassmembers enum * {}

-keep public enum **

-keepclassmembers public enum ** {
	public *;
}

#-keepclasseswithmembers public class * {
#    public static void main(java.lang.String[]);
#}

#-keep public class * extends java.applet.Applet
#-keep public class * extends javax.swing.JApplet
#-keep public class * implements javax.servlet.Servlet

#-keepnames class * implements java.io.Serializable
#-keepclassmembers class * implements java.io.Serializable

-target 1.5
