package de.matrixweb.ne;

import org.junit.Test;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
public class CommonJsTest extends AbstractTestSetup {

  /**
   * @param i
   */
  public CommonJsTest(final int i) {
  }

  /**
   * 
   */
  @Test
  public void testRequire() {
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
