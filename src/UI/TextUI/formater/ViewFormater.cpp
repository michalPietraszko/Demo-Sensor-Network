#include <BasicFormater.hpp>
#include <MenuViewFormater.hpp>

/* MenuViewFormater */
const std::vector<FormatGlyphs> MenuViewFormater::headline{FormatGlyphs::element,
                                                              FormatGlyphs::newLine,
                                                              FormatGlyphs::newLine};

const std::vector<FormatGlyphs> MenuViewFormater::cellName{FormatGlyphs::nextInList,
                                                              FormatGlyphs::element,
                                                              FormatGlyphs::newLine};

const std::vector<FormatGlyphs> MenuViewFormater::cellStatus{FormatGlyphs::tab,
                                                                FormatGlyphs::element,
                                                                FormatGlyphs::newLine,
                                                                FormatGlyphs::newLine};

/* BasicFormater */
const std::vector<FormatGlyphs> BasicFormater::any{FormatGlyphs::element, FormatGlyphs::newLine};

/**
 *  headline\n
 *  \\n
 *  1. cellName\n
 *  \t cellStatus\n
 *  \\n
 *  2. ...
 */