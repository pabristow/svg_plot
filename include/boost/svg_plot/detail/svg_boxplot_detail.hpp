/*! \file

   \brief Boost.Plot SVG Box plot Implemention details.
   \details See svg_boxplot.hpp for user functions.
   See also svg_style_detail.hpp for enum plot_doc_structure.
   Caution: these two enum and ids must match because
   the enum value is used to index the array of id strings.
   void set_ids() copies all strings to matching image.get_g_element(i).id()
   So add any new id items to both!

   \author Jacob Voytko and Paul A. Bristow
*/

// file svg_boxplot_detail.hpp
// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_BOXPLOT_DETAIL_HPP
#define BOOST_SVG_BOXPLOT_DETAIL_HPP

namespace boost {
namespace svg {
namespace boxplot
{
  enum boxplot_doc_structure
  { //! \enum boxplot_doc_structure groups that form the boxplot svg document structure.  Order controls the painting order, later ones overwriting earlier layers.
    PLOT_BACKGROUND = 0, //!< the whole svg image. (Must be zero to index array document_ids_[])
    PLOT_WINDOW_BACKGROUND, //!< the smaller plot window (if used).
    X_AXIS, //!< X axis group element.
    Y_AXIS, //!< Y axis group element.
    X_TICKS, //!< X ticks group element.
    Y_MAJOR_TICKS, //!< Y majro ticks group element.
    Y_MINOR_TICKS, //!< Y minor ticks group element.
    Y_MAJOR_GRID, //!< Y major grid group element.
    Y_MINOR_GRID, //!< Y minor grid group element.
    VALUE_LABELS, //!< axis values labels group element.
    Y_LABEL, //!< Y labels group element.
    X_LABEL, //!< X labels group element.
    BOX_AXIS,  //!< Box axis group element.
    BOX, //!< box group element.
    MEDIAN, //!< median group element.
    WHISKER, //!< whisker group element.
    MILD_OUTLIERS, //!< mild outliers group element.
    EXTREME_OUTLIERS, //!< extreme outliers group element.
    DATA_VALUE_LABELS, //!< Data values labels group element.
    PLOT_TITLE, //!< Whole plot title text group element.
    PLOT_NOTES, //!< Free text and shapes to annotate diagram.
    BOXPLOT_DOC_CHILDREN // 22 items
  };

  //! String descriptors used in SVG XML (matching enum boxplot_doc_structure).
  std::string document_ids_[22] =
  {
      "imageBackground", //< the whole svg image.
      "plotBackground", //< the smaller plot window (if used).
      "X-axis",
      "Y-axis",
      "xMajorTicks", // Probably not used for Boxplots.
      "yMajorTicks",
      "yMinorTicks",
      "YGrid",
      "yMinorGrid",
      "valueLabels", // tick Value Labels
      "yLabel",
      "xLabel",  // axis text labels "length (cm)"
      "boxAxis",
      "Box", // vertical Box axis line and box.
      "Median",
      "whisker",
      "mild_outliers",
      "extreme_outliers",
      "Data Values",
      "title",  // of the whole plot.
      "plotNotes", // Free text and shapes to annotate diagram.
      "boxplotDocChildren" // This last string is not used.
  }; // std::string document_ids_

} // namespace boxplot
} // namespace svg
} // namespace boost

#endif // BOOST_SVG_BOXPLOT_DETAIL_HPP
