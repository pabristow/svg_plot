/*! \file
    \brief Plot document structure whose order controls the painting order,
    later layers overwriting earlier layers.
*/

// svg_style_detail.hpp
// Copyright Jacob Voytko 2007
// Copyright  Paul A. Bristow 2008, 2013, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_STYLE_DETAIL_HPP
#define BOOST_SVG_SVG_STYLE_DETAIL_HPP

#include <string>
// using std::string;

// This module provides an ID @c std::string from a named parameter 
//(macro like PLOT_WINDOW_BACKGROUND)
// so, for example, document_ids_[PLOT_BACKGROUND] == "background".
// to be used as a SVG group is thus: <g id="background" ... /g>

namespace boost
{
namespace svg
{
namespace detail
{
// Caution: these two *enum and ids must match* because
// the enum value is used to index the array of id strings.
// void set_ids() copies all strings to matching image.get_g_element(i).id()
// Add any new id items to both!

enum plot_doc_structure
{ //! \enum plot_doc_structure Plot document structure whose order controls the painting order, later ones overwriting earlier layers.
  PLOT_BACKGROUND = 0, //! Must be zero to index array document_ids_[]
    PLOT_WINDOW_BACKGROUND, //! the smaller plot window (if used).
    PLOT_Y_MINOR_GRID, //! Y minor grid.
    PLOT_Y_MAJOR_GRID, //! Y major grid.
    PLOT_X_MINOR_GRID, //! X minor grid.
    PLOT_X_MAJOR_GRID, //! X major grid.
    PLOT_Y_AXIS, //! X axis line.
    PLOT_X_AXIS, //! Y axis line.
    PLOT_Y_MINOR_TICKS, //! Y minor ticks.
    PLOT_X_MINOR_TICKS, //! X minor ticks
    PLOT_Y_MAJOR_TICKS, //! Y major ticks.
    PLOT_X_MAJOR_TICKS, //! X major ticks.
    PLOT_X_TICKS_VALUES, //! X-axis tick values labels, for example 10, 20, 30 ...
    PLOT_Y_TICKS_VALUES, //! Y-axis tick values labels, for example 1, 2, 3 ...
    PLOT_Y_LABEL, //! Y axis text labels "length (cm)".
    PLOT_X_LABEL, //! X axis text labels "height (m)".
    PLOT_DATA_LINES, //! Lines joining data points.
    PLOT_DATA_UNC3, //!  Uncertainty zone of 3 sd from data points.
    PLOT_DATA_UNC2, //! Uncertainty zone of 2 sd from data points.
    PLOT_DATA_UNC1, //! Uncertainty zone of 1 sd from data points.
    PLOT_DATA_POINTS, //! Normal data point markers.
    PLOT_LIMIT_POINTS, //! 'At limit' infite or NaN data point markers.
    PLOT_LEGEND_BACKGROUND, //! Legend box.
    PLOT_LEGEND_POINTS, //! Legend data series point markers, circle, cross...
    PLOT_LEGEND_TEXT, //! Legend text describing each data series.
    PLOT_TITLE, //! Title of the whole plot.
    PLOT_X_POINT_VALUES, //! X Data point value labels.
    PLOT_Y_POINT_VALUES, //! Y Data point value labels.
    PLOT_FUNCTIONS, //! Lines and curves, often to show a fit to the data.
    PLOT_NOTES, //! Free text and shapes to annotate a plot.
    SVG_PLOT_DOC_CHILDREN //! Last enum value used as count of children (22).
};

std::string document_ids_[]= //!< Strings used describe and identify SVG layers in .svg files (matching enum plot_doc_structure).
{ // Care: must match enum plot_doc_structure.
    "imageBackground", // the whole svg image.
    "plotBackground", // the smaller plot window (if used).
    "yMinorGrid",
    "yMajorGrid",
    "xMinorGrid",
    "xMajorGrid",
    "yAxis",
    "xAxis", // the X and Y axis lines.
    "yMinorTicks",
    "xMinorTicks",
    "yMajorTicks",
    "xMajorTicks",
    "xTicksValues", //! X-axis tick values labels, for example 10, 20, 30 ...
    "yTicksValues", //! Y-axis tick values labels, for example 1, 2, 3 ...
    "yLabel",
    "xLabel",   // axis text labels "length (cm)"
    "plotLines", // normal data point markers.
    "plotUnc3",   // Uncertainty zone of 3 sd from data points.
    "plotUnc2",   // Uncertainty zone of 2 sd from data points.
    "plotUnc1",   // Uncertainty zone of 2 sd from data points.
    "plotPoints", // normal data point markers.
    "limitPoints",  // at limit or NaN data point markers.
    "legendBackground", // legend box.
    "legendPoints", // data series point markers, circle, cross...
    "legendText", // text describing each data series.
    "title",  // of the whole plot.
    "plotXValues","plotYValues", // Data point value labels.
    "plotFunctions", // Lines and curves, often to show a fit to the data.
    "plotNotes", // Free text and shapes to annotate diagram.
    "plotDocChildren" // This last string is not used.
}; //  std::string document_ids_

} // namespace detail
} // namespace svg
} // namespace boost
#endif // BOOST_SVG_SVG_STYLE_DETAIL_HPP
