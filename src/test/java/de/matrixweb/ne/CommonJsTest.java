package de.matrixweb.ne;

import org.junit.Test;

import de.matrixweb.ne.NativeEngine.NativeEngineException;

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
   * @throws NativeEngineException
   */
  @Test
  public void testRequire() throws NativeEngineException {
    final NativeEngine ne = new NativeEngine(new StringFunctor("") {
      @Override
      public String call(final String input) {
        return "exports.echo = function(val) { return val; };";
      }
    });
    try {
      assertThat(
          ne.execute("var echo = require('somesource').echo; echo('hello')"),
          is("hello"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * @throws NativeEngineException
   */
  @Test
  public void testRequireWithSubmodules() throws NativeEngineException {
    final String top = "var sub = require('./sub');exports.echo = sub.echo2;";
    final String sub1 = "var sub2 = require('../lib2/sub'); exports.echo2 = sub2.echo3";
    final String sub2 = "exports.echo3 = function(val) { return val; }";
    final NativeEngine ne = new NativeEngine(new StringFunctor("") {
      @Override
      public String call(final String input) {
        if ("lib/top".equals(input)) {
          return top;
        } else if ("lib/sub".equals(input)) {
          return sub1;
        } else if ("lib2/sub".equals(input)) {
          return sub2;
        }
        throw new RuntimeException("Module '" + input + "' not found");
      }
    });
    try {
      assertThat(
          ne.execute("var top = require('./lib/top'); top.echo('hello');"),
          is("hello"));
    } finally {
      ne.dispose();
    }
  }

  /**
   * @throws NativeEngineException
   */
  @Test
  public void testModulesExport() throws NativeEngineException {
    final NativeEngine ne = new NativeEngine(new StringFunctor("") {
      @Override
      public String call(final String input) {
        return "module.exports.echo = function(val) { return val; };";
      }
    });
    try {
      assertThat(
          ne.execute("var some = require('somesource'); some.echo('hello')"),
          is("hello"));
    } finally {
      ne.dispose();
    }
  }

}
