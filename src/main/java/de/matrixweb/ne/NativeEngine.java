package de.matrixweb.ne;

import com.googlecode.javacpp.BytePointer;
import com.googlecode.javacpp.FunctionPointer;
import com.googlecode.javacpp.Loader;
import com.googlecode.javacpp.Pointer;
import com.googlecode.javacpp.annotation.Cast;
import com.googlecode.javacpp.annotation.Name;
import com.googlecode.javacpp.annotation.Namespace;
import com.googlecode.javacpp.annotation.Platform;
import com.googlecode.javacpp.annotation.StdString;

/**
 * @author marwol
 */
@Platform(include = "native-engine.h", includepath = { "src/main/cpp",
    "target/v8/include" }, link = { "native-engine" }, linkpath = "target/ne")
public class NativeEngine {

  private NativeEngineImpl impl;

  private final StringFunctionCallback callback;

  /**
   * 
   */
  public NativeEngine() {
    this(null);
  }

  /**
   * @param stringFunctor
   */
  public NativeEngine(final StringFunctor stringFunctor) {
    this.impl = new NativeEngineImpl();
    this.callback = new StringFunctionCallback() {
      @Override
      @Cast("char*")
      public BytePointer call(@Cast("const char*") final BytePointer input) {
        return stringFunctor == null ? input : new BytePointer(
            stringFunctor.call(input.getString()));
      }
    };
  }

  /**
   * @param script
   */
  public synchronized void addScript(final String script) {
    this.impl.addScript(script);
  }

  /**
   * @param name
   */
  public synchronized void prepareRun(final String name) {
    this.impl.prepareRun(name);
  }

  /**
   * @param input
   * @return Returns the output of the executed scripts as {@link String}
   */
  public synchronized String execute(final String input) {
    this.impl.setStringFunctionCallback(this.callback);
    return this.impl.execute(input);
  }

  /**
   * 
   */
  public synchronized void dispose() {
    this.impl.deallocate();
    this.impl = null;
  }

  @Namespace("ne")
  @Name("NativeEngine")
  private static class NativeEngineImpl extends Pointer {

    static {
      Loader.load();
    }

    public NativeEngineImpl() {
      allocate();
    }

    private native void allocate();

    public native void setStringFunctionCallback(StringFunctionCallback callback);

    public native void addScript(@StdString String script);

    public native void prepareRun(@StdString String name);

    @StdString
    public native String execute(@StdString String input);

  }

  /**
   * @author markusw
   */
  private static class StringFunctionCallback extends FunctionPointer {

    static {
      Loader.load();
    }

    protected StringFunctionCallback() {
      allocate();
    }

    private final native void allocate();

    /**
     * @param input
     * @return Returns the callbacks response as {@link String}
     */
    @Cast("char*")
    public native BytePointer call(@Cast("const char*") BytePointer input);

  }

}
