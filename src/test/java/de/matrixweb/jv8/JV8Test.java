package de.matrixweb.jv8;

import javax.script.ScriptException;

import org.junit.Test;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
public class JV8Test {

  /**
   * @throws ScriptException
   */
  @Test
  public void startup() throws ScriptException {
    final JV8ScriptEngineFactory factory = new JV8ScriptEngineFactory();
    final JV8 engine = factory.getScriptEngine();
    System.out.println(engine);
    assertThat((String) engine.eval("'Hello World!'"), is("Hello World!"));
  }

}
