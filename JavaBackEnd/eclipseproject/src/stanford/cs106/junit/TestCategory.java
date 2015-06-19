package stanford.cs106.junit;

import java.lang.annotation.*;

@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface TestCategory {
	String value() default "";
}
