package de.matrixweb.jv8;

import java.io.Reader;

import javax.script.Bindings;
import javax.script.ScriptContext;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;
import javax.script.ScriptException;

import com.googlecode.javacpp.Loader;
import com.googlecode.javacpp.Pointer;
import com.googlecode.javacpp.annotation.Name;
import com.googlecode.javacpp.annotation.Namespace;
import com.googlecode.javacpp.annotation.Platform;

/**
 * @author marwol
 */
@Platform(include = "jv8.h", includepath = { "src/main/cpp", "target/v8/include" }, link = { "v8", "jv8" }, linkpath = "target/jv8")
@Namespace("jv8")
public class JV8 implements ScriptEngine {

  private final JV8Impl impl;

  /**
   * 
   */
  public JV8() {
    this.impl = new JV8Impl();
  }

  /**
   * @see javax.script.ScriptEngine#eval(java.lang.String,
   *      javax.script.ScriptContext)
   */
  @Override
  public Object eval(final String script, final ScriptContext context) throws ScriptException {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#eval(java.io.Reader,
   *      javax.script.ScriptContext)
   */
  @Override
  public Object eval(final Reader reader, final ScriptContext context) throws ScriptException {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#eval(java.lang.String)
   */
  @Override
  public Object eval(final String script) throws ScriptException {
    return this.impl.eval(script);
  }

  /**
   * @see javax.script.ScriptEngine#eval(java.io.Reader)
   */
  @Override
  public Object eval(final Reader reader) throws ScriptException {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#eval(java.lang.String,
   *      javax.script.Bindings)
   */
  @Override
  public Object eval(final String script, final Bindings n) throws ScriptException {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#eval(java.io.Reader, javax.script.Bindings)
   */
  @Override
  public Object eval(final Reader reader, final Bindings n) throws ScriptException {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#put(java.lang.String, java.lang.Object)
   */
  @Override
  public void put(final String key, final Object value) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#get(java.lang.String)
   */
  @Override
  public Object get(final String key) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#getBindings(int)
   */
  @Override
  public Bindings getBindings(final int scope) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#setBindings(javax.script.Bindings, int)
   */
  @Override
  public void setBindings(final Bindings bindings, final int scope) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#createBindings()
   */
  @Override
  public Bindings createBindings() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#getContext()
   */
  @Override
  public ScriptContext getContext() {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#setContext(javax.script.ScriptContext)
   */
  @Override
  public void setContext(final ScriptContext context) {
    throw new UnsupportedOperationException("TODO");
  }

  /**
   * @see javax.script.ScriptEngine#getFactory()
   */
  @Override
  public ScriptEngineFactory getFactory() {
    throw new UnsupportedOperationException("TODO");
  }

  @Name("JV8")
  private static class JV8Impl extends Pointer {

    static {
      Loader.load();
    }

    public JV8Impl() {
      allocate();
    }

    private native void allocate();

    public native V8Value eval(final String script) throws ScriptException;

  }

  @Name("Value")
  private static abstract class V8Value extends Pointer {
  }

}
