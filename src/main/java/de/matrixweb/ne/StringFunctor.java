package de.matrixweb.ne;

/**
 * @author markusw
 */
public abstract class StringFunctor {

  private final String name;

  /**
   * @param name
   */
  public StringFunctor(final String name) {
    this.name = name;
  }

  /**
   * @return Returns the name this function should have in v8
   */
  public String getName() {
    return this.name;
  }

  /**
   * @param input
   *          The input {@link String} to operate on
   * @return Returns the processed {@link String}
   */
  public abstract String call(String input);

}
