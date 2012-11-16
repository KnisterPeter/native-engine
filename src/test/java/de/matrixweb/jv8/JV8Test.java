package de.matrixweb.jv8;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

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
  public void evalString() throws ScriptException {
    final JV8ScriptEngineFactory factory = new JV8ScriptEngineFactory();
    final JV8 engine = factory.getScriptEngine();
    assertThat((String) engine.eval("'Hello World!'"), is("Hello World!"));
  }

  /**
   * @throws ScriptException
   * @throws IOException
   */
  @Test
  public void evalReader() throws ScriptException, IOException {
    final JV8ScriptEngineFactory factory = new JV8ScriptEngineFactory();
    final JV8 engine = factory.getScriptEngine();
    final Reader reader = new InputStreamReader(getClass().getResourceAsStream(
        "/simple_string.js"));
    try {
      assertThat((String) engine.eval(reader), is("Hello World!"));
    } finally {
      reader.close();
    }
  }

}
