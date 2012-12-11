package de.matrixweb.ne;

import org.junit.Test;

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
      ne.addScript("function echo2(msg) { return msg; }");
      assertThat(ne.execute("echo('hallo')"), is("hallo"));
      assertThat(ne.execute("echo('hallo2')"), is("hallo2"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * 
   */
  @Test
  public void testNativeEngineCallback() {
    final NativeEngine ne = new NativeEngine();
    try {
      ne.addCallbackFunction(new StringFunctor("resolve") {
        @Override
        public String call(final String input) {
          return "welt";
        }
      });
      ne.addScript("function run(msg) { return new String(resolve(msg)); }");
      assertThat(ne.execute("run('hallo')"), is("welt"));
    } finally {
      ne.dispose();
    }
  }

}
