/*!
  \file
  \brief Create 2D XY plots in Scalable Vector Graphic (SVG) format.
  \details Provides @c svg_2d_plot data and function to create plots,
  and @c svg_2d_plot_series to allow data values to be added.

  Very many functions allow fine control of the appearance and
  layout of plots, data markers and lines.\n

  (Many items common to 1D and 2D use functions and classes in @c axis_plot_frame).
  #define BOOST_SVG_DIAGNOSTICS can be added to output diagnostic information.

  \author Jacob Voytko & Paul A. Bristow
 */

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009, 2012, 2013, 2014, 2016

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_2D_PLOT_HPP
#define BOOST_SVG_SVG_2D_PLOT_HPP

#ifdef _MSC_VER
#pragma warning(push)
#  pragma warning (disable : 4800) // Forcing value to bool 'true' or 'false' (performance warning).
#  pragma warning (disable : 4512) // Assignment operator could not be generated.
#endif

#include <boost/iterator/transform_iterator.hpp>
//  using boost::make_transform_iterator;

#include <boost/quan/unc.hpp> // Values with uncertainty.
#include <boost/quan/meas.hpp> // Values with timestamp and uncertainty.

#include <boost/svg_plot/svg.hpp>
#include <boost/svg_plot/svg_style.hpp>
#include <boost/svg_plot/detail/axis_plot_frame.hpp> // Code shared with 2D.
#include <boost/svg_plot/detail/functors.hpp>
// using boost::svg::detail::unc_2d_convert;
#include <boost/svg_plot/detail/numeric_limits_handling.hpp>
#include <boost/svg_plot/detail/functors.hpp>
#include <boost/svg_plot/detail/auto_axes.hpp>

#include <map> // for map & multimap
#include <string>
#include <sstream>
#include <utility>
#include <ostream>
#include <iterator>
#include <exception>
#include <iostream> // for debug.
//using std::cout;
//using std::endl;

namespace boost
{
  namespace svg
  {
//! \endcond

    // Forward declarations.
//! \cond DETAIL
    const std::string strip_e0s(std::string s); // Strip unnecessary zeros and e and sign.
//! \endcond
    class svg_2d_plot; // 2D Plot framework.
    class svg_2d_plot_series; // One series of data to plot.

 /*!  \class boost::svg::svg_2d_plot_series
      \brief Holds a series of 2D data values (points) to be plotted.

      \details Data values are sorted into normal and 'at limits':
        NaN, infinity or too small or too large.\n\n
       Member functions allow control of data points markers and lines joining them,
       and their appearance, shape, color and size.\n
       Data points can include their value,
       and optionally uncertainty and optionally number of degrees of freedom.\n

       Each data series can have a title that can be shown on a legend box with identifying symbols.

        @c std::multimap is used rather than @c std::vector of @c std::pair s because
        @c std::multimap sorts and ensures that lines joining data points
        are unaffected by the order in which data is presented.
       (For 1-D a vector of doubles can be used).
   */

   
   class svg_2d_plot_series
    {

     void draw_straight_lines(const svg_2d_plot_series& series);

    friend void draw_straight_lines(const svg_2d_plot_series&);
    public:
//! \cond DETAIL
      // 2-D Data series points to plot.
      std::multimap<Meas, unc<false> > series_; //!< Normal 'OK to plot' data values.
    //     std::multimap<unc<false>, unc<false>> series_; //!< Normal 'OK to plot' data values.
      std::multimap<double, double> series_limits_; //!< 'limit' values: too big or small, or NaN.

      std::string title_; //!< Title of data series (to show on legend using legend style).
      plot_point_style point_style_; //!< Data point marker like circle, square...
      plot_point_style limit_point_style_; //!< At limit data point marker. Default is cone pointing down.
      plot_line_style line_style_; //!< Style (color, width...) of line joining data points.
      bar_style bar_style_; //!< Style of bar used in histograms.
      histogram_style histogram_style_; //!< Style of histogram.
//! \endcond

      /*
        Constructor for a data series to plot
        \tparam T an STL container: for example: @c multimap.
        \param begin Starting iterator into container of data series begin() to start at the beginning.
        \param end Ending iterator into container of data series, end() to finish with the last item.
        \param title Title for the plot.
       */
       template <typename T>
       svg_2d_plot_series(
        T begin,
        T end,
        std::string title = "");
      // Function declarations only - definitions below).
      // Set functions for the plot series.
      svg_2d_plot_series& fill_color(const svg_color& col_);
      svg_2d_plot_series& stroke_color(const svg_color& col_);
      svg_2d_plot_series& shape(point_shape shape_);
      svg_2d_plot_series& size(int size_);
      svg_2d_plot_series& line_color(const svg_color& col_);
      svg_2d_plot_series& area_fill(const svg_color& col_);
      svg_2d_plot_series& line_width(double wid_);
      svg_2d_plot_series& line_on(bool on_);
      svg_2d_plot_series& bezier_on(bool on_);
      svg_2d_plot_series& bar_opt(bar_option);
      svg_2d_plot_series& bar_width(double wid_);
      svg_2d_plot_series& bar_color(const svg_color& col);
      svg_2d_plot_series& bar_area_fill(const svg_color& col);
      svg_2d_plot_series& histogram(histogram_option opt_);

      // Declarations of Get functions for the plot series.
      plot_line_style line_style();
      double line_width();
      bool bezier_on();
      bool line_on();
      svg_color& line_color();
      svg_color& area_fill();
      int size();
      point_shape shape();
      bar_option bar_opt();
      double bar_width();
      svg_color& bar_color();
      svg_color& bar_area_fill();
      int values_count(); // Number of normal values in data series.
      int limits_count(); // Number of values 'at limit' in data series.

   }; // class svg_2d_plot_series

   // svg_2d_plot_series Member Functions Definitions.

   // svg_2d_plot_series constructor.
      /*!
        Constructor for a data series to plot
        \tparam T an STL container: for example: @c multimap.
        \param begin Starting iterator into container of data series begin() to start at the beginning.
        \param end Ending iterator into container of data series, end() to finish with the last item.
        \param title Title for the plot.
       */

  template <typename T>
  svg_2d_plot_series::svg_2d_plot_series(
      T begin, // \param Begin of data series.
      T end, // \param End of data series.
      std::string title /*  \param @c std::string title Title of data series.
      */
    )
    :
    title_(title), //!< Title of a series of data values.
    // plot_point_style(const svg_color& fill = blank, const svg_color& stroke = black,
    // int size = 5, point_shape shape = circlet, const std::string& symbols = "X")
    point_style_(black, white, 5, circlet), // Default point style (default fill white).
    limit_point_style_(grey, blank, 10, cone), // Default limit (infinity or NaN) point style.
    line_style_(black, blank, 2, false, false), // Default line style, no fill, width 2, no line_on, no bezier.
    bar_style_(black, blank, 3, no_bar), // Default black, no fill, stick width 3, no bar.
    // -2 block to Y-axis, -1 stick to Y-axis, no_bar,  +1 stick to x_axis, -2 block to X-axis.
    histogram_style_(no_histogram)
  { // Constructor.
    for(T i = begin; i != end; ++i)
    { // Sort data points into normal and 'at-limit' series.
      std::pair<Meas, unc<false> > temp = *i;
      // Only handle uncoordinated uncertainties case unc<false>
      //std::pair<unc<false>, unc<false>> temp = *i;
      Meas ux = temp.first;
      unc<false> uy = temp.second;
      std::pair<double, double> xy = std::make_pair<double, double>(ux.value(), uy.value());
      if(detail::pair_is_limit(xy))
      { // Either x and/or y is 'at-limit'.
        series_limits_.insert(xy);
      }
      else
      { // Normal data values for both x and y.
        series_.insert(temp);
      }
    }
  } // svg_2d_plot_series

  // Definitions of svg_2d_plot_series member functions.

  //svg_2d_plot_series & boost::svg::svg_2d_plot_series::fill_color(const svg_color &col_)
  svg_2d_plot_series& svg_2d_plot_series::fill_color(const svg_color& col_)
  { //! Set data series point marker fill color.
    point_style_.fill_color_ = col_;
    return *this;
  }

   svg_2d_plot_series& svg_2d_plot_series::stroke_color(const svg_color& col_)
  { //! Set Data series point marker stroke color.
    point_style_.stroke_color_ = col_;
    return *this;
  }

  svg_2d_plot_series& svg_2d_plot_series::shape(point_shape shape_)
  { //! Set Data series point marker shape.
    point_style_.shape_ = shape_;
    return *this;
  }

  svg_2d_plot_series& svg_2d_plot_series::size(int size_)
  { //! Set Data series point marker size.
    point_style_.size_ = size_;
    return *this;
  }

  svg_2d_plot_series& svg_2d_plot_series::line_color(const svg_color& col_)
  { //! Set Data series line color.
    line_style_.stroke_color_ = col_;
    return *this;
  }

  svg_2d_plot_series& svg_2d_plot_series::area_fill(const svg_color& col_)
  { //! Set Data series area fill color.
    line_style_.area_fill_ = col_;
    /*! \note @c area_fill(false) will produce a @b blank color, and so NO FILL.

    @c area_fill(blank) will produce the default non-blank color (black?).
    */
    return *this;
  }

  svg_2d_plot_series& svg_2d_plot_series::line_width(double wid_)
  { //! Set data series line width.
    line_style_.width_ = wid_; //! (Sets legend line width too).
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  svg_2d_plot_series& svg_2d_plot_series::line_on(bool on_)
  { //! Set @c true to draw line linking data points.
    line_style_.line_on_ = on_;
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  svg_2d_plot_series& svg_2d_plot_series::bezier_on(bool on_)
  { //! Set @c true to draw bezier curved line linking data points.
    line_style_.bezier_on_ = on_;
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  svg_2d_plot_series& svg_2d_plot_series::bar_opt(bar_option opt_)
  { //! Set bar options.
    bar_style_.bar_option_ = opt_;
    return *this;  //! \return Reference to svg_2d_plot_series to make chainable.
  }

  svg_2d_plot_series& svg_2d_plot_series::bar_width(double wid_)
  { //! Set Bar width.
    bar_style_.width_ = wid_;
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  svg_2d_plot_series& svg_2d_plot_series::bar_color(const svg_color& col)
  {//! Set bar color.
    bar_style_.color_ = col;
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  svg_2d_plot_series& svg_2d_plot_series::bar_area_fill(const svg_color& col)
  { //! \param col Set bar area fill color.
    bar_style_.area_fill_ = col;
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  plot_line_style svg_2d_plot_series::line_style()
  { //! \return Line style for line joining data points.
    return line_style_;
  }

  double svg_2d_plot_series::line_width()
  { //! \return Width of line joining data points.
    return line_style_.width_;
  }

  bool svg_2d_plot_series::bezier_on()
  { //! \return @c true if line joing data points should be a bezier curve.
    return svg_2d_plot_series::line_style_.bezier_on_;
  }

  bool svg_2d_plot_series::line_on()
  { //! \return @c true if a line is to join data points.
    return svg_2d_plot_series::line_style_.line_on_;
  }

 svg_color& svg_2d_plot_series::line_color()
  { //! \return color of a line to join data points.
    return line_style_.stroke_color_;
  }

  svg_color& svg_2d_plot_series::area_fill()
  { //! \return Color for any area fill below line(s) joining data points.
    return line_style_.area_fill_;
  }

  int svg_2d_plot_series::size()
  { //! \return Size of data point marker(s).
    return point_style_.size_;
  }

  point_shape svg_2d_plot_series::shape()
  { //! \return Shape of data point marker(s).
    return point_style_.shape_;
  }

  bar_option svg_2d_plot_series::bar_opt()
  { //! \return Bar options.
    return bar_style_.bar_option_;
  }

  double svg_2d_plot_series::bar_width()
  { //! \return Bar width.
    return bar_style_.width_;
  }

  svg_color& svg_2d_plot_series::bar_color()
  {  //! \return Bar color.
    return bar_style_.color_;
  }

  svg_color& svg_2d_plot_series::bar_area_fill()
  { //! \return Color of bar area fill.
    return bar_style_.area_fill_;
  }

  svg_2d_plot_series& svg_2d_plot_series::histogram(histogram_option opt_)
  { /*!
      \param opt_ no_histogram = 0,
      \param opt_ bar = +1 // Stick or column line (stroke width) vertical to X-axis.
    */
    histogram_style_.histogram_option_ = opt_;
    return *this; //! \return Reference to svg_2d_plot_series to make chainable.
  }

  int svg_2d_plot_series::values_count()
  { //! \return number of normal values in a data series.
    // Assume can never have more than max_int values in the data series.
    // Or could return size_t?
    return static_cast<int>(series_.size());
  }

  int svg_2d_plot_series::limits_count()
  {  //! \return number of values 'at limit' in a data series.
    return static_cast<int>(series_limits_.size());
  }

  // End svg_2d_plot_series Member Functions Definitions.

  //  class svg_2d_plot definitions.
  /*!
    \brief Provides svg_2d_plot data and member functions to create plots.\n
       Very many functions allow very fine control of the
       appearance and layout of plots, data markers and lines.
    \sa svg_2d_plot_series that allows data values to be added.
    \sa svg_1d_plot.hpp for 1-D version.
    \details
       svg_2d_plot allows us to store plot state locally in svg_plot.\n
       (We don't store it in svg because transforming the points after they are
       written to the document would be difficult. We store the Cartesian
       coordinates locally and transform them before we write them).\n
       (svg_2d_plot inherits from axis_plot_frame.hpp containing functions common to 1 and 2-D).
   */
  class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
  {
     friend void show_2d_plot_settings(svg_2d_plot&);
     friend class svg_2d_plot_series;
     friend class detail::axis_plot_frame<svg_2d_plot>;
     // axis_plot_frame.hpp contains functions common to 1 and 2-D.

   // private:
  public:

  //! \cond DETAIL
      // Member data names conventionally end with _,
      // for example: border_margin_,
      // and corresponding set & get accessor functions are named without _ suffix,
      // for example: border_margin() & border_margin(int).

      double x_scale_; //!< scale factor used by transform() to go from Cartesian to SVG coordinates.
      double x_shift_; //!< shift factor used by transform() to go from Cartesian to SVG coordinates.
      double y_scale_; //!< scale factor used by transform() to go from Cartesian to SVG coordinates.
      double y_shift_; //!< shift factor used by transform() to go from Cartesian to SVG coordinates.

      svg image_; //!< Stored so as to avoid rewriting style information constantly.

      double text_margin_; //!< Marginal space around text items like title, text_margin_ * font_size to get distance in svg units.

      text_style a_style_; //!< Defaults for text_style (contains font size & type etc).
      text_style title_style_; //!< Style for plot title.
      text_style legend_style_; //!< Style for legend title and text.
      text_style x_axis_label_style_; //!< Style for tick labels on X-axis.
      text_style x_value_label_style_; //!< Style for data point value labels on X-axis.
      text_style y_axis_label_style_; //!< Style for tick labels on Y-axis.
      text_style y_value_label_style_;//!< Style for data point value labels on Y-axis.
      text_style point_symbols_style_; //!< Style used for symbol marking a data point.

      text_element title_info_; //!< Plot title text.
      text_element legend_header_; //!< Legend box header or title (if any).
      text_element x_label_info_; //!< X-axis label text, for example: "length".
      text_element x_value_label_info_; //!< X-axis tick value text, for example: "1.2" or "1.2e+001"
      text_element y_label_info_; //!< Y-axis label text, for example: "volume".
      text_element x_units_info_; //!< X-axis units, for example: "mm".
      text_element y_units_info_; //!<  Y-axis units, for example: "min". (2-D only).
      text_element y_value_label_info_; //!< Y-axis tick value text, for example: "1.2" or "1.2e+001"

      text_style value_style_; //!< Style used for data point value label.
      value_style x_values_style_; //!< Data point X value marking, font, size etc.
      value_style y_values_style_; //!< Data point Y value marking, font, size etc.
      bool x_plusminus_on_; //!<  http://en.wikipedia.org/wiki/Plus-minus_sign Unicode \&\#0xB1; HTML \&plusmn;

      rotate_style y_value_label_rotation_; //!< Direction point Y value labels written (default horizontal).
      int y_value_precision_; //!< std::ios precision used for Y value labels (default 3).
      std::ios_base::fmtflags y_value_ioflags_; //!< std::iosflags used for Y vale labels (default ios::dec).

      box_style image_border_; //!< rectangular border box style of all image width, color...
      box_style plot_window_border_; //!< rectangular border box style of plot window width, color...
      box_style legend_box_; //!< rectangular box style of legend width, color...

      // TODO doubles also could be float?
      double plot_left_; //!< SVG X coordinate (pixels) of left side of plot window.
      double plot_right_; //!< SVG X coordinate of right side of plot window.
      double plot_top_; //!< SVG Y coordinate of top side of plot window.
      double plot_bottom_; //!< SVG Y coordinate of bottom side of plot window.

      const double margin = 0.5; //!< Plot window margin to allow to rounding etc 
      //! when checking if a point is inside window with @c is_in_window function.

      // enum legend_places{ nowhere, inside...}
      legend_places legend_place_; //!< Place for any legend box.
      double legend_width_; //!< Width of legend box (pixels).
      double legend_height_; //!< Height of legend box (in pixels).
      // Size of legend box is controlled by its contents,
      // but helpful to store computed coordinates.
      double legend_left_; //!< Left of legend box.
      double legend_top_; //!< Top of legend box.
      // Both optionally set by legend_top_left.
      double legend_right_; //!< SVG Coordinates of right of legend box,
      double legend_bottom_; //!< SVG Coordinates of bottom of legend box.
      size_t legend_longest_; //!< longest (both header & data) string in legend box.
      // (used to calculate how big the legend box needs to be, and thus position of it and plot window).

      axis_line_style x_axis_; //!< Style of X-axis.
      axis_line_style y_axis_; //!< Style of Y-axis.

      ticks_labels_style x_ticks_; //!< Style of X-axis tick marks and labels.
      ticks_labels_style y_ticks_; //!< Style of Y-axis tick marks and labels.

      bool title_on_; //!< true if to display a title for the whole plot.
      bool legend_on_; //!< true if to provide a legend box.
      bool outside_legend_on_; //!< true if legend box should be outside the plot window.
      bool legend_lines_; //!< true if to add a colored line for each data series in legend box.
      bool plot_window_on_; //!< true if to use a separate plot window (not the whole image).
      bool x_ticks_on_; //!< true if X-axis to have ticks.
      bool y_ticks_on_; //!< true if Y-axis to have ticks.
      bool x_values_on_; //!< true if values of X data are shown (as 1.23).
      bool y_values_on_; //!< true if values of Y data are shown (as 3.45).
      bool xy_values_on_; //!< true if values of X & Y pairs are shown (as 1.23, 3.43).

      int x_axis_position_; //!< Intersection with Y-axis, or not.
      int y_axis_position_; //!< Intersection with X-axis, or not.

      // Parameters for calculating confidence intervals (for both X and Y values).
      // These might be picked up from uncertain types.
      double alpha_; //!< = 0.05; // oss.iword(confidenceIndex) / 1.e6; // Pick up alpha.
      double epsilon_; // = 0.01; // = oss.iword(roundingLossIndex) / 1.e3; // Pick up rounding loss.
      int uncSigDigits_; // = 2; // = oss.iword(setUncSigDigitsIndex);  // Pick up significant digits for uncertainty.
      bool isNoisyDigit_; // = false; // Pick up?

      bool autoscale_check_limits_; //!< true if to check autoscale values for infinity, NaN, max, min.
      bool x_autoscale_; //!< true if to use any X-axis autoscale values.
      double autoscale_plusminus_; //!< For uncertain values, allow for text_plusminus ellipses showing 67%, 95% and 99% confidence limits.\n
      //!< For example, if a max value is 1.2 +or- 0.02, then 1.4 will be used for autoscaling the maximum.\n
      //!< Similarly, if a min value is 1.2 +or- 0.02, then 1.0 will be used for autoscaling the minimum.

      double text_plusminus_; // Nominal factor of 1. (default) corresponds to 67% confidence limit.

      bool x_include_zero_; //!< true if autoscaled, to include zero.
      int  x_min_ticks_;  //!< If autoscaled, set a minimum number of X ticks.
      double x_tight_; //!< Tolerance used by autoscale to avoid extra ticks.
      int  x_steps_;  //!< true if autoscale to set any prescaling to multiple of decimal 1, 2, 5, 10 etc.

      double x_auto_min_value_; //!< Values calculated by scale_axis (used only if x_autoscale == true).
      double x_auto_max_value_; //!< Values calculated by scale_axis (used only if x_autoscale == true).

      double x_auto_tick_interval_; //!< X tick major interval.
      int x_auto_ticks_; //!< Number of X ticks.

      bool y_autoscale_; //!< @c true if to use any y_axis autoscale values.
      bool y_include_zero_; //!< @c true if autoscale to include zero.
      int  y_min_ticks_;  //!< If autoscaled, set a minimum number of Y ticks.
      double y_tight_; //!< Tolerance used by autoscale to avoid extra ticks.
      int  y_steps_;  //!< If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc.

      double y_auto_min_value_; //!< Values calculated by scale_axis (used only if y_autoscale == true).
      double y_auto_max_value_; //!< Values calculated by scale_axis (used only if y_autoscale == true).
      double y_auto_tick_interval_; //!< tick major interval (calculated by Y autoscale).
      int y_auto_ticks_; //!< Number of ticks (calculated by Y autoscale).

      std::vector<svg_2d_plot_series> serieses_; //!< Store of several series of data points for transformation.
      std::vector<text_element> notes_; //!< Store of text for annotation.  (Not used yet?)

      std::string plot_window_clip_; /*!< = "clip_plot_window" id for clippath
        http://www.w3.org/TR/SVG/masking.html#ClipPathElement 14.1 Introduction clipping paths,\n
        which uses any combination of 'path', 'text' and basic shapes
        to serve as the outline where everything on the "inside" of the outline
        is allowed to show through but everything on the outside is masked out.
        So the plot_window_clip_ limits display to a plot_window rectangle.
      */
//! \endcond

    public:
      /*! \brief Default constructor inheriting from public @c axis_plot_frame<svg_2d_plot>,
        providing all the very many default plot options,
        some of which use some or all of the style defaults.\n
        All these settings can be changed by these chainable functions.\n @b Example:

        \code
svg_2d_plot my_plot;
my_plot.background_color(ghostwhite) // Whole image.
  .legend_border_color(yellow) // Just the legend box.
  .legend_background_color(lightyellow)  // Fill color of the legend box.
  .plot_background_color(svg_color(white)) // Just the plot window
  .plot_border_color(svg_color(green)) // The border rectangle color.
  .plot_border_width(1) // Thin border (SVG units, default pixels).
  .title_color(red) // Title of whole image.
;
        \endcode
        \n
        \sa Rationale for default plot options and style settings in documentation.
      */

      svg_2d_plot()
        :
        // See documentation for default settings rationale.
        // text_styles:
        title_style_(18, default_font, "", ""),  // last "bold" ?
        legend_style_(14, default_font, "", ""), // 2nd "italic"?
        x_axis_label_style_(14, default_font, "", ""),
        x_value_label_style_(12, default_font, "", ""), // X-axis tick labels.
        // Separate X and Y to allow axes to have different styles.
        y_axis_label_style_(14, default_font, "", ""),
        y_value_label_style_(12, default_font, "", ""), // Y-axis tick labels.
        point_symbols_style_(12, "Lucida Sans Unicode"), // Used for data point marking.
        title_info_(0, 0, "", title_style_, center_align, horizontal),
        x_label_info_(0, 0, "", x_axis_label_style_, center_align, horizontal),
        x_units_info_(0, 0, "", x_value_label_style_, center_align, horizontal),
        x_value_label_info_(0, 0, "", x_value_label_style_, center_align, horizontal),
        x_axis_(X, -10., +10., black, 1, 0, true, false, true),
        y_axis_(Y, -10., +10., black, 1, 0, true, false, true),
        // Might fill in all values, but there are rather many for ticks_labels_style,
        // And this would separate any changes in styles defaults from plot default.  TODO?
        x_ticks_(X, x_value_label_style_),// so for other defaults see ticks_labels_style.
        y_ticks_(Y, y_value_label_style_),
        y_label_info_(0, 0, "", y_axis_label_style_, center_align, upward),
        y_units_info_(0, 0, "", y_axis_label_style_, center_align, upward),
        y_value_label_info_(0, 0, "", y_value_label_style_, center_align, upward), //
        text_margin_(2.), // for axis label text, as a multiplier of the font size.
        // Should allow a 'half line space' above and below the label text.
        image_border_(yellow, white, 2, 3, true, true),
        // margin (parameter 4) needs to be at least the width of the border (parameter 3) to ensure any border color shows.
        // margin should be about axis tick label font size to
        // allow for axis value labels that mark the min and max
        // that must extend about half a font width beyond the plot window border.
        // This is set dynamically in calculate_plot_window because user can change tick value label font size.

        plot_window_border_(lightslategray, svg_color(255, 255, 255), 2, 3, true, false),
        legend_box_(yellow, white, 1, 2, true, true),
        legend_header_(0, 0, "", legend_style_, center_align, horizontal),
        legend_width_(200), // width of legend box (pixels) // TODO isn't this calculated?
        legend_height_(0), // height of legend box (pixels)
        legend_left_(-1), legend_right_(-1), legend_top_(-1), legend_bottom_(-1), // Default top left of plot window.
        legend_place_(outside_right), // default but interacts with using plot_window.
        legend_on_(false),
        legend_longest_(0),
        outside_legend_on_(true),
        plot_window_clip_("plot_window"), // for <clipPath id="plot_window" ...
        title_on_(true),
        plot_window_on_(true),
        // Can have either or both X and Y value shown.
        x_values_on_(false), // If X values of data points are shown.
        y_values_on_(false), // If Y values of data points are shown.
        xy_values_on_(false), // If X & Y values of data are shown as a pair.
        x_values_style_(horizontal, 3, std::ios::dec, true, value_style_, black, black, false, false),
        y_values_style_(downward, 3, std::ios::dec, true, value_style_, black, black, false, false),

        text_plusminus_(1.),
        // Confidence interval parameters.
        // (Could provide functions for the user to control these).
        alpha_(0.05), //!<  alpha for calculating confidence interval, default 0.05 for 95%.
        // oss.iword(confidenceIndex) / 1.e6; 95% confidence.
        epsilon_(0.05), // = oss.iword(roundingLossIndex) / 1.e3; // = allow 5% rounding loss.
        uncSigDigits_(2), // = oss.iword(setUncSigDigitsIndex); // ISO standard =2 by default.
        isNoisyDigit_(false), // Add extra digit to display?

        // Autoscaling defaults.
        autoscale_check_limits_(true), // Do check all value for limits, infinity, max, min, NaN.
        autoscale_plusminus_(3.), // Allow 3 uncertainty (standard deviation) for 99% confidence ellipse.

        x_autoscale_(false),
        x_include_zero_(false), // If autoscaled, include zero on X-axis.
        x_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
        x_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
        x_tight_(1e-6), // margin that point can lie outside top and bottom tick.

        y_autoscale_(false),
        y_include_zero_(false), // If autoscaled, include zero on Y-axis.
        y_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
        y_steps_(0),  //!< If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
        y_tight_(1e-6), //!< margin that point can lie outside top and bottom tick without triggering another interval and tick .

        // Used to transform Cartesian to SVG.
        x_scale_(1.), x_shift_(0.),
        y_scale_(1.), y_shift_(0.),
        x_axis_position_(0),
        y_axis_position_(0)

      {
        size(500, 400); // Default image size for 2-D.
        // 2-D usually needs to be squarer than 1-D.

        using namespace boost::svg::detail; // Avoid need for prefix detail::

        // Build the document tree by adding all children of the root node.
        for(int i = 0; i < SVG_PLOT_DOC_CHILDREN; ++i)
        {
          image_.add_g_element();
        }
        set_ids();

        // Set other SVG color, stroke & width defaults for various child PLOT nodes.

        image_.g(PLOT_BACKGROUND).style().fill_color(image_border_.fill_);
        image_.g(PLOT_BACKGROUND).style().stroke_color(image_border_.stroke_);
        image_.g(PLOT_BACKGROUND).style().stroke_width(image_border_.width_); //
        image_.g(PLOT_WINDOW_BACKGROUND).style().fill_color(plot_window_border_.fill_);
        image_.g(PLOT_WINDOW_BACKGROUND).style().stroke_width(plot_window_border_.width_).stroke_color(plot_window_border_.stroke_);
        image_.g(PLOT_LIMIT_POINTS).style().stroke_color(lightslategray).fill_color(antiquewhite);
        image_.g(PLOT_X_AXIS).style().stroke_color(black).stroke_width(x_axis_.width());
        image_.g(PLOT_Y_AXIS).style().stroke_color(black).stroke_width(y_axis_.width());

        image_.g(PLOT_DATA_UNC3).style().stroke_color(blank).fill_color(lightgoldenrodyellow).stroke_width(1);
        image_.g(PLOT_DATA_UNC2).style().stroke_color(peachpuff).fill_color(peachpuff).stroke_width(1);
        image_.g(PLOT_DATA_UNC1).style().stroke_color(pink).fill_color(pink).stroke_width(1);

        //image_.g(detail::PLOT_NOTES).style().fill_color(black);

        // Note that widths are stored in member data *and* copied here.
        // Not sure if this is wise but ...

        // Ticks
        if(x_ticks_.use_up_ticks() || x_ticks_.use_down_ticks())
        {
          image_.g(PLOT_X_MAJOR_TICKS).style().stroke_width(x_ticks_.major_tick_width_).stroke_color(black);
          image_.g(PLOT_X_MINOR_TICKS).style().stroke_width(x_ticks_.minor_tick_width_).stroke_color(black);
        }
        if(y_ticks_.left_ticks_on_ || y_ticks_.right_ticks_on_)
        {
          image_.g(PLOT_Y_MAJOR_TICKS).style().stroke_width(y_ticks_.major_tick_width_).stroke_color(black);
          image_.g(PLOT_Y_MINOR_TICKS).style().stroke_width(y_ticks_.minor_tick_width_).stroke_color(black);
        }
        // Grids.
        // Default color & width for grid, used or not.
        // Might avoid empty grid stuff if this was only done if grid used?  TODO
        image_.g(PLOT_X_MAJOR_GRID).style().stroke_width(x_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
        image_.g(PLOT_X_MINOR_GRID).style().stroke_width(x_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
        image_.g(PLOT_Y_MAJOR_GRID).style().stroke_width(y_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
        image_.g(PLOT_Y_MINOR_GRID).style().stroke_width(y_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
        image_.g(PLOT_DATA_LINES).style().stroke_width(2); // default width.
        // Alter with plot.data_lines_width(4);

        legend_place_ = (plot_window_on_) ? outside_right : inside; // Defaults.
        // Note if set plot_window_on() then also need to set legend_place.
        // (if set a default in plot_window then call to set legend place must to come *after* it.
        // So not set a default?  But leaving it as inside is worse?)

        x_ticks_on_ = x_ticks_.up_ticks_on_ || x_ticks_.down_ticks_on_;
        y_ticks_on_ = y_ticks_.left_ticks_on_ || y_ticks_.right_ticks_on_;

        if (title_info_.text() == "")
        { // Avoid leaving unnecessary space etc for a title.
          title_on_ = false;
        }
        else
        { // Assume if title is provided, then want to use it.
          title_on_ = true; // Can be switched off later with `my_1d_plot.title_on(false);`
        }

        if(x_label_info_.text() != "")
        { // No label provided, so avoid making space for it.
          x_axis_.label_on_ = false;
        }
        else
        { // Assume if label is provided, then want to use it.
          x_axis_.label_on_ = true; // Can be switched off later with `my_1d_plot.label_on(false);
        }

        if(y_label_info_.text() != "")
        { // No label provided, so avoid making space for it.
          y_axis_.label_on_ = false;
        }
        else
        { // Assume if label is provided, then want to use it.
          y_axis_.label_on_ = true; // Can be switched off later with `my_1d_plot.label_on(false);
        }
      } // svg_2d_plot() default constructor.

    //private:
    public: // Temporary for gil experiments.

      // svg_2d_plot Implementation Member Functions.

//! \cond DETAIL
      void set_ids()  // This only used once in constructor and should be inlined.
      { //! document ids for use in <g id = "PLOT_TITLE".../>
        for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
        { // Order determines the painting order.
          image_.g(i).id(detail::document_ids_[i]);
        }
      } //  void set_ids()

      void transform_pair(std::pair<double, double>& pt)
      { //! Transform both x and y from Cartesian to SVG coordinates.
        //! SVG image is 0, 0 at top left, Cartesian at bottom left.
        // scale and shift are set in calculate_plot_window().
        transform_point(pt.first, pt.second);
      }
      void calculate_plot_window()
      { //! The plot window is used to set a clip path:
        //! this ensures that data points and lines (and anything else)
        //! outside this window are NOT drawn.

        //! All calculation use svg units, pixels by default.

        // Start by assuming we can use all the svg image,
        // but reduce by the width of any image border.
        plot_left_ = 0 + image_border_width(); // Top left of image.
        plot_top_ = 0 + image_border_width();
        plot_right_ = image_.x_size() - image_border_width(); // Bottom right of image.
        plot_bottom_ = image_.y_size() - image_border_width();

        if(title_on_)
        { // Leave space at top for title.
          // TODO what if want to put title at bottom?
          plot_top_ += title_font_size() * (text_margin_ + 0.5);
        }

        // Deal with muddle where text_style is stored in two places
        // by copying to ensure they are the same.
        // TODO Sort this out properly by a major class reorganisation!
        x_axis_label_style_ = x_label_info_.textstyle();
        if (x_label_info_.textstyle() != x_axis_label_style_ )
        {
          std::cout << "x_label_info_.textstyle() != x_axis_label_style_" << std::endl;
        }

        y_axis_label_style_ = y_label_info_.textstyle();
        if (y_label_info_.textstyle() != y_axis_label_style_ )
        {
          std::cout << "y_label_info_.textstyle() != y_axis_label_style_!" << std::endl;
        }

        // Assume that X-axis labels are always at bottom.
        if(x_axis_.label_on_ == true == true && x_label_info_.text() != "")
        { // Leave space at bottom for X-axis label.
          if (x_label_info_.textstyle().font_size() != x_axis_label_style_.font_size())
          { // Temporary check.
            std::cout << "x_label_info_.textstyle().font_size() "<< x_label_info_.textstyle().font_size() << std::endl;
            std::cout << "x_axis_label_style_.font_size() " << x_axis_label_style_.font_size() << std::endl;
          }

          plot_bottom_ -= x_axis_label_style_.font_size() * text_margin_;
          // plot_bottom_ -= x_label_info_.textstyle().font_size() * text_margin_; // OK
        }
        // Assume that Y-axis labels are always at left.
        if(y_axis_.label_on_ == true == true && y_label_info_.text() != "")
        { // Leave space at left for Y-axis label.
          if (y_label_info_.textstyle().font_size() != y_axis_label_style_.font_size())
          { // Temporary check.
            std::cout << "y_label_info_.textstyle().font_size() "<< y_label_info_.textstyle().font_size() << std::endl;
            std::cout << "y_axis_label_style_.font_size() " << y_axis_label_style_.font_size() << std::endl;
          }
          plot_left_ += y_axis_label_style_.font_size() * text_margin_;
        }

        if(plot_window_on_)
        {
          // A margin is needed to allow any plot window border rectangle to show OK.
          // A minimum small margin (default 3) is to prevent it overlapping the image border.
          // Might be == image_border size?
          // Also must allow for axis value labels that mark the min and max ticks
          // that might extend beyond the plot window border, 
          // but must extend not beyond image or will be cutoff in mid-value_label.
          // if writing is vertical need only half a font, but half x_ticks_.label_max_space_ if horizontal.
          // x_ticks_.label_max_space_ calculated this - but is not calculated yet!
          // So just allow a few chars.

          // x-axis margin adjustment.

          //std::cout << "x font size " << x_ticks_values_font_size() << std::endl;
          double x_value_space;
            // Really want
            // value_space = x_ticks_.label_max_space_ /2.; // Assume value label is center aligned.
          if (x_ticks_.label_rotation_ != horizontal)
          { // vertical-ish x value labels just need half a font.
            x_value_space = x_ticks_values_font_size() / 2; // half a single font width.
          }
          else
          { // horizontal(?-ish) so need space for half the label, assumed 4 chars.
           // x_value_space = x_value_label_style_.font_size() * 2 ; // few font widths.
            x_value_space = x_ticks_values_font_size() * 2 ; // few font widths.
          }
          //std::cout << "x value_font_space " << x_value_space << std::endl;
          double border_margin = (std::max)(image_border_.margin_, x_value_space);
          //std::cout << "x left-right border_margin = " << border_margin << std::endl;
          //std::cout << "  plot_left before margin " << plot_left_ << std::endl;
          plot_left_ += border_margin;
         // std::cout << "  plot_left after margin " << plot_left_ << std::endl;
          //std::cout << "  plot_right before margin " << plot_right_ << std::endl;
          plot_right_ -= border_margin;
          //std::cout << "  plot_right after margin " << plot_right_ << "\n"<< std::endl;

          // y-axis top bottom margin adjustment to avoid collisions with title or off image.
          //std::cout << "y font size " << y_ticks_values_font_size() << std::endl;
          double y_value_space;
          if ((y_ticks_.label_rotation_ == downward) | (y_ticks_.label_rotation_ == upward))
          { // vertical y axis value labels need space for half the label, assumed 4 chars.
            y_value_space = y_ticks_values_font_size() * 2; // a few font widths.
          }
          else
          { // horizontal(-ish?) axis label so need space for just half the font width.
            y_value_space = y_ticks_values_font_size() / 2; // half a single font width.
          }
          //std::cout << "y value_font_space " << y_value_space << std::endl;
          border_margin = (std::max)(image_border_.margin_, y_value_space);
          //std::cout << "y top-bottom border_margin = " << border_margin << std::endl;
          //std::cout << "  plot_top before margin " << plot_top_ << std::endl;
          plot_top_ += border_margin;
         // std::cout << "  plot_top after margin " << plot_top_ << std::endl;
         // std::cout << "  plot_bottom before margin " << plot_bottom_ << std::endl;
          plot_bottom_ -= border_margin;
         // std::cout << "  plot_bottom after margin " << plot_bottom_ << std::endl;
        }
        // Why not check if legend wanted first?
        size_legend_box(); // Size depends on its contents.
        place_legend_box(); // according to options chosen.

        // Because there may be several datasets,
        // and the scaling can be done by anyone or all of them.
        // my_plot.x_autoscale(my_data1) // for 1 dataset.
        // or my_plot.x_autoscale(my_datas) // for a vector of several datasets.
        // calculates the min & max, increments & ticks.
        if (x_autoscale_)
        { // Use calculated autoscale values.
          // Autoscale has been done in my_data.x_autoscale(my_data);
          // and saved in x_auto_min_value_, x_auto_max_value_, & x_auto_tick_interval_,
          // so copy these values to use them:
          x_axis_.min_ = x_auto_min_value_;
          x_axis_.max_ = x_auto_max_value_;
          x_ticks_.major_interval_  = x_auto_tick_interval_;
          // else ignore auto values, even if have been calculated.
        }

        if (y_autoscale_)
        { // Use calculated autoscale values.
          // Autoscale has been done in my_data.y_autoscale(my_data);
          // and saved in y_auto_min_value_, y_auto_max_value_, & y_auto_tick_interval_,
          // so copy these values to use them:
          y_axis_.min_ = y_auto_min_value_;
          y_axis_.max_ = y_auto_max_value_;
          y_ticks_.major_interval_  = y_auto_tick_interval_;
          // else ignore auto values, even if have been calculated.
        }

        // Check if the axes will intersect.
        // X-axis position is determined by the range of Y min and max label values.
        x_axis_position_ = x_intersects_y;  // Assume X-axis will intersect Y-axis (range of Y values includes zero).
        if (y_axis_.min_ > (std::numeric_limits<double>::min)()) // All Y values definitely > zero.
        { // y_min_ > 0, so X-axis will not intersect Y-axis, so put X-axis line on bottom plot window.
          x_axis_position_ = bottom; // X-axis to bottom.
          x_ticks_.ticks_on_window_or_on_axis_ = -1; //  X-axis ticks to bottom of plot window.
        }
        else if(y_axis_.max_ < -(std::numeric_limits<double>::min)())  // all Y values definitely < zero.
        { // // y_max_ < 0, so X-axis will not intersect Y-axis, so put X-axis line on top plot window.
          x_axis_position_ = top; // X-axis to top of plot window.
          x_ticks_.ticks_on_window_or_on_axis_ = +1; // X-axis ticks to top of plot window.
        }
        // Y-axis position is determined by the range of X values.
        y_axis_position_ = y_intersects_x;  // Assume Y-axis will intersect X-axis (X range includes zero).
        if (x_axis_.min_ > (std::numeric_limits<double>::min)()) // X values all definitely > zero.
        { // Y-axis > 0, so will not intersect X-axis.
          y_axis_position_ = left; // Y-axis free to left of end of X-axis.
           // Need to put the labels on the plot window instead of the X-axis.
         y_ticks_.ticks_on_window_or_on_axis_ = -1; // Y-axis ticks to left of window (because floating off end of X-axis).
        }
        else if (x_axis_.max_ < -(std::numeric_limits<double>::min)()) // X all definitely < zero.
        { // Y-axis < 0, so will not intersect X-axis.
          y_axis_position_ = right; // Y-axis to right of plot window.
          y_ticks_.ticks_on_window_or_on_axis_ = +1; // Y-axis ticks to right of plot window.
        }

        // Ensure both axis and ticks have the *same* range.
        // (To use the separation, made to give the potential for different ranges,
        // one would have to *not* do this, but to make sure they are both assigned correctly).
        x_ticks_.max_ = x_axis_.max_;
        x_ticks_.min_ = x_axis_.min_;
        y_ticks_.max_ = y_axis_.max_;
        y_ticks_.min_ = y_axis_.min_;

        // Calculate the number of chars of the longest value labels.
        x_ticks_.longest_label(); // Updates label_max_length_
        y_ticks_.longest_label();

        // Check that labels won't collide and advise if they will - seems very difficult.
        // Change rotation to avoid collision ? Conclude not practicable.

        y_ticks_.label_max_space_ = 0.; // Work out space for y labels, depending on orientation.
        if (y_ticks_.label_rotation_ == horizontal)
        { // Move plot left edge right to give space for y_value_precision_ digits.
           y_ticks_.label_max_space_ += y_ticks_.label_max_length_; // SVG units (default pixels).
        }
        else if((y_ticks_.label_rotation_ == upward) || (y_ticks_.label_rotation_ == downward))
        { // Only need one char & 1 space width from Y-axis value label.
          y_ticks_.label_max_space_ += 2 * y_value_label_style_.font_size();
        }
        else
        { // Assume some slope 45, so diagonally down from tick,
          // and takes a bit less room.
          y_ticks_.label_max_space_ = y_ticks_.label_max_length_ * sin45;
        }

        if (y_ticks_.major_value_labels_side_ != 0)
        { // Some major tick value labels wanted.
          if ((y_ticks_.ticks_on_window_or_on_axis_ < 0) // On left of plot window.
            && (y_ticks_.major_value_labels_side_ < 0) ) // & labels on left.
          {  // Contract plot window left edge to right to make space for value labels on left.
            plot_left_ += y_ticks_.label_max_space_;
          }
          else if ((y_ticks_.ticks_on_window_or_on_axis_ > 0) // On right of plot window.
            && (y_ticks_.major_value_labels_side_ > 0) ) // & labels to right.
          {  // Contract plot window right to left to make space for value labels on right.
           plot_right_ -= y_ticks_.label_max_space_;
          }
          else
          { // y_ticks_.ticks_on_window_or_on_axis_ == 0
            // no value labels on plot window (may be on mid-plot Y-axis line).
            // Ignore the unusual case of Y-axis line too close to the axis label.
            // In this case the value labels may overflow the plot window
            // and collide with the axis label!
            // User must change to put value label downward, or on other side of the axis line.
            // using major_value_labels_side(int d)
            // to set tick value labels to left (<0), none (==0) or right (>0).
          }
        } // y_ticks_. major_value_labels_side

        x_ticks_.label_max_space_ = 0.; // Work out the longest ticks values label for X-Axis.
        if (x_ticks_.label_rotation_ == horizontal)
        { // Only 1 char height & small space needed if labels are horizontal.
          x_ticks_.label_max_space_ += 1.5 * x_value_label_style_.font_size(); // 2 SVG chars
        }
        else if ((x_ticks_.label_rotation_ == upward) || (x_ticks_.label_rotation_ == downward))
        { // ! X_axis ticks labels vertical so will need enough for all the characters in the label.
            x_ticks_.label_max_space_ += x_ticks_.label_max_length_ ; // in SVG units pixels.
        }
        else
        { // Assume label is sloping, say 45, so * sin(45) = 0.707.
            x_ticks_.label_max_space_ += x_ticks_.label_max_length_ * sin45; // SVG 'chars'.
        }

        if (x_ticks_.major_value_labels_side_ != 0)
        { // Some tick value labels.
          if ((x_ticks_.ticks_on_window_or_on_axis_ < 0) // ticks on bottom of plot window.
             && (x_ticks_.major_value_labels_side_ < 0) ) // & labels on bottom too.
          {  // Contract plot window bottom edge up to make space for X value labels on bottom.
            plot_bottom_ -= x_ticks_.label_max_space_; // Move up.
          }
          else if ((x_ticks_.ticks_on_window_or_on_axis_ > 0) //
             && (x_ticks_.major_value_labels_side_ > 0) ) // & x labels to top.
          { // Move top of plot window down to give space for x value labels.
            plot_top_ += x_ticks_.label_max_space_; // Move down.
          }
          else
          { // no labels on plot window (may be on mid-plot X-axis).
            // See also notes above on case where labels can overwrite axis.
          }
        } // x_ticks_. major_value_labels_side

        // Make space for any Y ticks.
        if(y_ticks_.left_ticks_on_)
        { // Start left of plot to right to give space for biggest of any external left ticks.
          plot_left_ += (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_); // Avoid macro max trap!
        }
        // Make space for any X ticks.
        if(x_ticks_.down_ticks_on_)
        { // Start bottom of plot higher to give space for any external down ticks.
          plot_bottom_ -= (std::max)(x_ticks_.major_tick_length_, x_ticks_.minor_tick_length_);// Avoid macro max trap!
        }

        if (x_axis_.axis_line_on_)
        { // Want an horizontal X-axis line, so check if range includes zero, so axes intersect,
          // and x_axis_ is svg coordinate of Y-axis (usually y = 0).
          // If not fiX-axis to bottom (or top) of the plot window.
          if ((x_axis_position_ == bottom) // All Y values definitely > zero.
            && !(x_ticks_.ticks_on_window_or_on_axis_ < 0) ) // & not already at bottom.
          { // y_min_ > 0 so X-axis will not intersect Y-axis, so use plot window.
            plot_bottom_ -= x_ticks_.label_max_space_; // Move up for the ticks value labels.
            x_axis_.axis_ = plot_bottom_; // Put X-axis on bottom.
          }
          else if ((x_axis_position_ == top)  // All Y values definitely < zero.
            && !(x_ticks_.ticks_on_window_or_on_axis_ > 0) ) // & not already at top.
          { // // y_max_ < 0 so X-axis will not intersect Y-axis, so use plot window.
             plot_top_ += x_ticks_.label_max_space_; // Move down for labels.
             x_axis_.axis_ = plot_top_; // Put X-axis on top.
          }
          else
          { // y_axis_position_ == y_intersects_x
            // Calculate below after transform is calculated.
          }
        } // if (use_x_axis_line_)

        if (y_axis_.axis_line_on_)
        { // Want a vertical Y-axis line, so check if range includes zero, so axes intersect,
          // and y_axis_ is svg coordinate of X-axis (usually x = 0).
          // If not fiX-axis to left (or right) of the plot window.
          if (y_axis_position_ == left) // All X values definitely > zero.
             //&& !(y_ticks_.ticks_on_window_or_on_axis_ < 0) // & not already at left.
          { // Y-axis will not intersect X -axis, so put Y-axis line on plot window.
            y_axis_.axis_ = plot_left_; // Y-axis to left,
            //plot_left_ += 2 * y_label_info_.font_size(); // with a space.
          }
          else if (y_axis_position_ == right) // All X values definitely < zero.
            //&& !(y_ticks_.ticks_on_window_or_on_axis_ > 0) // & not already at right.
          {
            y_axis_.axis_ = plot_right_; // Y-axis to right of plot window,
          }
          else
          { // x_axis_position_ == x_intersects_y
            // Calculate below after transform is calculated.
          }
        } // if (use_y_axis_line_)

        if (plot_right_ <= plot_left_)
        {
          std::cout << "plot window left x " << plot_left_ << ", right " << plot_right_ <<"!" << std::endl;
          throw std::runtime_error("Plot window right <= left!");
        }

        if (plot_top_ >= plot_bottom_)
        {
          std::cout << "plot window top y " << plot_top_ << ", bottom " << plot_bottom_ << "!" << std::endl;
          throw std::runtime_error("Plot window top >= bottom!");
        }

        // Calculate scale and shift factors for transform from Cartesian to plot.
        // SVG image is 0, 0 at top left,y increase *downwards*
        // Cartesian 0, 0 at bottom left, y increasing upwards.
        x_scale_ = (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_);
        if (!boost::math::isnormal(x_scale_))
        { // Check reasonableness of x scaling.
          std::cout << "x_scale = " << x_scale_ << ", plot_right = " << plot_right_ << ", plot_left = " << plot_left_
            << ", x_axis_.max_ " << x_axis_.max_ << "x_axis_.min_" << x_axis_.min_ << std::endl;
          throw std::range_error("X scaling wrong!");
        }
        x_shift_ = plot_left_ - x_axis_.min_ * (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_);
        if (!boost::math::isnormal(x_shift_))
        { // Check reasonableness of x scaling.
          std::cout << "x_shift_ = " << x_shift_ << ", plot_right = " << plot_right_ << ", plot_left = " << plot_left_
            << ", x_axis_.max_ " << x_axis_.max_ << "x_axis_.min_" << x_axis_.min_ << std::endl;
          throw std::range_error("X shift wrong!");
        }
        
        y_scale_ = -(plot_bottom_-plot_top_) / (y_axis_.max_-y_axis_.min_);


        y_shift_ = plot_top_ - (y_axis_.max_ * (plot_top_ - plot_bottom_) / (y_axis_.max_ - y_axis_.min_));



        if (x_axis_.axis_line_on_)
        {
          if(x_axis_position_ == x_intersects_y)
          { // Y Range *does* include zero, so x_axis_ not yet calculated.
            double y(0.); // Use y = 0
            transform_y(y);
            x_axis_.axis_ = y; // svg Y coordinate of horizontal X-axis line.
          }
        }
        if (y_axis_.axis_line_on_)
        { // May need to calculate axes, if will intersect.
          if(y_axis_position_ == y_intersects_x)
          { // X Range *does* include zero, so y_axis_ not yet calculated.
            double x(0.);
            transform_x(x);
            y_axis_.axis_ = x; // SVG x coordinate of vertical Y-axis.
          }
        }
        if (plot_window_on_)
        { // Draw plot window rectangle with border and/or background.
          image_.g(detail::PLOT_WINDOW_BACKGROUND).push_back(
            new rect_element(plot_left_, plot_top_, (plot_right_ - plot_left_), plot_bottom_ - plot_top_));
        }
#ifdef BOOST_SVG_DIAGNOSTICS
        std::cout << "plot window left x " << plot_left_ << ", right " << plot_right_ << ", bottom y " << plot_bottom_ << ", top " << plot_top_ << "." << std::endl;
#endif
      } // calculate_plot_window

      void draw_y_axis()
      { //! Draw the Y-axis line, grids and ticks with labels.
        double x;
        x = y_axis_.axis_; // Y-axis (x = 0) transformed into X SVG coordinates.
        // Perhaps to left or right of plot window if X values do not include zero.

        if (y_axis_.axis_line_on_)
        { // Want a Y-axis line.
          if (y_axis_position_ == y_intersects_x)
          { // Draw the vertical Y-axis line at cartesian x = 0).
            double ybottom = plot_bottom_;
            double ytop = plot_top_;
            if (x_axis_position_ == x_intersects_y)
            {
              if (x_ticks_.down_ticks_on_)
              {
                if (x_ticks_.ticks_on_window_or_on_axis_ < 0) // at bottom
                { // Extend the vertical line down in lieu of longest tick.
                  ybottom += (std::max)(x_ticks_.minor_tick_length_, x_ticks_.major_tick_length_);// Avoid macro max trap!
                }
              }
              else if (x_ticks_.up_ticks_on_)
              {
                if (x_ticks_.ticks_on_window_or_on_axis_ > 0) // at top
                { // Extend the vertical line up in lieu of longest tick.
                  ytop += (std::max)(x_ticks_.minor_tick_length_, x_ticks_.major_tick_length_);// Avoid macro max trap!
                }
              }
            }
            image_.g(detail::PLOT_Y_AXIS).line(x, ytop, x, ybottom);
            // <g id="yAxis" stroke="rgb(0,0,0)"><line x1="70.5" y1="53" x2="70.5" y2="357"/>
            if (y_ticks_.ticks_on_window_or_on_axis_ < 0) //(y_axis_position_ == left)
            { // Draw vertical line holding the ticks on the left of plot window.
              image_.g(detail::PLOT_Y_AXIS).line(plot_left_, plot_top_, plot_left_, plot_bottom_);
            }
            else
            {// Draw vertical line holding the ticks on the right of plot window.
              image_.g(detail::PLOT_Y_AXIS).line(plot_right_, plot_top_, plot_right_, plot_bottom_);
            }
          }
          else if (y_axis_position_ == left)
          { // Draw on the left of plot window.
            image_.g(detail::PLOT_Y_AXIS).line(plot_left_, plot_top_, plot_left_, plot_bottom_);
          }
          else if (y_axis_position_ == right)
          {// Draw on the right of plot window.
            image_.g(detail::PLOT_Y_AXIS).line(plot_left_, plot_top_, plot_right_, plot_bottom_);
          }
          else
          { // ??? Warn that things have gone wrong?
#ifdef BOOST_SVG_DIAGNOSTICS
            std::cout << "y axis error, position " << y_axis_position_ << ", window from  " << plot_left_ << " to " << plot_left_ << ", is OUTside window bottom " << plot_bottom_ << " to top " << plot_top_ << std::endl;
#endif

          }
        }

        // Access the paths for the ticks & grids, ready for additions.
        path_element& minor_tick_path = image_.g(detail::PLOT_Y_MINOR_TICKS).path();
        path_element& major_tick_path = image_.g(detail::PLOT_Y_MAJOR_TICKS).path();
        path_element& minor_grid_path = image_.g(detail::PLOT_Y_MINOR_GRID).path();
        path_element& major_grid_path = image_.g(detail::PLOT_Y_MAJOR_GRID).path();

        // y_minor_jump is the interval between minor ticks.
        double y_minor_jump = y_ticks_.major_interval_ / ((double)(y_ticks_.num_minor_ticks_ + 1.) );
        // TODO Problem here with using floating point?
        // Was i < y_max_; but didn't show the tick and value at y_max_ so now i <= y_max_;
        // But may still fail if a ls or few bits out? Seems to fail for y = 100, for example.

        // Draw the ticks on the positive side.
        for(double y = 0; y <= y_axis_.max_; y += y_ticks_.major_interval_)
        {
          for(double j = y + y_minor_jump;
            j < (y + y_ticks_.major_interval_) * (1. - 2 * std::numeric_limits<double>::epsilon());
            j += y_minor_jump)
          { // Draw minor tick.
            // This will output 'orphaned' minor ticks that are beyond the plot window,
            // if the last major tick does not coincide with the plot window.
            // These are just ignored in draw_x_minor_tick.
            // There might be 9 of them,
            // if you have the common 9 minor tick between major ticks!
            // TODO this seems ugly - as does the negative ones below.
            if (j != 0. || ! x_axis_.axis_line_on_)
            { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
              // (won't be X-axis line for 1-D where the zero tick is always wanted).
              draw_y_minor_tick(j, minor_tick_path, minor_grid_path);
            }
          }
          // Draw major tick.
          if ((y != 0. || ! x_axis_.axis_line_on_) // axis line requested.
            || (y_ticks_.ticks_on_window_or_on_axis_ != 0)) // ticks & labels on plot window.
          { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
            // (won't be X-axis line for 1-D where the zero tick is always wanted).
            draw_y_major_tick(y, major_tick_path, major_grid_path);
          }
          else
          {
            // std::cout << "Missed y " << y << std::endl; // Only miss 0s
          }
        }

        // Draw the ticks on the negative side.
        for(double y = 0; y >= y_axis_.min_; y -= y_ticks_.major_interval_)
        {
          for(double j = y; j > y - y_ticks_.major_interval_; j-= y_ticks_.major_interval_ / (y_ticks_.num_minor_ticks_ + 1))
          { // Draw minor ticks.
            if ( (j != 0. || ! y_axis_.axis_line_on_)
              || (y_ticks_.ticks_on_window_or_on_axis_ != 0) // ticks & labels on plot window.
              )
            { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
              // (won't be X-axis line for 1-D where the zero tick is always wanted).
              draw_y_minor_tick(j, minor_tick_path, minor_grid_path);
            }
          }
          if ((y != 0. || ! x_axis_.axis_line_on_)
            || (y_ticks_.ticks_on_window_or_on_axis_ != 0) ) // ticks & labels on plot window.
          { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
            // (won't be X-axis line for 1-D where the zero tick is always wanted).
            draw_y_major_tick(y, major_tick_path, major_grid_path);
          }
        }
      } // draw_y_axis

      void draw_y_axis_label()
      { //! Draw a vertical Y-axis label, and optional y units.
        // Y-label color is set in constructor thus:
        // image_.g(detail::PLOT_Y_LABEL).style().stroke_color(black);
        // and changed using y_label_color(color);
        // Similarly for font family and size.

        std::string label = y_label_info_.text();
        if ((y_axis_.label_units_on_) && (y_units_info_.text() != ""))
        { // Append the units, user must provide any enclosing ()s, if wanted.
          label += y_units_info_.text(); // for example: "distance (km)".
        }

        // Glyphs for western characters are aligned with the left bottom of capital letter,
        // so need to allow for any descenders.
        double x = plot_left_; // left edge of plot window.
        double y = (plot_bottom_ + plot_top_) / 2.; // center on the plot window.
        if (y_ticks_.ticks_on_window_or_on_axis_ < 0) // -1 means left
        { // Ticks value labels left of plot window.
          if (y_ticks_.major_value_labels_side_ < 0) // -1 means value label to left of Y-axis.
          { // tick values labels are to left of Y-axis.
            // Shift right to allow for any tick value labels.
            if ((y_ticks_.label_rotation_ == downward) || (y_ticks_.label_rotation_ == upward))
            { // downward tick value label direction 90 vertical up or down, or 60 steep degrees (might handle 60 separately).
              if (y_ticks_.major_value_labels_side_ < 0) // tick value labels are to left of plot window.
              { // Allow space for tick value labels font size to left of Y-axis or plot window.
                x -= y_value_label_info_.textstyle().font_size() * 1.3; //
              }
              if (y_ticks_.left_ticks_on_ == true)
              {  // Allow for any leftward ticks.
                x -= 1.1 * (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_);// And avoid macro max trap!
              }
              x -= 0.7 * (y_label_info_.textstyle().font_size() + y_value_label_info_.textstyle().font_size()); // best compromise?

            }
            else if ((y_ticks_.label_rotation_ == steepdown) || (y_ticks_.label_rotation_ == steepup))
            { // downward tick value label direction 90 vertical up or down, or 60 steep degrees (might handle 60 separately).
              if (y_ticks_.major_value_labels_side_ < 0) // tick value labels are to left of plot window.
              { // Allow space for tick value labels font size to left of Y-axis or plot window.
                x -= y_value_label_info_.textstyle().font_size() * 1.3; //
              }
              if (y_ticks_.left_ticks_on_ == true)
              {  // Allow for any leftward ticks.
                x -= 1.1 * (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_);// And avoid macro max trap!
              }
              x -= 0.4 * (y_label_info_.textstyle().font_size() + y_value_label_info_.textstyle().font_size()); // best compromise?
            }
            else if ((y_ticks_.label_rotation_ == uphill)  || (y_ticks_.label_rotation_ == downhill))
            { // sloping 45 degrees .
               x -= y_ticks_.label_max_space_ * sin45;
              if (y_ticks_.left_ticks_on_ == true)
              {  // Move left for any leftward ticks, and a small space.
                x -= 1.1 * (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_); // And avoid macro max trap!
                //x -= 1.2 * y_label_info_.textstyle().font_size() ; // Shift left to suit Y labels.
                //x -= 1.2 * y_value_label_info_.textstyle().font_size() ; // Shift left to suit Y labels.
                //x -= 1.2 * (std::min)(y_label_info_.textstyle().font_size(), y_value_label_info_.textstyle().font_size() ); // better
                x -= 0.7 * (y_label_info_.textstyle().font_size() + y_value_label_info_.textstyle().font_size()); // best compromise?
             }
            }
            else if ((y_ticks_.label_rotation_ == slopeup) || (y_ticks_.label_rotation_ == slopedownhill))
            { // sloping 30 degrees.
               x -= y_ticks_.label_max_space_ * sin45;
              if (y_ticks_.left_ticks_on_ == true)
              {  // Move left for any leftward ticks, and a small space.
                x -= 1.1 * (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_); // And avoid macro max trap!
                //x -= 1.2 * y_label_info_.textstyle().font_size() ; // Shift left to suit Y labels.
                //x -= 1.2 * y_value_label_info_.textstyle().font_size() ; // Shift left to suit Y labels.
                //x -= 1.2 * (std::min)(y_label_info_.textstyle().font_size(), y_value_label_info_.textstyle().font_size() ); // better
                x -= 0.7 * (y_label_info_.textstyle().font_size() + y_value_label_info_.textstyle().font_size()); // best compromise?
             }
            }
            else if  (y_ticks_.label_rotation_ == horizontal)
            { // horizontal
              if (y_ticks_.left_ticks_on_ == true)
              {  // Move left for any leftward ticks, and a small space.
                x -= 1.1 * (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_); // And avoid macro max trap!
              }
              x -= y_ticks_.label_max_space_; // Move left for the longest tick value label. (Might be zero?)
              //x -= y_label_info_.textstyle().font_size() * 1.0; // Shift left to suit Y labels.
              x -= 0.6 * (y_label_info_.textstyle().font_size() + y_value_label_info_.textstyle().font_size()); // best compromise?
            }
            else
            {
              std::cout << " Rotation of Y label" << y_ticks_.label_rotation_ << "not yet implemented!" << std::endl;
            }
          }
          else if (y_ticks_.major_value_labels_side_ > 0)
          { // +1 means Y Tick labels to right of Y-axis.
             x -= y_label_info_.textstyle().font_size() * 1.7;  // Move left from Y-axis.
          }
          else
          { // y_ticks_.y_major_value_labels_side_ == 0 means no tick value labels.
            x -= y_label_info_.textstyle().font_size() * 1.7;  // Move left from Y-axis.
          }
          if (y_ticks_.left_ticks_on_)
          { // Shift right for biggest of any leftward ticks.
            x += (std::max)(y_ticks_.minor_tick_length_, y_ticks_.major_tick_length_);
          }
        }
        else if (y_ticks_.ticks_on_window_or_on_axis_ > 0)
        { // tick values labels are to right of Y-axis.
          //
           x = 0. +  image_border_width(); // Start Y Label just right of the image left side.
           x += image_border_.margin_;
           x += y_label_info_.textstyle().font_size() * 1.; // Shift right to suit Y labels.
        }
        else if (y_ticks_.ticks_on_window_or_on_axis_ == 0)
        {  // Ticks are ON the Y-axis line,
           // so Y label is just right the plot left.
           // Character starts at bottom of capital letter, so allow for descenders.
           x = 0. +  image_border_width(); // Start Y Label just right of the image left side.
           x += image_border_.margin_;
           x += y_label_info_.textstyle().font_size() * 1.; // Shift right to suit Y labels.
        }
        // Glyph is at bottom left of western characters.

        image_.g(detail::PLOT_Y_LABEL).push_back(new
          text_element(x, // distance from left side of image.
          y, // center on the plot window.
          label, // "Y-Axis" for example.
          y_axis_label_style_, // font and size
          center_align, // One might want it to left or right_align?
          upward) // Y label must be drawn vertically.
          );
      } // draw_y_axis_label

      void draw_y_major_tick(double value, path_element& tick_path, path_element& grid_path)
      { //! Draw a Y-axis major tick, tick value labels & grids.
        double y(value); // for tick and/or grid.
        transform_y(y); // Cartesian to SVG coordinates.
        if((y < plot_top_ - 0.01) || (y > plot_bottom_ + 0.01))
          // Allow a bit extra to allow for round-off errors.
        { // tick value is way outside plot window, so nothing to do.
          return;
        }
        double x_left(0.); // Left end of tick.
        double x_right(image_.y_size()); // Right end of tick.
        if(y_ticks_.major_grid_on_ == true)
        { // Draw horizontal major Y grid line.
          if(!plot_window_on_ == true)
          {
            if(y_ticks_.major_value_labels_side_ < 0) // left
            { // Start further right to give space for Y-axis value label.
              y -= y_value_label_style_.font_size() * text_margin_;
            }

            if(y_ticks_.left_ticks_on_ == true)
            { // And similarly space for left ticks.
              y -= y_ticks_.major_tick_length_;
            }
          }
          else
          { // plot_window_on_ to use full width of plot window.
            x_left = plot_left_ + plot_window_border_.width_; // Don't write over either border.
            x_right = plot_right_ - plot_window_border_.width_;
          }
          grid_path.M(x_left, y).L(x_right, y); // Horizontal grid line.
       } // y_major_grid_on

        // Draw major ticks & value label, if necessary.
        double y_tick_length = y_ticks_.major_tick_length_;
        if (y_ticks_.ticks_on_window_or_on_axis_ < 0)
        { // Start ticks on the plot window border left.
          x_left = plot_left_; // x_left = left,
          x_right = plot_left_; //  x_right = right.
        }
        else if (y_ticks_.ticks_on_window_or_on_axis_ > 0)
        { // Start ticks on the plot window border right.
          x_left = plot_right_;
          x_right = plot_right_;
        }
        else // y_ticks_.ticks_on_window_or_on_axis_== 0
        { // Internal style ticks on vertical Y-axis.
          x_left = y_axis_.axis_; // Y-axis line.
          x_right = y_axis_.axis_;
        }
        if(y_ticks_.left_ticks_on_)
        {
          x_left -= y_tick_length; // left
        }
        if (y_ticks_.right_ticks_on_)
        {
          x_right += y_tick_length; // right.
        }
        //if ((y <= (plot_bottom_ - 0.01)) && (y >= (plot_top_ + 0.01)))
        //{ // Make sure that we are drawing inside the allowed window.
        // TODO remove this completely.  Should not be necessary.
        tick_path.M(x_left, y).L(x_right, y); // Draw the major tick.
        // leaving x_left at the left most end of any tick,
        // and x_right at the rightmost end of any tick.
        // These may be on the axis line.
        // y is the vertical tick position.
        //} // in plot window

        if(y_ticks_.major_value_labels_side_ != 0)
        { // Label the tick with a value, for example "1.2"
          std::stringstream label;
          label.precision(y_ticks_.value_precision_);
          label.flags(y_ticks_.value_ioflags_); // set ALL IOflags.
          label << value; // Example: label.str() == "20" or "0.25" or "1.2e+015"
          if (y_ticks_.strip_e0s_)
          { // Remove unecessary e, +, leadings 0s.
            std::string v = strip_e0s(label.str());
            label.str(v);
          }

          double x = 0; // Where to start writing from, at end of left or right tick, if any.
          // = 0 is only to avoid unitialised warning.
          align_style alignment = center_align;
          // Adjustments to provide space from end of tick before or after writing label.
          if (y_ticks_.label_rotation_ == horizontal)
          {  // Just shift up to center value digits on tick.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y += y_value_label_style_.font_size() * 0.2;
              x = x_left - y_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
             y += y_value_label_style_.font_size() * 0.2;
             x = x_right + y_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == upsidedown)
           {  // Just shift up to center value digits on tick.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y -= y_value_label_style_.font_size() * 0.1;
              x = x_left - y_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y -= y_value_label_style_.font_size() * 0.1;
              x = x_right + y_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
          }
          else if (y_ticks_.label_rotation_ == uphill)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y -= y_value_label_style_.font_size() * 0.2;
              x = x_left - y_value_label_style_.font_size() * 0.2;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y += y_value_label_style_.font_size() * 0.2;
              x = x_right + y_value_label_style_.font_size() * 0.7;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == slopeup)
          { // Assume some 30 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y -= y_value_label_style_.font_size() * 0.2;
              x = x_left - y_value_label_style_.font_size() * 0.2;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y += y_value_label_style_.font_size() * 0.2;
              x = x_right + y_value_label_style_.font_size() * 0.7;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == downhill)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y += y_value_label_style_.font_size() * 0.3;
              x = x_left - y_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y -= y_value_label_style_.font_size() * 0.3;
              x = x_right + y_value_label_style_.font_size() * 0.1;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == slopedownhill)
          { // Assume some 30 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y += y_value_label_style_.font_size() * 0.3;
              x = x_left - y_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y -= y_value_label_style_.font_size() * 0.3;
              x = x_right + y_value_label_style_.font_size() * 0.1;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == steepdown)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y += y_value_label_style_.font_size() * 0.3;
              x = x_left - y_value_label_style_.font_size() * 0.5;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y -= y_value_label_style_.font_size() * 0.3;
              x = x_right + y_value_label_style_.font_size() * 0.1;
              alignment = left_align;
            }
          }

          else if (y_ticks_.label_rotation_ == upward)
          { // Tick value label straight up vertically on Y-axis.
            y -= y_value_label_style_.font_size() * 0.1;
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              x = x_left - y_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for right than left if rotated.
              alignment = center_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              x = x_right + y_value_label_style_.font_size() * 1.5;
              alignment = center_align;
            }
          }
          else if (y_ticks_.label_rotation_ == steepup)
          { // Tick value label straight up vertically on Y-axis.
            y -= y_value_label_style_.font_size() * 0.1;
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              x = x_left - y_value_label_style_.font_size() * 0.5;
              // Seems to need a bit more space for right than left if rotated.
              alignment = center_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              x = x_right + y_value_label_style_.font_size() * 1.5;
              alignment = center_align;
            }
          }
          else if (y_ticks_.label_rotation_ == downward)
          { // Tick value label straight down vertically on Y-axis.
            y -= y_value_label_style_.font_size() * 0.1;
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              x = x_left - y_value_label_style_.font_size() * 1.2;
              // Seems to need a bit more space for right than left if rotated.
              alignment = center_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              x = x_right + y_value_label_style_.font_size() * 0.7;
              alignment = center_align;
            }
          }
          else
          { // Others not yet implemented.
            return; // Without any value label.
         } // All rotations.

        if (x <= 0)
        { // Sanity checks on svg coordinates.
          throw std::runtime_error("Y-tick X value negative!");
        }
        if (y <= 0)
        {
          throw std::runtime_error("Y-tick Y value negative!");
        }

        if(y_ticks_.ticks_on_window_or_on_axis_ != 0)
          { // External to plot window style left or right.
            // Always want all values including "0", if labeling external to plot window.
            // y_ticks_.ticks_on_window_or_on_axis_ == true != 0
            image_.g(detail::PLOT_Y_TICKS_VALUES).text(
              x,
              y,
              label.str(),
              y_ticks_.value_label_style_, // text_style - font etc
              alignment, y_ticks_.label_rotation_);
          }
          else
          { // ! y_ticks_.y_ticks_on_plot_window_ == 0 'Internal' - value labels either side of vertical Y-axis.
            if ((value != 0) && y_axis_.axis_line_on_)
            { // Avoid a zero ON the Y-axis if it would be cut through by any horizontal X-axis line.
              image_.g(detail::PLOT_Y_TICKS_VALUES).text(
                x,
                y,
                label.str(), // "1.23"
                y_ticks_.value_label_style_, // text_style - font etc
                alignment,
                y_ticks_.label_rotation_);
            }
          } // either on plot window or 'on axis'.
        } // want value label on tick
      } // draw_y_major_tick

      void draw_y_minor_tick(double value, path_element& tick_path, path_element& grid_path)
      { //! Draw a Y-axis minor tick and optional grid. (minor ticks do not have value labels).
        double x_left(0.); // Start on vertical Y-axis line.
        double x_right(image_.y_size()); // right edge of image.
        double y(value); // Tick position and value label,
        transform_y(y); // Convert y to svg.

        if(y_ticks_.minor_grid_on_)
        { // Draw the minor grid, if wanted.
          if(!plot_window_on_)
          {
            if(x_axis_.label_on_)
            {
              x_left += y_value_label_style_.font_size() * text_margin_;
              x_right -= y_value_label_style_.font_size() * text_margin_;
            }
          }
          else
          { // plot_window_on_
            x_left = plot_left_ + plot_window_border_.width_;
            x_right = plot_right_ - plot_window_border_.width_; // Ensure just *inside* window?
          }
          if((y >= plot_top_) && (y <= plot_bottom_) && (x_left >= plot_left_) && (x_right <= plot_right_) )
          { // Make sure that we are drawing inside the allowed plot window.
            // Note comparisons are 'upside-down' - y is increasing downwards!
            grid_path.M(x_left, y).L(x_right, y); // Draw horizontal grid line.
          }
          else
          {
            // Just ignore outside plot window
          }
        } // y_minor_grid

        // Draw y minor ticks.
        if(y_ticks_.ticks_on_window_or_on_axis_ < 0)
        { // Put y minor ticks on the plot window border left.
          x_left = plot_left_;
          x_right = plot_left_;
        }
        else if (y_ticks_.ticks_on_window_or_on_axis_ > 0)
        { // Put y minor ticks on the plot window border left.
          x_left = plot_right_;
          x_right = plot_right_;
        }
        else
        { // Internal style, y_ticks_.ticks_on_window_or_on_axis_ == 0
          x_left = y_axis_.axis_; // On the Y-axis line itself.
          x_right = y_axis_.axis_;
        }
        if(y_ticks_.left_ticks_on_)
        {
          x_left -= y_ticks_.minor_tick_length_;
        }
        if(y_ticks_.right_ticks_on_)
        {
          x_right += y_ticks_.minor_tick_length_;
        }
        //if((x_left >= plot_left_) && (x_right <= plot_right_) && (y <= plot_bottom_) && (y >= plot_top_))
        // but can never be inside if left tick!
        if((y <= plot_bottom_) && (y >= plot_top_))
        { // Make sure that we are drawing inside of the allowed plot window.
          tick_path.M(x_left, y).L(x_right, y); // Draw the horizontal tick.
        }
        else
        { // Tick is outside the window, so warn.
#ifdef BOOST_SVG_DIAGNOSTICS
          std::cout << "y minor tick at y = " << y << ", from x = " << x_left << " to " << x_right 
            << ", is OUTside Y window bottom " << plot_bottom_ << " to top " << plot_top_ << "." << std::endl;
#endif
        }
      } // void draw_y_minor_tick

      //! Check if a point is within the plot window (or not too far outside).
      bool is_in_window(double x, double y)
      {
        if ((x < plot_left_ - margin) || (x > plot_right_ + margin)
          || (y < plot_top_ - margin) || (y > plot_bottom_ + margin))
        {

#ifdef BOOST_SVG_DIAGNOSTICS
          std::cout << "window x = " << x << ", window y = " << y << " is outside plot window! " << std::endl;
#endif
          return false;
        }
        else
        {
          return true;
        }
      } // bool is_in_window(double x, double y)

      // void draw_straight_lines(const svg_2d_plot_series& series);

      //! Add line between series of data points (straight rather than a Bezier curve).
      //! Area fill with color if specified.
      void draw_straight_lines(const svg_2d_plot_series& series)
      {
        g_element& g_ptr = image_.g(detail::PLOT_DATA_LINES).add_g_element();
        g_ptr.clip_id(plot_window_clip_);
        g_ptr.style().stroke_color(series.line_style_.stroke_color_);
        g_ptr.style().fill_color(series.line_style_.area_fill_); // Now set in path below too.
        g_ptr.style().stroke_width(series.line_style_.width_);
        path_element& path = g_ptr.path();
        path.style().fill_color(series.line_style_.area_fill_);
        bool is_fill = !series.line_style_.area_fill_.is_blank();
        path.style().fill_on(is_fill); // Ensure includes a fill="none" if no fill.  
        //path.style().fill_color(series.line_style_.area_fill_); // Duplicates so no longer needed?

        size_t outside_window = 0;  // Data points that lie outside the plot window.
        size_t inside_window = 0;  // OK data points that lie inside the plot window.

        // If required to fill the area under the plot,
        // we first have to move from the X-axis (y = 0) to the first point,
        // and again to the X-axis (y = 0) at the end after the last point. 

        if (series.series_.size() < 2)
        { // Need at least two points for a line joining them.
          std::cout << "Only " << series.series_.size() << " points in series " << series.title_ << ", so no line drawn!" << std::endl;
          // Also need two point  *inside window*, but that is checked later. 
        }
        else
        {
          std::multimap<Meas, unc<false> >::const_iterator j = series.series_.begin();
          //std::multimap<unc<false>, unc<false> >::const_iterator j = series.series_.begin();
          // Using std::multimap<double, double> was prev_x = (*j).first;
            // was unc<false> prev_ux = (*j).first; for unc rather than Meas

          double prev_x = 0; // Previous X and 
          double prev_y = 0; // Y of a data point.
          Meas prev_ux = (*j).first; // x
          Meas prev_uy = (*j).second; // y
          // Assigned to keep compiler quiet at line 1817 "warning C4701: potentially uninitialized local variable 'prev_x' used."
          double y0 = 0.; // y = 0, so is start point for fill area on horizontal X-axis.

          // Try to find a first point inside the plot window.
          // It may not be the first point in the series.
          while (j != series.series_.end())
          {
            prev_ux = (*j).first; // x
            prev_uy = (*j).second; // y
            prev_x = prev_ux.value(); // 1st point X-value.
            prev_y = prev_uy.value(); // 1st point Y-value.
#ifdef BOOST_SVG_DIAGNOSTICS
          std::cout << "1st value x = " << prev_x << ", value y = " << prev_y << "." << std::endl;
#endif
            transform_point(prev_x, prev_y);
            //if ((prev_x < plot_left_) || (prev_x > plot_right_) || (prev_y < plot_top_) || (prev_y > plot_bottom_))
            if (is_in_window(prev_x, prev_y) == false)
            { // Data point is OUTside plot window, so can't draw a line from y = 0 to this point.
              // so try the next point to see if that is 'good' - inside the window.
#ifdef BOOST_SVG_DIAGNOSTICS
              std::cout << "1st x = " << prev_x << ", y = " << prev_y << " is outside plot window! " << std::endl;
#endif
              ++outside_window;
              ++j;
            }
            else
            { // Point is inside plot window, so is usable as a 1st point.
              ++inside_window;
              if (is_fill == true)
              { // Move to 1st point.
                //path.style().fill_color(series.line_style_.area_fill_); // Duplicates so no longer needed?
                transform_y(y0);
                path.M(prev_x, y0); // Start on X-axis
                path.L(prev_x, prev_y); // and draw line to 1st point.
                // This is to ensure fill.
              }
              else
              {
                path.M(prev_x, prev_y);  // Just move to 1st good X point.
              }

              break;  // and continue drawing lines below in another while loop below.
              // leaving j pointing to 2nd point.
            }
          } // while j != series_.end()
          if (inside_window == 0)
          {
            std::cout << "No start point in series " << series.title_ << " is within plot window!" << std::endl;
            // So no point trying to draw a line!
            return;
          }
          else
          {
#ifdef BOOST_SVG_DIAGNOSTICS
            std::cout << "Found 1st point in plot window x = " << prev_x << ", y = " << prev_y << "." << std::endl;
#endif
          }

          double temp_x(0.);
          double temp_y;
          for(; j != series.series_.end(); ++j)
          {
           // unc<false> temp_ux = (*j).first;
            Meas temp_ux = (*j).first;
            temp_x = temp_ux.value();
            //unc<false> temp_uy = (*j).second;
            Meas temp_uy = (*j).second;
            temp_y = temp_uy.value();
#ifdef BOOST_SVG_DIAGNOSTICS
            //std::cout << "Line to x = " << temp_x << ", y = " << temp_y << "." << std::endl;
#endif
            transform_point(temp_x, temp_y);
            if (is_in_window(temp_x, temp_y) == true)
            //if ((temp_x >= plot_left_) && (temp_x <= plot_right_) && (temp_y >= plot_top_) && (temp_y <= plot_bottom_))
            { // Data point is inside or on plot window, so draw a line to the point.
              ++inside_window;
              path.L(temp_x, temp_y); // Line to next point.
              prev_x = temp_x;
              prev_y = temp_y;
            }
            else
            { // Ignore any data point values outside the plot window.
              // Not sure what this will do if area fill chosen.
              ++outside_window;
#ifdef BOOST_SVG_DIAGNOSTICS
              std::cout << "Line draw ignoring  x = " << temp_x << ", y = " << temp_y << std::endl;
#endif
            }
          } // for j'th point

           BOOST_ASSERT(inside_window -1 + outside_window == series.series_.size());
           // -1 for the point on the x axis needed for fill.

          if(is_fill == true)
          { // Area fill wanted.
            temp_y = 0; // y = 0, so on horizontal X-axis.
            transform_y(temp_y); // X-axis line SVG coordinate.
            path.L(temp_x, temp_y).z(); // Draw line to X-axis & closepath with Z.
            // to ensure area below is filled.
          }
        }
#ifdef BOOST_SVG_DIAGNOSTICS
            std::cout << "Draw_lines plotted " << inside_window << " lines, and outside window " << outside_window
            << ", size of series = " << series.series_.size() << std::endl;
#endif
      } // draw_straight_lines

      void draw_bezier_lines(const svg_2d_plot_series& series)
      { //! Add Bezier curve line between data points.
        //! Warning:
        //! At present it is assumed that all data points lie within the plot window.
        //! If this is not true, then strange and unpredictable curves will be produced!

        g_element& g_ptr = image_.g(detail::PLOT_DATA_LINES).add_g_element();
        g_ptr.clip_id(plot_window_clip_);
        g_ptr.style().stroke_color(series.line_style_.stroke_color_);
        path_element& path = g_ptr.path();

        // Do not store or use uncertainty info for Bezier.
        std::pair<double, double> n; // current point.
        std::pair<double, double> n_minus_1; // penultimate.
        std::pair<double, double> n_minus_2; // 'pen-penultimate'.
        std::pair<double, double> fwd_vtr;
        std::pair<double, double> back_vtr;

        bool is_fill = !series.line_style_.area_fill_.is_blank();
        if(is_fill == false)
        {
          path.style().fill_on(false); // Default path constructor is false.
        }
        else
        { // !is_blank so DO want area fill.
          path.style().fill_color(series.line_style_.area_fill_);
        }

        if(series.series_.size() > 2)
        { // Need >= 3 points for a cubic curve (start point, 2 control points, and end point).
          std::multimap<Meas, unc<false> >::const_iterator iter = series.series_.begin();
 //         std::multimap<unc<false>, unc<false> >::const_iterator iter = series.series_.begin();
          std::pair<Meas, unc<false> > un_minus_1 = *(iter++); // 1st unc X & Y data.
//          std::pair<unc<false>, unc<false> > un_minus_1 = *(iter++); // 1st unc X & Y data.
          n_minus_1 = std::make_pair(un_minus_1.first.value(), un_minus_1.second.value()); // X and Y values.
          //n_minus_1 = *(iter++);  // begin()
          transform_pair(n_minus_1);
          // Should check that point is inside plot window. TODO?
          {
            std::pair<Meas, unc<false> > un = *(iter++); // Middle point of trio for bezier.
            n = std::make_pair(un.first.value(), un.second.value()); // X and Y values.
          }
          transform_pair(n);
          // Should check that point is inside plot window. TODO?
          path.M(n_minus_1.first, n_minus_1.second); // move m_minus_1, the 1st data point.

          double control = 0.1;
          // 0.2 is a scaling factor that Jake used to define the magnitude of the
          // vector of the current control point to be placed, basically
          // taking advantage of the auto-drawing of Bezier curves that exists in
          // the SVG format, and this is his attempt to give the control point the
          // proper length.

          // Experiment suggests that 0.2 gives distorsions with exp curves.
          // 0.05 is just visually OK with 50 points, but 100 are better.

          for(; iter != series.series_.end(); ++iter)
          {
            n_minus_2 = n_minus_1;
            n_minus_1 = n;
            {
              std::pair<Meas, unc<false> > un = *iter; // middle
              n = std::make_pair(un.first.value(), un.second.value()); // X and Y values.
            }
            transform_pair(n);
            // Should check that point is inside plot window. TODO?

            back_vtr.first = ((n_minus_1.first - n.first) + // (x diff - x previous diff) * control
              (n_minus_2.first - n_minus_1.first)) * control;
            back_vtr.second = ((n_minus_1.second - n.second) + // y
              (n_minus_2.second - n_minus_1.second)) * control;

            // 8.3.6 The cubic Bezier curve commands path.S(x, y).
            // Start point, end point, & two control points.
            // Example: S378.5,519.3 381,519.3 ...
            // S end_control_point, end point
            // Start is reflection of last point's control point.
            path.S(n_minus_1.first + back_vtr.first, // x
              n_minus_1.second + back_vtr.second, // y - end control point
              n_minus_1.first, n_minus_1.second); // x, y - end point.
          } // for
          // Last point.
          back_vtr.first = 0.;
          back_vtr.second = (n.second - n_minus_1.second) * control;
          path.S(n.first + back_vtr.first, // x
            n.second + back_vtr.second, // y
            n.first, n.second); // x, y
        }
        else
        { // Only one or two points, so no curving possible!
          draw_straight_lines(series);
        }
      } // draw_bezier_lines

      void draw_plot_lines()
      { //! Draw line through data series, Bezier curved or straight, or none.
        for(unsigned int i = 0; i < serieses_.size(); ++i)
        {
          if(serieses_[i].line_style_.bezier_on_)
          { // curved.
            draw_bezier_lines(serieses_[i]);
          }
          else if(serieses_[i].line_style_.line_on_)
          {
            draw_straight_lines(serieses_[i]);
          }
          else
          { // No line joining points.
          }
        }
      } // draw_plot_lines

      void draw_plot_points()
      { //! Draw normal 'good' non-limit points, and then any 'at limits' points.
        double x(0.);
        double y(0.);
        for(unsigned int i = 0; i < serieses_.size(); ++i)
        {
          g_element& g_ptr = image_.g(detail::PLOT_DATA_POINTS).add_g_element();

          g_ptr.style()
            .fill_color(serieses_[i].point_style_.fill_color_)
            .stroke_color(serieses_[i].point_style_.stroke_color_);

          size_t ignored = 0;
          size_t plotted = 0;

          for(std::multimap<Meas, unc<false> >::const_iterator j = serieses_[i].series_.begin();
          //for(std::multimap<unc<false>, unc<false> >::const_iterator j = serieses_[i].series_.begin();
            j != serieses_[i].series_.end(); ++j)
          {
            Meas ux = j->first;
            //unc<false> ux = j->first;
            x = ux.value(); // Just the X value.
            //double vx = x; // Note the true X value.
            unc<false> uy = j->first;
            uy = j->second;
            y = uy.value(); // Just the Y value.
            //double vy = y; // Note the true Y value.
            transform_point(x, y); // Note x and y are now SVG coordinates.
            if((x > plot_left_) && (x < plot_right_) && (y > plot_top_) && (y < plot_bottom_))
            { // Data point is inside plot window, so draw a point.
              plotted++;
              draw_plot_point(x, y, g_ptr, serieses_[i].point_style_, ux, uy); // Add the unc ux and uy to allow access to uncertainty.
              // TODO might refactor so that only pass ux, and uy.
              g_element& g_ptr_vx = image_.g(detail::PLOT_X_POINT_VALUES).add_g_element();
              if (x_values_on_)
              { // Show the value of the X data point too.
                // void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, double value)

                draw_plot_point_value(x, y, g_ptr_vx, x_values_style_, serieses_[i].point_style_, ux);
              }
              g_element& g_ptr_vy = image_.g(detail::PLOT_Y_POINT_VALUES).add_g_element();
              if (y_values_on_)
              { // show the value of the Y data point too.
                draw_plot_point_value(x, y, g_ptr_vy, y_values_style_,serieses_[i].point_style_, uy);
              }
              if (xy_values_on_)
              { // Show the two values of the X & Y data as a pair.
                draw_plot_point_values(x, y, g_ptr_vx, g_ptr_vy, x_values_style_, y_values_style_, ux, uy);
              }
            } // if inside plot window
            else
            { // Ignore any data point values outside the plot window.
              ignored++;
#ifdef BOOST_SVG_DIAGNOSTICS
              std::cout << "Ignoring x = " << x << ", y = " << y << std::endl;
#endif
            }
          } // for j
#ifdef BOOST_SVG_DIAGNOSTICS
          std::cout << plotted << " plotted " << ", and " << ignored << " ignored, " 
            << "size of series = " << serieses_[i].series_.size() << std::endl;
          BOOST_ASSERT(plotted + ignored == serieses_[i].series_.size());
#endif
        } // for normal points.

        //! Draw the abnormal 'at_limit' points (if any).

        using boost::svg::detail::limit_NaN;

        for(unsigned int i = 0; i < serieses_.size(); ++i)
        {
          g_element& g_ptr = image_.g(detail::PLOT_LIMIT_POINTS);
          size_t limit_point_in_window_count = 0;
          size_t limit_point_edge_window_count = 0;
          size_t inf_count = 0;
          size_t x_inf_count = 0;
          size_t y_inf_count = 0;
          size_t nan_count = 0;
          size_t x_nan_count = 0;
          size_t y_nan_count = 0;

          for(std::multimap<double,double>::const_iterator j = serieses_[i].series_limits_.begin();
            j!=serieses_[i].series_limits_.end(); ++j)
          {
            x = j->first;
            y = j->second;
            if (limit_NaN(x))
            { // x is NaN (rather than too big or too small).
              x_nan_count++;
              x = 0.;
              transform_x(x);
              // If include zero, OK, else plot on left or right as appropriate.
              if (x < plot_left_)
              {
                x = plot_left_;
              }
              else if (x > plot_right_)
              {
                x = plot_right_;
              }
              //else X-axis includes zero, so x is OK.
            }
            else
            { // x Not NaN (so is inf or max)
              x_inf_count++;
              transform_x(x);
              if (x < plot_left_)
              {
                x = plot_left_;
              }
              else if (x > plot_right_)
              {
                x = plot_right_;
              }
              // else is inside X-axis plot window.
            }
            if (limit_NaN(y))
            { // y is NaN (rather than too big or too small).
              y_nan_count++;
              y = 0.;
              transform_y(y);
              // If include zero, OK, else plot on left or right as appropriate.
              if (y < plot_top_)
              { // Note Y SVG coordinate increase downwards.
                y = plot_top_;
              }
              else if (y > plot_bottom_)
              {
                y = plot_bottom_;
              }
              //else Y-axis includes zero, so y is OK.
            }
            else
            { // y Not NaN (so is inf or max).
              y_inf_count++;
              transform_y(y);
              if (y < plot_top_)
              {
                y = plot_top_;
              }
              else if (y > plot_bottom_)
              {
                y = plot_bottom_;
              }
              // else is inside plot window, so draw a limit point marker.
              // draw_plot_point(x, y, g_ptr, plot_point_style(lightgray, whitesmoke, s, cone)); default.
            }
            serieses_[i].limit_point_style_.stroke_color_ = image_.g(detail::PLOT_LIMIT_POINTS).style().stroke_color();
            serieses_[i].limit_point_style_.fill_color_ = image_.g(detail::PLOT_LIMIT_POINTS).style().fill_color();
            // This is a kludge.  limit_point_style_ should probably be common to all data series.

            if((x > plot_left_) && (x < plot_right_) && (y > plot_top_) && (y < plot_bottom_))
            { // Is inside plot window, so draw a point.
              // draw_plot_point(x, y, g_ptr, plot_point_style(blank, blank, s, cone)); default.
              draw_plot_point(x, y, g_ptr, serieses_[i].limit_point_style_, unc<false>(), unc<false>());
              limit_point_in_window_count++;
            }
            else
            { // Outside, or on edge of window.
              draw_plot_point(x, y, g_ptr, serieses_[i].limit_point_style_, unc<false>(), unc<false>());  // No uncertainty info for values at limit infinity & NaN.
              limit_point_edge_window_count++;
            }
          } // for limit point
#ifdef BOOST_SVG_DIAGNOSTICS
          std::cout << limit_point_in_window_count << " limit points in window, " << limit_point_edge_window_count  << " limits points on edge of window." << std::endl;
          std::cout << "X: " << x_nan_count << " NaNs, " << x_inf_count << " infinities, Y: " << y_nan_count << " NaNs, " << y_inf_count << " infinities." << std::endl;
#endif


        } // for series


      } //  void draw_plot_points()

      void draw_bars()
      { //! Draw normal bar chart for 'good' non-limit points.
        g_element& g_ptr = image_.g(detail::PLOT_DATA_POINTS).add_g_element(); // Moved up out of loop.
        double x(0.);
        double y(0.); // Cartesian coord y = 0.
        double x0(0.); // Y-axis line.
        double y0(.0); // X-axis line.
        transform_y(y0); // SVG coordinate of horizontal X-axis line.
        transform_x(x0); // SVG coordinate of vertical Y-axis line.
        for(unsigned int i = 0; i < serieses_.size(); ++i)
        {
          if (serieses_[i].bar_style_.bar_option_ == no_bar)
          { // No bars wanted for this series.
            continue;
          }
          g_ptr.style().stroke_color(serieses_[i].bar_style_.color_); // stroke color of stick or rectangle block.
            //.fill_color(series[i].bar_style_.area_fill_); // Only used for rectangle stroke.
            //.stroke_width(series[i].bar_style_.width_) // Used for width of stick and rectangle block.
          path_element& path = g_ptr.path();
          //path.fill(series[i].bar_style_.area_fill_ != blank);
          path.fill_on(false);

          double h_w = serieses_[i].bar_style_.width_; // For block bar chart.
          //double h_h = 0.;
          for(std::multimap<Meas, unc<false> >::const_iterator j = serieses_[i].series_.begin();
            j != serieses_[i].series_.end(); ++j)
          { // All the 'good' data points.
            Meas ux = j->first;
            x = ux.value();
            unc<false> uy = j->second;
            y = uy.value();
            transform_point(x, y);
            if((x > plot_left_)  && (x < plot_right_) && (y > plot_top_)  && (y < plot_bottom_))
            { // Is inside plot window, so some bar to draw.
              switch(serieses_[i].bar_style_.bar_option_)
              { // -2 block to Y-axis,-1 stick to Y-axis, none, +1 stick to X-axis, -2 block to X-axis.
              case y_block: // Draw a rectangle centered on the data point horizontally to Y-axis.
                 {
                   g_ptr.style().stroke_width(serieses_[i].line_style_.width_) // line_width used for rectangle line width.
                     .fill_color(serieses_[i].bar_style_.area_fill_);
                   double h_left = x;
                   double h_top = y - h_w / 2; // Start a half-width above the data point center.
                   path.M(h_left, h_top).L(h_left, h_top + h_w).L(x0, h_top + h_w).L(x0, h_top).z();
                 }
                 break;
              case y_stick:
                 path.style().stroke_width(serieses_[i].bar_style_.width_); // bar_width used for stick line width.
                 path.M(x, y).L(x0, y); // Draw a line from point horizontally to Y-axis.
                 break;
              case none:
                 break; // Already handled above, so should not get here.
              case x_stick:
                 path.style().stroke_width(serieses_[i].bar_style_.width_); // bar_width used for stick line width.
                 path.M(x, y).L(x, y0); // Draw a line from point vertically to X-axis.
                 break;
              case x_block: // Draw a rectangle centered on the data point vertically to X-axis.
               {
                 g_ptr.style().stroke_width(serieses_[i].line_style_.width_) // line_width used for rectangle line width.
                   .fill_color(serieses_[i].bar_style_.area_fill_);
                 double h_left = x - h_w / 2; // Start a half width left of the data point center.
                 double h_top = y;
                 path.M(h_left, h_top).L(h_left + h_w, h_top).L(h_left + h_w, y0).L(h_left, y0).z();
               }
                break;
              } // switch
            } // for
          } // for normal points
        }
        // Ignore all the 'bad' at_limit points.
      } //  void draw_bars()

      void draw_histogram()
      { /*!
         Draw a histogram with variable width but contiguous bins.
         Histograms differ from bar charts in the the *area* denotes the value,
         whereas the bar *height* denotes the value for a bar chart.
         bin widths are provided from the X-axis data series values.
         The 1st data X-value provides the start of the 1st bin,
         the 2nd data X-value provides the end of the 1st bin,
         and the 1st Y-value the area of the 1st bin,
         and the start of the second bin, and so on, until the
         width of last bin is calculated from the last data point in series,
         that must have a zero area.  ? NaN
         Bins can be the same (most common) or different widths.
         Intervals must not overlap and bins must be adjacent.
         http://en.wikipedia.org/wiki/Histogram

         Attempts to allow a row or horizontal were abandoned because of complications
         with the use of map which orders the x values providing the bins.
         Using the y values for the bins implies changing the Y axes labeling and scaling too.
        */

        g_element& g_ptr = image_.g(detail::PLOT_DATA_POINTS).add_g_element(); // Moved up out of loop.
        for(unsigned int i = 0; i < serieses_.size(); ++i)
        { // for each data series.
          if (serieses_[i].histogram_style_.histogram_option_ == no_histogram)
          { // No histogram wanted for this series.
            continue;
          }
          // Get the color scheme.
          g_ptr.style().stroke_color(serieses_[i].line_style_.stroke_color_); // stroke color around bin blocks.
          g_ptr.style().fill_color(serieses_[i].line_style_.area_fill_);
          g_ptr.style().stroke_width(serieses_[i].line_style_.width_); // line_width used for stick line width.

          path_element& path = g_ptr.path();
          path.fill_on(serieses_[i].line_style_.area_fill_ != blank);
          if (path.fill_on() == true)
          {
            path.style().fill_color(serieses_[i].line_style_.area_fill_);
          }
          else
          {
            path.style().fill_color(blank);
          }

          std::multimap<Meas, unc<false> >::const_iterator last = serieses_[i].series_.end();
          last--; // Final pair with first the last bin end, and value zero or NaN.
          Meas u = last->second;
          if (u.value() != 0)
          {
            std::cout << "Last bin end " << last->first << " should have zero value! but is "  << last->second << std::endl;
            // Or Throw? or skip this series?
          }
          for(std::multimap<Meas, unc<false> >::const_iterator j = serieses_[i].series_.begin();
            j != last; ++j)
          { // All the 'good' 'real' data points.
            Meas ux = j->first;
            double x = ux.value();
            unc<false> uy =  j->second;
            double y = uy.value();
            std::multimap<Meas, unc<false> >::const_iterator j_next = j;
            j_next++;
            if (j != last)
            { // Draw a column (perhaps filled) to show bin.
              Meas ux_next= j_next->first;
              double x_next = ux_next.value();
              double w = x_next - x;
              double h = y / w;
              // std::cout << x << ' ' << y << ' ' << w << ' '  << h << std::endl;
              double yy = h;
              double y0(0.); // X-axis line.
              transform_y(y0); // SVG y coordinate of horizontal X-axis line.
              transform_x(x); // SVG x coordinate of start of bin,
              transform_x(x_next);  // SVG x coordinate of end of bin,
              transform_y(yy); // SVG y coordinate of height of bin.
              //if((x > plot_left_)  && (x < plot_right_) && (y > plot_top_)  && (y < plot_bottom_))
              //{ // Is inside plot window, so some columns to draw. TODO checks?
                path.M(x, y0).L(x, yy) // Draw a line from point vertically from X-axis.
                  .L(x_next, yy) // & horizonally to next bin end (next x value).
                  .L(x_next, y0) // back to X-axis.
                  .Z(); // So will fill.
             } // if
          } // for series
        } // for normal points.
        // Ignore all the 'bad' at_limit points.
      } //  void draw_histogram()

      void update_image()
      { //! Draw the whole SVG image.
        clear_all();
        // svg paint rules are that later 'painting' writes over
        // previous painting, so the order of drawing is important.

        // Draw image background (perhaps with border and/or fill color).
        image_.g(detail::PLOT_BACKGROUND).push_back(
          new rect_element(0, 0, image_.x_size(),  image_.y_size()));

        calculate_plot_window();
        draw_title(); // Moved to ensure plot_X and Y are valid.

        // Define the clip path for the plot window.
        // We don't want to allow overlap of the plot window border lines,
        // thus the minor adjustments.
        // TODO should this be border thickness?
        // Actually we DO want to allow a small amount of overlap
        // so round point can lie on the axis line without being chopped in half or not show at all!!!
        image_.clip_path(rect_element(plot_left_ + 1, plot_top_ + 1,
          plot_right_ - plot_left_ - 2, plot_bottom_ - plot_top_ - 2),
          plot_window_clip_);
        // <clipPath id="plot_window"><rect x="35" y="38" width="309" height="322"/></clipPath>

        image_.g(detail::PLOT_DATA_POINTS).clip_id(plot_window_clip_);

        // Draw axes, labels & legend, as required.
        draw_x_axis(); // Must do X-axis first.
        draw_y_axis();
        if(legend_on_)
        {
          draw_legend();
        }
        if(x_axis_.label_on_)
        {
          draw_x_axis_label();
        }
        if(y_axis_.label_on_)
        {
          draw_y_axis_label();
        }

        draw_plot_lines(); // Draw lines between points.
        draw_plot_points();
        draw_bars();
        draw_histogram();
    } // void update_image()
//! \endcond

    public: // Declarations of member functions.

      // All return @c *this to permit chaining, documented with
      // \return Reference to svg_2d_plot_series to make chainable.

      // write() has two flavors, a file and a ostream.
      // The file version opens an ostream, and calls the stream version.
      // The stream version first clears all unnecessary data from the graph,
      // builds the document tree, and then calls the write function for the root
      // document node, which calls all other nodes through the Visitor pattern.
      svg_2d_plot& write(const std::string& file);
      svg_2d_plot& write(std::ostream& s_out);

      // Member functions to set and get plot options.

      // These below only refer to 2D plot.
      // See axis_plot_label.hpp for all the many 1D functions X-Axis.

      axis_line_style& x_axis();
      axis_line_style& y_axis();
      ticks_labels_style& x_ticks();
      ticks_labels_style& y_ticks();
      svg_2d_plot& y_label_on(bool cmd);
      bool y_label_on();
      svg_2d_plot& x_label_on(bool cmd);
      bool x_label_on();
      svg_2d_plot& y_major_labels_side(int place);
      int y_major_labels_side();
      svg_2d_plot& y_major_label_rotation(rotate_style rot);
      int y_major_label_rotation();
      svg_2d_plot& y_axis_width(double width);
      double y_axis_width();
      svg_2d_plot& y_value_precision(int digits);
      int y_value_precision();
      svg_2d_plot& y_value_ioflags( std::ios_base::fmtflags flags);
      int y_value_ioflags();
      svg_2d_plot& y_labels_strip_e0s(bool cmd);
      bool y_labels_strip_e0s();
      svg_2d_plot& y_axis_color(const svg_color& col);
      svg_color y_axis_color();
      svg_2d_plot& y_axis_label_color(const svg_color& col);
      svg_color y_axis_label_color();
      svg_2d_plot& y_label_units_on(bool b);
      bool y_label_units_on();
      svg_2d_plot& y_axis_value_color(const svg_color& col);
      svg_color y_axis_value_color();
      svg_2d_plot& y_label_width(double width);
      double y_label_width();
      svg_2d_plot& y_major_grid_color(const svg_color& col);
      const svg_color y_major_grid_color();
      svg_2d_plot& y_minor_grid_color(const svg_color& col);
      const svg_color y_minor_grid_color();
      svg_2d_plot& y_major_tick_color(const svg_color& col);
      const svg_color y_major_tick_color();
      svg_2d_plot& y_minor_tick_color(const svg_color& col);
      const svg_color y_minor_tick_color();
      const std::string y_axis_position();
      svg_2d_plot& y_range(double min_y, double max_y);
      std::pair<double, double> y_range();
      double y_min();
      double y_max();
      bool y_autoscale();
      svg_2d_plot& y_autoscale(bool b);
      bool y_values_on();
      svg_2d_plot& y_values_on(bool b);
      bool xy_values_on();
      svg_2d_plot& xy_values_on(bool b);
      bool y_plusminus_on();
      svg_2d_plot& y_plusminus_on(bool b);
      svg_2d_plot& y_plusminus_color(const svg_color& col);
      const svg_color y_plusminus_color();
      bool y_addlimits_on();
      svg_2d_plot& y_addlimits_on(bool b);
      svg_2d_plot& y_addlimits_color(const svg_color& col);
      const svg_color y_addlimits_color();
      bool y_df_on();
      svg_2d_plot& y_df_on(bool b);
      svg_2d_plot& y_df_color(const svg_color& col);
      const svg_color y_df_color();
      svg_2d_plot& y_decor(const std::string& pre, const std::string& sep = "", const std::string& suf = "");
      const std::string y_prefix();
      const std::string y_suffix();
      const std::string y_separator();
      svg_2d_plot& y_autoscale(double minimum, double maximum);
      svg_2d_plot& y_autoscale(std::pair<double, double> p);
      template <typename T> // T an STL container: array, vector ...
      svg_2d_plot& y_autoscale(const T& begin, const T& end); // Data series using iterators to
      template <typename T> // T an STL container: array, vector ...
      svg_2d_plot& y_autoscale(const T& container); // Whole data series.
      template <typename T> // T a 2D STL container: array, vector ...
      svg_2d_plot& xy_autoscale(const T& container); // Whole data series,
      bool xy_autoscale();
      double y_major_tick_length();
      svg_2d_plot& y_major_tick_length(double length);
      double y_major_interval();
      svg_2d_plot& y_major_interval(double inter);
      svg_2d_plot& y_minor_tick_length(double length);
      double y_minor_tick_length();
      svg_2d_plot& y_num_minor_ticks(unsigned int num);
      unsigned int y_num_minor_ticks();
      svg_2d_plot& y_label_axis(const std::string& str);
      std::string y_label_axis();
      svg_2d_plot& y_major_tick_width(double width);
      double y_major_tick_width();
      svg_2d_plot& y_minor_tick_width(double width);
      double y_minor_tick_width();
      svg_2d_plot& x_ticks_on_window_or_axis(int);
      int x_ticks_on_window_or_axis();
      svg_2d_plot& x_major_labels_side(int);
      int x_major_labels_side();
      svg_2d_plot& y_ticks_on_window_or_axis(int);
      int y_ticks_on_window_or_axis();
      svg_2d_plot& y_ticks_left_on(bool cmd);
      bool y_ticks_left_on();
      svg_2d_plot& y_ticks_right_on(bool cmd);
      bool y_ticks_right_on();
      svg_2d_plot& y_major_grid_on(bool is);
      bool y_major_grid_on();
      svg_2d_plot& y_minor_grid_on(bool is);
      bool y_minor_grid_on();
      svg_2d_plot& y_minor_grid_width(double width);
      double y_minor_grid_width();
      svg_2d_plot& y_major_grid_width(double width);
      double y_major_grid_width();
      svg_2d_plot& y_label_font_size(unsigned int i);
      unsigned int y_label_font_size();
      svg_2d_plot& y_label_weight(std::string s);
      const std::string& y_label_weight();
      svg_2d_plot& y_label_font_family(const std::string& family);
      const std::string& y_label_font_family();

      svg_2d_plot& y_ticks_values_color(const svg_color& col);
      svg_color y_ticks_values_color();
      svg_2d_plot& y_ticks_values_precision(int p);
      int y_ticks_values_precision();
      svg_2d_plot& y_ticks_values_ioflags(std::ios_base::fmtflags f);
      std::ios_base::fmtflags y_ticks_values_ioflags();

      svg_2d_plot& y_ticks_values_font_size(unsigned int i);
      unsigned int y_ticks_values_font_size();
      svg_2d_plot& y_ticks_values_font_family(const std::string& family);
      const std::string& y_ticks_values_font_family();

      svg_2d_plot& y_values_font_size(unsigned int i);
      unsigned int y_values_font_size();
      svg_2d_plot& y_values_font_family(const std::string& family);
      const std::string& y_values_font_family();
      svg_2d_plot& y_values_color(const svg_color& col);
      svg_color y_values_color();
       svg_2d_plot& y_values_rotation(rotate_style rotate);
      int y_values_rotation();
      svg_2d_plot& y_values_precision(int p);
      int y_values_precision();
      svg_2d_plot& y_values_ioflags(std::ios_base::fmtflags f);
      std::ios_base::fmtflags y_values_ioflags();

      // Versions of plot functions to add data series from a container, all or part,
      // declarations including defaults for parameters (except containers, of course).
      template <typename T>
      svg_2d_plot_series& plot(const T& container, const std::string& title = "");

      template <typename T, typename U>
      svg_2d_plot_series& plot(const T& container, const std::string& title = "",
        U functor = boost::svg::detail::pair_double_2d_convert() );

      template <typename T>
      svg_2d_plot_series& plot(const T& begin, const T& end, const std::string& title = "");

      template <typename T, typename U>
      svg_2d_plot_series& plot(const T& begin, const T& end, const std::string& title = "",
        U functor = boost::svg::detail::pair_double_2d_convert() );

 }; // class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>

   // Definition of class svg_2d_plot member functions.

      axis_line_style& svg_2d_plot::x_axis()
      { //! \return @c true if horizontal X-axis line to be drawn.
        return x_axis_;
      }

     axis_line_style&  svg_2d_plot::y_axis()
      { //! \return @c true if vertical Y-axis line to be drawn.
        return y_axis_;
      }

      ticks_labels_style& svg_2d_plot::x_ticks()
      { //! \return @c true if ticks are to be marked on the X-axis.
        return x_ticks_;
      }

      ticks_labels_style& svg_2d_plot::y_ticks()
      { //! \return @c true if ticks are to be marked on the Y-axis.
        return y_ticks_;
      }

      svg_2d_plot& svg_2d_plot::y_label_on(bool cmd)
      { //! Set @c true if to label Y-axis with name (and units).
        y_axis_.label_on_ = cmd;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_label_on()
      { //! \return @c true if Y-axis is to labelled.
        return y_axis_.label_on_;
      }

      svg_2d_plot& svg_2d_plot::x_label_on(bool cmd)
      { //! Set to include an X-axis text label.
        //! \see x_label_units
        x_axis_.label_on_ = cmd;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::x_label_on()
      { //! \return @c true if to include an X-axis text label.
        return x_axis_.label_on_;
      }

      svg_2d_plot& svg_2d_plot::y_major_labels_side(int side)
      { /*! Position of labels for major ticks on vertical Y-axis line.
           \arg \c side > 0 label to left of Y-axis line (default),
           \arg \c side = 0 (false) means no major tick labels on Y-axis.
           \arg \c side > 0 means to right of Y-axis line.
        */
        y_ticks_.major_value_labels_side_ = side;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_major_labels_side()
      { //! \return Position of labels (if any) for major ticks on vertical Y-axis line.
        return y_ticks_.major_value_labels_side_;
      }

      svg_2d_plot& svg_2d_plot::y_major_label_rotation(rotate_style rot)
      { /*! Rotation or orientation of labels for major ticks on vertical Y-axis line.
        \param rot Default orientation is horizontal (0).
        \see @c rotate_style for possible values: horizontal, uphill...
        */
        y_ticks_.label_rotation_ = rot;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_major_label_rotation()
      { //! \return Rotation of Y-axis major tick labels.
        return  y_ticks_.label_rotation_ ;
      }

      svg_2d_plot& svg_2d_plot::y_axis_width(double width)
      { //! Set width of Y-axis line.
        image_.g(detail::PLOT_Y_AXIS).style().stroke_width(width);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_axis_width()
      {//! \return Width of Y-axis line.
        return image_.g(detail::PLOT_Y_AXIS).style().stroke_width();
      }

      svg_2d_plot& svg_2d_plot::y_value_precision(int digits)
      { /*! Set precision of Y tick label values in decimal digits (default 3).

        @b Example:
        */
        /*!
          \code
my_plot.x_value_ioflags(ios::dec | ios::scientific).x_value_precision(2);
          \endcode
        */
        y_ticks_.value_precision_ = digits;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_value_precision()
      { //! \return Precision of Y tick value labels in decimal digits (default 3).
        return y_ticks_.value_precision_;
      }

      svg_2d_plot& svg_2d_plot::y_value_ioflags(std::ios_base::fmtflags flags)
      { //! Set std::ioflags of Y tick label values (default 0x201 == dec).

        //! @b Example:
        /*!
          \code
my_plot.x_value_ioflags(ios::dec | ios::scientific).x_value_precision(2);
          \endcode
        */
        y_ticks_.value_ioflags_ = flags;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_value_ioflags()
      { //! \return All stream ioflags for control of format of Y tick value labels.
        return y_ticks_.value_ioflags_;
      }

      svg_2d_plot& svg_2d_plot::y_labels_strip_e0s(bool cmd)
      { //! If @c true then strip unnecessary zeros, signs from labels.
        y_ticks_.strip_e0s_ = cmd;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_labels_strip_e0s()
      { //! \return @c true if to strip unnecessary zeros, signs from labels.
        return y_ticks_.strip_e0s_;
      }

      svg_2d_plot& svg_2d_plot::y_axis_color(const svg_color& col)
      { //! Set Y-axis linecolor. (set only stroke color).
        image_.g(detail::PLOT_Y_AXIS).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      svg_color svg_2d_plot::y_axis_color()
      { //! \return Ty_axis stroke color.
        return image_.g(detail::PLOT_Y_AXIS).style().stroke_color();
      }

      svg_2d_plot& svg_2d_plot::y_axis_label_color(const svg_color& col)
      { //! Set y_axis stroke color.
        //! \note Setting the stroke color may produce fuzzy characters :-(
        image_.g(detail::PLOT_Y_LABEL).style().fill_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      svg_color svg_2d_plot::y_axis_label_color()
      { //! \return Y-axis label stroke color.
        //  y_label_info_.style().stroke_color();
        return image_.g(detail::PLOT_Y_LABEL).style().fill_color();
      }

      svg_2d_plot& svg_2d_plot::y_label_units_on(bool b)
      { //! Set @c true to add @b units text to the Y-axis label.
        //! \sa @c svg_2d_plot::y_label_units
        y_axis_.label_units_on_ = b;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_label_units_on()
      { //! \return @c true if to add units text to the Y-axis label.
        return y_axis_.label_units_on_;
      }

      svg_2d_plot& svg_2d_plot::y_axis_value_color(const svg_color& col)
      { //! Set color of Y-axis @b value labels.
        image_.g(detail::PLOT_Y_TICKS_VALUES).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      svg_color svg_2d_plot::y_axis_value_color()
      { //! \return Color of Y-axis tick value labels.
        // Only return the stroke color.
        return image_.g(detail::PLOT_Y_TICKS_VALUES).style().stroke_color();
      }

      svg_2d_plot& svg_2d_plot::y_label_width(double width)
      { //! Set width of Y-axis value labels.
        image_.g(detail::PLOT_Y_LABEL).style().stroke_width(width);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_label_width()
      { //! \return Width of Y-axis value labels.
        return image_.g(detail::PLOT_Y_LABEL).style().stroke_width();
      }

      svg_2d_plot& svg_2d_plot::y_major_grid_color(const svg_color& col)
      { //! Set color of Y major grid lines.
        image_.g(detail::PLOT_Y_MAJOR_GRID).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_major_grid_color()
      { //! \return Color of Y major grid lines.
        return image_.g(detail::PLOT_Y_MAJOR_GRID).style().stroke_color();
      }

      svg_2d_plot& svg_2d_plot::y_minor_grid_color(const svg_color& col)
      { //! Set color of Y minor grid lines.
        image_.g(detail::PLOT_Y_MINOR_GRID).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_minor_grid_color()
      { //! \return Color of Y minor grid lines.
        return image_.g(detail::PLOT_Y_MINOR_GRID).style().stroke_color();
      }

      svg_2d_plot& svg_2d_plot::y_major_tick_color(const svg_color& col)
      { //! Set color of Y major tick lines.
        image_.g(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_major_tick_color()
      { //! \return Color of Y major tick lines.
        return image_.g(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color();
      }

      svg_2d_plot& svg_2d_plot::y_minor_tick_color(const svg_color& col)
      {  //! Set color of Y minor tick lines.
        image_.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_minor_tick_color()
      { //! \return Color of Y minor tick lines.
        return image_.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_color();
      }

      const std::string svg_2d_plot::y_axis_position()
      { //! \return Text information about Y-axis position.
        switch(y_axis_position_)
        {
        case left:
          return "y_axis_position left (all X values < 0)"; break;
        case y_intersects_x:
          return "y_axis_position intersects X-axis (X range includes zero)"; break;
        case right:
          return "y_axis_position right (all X values > 0)"; break;
        default:
          return "?"; break;
        }
      } // const std::string svg_2d_plot::y_axis_position()

      svg_2d_plot& svg_2d_plot::y_range(double min_y, double max_y)
      { //! Set the range (max and min) for Y-axis from the parameters provided.
        if (!boost::math::isfinite(min_y))
        {
          throw std::runtime_error("Y range: min not finite!");
        }
        if (!boost::math::isfinite(max_y))
        {
          throw std::runtime_error("Y range: max not finite!");
        }

        if(max_y <= min_y)
        { // max <= min.
          throw std::runtime_error("Y range: y max <= y min!");
        }
        if((max_y - min_y) < std::numeric_limits<double>::epsilon() * 1000)
        { // Range too small to display.
          throw std::runtime_error("Y range too small!" );
        }
        y_axis_.min_ = min_y;
        y_axis_.max_ = max_y;
        y_autoscale_ = false;
       return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      std::pair<double, double> svg_2d_plot::y_range()
      { //! \return The range (max and min) for Y-axis.
        std::pair<double, double> r;
        r.first = y_axis_.min_;
        r.second = y_axis_.max_;
        return r;
      }

      double svg_2d_plot::y_min()
      { //! \return Minimum for Y-axis.
        // Can't check finite-ness and relation to @c y_max.
        return y_axis_.min_;
      }

      double svg_2d_plot::y_max()
      {  //! \return Maximum for Y-axis.
        // Can't check finite-ness and relation to @c y_min.
        return y_axis_.max_;
      }

      bool svg_2d_plot::y_autoscale()
      { //! \return @c true if to autoscale minimum and maximum for Y-axis.
        return y_autoscale_;
      }

      svg_2d_plot& svg_2d_plot::y_autoscale(bool b)
      { //! Set @c true if to autoscale minimum and maximum for Y-axis.
        y_autoscale_ = b;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_values_on()
      { /*! \return @c true if values of Y data points are shown (for example: 1.23).

        (Will override xy_values_on that would otherwise cause overwriting).
        So the last values_on setting will prevail.
        */
        return y_values_on_;
      }

      svg_2d_plot&  svg_2d_plot::y_values_on(bool b)
      { //! Set @c true if values of Y data points are shown (for example: 1.23, 2.34).
        if(xy_values_on() == true)
        { // Would be overwritten by XY pair.
          xy_values_on(false);
        }
        y_values_on_ = b;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::xy_values_on()
      { //! \return @c true if values of X and Y data points are shown (for example: 1.23).
        return xy_values_on_;
      }

      svg_2d_plot&  svg_2d_plot::xy_values_on(bool b)
      { /*! Set @c true if values of X and Y data points are to be shown (as 1.23).

        (Will override x_values_on and/or y_values_on that would otherwise cause overwriting).
          */
        if(x_values_on())
        { // Would be overwritten by XY pair.
          x_values_on(false);
        }
        if(y_values_on())
        { // Would be overwritten by XY pair.
          y_values_on(false);
        }
        xy_values_on_ = b;
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_plusminus_on()
      {//! \return @c true if values of Y data points are to include uncertainty estimates.
        return y_values_style_.plusminus_on_;
      }

      svg_2d_plot& svg_2d_plot::y_plusminus_on(bool b)
      { //! Set true if values of Y data points are to include uncertainty estimates.
        y_values_style_.plusminus_on_ = b;
        return *this;
      } //! \return Reference to svg_2d_plot to make chainable.

      svg_2d_plot& svg_2d_plot::y_plusminus_color(const svg_color& col)
      {  //! Set color of Y uncertainty of value.
        y_values_style_.plusminus_color_ = col;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_plusminus_color()
      { //! \return Color of Y uncertainty of value.
        return y_values_style_.plusminus_color_;
      }

      bool svg_2d_plot::y_addlimits_on()
      {//! \return @c true if values of Y data points are to include confidence interval.
        return y_values_style_.addlimits_on_;
      }

      svg_2d_plot& svg_2d_plot::y_addlimits_on(bool b)
      { //! Set true if values of Y data points are to include confidence interval.
        y_values_style_.addlimits_on_ = b;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      } 

      svg_2d_plot& svg_2d_plot::y_addlimits_color(const svg_color& col)
      {  //! Set color of Y confidence interval.
        y_values_style_.addlimits_color_ = col;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_addlimits_color()
      { //! \return Color of Y confidence interval.
        return y_values_style_.addlimits_color_;
      }

      bool svg_2d_plot::y_df_on()
      { //! \return @c true if values of Y data points are to include degrees of freedom estimates.
        return svg_2d_plot::y_values_style_.df_on_;
      }

      svg_2d_plot& svg_2d_plot::y_df_on(bool b)
      { //! Set @c true if values of Y data points are to include degrees of freedom estimates.
        y_values_style_.df_on_ = b;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      svg_2d_plot& svg_2d_plot::y_df_color(const svg_color& col)
      {  //! Set color of Y degrees of freedom.
        y_values_style_.df_color_ = col;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const svg_color svg_2d_plot::y_df_color()
      { //! \return Color of Y degrees of freedom.
        return y_values_style_.df_color_;
      }

      /*! \brief Set prefix, separator and suffix for Y-axis.\n @b Example:


     
\code         
my_1d_plot.x_decor("[ x = ", "", "&#x00A0;sec]");
\endcode
       \note If you want a space, you must use a @b Unicode space.
      */     
      svg_2d_plot& svg_2d_plot::y_decor(const std::string& pre, const std::string& sep, const std::string& suf)
      {
        y_values_style_.prefix_ = pre;
        y_values_style_.separator_ = sep;
        y_values_style_.suffix_ = suf;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const std::string svg_2d_plot::y_prefix()
      { //! Get the prefix (only used if separator != "")
        return y_values_style_.prefix_;
      }

      const std::string svg_2d_plot::y_suffix()
      { //! Get the suffix (only used if separator != "")
        return y_values_style_.suffix_;
      }

      const std::string svg_2d_plot::y_separator()
      { //! Get separator (also controls use of the prefix & suffix - they are only used if separator != "").
        //! \note For a space, you must use a Unicode space \code "&#x00A0;" \endcode
        //! rather than " ".
        return y_values_style_.separator_;
      }

      svg_2d_plot& svg_2d_plot::y_autoscale(double minimum, double maximum)
      { /*! Set minimum & maximum Y values to use to autoscale Y-axis.
        \param minimum Value to use for autoscale that will be rounded down. 
        \param maximum Value to use for autoscale that will be rounded up. 
        */
        scale_axis(minimum, maximum, // double min and max from two doubles.
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_, autoscale_plusminus_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true;  // Default becomes to use these calculated values.
        return *this; //! \return reference to svg_2d_plot to make chainable.
      } // autoscale(pair<double, double> p)

      svg_2d_plot& svg_2d_plot::y_autoscale(std::pair<double, double> p)
      { //! Set Y min & max values as a \b pair to use to autoscale.
        scale_axis(p.first, p.second, // double min and max from pair.
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_, autoscale_plusminus_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true;  // Default to use calculated values.
        return *this; //! \return reference to svg_2d_plot to make chainable.
      } // autoscale(pair<double, double> p)

      template <typename T> //! \tparam T an STL container: array, vector ...
      svg_2d_plot& svg_2d_plot::y_autoscale(const T& begin, const T& end)
      { //! Data series using iterator's range to use to calculate autoscaled values.
        scale_axis(begin, end,
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_, autoscale_plusminus_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true; // Default to use calculated values.
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      template <typename T> // T an STL container: array, vector ...
      svg_2d_plot& svg_2d_plot::y_autoscale(const T& container)
      { //! Whole data series to use to calculate autoscaled values.
        scale_axis(container.begin(), container.end(), // All the container.
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_, autoscale_plusminus_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true;  // Default to use calculated values.
        return *this;
      }

      template <typename T> // T a 2D STL container: array, vector ...
      svg_2d_plot& svg_2d_plot::xy_autoscale(const T& container)
      { //! Whole data series to use to calculate autoscaled values for \b both X and Y axes.
        scale_axis(container, // All the container.
          &x_auto_min_value_, &x_auto_max_value_, &x_auto_tick_interval_, &x_auto_ticks_,
          &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
          autoscale_check_limits_, autoscale_plusminus_,
          x_include_zero_, x_tight_, x_min_ticks_, x_steps_,
          y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
          x_autoscale_ = true; // Default to use calculated values.
          y_autoscale_ = true; // Can be switch off with autoscale(false);
        return *this; //! \return reference to svg_2d_plot to make chainable.
      } // xy_autoscale

      bool svg_2d_plot::xy_autoscale()
      { //! \return @c true if to autoscale both X and Y Axes.
        return y_autoscale_ && x_autoscale_;
      }

      svg_2d_plot& svg_2d_plot::y_major_interval(double inter)
      { //! Set major interval between ticks on Y-axis.
        y_ticks_.major_interval_ = inter;
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_major_interval()
      { //! \return major interval between ticks on Y-axis.
        return y_ticks_.major_interval_;
      }

      svg_2d_plot& svg_2d_plot::y_major_tick_length(double length)
      { //! Set major tick length on Y-axis.
        y_ticks_.major_tick_length_ = length;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_major_tick_length()
      { //! \return Major tick length on Y-axis.
        return y_ticks_.major_tick_length_;
      }

      svg_2d_plot& svg_2d_plot::y_minor_tick_length(double length)
      { //! Set minor tick length on Y-axis.
        y_ticks_.minor_tick_length_ = length;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_minor_tick_length()
      { //! \return Minor tick length on Y-axis.
        return y_ticks_.minor_tick_length_;
      }

      svg_2d_plot& svg_2d_plot::y_num_minor_ticks(unsigned int num)
      { //! Set number of minor ticks on Y-axis.
        y_ticks_.num_minor_ticks_ = num;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      unsigned int svg_2d_plot::y_num_minor_ticks()
      { //! \return Number of minor ticks on Y-axis.
        return y_ticks_.num_minor_ticks_;
      }

      svg_2d_plot& svg_2d_plot::y_label_axis(const std::string& str)
      { //! Set text to label Y-axis.
        y_label_info_.text(str);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      std::string svg_2d_plot::y_label_axis()
      { //! \return text to label Y-axis.
        return y_label_info_.text();
      }

      svg_2d_plot& svg_2d_plot::y_major_tick_width(double width)
      { //! Set width of major ticks on Y-axis.
        y_ticks_.major_tick_width_ = width;
        image_.g(detail::PLOT_Y_MAJOR_TICKS).style().stroke_width(width);
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_major_tick_width()
      { //! \return Width of major ticks on Y-axis.
        return y_ticks_.major_tick_width_;
      }

      svg_2d_plot& svg_2d_plot::y_minor_tick_width(double width)
      { //! Set width of minor ticks on Y-axis.
        y_ticks_.minor_tick_width_ = width;
        image_.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_width(width);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_minor_tick_width()
      { //! \return Width of minor ticks on Y-axis.
        return y_ticks_.minor_tick_width_;
      }

      svg_2d_plot& svg_2d_plot::x_ticks_on_window_or_axis(int side)
      { // Set if ticks on the plot window or on the X-axis.
        //! \param side -1 ticks downward, 0 no ticks, +1 ticks upward.
        x_ticks_.ticks_on_window_or_on_axis_ = side;
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::x_ticks_on_window_or_axis()
      { //! \return if ticks on the plot window or on the X-axis.
        return x_ticks_.ticks_on_window_or_on_axis_;
      }

      svg_2d_plot& svg_2d_plot::x_major_labels_side(int side)
      { /*! Set which side (up, down or none) for major ticks label values:
          \param side -1 labels downward, 0 no labels, +1 labels upward.
        */
        x_ticks_.major_value_labels_side_ = side;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::x_major_labels_side()
      { //! \return side for label values for major ticks.
        return x_ticks_.major_value_labels_side_;
      }

      svg_2d_plot& svg_2d_plot::y_ticks_on_window_or_axis(int cmd)
      { /*!  Set Y ticks on window or axis
          \param cmd -1 left of plot window, 0 on Y-axis, +1 right of plot window.
        */
        y_ticks_.ticks_on_window_or_on_axis_ = cmd;
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_ticks_on_window_or_axis()
      { //! \return true if Y-axis ticks wanted on the window (rather than on axis).\n
        //! -1 left of plot window, 0 on Y-axis, +1 right of plot window.
        return y_ticks_.ticks_on_window_or_on_axis_;
      }

      svg_2d_plot& svg_2d_plot::y_ticks_left_on(bool cmd)
      { //! Set true if ticks on the Y-axis are to be on left of axis line.
        y_ticks_.left_ticks_on_ = cmd;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_ticks_left_on()
      { //! \return true if ticks on the Y-axis are to be on left of axis line.
        return y_ticks_.left_ticks_on_;
      }

      svg_2d_plot& svg_2d_plot::y_ticks_right_on(bool cmd)
      { //! Set true if ticks on the Y-axis are to be on right of axis line.
        y_ticks_.right_ticks_on_ = cmd;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_ticks_right_on()
      { //! \return true if ticks on the Y-axis are to be on right of axis line.
        return y_ticks_.right_ticks_on_;
      }
      // Only need y_ticks_left_on & y_ticks_right_on in 2D plots.

      svg_2d_plot& svg_2d_plot::y_major_grid_on(bool is)
      { //! Set true to include major grid lines.
        y_ticks_.major_grid_on_ = is;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_major_grid_on()
      { //! \return true to include major grid lines.
        return y_ticks_.major_grid_on_;
      }

      svg_2d_plot& svg_2d_plot::y_minor_grid_on(bool is)
      {//! Set true to include minor grid lines.
        y_ticks_.minor_grid_on_ = is;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      bool svg_2d_plot::y_minor_grid_on()
      {//! Set true to include minor grid lines.
        return y_ticks_.minor_grid_on_;
      }

      svg_2d_plot& svg_2d_plot::y_minor_grid_width(double width)
      { //! Set width of minor grid lines.
        y_ticks_.minor_grid_width_ = width;
        image_.g(detail::PLOT_Y_MINOR_GRID).style().stroke_width(width);
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_minor_grid_width()
      {//! \return Width of minor grid lines.
        return y_ticks_.minor_grid_width_;
      }

      svg_2d_plot& svg_2d_plot::y_major_grid_width(double width)
      { //! Set width of major grid lines.
        y_ticks_.major_grid_width_ = width;
        image_.g(detail::PLOT_Y_MAJOR_GRID).style().stroke_width(width);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      double svg_2d_plot::y_major_grid_width()
      {//! Set width of major grid lines.
        return y_ticks_.major_grid_width_;
      }

      svg_2d_plot& svg_2d_plot::y_label_font_size(unsigned int i)
      { //! Set Y-axis label text font size.
        // May be best to tie label & unit font sizes together?
        // y_axis_label_style_.font_size(i);
        y_units_info_.textstyle().font_size(i);
        y_label_info_.textstyle().font_size(i);
        return *this;
      }

      unsigned int svg_2d_plot::y_label_font_size()
      { //! \return Y-axis label text font size.
        // return y_axis_label_style_.font_size();
        return y_label_info_.textstyle().font_size();
      }

      svg_2d_plot& svg_2d_plot::y_label_weight(std::string s)
      { //! Set Y-axis label text font weight (for example: "bold").
        //! ("bold" is only one that works so far, and quality may be poor for some browsers).
        //y_axis_label_style_.font_weight(s);
        y_label_info_.textstyle().font_weight(s);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const std::string& svg_2d_plot::y_label_weight()
      { //! \return Y-axis label text font weight (for example: "bold").
        // return @c y_axis_label_style_.font_weight();
        return y_label_info_.textstyle().font_weight();
      }

      svg_2d_plot& svg_2d_plot::y_label_font_family(const std::string& family)
      { /*! Set Y-axis label text font family (for example: "Lucida Sans Unicode").
          Available fonts depend on the program rendering the SVG XML, usually a browser.
          The default font (usually "Lucida Sans Unicode") is used
          if a renderer (in a browser or a converter to PDF like RenderX)
          does not provide the font specified.
          A Unicode font has a better chance of providing Unicode symbols, for example, specified as @c \&\#x221E;.
          These fonts are probably usable:
          \code
            "arial", "impact", "courier", "lucida console",  "Lucida Sans Unicode", "Verdana", "calibri", "century",
            "lucida calligraphy", "tahoma", "vivaldi", "informal roman", "lucida handwriting", "lucida bright", "helvetica"
          \endcode
        */
        y_axis_label_style_.font_family(family);
        y_label_info_.textstyle().font_family(family);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const std::string& svg_2d_plot::y_label_font_family()
      { //! \return Font family for label on Y-axis.
        // return y_axis_label_style_.font_family();
        return y_label_info_.textstyle().font_family();
      }

      // Y-axis tick value labels style.

      svg_2d_plot& svg_2d_plot::y_ticks_values_color(const svg_color& col)
      { //! Set color for Y_axis tick values.
        // Function could set both fill (middle) and stroke (outside),
        // but just setting fill if simplest,
        // but does not allow separate inside & outside colors.
        y_ticks_.values_color_ = col;
        image_.g(detail::PLOT_Y_TICKS_VALUES).style().fill_color(col);
        // Setting stroke seems to produce fuzzy letters.
        //svg_2d_plot().image_.g(PLOT_Y_POINT_VALUES).style().stroke_color(col);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      svg_color svg_2d_plot::y_ticks_values_color()
      { //! \return color for Y-axis ticks values.
        // Function could greturn either fill and stroke,
        // image_.g(detail::PLOT_Y_TICKS_VALUES).style().fill_color();
        return y_ticks_.values_color_;
      }

      // Y values
      svg_2d_plot& svg_2d_plot::y_ticks_values_precision(int p)
      { //! Set @c std::iostream decimal digits precision of ticks Y values.
         y_ticks_.value_precision_ = p;
         return *this;
      }

      int svg_2d_plot::y_ticks_values_precision()
      { //! \return  @c std::iostream decimal digits precision of ticks Y values..
        return y_ticks_.value_precision_;
      }

      svg_2d_plot& svg_2d_plot::y_ticks_values_ioflags(std::ios_base::fmtflags f)
      { //! Set @c std::iostream format flags of ticks Y values.
        //! Useful to set hexadecimal, fixed and scientific, (std::ios::scientific).
        y_ticks_.value_ioflags_ = f;
        return *this;
      }

      std::ios_base::fmtflags svg_2d_plot::y_ticks_values_ioflags()
      { //! \return  @c std::iostream format flags of ticks Y values.
        //! Might be used to set hexadecimal, fixed and scientific, (std::ios::scientific).
        return y_ticks_.value_ioflags_;
      }

      svg_2d_plot& svg_2d_plot::y_ticks_values_font_size(unsigned int i)
      { //! Set font size for Y-axis ticks values (svg units, default pixels).
        y_ticks_.value_label_style_.font_size(i);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      unsigned int svg_2d_plot::y_ticks_values_font_size()
      {//! \return Font size for Y-axis values.
        // return derived().y_ticks_.value_label_style_.font_size();
        return y_ticks_.value_label_style_.font_size();
      }

      svg_2d_plot& svg_2d_plot::y_ticks_values_font_family(const std::string& family)
      { /*! Set font family for Y-axis ticks values.
          Available fonts depend on the program rendering the SVG XML, usually a browser.
         The default font (usually "verdana") is used if a render program does not provide the font specified.

          These are probably usable:
        */

        /*!         
          \code
"arial", "impact", "courier", "lucida console",  "Lucida sans unicode", "verdana", "calibri", "century",
"lucida calligraphy", "tahoma", "vivaldi", "informal roman", "lucida handwriting", "lucida bright", "helvetica"
          \endcode
        */

        y_ticks_.value_label_style_.font_family(family);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const std::string& svg_2d_plot::y_ticks_values_font_family()
      { //! \return font family for Y-axis ticks values.
        return y_ticks_.value_label_style_.font_family();
      }

      // Y-axis values.
      svg_2d_plot& svg_2d_plot::y_values_font_size(unsigned int i)
      { //! Set font size for Y-axis values.
        y_values_style_.values_text_style_.font_size(i);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      unsigned int svg_2d_plot::y_values_font_size()
      {//! \return Font size for Y-axis values.
        return y_values_style_.values_text_style_.font_size();
      }

      svg_2d_plot& svg_2d_plot::y_values_font_family(const std::string& family)
      { //! Set font family for Y-axis values.
        //! Available fonts depend on the program rendering the SVG XML, usually a browser.
        //! The default font (usually "verdana") is used if a render program does not provide the font specified.
        //! These are probably usable:
        //! "arial", "impact", "courier", "lucida console",  "Lucida sans unicode", "verdana", "calibri", "century",
        //! "lucida calligraphy", "tahoma", "vivaldi", "informal roman", "lucida handwriting", "lucida bright", "helvetica"
        y_values_style_.values_text_style_.font_family(family);
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      const std::string& svg_2d_plot::y_values_font_family()
      { //! \return Font family for Y-axis values.
        return y_values_style_.values_text_style_.font_family();
      }

      svg_2d_plot& svg_2d_plot::y_values_color(const svg_color& col)
      { //! Set color for Y-axis values.
        // Function could set both fill (middle) and stroke (outside),
        // but just setting fill if simplest,
        // but does not allow separate inside & outside colors.
        image_.g(detail::PLOT_Y_POINT_VALUES).style().fill_color(col);
        //svg_2d_plot().image_.g(PLOT_Y_POINT_VALUES).style().stroke_color(col);
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      svg_color svg_2d_plot::y_values_color()
      { //! \return Color for Y-axis values.
        // Function could get either fill and stroke color,
        // return svg_2d_plot().image_.g(PLOT_Y_POINT_VALUES).style().stroke_color();
        return image_.g(detail::PLOT_Y_POINT_VALUES).style().fill_color();
      }

      svg_2d_plot& svg_2d_plot::y_values_rotation(rotate_style rotate)
      { //! Set rotation for value labels on Y-axis ticks.
        //! \see rotate_style
        y_values_style_.value_label_rotation_ = rotate;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_values_rotation()
      { //! \return rotation for value labels on Y-axis.
        return y_values_style_.value_label_rotation_;
      }

      svg_2d_plot& svg_2d_plot::y_values_precision(int p)
      { //! Set @c iostream precision for data points Y values.
        y_values_style_.value_precision_ = p;
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      }

      int svg_2d_plot::y_values_precision()
      { //! \return @c iostream precision for data points Y values.
        return y_values_style_.value_precision_;
      }

      svg_2d_plot& svg_2d_plot::y_values_ioflags(std::ios_base::fmtflags f)
      { //! Set @c iostream format flags for data point values.
        y_values_style_.value_ioflags_ = f;
        return *this; //! \return reference to svg_2d_plot to make chainable.
      }

      std::ios_base::fmtflags svg_2d_plot::y_values_ioflags()
      { 
        return y_values_style_.value_ioflags_;  //! \return @c std::iostream format flags for data point values.
      }

      svg_2d_plot& svg_2d_plot::write(const std::string& file)
      { //!  Write the plot image to a named file (default suffix .svg, added if no type already appended to file name).
        std::string filename(file); // Copy to avoid problem with const if try to append.
        if (filename.find(".svg") == std::string::npos)
        { // No file type suffix, so provide the default .svg.
          filename.append(".svg");
        }
        std::ofstream fout(filename.c_str());
        if(fout.fail())
        {
          throw std::runtime_error("Unable to open " + filename);
        }
        write(fout); // Using the ostream version of write.
        return *this; //! \return Reference to svg_2d_plot to make chainable.
      } // write(file)

   /*! \brief Add a container of a data series to the plot.\n
       (Version converting to Meas using double with pair_double_2d_convert).\n @b Example:
      \code
my_plot.plot(data1, "Sqrt(x)");
      \endcode
      \note This version assumes that @b ALL the data values in the container is used.
       \tparam T Type of data in series (must be convertible to Meas).
    */
  template <typename T>
  svg_2d_plot_series& svg_2d_plot::plot(const T& container, const std::string& title)
  {
    serieses_.push_back(
      svg_2d_plot_series(
      boost::make_transform_iterator(container.begin(), boost::svg::detail::pair_Meas_2d_convert<false>()),
      boost::make_transform_iterator(container.end(), boost::svg::detail::pair_Meas_2d_convert<false>()),
      title)
    );
    return serieses_[serieses_.size()-1]; //! \return Reference to data series just added to make chainable.
  }

  /*! Add a container of a data series to the plot.\n
     This version permits a custom functor (rather than default conversion to @c double).\n
     \note that this version assumes that @b ALL the data values in the container is used.
    */  template <typename T, typename U>
  svg_2d_plot_series& svg_2d_plot::plot(const T& container, const std::string& title /* = "" */, U functor /* = pair_double_2d_convert*/)
  {
    serieses_.push_back(
      svg_2d_plot_series(
      boost::make_transform_iterator(container.begin(), functor),
      boost::make_transform_iterator(container.end(),   functor),
      title)
    );
    return serieses_[serieses_.size()-1]; //! \return Reference to data series just added to make chainable.
  }

 /*! \brief Add a data series to the plot (by default, converting automatically to @c unc doubles).\n
      This version permits @b part of the container to be used, a partial range, using iterators begin to end.\n
      For example:
      \code
my_2d_plot.plot(my_data.begin(), my_data.end(), "My container");
      \endcode

      \code
my_2d_plot.plot(&my_data[1], &my_data[3], "my_data 1 to 3"); // Add part of data series.
      \endcode
   */
  template <typename T>
  svg_2d_plot_series& svg_2d_plot::plot(const T& begin, const T& end, const std::string& title)
  {
#ifdef _MSC_VER
    serieses_.push_back
    (
      svg_2d_plot_series(
      boost::make_transform_iterator(begin, boost::svg::detail::unc_1d_convert() ), // Xs
      boost::make_transform_iterator(end, boost::svg::detail::unc_1d_convert() ), // Ys
      title)
    );

#endif // _MSC_VER

    return serieses_[serieses_.size() - 1]; //! \return Reference to data series just added to make chainable.
  } // plot(const T& begin, const T& end, const std::string& title = "")

/* Add (part of) a container of a data series to the plot, using a functor.
      This version permits part of the container to be used, a partial range, using iterators begin to end.\n
      Version with custom functor, rather than automatically converting to double).
    */
  template <typename T, typename U>
  svg_2d_plot_series& svg_2d_plot::plot(const T& begin, const T& end, const std::string& title, U functor)
  {
    serieses_.push_back(
      svg_2d_plot_series(
      boost::make_transform_iterator(begin, functor),
      boost::make_transform_iterator(end,   functor),
      title)
    );
    return serieses_[serieses_.size() - 1]; //! \return Reference to data series just added to make chainable.
  }

/*! Write the SVG image to a @c std::ostream.
*/
   svg_2d_plot& svg_2d_plot::write(std::ostream& s_out)
   {
     update_image();
     image_.write(s_out); // Use the std::ostream version of write.
     return *this; //! \return Reference to svg_2d_plot to make chainable.
   }

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

  } // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_2D_PLOT_HPP
