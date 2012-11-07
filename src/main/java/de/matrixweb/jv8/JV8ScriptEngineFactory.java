package de.matrixweb.jv8;

import java.util.List;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;

/**
 * @author marwol
 */
public class JV8ScriptEngineFactory implements ScriptEngineFactory {

  private JV8 engine;

  /**
   * @see javax.script.ScriptEngineFactory#getEngineName()
   */
  @Override
  public String getEngineName() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getEngineVersion()
   */
  @Override
  public String getEngineVersion() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getExtensions()
   */
  @Override
  public List<String> getExtensions() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getMimeTypes()
   */
  @Override
  public List<String> getMimeTypes() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getNames()
   */
  @Override
  public List<String> getNames() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getLanguageName()
   */
  @Override
  public String getLanguageName() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getLanguageVersion()
   */
  @Override
  public String getLanguageVersion() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getParameter(java.lang.String)
   */
  @Override
  public Object getParameter(final String key) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getMethodCallSyntax(java.lang.String,
   *      java.lang.String, java.lang.String[])
   */
  @Override
  public String getMethodCallSyntax(final String obj, final String m, final String... args) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getOutputStatement(java.lang.String)
   */
  @Override
  public String getOutputStatement(final String toDisplay) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getProgram(java.lang.String[])
   */
  @Override
  public String getProgram(final String... statements) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngineFactory#getScriptEngine()
   */
  @Override
  public ScriptEngine getScriptEngine() {
    if (this.engine == null) {
      this.engine = new JV8();
    }
    return this.engine;
  }

}
