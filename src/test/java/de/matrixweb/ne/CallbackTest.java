package de.matrixweb.ne;

import org.junit.Test;

import de.matrixweb.ne.NativeEngine.NativeEngineException;

import static org.junit.Assert.*;

import static org.hamcrest.CoreMatchers.*;

/**
 * @author marwol
 */
public class CallbackTest extends AbstractTestSetup {

  /**
   * @param i
   */
  public CallbackTest(final int i) {
  }

  /**
   * @throws NativeEngineException
   */
  @Test
  public void testCallbacks() throws NativeEngineException {
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

}
