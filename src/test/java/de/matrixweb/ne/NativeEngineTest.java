package de.matrixweb.ne;

import org.junit.Test;

import com.googlecode.javacpp.BytePointer;

import de.matrixweb.ne.NativeEngine.StringFunctionCallback;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
public class NativeEngineTest {

  /**
   * 
   */
  @Test
  public void testNativeEngine() {
    final NativeEngine ne = new NativeEngine();
    try {
      ne.addScript("function echo(msg) { return msg; }");
      ne.prepareRun("echo");
      assertThat(ne.execute("hallo"), is("hallo"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * 
   */
  @Test
  public void testNativeEngineCallback() {
    final NativeEngine ne = new NativeEngine(new StringFunctionCallback() {
      @Override
      public BytePointer call(final BytePointer p) {
        final String str = p.getString();
        System.out.println("Callback Output: " + str);
        return new BytePointer("welt");
      }
    });
    try {
      ne.addScript("function run(msg) { return resolver.resolve(msg); }");
      ne.prepareRun("run");
      assertThat(ne.execute("hallo"), is("welt"));
    } finally {
      ne.dispose();
    }
  }

}
