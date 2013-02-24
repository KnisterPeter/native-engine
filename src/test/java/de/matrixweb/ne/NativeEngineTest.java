package de.matrixweb.ne;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
@RunWith(Parameterized.class)
public class NativeEngineTest {

  private static final int NUMBER = 100;

  /**
   * @return ...
   */
  @Parameters
  public static List<Object[]> data() {
    final List<Object[]> list = new ArrayList<Object[]>();
    for (int i = 0; i < NUMBER; i++) {
      list.add(new Object[] { i });
    }
    return list;
  }

  /**
   * @param i
   */
  public NativeEngineTest(final int i) {
  }

  /**
   * 
   */
  @Test
  public void testNativeEngine() {
    final NativeEngine ne = new NativeEngine();
    try {
      ne.addScript("function echo(msg) { return msg; }");
      ne.addScript("function echo2(msg) { return msg; }");
      assertThat(ne.execute("echo('hallo')"), is("hallo"));
      assertThat(ne.execute("echo('hallo2')"), is("hallo2"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * 
   */
  @Test
  public void testNativeEngineCallback() {
    final NativeEngine ne = new NativeEngine();
    try {
      ne.addCallbackFunction(new StringFunctor("resolve") {
        @Override
        public String call(final String input) {
          return "welt";
        }
      });
      ne.addScript("function run(msg) { return new String(resolve(msg)); }");
      assertThat(ne.execute("run('hallo')"), is("welt"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * 
   */
  @Test
  public void testNativeEngineCallbacks() {
    final NativeEngine ne = new NativeEngine();
    try {
      ne.addCallbackFunction(new StringFunctor("a") {
        @Override
        public String call(final String input) {
          return "aa";
        }
      });
      ne.addCallbackFunction(new StringFunctor("b") {
        @Override
        public String call(final String input) {
          return "bb";
        }
      });
      assertThat(ne.execute("new String(a('') + b(''));"), is("aabb"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * 
   */
  @Test
  public void testNativeEngineRequire() {
    final NativeEngine ne = new NativeEngine(new StringFunctor("") {
      @Override
      public String call(final String input) {
        return "exports.echo = function(val) { return val; };";
      }
    });
    try {
      assertThat(
          ne.execute("var echo = require('somesource').echo; new String(echo('hello'));"),
          is("hello"));
    } finally {
      ne.dispose();
    }
  }

}
