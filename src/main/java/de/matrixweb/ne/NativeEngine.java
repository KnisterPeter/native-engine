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
@Platform(include = "native-engine.h", includepath = { "src/main/cpp", "target/v8/include" }, link = { "native-engine" }, linkpath = "target/ne")
public class NativeEngine {

  private NativeEngineImpl impl;

  /**
   * 
   */
  public NativeEngine() {
    this.impl = new NativeEngineImpl();
    this.impl.setResolverCallback(new ResolverCallback() {
      @Override
      public BytePointer call(final BytePointer p) {
        String str = p.getString();
        return new BytePointer("result + " + str);
      }
    });
  }

  /**
   * @param script
   */
  public void addScript(final String script) {
    this.impl.addScript(script);
  }

  /**
   * @param name
   */
  public void prepareRun(final String name) {
    this.impl.prepareRun(name);
  }

  /**
   * @param input
   * @return Returns the output of the executed scripts as {@link String}
   */
  public String execute(final String input) {
    return this.impl.execute(input);
  }

  /**
   * 
   */
  public void dispose() {
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

    public native void setResolverCallback(ResolverCallback callback);

    public native void addScript(@StdString String script);

    public native void prepareRun(@StdString String name);

    @StdString
    public native String execute(@StdString String input);

  }

  private static class ResolverCallback extends FunctionPointer {

    static {
      Loader.load();
    }

    protected ResolverCallback() {
      allocate();
    }

    private final native void allocate();

    @Cast("char*")
    public native BytePointer call(@Cast("const char*") BytePointer p);

  }

}
