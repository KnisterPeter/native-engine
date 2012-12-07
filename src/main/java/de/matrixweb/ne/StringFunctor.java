package de.matrixweb.ne;

/**
 * @author markusw
 */
public interface StringFunctor {

  /**
   * @param input
   *          The input {@link String} to operate on
   * @return Returns the processed {@link String}
   */
  String call(String input);

}
