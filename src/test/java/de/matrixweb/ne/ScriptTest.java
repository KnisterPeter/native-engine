package de.matrixweb.ne;

import org.junit.Test;

import de.matrixweb.ne.NativeEngine.NativeEngineException;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
public class ScriptTest extends AbstractTestSetup {

  /**
   * @param i
   */
  public ScriptTest(final int i) {
  }

  /**
   * @throws NativeEngineException
   */
  @Test
  public void runSimpleScript() throws NativeEngineException {
    final NativeEngine ne = new NativeEngine();
    try {
      assertThat(ne.execute("(function(msg) { return msg; })('a')"), is("a"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * @throws NativeEngineException
   */
  @Test
  public void runMultipleScriptsMultipleTimes() throws NativeEngineException {
    final NativeEngine ne = new NativeEngine();
    try {
      ne.addScript("function echo(msg) { return msg; }");
      ne.addScript("function echo2(msg) { return msg; }");
      assertThat(ne.execute("echo('hallo')"), is("hallo"));
      assertThat(ne.execute("echo('hallo2')"), is("hallo2"));
      assertThat(ne.execute("echo2('hallo2')"), is("hallo2"));
    } finally {
      ne.dispose();
    }
  }

}
