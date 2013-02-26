package de.matrixweb.ne;

import com.googlecode.javacpp.BytePointer;
import com.googlecode.javacpp.FunctionPointer;
import com.googlecode.javacpp.Loader;
import com.googlecode.javacpp.Pointer;
import com.googlecode.javacpp.annotation.ByVal;
import com.googlecode.javacpp.annotation.Cast;
import com.googlecode.javacpp.annotation.Name;
import com.googlecode.javacpp.annotation.Namespace;
import com.googlecode.javacpp.annotation.Platform;
import com.googlecode.javacpp.annotation.StdString;

/**
 * @author marwol
 */
@Platform(include = "native-engine.h", includepath = { "src/main/cpp",
    "target/v8/include" }, link = { "native-engine", "v8" }, linkpath = "target/ne")
public class NativeEngine {

  private NativeEngineImpl impl;

  /**
   * 
   */
  public NativeEngine() {
    createNativeEngine();
    this.impl.setRequireCallback(new FunctionCallback("__requireCallback__") {
      @Override
      @Cast("char*")
      public BytePointer call(@Cast("const char*") final BytePointer input) {
        throw new IllegalStateException("No require callback defined");
      }
    });
  }

  /**
   * @param requireCallback
   * 
   */
  public NativeEngine(final StringFunctor requireCallback) {
    createNativeEngine();
    this.impl.setRequireCallback(new FunctionCallback("__requireCallback__") {
      @Override
      @Cast("char*")
      public BytePointer call(@Cast("const char*") final BytePointer input) {
        return new BytePointer(requireCallback.call(input.getString()));
      }
    });
  }

  private synchronized void createNativeEngine() {
    this.impl = new NativeEngineImpl();
  }

  /**
   * @param functor
   */
  public void addCallbackFunction(final StringFunctor functor) {
    this.impl.addFunctionCallback(functor.getName(), new FunctionCallback(
        functor.getName()) {
      @Override
      @Cast("char*")
      public BytePointer call(@Cast("const char*") final BytePointer input) {
        return new BytePointer(functor.call(input.getString()));
      }
    });
  }

  /**
   * @param script
   */
  public synchronized void addScript(final String script) {
    addScript("<unknown>", script);
  }

  /**
   * @param name
   * @param script
   */
  public synchronized void addScript(final String name, final String script) {
    this.impl.addScript(name, script);
  }

  /**
   * @param input
   * @return Returns the output of the executed scripts as {@link String}
   */
  public synchronized String execute(final String input) {
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
  @Name("NativeEngine<FunctionCallback>")
  private static class NativeEngineImpl extends Pointer {

    static {
      Loader.load();
    }

    public NativeEngineImpl() {
      allocate();
    }

    private native void allocate();

    public native void setRequireCallback(@ByVal FunctionCallback callback);

    public native void addFunctionCallback(@StdString String name,
        @ByVal FunctionCallback callback);

    public native void addScript(@StdString String name,
        @StdString String script);

    @StdString
    public native String execute(@StdString String input);

  }

  /**
   * @author markusw
   */
  @Name("FunctionCallback")
  private static class FunctionCallback extends FunctionPointer {

    static {
      Loader.load();
    }

    private final String name;

    protected FunctionCallback(final String name) {
      allocate();
      this.name = name;
    }

    private final native void allocate();

    public String getName() {
      return this.name;
    }

    /**
     * @param input
     * @return Returns the callbacks response as {@link String}
     */
    @Cast("char*")
    public native BytePointer call(@Cast("const char*") BytePointer input);

  }

}
