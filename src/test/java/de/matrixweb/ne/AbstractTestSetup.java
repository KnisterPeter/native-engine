package de.matrixweb.ne;

import java.util.ArrayList;
import java.util.List;

import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

/**
 * @author markusw
 */
@RunWith(Parameterized.class)
public abstract class AbstractTestSetup {

  private static final int NUMBER = 1;

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

}
