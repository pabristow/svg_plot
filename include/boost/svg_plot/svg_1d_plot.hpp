/*!
  \file
  \brief Create 1D plots in Scalable Vector Graphic (SVG) format.
  \details Provides @c svg_1d_plot data and function to create plots, and @c svg_1d_plot_series to allow data values to be added.\n

  Very many functions allow fine control of the appearance and layout of plots and data markers.\n

  (Items common to 1D and 2D use functions and classes in @c axis_plot_frame).
 */

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2009, 2011, 2012, 2013, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_1D_PLOT_HPP
#define BOOST_SVG_SVG_1D_PLOT_HPP

#include <boost/iterator/transform_iterator.hpp>
// using boost::make_transform_iterator;

#include <boost/svg_plot/svg.hpp>
#include <boost/svg_plot/svg_style.hpp>
#include <boost/svg_plot/detail/axis_plot_frame.hpp> // Code shared with 2D.
#include <boost/svg_plot/detail/functors.hpp>
//using boost::svg::detail::unc_1d_convert;
#include <boost/svg_plot/detail/numeric_limits_handling.hpp>
//#include <boost/quan/unc.hpp>
#include <boost/quan/meas.hpp>

using boost::svg::detail::limit_NaN;  // Handling of non-finite data values.

#include <boost/svg_plot/detail/auto_axes.hpp>
// Provides variants for @c void boost::svg::scale_axis

#include <vector>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>

namespace boost
{
namespace svg
{
  // Forward declarations.
//! \cond DETAIL
  const std::string strip_e0s(std::string s); // Strip unnecessary zeros and e and sign - to minimize value-label length.
//! \endcond
  class svg_1d_plot;   // 1D Plot (that may include one or more data-series).

  //class svg_1d_plot_series; // A data-series to be added to a 1D Plot.

// ------------------------------------------------------------------
// This allows us to store plot state locally in svg_plot.
// Not stored in "svg" because transforming the points after they are
// written to the document would be difficult. We store the Cartesian
// coordinates locally and transform them before we write them.
// ------------------------------------------------------------------
 /*! \class boost::svg::svg_1d_plot_series
   \brief Holds a series of data values (points) to be plotted.
   \details Scan each data-point sorting them into the appropriate
   @c std::vectors, normal or not (NaN or infinite).\n
   Member functions allow control of data-points markers and lines joining them,
   and their appearance, shape, color and size.\n
   data-points can include their value, and optionally uncertainty and number of degrees of freedom.\n

   Each data-series can have a title that can be shown on a legend-box with identifying symbols.
*/
class svg_1d_plot_series
{
public:
//! \cond DETAIL
  std::vector<Meas> series_; //!< Normal 'OK to plot' @c Meas data, values including uncertainty and timestamp.
  //std::vector<unc<false> > series_; //!< Normal 'OK to plot' data values.
  std::vector<double> series_limits_; //!< 'limit' values: too big, too small or NaN.
  // TODO should these be unc too?  Uncertainty info is meaningless, but timestamp etc are OK.

  std::string title_; //!< title of data-series (to show on legend using legend_style).
  plot_point_style point_style_; //!< circle, square...
  // Limit Marker  settings now in svg_1d_plot class.
  //plot_point_style limit_point_style_; //!< Default is cone pointing down for 2D or point right for 1D.
  plot_line_style line_style_; //!< No line style for 1-D, only for 2-D.
//! \endcond

  // Constructor svg_1d_plot_series.
  /* Scan each data-point between the iterators that are passed,
    sorting them into the appropriate @c std::vectors, either normal or not (NaN or infinite).

    \tparam C An STL container: \code std::array, std::vector<double>, std::set, std::map ... \endcode
  */

  template <typename C>
  svg_1d_plot_series(
  C begin,
  C end,
  const std::string& title = ""); // Title of plot data-series, for example:"Monday", "Tuesday"...

  // Forward declarations of Set functions for the plot series.
  svg_1d_plot_series& fill_color(const svg_color& col_); // Set fill color for plot point marker(s) (chainable).
  svg_1d_plot_series& stroke_color(const svg_color& col_); // Set stroke color for plot point marker(s) (chainable).
  svg_1d_plot_series& shape(point_shape shape_); // Set shape for plot point marker(s) (chainable).
  svg_1d_plot_series& symbols(const std::string s); // Set symbol(s) for plot point marker(s) (chainable).
  svg_1d_plot_series& size(int size_); // Set font size for plot point marker(s) (chainable).
  svg_1d_plot_series& line_color(const svg_color& col_); // Set color for any line joining data-points (chainable).
  svg_1d_plot_series& line_width(double wid_); // Set line width for plot point marker(s) (chainable).
  svg_1d_plot_series& line_on(bool on_); // Set true for line joining points for plot point marker(s) (chainable).
  svg_1d_plot_series& bezier_on(bool on_); // Set true for curve joining points for plot point marker(s) (chainable).
//  svg_1d_plot_series& plus_inf_limit_point_color(const svg_color& col_); // Set stroke color for 'at +infinity limits' point marker.

  // Get functions for the plot series.
  svg_color fill_color(); // Get fill color for plot point marker(s).
  svg_color stroke_color(); // Get stroke color for plot point marker(s).
  int size(); // Get size for plot point marker(s).
  point_shape shape(); // Get shape for plot point marker(s).
  const std::string symbols(); // Get symbols for plot point marker(s).
  double line_width(); // Get line width for plot point marker(s).
  bool line_on(); // Get true if line is to join data-points.
  bool bezier_on(); // Get true if curve if to join data-points.
  size_t series_count(); // Get number of normal data-points in this data-series.
  size_t series_limits_count(); // Get number of 'at limits' data-points in this data-series.
}; // class svg_1d_plot_series

 /*! \class boost::svg::svg_1d_plot
     \brief All settings for a plot that control the appearance,
     and functions to get and set these settings.\n
     (But see @c svg_1d_plot_series to control appearance of data-points).

      @c axis_plot_frame.hpp contains functions common to 1 and 2-D.
      \details Several versions of the function @c plot are provided to allow data to be in different sources,
      and to allow either all data in a container or just a sub-range to be plotted.
      \sa @c svg_2d_plot.hpp for the 2-D version.
  */
class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>
{
  friend void show_1d_plot_settings(svg_1d_plot&, std::ostream&);
  //friend void show_1d_plot_settings(svg_1d_plot&); // Surely not needed?
  friend class detail::axis_plot_frame<svg_1d_plot>;
  // axis_plot_frame.hpp contains functions common to 1-D and 2-D.

public:
  //protected: // Perhaps, but seems little benefit?

  // Data members Declarations.
//! \cond DETAIL
  // Member data names conventionally end with _, for example: border_margin_,
  // and set & get accessor functions are named *without* _ suffix,
  // for example: border_margin() & border_margin(int).

  double x_scale_; //!< Scale used for transform from Cartesian to SVG coordinates.
  double x_shift_; //!< Shift from SVG origin is top left, Cartesian is bottom right.
  double y_scale_; //!< Scale used for transform from Cartesian to SVG coordinates.
  double y_shift_; //!< Shift from SVG origin is top left, Cartesian is bottom right.

  svg image_; //!< Stored so as to avoid rewriting style information constantly.

  double text_margin_; //!< Marginal space around text items like title,

  text_style a_style_; //!< Default text style that contains font size & type etc.
  text_style title_style_; //!< style (font etc) of title.
  text_style legend_text_style_; //!<  style (font etc of legend).
  text_style legend_title_style_; //!< Style for legend title.
  text_style x_axis_label_style_; //!< style of X axis label.
  text_style y_axis_label_style_;  //!< Not used for 1D, but needed by axis_plot_frame.hpp.
  text_style x_ticks_value_label_style_; //!< style of X-ticks value-label.
  text_style y_ticks_value_label_style_; //!< Not used for 1D, but needed by axis_plot_frame.hpp.
  text_style point_symbols_style_; //!< Used for data-point marking.
  text_style value_style_; //!< Used for data-point value-label.  (should be named value_label_style?)
  // Not finite values.
  plot_point_style nan_point_style_; //!< Default is cone pointing down for 2D, or NaN or point left or right for 1D.
  plot_point_style plus_inf_point_style_; //!< Default is cone pointing right for 2D, or NaN or point left or right for 1D.
  plot_point_style minus_inf_point_style_; //!< Default is cone pointing left for 2D, or NaN or point left or right for 1D.
  // Outside plot window values.
  plot_point_style plus_limit_point_style_; //!< Default is cone pointing up for 2D, or NaN or point left or right for 1D.
  plot_point_style minus_limit_point_style_; //!< Default is cone pointing down for 2D, or NaN or point left or right for 1D.

  value_style x_values_style_; //!< Used for data-point value marking.
  int x_value_label_rotation_; //!< Direction point value-labels is written (degree). Default horizonal (0).
  int x_value_precision_; //!< Decimal digits precision for X-axis value-labels. (if == 2, 1.2, 6 == 1.23456...)
  std::ios_base::fmtflags x_value_ioflags_; //!< std::iosflags used for Y-axis value-labels (default std::ios::dec).

  // text_elements hold position & alignment, and indirectly via text_style, font_family, font_size, bold, italic...
  text_element title_info_; //!< Title of whole plot.
  text_element legend_title_; //!< Title of legend-box header or title (if any).
  text_element x_label_info_; //!< X-axis label, Example: "length of widget"
  text_element x_units_info_; //!< X-axis units. For example, to display, "length (meter)".
  text_element x_ticks_value_label_info_; //!< X-axis tick-value-labels, for example: "1.2", "1.4" or "1.2e6".

  // No Y-axis info for 1D.

  box_style image_border_; //!< Style of rectangular border of all image width, color...
  box_style plot_window_border_; //!< Style of rectangular border of plot window width, color...(not the full image size).
  box_style legend_box_; //!< Style of rectangular box of legend width, color...

  // Plot window location coordinates (default) pixels) (calculate_plot_window() sets these values).
  double plot_left_; //!< SVG X coordinate (pixels) of left side of plot window.
  double plot_right_; //!< SVG X coordinate of right side of plot window.
  double plot_top_; //!< SVG Y coordinate of top side of plot window.
  double plot_bottom_; //!< SVG Y coordinate of bottom side of plot window.

  const double margin = 0.5; //!< Plot window margin to allow for rounding etc
  //!                             when checking if a point is inside window with @c is_in_window function.

  // enum legend_places{ where, aspect_ratio, inside...}
  bool is_legend_title_; //!< @c true if legend_title_.text() != "" (for example: @c .legend_title("My Legend");) (default @c false).

  legend_places legend_place_; //!< Place for any legend-box, inside, outside, left, right.
  double legend_width_; //!< Width of legend-box (pixels).
  double legend_height_; //!< Height of legend-box (in pixels).
  //!< Size of legend-box is controlled by its contents,
  //!< but helpful to store computed coordinates.
  double legend_left_; //!< Left of legend-box.
  double legend_top_; //!< Top of legend-box.
  // Both optionally set by legend_top_left.
  double legend_right_; //!< SVG Coordinates of right of legend-box,
  double legend_bottom_; //!< bottom of legend-box.
  double x_axis_vertical_; //!< Vertical position of 1D horizontal X-axis line as fraction of window.
  //! 0.5 is at middle(useful if no labels) (default),
  //! 0.8 is near bottom (useful if value-labels go upward),
  //! 0.2 is near top (useful if value-labels go downward).

  bool is_a_point_marker_; //! @c true if any data-series have point markers to show in legend (default @c false).
  bool is_a_data_series_line_;  //!< @c true if any series have lines to show in legend (default @c false). Example: @c .line_on(true).
  bool is_a_data_series_text_;  //!< @c true is any series should show text describing the data-series (default @c false). For example: @c my_plot.plot(my_data_0, "my_data_0_text");
  double legend_title_font_size_; //!< Font size of legend header/title.
  double legend_text_font_size_; //!< Font size of legend text.
 // double series_text_font_size_; //!< Font size of lines of text describing data-series.
  double legend_widest_line_; //!< Width of longest of legend header/title and widest data-series pointer+line+text.
  double biggest_point_marker_font_size_; //!< Biggest point marker symbol - determines vertical spacing.

  // Leave a vertical space before any text (if text_margin_ == 1.5 then height of one biggest font).
  // Leave a horizontal space before any text (if text_margin_ == 1.5 then width of one biggest font).
  // For example, if font size is 10 and text_margin is 1.5 and aspect ratio is 0.6 then
  // Legend_font_size_ = 10, text_margin = 1.5, aspect ratio =  0.6, Vertical_spacing = 15, horizontal_spacing = 9
  double vertical_title_spacing_; // = derived().legend_title_font_size_ * derived().text_margin_; // suits header text.
  double vertical_text_spacing_; // = derived().legend_title_font_size_; // One line vertically.
  double vertical_marker_spacing_; // = derived().biggest_point_marker_font_size_ * 0.8; // Spacing for biggest markers.
   double vertical_line_spacing_; // = derived().biggest_point_marker_font_size_ * 0.8; // Suits line spacing of markers, lines and text.

  double horizontal_title_spacing_; // = derived().legend_title_font_size_ * aspect_ratio; // legend_font width,
  double horizontal_line_spacing_; // = derived().legend_font_size_ * aspect_ratio; // legend_font width, line width, also used if no line to show in a series.
  double horizontal_marker_spacing_; // = derived().biggest_point_marker_font_size_ * 0.8 * aspect_ratio; // Width of biggest marker used if no marker on a series).

  axis_line_style x_axis_; //!< style of X axis line.
  axis_line_style y_axis_; //!< style of Y axis line. (Meaningless for 1D but added to permit shared code in axis_plot_frame.hpp!)

  ticks_labels_style x_ticks_; //!< style of X axis tick value-labels.
  ticks_labels_style y_ticks_; //!< style of Y axis tick value-labels. (Meaningless for 1D but added to permit shared code in axis_plot_frame.hpp!)

  bool title_on_; //!< If true include a title for the whole plot.
  bool legend_on_; //!< If true include a legend-box.
  bool outside_legend_on_; //!< If true, place legend-box outside the plot window.
  bool legend_lines_; //!< If true, include data colored line type in legend-box.
  bool plot_window_on_; //!< Use a separate plot window (not whole image).
  bool x_ticks_on_; //!< Ticks on X axis will be shown.
  bool x_values_on_; //!< values of data are shown by values markers.
  int  x_axis_position_; //!< \see boost::svg::x_axis_intersect.

  // Parameters for calculating confidence intervals (for both X and Y values).
  // These might be picked up from uncertain types.
  double alpha_; // = 0.05; // oss.iword(confidenceIndex) / 1.e6; //!< Pick up alpha or confidence, as a fraction.
  double epsilon_; // = 0.01; // = oss.iword(roundingLossIndex) / 1.e3; //!< Pick up rounding loss, as a fraction.
  int uncSigDigits_; // = 2; // = oss.iword(setUncSigDigitsIndex);  //<! Pick up significant digits for showing implied uncertainty.
  bool isNoisyDigit_; // = false; //!> Is an extra 'noisy' decimal digit shown. TODO Pick up?

  // Autoscaling
  bool autoscale_check_limits_; //!< If true, then check autoscale values for infinity, NaN, max, and min.
  bool x_autoscale_; //!< If true, use any autoscale values for scaling the X axis.
  double autoscale_plusminus_; //!< For uncertain values, allow for text_plusminus ellipses showing 67%, 95% and 99% confidence limits.\n
  //!< For example, if a max value is 1.2 +or- 0.02, then 1.4 will be used for autoscaling the maximum.\n
  //!< Similarly, if a min value is 1.2 +or- 0.02, then 1.0 will be used for autoscaling the minimum.
  double text_plusminus_; // Nominal factor of 1. (default) corresponds to 67% confidence limit.
  bool x_include_zero_; //!< If autoscaled, include zero.
  int  x_min_ticks_;  //!< If autoscaled, set a minimum number of ticks.
  double x_tight_; //!< How much a value can go beyond the tick value before another tick is required.
  int  x_steps_;  //!< If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc.

  // Values calculated by scale_axis, and is used only if x_autoscale == true.
  double x_auto_min_value_; //!< X minimum value calculated by autoscaling.
  double x_auto_max_value_;//!< X maximum value calculated by autoscaling.
  double x_auto_tick_interval_; //!< X axis tick major interval.
  int x_auto_ticks_; //!< Number of X axis ticks.

  // Similar y_auto value for 2-D
  bool y_autoscale_; //!< Always false for 1-D plot because Y axis is not autoscaled.

  std::string plot_window_clip_; //!< = "clip_plot_window" id for clippath
  //!< http://www.w3.org/TR/SVG/masking.html#ClipPathElement 14.1 Introduction
  //!< clipping paths, which uses any combination of 'path', 'text' and basic shapes
  //!< to serve as the outline. Everything on the "inside" of the outline
  //!< is allowed to show through but everything on the outside is masked out.
  //!< So the plot_window_clip_ limits display to a plot_window rectangle.

  std::vector<svg_1d_plot_series> serieses_;  //!< The (perhaps several) series of data-points for transformation.
  //!< These are sorted into two vectors for normal and abnormal (max, inf and NaN).

public:
// Function Declarations.
  svg_1d_plot(); // Constructor of a 1-D plot.
  void set_ids(); // Set the SVG XML ids for various layers of the plot.
  void calculate_plot_window(); // Calculate the position of the plot window.
  void calculate_transform(); // Calculate transform form Euclidian to SVG coordinate.
  void draw_axes(); // Draw the X-axis (and, for 2-D, also the Y-axis) of the plot.
  void update_image(); /* Calls functions to add all plot information to the image, including
  plot window, axes, ticks, labels, grids, legend, and finally all the data-series.*/
//! \endcond

  // ------------------------------------------------------------------------
  //! write() has two versions: to an @c std::ostream and to a file.
  //! The stream version first clears all unnecessary data from the graph,
  //! builds the document tree, and then calls the write function for the root
  //! document node, which calls all other nodes through the Visitor pattern.
  //! The file version opens an @c std::fstream, and calls the @c std::ostream version.
  // ------------------------------------------------------------------------
  svg_1d_plot& write(const std::string& file); //!< Write an SVG plot to a file.
  svg_1d_plot& write(std::ostream& os); //!> Write an SVG plot to a stream.

  // Declarations of several versions of function plot to add data-series (with defaults).
  template <typename T>
  svg_1d_plot_series& plot(const T& container, const std::string& title = "");
  template <typename T>
  svg_1d_plot_series& plot(const T& begin, const T& end, const std::string& title = "");

  template <typename T, typename U>
  svg_1d_plot_series& plot(const T& container, const std::string& title = "", U functor = boost::svg::detail::double_1d_convert());
  template <typename T, typename U>
  svg_1d_plot_series& plot(const T& begin, const T& end, const std::string& title = "", U functor = boost::svg::detail::double_1d_convert());
  // GCC seems to need functor to have trailing (), but MSVC does not.
}; // class svg_1d_plot

// End svg_1d_plot Member functions declarations.

// Definitions were in svg_1d_plot.ipp
//#ifndef BOOST_SVG_SVG_1D_PLOT_IPP
//#  include <boost/svg_plot/impl/svg_1d_plot.ipp>
//#endif
// But this caused b2 trouble, so svg_1d_plot.ipp is now appended below.

/*
  \brief Create 1D plots in Scalable Vector Graphic (SVG) format.
  \details Definitions of @c svg_1d_plot_series and @c svg_1d_plot member functions.
*/

/*!
  \tparam C An iterator into an STL container: @c array, @c std::vector<double>, @c std::vector<unc>, @c std::vector<Meas>, @c std::set, @c std::map ...
  \param begin Iterator to 1st element in container to show in the plot.
  \param end Iterator to last element in container to show in the plot.
  \param title Title of series of data values.
*/
template <typename C>
svg_1d_plot_series::svg_1d_plot_series(C begin, C end, const std::string& title)
: // Constructor.
title_(title), // of data-series.
point_style_(black, blank, 5, vertical_line, ""), // Default data-point marker style vertical line for 1D plots.
//limit_point_style_(lightgrey, whitesmoke, 10, cone_point_right, ""), // Default limit//  (inf or NaN) point style.  right-pointing pointer.
//limit_point_style_(red, green, 20, cone_point_right, ""), // Default limit (inf or NaN) point style is right-pointing pointer.
line_style_(black, blank, 2, false, false) // Default line style, black, no fill, width, line_on, bezier_on false
{
  /*
    std::cout << "typeid(C).name() = " << typeid(C).name() << std::endl;
    typeid(C).name() =
    class boost::transform_iterator
    <
      class boost::svg::detail::unc_1d_convert, class std::_Vector_const_iterator
      <
        class std::_Vector_val
        <
          class boost::svg::unc, class std::allocator
          <
            class boost::svg::unc
          >
        >
      >,
      struct boost::use_default, struct boost::use_default
    >
  */

  for(C i = begin; i != end; ++i)
  {  // Might be useful to have defaults for begin and end? But unclear how.
    //double temp = *i; // assume type is just double.
    //boost::svg::unc temp = *i; // Assumes unc type.
    //unc<false> temp = *i; // Should provide double, unc, meas ... type.
    // So use auto to automatically make temp the right type.
    auto temp = *i; // Should provide the right type: double, unc, meas ... type.
    //std::cout << "typeid(temp).name() = " << typeid(temp).name() << std::endl;
    //   typeid(temp).name() = class Meas
    //   typeid(temp).name() = class boost::svg::unc
    if(detail::is_limit(temp.value()))
    {
      series_limits_.push_back(temp.value()); // 'limit' values: too big, too small or NaN.
      // This assumes that uncertainty info is meaningless?
    }
    else
    {
      series_.push_back(temp); // Normal 'OK to plot' data values (including uncertainty info).
      // Should copy name and other info too if applicable.
    }
  }
} // svg_plot_series constructor.

// Definitions of svg_plot_series Member Functions to set up the plot colors, font, size, markers, lines etc.

svg_1d_plot_series& svg_1d_plot_series::fill_color(const svg_color& col_)
{ //! Set fill color for plot point marker(s).
  point_style_.fill_color_ = col_;
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

svg_color svg_1d_plot_series::fill_color()
{ //! Get fill color for plot point marker(s).
  return point_style_.fill_color_; //! \return Fill color for plot point marker(s).
}

svg_color svg_1d_plot_series::stroke_color()
{ //! Get stroke color for plot point marker(s).
  return point_style_.stroke_color_; //! \return Stroke color for plot point marker(s).
}

svg_1d_plot_series& svg_1d_plot_series::stroke_color(const svg_color& col_)
{ //! Set stroke color for plot point marker(s).
  point_style_.stroke_color_ = col_;
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::shape(point_shape shape_)
{ //! Set shape for plot point marker(s). Example: @c .shape(square), .shape(circlet)
  point_style_.shape_ = shape_;
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

point_shape svg_1d_plot_series::shape()
{ //! Get shape for plot point marker(s).
  return point_style_.shape_; //! \return Shape for plot point marker(s).
}

svg_1d_plot_series& svg_1d_plot_series::symbols(const std::string s)
{ //! Set symbol for plot point marker(s).
  point_style_.symbols_ = s;
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::size(int size_)
{ //! Set size of plot point marker(s).
  //point_style_.size_ = size_;
  //point_style_.symbols_style_.font_size(i); // in case using a symbol.
  point_style_.size(size_);
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

int svg_1d_plot_series::size()
{ //! \return  size of plot point marker(s).
  return point_style_.size();
}

const std::string svg_1d_plot_series::symbols()
{ //! \return  symbol for plot point marker(s).
  return point_style_.symbols_;
}

svg_1d_plot_series& svg_1d_plot_series::line_color(const svg_color& col_)
{ //! Set color of any line joining plot points.
  line_style_.stroke_color_ = col_;
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::line_width(double wid_)
{ /*! Set width of any line joining plot points.
   \return Reference to @c svg_1d_plot_series to make chainable.
   */
  line_style_.width_ = wid_;
  return *this; // Make chainable.
}

double svg_1d_plot_series::line_width()
{ //! \return  Width of any line joining plot points.
  return line_style_.width_;
}

svg_1d_plot_series& svg_1d_plot_series::line_on(bool on_)
{ /*! Set @c true if to draw a line joining plot points.
   \return Reference to @c svg_1d_plot_series to make chainable.
   */
  line_style_.line_on_ = on_;
  return *this; // \return Reference to @c svg_1d_plot_series to make chainable.
}

bool svg_1d_plot_series::line_on()
{ //! \return @c true if to draw a line joining plot points.
  return line_style_.bezier_on_;
}

svg_1d_plot_series& svg_1d_plot_series::bezier_on(bool on_)
{ //! Set @c true if to draw bezier curved line joining plot points.
  line_style_.bezier_on_ = on_;
  return *this; //! \return Reference to @c svg_1d_plot_series to make chainable.
}

bool svg_1d_plot_series::bezier_on()
{ //! \return @c true if to draw bezier curved line joining plot points.
  return line_style_.bezier_on_;
}

size_t svg_1d_plot_series::series_count()
{ //! \return Number of normal 'OK to plot' data values in data-series.
  return series_.size();
}

size_t svg_1d_plot_series::series_limits_count()
{ //!  \return Number of 'at limit' values: too big, too small or NaN data values in data-series.
  return series_limits_.size();  // TODO limit_count would be better name?
}

// Definitions of svg_plot member functions.

//! \cond DETAIL
// Do not document non-user functions.

void svg_1d_plot::update_image()
{ //! Calls functions to add all plot information to the image, including
  //! plot window, axes, ticks, labels, grids, legend, and finally all the data-series.

  clear_all(); // Removes all elements that will show up in a subsequent draw.

  // Draw plot background.
  image_.gs(detail::PLOT_BACKGROUND).push_back(
    new rect_element(0, 0, image_.x_size(),  image_.y_size()));

  calculate_plot_window();
  calculate_transform();
  draw_title(); // Call after above to the plot_x and y are defined.
  if(x_axis_.axis_line_on_)
  {
    draw_axes();
  }
  if(legend_on_)
  {
    draw_legend();
  }
  if(x_axis_.label_on_)
  {
    draw_x_axis_label();
  }
  double y(0.); // All 1-D points are plotted are on the horizontal X axis (y = 0) axis.
  transform_y(y);
  if ((y < plot_top_) || (y > plot_bottom_))
  { // So Y position being wrong should never happen! (error in transform?)
    throw std::runtime_error("transform_y(y=0) outside plot window!");
  }
  // Symbols are offset downwards because
  // the origin of the point is the top left of the glyph.
  // Need to offset by the height and width of the font size?
  y -= 3.; 

  for(unsigned int i = 0; i < serieses_.size(); ++i)
  { // Plot the normal data-points for each of the i data-series.
    g_element& g_ptr = image_.gs(detail::PLOT_DATA_POINTS).add_g_element();
    // Set the color for all the data-series markers.
    g_ptr.style().stroke_color(serieses_[i].point_style_.stroke_color_);
    g_ptr.style().fill_color(serieses_[i].point_style_.fill_color_);

    for(unsigned int j = 0; j < serieses_[i].series_.size(); ++j)
    { // Draw jth point for ith serieses.
      Meas ux = serieses_[i].series_[j];
      // unc<false> ux = serieses_[i].series_[j];
      double x = ux.value();

      transform_x(x);
      if((x >= plot_left_) && (x <= plot_right_)) // Check point is inside plot_window.
      // TODO May need a margin here to avoid points just over the window not being shown.
      {
        draw_plot_point(x, y, g_ptr, serieses_[i].point_style_, ux, Meas()); // Marker.
        // (y uncertainty is zero for 1d X values).
        // draw_plot_point(x, y, g_ptr, serieses_[i].point_style_, ux, unc<false>()); // Marker.
        // (y uncertainty is zero).
        if (x_values_on_)
        { // Show the value (& perhaps uncertainty) of the data-point too.
          g_element& g_ptr_v = image_.gs(detail::PLOT_X_POINT_VALUES).add_g_element();
          draw_plot_point_value(x, y, g_ptr_v, x_values_style_, serieses_[i].point_style_, ux);
          // TODO Might separate X and Y colors?
        }
        else
        { // Don't plot anything? Might leave a marker to show an "off the scale" value?
        }
      } // if in window
    } // for j
  } // for i all normal

  // Draw all the not-normal +/-infinity, NaN, or at_limit points.
  for(unsigned int i = 0; i < serieses_.size(); ++i)
  {
    g_element& g_ptr = image_.gs(detail::PLOT_LIMIT_POINTS);  // Limit points layer.

    y += 6; // Put the limit markers on the level of the X-axis line to avoid possible collision 
    // with any marker right at either end of the X-axis line.
    // This puts the NaN marker over the origin, but that's OK?

    for (unsigned int j = 0; j != serieses_[i].series_limits_.size(); ++j)
    {
      double x = serieses_[i].series_limits_[j];
      if (limit_NaN(x))
      { // is NaN rather than just too big or too small to show in the plot window.
        double x0 = 0.; // Avoid previous use of x.
        transform_x(x0);
        // If include zero, OK, else plot on left or right as appropriate.
        if (x0 < plot_left_)
        {
          x0 = plot_left_;
        }
        else if (x0 > plot_right_)
        {
          x0 = plot_right_;
        }
        // else X axis includes zero, so x0 is OK.
        g_ptr.style().stroke_color(nan_point_style_.stroke_color_);
        g_ptr.style().fill_color(nan_point_style_.fill_color_);
        draw_plot_point(x0, y, g_ptr, nan_point_style_, Meas(), Meas());
      }
      else
      { // Not NaN assume infinite.
        transform_x(x);
        // Avoid overwriting any data marker at either end of the horizontal line.
        // Can still overlap marker with half a font height?  But better if raise the data-points a bit more
        if (x < plot_left_)
        {
          x = plot_left_ - minus_inf_point_style_.size_ /2; // Just half a font size to left of left plot box.
          g_ptr.style().stroke_color(minus_inf_point_style_.stroke_color_);
          g_ptr.style().fill_color(minus_inf_point_style_.fill_color_);
          draw_plot_point(x, y, g_ptr, minus_inf_point_style_, Meas(), Meas()); // Draw the limit marker.
        }
        else if (x > plot_right_)
        {
          x = plot_right_ + plus_inf_point_style_.size_ / 2; // Just half a font size to right of right plot box.
          g_ptr.style().stroke_color(plus_inf_point_style_.stroke_color_);
          g_ptr.style().fill_color(plus_inf_point_style_.fill_color_);
          draw_plot_point(x, y, g_ptr, plus_inf_point_style_, Meas(), Meas()); // Draw the limit marker.
        }
        // else is inside plot window, so draw a limit point marker TODO.
      }
    } // for j limits point
  } // for i data_series
} //   void update_image()

  /*! Default constructor.
      Many (but not all - see below) default values here.
      See documentation for default settings rationale.
  */
  svg_1d_plot::svg_1d_plot() :

    title_style_(18, "Verdana", "", ""),  // last "bold" ?
    legend_text_style_(10, "Verdana", "", ""), // 2nd "italic"?
    x_axis_label_style_(10, "Verdana", "", ""),
    x_ticks_value_label_style_(10, "Verdana", "", ""),
    point_symbols_style_(10, "Lucida Sans Unicode"), // Used for data-point marking.
    value_style_(10, "Verdana", "", ""), // Used for data-point value-labels.

    nan_point_style_(green, white, 20, cone_point_down, ""), // Colors and size for NaN markers.
    plus_inf_point_style_(red, white, 10, cone_point_right, ""), // Colors and size for +infinity markers.
    minus_inf_point_style_(blue, white, 10, cone_point_left, ""), // Colors and size for -infinity markers.

    plus_limit_point_style_(red, white, 20, cone_point_up, ""), // Colors and size for outside window markers.
    minus_limit_point_style_(blue, white, 20, cone_point_down, ""), // Colors and size for outside window markers.

    title_info_(0, 0, "", title_style_, align_style::center_align, horizontal),
    //title_info_(0, 0, "Plot of data", title_style_, center_align, horizontal), when text concatenation solved?
    //x_label_info_(0, 0, "X Axis", x_axis_label_style_, center_align, horizontal),
    //x_units_info_(0, 0, " (units)", x_ticks_value_label_style_, center_align, horizontal),
    x_label_info_(0, 0, "", x_axis_label_style_, align_style::center_align, horizontal), // Null strings for now.
    x_ticks_value_label_info_(0, 0, "", x_ticks_value_label_style_, align_style::center_align, horizontal), // X-axis tick value-label, for example: "1.2" or "1.2e1".
    x_units_info_(0, 0, "", x_ticks_value_label_style_, align_style::center_align, horizontal),
    x_axis_(X, -10., +10., black, 1, 0, true, false, true),
    y_axis_(Y, 0., +1., black, 1, 0, false, false, false), // Not used for 1D.

    // Might fill in all values, but there are rather many for ticks_labels_style,
    x_ticks_(X, x_ticks_value_label_style_),// so for defaults see ticks_labels_style.

    // Should allow a 'fraction font space' above and below the text for any descenders.
    // so real height including any descenders is font_height * 1.25
    text_margin_(1.25), // for title and axis label text, allowing 25% extra for any descenders.
    // as a multiplier of the biggest EM box font size of legend title and any marker symbols,
    // used, for example:
    //   derived().vertical_title_spacing_ = derived().legend_text_font_size_ * derived().text_margin_; 
    // But does not allow for any space between lines, so may need explicit space.

    image_border_(yellow, white, 1, 10, true, true), // margin should be about axis label font size.
    plot_window_border_(lightgoldenrodyellow, svg_color(255, 255, 255), 1, 3, true, false),
    legend_box_(yellow, white, 1, 1, true, true),
    legend_title_(0, 0, "", legend_text_style_, align_style::center_align, static_cast<int>(horizontal)),
    legend_width_(200), // width of legend-box (pixels) // TODO isn't this calculated?
    legend_height_(0), // height of legend-box (pixels)
    legend_left_(-1), legend_right_(-1),legend_top_(-1),legend_bottom_(-1), // Default top left of plot window.
    x_axis_vertical_(0.5), // Vertical position of 1D horizontal X-axis line as fraction of window.
    legend_place_(outside_right), // default but interacts with using plot_window.
    legend_on_(false),
    legend_widest_line_(0), //!< Longest width (on X-axis) of sum of point marker, line and data-series text and legend title.
    // (used to calculate how wide the legend-box needs to be, and thus position of it and plot window).

    vertical_title_spacing_(0), // Legend header/title vertical spacing.
     //  derived().vertical_title_spacing_ = derived().legend_title_font_size_ * derived().text_margin_;
    vertical_text_spacing_(0), // = derived().legend_font_size_; // One line vertically.
    vertical_marker_spacing_(0), // = derived().biggest_point_font_size_ ; // Suits line spacing of markers, lines and text.
    vertical_line_spacing_(0), // = max(derived(). vertical_marker_spacing_ and vertical_text_spacing_); // Suits data_series line spacing of markers/lines/text.

    horizontal_title_spacing_(0), // = derived().legend_font_size_ * aspect_ratio; // legend_font width, used as a font .
    horizontal_line_spacing_(0), // = derived().legend_font_size_ * aspect_ratio; // legend_font width, line width, also used if no line to show in a series.
    horizontal_marker_spacing_(0), // = derived().biggest_point_font_size_ * 0.8 * aspect_ratio; // Width of biggest marker used if no marker on a series).

    outside_legend_on_(true),
    legend_lines_(false), // Not very useful for 1 D as already showing data-point marker shapes.
    plot_window_clip_("plot_window"), // for <clipPath id="plot_window" ...
    title_on_(true),
    plot_window_on_(true),
    x_values_on_(false), // By default, don't label point values.
    x_values_style_(static_cast<int>(horizontal), align_style::left_align, 3, std::ios::dec, true, value_style_, black, black, false, false),
    // Confidence interval parameters.
    // (Could provide functions for the user to control these).
    alpha_(0.05), // oss.iword(confidenceIndex) / 1.e6; 95% confidence.
    epsilon_(0.05), // = oss.iword(roundingLossIndex) / 1.e3; // = allow 5% rounding loss.
    uncSigDigits_(2), // = oss.iword(setUncSigDigitsIndex); // ISO standard =2 by default.
    isNoisyDigit_(false), // Add extra digit to display?
    // Autoscaling.
    autoscale_check_limits_(true), // Do check all value for limits, infinity, max, min, NaN.
    autoscale_plusminus_(3.), // Allow 3 uncertainty (standard deviation) for 99% confidence ellipse.
    text_plusminus_(1.), // Nominal factor of 1. (default) corresponds to 67% confidence limit.
    x_autoscale_(false),
    x_include_zero_(false), // If autoscaled, include zero.
    x_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
    x_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
    x_tight_(1e-6), // margin that point can lie outside top and bottom tick.
    y_autoscale_(false), // Not used for 1-D.
    // Used to transform Cartesian to SVG.
    x_scale_(1.), x_shift_(0.),
    y_scale_(1.), y_shift_(0.),
    x_axis_position_(0) // Move into axis_style?
  {
    size(500, 200); // Default image size.
  // Only needs to be quite shallow (y_size) for a 1-D plot.
  // (But may need more height (y_size) if long value-labels are used, for example: "[time = 1.23 +-0.02 sec]").
  // 200 barely leaves enough room for five data-series in any legend-box).
  // (2-D usually needs to be much more rectangular).

  using namespace boost::svg::detail; // Avoid detail:: specification.

  // Build the document tree & add all the children of the root node.
  for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
  {
    image_.add_g_element();
  }
  set_ids();

  // Set other SVG color, stroke & width defaults for various child PLOT nodes.
  image_.gs(PLOT_BACKGROUND).style().stroke_color(image_border_.stroke_);
  image_.gs(PLOT_BACKGROUND).style().stroke_width(image_border_.border_width_); //
  image_.gs(PLOT_BACKGROUND).style().fill_color(image_border_.fill_);
  image_.gs(PLOT_WINDOW_BACKGROUND).style().fill_color(plot_window_border_.fill_);
  image_.gs(PLOT_WINDOW_BACKGROUND).style().stroke_width(plot_window_border_.border_width_).stroke_color(plot_window_border_.stroke_);
  image_.gs(PLOT_LIMIT_POINTS).style().stroke_color(lightslategray).fill_color(antiquewhite);
  image_.gs(PLOT_X_AXIS).style().stroke_color(black).stroke_width(x_axis_.width());
  image_.gs(PLOT_DATA_UNC3).style().stroke_color(lightgoldenrodyellow).fill_color(lightgoldenrodyellow).stroke_width(1);
  image_.gs(PLOT_DATA_UNC2).style().stroke_color(peachpuff).fill_color(peachpuff).stroke_width(1);
  image_.gs(PLOT_DATA_UNC1).style().stroke_color(magenta).fill_color(pink).stroke_width(1);

  // Note that widths are stored in member data *and* copied here.
  // Not sure if this is wise but ...
  // Font info defaults are set by the constructor above.

  // Ticks.
  y_ticks_.left_ticks_on_ = false; // Needed to ensure don't extend X-axis line.
  y_ticks_.right_ticks_on_ = false;

  if(x_ticks_.use_up_ticks() || x_ticks_.use_down_ticks())
  {
    image_.gs(PLOT_X_MAJOR_TICKS).style().stroke_width(x_ticks_.major_tick_width_).stroke_color(black);
    image_.gs(PLOT_X_MINOR_TICKS).style().stroke_width(x_ticks_.minor_tick_width_).stroke_color(black);
  }
  // Grids.
  // Default color & width for grid, used or not.
  image_.gs(PLOT_X_MAJOR_GRID).style().stroke_width(x_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
  image_.gs(PLOT_X_MINOR_GRID).style().stroke_width(x_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
  //image_.gs(PLOT_DATA_LINES).style().stroke_width(2); // default width.
  // Alter with, for example: plot.data_lines_width(4);

  legend_place_ = (plot_window_on_) ? outside_right : inside; // Defaults.
  // Note if set plot_window_on() then also need to set legend_place.
  // (if set a default in plot_window then call to set legend place must to come *after* it.
  // So not set a default?  But leaving it as inside is worse?)

  x_ticks_on_ = x_ticks_.up_ticks_on_ || x_ticks_.down_ticks_on_;
  // Only 2D has left and right y ticks.
  x_ticks_.ticks_on_window_or_on_axis_ = 0; // Make ticks (and ticks value-labels) on X-axis line the default.
  // This will place the labels just under the horizontal X-axis line,
  // rather than below the plot window border.
  // This over-rides the default in class ticks_labels_style.

  if (title_info_.text() == "")
  { // Avoid leaving unnecessary space etc for a title.
    title_on_ = false;
  }
  else
  {
    title_on_ = true; // Can be switched off later with `my_1d_plot.title_on(true);`
  }
} // svg_1d_plot::svg_1d_plot() Default constructor.

void svg_1d_plot::calculate_plot_window()
{ //! Calculate the size and position of the plot window,
  //! taking account of the length and font size of axes labels, axis ticks, title and legend-box.
  //! This version is only for 1-D.
  //! All calculations use SVG units, pixels by default.

  // Start by assuming we can use all the svg image,
  // but reduce by the width of any image border.
  plot_left_ = 0 + image_border_width(); // Top left of image.
  plot_top_ = 0 + image_border_width();
  plot_right_ = image_.x_size() - image_border_width(); // Bottom right of image.
  plot_bottom_ = image_.y_size() - image_border_width();

  // Leave one char space each side for any limit markers showing +/-infinity and/or NaN.
  // (half this might do?)
  plot_left_ += minus_inf_point_style_.size_;
  plot_right_ -= plus_inf_point_style_.size_;

  if(title_on_ && title_info_.text() != "")
  { // Leave space at top for title.
    // Title at bottom (or sides even) option not implemented.
    plot_top_ += title_font_size() * (text_margin_ + 0.5);
  }

  // Assume that X-axis labels are always at bottom for 1D plot.
  if(x_axis_.label_on_ == true && x_label_info_.text() != "")
  { // Leave space below plot window at bottom for X axis label (unless empty string).
      plot_bottom_ -= x_axis_label_style_.font_size() * text_margin_;
  }
  if(plot_window_on_ == true)
  { // Needed to allow any plot window border rectangle to show OK.
    // A small margin is to prevent it overlapping the image border.
    // Also allows for axis value-labels that mark the min and max
    // that must extend about half a font width beyond the plot window border.
    plot_left_ +=  image_border_.margin_;
    plot_right_ -=  image_border_.margin_;
    plot_top_ += image_border_.margin_;
    plot_bottom_ -=  image_border_.margin_;
  }

  // Size if necessary - else (re-)initialise to zero.
  size_legend_box(); // depending on its contents.
  place_legend_box();

  // Because there may be several datasets,
  // and the scaling can be done by any one or all of them.
  // my_plot.autoscale(my_data1) // for 1 dataset.
  // or my_plot.autoscale(my_datas) // for a vector of several datasets.
  // calculates the min & max, increments & ticks.
  if (x_autoscale_)
  { // Use calculated autoscale values.
    // Autoscale has been done in my_data.autoscale(my_data);
    // and saved in x_auto_min_value_, x_auto_max_value_, & x_auto_tick_interval_,
    // so copy these values to use them:
    x_axis_.min_ = x_auto_min_value_;
    x_axis_.max_ = x_auto_max_value_;
    x_ticks_.major_interval_  = x_auto_tick_interval_;
  }
  else
  { // Ignore auto values, even if they have been calculated.
  }
  // Copy X-axis min & max to ticks.
  x_ticks_.min_ = x_axis_.min_;
  x_ticks_.max_ = x_axis_.max_;
  // Ensure both axis and ticks have the *same* range.
  // (To use them separately (designed to give the potential for different ranges)
  // one would have to *not* do this,
  // but to make sure they are both assigned correctly).

  //x_axis_.axis_ = (plot_bottom_) * x_axis_vertical_ + plot_top_ ;  // too low
  //x_axis_.axis_ = (plot_bottom_) * x_axis_vertical_ - plot_top_ ;  // too high
  //x_axis_.axis_ = (plot_bottom_) * x_axis_vertical_;  // tiny amount too  high.
  // x_axis_.axis_ = (plot_bottom_ + plot_top_) * x_axis_vertical_;
    x_axis_.axis_ = (plot_bottom_ - plot_top_) * x_axis_vertical_ + plot_top_;
  // Put X-axis fraction of way up plot window.
  // 0.5 is at middle(useful if no labels),
  // 0.8 is near bottom (useful if value-labels go upward),
  // 0.2 is near top (useful if value-labels go downward).

  if(plot_window_on_ == true)
  { // Using a plot window and NOT using all image.
    // Calculate the number of chars of the longest tick value-label.
    x_ticks_.longest_label(); // Updates label_max_length_
    x_ticks_.label_max_space_ = 0; // Work out the longest tick value-label for X-Axis.
    if (x_ticks_.label_rotation_ == static_cast<int>(horizontal))
    { // Only 1 char height & 1 space needed if labels are horizontal.
      x_ticks_.label_max_space_ = 2 * x_ticks_value_label_style_.font_size() * aspect_ratio; // SVG chars.
      // Should this be just 2 * font_size
    }
    else if ((x_ticks_.label_rotation_ == static_cast<int>(upward)) 
      || (x_ticks_.label_rotation_ == static_cast<int>(downward)))
    { // ! horizontal so will need more than 2 chars worth.
        x_ticks_.label_max_space_+= x_ticks_.label_max_length_ * x_ticks_value_label_style_.font_size() * aspect_ratio; // SVG chars.
    }
    else
    { // Assume label is sloping, say 45, so * sin(45) = 0.707.
        x_ticks_.label_max_space_+= x_ticks_.label_max_length_ * x_ticks_value_label_style_.font_size() * aspect_ratio * sin45; // SVG 'chars'.
    }

    // Make space for any ticks pointing below the plot window.
    if(x_ticks_.down_ticks_on_)
    { // Start bottom of plot higher to give space for any external down ticks.
      plot_bottom_ -= (std::max)(x_ticks_.major_tick_length_, x_ticks_.minor_tick_length_);// And avoid macro max trap!
    }

    if (x_axis_.axis_line_on_)
    { // Want a X-axis line, so check if range includes zero, meaning that X and Y axes intersect,
      // and x_axis_ is svg coordinate of Y-axis (usually y = 0).
      // If not fix axis to bottom of the plot window.
      if ((x_axis_position_ == bottom) // All X data values definitely > zero.
        && !(x_ticks_.ticks_on_window_or_on_axis_ < 0) ) // & not already on bottom of plot window.
      { // y_min_ > 0 so X-axis will not intersect Y-axis, so use plot window border.
        plot_bottom_ -= x_ticks_.label_max_space_; // Move up for the value-labels.
        x_axis_.axis_ = plot_bottom_; // Put X-axis on bottom on plot window.
      }
      else if ((x_axis_position_ == top)  // All x data values definitely < zero.
        && !(x_ticks_.ticks_on_window_or_on_axis_ > 0) ) // & not already on top of plot window.
      { // // y_max_ < 0 so X-axis will not intersect Y-axis, so use plot window border.
          plot_top_ += x_ticks_.label_max_space_; // Move down for labels.
          x_axis_.axis_ = plot_top_; // Put X-axis on top of plot window border.
      }
      else
      { // y_axis_position_ == y_intersects_x
        // Calculate below after transform is calculated.
      }
    } // if (use_x_axis_line_)

    // Make space for any tick value-labels.
    if (x_ticks_.major_value_labels_side_ != 0)
    { // There are some tick value-labels.
      // If ticks and value-labels are on plot window border, the need to allow space for them.
      if ((x_ticks_.ticks_on_window_or_on_axis_ < 0) // ticks on bottom of plot window.
          && (x_ticks_.major_value_labels_side_ < 0) ) // & tick value-labels below axis line too.
      {  // Contract plot window bottom edge up to make space for X tick value-labels on bottom.
        plot_bottom_ -= x_ticks_.label_max_space_; // Move up.
      }
      else if ((x_ticks_.ticks_on_window_or_on_axis_ > 0) //
          && (x_ticks_.major_value_labels_side_ > 0) ) // & X tick value-labels to top of plot window.
      { // Move top of plot window down to give space for X tick value-labels.
        plot_top_ += x_ticks_.label_max_space_; // Move window top down.
      }
      else // (x_ticks_.major_value_labels_side_ == 0)
      { // X-ticks on the X-axis line (not on plot window border).
        // Don't need to allow any extra space.
        // (But if X-axis is near plot window border, may overlap it, and any X-axis labels!)
      }
    } // x_ticks_. major_value_labels_side
  } // plot_window_on_
  if(plot_window_on_ == true)
  { // Draw plot window border as a rectangular box.
    image_.gs(detail::PLOT_WINDOW_BACKGROUND).push_back(
      new rect_element(plot_left_, plot_top_, (plot_right_ - plot_left_), plot_bottom_ - plot_top_));
  } // plot_window_on_
} //  void calculate_plot_window()

void svg_1d_plot::draw_axes()
{ //! Add information to the plot image for X-axis lines.
  //! (For 1-D, there is, of course, only the horizontal X-axis,
  //! but there can be a vertical Y-axis line at x = 0).
  double x(0.);
  transform_x(x);
  double y1(0.);
  double y2(image_.y_size());
  // Draw origin, making sure it is in the plot window.
  if(x_axis_.axis_line_on_ && (x >= plot_left_) && (x <= plot_right_))
  {
    if(!plot_window_on_)
    { // Use whole image.
      if(title_on_)
      { // Allow space for title, taking account of font size.
        y1 += title_info_.textstyle().font_size() * text_margin_;
      }
      if(x_axis_.label_on_)
      {// Allow space for x tick values, taking account of font size.
        y2 -= x_label_info_.textstyle().font_size() * text_margin_;
      }
    }
    else
    { // Use plot window.
      y1 = plot_top_;
      y2 = plot_bottom_;
    }
    image_.gs(detail::PLOT_X_AXIS).line(x, y1, x, y2); // Draw vertical Y-axis line.
  }
  else
  { // Won't fit into window.  Throw or warning message?
  }
  draw_x_axis(); // Draw horizontal X-axis line.
} //  draw_axes()

void svg_1d_plot::set_ids()
{ //! Document ids for use in identifying group elements, for example: <g id = "PLOT_TITLE".../g>
  for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
  {
    svg_1d_plot::image_.gs(i).id(detail::document_ids_[i]);
  }
} //  void set_ids()

void  svg_1d_plot::calculate_transform()
{ //! Calculate scale and shift factors for transforming from Cartesian to SVG plot.
  //! SVG image is (0, 0) at top left, Cartesian (0, 0) at bottom left.
  x_scale_ = (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_);
  x_shift_ = plot_left_ - (x_axis_.min_ * (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_));
  y_scale_ = 1.;
  // y_shift_ = plot_top_ - (plot_top_ - plot_bottom_) / 2.;
  // y_shift_ = plot_top_ - (plot_top_ - plot_bottom_)  * x_axis_vertical_;  // OK at 0.5
  y_shift_ = plot_top_ - (plot_top_ - plot_bottom_) * x_axis_vertical_;
} // void calculate_transform()

//! \endcond

// User svg_1d_plot Member functions definitions.
/////////////////////////////////////////////////

/*! Write SVG image to the specified file, providing the suffix @c .svg if no suffix given.

  \details @c write() has two versions: to an @c ostream and to a file.
  The stream version first clears all unnecessary data from the graph,
  builds the document tree, and then calls the @c write function for the root
  document node, which calls all other nodes through the Visitor pattern.

  \param file Filename to write.
  \note This file version opens an @c ostream, and calls the @c ostream version of write.

  \return @c *this to make chainable.
 */
svg_1d_plot& svg_1d_plot::write(const std::string& file)
{
  std::string filename(file); // Copy to avoid problems with const if need to append.
  if (filename.find(".svg") == std::string::npos)
  { // No file type suffix, so provide the default .svg.
    filename.append(".svg");
  }
  std::ofstream fout(filename.c_str());
  if(fout.fail())
  {
    throw std::runtime_error("Failed to open file " + filename + "!\n");
  }
  image_.image_filename(filename);
  // Note filename for optional output as comment in the .svg file.
  svg_1d_plot::write(fout); // Use the ostream version.

  if (fout.rdstate() != std::ios_base::goodbit)
  {
    std::cout << "Writing to file " << filename << " failed!" << std::endl;
  }
  fout.close();
  if (fout.rdstate() != std::ios_base::goodbit)
  {
    std::cout << "Closing file " << filename << " failed!" << std::endl;
  }
  else
  {
    std::cout << "Plot written to file " << filename << "." << std::endl;
  }
  return *this;
}

svg_1d_plot& svg_1d_plot::write(std::ostream& os)
{ /*! Write SVG image to the specified @c std::ostream.
    \param os  @c std::ostream to write out the plot.

    \note This function also is used by the write to file function.
    \return @c *this to make chainable.
  */
  update_image();
  /*!
    The default stream precision of 6 decimal digits is probably excessive for plots.
    4.1 Basic data types, integer or float in decimal or scientific (using E format).
    If image size is under 1000 x 1000, the SVG plot default precision of 3 is probably sufficient.
    This reduces .svg file sizes significantly for curves represented with many data-points.
    For example, if a curve is shown using 100 points,
    reducing to coord_precision(3) from default of 6 will reduce file size by 300 bytes.
  */
  image_.write(os);

  return (svg_1d_plot&) *this;
} // write


 /*! Add a data-series to the plot (by default, converting to @c unc @c doubles), with optional data-series title.

  \tparam T Floating-point type of the data (@c T must be convertible to @c double).

  \param container Container (for example std::vector) for the data to be added to the plot.
  \param title Optional title for the data-series (default none).

  \return Reference to data-series just added (to make chainable).

  \note This version assumes that \b ALL the data values in the container are used.
  */
  //! @b Example:
/*!
  \code
    std::vector<float> my_data; // my container.
    my_data.push_back(2.f); // Fill container with some data.
    my_data.push_back(3.f);
    my_data.push_back(4.f);
    my_1d_plot.plot(my_data, "All data in my container"); // Plot all data in container.
  \endcode
*/
template <typename T>
svg_1d_plot_series& svg_1d_plot::plot(const T& container, const std::string& title /*= "" */)
{
  serieses_.push_back(
    svg_1d_plot_series(
    boost::make_transform_iterator(container.begin(), detail::meas_1d_convert()),
    boost::make_transform_iterator(container.end(), detail::meas_1d_convert()),
    //boost::make_transform_iterator(container.begin(), detail::unc_1d_convert<false>()),
    //boost::make_transform_iterator(container.end(), detail::unc_1d_convert<false>()),
    title)
  );
  return serieses_[serieses_.size() - 1];
} // plot(const T& container, const std::string& title)

 /*! Add a data-series to the plot (by default, converting to unc doubles), with optional title.
    \tparam T floating-point type of the data (T must be convertible to double).
    \param begin Iterator to 1st data item in container.
    \param end Iterator to one-beyond-end of data in container.
    \param title Optional title for the plot (default is no title).
    \return Reference to the data-series just added.
    \note This version permits a @b partial range of the container, from begin to end, to be used.

    Example:
    \code
my_1d_plot.plot(my_data.begin(), my_data.end(), "My container"); // Whole container of data values.
my_1d_plot.plot(&my_data[1], &my_data[4], "my_data 1 to 4"); // Add part of data-series
    \endcode

    \warning last == end  which is one past the last, so this only does 1, 2 and 3 -  \b not 4!
  */
  template <class T>  // \tparam T floating-point type of the data (T must be convertible to double).
svg_1d_plot_series& svg_1d_plot::plot(const T& begin, const T& end, const std::string& title)
{
  serieses_.push_back(
    svg_1d_plot_series(
    boost::make_transform_iterator(begin, detail::unc_1d_convert<false>()),
    boost::make_transform_iterator(end, detail::unc_1d_convert<false>()),
    title)
  );
  /*!
    */
  return serieses_[serieses_.size() - 1]; // Reference to data-series just added.
} // plot

/*!
  \brief Add a data-series in a container to the plot, with optional title.

  \tparam T floating-point type of the data (which must be convertible to @c double).
  \tparam U functor floating-point type (default is @c double_1d_convert).
  \param container Container for data (for example @c std::vector) that contains the data to be added to the plot.
  \param title Optional title for the plot (default is no title).
  \param functor Custom functor to convert data value to @c double.
  \return a reference to data-series just added (to make chainable).
  \note This version of function @c plot includes a functor, allowing other types to just convert data values to @c double (the default).
*/

template <typename T, typename U>
svg_1d_plot_series& svg_1d_plot::plot(const T& container, const std::string& title /* = "" */, U functor /*= double_1d_convert*/)
{
  serieses_.push_back(
    svg_1d_plot_series(
    boost::make_transform_iterator(container.begin(), functor),
    boost::make_transform_iterator(container.end(),   functor),
    title)
  );
  return serieses_[serieses_.size() - 1]; //  Reference to data-series just added to make chainable.
} // plot

/*!
   \brief Add a data-series to the plot, with optional title. (Version with custom functor, rather than to @c double).
    \tparam T Floating-point type of the data (which must be convertible to @c double).
    \tparam U Functor floating-point type (default is @c double_1d_convert).
    \param begin Iterator to 1st data item in container.
    \param end Iterator to one-beyond-end of data in container.
    \param title Optional title for the plot (default is no title).
    \param functor Custom functor.
    \return a reference to data-series just added (to make chainable).
    \note This version permits a @b partial range of the container (within begin to end) to be used.
*/
template <typename T, typename U>
svg_1d_plot_series& svg_1d_plot::plot(const T& begin, const T& end, const std::string& title /* = ""*/,
   U functor /* = double_1d_convert */)
{
  serieses_.push_back(
    svg_1d_plot_series(
      boost::make_transform_iterator(begin, functor),
      boost::make_transform_iterator(end,   functor),
      title)
  );
  return serieses_[serieses_.size() - 1];
} // plot

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_1D_PLOT_HPP
