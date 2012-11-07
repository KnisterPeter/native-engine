package de.matrixweb.jv8;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;
import javax.script.ScriptException;

import org.junit.Test;

/**
 * @author marwol
 */
public class JV8Test {

  @Test
  public void startup() throws ScriptException {
    ScriptEngineFactory factory = new JV8ScriptEngineFactory();
    ScriptEngine engine = factory.getScriptEngine();
    engine.eval("var a = 'b';");
  }

}
