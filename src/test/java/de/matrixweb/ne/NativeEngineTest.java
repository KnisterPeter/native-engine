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
    NativeEngine ne = new NativeEngine();
    ne.addScript("function echo(msg) { return msg; }");
    ne.prepareRun("echo");
    assertThat(ne.execute("hallo"), is("hallo"));
  }

}
