package de.matrixweb.jv8;

import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

import javax.script.ScriptException;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
public class JV8Test {

  private JV8 engine;

  /**
   * 
   */
  @Before
  public void setUp() {
    final JV8ScriptEngineFactory factory = new JV8ScriptEngineFactory();
    this.engine = factory.getScriptEngine();
  }

  /**
   * @throws ScriptException
   */
  @Test
  public void evalString() throws ScriptException {
    assertThat((String) this.engine.eval("'Hello World!'"), is("Hello World!"));
  }

  /**
   * @throws ScriptException
   * @throws IOException
   */
  @Test
  public void evalReader() throws ScriptException, IOException {
    final Reader reader = new InputStreamReader(getClass().getResourceAsStream(
        "/simple_string.js"));
    try {
      assertThat((String) this.engine.eval(reader), is("Hello World!"));
    } finally {
      reader.close();
    }
  }

  /**
   * @throws ScriptException
   */
  @Test
  public void testPut() throws ScriptException {
    final File file = new File("/tmp");
    this.engine.put("file", file);
    assertThat((Boolean) this.engine.eval("file.exists()"), is(true));
  }

}
