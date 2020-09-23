 /*!  \file
  \brief SVG Plot functions common to 1D, 2D and Boxplots.
  \details Functions are derived from base class @c axis_plot_frame.
  \details

  #define BOOST_SVG_LEGEND_DIAGNOSTICS for diagnostics of plot legend.
  #define BOOST_SVG_TITLE_DIAGNOSTICS for diagnostics of plot title.
  #define BOOST_SVG_POINT_DIAGNOSTICS for diagnostics of data point markers.

  \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009, 2012, 2013, 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Diagnostic output switched on by defining
// BOOST_SVG_DIAGNOSTICS  // General.
// BOOST_SVG_POINT_DIAGNOSTICS  // For plot point.
// BOOST_SVG_LEGEND_DIAGNOSTICS // For legend box.

#ifndef BOOST_SVG_AXIS_PLOT_FRAME_HPP
#define BOOST_SVG_AXIS_PLOT_FRAME_HPP

#include <boost/svg_plot/svg_style.hpp>
#include <boost/svg_plot/svg.hpp>
#include <boost/svg_plot/detail/svg_tag.hpp>
#include <boost/svg_plot/detail/numeric_limits_handling.hpp>
// using boost::math::fpclassify for
// boost::math::
// template <class T>bool isfinite (T);
// template <class T>bool isinf (T);
// template <class T> bool isnan (T);
#include <boost/svg_plot/detail/auto_axes.hpp>
#include <boost/quan/unc.hpp>
// using boost::svg::unc;
#include <boost/quan/meas.hpp>

#include <limits>
#include <string>
// using std::string;

#include <iostream> // for testing only.
 //using std::cerr;
 //using std::cout;
 //using std::endl;

#include <sstream>
//using std::ostream;
#include <utility>
// using std::pair

namespace boost
{
  namespace svg
  {
    // JV: G++ can't resolve names in these enums in axis_plot_frame
    //    when they are in svg_2d_plot.hpp

    /*! Number of standard deviations used for text_plusminus text display.\n
      Nominal factor of 2 (strictly 1.96) corresponds to 95% confidence limit.
    */
    static const double text_plusminus = 2.;

    static const double sin45 = 0.707; //!< Used to calculate 'length' if axis value labels are sloping.
    static const double reducer = 0.9; //!< To make uncertainty and degrees of freedom estimates a bit smaller font to help distinguish from value.
 //   static const double aspect_ratio = 0.6;  //!< Guess at average height to width of font. Now in svg_style.hpp

    // x_axis_position_ and y_axis_position_ use x_axis_intersect & y_axis_intersect
    enum x_axis_intersect
    { //! \enum x_axis_intersect If and how the X axes intersects Y axis.
      bottom = -1, //!< X-axis free below bottom of end of Y-axis (case of all Y definitely < 0).
      x_intersects_y = 0, //!< x_intersects_y when Y values include zero, so X intersects the Y axis.
      top = +1 //!< X-axis free above top of X-axis (case of all Y definitely > 0).
      };

    enum y_axis_intersect
    { //! \enum y_axis_intersect  If and how the Y axes intersects X axis.
      left = -1, //!< Y-axis free to left of end of X-axis (case of all X definitely < 0).
      y_intersects_x = 0, //!< y_intersects_x when X values include zero, so intersects the X axis.
      right = +1 //!< Y-axis free to left of end of X-axis (case of all X definitely > 0).
     };

    enum legend_places
    { //! \enum legend_places Placing of legend box, if requested by legend_on == true.
      nowhere = 0, //!< Placing of legend box not requested or not calculated yet.
      inside = -1,  //!< Default place for inside is top left of plot window, (exact location controlled by legend_top_left()).
      outside_left = 1, //!< Outside on the left of the graph.
      outside_right = +2, //!< Outside right (Default).
      outside_top = +3, //!< Outside at top.
      outside_bottom = +4, //!< Outside at bottom.
      somewhere = +5 //!< legend_top_left(x, y)
    };

    namespace detail
    { //! \namespace detail Holds base class axis_plot_frame for 1D, 2D and Box plots.

      /*! \brief Used as base class for 1D, 2D and Box plots.
         \details For example:

         \code
         svg_1d_plot, svg_2d_plot, svg_boxplot\n
         class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>\n
         class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
         \endcode

       All set functions return `derived() == *this` to permit chaining,
       for example:

       \code
         my_plot.background_color(pink)
         .background_border_color(blue)
         .copyright_holder("Paul A. Bristow);
         ...
       \endcode

       Shapes and glyphs can have (or may need) BOTH fill and stroke to be set.
       Both are usually the same in this application.
       If both are set, stroke is considered 'more important',
       and so is returned by all get functions.
       */

      // Max and min three item functions use to compare font sizes.
      template <typename T>
      inline
        T max(T a, T b, T c)
      { //! \return maximum of three items, similar to std::max for two items.
        return ((a > b) ? a : (b > c) ? b : c);
      }

      template <typename T>
      inline
        T min(T a, T b, T c)
      { //! \return minimum of three items, similar to std::min for two items.
        return ((a < b) ? a : (a < c) ? a : c);
      }

      template <class Derived>
      class axis_plot_frame
      {
      protected:
        // We don't use the SVG coordinate transform because then text would
        // be flipped. Might use it to scale the image for resizes.

        // Protected (?) Member Functions Declarations (defined below):
//! \cond DETAIL

         void transform_point(double &x, double &y); // Scale & shift both X & Y to graph Cartesian coordinates.
         void transform_x(double &x); // Scale & shift both X to graph Cartesian coordinates.
         void transform_y(double &y); // Scale & shift both Y to graph Cartesian coordinates.
         void draw_x_minor_tick(double j, path_element& tick_path, path_element& grid_path); // (& grid).
         void draw_x_major_tick(double i, path_element& tick_path, path_element& grid_path); // (& grid).
         void draw_x_axis();
         void draw_legend();
         void size_legend_box();  // Compute the size of the legend box from fonts, title and marker points and text.
         void place_legend_box();
         void draw_title();
         void adjust_limits(double& x, double& y);
         void draw_x_axis_label();
         void draw_plot_point(double x, double y, g_element& g_ptr, const plot_point_style& sty);
         void draw_plot_point(double x, double y, g_element& g_ptr, plot_point_style& sty, unc<false> ux, unc<false> uy);
         void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, Meas uvalue);
         void draw_plot_point_values(double x, double y, g_element& x_g_ptr, g_element& y_g_ptr, const value_style& x_sty, const value_style& y_sty, Meas uncx, Meas uncy);
         void draw_plot_point_values(double x, double y, g_element& x_g_ptr, g_element& y_g_ptr, const value_style& x_sty, const value_style& y_sty, Meas uncx, unc<false> uncy);

        // Clear functions.
        /*! \brief Clear all layers of the plot.
            \details
              When writing to multiple documents, the contents of the plot
              may change significantly between. Rather than figuring out what
              has and has not changed, just erase the contents of the
              legend, title... in the document and start over.
        */
         void clear_all(); // Calls all the other clear_* functions.

         void clear_legend(); //!< Clear the legend layer of the SVG plot.
         void clear_background(); //!< Clear the whole image background layer of the SVG plot.

         void clear_x_axis(); //!< Clear the X axis layer of the SVG plot.
         void clear_y_axis(); //! Clear the Y axis layer of the SVG plot.
         void clear_title(); //!< Clear the plot title layer of the SVG plot.
         void clear_points(); //!< Clear the data points layer of the SVG plot.
         void clear_plot_background(); //!< Clear the plot area background layer of the SVG plot.
         void clear_grids(); //!< Clear the  grids layer of the SVG plot.

//! \endcond

  public: // Could be private but benefits unclear?

//        private:
          Derived& derived()
          { //! Uses Curiously Recurring Template Pattern to allow 1D and 2D to reuse common code.
            //! See http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern .
            //! Sadly this has the most unwelcome effect of terminally confusing the Visual Studio Intellisense,
            //! and sometimes the debugger as well :-(
            return static_cast<Derived&>(*this);
          }
          const Derived& derived()const
          {  //! const version of derived()
            return static_cast<const Derived&>(*this);
          }
        public:
          // Set & get member function Declarations:


          // Note that Doxygen needs the comments here with the declarations,
          // because comments with the definitions seem to be ignored.

          Derived& size(int x, int y); //!<  Set SVG image size (SVG units, default pixels).
          std::pair<double, double> size(); //!<  \return SVG image size, both horizontal width and vertical height (SVG units, default pixels).

          Derived& x_size(int i); //!< Set SVG image X-axis size (SVG units, default pixels).
          int x_size(); //!<  \return SVG image X-axis size as horizontal width (SVG units, default pixels).
          int image_x_size(); //!<  Obselete - deprecated use x_size()
          Derived& image_x_size(int i); //!<  Obselete - deprecated - use x_size().

          int y_size(); //!<  \return SVG image Y-axis size as vertical height (SVG units, default pixels).
          Derived& y_size(int i); //!<  Set SVG image Y-axis size (SVG units, default pixels).
          int image_y_size(); //!<  Obselete - deprecated - use y_size()
          Derived& image_y_size(int i); //!<  Obselete - deprecated - use y_size()

          svg_color background_color(); //!< \return Plot background color.
          Derived& background_color(const svg_color& col); //!<  Set plot background color.
          Derived& background_border_color(const svg_color& col); //!< Set plot background border color.
          svg_color background_border_color(); //!< \return plot background border color.
          Derived& background_border_width(double w); //!< Set plot background border width.
          double background_border_width(); //!<  \return Plot background border width.
          Derived& description(const std::string d); //!<  Writes description to the document for header as \verbatim <desc> My Description </desc>. \endverbatim
          const std::string& description(); //!<  \return  Description of the document for header as \verbatim <desc> My description </desc>. \endverbatim
          Derived& document_title(const std::string d); //!<  Set document title to the document for header as \verbatim <title> My Title </title>. \endverbatim
          std::string document_title(); //!<  \return Document title to the document for header as \verbatim <title> My Title </title>. \endverbatim
          /*! Writes copyright_holder metadata to the SVG document
            (for header as <!-- SVG Plot Copyright Paul A. Bristow 2007 --> )
            /and as metadata: \verbatim <meta name="copyright" content="Paul A. Bristow" /> \endverbatim
            */
          Derived& copyright_holder(const std::string d);
          const std::string copyright_holder(); //!<  \return SVG document copyright holder.
          /*! Writes copyright date to the SVG document.
            and as metadata: \verbatim <meta name="date" content="2007" /> \endverbatim
           */
          Derived& copyright_date(const std::string d);
          const std::string copyright_date(); //!<  \return SVG document copyright_date.
           /*! Set license conditions for reproduction, attribution, commercial use, and derivative works,
            usually "permits", "requires", or "prohibits", and set license_on == true.
          */
          Derived& license(
            std::string repro= "permits",
            std::string distrib = "permits",
            std::string attrib = "requires",
            std::string commercial = "permits",
            std::string derivative = "permits");
          Derived&  license_on(bool l); //!<  Set if license conditions should be included in the SVG document.
          bool license_on(); //!<  \return @c true if license conditions should be included in the SVG document.
          Derived& boost_license_on(bool l); //!<  Set @c true if the Boost license conditions should be included in the SVG document.
          bool boost_license_on();  //!<  \return @c true if the Boost license conditions should be included in the SVG document.
          const std::string license_reproduction(); //!<  \return  SVG document  reproduction license conditions, usually "permits", "requires", or "prohibits".
          const std::string license_distribution(); //!<  \return  SVG document  distribution license conditions, usually "permits", "requires", or "prohibits".
          const std::string license_attribution(); //!<  \return  SVG document  attribution license conditions, usually "permits", "requires", or "prohibits".
          const std::string license_commercialuse();  //!<  \return  SVG document  commercial use license conditions, usually "permits", "requires", or "prohibits".
          Derived& coord_precision(int digits); /*!< Precision of SVG coordinates in decimal digits (default 3).
              3 decimal digits precision is sufficient for small images.
              4 or 5 decimal digits precision will give higher quality plots,
              especially for larger images, at the expense of larger .svg files,
              particularly if there are very many data points.
           */
          int coord_precision(); //!<  \return  precision of SVG coordinates in decimal digits.
          /*! Set precision of X-tick label values in decimal digits (default 3).
              3 decimal digits precision is sufficient for small images.
              4 or 5 decimal digits precision will give more cluttered plots.
              If the range of labels is very small, then more digits will be essential.
           */
          Derived& x_value_precision(int digits);
          int x_value_precision(); //!<  \return Precision of X-tick label values in decimal digits
          Derived& x_value_ioflags(std::ios_base::fmtflags flags); /*!< Set iostream std::ios::fmtflags for X value label (default decimal == 0X201).
              Mainly useful for changing to scientific, fixed or hexadecimal format.
              For example:
              \code
                 myplot.x_value_ioflags(std::ios::dec | std::ios::scientific)
              \endcode
            */
          std::ios_base::fmtflags x_value_ioflags(); //!<  \return  stream std::ios::fmtflags for control of format of X value labels.
          Derived& x_labels_strip_e0s(bool cmd); //!<  Set if to strip redundant zeros, signs and exponents, for example, reducing "1.2e+000" to "1.2"
            //! This markedly reduces visual clutter, and is the default.
          bool y_labels_strip_e0s(); //!<  \return  if to strip redundant zeros, signs and exponents, for example, reducing "1.2e+000" to "1.2"
          /*!
              Set a title for plot.
              The string may include Unicode for greek letter and symbols.
              @b example:
              A title that includes a greek omega and degree symbols, for example:
              \code
                my_plot.title("Plot of &#x3A9; function (&#x00B0;C)");
              \endcode

              Unicode symbols are at http://unicode.org/charts/symbols.html.
            */
          Derived& title(const std::string title); //! \return @c std::string text for the title,
          //! (whose string may include Unicode for greek letter and symbols).
          const std::string title(); //!<  \return Title for plot (whose string may include Unicode for greek letter and symbols).
          text_style& title_style(); //!<  \return All style info for the title, font, famil, size ... (SVG units, default pixels).

          Derived& title_font_size(int i); //!<  Sets the font size for the title (SVG units, default pixels).
          int title_font_size(); //!<  return Font size for the title (SVG units, default pixels). Example: /code std::cout << my_plot.title_font_size() ... \endcode
          Derived& title_font_family(const std::string& family); //!<  Set the font family for the title (for example: .title_font_family("Lucida Sans Unicode");
          const std::string& title_font_family(); //!<  \return Font family for the title.
          Derived& title_font_style(const std::string& style);  //!<  Set the font style for the title (default normal).
          const std::string& title_font_style(); //!<  \return  Font style for the title (default normal).
          Derived& title_font_weight(const std::string& weight); //!<  Set the font weight for the title (default normal).
          const std::string& title_font_weight(); //!<  \return  Font weight for the title.
          Derived& title_font_stretch(const std::string& stretch); //!<  Set the font stretch for the title (default normal), wider or narrow.
          const std::string& title_font_stretch(); //!<  \return  Font stretch for the title.
          Derived& title_font_decoration(const std::string& decoration); //!<  Set the font decoration for the title (default normal, or underline, overline or strike-thru).
          const std::string& title_font_decoration(); //!<  \return Font decoration for the title (default normal, or underline, overline or strike-thru).
          Derived& title_font_rotation(rotate_style rotate); //!<  Set the rotation for the title font (degrees, 0 to 360 in steps using rotate_style, for example horizontal, uphill...
          int title_font_rotation(); //!<  \return  the rotation for the title font (degrees).
          Derived& title_font_alignment(align_style alignment);  //!<  Set the alignment for the title.
          align_style title_font_alignment(); //!<  \return  the alignment for the title.
          Derived& title_text_length(double); //!<  Sets the text_length for the title (SVG units, default pixels).
          double title_text_length(); //!<  return text_length for the title (SVG units, default pixels).
          // Example: /code std::cout << my_plot.title_text_length() ... \endcode

          // Legend.
          Derived& legend_width(double width); //!<  Set the width for the legend box.
          double legend_width(); //!<  \return Width for the legend box.
          Derived& legend_title(const std::string title); //!<  Set the title for the legend.
          const std::string legend_title(); //!<  \return Title for the legend.
          Derived& legend_font_weight(const std::string& weight); //!<  Set the font weight for the legend title.
          const std::string& legend_font_weight(); //!<  \return  Font weight for the legend text.
          Derived& legend_font_family(const std::string& family); //!<  Set the font family for the legend text.
          const std::string& legend_font_family(); //!<  \return  the font family for the legend title.
          Derived& legend_title_font_size(int size); //!<  \return Font size for the legend title.
          int legend_title_font_size(); //!<  \return Font size for the legend title (svg units, default pixels).
          Derived& legend_text_font_size(int size); //!<  Set Font size for the legend text.
          int legend_text_font_size(); //!<  \return Font size for the legend text (svg units, default pixels).
          //Derived& legend_text_color(const svg_color& col); //!<  Set color for the legend text.
          //svg_color legend_text_color(); //!<  \return Color for the legend text.

          Derived& legend_title_font_weight(const std::string& weight); //!<  Set the font weight for the legend title.
          const std::string& legend_title_font_weight(); //!<  \return  Font weight for the legend title.

          Derived & legend_text_font_weight(const std::string & weight);

          const std::string & legend_text_font_weight();

          Derived& legend_top_left(double x, double y); //!<  Set position of top left of legend box (svg coordinates, default pixels).
            //! (Bottom right is controlled by contents, so the user cannot set it).
          const std::pair<double, double> legend_top_left(); //!<  \return  SVG coordinate (default pixels) of top left of legend box.
          const std::pair<double, double> legend_bottom_right(); //!<  \return SVG coordinate (default pixels) of bottom right of legend box.
          Derived& legend_lines(bool is); /*!< Set true if legend should include samples of the lines joining data points.
            This allows different series of data points to be distinguished by different color and/or width.
            This is especially useful to show plots of different functions and/or different parameters in different colors.
            */
          bool legend_lines(); //!< \return  true if legend should include samples of the lines joining data points.
          Derived& legend_on(bool cmd); //!< Set @c true if a legend is wanted.
          bool legend_on(); //!< \return @c true if a legend is wanted.
          Derived& x_axis_vertical(double fraction); //!< Set vertical position of X-axis for 1D as fraction of plot window.
          bool x_axis_vertical(); //!< \return vertical position of X-axis for 1D as fraction of plot window.
          Derived& legend_place(legend_places l); //!<  Set the position of the legend, \see  boost::svg::legend_places
          legend_places legend_place(); //!<  \return  the position of the legend, \see  boost::svg::legend_places
          bool legend_outside(); //!<  \return  if the legend should be outside the plot area.
          //Derived& legend_title_font_size(int size); //!<  Set legend header font size (svg units, default pixels).
          //int legend_title_font_size(); //!<\return  legend header font size (svg units, default pixels).
          Derived& plot_window_on(bool cmd); //!<Set true if a plot window is wanted (or false if the whole image is to be used).
          bool plot_window_on();//!<\return  true if a plot window is wanted (or false if the whole image is to be used).
          Derived& plot_border_color(const svg_color& col); //!<Set the color for the plot window background.
          svg_color plot_border_color(); //!<\return  the color for the plot window background.
          Derived& plot_border_width(double w); //!<Set the margin around the plot window border (svg units, default pixels).
            //! \details This prevents the plot window getting too close to other elements of the plot.
          double plot_border_width(); //!<\return  the width for the plot window border (svg units, default pixels).
          Derived& image_border_margin(double w); //!< Set the margin around the plot window border (svg units, default pixels).
            //! \details This prevents the plot window getting too close to other elements of the plot.
          double image_border_margin(); //!<\return  the margin around the plot window border (svg units, default pixels).
          Derived& image_border_width(double w); //!< Set the svg image border width (svg units, default pixels).
          double image_border_width(); //!<\return  the svg image border width (svg units, default pixels).
          Derived& plot_window_x(double min_x, double max_x);//!< Set the minimum and maximum (cartesian data units) for the plot window X axis.
            //! This is normally calculated from other plot values.
          Derived& plot_window_y(double min_y, double max_y);//!< Set the minimum and maximum (cartesian data units) for the plot window Y axis.
            //! This is normally calculated from other plot values.
          std::pair<double, double> plot_window_x(); //!<\return  both the left and right (X axis) of the plot window.
          double plot_window_x_left(); //!<\return  left of the plot window.
          double plot_window_x_right(); //!<\return  right of the plot window.
          double plot_window_y_top(); //!<\return  top of the plot window.
          double plot_window_y_bottom(); //!<\return  top of the plot window.
          std::pair<double, double> plot_window_y(); //!<\return  both the top and bottom (Y axis) of the plot window.
          double x_minor_interval(); //!<\return  interval between X minor ticks.
          double y_minor_interval(); //!<\return  interval between Y minor ticks.
          Derived& x_ticks_up_on(bool cmd); //!<Set true if X major ticks should mark upwards.
          bool x_ticks_up_on(); //!<\return  true if X major ticks should mark upwards.
          Derived& x_ticks_down_on(bool cmd); //!<Set true if Y major ticks should mark upwards.
          bool x_ticks_down_on(); //!<\return  true if Y major ticks should mark upwards.
          Derived& x_label_on(bool cmd); //!<\return  true if X major ticks should mark downwards.
          bool x_label_on(); /*!< Set true if want to show X-axis label text.
            Also switched on by setting label text.
            (on the assumption that if label text is set, display is also wanted,
            but can be switched off if  \b not required).
           */
          Derived& x_label_font_size(int i); //!<Set X axis label font size (svg units, default pixels).
          int x_label_font_size(); //!<\return  X axis label font size (svg units, default pixels).
          Derived& x_value_font_size(int i); //!<Set X tick value label font size (svg units, default pixels).
           int x_value_font_size(); //!<\return  X tick value label font size (svg units, default pixels).
          Derived& x_label_font_family(const std::string& family); /*!< Set X tick value label font family.
           Available fonts depend on the program rendering the SVG XML, usually a browser.
          The default font (usually "default_font") is used if a render program does not provide the font specified.
          These are probably usable:
          \code
            "arial", "impact", "courier", "lucida console",  "Lucida sans unicode", "verdana", "calibri", "century",
            "lucida calligraphy", "tahoma", "vivaldi", "informal roman", "lucida handwriting", "lucida bright", "helvetica"
          \endcode
          */
          const std::string& x_label_font_family(); //!<\return  X tick value label font family.
          Derived& x_axis_label_color(const svg_color& col); //!<Set X axis label color, for example, red.
          svg_color x_axis_label_color(); //!<\return  X axis label color.
          //!<X-axis ticks values label style.
          Derived& x_ticks_values_color(const svg_color& col);  //!<Set X axis tick value label color.
          svg_color x_ticks_values_color(); //!<\return  X-axis ticks value label color.
          Derived& x_ticks_values_precision(int p); //!<Set iostream decimal digits precision of data point X values near data points markers.
          int x_ticks_values_precision(); //!<\return  iostream decimal digits precision of data point X values near data points markers.
          Derived& x_ticks_values_ioflags(std::ios_base::fmtflags f); //!<Set iostream format flags of data point X values near data points markers.
          std::ios_base::fmtflags x_ticks_values_ioflags(); //!<\return  iostream format flags of data point X values near data points markers.

          Derived& x_ticks_values_font_family(const std::string& family); /*!< Set X ticks value label font family.
            Available fonts depend on the program rendering the SVG XML, usually a browser.
            The default font (usually "default_font") is used if a render program does not provide the font specified.
             These are probably usable:
             \code
 "arial", "impact", "courier", "lucida console",  "Lucida sans unicode", "verdana", "calibri", "century",
 "lucida calligraphy", "tahoma", "vivaldi", "informal roman", "lucida handwriting", "lucida bright", "helvetica"
             \endcode
           */
          const std::string& x_ticks_values_font_family(); //!<\return  X ticks value label font family.
          Derived& x_ticks_values_font_size(int i); //!<Set X ticks value label font size (svg units, default pixels).
          int x_ticks_values_font_size(); //!<Set X ticks value label font size (svg units, default pixels).

          Derived& x_ticks_on_window_or_axis(int side);
            /*!<  Set position of X ticks on window or axis.
              \param side -1 X ticks on bottom of plot window,
                       0 X ticks on X-axis horizontal line,
                       +1 X ticks top of plot window.
            */
          int x_ticks_on_window_or_axis(); //!<\return true if X axis ticks wanted on the window (rather than on axis).\n
            //!<-1 bottom of plot window, 0 on horiztonal X axis , +1 top of plot window.
          Derived& x_label_units_on(bool cmd); //!<Set true if want X axis label to include units (as well as label like "length").
            //!<\see x_label_units which also sets true.
          bool x_label_units_on(); //!<Set true if want X axis label to include units (as well as label like "length").
          Derived& x_major_labels_side(int side); /*!< Position of labels for X major ticks on horizontal X axis line.
               \param side > 0 X tick value labels to left of Y axis line (default), 0 (false) no major X tick value labels on Y axis, 0 X tick labels to right of Y axis line.
            */
          int x_major_labels_side(); //!<\return the side for X ticks major value labels.
          Derived& x_major_label_rotation(rotate_style rot); /*!< Set rotation for X ticks major value labels. (Default horizontal).
               \see rotate_style
            */
          rotate_style x_major_label_rotation(); /*!< \return rotation for X ticks major value labels.
                \see rotate_style
             */
          Derived& title_on(bool cmd); //!<If set true, show a title for the plot. Note: is set true by setting a title.
          bool title_on(); //!<\return true if will show a title for the plot.
          Derived& x_major_grid_on(bool is); //!<If set true, will include a major X-axis grid.
          bool x_major_grid_on();  //!<\return true if will include a major X-axis grid.
          Derived& x_minor_grid_on(bool is); //!<If set true, will include a minor X-axis grid.
          bool x_minor_grid_on();   //!<\return true if will include a major X-axis grid.
          Derived& axes_on(bool is); //!< Set @c true if to draw \b both x and y axes (note plural axes).
          bool axes_on(); //!< \return @c true if to draw \b both x and y axis on.
          Derived& x_axis_on(bool is); //!<If set true, draw a horizontal X-axis line.
          bool x_axis_on(); //!<\return true if will draw a horizontal X-axis line.
          Derived& y_axis_on(bool is); //!<If set true, draw a vertical Y-axis line.
          bool y_axis_on(); //!<\return true if will draw a horizontal X-axis line.
          Derived& title_color(const svg_color& col); //!<Set the color of any title of the plot.
          svg_color title_color(); //!<\return the color of any title of the plot.
          Derived& legend_color(const svg_color& col); //!<Set the color of the title of the legend.
          svg_color legend_color(); //!<\return  the color of the title of the legend.
          Derived& legend_background_color(const svg_color& col); //!<Set the background fill color of the legend box.
          svg_color legend_background_color(); //!<\return  the background fill color of the legend box.
          bool legend_box_fill_on(); //!<\return true if legend box has a background fill color.
          Derived& legend_border_color(const svg_color& col); //!<Set the border stroke color of the legend box.
          svg_color legend_border_color(); //!<\return  the border stroke color of the legend box.
          Derived& plot_background_color(const svg_color& col); //!<Set the fill color of the plot window background.
          svg_color plot_background_color(); //!<\return  the fill color of the plot window background.
          const std::string x_axis_position(); //!<\return  the position (or intersection with Y-axis) of the X-axis.
          Derived& x_axis_color(const svg_color& col); //!<Set the color of the X-axis line.
          svg_color x_axis_color(); //!<\return  the color of the X-axis line.
          Derived& y_axis_color(const svg_color& col); //!<Set the color of the Y-axis line.
          svg_color y_axis_color(); //!<\return  the color of the Y-axis line.
          Derived& x_label_color(const svg_color& col); //!<\return  the color of the Y-axis line.
          svg_color x_label_color(); //!<\return  the color of X-axis label (including any units).
          Derived& x_label_width(double width); //!<Set the width (boldness) of X-axis label (including any units).
            //! (not recommended until browsers implement better).
          double x_label_width(); //!<\return  the width (boldness) of X-axis label (including any units).
          Derived& y_label_color(const svg_color& col); //!<Set the color of Y-axis label (including any units).
          svg_color y_label_color(); //!<\return  the color of Y-axis label (including any units).
          Derived& x_major_tick_color(const svg_color& col); //!<Set the color of X-axis major ticks.
          svg_color x_major_tick_color(); //!<\return  the color of X-axis major ticks.
          Derived& x_minor_tick_color(const svg_color& col); //!<Set the color of X-axis minor ticks.
          svg_color x_minor_tick_color(); //!<\return  the color of X-axis minor ticks.
          Derived& x_major_grid_color(const svg_color& col); //!<Set the color of X-axis major grid lines.
          svg_color x_major_grid_color(); //!<Set the color of X-axis major grid lines.
          Derived& x_major_grid_width(double w); //!<Set the width of X-axis major grid lines.
          double x_major_grid_width(); //!<\return  the color of X-axis major grid lines.
          Derived& x_minor_grid_color(const svg_color& col); //!<Set the color of X-axis minor grid lines.
          svg_color x_minor_grid_color(); //!<\return  the color of X-axis minor grid lines.
          Derived& x_minor_grid_width(double w); //!<Set the width of X-axis minor grid lines.
          double x_minor_grid_width(); //!<\return  the width of X-axis minor grid lines.
          Derived& x_axis_width(double width); //!<Set the width of X-axis lines.
          double x_axis_width(); //!<\return  the width of X-axis lines.
          Derived& data_lines_width(double width); //!<Set the width of lines joining data points.
          double data_lines_width(); //!<\return  the width of lines joining data points.
          Derived& x_label(const std::string& str); //!<Set the text to label the X-axis (and set x_label_on(true)).
          std::string x_label(); //!<\return  the text to label the X-axis.
          Derived& x_label_units(const std::string& str); //!<Set the text to add units to the X-axis label.
          std::string x_label_units(); //!<\return  the text to add units to the X-axis label.
            //!<The label will only be shown if  x_label_on() == true.
          Derived& y_label(const std::string& str); //!<Set the text for the Y-axis label (and set y_label_on(true)).
          std::string y_label(); //!<\return  the text for the Y-axis label.
            //!< The label will only be shown if  y_label_on() == true.
          Derived& y_label_units(const std::string& str); //!<Set the text to add units to the Y-axis label.
          std::string y_label_units(); //!<\return  the text to add units to the X-axis label.
          Derived& x_values_on(bool b); //!<Set true to show data point values near data points markers.
          bool x_values_on(); //!<\return true if to show data point values near data points markers.
          Derived& x_values_font_size(int i); //!<Set font size of data point X values near data points markers.
          int x_values_font_size(); //!<\return  font size of data point X values near data points markers.
          Derived& x_values_font_family(const std::string& family); //!<Set font family of data point X values near data points markers.
          const std::string& x_values_font_family(); //!<\return font family of data point X values near data points markers.
          Derived& x_major_interval(double inter); //!<Set the interval between X-axis major ticks.
          double x_major_interval();  //!<\return  the interval between X-axis major ticks.

          Derived& x_values_color(const svg_color& col); //!<Set the color of data point X values near data points markers.
          svg_color x_values_color(); //!<\return  the color of data point X values near data points markers.
          Derived& x_values_rotation(rotate_style rotate); //!<\return  the rotation (rotate_style) of data point X values near data points markers.
          int x_values_rotation(); //!<Set the rotation (rotate_style) of data point X values near data points markers.
          Derived& x_values_precision(int p); //!<Set iostream decimal digits precision of data point X values near data points markers.
          int x_values_precision(); //!<\return  iostream decimal digits precision of data point X values near data points markers.
          Derived& x_values_ioflags(std::ios_base::fmtflags f); //!<Set iostream format flags of data point X values near data points markers.
          std::ios_base::fmtflags x_values_ioflags(); //!<\return  iostream format flags of data point X values near data points markers.
          Derived& x_plusminus_on(bool b); //!<Set if to append std_dev estimate to data point X values near data points markers.
          bool x_plusminus_on(); //!<\return true if to append std_dev estimate to data point X values near data points markers.
          Derived& x_plusminus_color(const svg_color& col); //!<Set the color of X std_dev of value, for example, the color of 0.02 in "1.23 +-0.02 (9)".
          svg_color x_plusminus_color(); //!<\return the color of X std_dev of value, for example, the color of 0.02 in "1.23 +-0.02 (9)".

          Derived& x_addlimits_on(bool b); //!<Set if to append confidence limits to data point X values near data points markers.
          bool x_addlimits_on(); //!<\return true if to append confidence limits estimate to data point X values near data points markers.
          Derived& x_addlimits_color(const svg_color& col); //!<Set the color of X confidence limits of value, for example, the color  in "<1.23, 1.45>".
          svg_color x_addlimits_color(); //!<\return the color of X confidence limits of value, for example, the color of  "<1.23, 1.45>)".


          Derived& x_df_on(bool b); //!<Set true if to append a degrees of freedom estimate to data point X values near data points markers.
          bool x_df_on(); //!<\return true if to append a degrees of freedom estimate to data point X values near data points markers.
          Derived& x_df_color(const svg_color& col); //!<Set the color of X degrees of freedom, for example, the color of 9 in "1.23 +-0.02 (9)".
          svg_color x_df_color(); //!<\return the color of X degrees of freedom, for example, the color of 9 in "1.23 +-0.02 (9)".
          Derived& x_id_on(bool b); //!<Set true if to append append an ID or name to data point X values near data points markers.
          bool x_id_on(); //!<\return true if to append an ID or name to data point X values near data points markers.
          Derived& x_id_color(const svg_color& col); //!<Set the color of X id or name, for example, the color of text in "my_id".
          svg_color x_id_color(); //!<\return the color of X X id or name, for example, the color of text in "my_id".
          Derived& x_datetime_on(bool b); //!<Set true if to append date time to data point X values near data points markers.
          bool x_datetime_on(); //!<\return true if to append an date time to data point X values near data points markers.
          Derived& x_datetime_color(const svg_color& col); //!<Set the color of X  date time , for example, the color of text in "".
          svg_color x_datetime_color(); //!<\return the color of X date time, for example, the color of text in "".
          Derived& x_order_on(bool b); //!<Set true if to append append an order # to data point X values near data points markers.
          bool x_order_on(); //!<\return true if to append an order # to data point X values near data points markers.
          Derived& x_order_color(const svg_color& col); //!<Set the color of X order #, for example, the color of #42.
          svg_color x_order_color(); //!<\return the color of X order #, for example, the color of #42.
          Derived& x_decor(const std::string& pre, const std::string& sep = "", const std::string& suf = "");
          /*!< Set prefix, separator and suffix together for x_ values.
            Note if you want a space, you must use a Unicode space "&#x00A0;",
            for example, ",&#x00A0;" rather than ASCII space", ".
            If 1st char in separator == \n, then Y values and info will be on a newline below.
          */
          const std::string x_suffix(); //!<\return the suffix (only used if separator != "")
          const std::string x_separator(); //!<\return the separator, perhaps including Unicode.
          const std::string x_prefix(); //!<\return the prefix.

          Derived& x_major_tick_length(double length); //!<Set length of X major ticks (SVG units, default pixels).
          double x_major_tick_length(); //!<Set length of X major ticks (SVG units, default pixels).
          Derived& x_major_tick_width(double width); //!<Set width of X major ticks (SVG units, default pixels).
          double x_major_tick_width(); //!<Set width of X major ticks (SVG units, default pixels).
          Derived& x_minor_tick_length(double length); //!<Set length of X minor ticks (SVG units, default pixels).
          double x_minor_tick_length(); //!<\return  length of X minor ticks (SVG units, default pixels).
          Derived& x_minor_tick_width(double width); //!<Set width of X minor ticks (SVG units, default pixels).
          double x_minor_tick_width(); //!<\return  width of X minor ticks (SVG units, default pixels).
          Derived& x_major_tick(double d); //!<Set interval (Cartesian units) between major ticks.
          double x_major_tick(); //!<\return  interval (Cartesian units) between major ticks.
          Derived& x_minor_interval(double interval); //!<Set interval between X-axis minor ticks.
          Derived& x_num_minor_ticks(int num); //!<Set number of X-axis minor ticks between major ticks.
          int x_num_minor_ticks(); //!<\return  number of X-axis minor ticks between major ticks.
          Derived& x_range(double min_x, double max_x); /*!< Set the range of values on the X-axis.
            The minimum and maximum values must be finite and not too near
            to the minima or maxima that can be represented by floating point double,
            std::numeric_limits<double>::min() or std::numeric_limits<double>::max(),
            and the range must not be too small.
            */
          std::pair<double, double> x_range(); //!<\return  the range of values on the X-axis.
          Derived& x_min(double min_x); //!<Set the minimum value on the X-axis.
          double x_min(); //!<\return  the minimum value on the X-axis.
          Derived& x_max(double x); //!<Set the maximum value on the X-axis.
          double x_max(); //!<\return  the maximum value on the X-axis.

          //!<autoscale set & get parameters,
          //!<Note: all these *MUST* preceed x_autoscale(data) call.
          Derived& autoscale_plusminus(double); //!< Set how many std_dev or standard deviations to allow for ellipses when autoscaling.
          double autoscale_plusminus(); //!< \return How many std_dev or standard deviations allowed for ellipses when autoscaling.

          Derived& confidence(double); //!<Set confidence alpha for display of confidence intervals (default 0.05 for 95%).
          double confidence(); //!<\return Confidence alpha for display of confidence intervals (default 0.05 for 95%).

          /*! Set @c true to check that values used for autoscale are within limits.
            Default is @c true, but can switch off checks for speed if user can be sure all values are 'inside limits'.
          */
          Derived& autoscale_check_limits(bool b);
          bool autoscale_check_limits(); //!< \return @c true if to check that values used for autoscaling are within limits.
          bool autoscale(); //!< \return @c true if to use autoscale values autoscaling for X-axis.
          Derived& autoscale(bool b);  //!< Set @c true if to use autoscale values for X-axis.
          bool x_autoscale();  //!< \return @c true if to use autoscale value for X-axis.
          Derived& x_autoscale(bool b); //!< Set @c true if to use autoscale values for X-axis.
          Derived& x_autoscale(std::pair<double, double> p); //!<autoscale X axis using a pair of doubles.
          template <class T> //!< \tparam T an STL container: array, vector ...
          Derived& x_autoscale(const T& container); //!<autoscale X axis using the whole data series.
          template <class T> //!<\tparam T an STL container: array, vector ...
          Derived& x_autoscale(const T& begin, const T& end); //!<Data series using iterators.
          Derived& x_with_zero(bool b); //!<Set X-axis autoscale to include zero (default = false).
          bool x_with_zero(); //!<\return  true if X-axis autoscale to include zero (default = false).
          Derived& x_min_ticks(int min_ticks); //!<Set X-axis autoscale to include at least minimum number of ticks (default = 6).
          int x_min_ticks(); //!<\return  X-axis autoscale minimum number of ticks.
          Derived& x_steps(int steps); /*!< Set autoscale to set ticks in steps multiples of:\n
             2,4,6,8,10, if 2\n
             or 1,5,10 if 5\n
             or 2,5,10 if 10.\n
             default = 0 (none).
             \note: Must \b preceed x_autoscale(data) call).
             */
          int x_steps(); //!<\return  autoscale to set ticks in steps.
          Derived& x_tight(double tight); //!<Set tolerance to autoscale to permit data points slightly outside both end ticks.
          double x_tight(); //!<\return  tolerance given to autoscale to permit data points slightly outside both end ticks.
          //!<Get results of autoscaling.
          double x_auto_min_value(); //!< \return X-axis minimum value computed by autoscale.
          double x_auto_max_value(); //!< \return X-axis maximum value computed by autoscale.
          double x_auto_tick_interval(); //!<\return  the X-axis major tick interval computed by autoscale.
          int x_auto_ticks(); //!<\return  the X-axis number of major ticks computed by autoscale.
          Derived& nan_limit_color(const svg_color&); //!<Set the color for NaN point stroke color.
          svg_color nan_limit_color(); //!<\return  the color for the NaN point stroke color.
          Derived& nan_limit_fill_color(const svg_color&); //!<Set the color for NaN point fill color.
          svg_color nan_limit_fill_color(); //!<\return  the color for the NaN point fill color.
          // This fails because size is not in svg_style.
          Derived& nan_limit_size(const int size); //!< Set the size for NaN point.
          int nan_limit_size(); //!<\return The size for the NaN point.

          Derived& plus_inf_limit_color(const svg_color&); //!<Set the color for +infnity point stroke color.
          svg_color plus_inf_limit_color(); //!<\return  the color for the +infnity point stroke color.
          Derived& plus_inf_limit_fill_color(const svg_color&); //!<Set the color for +infnity point fill color.
          svg_color plus_inf_limit_fill_color(); //!<\return  the color for the +infnity point fill color.
          // This fails because size is not in svg_style.
          Derived& plus_inf_limit_size(const int size); //!< Set the size for +infnity point.
          int plus_inf_limit_size(); //!<\return The size for the NaN point.

          Derived& minus_inf_limit_color(const svg_color&); //!<Set the color for -infinity point stroke color.
          svg_color minus_inf_limit_color(); //!<\return  the color for the -infinity point stroke color.
          Derived& minus_inf_limit_fill_color(const svg_color&); //!<Set the color for -infinity point fill color.
          svg_color minus_inf_limit_fill_color(); //!<\return  the color for the -infinity point fill color.
          // This fails because size is not in svg_style?
          Derived& minus_inf_limit_size(const int size); //!< Set the size for -infinity point.
          int minus_inf_limit_size(); //!<\return The size for the -infinity point.

          Derived& draw_note
            (double x, double y, std::string note, rotate_style rot = horizontal, align_style al = center_align, const svg_color& = black, text_style& tsty = no_style);
           /*!< \brief Annotate plot with a  text string (perhaps including Unicode), putting note at SVG Coordinates X, Y.
            \details Defaults color black, rotation horizontal and align = center_align
            Using center_align is recommended as it will ensure that will center correctly
            (even if original string is made much longer because it contains Unicode,
            for example Greek or math symbols, taking about 6 characters per symbol)
            because the render engine does the centering.
          */
          Derived& draw_line(double x1, double y1, double x2, double y2, const svg_color& col = black);
          /*!< Annotate plot with a line from SVG Coordinates X1, Y1 to X2, Y2. (Default color black).
              Note \b NOT the data values. See draw_plot_line if want to use user coordinates.
            */
          Derived& draw_plot_line(double x1, double y1, double x2, double y2, const svg_color& col = black);
          /*!< \brief Annotate plot with a line from user's Cartesian Coordinates X1, Y1 to X2, Y2.
              \details For example, -10, -10, +10, +10, Default color black.
            */
          Derived& draw_plot_curve(double x1, double y1, double x2, double y2, double x3, double y3,const svg_color& col = black);
          /*!< \brief Annotate plot with a line from user's Cartesian Coordinates X1, Y1 via X2, Y2 to X3, Y3.
              \details For example, -10, -10, +10, +10, Default color black.
          */

          Derived& one_sd_color(const svg_color&); //!<Set the color for the one standard deviation (~67% confidence) ellipse fill.
          svg_color one_sd_color(); //!<\return Color for the one standard deviation (~67% confidence) ellipse fill.
          Derived& two_sd_color(const svg_color&); //!<Set the color for two standard deviation (~95% confidence) ellipse fill.
          svg_color two_sd_color(); //!<\return Color for two standard deviation (~95% confidence) ellipse fill.
          Derived& three_sd_color(const svg_color&); //!<Set the color for three standard deviation (~99% confidence) ellipse fill.
          svg_color three_sd_color(); //!<\return Color for three standard deviation (~99% confidence) ellipse fill.

          // Image info (& identical const version).

          //svg& get_svg()
          //{
          //  derived()._update_image();
          //  return derived().image_;
          //}

          //const svg& get_svg() const
          //{
          //  derived()._update_image();
          //  return derived().image_;
          //}
        }; // template <class Derived> class axis_plot_frame


    } // detail

    namespace detail
    { //! \namespace detail Holds base class @c axis_plot_frame used for 1D, 2D and Box plots.
//! \cond DETAIL

          /* \brief Clear all layers of the plot.
              \details
                When writing to multiple documents, the contents of the plot
                may change significantly between. Rather than figuring out what
                has and has not changed, just erase the contents of the
                legend, title... in the document and start over.
          */
      template <class Derived>
      void axis_plot_frame<Derived>::clear_all()
      {
        clear_legend();
        clear_background();
        clear_x_axis();
        clear_y_axis();
        clear_title();
        clear_points();
        clear_plot_background();
        clear_grids();
      }

      std::string sv(double v, const value_style& sty, bool);

      template <class Derived>
      void axis_plot_frame<Derived>::clear_background()
      { //!< Clear the whole image background layer of the SVG plot.
        derived().image_.g(PLOT_BACKGROUND).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_title()
      { //!< Clear the plot title layer of the SVG plot.
        derived().image_.g(PLOT_TITLE).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_points()
      {  //!< Clear the data points layer of the SVG plot.
        derived().image_.g(PLOT_DATA_POINTS).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_plot_background()
      { //!< Clear the plot area background layer of the SVG plot.
        derived().image_.g(PLOT_WINDOW_BACKGROUND).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_legend()
      { //!< Clear the legend layer of the SVG plot.
        derived().image_.g(PLOT_LEGEND_BACKGROUND).clear();
        derived().image_.g(PLOT_LEGEND_POINTS).clear();
        derived().image_.g(PLOT_LEGEND_TEXT).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_x_axis()
      { //!< Clear the X axis layer of the SVG plot.
        derived().image_.g(PLOT_X_AXIS).clear();
        derived().image_.g(PLOT_X_MINOR_TICKS).clear();
        derived().image_.g(PLOT_X_MAJOR_TICKS).clear();
        derived().image_.g(PLOT_X_LABEL).clear();
        derived().image_.g(PLOT_X_TICKS_VALUES).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_y_axis()
      { //!< Clear the Y axis layer of the SVG plot.
        derived().image_.g(PLOT_Y_AXIS).clear();
        derived().image_.g(PLOT_Y_MINOR_TICKS).clear();
        derived().image_.g(PLOT_Y_MAJOR_TICKS).clear();
        derived().image_.g(PLOT_Y_LABEL).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::clear_grids()
      { //!< Clear the  grids layer of the SVG plot.
        derived().image_.g(PLOT_X_MAJOR_GRID).clear();
        derived().image_.g(PLOT_X_MINOR_GRID).clear();
        derived().image_.g(PLOT_Y_MAJOR_GRID).clear();
        derived().image_.g(PLOT_Y_MINOR_GRID).clear();
      }

      template <class Derived>
      void axis_plot_frame<Derived>::transform_point(double& x, double& y)
      { //!< Scale & shift both X & Y to graph Cartesian coordinates.
        x = derived().x_scale_ * x + derived().x_shift_;
        y = derived().y_scale_ * y + derived().y_shift_;
        adjust_limits(x, y); // In case either hits max, min, infinity or NaN.
      }

      //template <class Derived>
     // void axis_plot_frame<Derived>::transform_point(double& x, double& y);
      // Protected Member Functions Definitions in axis_plot_frame.ipp:
      // TODO do we need to adjust_limit(x); // In case hits max, min, infinity or NaN?
      // This implies that user's choice of X-axis range is wrong?
      // So more drastic action like throwing might be least worst action?

      template <class Derived>
      void axis_plot_frame<Derived>::transform_x(double& x)
      { //!< Scale and shift X value only.
        x = derived().x_scale_ * x + derived().x_shift_;
      }

      template <class Derived>
      void axis_plot_frame<Derived>::transform_y(double& y)
      { //!< Scale and shift Y value only.
        y = derived().y_scale_ * y + derived().y_shift_;
      }

      template <class Derived>
      void axis_plot_frame<Derived>::draw_x_minor_tick(double value, path_element& tick_path, path_element& grid_path)
      { //!< Draw X-axis minor ticks, and optional grid. (Value is NOT (yet) shown beside the minor tick).
        double x(value); // Tick position and tick value label,
        transform_x(x); // Convert to svg.
        double y_bottom(0.); // Start on the horizontal X-axis line.
        double y_top(derived().image_.y_size()); // Image top.

        // Draw the minor grid, if wanted.
        if (derived().x_ticks_.minor_grid_on_)
        {
          if (!derived().plot_window_on_)
          { // Use whole image.
            // Make space for title and X-axis labels.
            if (derived().title_on_)
            { // Allow text_margin_ * font_size around text (pixels).
              y_bottom += derived().title_info_.textstyle().font_size() * derived().text_margin_;
            }
            if (derived().x_axis_.label_on_)
            {
              y_top -= derived().x_label_info_.textstyle().font_size() * derived().text_margin_;
            }
          }
          else
          { // plot_window_on_ == true.
            y_bottom = derived().plot_top_ + derived().plot_window_border_.border_width_; // Top.
            y_top = derived().plot_bottom_ - derived().plot_window_border_.border_width_; // Bottom. Ensure *inside* window.
          }
          // Make sure that we are drawing inside the allowed window.
          if ((x >= derived().plot_left_) && (x <= derived().plot_right_)) // allow = too?
          {
            //std::cerr << "Writing draw_x_minor_tick grid inside plot window: x = "
            //  << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
            grid_path.M(x, y_bottom).L(x, y_top); // Draw grid line.
          }
          else
          { // This will happen but is designed to be ignored!
            // See comment in draw_x_axis
            // std::cerr << "Writing draw_x_minor_tick grid OUTside plot window: x = "
            //  << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
          }
        } // x_minor_grid

        // Draw x minor ticks.
        if (derived().x_ticks_.ticks_on_window_or_on_axis_ < 0)
        { // Put minor ticks on the plot window border bottom.
          y_bottom = derived().plot_bottom_; // on the window line.
          y_top = derived().plot_bottom_; // y_bottom = upper, y_top = lower end of tick.
        }
        else if (derived().x_ticks_.ticks_on_window_or_on_axis_ > 0)
        { // Put minor ticks on the plot window border top.
          y_bottom = derived().plot_top_; // on the window line.
          y_top = derived().plot_top_; // y_bottom = upper, y_top = lower end of tick.
        }
        else // derived().x_ticks_.ticks_on_window_or_on_axis_ == 0
        { // Internal style, draw tick up and/or down from the X-axis line.
          y_bottom = derived().x_axis_.axis_; // ON X-axis horizontal line.
          y_top = derived().x_axis_.axis_;
        }
        if (derived().x_ticks_.up_ticks_on_)
        {
          y_bottom -= derived().x_ticks_.minor_tick_length_; // up
        }
        if (derived().x_ticks_.down_ticks_on_)
        {
          y_top += derived().x_ticks_.minor_tick_length_; // down.
        }
       // Make sure that we are drawing inside the allowed window.
        if ((x >= derived().plot_left_) && (x <= derived().plot_right_)) // TODO allow < or <=
        {
          tick_path.M(x, y_bottom).L(x, y_top);
          // No value labels on minor ticks, at present.
        }
        else
        { // This will happen but is designed to be ignored!
          // See comment in draw_x_axis
          //std::cerr << "Writing draw_x_minor_tick OUTside plot window: x = "
          // << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
        }
      } // void draw_x_minor_tick

      template <class Derived>
      void axis_plot_frame<Derived>::draw_x_major_tick(double value, path_element& tick_path, path_element& grid_path)
      { //! Draw major ticks - and grid too if wanted.
        //! If major_value_labels_side then value shown beside the major tick.
        double x(value); //
        transform_x(x); // x value in svg.
        if ((x < derived().plot_left_ - 0.01) || (x > derived().plot_right_ + 0.01))
        // Allow a bit extra to allow for round-off errors.
        { // tick value is way outside plot window, so nothing to do.
#ifdef BOOST_SVG_POINT_DIAGNOSTICS
            //std::cout << "Tick skipped as outside plot window "<< derived().plot_left_ << " at x = " << x << std::endl;
            // This *was* displayed for a plot.
#endif // BOOST_SVG_POINT_DIAGNOSTICS
          return;
        }
        double y_up(0.); // upper end of tick.
        double y_down(derived().image_.x_size()); // y_down = lower end of tick.
        if (derived().x_ticks_.major_grid_on_)
        { // Draw major grid vertical line.
          if (!derived().plot_window_on_)
          { // Allow a modest margin around text of title and X-axis labels, if in use.
            // text_margin_(2.), // for axis label text, as a multiplier of the font size.
            // Should allow a 'half line space' above and below the label text.
            if (derived().title_on_)
            {
              y_up += derived().title_info_.textstyle().font_size() * derived().text_margin_;
            }
            if (derived().x_ticks_.major_value_labels_side_ != 0)
            { // Value may be shown either side the major tick.
              y_down -= derived().x_label_info_.textstyle().font_size() * derived().text_margin_;
            }
          }
          else
          { // plot_window_on_ == true
            y_up = derived().plot_top_; // Bottom of plot window.
            y_down = derived().plot_bottom_; // Top of plot window.
          }
          //if((y_down <= derived().plot_bottom_) && (y_up >= derived().plot_top_) && (x >= derived().plot_left_) && (x <= derived().plot_right_))
          //{ // Make sure that we are drawing inside the allowed window.
          grid_path.M(x, y_up).L(x, y_down); // Vertical grid line.
        //}
        } // use_x_major_grid

        // Draw major tick (perhaps as well as grid - ticks might be wider than grid).
        // Make sure that we are drawing inside the allowed plot window.
        //if((x >= derived().plot_left_) && (x <= derived().plot_right_)) // now <=
        //{ Removed these checks as round off causes trouble.
        double x_tick_length = derived().x_ticks_.major_tick_length_;
        if (derived().x_ticks_.ticks_on_window_or_on_axis_ < 0)
        { // Put the ticks on the plot window border (was external).
          y_up = derived().plot_bottom_; // on the window line.
          y_down = derived().plot_bottom_; // y_up = upper, y_down = lower.
          if (derived().x_ticks_.up_ticks_on_)
          {
            y_up -= x_tick_length; // up
          }
          if (derived().x_ticks_.down_ticks_on_)
          {
            y_down += x_tick_length; // down.
          }
        }
        else if (derived().x_ticks_.ticks_on_window_or_on_axis_ > 0)
        { // Put the ticks on the plot window border (was external).
          y_up = derived().plot_top_; // on the window line.
          y_down = derived().plot_top_; // y_up = upper, y_down = lower.
          if (derived().x_ticks_.up_ticks_on_)
          {
            y_up -= x_tick_length; // up
          }
          if (derived().x_ticks_.down_ticks_on_)
          {
            y_down += x_tick_length; // down.
          }
        }
        else
        { // Draw tick from the central X axis line (Internal_style).
          y_up = derived().x_axis_.axis_; // X-axis line.
          y_down = derived().x_axis_.axis_;
          if (derived().x_ticks_.up_ticks_on_)
          {
            y_up -= x_tick_length; // up
          }
          if (derived().x_ticks_.down_ticks_on_)
          {
            y_down += x_tick_length; // down.
          }
        }
        tick_path.M(x, y_up).L(x, y_down);
        // Leaving current position at the bottom end of the tick.
        // y_up and y-down are the ends of the tick.
        // These may be on the axis line, or the plot window.

        if (derived().x_ticks_.major_value_labels_side_ != 0)
        { // Show a value by the horizontal X-axis tick as "1.2" or "3.4e+000"...
          std::stringstream tick_value_label;
          tick_value_label.precision(derived().x_ticks_.value_precision_);
          tick_value_label.flags(derived().x_ticks_.value_ioflags_);
          tick_value_label << value; // for tick "4", "1.2" or "3.4e+000"...
          if (derived().x_ticks_.strip_e0s_)
          { // Remove unecessary e, +, leadings 0s.
            std::string v = strip_e0s(tick_value_label.str());
            tick_value_label.str(v);
          }
          double y = 0; // Where to start writing from, at end of bottom or top tick, if any.
          // = 0 is only to avoid unitialised warning.
          align_style alignment = center_align;
          // Adjustments to provide space from end of tick before or after writing label.
          if (derived().x_ticks_.label_rotation_ == upward) // vertical writing up.
          {  // Shift to center value digits and minus sign on tick.
            x += derived().x_value_label_style_.font_size() * 0.2;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little below y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 0.6;
              alignment = right_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little above y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == downward)
          {  // Should handle other directions too.
            x -= derived().x_value_label_style_.font_size() * 0.3;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little below y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little above y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == steepup)
          {  // Should handle other directions too.
            x -= derived().x_value_label_style_.font_size() * 0.3;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels upward, so start a little below y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little above y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == uphill)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            x += derived().x_value_label_style_.font_size() * 0.5;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little to bottom of y_bottom.
              y = y_down + derived().x_value_label_style_.font_size() * sin45;
              // Seems to need a bit more space for top than bottom if rotated.
              alignment = right_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little to top of y_top.
              y = y_up - derived().x_value_label_style_.font_size() * 0.3;
              alignment = left_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == slopeup)
          { // Assume for 30 degree slope, need about sqrt(2) less space.
            x += derived().x_value_label_style_.font_size() * 0.5;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little to bottom of y_bottom.
              y = y_down + derived().x_value_label_style_.font_size() * sin45;
              // Seems to need a bit more space for top than bottom if rotated.
              alignment = right_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little to top of y_top.
              y = y_up - derived().x_value_label_style_.font_size() * 0.2;
              alignment = left_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == downhill)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            x -= derived().x_value_label_style_.font_size() * 0.3;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little to bottom of y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for top than bottom if rotated.
              alignment = left_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little to top of y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.3;
              alignment = right_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == slopedownhill)
          { // Assume some 30 slope, so need about sqrt(2) less space.
            x -= derived().x_value_label_style_.font_size() * 0.3;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little to bottom of y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for top than bottom if rotated.
              alignment = left_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little to top of y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.3;
              alignment = right_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == steepdown)
          {  // Should handle other directions too.
            x -= derived().x_value_label_style_.font_size() * 0.3;
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom, so start a little below y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little above y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
          }
          else if (derived().x_ticks_.label_rotation_ == horizontal)
          { // Tick value label on X-axis is normal default horizontal.
            if (derived().x_ticks_.major_value_labels_side_ < 0)
            { // labels to bottom of tick, so start a little below bottom of y_down.
              y = y_down + derived().x_value_label_style_.font_size() * 1.3; // 1.3 allows 1/3 font space.
              alignment = center_align; // center on the tick.
            }
            else if (derived().x_ticks_.major_value_labels_side_ > 0)
            { // labels to top, so start a little to top of y_up.
              y = y_up - derived().x_value_label_style_.font_size() * 0.7;
              alignment = center_align;
            }
          }
          else
          { // upsidedown, backup... - can't see any conceivable use for these.
            return; // Others not yet implemented.
          } // rotations
          if (x <= 0)
          { // Sanity checks on svg coordinates.
            throw std::runtime_error("X-tick X value wrong!");
          }
          if (y <= 0)
          {
            throw std::runtime_error("X-tick Y value wrong!");
          }
          // Draw the X ticks value labels, "1", "2" "3" ...
          if (derived().x_ticks_.ticks_on_window_or_on_axis_ != 0)
          { // External to plot window style bottom or top.
            // Always want all values including "0", if labeling external to plot window.
            // x_ticks_.ticks_on_window_or_on_axis_ == true != 0
            derived().image_.g(detail::PLOT_X_TICKS_VALUES).text(
              x,
              y,
              tick_value_label.str(),
              derived().x_value_label_info_.textstyle(), // font, size etc
              alignment, derived().x_ticks_.label_rotation_);
          }
          else
          {
            if ((value != 0) && derived().x_axis_.axis_line_on_)
            { // Avoid a "0" below the X-axis if it would be cut through by any internal vertical Y-axis line.
              derived().image_.g(detail::PLOT_X_TICKS_VALUES).text(
                x,
                y,
                tick_value_label.str(),
                derived().x_value_label_info_.textstyle(), // font, size etc
                alignment,
                derived().x_ticks_.label_rotation_);
            }
          } // on plot window or 'on axis'.
        }
        else
        { // Outside plot window!
          //std::cerr << "Warning : writing a draw_x_major_tick OUTSIDE the plot window: "
          //  "x = " << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
        }
      } // draw_x_major_tick

      template <class Derived>
      void axis_plot_frame<Derived>::draw_x_axis()
      { //! Draw horizontal X-axis line & plot window line to hold, and ticks and grids.
        if (derived().x_axis_.axis_line_on_)
        { // Want a horizontal X-axis line drawn.
          double xleft = derived().plot_left_;
          double xright = derived().plot_right_;
          if (derived().x_axis_position_ == x_intersects_y)
          { // Draw the horizontal X-axis line the full width of the plot window,
            // perhaps including an addition in lieu of a major tick.
            if (derived().y_ticks_.left_ticks_on_)
            {
              if (derived().y_ticks_.ticks_on_window_or_on_axis_ < 0) // left
              { // Extend the horizontal line left in lieu of longest tick.
                xleft -= (std::max)(derived().y_ticks_.minor_tick_length_, derived().y_ticks_.major_tick_length_);
              }
            }
            else if (derived().y_ticks_.right_ticks_on_)
            {
              if (derived().y_ticks_.ticks_on_window_or_on_axis_ > 0) // right
              { // Extend the horizontal X-axis line right in lieu of longest tick.
                xright += (std::max)(derived().y_ticks_.minor_tick_length_, derived().y_ticks_.major_tick_length_);
              }
            }
            double y = derived().x_axis_.axis_; // y = 0, (provided y range includes zero).
            derived().image_.g(PLOT_X_AXIS).line(xleft, y, xright, y);
            if (derived().x_ticks_.ticks_on_window_or_on_axis_ < 0) // bottom
            { // Draw a vertical line holding the ticks on the top of plot window.
              derived().image_.g(PLOT_X_AXIS).line(xleft, derived().plot_bottom_, xright, derived().plot_bottom_);
            }
            else if (derived().x_ticks_.ticks_on_window_or_on_axis_ > 0)  // top
            {// Draw a vertical line holding the ticks on the bottom of plot window.
              derived().image_.g(PLOT_X_AXIS).line(xleft, derived().plot_top_, xright, derived().plot_top_);
            }
          }
          else if (derived().x_axis_position_ == top)
          {
            derived().image_.g(PLOT_X_AXIS).line(xleft, derived().plot_top_, xright, derived().plot_top_);
          }
          else if (derived().x_axis_position_ == bottom)
          {
            derived().image_.g(PLOT_X_AXIS).line(xleft, derived().plot_bottom_, xright, derived().plot_bottom_);
          }
          else
          { // warn that things have gone wrong?
          }
        } // x_axis_.axis_line_on_

        // Access the paths for the ticks & grids, ready for additions.
        path_element& minor_tick_path = derived().image_.g(PLOT_X_MINOR_TICKS).path();
        path_element& major_tick_path = derived().image_.g(PLOT_X_MAJOR_TICKS).path();
        path_element& minor_grid_path = derived().image_.g(PLOT_X_MINOR_GRID).path();
        path_element& major_grid_path = derived().image_.g(PLOT_X_MAJOR_GRID).path();

        // x_minor_jump is the interval between minor ticks.
        double x_minor_jump = derived().x_ticks_.major_interval_ /
          (derived().x_ticks_.num_minor_ticks_ + 1.);

        // Draw the ticks on the positive side (right of zero).
        for (double x = 0.; x <= derived().x_axis_.max_; x += derived().x_ticks_.major_interval_)
        {
          for (double j = x + x_minor_jump;
            j < (x + derived().x_ticks_.major_interval_) * (1. - 2 * std::numeric_limits<double>::epsilon());
            j += x_minor_jump)
          { // Reduce test value by a few bits to avoid accumulated rounding error
            // that intermittently puts minor ticks *at same value as* major ticks.
            // This will output 'orphaned' minor ticks that are beyond the plot window,
            // if the last major tick does not coincide with the plot window.
            // These are just ignored in draw_x_minor_tick.
            // There might be 9 of them, so
            // if you have the common 9 minor tick between major ticks!
            // TODO this seems ugly - as does the negative ones below.
            draw_x_minor_tick(j, minor_tick_path, minor_grid_path);
          } // for j
          if ((x != 0. || !derived().y_axis_.axis_line_on_) || (derived().x_ticks_.ticks_on_window_or_on_axis_ != 0))
          { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line.
            // (won't be Y-axis line for 1-D where the zero tick is always wanted).
            draw_x_major_tick(x, major_tick_path, major_grid_path);
          }
        }

        // Draw the ticks on the negative side (left of zero).
        for (double x = 0.; x >= derived().x_axis_.min_; // ?? * (1. + 2 * std::numeric_limits<double>::epsilon());
          x -= derived().x_ticks_.major_interval_) // Want a close to test here?
        {
          // Draw minor ticks.
          for (double j = x - x_minor_jump;
            j > (x - derived().x_ticks_.major_interval_ + x_minor_jump) * (1. + 2 * std::numeric_limits<double>::epsilon());
            // Increase test value by a few bits to avoid accumulated rounding error
            // that intermittently puts minor ticks *at same value as* major ticks.
            j -= x_minor_jump)
          {
            if ((j != 0. || !derived().y_axis_.axis_line_on_) || (derived().x_ticks_.ticks_on_window_or_on_axis_ != 0))
            { // Avoid a minor tick at x == 0 where there *is* a vertical Y-axis line.
              // (won't be Y-axis line for 1-D where the zero tick is always wanted).
              // But no tick means no value label 0 either unless on_plot_window.
              draw_x_minor_tick(j, minor_tick_path, minor_grid_path);
            }
          }
          if ((x != 0. || !derived().y_axis_.axis_line_on_) || (derived().x_ticks_.ticks_on_window_or_on_axis_ != 0))
          { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line.
            // (won't be Y-axis line for 1-D where the zero tick is always wanted).
            // But no tick means no value label 0 either unless on_plot_window.
            draw_x_major_tick(x, major_tick_path, major_grid_path);
          }
        }
      } // void draw_x_axis()

      template <class Derived>
      void axis_plot_frame<Derived>::draw_x_axis_label()
      { //! Draw the X-axis label text (for example, length),
        //! and append any optional units (for example, km).
        // X-label color default is set in constructor thus:
        // image_.g(detail::PLOT_X_LABEL).style().stroke_color(black);
        // and changed using x_label_color(color);
        // Similarly for font family and size etc (must be same for both label and units).

        std::string x_label = derived().x_label_info_.text(); // x_axis_ label, and optional units.
        if (derived().x_axis_.label_units_on_ && (derived().x_units_info_.text() != ""))
        { // Append the units, if any, user providing brackets () if required.
          x_label += derived().x_units_info_.text(); // for example: "time (sec)".
        }

        double y = derived().plot_bottom_;
        // Glyphs for western characters are aligned with the left bottom of capital letter,
        // so need to allow 1/3 more below for any descenders.

        // cout << "derived().x_ticks_.ticks_on_window_or_on_axis_ " << derived().x_ticks_.ticks_on_window_or_on_axis_ << endl;
        // using derived(0 means debugging doesn't work!  So resort to old-fashioned print statements.
        if (derived().x_ticks_.ticks_on_window_or_on_axis_ < 0) // -1 means ticks on bottom of plot window.
        { // Ticks value labels below plot window.
          if (derived().x_ticks_.major_value_labels_side_ < 0) // bottom
          { // Shift down to allow for any tick value labels.
            if ((derived().x_ticks_.label_rotation_ == downward) || (derived().x_ticks_.label_rotation_ == upward))
            { // downward tick value label direction 90 up or down.
              y += derived().x_ticks_.label_max_space_;
              if (derived().x_ticks_.down_ticks_on_ == true)
              {  // Move down for any downward ticks.
                y += 1.1 * (std::max)(derived().x_ticks_.major_tick_length_, derived().x_ticks_.minor_tick_length_); // And avoid macro max trap!
                // and a small space.
                y += 0.7 * (derived().x_label_info_.textstyle().font_size() + derived().x_value_label_info_.textstyle().font_size()); // best compromise?
              }
            }
            else if ((derived().x_ticks_.label_rotation_ == steepdown) || (derived().x_ticks_.label_rotation_ == steepup))
            { // downward tick value label direction 60 up or down.
              y += derived().x_ticks_.label_max_space_;
              if (derived().x_ticks_.down_ticks_on_ == true)
              {  // Move down for any downward ticks.
                y += 1.1 * (std::max)(derived().x_ticks_.major_tick_length_, derived().x_ticks_.minor_tick_length_); // And avoid macro max trap!
                // and a small space.
                y += 0.5 * (derived().x_label_info_.textstyle().font_size() + derived().x_value_label_info_.textstyle().font_size()); // best compromise?
              }
            }
            else if ((derived().x_ticks_.label_rotation_ == uphill) || (derived().x_ticks_.label_rotation_ == downhill))
            { // sloping 45 degrees up or down.
              y += derived().x_ticks_.label_max_space_ * sin45; // Move down from end of tick.
              if (derived().x_ticks_.down_ticks_on_ == true)
              {  // Move down for any downward ticks.
                y += 1.1 * (std::max)(derived().x_ticks_.major_tick_length_, derived().x_ticks_.minor_tick_length_); // And avoid macro max trap!
                // and a small space.
                y += 0.7 * (derived().x_label_info_.textstyle().font_size() + derived().x_value_label_info_.textstyle().font_size()); // best compromise?
              }
            }
            else if ((derived().x_ticks_.label_rotation_ == slopeup) || (derived().x_ticks_.label_rotation_ == slopedownhill))
            { // sloping 30 degrees.
              y += derived().x_ticks_.label_max_space_ * sin45; // Move down from end of tick.
              if (derived().x_ticks_.down_ticks_on_ == true)
              {  // Move down for any downward ticks.
                y += 1.1 * (std::max)(derived().x_ticks_.major_tick_length_, derived().x_ticks_.minor_tick_length_); // And avoid macro max trap!
                // and a small space.
                y += 0.5 * (derived().x_label_info_.textstyle().font_size() + derived().x_value_label_info_.textstyle().font_size()); // best compromise?
              }
            }
            else if (derived().x_ticks_.label_rotation_ == horizontal)
            { // horizontal X ticks value labels (default).
              if (derived().x_ticks_.major_value_labels_side_ < 0)
              { //  Move down to allow space for font size of tick value labels below X-axis.
                y += derived().x_value_label_info_.textstyle().font_size();
              }
              y += derived().x_label_info_.textstyle().font_size() * 1.3; // Allow for the X-axis label font and space.
              // See also 1.3 factor drawing ticks.
            }
            else
            {
              std::cout << " Rotation of X label rotation" << derived().x_ticks_.label_rotation_ << "not yet implemented!" << std::endl;
            }
          }
          else if (derived().x_ticks_.major_value_labels_side_ > 0)
          { // Tick labels above, only ticks below, so just move down for height of label font.
            y += derived().x_label_info_.textstyle().font_size() * 1.3; // Allow for the X-axis label font and space.
          }
          else
          { // derived().x_ticks_.major_value_labels_side_ == 0
            // So no change for labels.
            y += derived().x_label_info_.textstyle().font_size() * 1.3; // Allow for the X-axis label font and space.
          }

          if (derived().x_ticks_.down_ticks_on_)
          { // Shift down for biggest of any ticks, and bit of space.
            y += 1.1 * (std::max)(derived().x_ticks_.minor_tick_length_, derived().x_ticks_.major_tick_length_);
          // y += derived().x_ticks_.value_label_style_.font_size() * 1.; // Shift down to suit tick labels?
          }
        }
        else if (derived().x_ticks_.ticks_on_window_or_on_axis_ > 0)
        {  // = +1 means ticks are on top of plot window.
           // Shift down from plot window bottom to suit X-axis label.
          y += derived().x_label_info_.textstyle().font_size() * 1.7;
        }
        else if (derived().x_ticks_.ticks_on_window_or_on_axis_ == 0)
        { // Ticks are ON the X-axis line, so X label is just below the plot bottom.
          // No space needed for ticks.
           // Character starts at bottom of capital letter, so allow for descenders.
           //y = derived().image_.y_size() - derived().image_border_width(); // Place X Label just above the image bottom.
           //y -= derived().image_border_.margin_;
          y += derived().x_label_info_.textstyle().font_size() * 1.7;
        }

        derived().image_.g(PLOT_X_LABEL).push_back(new text_element(
          ( // x position relative to the x-axis which is middle of plot window.
            derived().plot_right_ + derived().plot_left_) / 2,  // x coordinate - middle.
          y, // Down from plot window.
          x_label,
          derived().x_label_info_.textstyle(),
          center_align, horizontal)
        );
      } // void draw_x_axis_label()

      template <class Derived>
      void axis_plot_frame<Derived>::adjust_limits(double& x, double& y)
      { //! If value of a data point reaches limit of max, min, infinity,
        //! use the appropriate plot min or max value instead.
        if (detail::limit_max(x))
        {
          x = derived().plot_right_;
        }
        if (detail::limit_max(y))
        {
          y = derived().plot_top_;
        }
        if (detail::limit_min(x))
        {
          x = derived().plot_left_;
        }
        if (detail::limit_min(y))
        {
          y = derived().plot_top_;
        }
        // If value is NaN, use zero instead.
        // TODO Do we want/get a different color or shape for NaNs?
        if (detail::limit_NaN(x))
        {
          x = 0.;
          transform_x(x);
        }
        if (detail::limit_NaN(y))
        {
          y = 0.;
          transform_y(y);
        }
      } // void adjust_limits

      //! \brief Check if the text will fit into the space available.
      //! \details If force text into the x_size (using text_length option),
      //! then too large a font or too many characters
      //! may over-compress and push the glyphs to overlap,
      //! so warn here of overflow or over-compress.
      //! A factor of 1.6 more characters than width
      //! still allows bold characters to not quite collide or overlap.
      bool check_text_fit(const text_style& /*style*/, const std::string& text, double font_size, double title_svg_length, double image_size)
      {
        static const double squash_factor = 1.6;
        if (title_svg_length > image_size * squash_factor)
        { // Issue Warning that text like title is too long or too big font!
          std::cout << "Text \"" << text
            << "\"\n  with an estimated width " << title_svg_length << " (SVG units) may overflow plot space " << image_size
            << "\n  or over-compress text with compression ratio " << title_svg_length / image_size
            << ".\n  Reduce font size from " << font_size
            << ", or number of characters from " << text.size()
            << ", or increase image size from " << image_size
            << "?" << std::endl;
          // Might use text_style to show decoration?
          // For example:
          // Title "Plot showing short legend &#x26; sizes."
          //   width 378 (SVG units) may overflow plot image!
          //  (Reduce font size from 30, or number of characters from 21, or increase image size from 300).

          return false; // Warning given that may not fit or be legible.
        }
        return true; // Will fit.
      } //       bool check_text_fit(title_svg_length , int image_size )


      template <class Derived>
      void axis_plot_frame<Derived>::draw_title()
      { /*! Draw title (for the whole plot).
          Update title_info_ with position.
          Assumes align = center_align
          Using center_align will ensure that title will center correctly
          because the render engine does the centering.
          (Even if the original string is made much longer because it contains Unicode,
          Greek, math symbols etc, taking about 8 characters per symbol.
          For example, the Unicode symbol for square root is "&#x221A;" but takes only about one character width).
        */
        //Image size
        //  <text x="700" y="0" text-anchor="middle" font-size="20" font-family="Lucida Sans Unicode">Plot showing several data point marker shapes &#x26; sizes.</text>
        //  title size 59
        //  title font size 20
        //  title font size * size 1180
        //  Title "Plot showing several data point marker shapes &#x26; sizes."
        //   may overflow plot image! (Reduce font size or number of characters? Or increase image size?)

       //  std::cout << "Image  = " << derived().title_info_.x(derived().image_.x_size()) << std::endl;
        //  <text x="700" y="0" text-anchor="middle" font-size="20" font-family="Lucida Sans Unicode">Plot showing several data point marker shapes &#x26; sizes.</text>

        // Warn if plot title will overflow image (since title is centered, this will truncate at both ends).

        double title_svg_length = string_svg_length(derived().title(), derived().title_info_.textstyle());
#ifdef BOOST_SVG_TITLE_DIAGNOSTICS
        std::cout << "Drawing plot title, image x width " << derived().image_.x_size()  // default 700
          << "\n  Title = \"" << derived().title() << "\n"
          << "  title has = " << derived().title().size() << " characters" // number of chars.
          << ", title text SVG width = " << title_svg_length
          << ", title font size = " << derived().title_font_size()
          << ", title font family = " << derived().title_font_family()
          << ", title font weight = " << derived().title_font_weight()
          << ", title text_length = " << derived().title_text_length()
          << ",\n title_style_ = " << derived().title_style_
          << std::endl;
#endif // BOOST_SVG_TITLE_DIAGNOSTICS
//       bool check_text_fit(const text_style& style, std::string& text, double font_size, double title_svg_length, double image_size)

        // If force title into the x_size (using text_length option),
        // then too large a font or too many characters may over-compress and push the glyphs to overlap,
        // so warn here of overflow or over-compress.
        // A factor of 1.6 more characters than width still allows bold characters to not quite collide or overlap.
        check_text_fit(derived().title_info_.textstyle(), derived().title(), derived().title_font_size(), title_svg_length, derived().image_.x_size());

        /*
        // If force title into the x_size (using text_length option),
        // then too large a font or too many characters may over-compress and push the glyphs to overlap,
        // so warn here of overflow or over-compress. A factor of 1.6 more characters than width
        // still allows bold characters to not quite collide or overlap.
        if (title_svg_length > derived().image_.x_size() * 1.6)
        { // Issue Warning that title is too long or too big font!
          std::cout << "Title text \"" << derived().title_info_.text()
            << "\"\n  with an estimated width " << title_svg_length << " (SVG units) may overflow plot space " << derived().image_.x_size()
            <<  "\n  or over-compress text with compression ratio " << title_svg_length / derived().image_.x_size()
            << ".\n  Reduce font size from " << derived().title_font_size()
            << ", or number of characters from " << derived().title().size()
            << ", or increase image size from " << derived().image_.x_size()
            << "?." << std::endl;
          // Title "Plot showing short legend &#x26; sizes."
          //   width 378 (SVG units) may overflow plot image!
          //  (Reduce font size from 30, or number of characters from 21, or increase image size from 300).
        }
        */
        derived().title_info_.x(derived().image_.x_size() / 2.); // Center of image.
        double y = derived().title_info_.textstyle().font_size() * derived().text_margin_; // Leave a margin space above.
        // default text_margin_(2.), // for title & axis label text, as a multiplier of the font size.
        // Should allow a 'half line space' above and below the label text.
        // But text_margin == 1. is rather tight?
        derived().title_info_.y(y);
        derived().image_.g(PLOT_TITLE).push_back(new text_element(derived().title_info_));
      } // void draw_title()

  //! Size_legend_box Calculate how big the legend box needs to be to hold
  //! any legend title and
  //! the data point markers (symbols or shapes),
  //! and any line marks showing lines used joining points,
  //! and any data series descriptor text(s).
  template <class Derived>
  void axis_plot_frame<Derived>::size_legend_box()
  {
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
    std::cout << "***Size_legend_box***" << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS
    if (derived().legend_on_ == true)
    { // So want a legend box, set with .legend_on(true).
      // Check if any legend title or title.
      derived().is_legend_title_ = (derived().legend_title_.text() != "");
      derived().legend_title_font_size_ = derived().legend_title_style_.font_size_; // legend title font size.
      derived().legend_text_font_size_  = derived().legend_text_style_.font_size_; // legend data series marker descriptions text_font size. text font size.
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "Title " << derived().legend_title_.text() << ", .legend_title_font_size_ = " << derived().legend_title_font_size_
        << ", .legend_text_font_size_ = " << derived().legend_text_font_size_ << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS

      // Vertical spacing needs to use the largest of
      // all point marker Unicode symbol font sizes (may be different) and legend text font size.
      // https://www.w3.org/TR/SVG/text.html#GlyphsMetrics
      // Line sampple has small height compared to any font.

      size_t num_series = derived().serieses_.size(); // How many data series in this plot.
      derived().biggest_point_marker_font_size_ = derived().legend_text_font_size(); // Assume text until a bigger symbol is found.
      std::cout << "Assume text font derived().biggest_point_marker_font_size_ " << derived().biggest_point_marker_font_size_ << std::endl;
      // Below get biggest marker symbol point size in any series to get minimum vertical spacing between data series info lines.
      double longest_text = 0;
      std::size_t longest_text_chars_count = 0;
      derived().biggest_point_marker_font_size_ = 0;
      size_t longest_series_text_number = 0;
      for (size_t i = 0; i != num_series; ++i)
      {
        double point_size = 0.;
        if (derived().serieses_[i].point_style_.shape() != none)
        { // Some data pointer marker.
          derived().is_a_point_marker_ = true; // So will need to provide a space for any other series without a point marker.
          point_size = derived().serieses_[i].point_style_.size(); //
          std::cout << "point size series " << i << ", size " << point_size << std::endl;
          if (point_size > derived().biggest_point_marker_font_size_)
          {
            derived().biggest_point_marker_font_size_ = point_size;
            std::cout << "new biggest point size series " << i << ", size " << point_size << std::endl;
          }
        } // if shape != none

        if (derived().serieses_[i].line_on() == true)
        { // At least one series has a line joining points.
          derived().is_a_data_series_line_ = true; // So will need to provide a space for any other series without a line.
        }

        if (derived().serieses_[i].title_ != "")
        { // Some data series text descriptor.
          derived().is_a_data_series_text_ = true; // So will need to allow space for any data series without text.
          double series_string_length = string_svg_length(derived().serieses_[i].title_, derived().legend_text_style_);
          // string_svg_length avoids chars as Unicode hex increasing the length wrongly,
          // so each Unicode char counts only as one char.  Would be better to use the SVG method of estimating true length.

#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
          std::cout << " series title " << i << " " << derived().serieses_[i].title_
            << ", text string length " << series_string_length << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS
          if (series_string_length > longest_text)
          { // longest found so far.
            longest_text = series_string_length;
            longest_text_chars_count = derived().serieses_[i].title_.size();
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
            std::cout << " Greater SVG width from series " << i << ", width = " << series_string_length
              << ", longest_text_chars_count = " << longest_text_chars_count << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS
          }
        } // if a data series description text line.
      } // for num_series
      std::cout << "Biggest of text and title font derived().biggest_point_marker_font_size_ " << derived().biggest_point_marker_font_size_ << std::endl;

      // Compute Y-axis vertical spacing.
      derived().vertical_title_spacing_ = derived().legend_title_font_size_ * derived().text_margin_; // Legend header/title vertical spacing.
      //   text_margin = 1.25; //!< Add a fraction of font height to allow for descenders. 
      derived().vertical_line_spacing_ = derived().legend_text_font_size_ * derived().text_margin_; // Legend data_series text vertical spacing.
      // Add a fraction of the largest of the legend text or, if no legend title then the marker_symbol/line/text.
      derived().vertical_marker_spacing_ = derived().biggest_point_marker_font_size_ ; // Suits line spacing of markers, lines and text.
      // ?? Should this be * derived().text_margin_ - and no spaces between lines?

      // Compute X-axis horizontal spacing.
      derived().horizontal_title_spacing_ =      derived().legend_title_font_size_ * aspect_ratio; // legend_font width, used as a font.
      derived().horizontal_line_spacing_ = derived().legend_text_font_size_ * aspect_ratio; // legend_font width, line width, also used if no line to show in a series.
      derived().horizontal_marker_spacing_ = derived().biggest_point_marker_font_size_ * 0.72 * aspect_ratio; // Width of biggest marker used if no marker on a series).

// #ifdef BOOST_SVG_POINT_DIAGNOSTICS
      #ifdef      BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "**Spacings"
        "\nLegend_text_font_size_ = " << derived().legend_text_font_size_
        << ", Legend_title_font_size_ = " << derived().legend_title_font_size_
        << ", text_margin = " << derived().text_margin_
        << ", aspect ratio =  " << aspect_ratio
        << ", Vertical_title_spacing = " << derived().vertical_title_spacing_
        << ", Vertical_line_spacing = " << derived().vertical_line_spacing_
        << ", Vertical_marker_spacing = " << derived().vertical_marker_spacing_
        << ", horizontal_spacing = " << derived().horizontal_title_spacing_
        << ", horizontal_line_spacing = " << derived().horizontal_line_spacing_
        << ", horizontal_marker_spacing = " << derived().horizontal_marker_spacing_
        << std::endl;
#endif //BOOST_SVG_POINT_DIAGNOSTICS

      // X-Axis Compute the width of the longest data series marker and/or line and/or textline.
      double text_width = longest_text; // Actual char text as SVG units (default pixels).

      if (derived().is_a_point_marker_ == true)
      { // Markers for data points in a data series.
        text_width += derived().biggest_point_marker_font_size_ * aspect_ratio; // Data point marker, circlet, corss ...
        text_width += derived().biggest_point_marker_font_size_ * aspect_ratio; // and a same size space after the marker.
      }
      if (derived().is_a_data_series_line_ == true)
      { // Line used to join data points in a data series.
        text_width += derived().horizontal_marker_spacing_; // Line width.
        text_width += derived().horizontal_marker_spacing_; // Space after line.
      }
      text_width += derived().horizontal_title_spacing_; // text font space.
      std::cout << "Legend text string_svg_length = " <<  text_width << std::endl;

      // Compute width of title line.
      double title_width  = // Longest SVG of legend_title.
      string_svg_length(derived().legend_title_.text(), derived().legend_title_style_);
      title_width += derived().horizontal_title_spacing_;
      title_width += derived().horizontal_title_spacing_;
      std::cout << "Legend title string_svg_length = " <<  title_width << std::endl;

      bool use_title_width = false;
      // Use longest of legend title and longest_text.
      if (title_width > text_width)
      {
        derived().legend_widest_line_ = title_width;
        use_title_width = true;
        std::cout << "Using title_width " << title_width << " rather than text width " << text_width << std::endl;
        derived().legend_title_style_.text_length(title_width);
        std::cout << "Using title_width " << title_width << " for text_length " << derived().legend_title_style_.text_length() << std::endl;
      }
      else
      {
        derived().legend_widest_line_ =  text_width;
        std::cout << "Using text_width " << text_width << " rather than title width " << title_width << std::endl;
      }

#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "\nSet_legend box dimensions:"
        "\nLegend title \"" << derived().legend_title_.text()
        << ", chars = " << derived().legend_title_.text().size()
        << ", .legend_title_font_size_ = " << derived().legend_title_font_size_
        << ", .legend_text_font_size_ = " << derived().legend_text_font_size_
        << ",\n .legend_title_style_ = " << derived().legend_title_style_
        << ",\n .legend_text_style_ = " << derived().legend_text_style_
        << ",\n .legend_widest_line_ = " << derived().legend_widest_line_ << " svg units."
        << " or " << string_svg_length(derived().legend_title_.text(), derived().legend_title_style_) << " SVG units"
        << ", .biggest_point_marker_font_size_ = " << derived().biggest_point_marker_font_size_
        << ",\n longest text line " << longest_text
        << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS

      // X-axis Compute final legend box width.
      derived().legend_width_ = 2 * derived().legend_box_.margin_; // Always allow a tiny margin top and bottom.
      if (derived().legend_box_.border_on_ == true)
      {  // If a border, allow for its width left side.
        derived().legend_width_ += derived().legend_box_.border_width_;
      }
    //  derived().legend_width_ += 2 * derived().horizontal_title_spacing_; // Allow a blank space around both sides.
      derived().legend_width_ +=  1.5 * derived().horizontal_title_spacing_; // Allow a blank space around both sides is tighter.

      if (use_title_width == false)
      { // The markers, lines and text control the width.
        derived().legend_width_ += derived().legend_widest_line_;
      }
      else
      { // title determines the width entirely.
        derived().legend_width_ += derived().legend_widest_line_ * 0.72;
      }
      if (derived().legend_box_.border_on_ == true)
      {  // If a legend box border, allow for its width right side.
        derived().legend_width_ += derived().legend_box_.border_width_;
      }

#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "Legend box margin = " << derived().legend_box_.margin()
        << ", legend_box border width = " << derived().legend_box_.margin_ << std::endl;
      // Legend width 274.5, height 140
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS

      // Y-axis Compute legend box height.
      // legend_height must be *at least* enough for any legend title and text_margin(s) for descenders.
      // Or size of marker or line, whichever is the biggest, plus a small margin_ top and bottom.

       // Use height of whichever is the biggest of data point marker shape and text-font, derived().biggest_point_marker_font_size_
      // for vertical spacing, (assuming horizontal line markers have neglible height so fill use marker text font size).

      //derived().legend_height_ = derived().legend_box_.margin_; // Always allow a tiny margin top and bottom.
      if (derived().legend_box_.border_on_ == true)
      {  // If a legend box border, allow for its width top side.
        derived().legend_height_ += derived().legend_box_.border_width_;
      }

      derived().legend_height_ += derived().vertical_title_spacing_ /3; // Fraction of title-font space top of legend box.
      // to get origin below the top ready for any text.
      if ((derived().is_legend_title_) // A legend title line is wanted,
        && (derived().legend_title_.text() != "")) // but ignore legend title if an empty string.
      {
        derived().legend_height_ += derived().vertical_title_spacing_; // Legend title line
      }
      // Add more height depending on the number of lines of data point markers, lines and text.
      derived().legend_height_ += derived().vertical_marker_spacing_ * num_series; // 

      //derived().legend_height_ += derived().vertical_marker_spacing_; // a part space after based on the marker line.
      derived().legend_height_ += derived().vertical_title_spacing_; // a part space after based on the legend title font.

      if (derived().legend_box_.border_on_ == true)
      {  // If a legend box border, allow for its bottom side border width.
        derived().legend_height_ += derived().legend_box_.border_width_;
      }
//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "Legend width " << derived().legend_width_ << ", height " << derived().legend_height_ << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS

    }// legend_on_ == true
    else
    { // derived().legend_on_ == false means no legend wanted,
      // so set values to show legend positions invalid?
      derived().legend_height_ = 0.; // At least set the size to zero.
      derived().legend_width_ = 0.;
      //derived().legend_left_ = -1.;
      //derived().legend_right_ = -1.;
      //derived().legend_top_ = -1.;
      //derived().legend_bottom_ = -1.;
      return;
      }
    } //  void size_legend_box()

    template <class Derived>
    void axis_plot_frame<Derived>::place_legend_box()
    { //! Place legend box (if required).
      //! Default legend position is outside top right, level with plot window.
      if(derived().legend_on_ == true) // A legend box specified.
      {
        derived().outside_legend_on_ = true; // Unless legend is set to be inside.
        // Space around any legend box - beyond any plot or legend border.
        const double spacing = derived().legend_text_style_.font_size() * aspect_ratio; // = one legend title font width.
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
        std::cout << "Margin (and image border) between legend box and plot window  = " << spacing << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS
        switch (derived().legend_place_)
        {
        case nowhere:
          std::cout << "Legend box put nowhere!" << std::endl;  // Should not happen!
          return; // Actually places it at (0, 0), probably overwriting the plot!
        case somewhere:
          // Assume legend_top_left will place it somewhere where there is nothing else.
            derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
            derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
            break;
        case inside:
          derived().outside_legend_on_ = false;
          if (derived().legend_left_ == -1)
          { // Legend box position NOT been set by legend_top_left.
            // Default inside position is top left level with plot window.
          derived().legend_left_ = derived().image_border_.border_width_ + derived().image_border_.margin_; // left image edge + space.
          //derived().plot_left_ += derived().legend_width_ + spacing; // Push plot window right same amount to make room,
          derived().plot_left_ += derived().legend_width_; // Push plot window right same amount to make room,
          derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
          derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
          derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
        }
          else
          { // Legend position has been specified by legend_top_left.
            break;
          }
          break;

        // If legend is set to be outside plot window then reserve space for legend by reducing plot window.

        case outside_right: // Default legend position is top outside_right,
          // so that it isn't too close to the image edge or the plot window.
          derived().plot_right_ -= (derived().legend_width_ - spacing) ; // Narrow plot window from the right.
          derived().legend_left_ = derived().plot_right_ + spacing; // plot + border.
          derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
          derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
          derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
          break;
        case outside_left: // Legend position is outside left
          derived().plot_left_ += derived().legend_width_ - spacing; // Push plot window right same amount to make room,
          derived().legend_left_ = derived().image_border_.border_width_ + derived().image_border_.margin_; // left image edge + space.
          derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
          derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
          derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
          break;
        case outside_top:
          // Centered.
            derived().legend_left_ = derived().image_.x_size() / 2. - derived().legend_width_ / 2; // Center.
            derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
            derived().plot_top_ += derived().legend_height_ + spacing;
            derived().legend_top_ = derived().title_info_.y() + derived().title_font_size() * derived().text_margin_;
            derived().legend_top_ += spacing;
            derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
          break;
        case outside_bottom:
            // Centered.
            derived().legend_bottom_ = derived().image_.y_size();
            derived().legend_bottom_ -= (derived().image_border_.border_width_ + derived().image_border_.margin_); // up
            derived().legend_top_ = derived().legend_bottom_ - derived().legend_height_;
            derived().legend_left_ = derived().image_.x_size()/  2. - derived().legend_width_ / 2; // Center.
            derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
            derived().plot_bottom_ = derived().legend_top_;
            derived().plot_bottom_ -= 2 * spacing;
          break;
          } // switch
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
        std::cout << "Placed Legend box: left = " << derived().legend_left_
            << ", right = " << derived().legend_right_
            << ", width = " << derived().legend_width_
            << ", top = " << derived().legend_top_
            << ", bottom = " << derived().legend_bottom_
            << ", height = " << derived().legend_height_
            << ", Plot window box: left = " << derived().plot_left_
            << ", right = " << derived().plot_right_
            << ", top = " << derived().plot_top_
            << ", bottom = " << derived().plot_bottom_
            << std::endl;
        // Placed Legend box: left = 554, right = 682.4, width = 120, top = 48, bottom = 358, height = 310,
        // Plot window box: left = 26, right = 545.6, top = 48, bottom = 471
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS
          // Now that we know the size of legend box needed,
          // check if the location requested will fit.
          if ( (derived().legend_left_ < 0) || (derived().legend_left_ > derived().image_.x_size()))
          { // left legend box would be outside image?
            std::cout << "Legend left edge" << derived().legend_left_
              << " is outside image X-size = " << derived().image_.x_size()  << "!" << std::endl;
          }
          if ((derived().legend_right_ < 0) || (derived().legend_right_ > derived().image_.x_size()))
          { // right legend box would be outside image?
            std::cout << "Legend right edge " << derived().legend_right_
              << " is outside image X-size = " << derived().image_.x_size()  << "!" << std::endl;
          }
          if ((derived().legend_top_ < 0) || (derived().legend_top_ > derived().image_.y_size()))
          { // legend-box top edge outside image?
            std::cout << "Legend top edge " << derived().legend_top_
              << " outside image Y-size " << derived().image_.y_size()  << "!" << std::endl;
          }
          if ((derived().legend_bottom_  < 0 ) || (derived().legend_bottom_ > derived().image_.y_size()))
          { // egend box  bottom edge outside image?
            std::cout << "Legend bottom edge " << derived().legend_bottom_
              << " outside image Y-size " << derived().image_.y_size() << "!" << std::endl;
          }
          // Actual drawing of legend border and background in draw_legend() below.
        } // if legend_on_
      } //  void place_legend_box()

//! *************************************************************************************
    //! Draw the legend box.
    //! border and background,
    //! (using the size and position computed by function @c size_legend_box),
    //! and legend_title (if any and if required),
    //! and any data point marker lines,
    //! and any shapes for data point markers,
    //! and any data series descriptor text(s).
    template <class Derived>
    void axis_plot_frame<Derived>::draw_legend()
    {
      size_t num_series = derived().serieses_.size(); // How many data series.
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "Drawing Legend box border width = " << derived().legend_box_.width()
        <<  ", margin = " << derived().legend_box_.margin()
      // Legend box margin = 2, legend_box width = 1, legend width = 2
      << "\n" << num_series << " data series." << std::endl;
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS

#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
        std::cout <<  "Drawing Legend: legend_title_font_size " << derived().legend_title_font_size_
          << ", legend text font size = "  << derived().legend_text_font_size_
          << ", biggest marker symbol font size = "  << derived().biggest_point_marker_font_size_
          << ", text_margin = " << derived().text_margin_ << std::endl;
        // Drawing Legend: text_font_size 10, marker shape size = 10, marker symbol font size = 10, spacing = 10
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS

      // Assume legend box position has already been sized by function size_legend_box,
      // and positioned by function place_legend_box.
      double legend_x_start = derived().legend_left_; // Saved legend box location.
      double legend_width = derived().legend_width_;
      double legend_y_start = derived().legend_top_;
      double legend_height = derived().legend_height_;
#ifdef BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout <<  "Drawing Legend box: x = " << legend_x_start
        << ", legend Y_start = " << legend_y_start
        << ", width = "  << legend_width
        << ", height = "  << legend_height
        << std::endl;
      // Drawing Legend box: x = 554, legend Y_start = 48, width = 120, height = 310
      // Placed Legend box: left = 562.4, right = 682.4, width = 120, top = 48, bottom = 358, height = 310,
      // Plot window box: left = 26, right = 554, top = 48, bottom = 471
#endif // BOOST_SVG_LEGEND_DIAGNOSTICS
      // Select the legend layer.
      g_element* g_ptr = &(derived().image_.g(PLOT_LEGEND_BACKGROUND));

      // Set legend and background style.
      derived().image_.g(detail::PLOT_LEGEND_BACKGROUND)
        .style().fill_color(derived().legend_box_.fill()) //
        .stroke_color(derived().legend_box_.stroke())
        .stroke_width(derived().legend_box_.width())
        .stroke_on(derived().legend_box_.border_on())
        ;

      // Draw border box round legend with background.
      g_ptr->push_back(new rect_element(legend_x_start, legend_y_start, legend_width, legend_height));
      // Placed Legend box: left = 554, right = 682.4, width = 120, top = 48, bottom = 358, height = 310,
      // Plot window box: left = 26, right = 545.6, top = 48, bottom = 471

#ifdef      BOOST_SVG_LEGEND_DIAGNOSTICS
      std::cout << "***Legend Spacings:"
        "\nLegend_text_font_size_ = "  << derived().legend_text_font_size_
        << ", Legend_box_.margin_ = " << derived().legend_box_.border_width_
        << ", Legend_title_font_size_ = " << derived().legend_title_font_size_
        << ", text_margin = " << derived().text_margin_
        << ", aspect ratio =  " << aspect_ratio
        << ", Vertical_title_spacing = " << derived().vertical_title_spacing_
        << ", Vertical_line_spacing = " << derived().vertical_line_spacing_
        << ", Vertical_marker_spacing = " << derived().vertical_marker_spacing_
        << ", horizontal_spacing = " << derived().horizontal_title_spacing_
        << ", horizontal_line_spacing = " << derived().horizontal_line_spacing_
        << ", horizontal_marker_spacing = " << derived().horizontal_marker_spacing_
        << std::endl;
#endif //BOOST_SVG_POINT_DIAGNOSTICS

      double legend_y_pos = legend_y_start;
      if (derived().legend_box_.border_on_ == true)
      {
        legend_y_pos += derived().legend_box_.border_width_;  // If a border, don't write on it!
      }
     // legend_y_pos += derived().legend_box_.margin_; // Always allow a tiny margin top and bottom?
      legend_y_pos += derived().vertical_title_spacing_ /3 ; // space before title as a fraction of title font.

      if (derived().legend_title_.text() != "") //
      { // Is a legend title, so draw the centered legend title text for example: "My Plot Legend".
        derived().legend_title_.x(legend_x_start + legend_width / 2.); // / 2. to center horizontally in legend box.
        // Might be better to use center_align here because may fail if legend contains symbols in Unicode?
        legend_y_pos += derived().vertical_title_spacing_; // Ready to write title.
        derived().legend_title_.y(legend_y_pos);
        derived().image_.g(PLOT_LEGEND_TEXT).push_back(new text_element(derived().legend_title_));
        legend_y_pos += derived().vertical_title_spacing_ /3; // Leave a fraction space below legend title.
      } // is_header aka is_title

      g_ptr = &(derived().image_.g(PLOT_LEGEND_POINTS)); // Prepare to write marker, line and description text into legend box.
      g_element* g_inner_ptr = g_ptr;
      g_inner_ptr = &(derived().image_.g(PLOT_LEGEND_TEXT)); // Write legend title text into legend box.

      // Show any point marker, maybe line, & maybe text info for each of the data series.
      for(unsigned int i = 0; i != derived().serieses_.size(); ++i)
      { 
#ifdef BOOST_SVG_POINT_DIAGNOSTICS
          std::cout << "Data Series #" << i << " point_style = " <<  derived().serieses_[i].point_style()
            << "\n line style = " << derived().serieses_[i].line_style()
            << "\n point_symbol_style = " <<  derived().serieses_[i].point_font_style()  << std::endl;
#endif //BOOST_SVG_POINT_DIAGNOSTICS
        double legend_x_pos = legend_x_start + derived().legend_box_.margin();
        if (derived().legend_box_.border_on_ == true)
        {
          legend_x_pos += derived().legend_box_.border_width_;  // If a border, don't write on it!
        }
        legend_x_pos += derived().horizontal_title_spacing_; // space before point marker and/or line & text.
        legend_x_pos += derived().horizontal_marker_spacing_;  // Center of point marker symbol.

        legend_y_pos += derived().vertical_marker_spacing_ ; // size of biggest marker or description text font.

        g_inner_ptr = &(g_ptr->add_g_element());
        // Use both stroke & fill colors from the point marker's style.
        // Applies to both shapes AND lines.
        g_inner_ptr->style().stroke_color(derived().serieses_[i].point_style_.stroke_color_);
        g_inner_ptr->style().fill_color(derived().serieses_[i].point_style_.fill_color_);
        g_inner_ptr->style().stroke_width(derived().serieses_[i].line_style_.width_);
#ifdef BOOST_SVG_POINT_DIAGNOSTICS
//    std::cout << "g_inner_ptr.style().stroke_color() " << g_inner_ptr->style() << std::endl;
#endif //BOOST_SVG_POINT_DIAGNOSTICS
        plot_point_style& point_style = derived().serieses_[i].point_style_;

        if(point_style.shape_ != none)
        { // Is some data point marker shape to show in legend box.
          // ellipse is special case to show uncertainty of data point.
          bool was_unc_ellipse = false;
          if (point_style.shape_ == unc_ellipse)
          {  // Problem here with unc_ellipse with calculation of a suitable size
              // and also, more fundamentally, the legend box overwrites the PLOT_DATA_UNC layers,
            point_style.shape_ =  egg; // so as a hack, use a Unicode egg instead. 2B2C to 2B2F
            was_unc_ellipse = true; // Note so can restore after showing circle.
          }

          // Show a SVG plot point like star, circlet ...
          draw_plot_point(
            legend_x_pos,
            legend_y_pos - point_style.size_ / 5, // Move up a bit of a marker font size to align with text.
            *g_inner_ptr,
            point_style,
            unc<false>(), unc<false>());  // X and Y position.
            // was derived().serieses_[i].point_style_, unc(0.), unc(0.));
          legend_x_pos += derived().horizontal_marker_spacing_ * 2; // Trailing space after point-marker.

          if (was_unc_ellipse)
          { // Restore from using egg (or the data points won't use the unc_ellipse!)
            point_style.shape_ = unc_ellipse;
          }
        } // A marker shape for this series.
        else
        { // Other data series have a point marker (but not this one).
          if (derived().is_a_point_marker_ == true)
          {
            legend_x_pos += derived().horizontal_marker_spacing_ * 2;  // Leave a space where marker would be.
          }
        }

        // Line markers are really useful for 2-D lines and curves showing functions.
        if (derived().serieses_[i].line_style_.line_on_ == true) // Line joining points option is true,
        { // so need to draw a short line to show color for line joining points for that data series.
          // derived().legend_lines_ = true; // Some line is drawn for at least one data series.
          // Better - this should be set during the legend box sizing?
          if (derived().serieses_[i].line_style_.line_on_ || derived().serieses_[i].line_style_.bezier_on_)
          { // Use stroke color from line style.
            g_inner_ptr->style().stroke_color(derived().serieses_[i].line_style_.stroke_color_);
          }
          else
          { // Use point stroke color instead.
            g_inner_ptr->style().stroke_color(derived().serieses_[i].point_style_.stroke_color_); // OK with 1D.
          }
          g_inner_ptr->style().stroke_width(derived().serieses_[i].line_style_.width_);
          //std::cout << "line g_inner_ptr->style().stroke_color() " << g_inner_ptr->style().stroke_color() << std::endl;
          g_inner_ptr->push_back(new line_element( // Draw horizontal lines with appropriate color.
            legend_x_pos,
            legend_y_pos,
            legend_x_pos + derived().horizontal_line_spacing_, // Line sample is one char long/wide.
            legend_y_pos));
          legend_x_pos += derived().horizontal_line_spacing_; // Advance by short line distance
          legend_x_pos += derived().horizontal_title_spacing_; // & a space.
        } // legend line to draw
        else
        { // Might need a horizontal space if any other series have a line (but this series does not).
          if (derived().is_a_data_series_line_ == true)
          {
            legend_x_pos += derived().horizontal_line_spacing_; // Total is short line
            legend_x_pos += derived().horizontal_title_spacing_; // & a space.
          }
        } // line_on == true

        // Legend text for each Data Series added to the plot.
        if (derived().is_a_data_series_text_)
        {
          g_inner_ptr = &(derived().image_.g(PLOT_LEGEND_TEXT));
          g_inner_ptr->push_back(new text_element(
            legend_x_pos, // Allow space for the marker.
            legend_y_pos,
            derived().serieses_[i].title_, // Text for this data series.
            derived().legend_text_style_,
            left_align));
        }
      } // for
      legend_y_pos += derived().vertical_marker_spacing_; // Fraction of biggest font, ready for next line.
    } // void draw_legend()


          template <class Derived>
          void axis_plot_frame<Derived>::draw_plot_point(double x, double y, // X and Y values (in SVG coordinates).
            g_element& g_ptr,
            plot_point_style& point_style,
            unc<false> ux, unc<false> uy) // Default unc ux = 0. and uy = 0. ?  Meas?
          { /*! Draw a plot data point marker shape or symbol
              whose size and stroke and fill colors are specified in plot_point_style sty,
              possibly including uncertainty ellipses showing multiples of standard deviation.
            */
            /*
              For 1-D plots, the points do not *need* to be centered on the X-axis,
              and putting them just above, or sitting on, the X-axis is much clearer.
              For 2-D plots, the symbol center should, of course, be centered exactly on x, y.
              circle and ellipse are naturally centered on the point.
              for rectangle x and y half_height offset centers square on the point.
              But symbols are in a rectangular box and the offset is different for x & y
              even assuming that the symbol is centered in the rectangle.
              the vertical and horizontal ticks are deliberately offset above the axes.
              TODO Not sure this is fully resolved.
            */
            double point_size = point_style.size_;
            double half_height = point_size / 2.; // Offset by half the symbol size to try to centre symbol on the point coordinates.
            double half_width = point_size / 2.; // Offset by half the symbol size to try to centre symbol on the point x-coordinates.
            double third_height = point_size / 3.1; // Offset y vertical by third of the symbol size to try to centre symbol on the point y-coordinates.

#ifdef BOOST_SVG_POINT_DIAGNOSTICS
            std::cout << "point_style.size_ = "<< point_style.size_ << ", y offset half_height - " << half_height << std::endl; // Picks up font size shape(diamond).size(20) correctly here.
#endif // BOOST_SVG_POINT_DIAGNOSTICS
            point_style.symbols_style_.font_size(point_style.size_); // Sets plot_point marker font size.

            //  want this text_styling
            //point_style.symbols_style_.font_family("arial");
            //point_style.symbols_style_.font_weight("bold");
            //point_style.symbols_style_.font_decoration("underline");
            //point_style.symbols_style_.font_stretch("wider");
            //point_style.symbols_style_.font_style("italic");

            //point_style.symbols_style_.fill_color(sty.fill_color_); //
            //point_style.symbols_style_.stroke_color(sty.stroke_color_); //

#ifdef BOOST_SVG_POINT_DIAGNOSTICS
            std::cout << "point style() = "<< point_style.style() << std::endl;
            // Example: point style() = text_style(10, "Lucida Sans Unicode", "", "", "", "")  size is correct here now.
#endif // BOOST_SVG_POINT_DIAGNOSTICS
            // Whatever shape, text or line, want to use the point style.
            g_ptr.style().stroke_color(point_style.stroke_color_);
            g_ptr.style().fill_color(point_style.fill_color_);
#ifdef BOOST_SVG_POINT_DIAGNOSTICS
            std::cout << "plot point marker g_ptr.style() = " << g_ptr.style() << std::endl;
            // g_ptr.style() svg_style(RGB(255,255,0), RGB(255,0,0), 2, fill, stroke, width)
#endif // BOOST_SVG_POINT_DIAGNOSTICS
            switch(point_style.shape_) // Chosen from enum point_shape none, round, square, point, egg
            {
            case none:
              break; // Nothing to display.

            // Shapes using SVG line, circle or eclipse functions.

            // Now a Unicode, see below.
            //case circlet: // Use SVG circle function.
            //  g_ptr.circle(x, y, half_height);
            //  break;

            case point:
              g_ptr.circle(x, y, 1.); // Fixed size 1 pixel round.
              break;

            //case square: // Now using Unicode symbol.
            //  g_ptr.rect(x - half_width, y - half_height, point_size, point_size);
            //  break;

            case egg:
              // No need for x or y - half width to center on point.
              g_ptr.ellipse(x, y, half_height, point_size * 1.); // Tall thin egg!
              // of use UNicode ? 2B2D horixontal or 2B2F vertical.
              break;

            case unc_ellipse:
              { // std_dev horizontal (and, for 2D, vertical) ellipses for one, two and three standard deviations.
                double xu = ux.value(); //
                if (ux.std_dev() > 0)
                { // std_dev is meaningful.
                  xu +=  ux.std_dev();
                }
                transform_x(xu); // To SVG coordinates.
                double x_radius = std::abs<double>(xu - x);
                if (x_radius <= 0.)
                { // Make sure something is visible.
                  x_radius = 1.; // Or size?
                }

                double yu = uy.value();
                if (uy.std_dev() > 0)
                { // std_dev is meaningful.
                   yu += uy.std_dev();
                }

                transform_y(yu);
                double y_radius = std::abs<double>(yu - y);
                if (y_radius <= 0.)
                { // Make sure something is visible.
                  y_radius = 1.;
                }
                //image_.g(PLOT_DATA_UNC).style().stroke_color(magenta).fill_color(pink).stroke_width(1);
                // color set in svg_1d_plot         data at present.
                g_element* gu3_ptr = &(derived().image_.g(PLOT_DATA_UNC3));
                g_element* gu2_ptr = &(derived().image_.g(PLOT_DATA_UNC2));
                g_element* gu1_ptr = &(derived().image_.g(PLOT_DATA_UNC1));
                gu1_ptr->ellipse(x, y, x_radius, y_radius); //  Radii are one standard deviation.
                gu2_ptr->ellipse(x, y, x_radius * 2, y_radius * 2); //  Radii are two standard deviation..
                gu3_ptr->ellipse(x, y, x_radius * 3, y_radius * 3); //  Radii are three standard deviation..
                g_ptr.circle(x, y, 1); // Show x and y values at center using stroke and fill color of data point marker.
              }
              break;

             // Offset from center is not an issue with vertical or horizontal ticks.
             // But is needed for text and Unicode symbols.
             //point_size / 4. puts bottom tip on the X-axis,
             //point_size / 2. put center on the X-axis.
             //x, y, center on the X-axis - probably what is needed for 2-D plots.

            case vertical_tick: // Especially neat for 1-D points.
              g_ptr.line(x, y, x , y - point_size); // tick up from axis.
              break;
            case vertical_line:
              g_ptr.line(x, y + point_size, x , y - point_size); // line up & down from axis.
              break;
            case horizontal_tick:
              // horizontal_tick is pretty useless for 1-D because the horizontal line is on the X-axis.
              g_ptr.line(x, y, x + point_size, y ); // tick right from axis.
              break;
            case horizontal_line:
              g_ptr.line(x, y - point_size, x + point_size, y ); // line left & right from axis.
              // horizontal_line is pretty useless for 1-D because the horizontal line is on the X-axis.
              break;

            //  Shapes as symbols using SVG text function, (NOT using SVG line, circle or eclipse).
            case symbol: // Unicode symbol.  see https://unicode-search.net/ for search
              g_ptr.text(x, y + third_height, point_style.symbols(), point_style.style(), center_align, horizontal); // symbol(s), size and center.

              // Unicode symbols that work on most browsers are listed at
              // boost\math\libs\math\doc\sf_and_dist\html4_symbols.qbk,
              // http://www.htmlhelp.com/reference/html40/entities/symbols.html
              // and  http://www.alanwood.net/demos/ent4_frame.html
              // Geometric shapes http://www.unicode.org/charts/PDF/Unicode-3.2/U32-25A0.pdf
              // Misc symbols http://www.unicode.org/charts/PDF/U2600.pdf
              // The Unicode value in decimal 9830 or hex x2666 must be prefixed with & and terminated with ;
              // @b Example: &x2666; for diamond in xml
              // and then enveloped with "" to convert to a std::string, for example: "&#x2666;" for diamond.=
#ifdef BOOST_SVG_POINT_DIAGNOSTICS
              std::cout << "Unicode symbol font size " << point_style.symbols_style_.font_size()
                << ", at SVG x = " << x << ", y = " << y + half_height<< std::endl;
#endif // BOOST_SVG_POINT_DIAGNOSTICS
              break;

            case square:
              g_ptr.text(x, y + third_height, "&#x25A1;", point_style.symbols_style_, center_align, horizontal);
              // 25A1 white square
              break;

            case circlet: // 25CB WHITE CIRCLE or 25EF large circle
              g_ptr.text(x, y + third_height, "&#x25CB;", point_style.symbols_style_, center_align, horizontal);
              // square 25A1  circle 25CB
              break;
            case diamond:
              g_ptr.text(x, y + third_height, "&#x2666;", point_style.symbols_style_, center_align, horizontal);

#ifdef BOOST_SVG_POINT_DIAGNOSTICS
            std::cout << "sty.symbols_style_ " << point_style.symbols_style_ << std::endl;
#endif // BOOST_SVG_POINT_DIAGNOSTICS
#ifdef BOOST_SVG_POINT_DIAGNOSTICS
              std::cout << "Diamond style font size " << point_style.symbols_style_.font_size() << std::endl;
#endif // BOOST_SVG_POINT_DIAGNOSTICS
              // diamond, spades, clubs & hearts fill with expected fill_color.
              break;
            case asterisk: // https://unicode-search.net/unicode-namesearch.pl?term=ASTERISK for options
              // Several options but ASTERISK OPERATOR #2217
              // U+FE61 SMALL ASTERISK centers OK but is small.
              // 2732 is open center asterisk.
              // 273C is open center TEARDROP-SPOKED ASTERISK
              g_ptr.text(x, y + third_height, "&#x273C;", point_style.symbols_style_, center_align, horizontal);
              // asterisk is black filled.
              // .
              break;
            case lozenge:
              g_ptr.text(x, y + third_height, "&#x25CA;", point_style.symbols_style_, center_align, horizontal);
              // size / 3 to get tip of lozenge just on the X-axis.
              // lozenge seems not to fill?
              break;
            case club:
              g_ptr.text(x, y + third_height, "&#x2663;", point_style.symbols_style_, center_align, horizontal);
              // x, y, puts club just on the X-axis.
              break;
            case spade:
              g_ptr.text(x, y + third_height, "&#x2660;", point_style.symbols_style_, center_align, horizontal);
              //
              break;
            case heart:
              g_ptr.text(x, y + third_height , "&#x2665;", point_style.symbols_style_, center_align, horizontal);
              break;
            case outside_window: // Pointing down triangle used only to show data points that are outside plot window.
              {
                bool fill = (point_style.fill_color() != blank);
                g_ptr.triangle(x - half_height, y - point_size, x + half_height, y - point_size, x, y, fill);
                // Last point puts the bottom tip of the triangle on the X-axis (so may not be suitable for 2-D).
              }
              break;
             // Triangles and cones.
             // https://unicode.org/charts/PDF/U25A0.pdf Geometric Shapes
              // There are large and small, and filled black or white centre.
             // Could use black center &#x25BE for pointing down triangle,
             // or &#x25B4 for small black center up-pointing triangle
             // or &#x25BE for white center small down triangle.
             // or &#x25BA for white center point right triangle.
            case cone: // Synonym for cone_point_up
            case cone_point_up: // pointing-up triangle, white centre.
              g_ptr.text(x, y + third_height, "&#x25BD;", point_style.symbols_style_, center_align, horizontal);
              // https://unicode.org/charts/PDF/U25A0.pdf
              break;

            case cone_point_down: // pointing-down triangle, white centre.
               g_ptr.text(x, y + third_height, "&#x25BD;", point_style.symbols_style_, center_align, horizontal);
               // https://unicode.org/charts/PDF/U25A0.pdf
               break;

            case cone_point_right: // small pointing-right triangle, white centre (or 25b7 for bigger one).
               g_ptr.text(x, y + third_height, "&#x25B9;", point_style.symbols_style_, center_align, horizontal);
               // <text x="489" y="109" text-anchor="middle" font-size="10" font-family="Lucida Sans Unicode">&#x25B9</text>
               break;

            case cone_point_left: // small pointing-left triangle, white centre.
               g_ptr.text(x, y + third_height, "&#x25C3;", point_style.symbols_style_, center_align, horizontal);
               // https://unicode.org/charts/PDF/U25A0.pdf  Or larger triangle 25C1
               break;

            case triangle: // Pointing-up triangle, white centre.
               g_ptr.text(x, y  + third_height, "&#x25B4;", point_style.symbols_style_, center_align, horizontal);
                 // Also could use &#x25BC for pointing down triangle, or &#x25B4 for small up-pointing triangle.
                 // https://unicode.org/charts/PDF/U25A0.pdf
               break;
             case star:
               g_ptr.text(x, y  + third_height, "&#x2605;", point_style.symbols_style_, center_align, horizontal);
               break;

            case cross: // Not X. Size is full font size for other options see https://unicode-search.net/unicode-namesearch.pl?term=CROSS
              //g_ptr.line(x, y + point_size, x , y - point_size); // line up & down from axis,
              //g_ptr.line(x, y - point_size, x + point_size, y ); // & line left & right from axis.
              // Cross is pretty useless for 1-D because the horizontal line is on the X-axis.
             // g_ptr.text(x, y  + third_height, "&#x274C;", point_style.symbols_style_, center_align, horizontal);
              g_ptr.text(x, y  + third_height, "&#x272F;", point_style.symbols_style_, center_align, horizontal);
              break;
            }
          } // void draw_plot_point

          template <class Derived>
          void axis_plot_frame<Derived>::draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, Meas uvalue)
          { /*!
          void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, unc<false> uvalue)
             Write one data point (X or Y) value as a string, for example "1.23e-2",
             near the data point marker.
             Unnecessary e, +, \& leading exponent zeros may optionally be stripped,
             and the position and rotation controlled.
             std_dev estimate, typically standard deviation
             (approximately half conventional 95% confidence "plus or minus")
             may be optionally be appended.
             Degrees of freedom estimate (number of replicates) may optionally be appended.
             ID or name of point, order in sequence, and datetime may also be added.
             For example: "3.45 +-0.1(10)"\n
             The precision and format (scientific, fixed), and color and font type and size can be controlled too.
             */
            double value = uvalue.value(); // Most likely value or mean.
            double sd = uvalue.std_dev(); // Standard deviation for value.
            double df = uvalue.deg_free(); // Degrees of freedom estimate for value.
            unsigned short int types = uvalue.types();  //  unctypes_
            distribution_type distrib;
            if (types & UNC_UNIFORM)
            {
              distrib = uniform;
            }
            else if (types & UNC_TRIANGULAR)
            {
              distrib = triangular;
            }
            else
            { // Default.
              distrib = gaussian;
            }

            // Extra info from Meas.
            int order = uvalue.order_;
            std::string label_id = uvalue.id_;
            using boost::posix_time::ptime;
            ptime dt = uvalue.time_;

            std::stringstream label;
            label.precision(val_style.value_precision_);
            std::string stripped;
            if (val_style.value_precision_ <= 0)
            { // Use uncertainty to automatically control number of digits.
              int m = round_m(derived().epsilon_, sd, derived().uncSigDigits_, distrib);
              // Assume no need for stripping unecessary e, +, & leading exponent zeros.
              stripped = round_ms(value, m);
            }
            else
            { // Use user's chosen precision etc.
              label.flags(val_style.value_ioflags_);
              label << value; // "1.2" or "3.4e+001"...
              stripped =  (derived().x_ticks_.strip_e0s_) ?
                // Default is to strip unecessary e, +, & leading exponent zeros.
                strip_e0s(label.str())  // "1.2" or "3.4e1"...
                :
              stripped = label.str();
            }
            if (val_style.prefix_ != "")
            { // Want a prefix like "[" or "time = ".
              stripped = val_style.prefix_ + stripped;
            }
            int marker_size = point_style.size_; // point marker size
            int label_size = val_style.values_text_style_.font_size();
            // Offset of value label from point must be related mainly to
            // size of the data marker, less the value label font size.
            // May need to combine these two?

            rotate_style rot = val_style.value_label_rotation_;
            // http://www.w3.org/TR/SVG/coords.html#RotationDefined
            // transform="rotate(-45)" == uphill

            align_style al; // = center_align;
            switch (rot)
            {
            case horizontal: // OK
              al = center_align;
              y -= marker_size * 2;  // Up marker font size;
              // center_align means no x correction.
              break;
            case leftward: // horizontal but to left of marker.
              al = right_align;
              x -= marker_size * 1.3;  // left
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case rightward: // horizontal but to right of marker.
              al = left_align;
              x += marker_size * 1.1;  // right
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case upsidedown: // OK but upsidedown so not very useful!
              al = center_align;
              y += marker_size;  // Down marker font size;
             break;
            case slopeup: // -30 - OK
            case steepup: // -45 - OK
            case uphill: // -60 - OK
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.6;  // UP marker font size;
              break;
            case upward: // -90 vertical writing up - OK.
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.9;  // Up marker font size;
              break;
            case backup: // OK
              al = right_align;
              x -= marker_size * 1.5;  // Left
              y -= marker_size * 0.8;  // Up
              rot = downhill;
              break;

            case  slopedownhill: // 30 gentle slope down.
            case downhill: // 45 down.
            case steepdown: //  60 steeply down.
             al = left_align;
              x += marker_size * 0.4;  // Right;
              y += marker_size * 0.9;  // Down
              break;
            case downward: // OK
              al = left_align;
              x -= marker_size;  // Left
              y += marker_size;  // Up
             break;
            case backdown: // OK
              al = right_align;
              x -= marker_size * 0.5;  // Left
              y += marker_size * 1.5;  // down
              rot = uphill;
             break;
            } // switch
            text_element& t = g_ptr.text(x, y, stripped, val_style.values_text_style_, al, rot);  // X or Y value "1.23".
            int udf_font = static_cast<int>(val_style.values_text_style_.font_size() * reducer);
            // TODO what does this reducer do?

            std::string label_u; // std_dev or text_plusminus.
            // std::string label_df; // Degrees of freedom estimate.
            std::string pm_symbol = "&#x00A0;&#x00B1;"; //! Unicode space text_plusminus glyph.
            // Might also use ANSI symbol for text_plusminus 0xF1 == '\361' or char(241)
            // but seems to vary with different codepages:
            // LOCALE_SYSTEM_DEFAULT LOCALE_IDEFAULTANSICODEPAGE == 1252
            // LOCALE_SYSTEM_DEFAULT  LOCALE_IDEFAULTCODEPAGE ==  850 for country 44 (UK)
            // And seems to vary from console to printable files.
            // Spaces seem to get lost, so use 00A0 as an explicit space glyph.
            // Layout seems to vary with font - Times New Roman leaves no space after.
            //text_element& t = g_ptr.text(x, y, label_v, val_style.values_text_style_, al, rot);
           // Optionally, show std_dev as 95% confidence plus minus:  2.1 +-0.012 (23)

            // Extra info from Meas.
            using boost::posix_time::ptime;

            if ((val_style.plusminus_on_ == true) // text_plusminus uncertainty is wanted,
                && (sd > 0.) // and std_dev is a valid std_dev estimate.
              )
            {  // std_dev estimate usually expressed 67% confidence interval + or - standard deviation.
              sd *= derived().text_plusminus_; // typically + or - standard deviation.
              label_u = sv(sd, val_style, true); // stripped.
              t.tspan(pm_symbol).fill_color(val_style.plusminus_color_);
              t.tspan(label_u).fill_color(val_style.plusminus_color_).font_size(udf_font);
            }
            if (val_style.addlimits_on_ == true)
            { // Want confidence interval appended, for example: <1.23, 1.45>
              //alpha = 0.05; // oss.iword(confidenceIndex) / 1.e6; // Pick up alpha.
              //double epsilon = 0.01; // = oss.iword(roundingLossIndex) / 1.e3; // Pick up rounding loss.
              //int uncSigDigits = 2; // = oss.iword(setUncSigDigitsIndex);  // Pick up significant digits for uncertainty.
              //bool isNoisyDigit = false; // Pick up?
              if(derived().isNoisyDigit_)
              {
                derived().uncSigDigits_++;
              }
              std::pair<double, double> ci = conf_interval(value, sd, df, derived().alpha_, distrib);
              int m = round_m(derived().epsilon_, sd, derived().uncSigDigits_, distrib);
              using boost::lexical_cast;
              std::stringstream label_ci;
              label_ci << " &lt;" // '<' 003C is an XML predefined entity, so use name.
                  << lexical_cast<double>(round_ms(ci.first, m)) << ", "
                  << lexical_cast<double>(round_ms(ci.second, m))
                  << "&gt;"; // '>' 003e is an XML predefined entity, so use name.
              std::string label_limits = label_ci.str(); // For example: "<1.23, 1.45>"
              t.tspan(label_limits).fill_color(val_style.addlimits_color_).font_size(udf_font);
            }
            if (val_style.df_on_ == true // degrees of freedom is wanted.
                  && (df != (std::numeric_limits<unsigned short int>::max)()) // and deg_free is defined OK.
                )
            { // Degrees of freedom or number of values-1 used for this estimate of value.
              std::stringstream label_df;
              label_df.precision(4); // Might need 5 to show 65535?
              //label.flags(sty.value_ioflags_); // Leave at default.
              label_df << "&#x00A0;(" << df << ")"; // "123"
              // Explicit space symbol "\&#x00A0;" seems necessary.
              t.tspan(label_df.str()).fill_color(val_style.df_color_).font_size(udf_font);
            }
            if (val_style.id_on_) //
            {  // Add ID or name string.
              if (label_id.size() != 0)
              {
                label_id = " \"" + label_id + "\" ";
                t.tspan(label_id).fill_color(val_style.id_color_).font_size(udf_font);
              }
            }

            if (val_style.datetime_on_  && (dt != boost::posix_time::not_a_date_time)) // from uvalue.time_;
            {  // Add date and time stamp (if valid).
              std::ostringstream label_dt;
              label_dt << dt;
              t.tspan(label_dt.str()).fill_color(val_style.datetime_color_).font_size(udf_font);
            }
            if (val_style.order_on_) //
            {  // Add order in sequence number.
              std::ostringstream label_order;
              label_order << " #" << order;
              t.tspan(label_order.str()).fill_color(val_style.order_color_).font_size(udf_font);
            }

            if (val_style.suffix_ != "")
            { // Add a suffix like "]" or " sec]".
              t.tspan(val_style.suffix_);
            }
          } // void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, Meas uvalue)


          std::string sv(double v, const value_style& sty, bool precise = false)
          { //! Strip from double value any unnecessary e, +, & leading exponent zeros, reducing "1.200000" to "1.2" or "3.4e1"...
            // TODO rename for strip_value?
            std::stringstream label;
            // Precision of std_dev is usually less than precision of value,
            // label.precision((unc) ? ((sty.value_precision_ > 3) ?  sty.value_precision_-2 : 1) : sty.value_precision_);
            // Possible but simpler to fix at precision=2
            label.precision((precise) ? 2 : sty.value_precision_);
            label.flags(sty.value_ioflags_);
            label << v; // "1.2" or "3.4e+001"...
            return  (sty.strip_e0s_) ?
              // Default is to strip unnecessary e, +, & leading exponent zeros.
              strip_e0s(label.str())  // reduce to "1.2" or "3.4e1"...
              :
              label.str();  // Leave unstripped.
          } // std::string sv(double v, const value_style& sty)

         template <class Derived>
         void axis_plot_frame<Derived>::draw_plot_point_values(double x, double y, g_element& x_g_ptr, g_element& y_g_ptr, const value_style& x_sty, const value_style& y_sty, Meas uncx, unc<false> uncy)
          { /*! \brief Write the \b pair of data points X and Y values as a string.
               \details
               The x parameter also carries the measurement information for the pair,
               and so is a @c Meas, not just an @c unc<false> as is the Y parameter.
               If a separator starting with newline,
               then both on the same line, for example "1.23, 3.45", or "[5.6, 7.8]
               X value_style is used to provide the prefix and separator, and Y value_style to provide the suffix.
               For example,
               */
            /*!
              \verbatim x_style prefix("[ X=", and separator ",&#x00A0;Y= ", " and Y value_style = "]" will produce a value label like "[X=-1.23, Y=4.56]"\endverbatim

              \note You need to use a Unicode space for get space for all browsers.
              For a long a string you may need to make the total image size bigger,
              and to orient the value labels with care.
              draw_plot_point_values is only when both X and Y pairs are wanted.
           */
            // Avoid a warning about using &#x00A0; within Doxygen comment.
            // And there is something funny about verbatim commands, hence odd layout.
            using std::string;
            using std::stringstream;
            double vx = uncx.value();
            double vy = uncy.value();
            double ux = uncx.std_dev();
            double uy = uncy.std_dev();
            double dfx = uncx.deg_free();
            double dfy = uncy.deg_free();
            unsigned short int types = uncx.types();  //  unctypes_
            distribution_type distrib;
            if (types & UNC_UNIFORM)
            {
              distrib = uniform;
            }
            else if (types & UNC_TRIANGULAR)
            {
              distrib = triangular;
            }
            else
            { // Default.
              distrib = gaussian;
            }
            std::string label_xv = sv(vx, x_sty); //! Also strip unnecessary e, + and leading exponent zeros, if required.
            std::string label_yv = sv(vy, y_sty);
            if (x_sty.prefix_ != "")
            { // Want a prefix , for example, "["
              label_xv = x_sty.prefix_ + label_xv;
            }

            int marker_size = derived().serieses_[0].point_style_.size_;
            int label_size = x_sty.values_text_style_.font_size();
            // Offset of value labels from point must be related mainly to
            // size of the data marker, less the value label font size.

            rotate_style rot = x_sty.value_label_rotation_;
            align_style al; // = center_align;
            switch (rot)
            {
            case horizontal: // OK
              al = center_align;
              y -= marker_size * 2;  // Up marker font size;
              // center_align means no x correction.
              break;
            case leftward: // horizontal but to left of marker. OK
              al = right_align;
              x -= marker_size * 1.3;  // left
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case rightward: // horizontal but to right of marker.OK
              al = left_align;
              x += marker_size * 1.1;  // right
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case upsidedown: // OK but upsidedown so not very useful!
              al = center_align;
              y += marker_size;  // Down marker font size;
             break;
            case slopeup: // -30 - OK
            case steepup: // -45 - OK
            case uphill: // -60 - OK
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.6;  // UP marker font size;
              break;
            case upward: // -90 vertical writing up - OK.
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.9;  // Up marker font size;
              break;
            case backup: // OK
              al = right_align;
              x -= marker_size * 1.5;  // Left
              y -= marker_size * 0.8;  // Up
              rot = downhill;
              break;
            case  slopedownhill: // 30 gentle slope down.
            case downhill: // 45 down.
            case steepdown: //  60 steeply down.
              al = left_align;
              x += marker_size * 0.4;  // Right;
              y += marker_size * 0.9;  // Down
              break;
            case downward: // OK
              al = left_align;
              x -= marker_size;  // Left
              y += marker_size;  // Up
             break;
            case backdown: // OK
              al = right_align;
              x -= marker_size * 0.5;  // Left
              y += marker_size * 1.5;  // down
              rot = uphill;
             break;
            } // switch

            // If would be simpler to prepare a single string like "1.23 +- -0.3, 3.45 +- -0.1(10)"
            // but this would not allow change of font size, type and color
            // something that proves to be very effective at visually separating
            // value and std_dev, and degrees of freedom estimate,
            // and other information like date_time, id or name, and order in sequence.
            // So the coding complexity is judged with it
            // (even if it may not always work right yet ;-)
            // Tasteless colors and font changes are purely proof of concept!

            int fx = static_cast<int>(x_sty.values_text_style_.font_size() * reducer);

            // Make std_dev and df a bit smaller to distinguish from value by default (but make configurable).
            // Write X value (and optional std_dev and df).
            std::string label_xdf; // X degrees of freedom as string.

            text_element& t = x_g_ptr.text(x, y, label_xv, x_sty.values_text_style_, al, rot);
            // Optionally, show std_dev as 95% confidence plus minus:  2.1 +-0.012
            // and also optionally show degrees of freedom (23).

            string pm_symbol = "&#x00A0;&#x00B1;"; //! Unicode space text_plusminus glyph.
            // Spaces seem to get lost, so use \&x00A0 as an explicit space glyph.
            // Layout seems to vary with font - Times New Roman leaves no space after.
            if ((x_sty.plusminus_on_ == true) && (ux > 0.) )
            {  // std_dev estimate usually 67% confidence interval + or - standard deviation.
              ux *= derived().text_plusminus_; // typically  + or - standard deviation.
              std::string label_xu; // X std_dev as string.
              label_xu = sv(ux, x_sty, true);
              //t.tspan(pm_symbol).fill_color(darkcyan);
              // Should this be stroke_color?
              t.tspan(pm_symbol).fill_color(x_sty.plusminus_color_);
              t.tspan(label_xu).fill_color(x_sty.plusminus_color_).font_size(fx); // .font_weight("bold")
            }
            if (x_sty.addlimits_on_ == true)
            { // Want confidence interval appended, for example: <1.23, 1.45>
              //double alpha = 0.05; // oss.iword(confidenceIndex) / 1.e6; // Pick up alpha.
              //double epsilon = 0.01; // = oss.iword(roundingLossIndex) / 1.e3; // Pick up rounding loss.
              //int uncSigDigits = 2; // = oss.iword(setUncSigDigitsIndex);  // Pick up significant digits for uncertainty.
              //bool isNoisyDigit = false; // Pick up?
              if(derived().isNoisyDigit_)
              {
                derived().uncSigDigits_++;
              }
              std::pair<double, double> ci = conf_interval(vx, ux, dfx, derived().alpha_, distrib);
              int m = round_m(derived().epsilon_, ux, derived().uncSigDigits_, distrib);
              using boost::lexical_cast;
              std::stringstream label;
              label << " &lt;" // '<' 003C is an XML predefined entity, so use name.
                  << lexical_cast<double>(round_ms(ci.first, m)) << ", "
                  << lexical_cast<double>(round_ms(ci.second, m))
                  << "&gt;"; // '>' 003e is an XML predefined entity, so use name.
              std::string label_limits = label.str(); // For example: "<1.23, 1.45>"
              t.tspan(label_limits).fill_color(x_sty.addlimits_color_).font_size(fx);
            }

            if ((x_sty.df_on_ == true)  // Is wanted.
                 &&
                 (dfx != (std::numeric_limits<unsigned short int>::max)()) // and deg_free is defined OK.
               )
            { // Degrees of freedom (usually number of observations-1) used for this estimate of value.
              stringstream label;
              label.precision(4); // Might need 5 to show 65535?
              //label.flags(sty.value_ioflags_); // Leave at default.
              label << "&#x00A0;(" << dfx << ")"; // "123.5"
              // Explicit space "\&#x00A0;" seems necessary.
              label_xdf = label.str();
              t.tspan(label_xdf).fill_color(x_sty.df_color_).font_size(fx);
            }

            int fy = static_cast<int>(y_sty.values_text_style_.font_size() * reducer);
            // If a newline is 1st char in separator, put values on the next line below the marker,
            // else all on one line.
            bool sameline = (x_sty.separator_[0] != '\n');
            if (sameline)
            { // On same line so use X style for separator, but Y style for any text.
              t.tspan(x_sty.separator_).fill_color(x_sty.fill_color_).font_size(x_sty.values_text_style_.font_size());
              t.tspan(y_sty.separator_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              if (y_sty.prefix_ != "")
              { // Want a prefix, for example: "length ="
                t.tspan(y_sty.prefix_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              }
              t.tspan(label_yv, y_sty.values_text_style_); // Color.
              if (
                   (y_sty.plusminus_on_) // +/- is wanted.
                   && (uy > 0.) // Is valid std_dev estimate.
                 )
              { // std_dev estimate (usually 95% confidence interval + or - standard deviation).
                // Precision of std_dev is usually less than value,
                uy *= derived().text_plusminus_; // Typically + or - standard deviation.
                std::string label_yu;
                label_yu = "&#x00A0;" + sv(uy, y_sty, true);
                t.tspan(pm_symbol).font_family("arial").font_size(fy).fill_color(green);
                t.tspan(label_yu).fill_color(y_sty.plusminus_color_).font_size(fy);
              }
 //             value_style val_style = y_sty;
              if (y_sty.addlimits_on_ == true)
              { // Want confidence interval appended, for example: <1.23, 1.45>
                //double alpha = 0.05; // oss.iword(confidenceIndex) / 1.e6; // Pick up alpha.
                //double epsilon = 0.01; // = oss.iword(roundingLossIndex) / 1.e3; // Pick up rounding loss.
                //int uncSigDigits = 2; // = oss.iword(setUncSigDigitsIndex);  // Pick up significant digits for uncertainty.
                //bool isNoisyDigit = false; // Pick up?
                if(derived().isNoisyDigit_)
                {
                  derived().uncSigDigits_++;
                }
                std::pair<double, double> ci = conf_interval(vy, uy, dfy, derived().alpha_, distrib);
                int m = round_m(derived().epsilon_, uy, derived().uncSigDigits_, distrib);
                using boost::lexical_cast;
                std::stringstream label;
                label << " &lt;" // '<' 003C is an XML predefined entity, so use name.
                    << lexical_cast<double>(round_ms(ci.first, m)) << ", "
                    << lexical_cast<double>(round_ms(ci.second, m))
                    << "&gt;"; // '>' 003e is an XML predefined entity, so use name.
                std::string label_limits = label.str(); // For example: "<1.23, 1.45>"
                t.tspan(label_limits).fill_color(y_sty.addlimits_color_).font_size(fy);
              }
              if ((y_sty.df_on_ == true)
                && (dfy != (std::numeric_limits<unsigned short int>::max)()) // Is valid df.
                )
              { // degrees of freedom or number of values -1 used for this estimate.
                std::stringstream label;
                label.precision(4);
                //label.flags(sty.value_ioflags_); // Leave at default.
                label <<"&#x00A0;(" << dfy << ")"; // "123.5"
                std::string label_ydf;
                label_ydf = label.str();
                t.tspan(label_ydf).fill_color(y_sty.df_color_).font_size(fy);
              }
              if (y_sty.suffix_ != "")
              { // Want a suffix like "]" - with the Y values font size, (not reduced for std_dev info), and same color as prefix.
                t.tspan(y_sty.suffix_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              }
            } //
            else
            { // Move ready to put Y value on 'newline' below point marker.
              // Problem here if orientation is changed? - Yes - doesn't line up :-(
              //x_sty.separator_.substr(1); to ignore the \n newline indicator.
              t.tspan(x_sty.separator_.substr(1)).fill_color(x_sty.fill_color_).font_size(x_sty.values_text_style_.font_size());
              if (y_sty.prefix_ != "")
              { //
                label_yv = y_sty.prefix_ + label_yv;
              }
              double dy = y_sty.values_text_style_.font_size() * 1.2; // was 2.2 "newline"
              // Need to start a new text_element here because tspan rotation doesn't apply to whole string?
              text_element& ty = y_g_ptr.text(x, y + dy, label_yv, y_sty.values_text_style_, al, rot);

              if ((y_sty.plusminus_on_ == true) // Is wanted.
                  && (uy > 0.) // And is a valid std_dev estimate.
                  )
              {  // std_dev estimate usually 95% confidence interval + or - 2 standard deviation.
                 // Precision of std_dev is usually less than value,
                std::string label_yu = "&#x00A0;" + sv(uy, y_sty, true);
                ty.tspan(pm_symbol).font_family("arial").font_size(fy).fill_color(y_sty.plusminus_color_); // +/- sumbol.
                ty.tspan(label_yu).fill_color(y_sty.plusminus_color_).font_size(fy);
              }

              if (y_sty.addlimits_on_ == true)
              { // Want confidence interval appended, for example: <1.23, 1.45>
                // TODO pick up alpha, epsilon from somewhere?
                //double alpha = 0.05; // oss.iword(confidenceIndex) / 1.e6; // Pick up alpha.
                //double epsilon = 0.01; // = oss.iword(roundingLossIndex) / 1.e3; // Pick up rounding loss.
                //int uncSigDigits = 2; // = oss.iword(setUncSigDigitsIndex);  // Pick up significant digits for uncertainty.
                //bool isNoisyDigit = false; // Pick up?
                if(derived().isNoisyDigit_)
                {
                  derived().uncSigDigits_++;
                }
                std::pair<double, double> ci = conf_interval(vy, uy, dfy, derived().alpha_, distrib);
                int m = round_m(derived().epsilon_, uy, derived().uncSigDigits_, distrib);
                using boost::lexical_cast;
                std::stringstream label;
                label << " &lt;" // '<' 003C is an XML predefined entity, so use name.
                    << lexical_cast<double>(round_ms(ci.first, m)) << ", "
                    << lexical_cast<double>(round_ms(ci.second, m))
                    << "&gt;"; // '>' 003e is an XML predefined entity, so use name.
                std::string label_limits = label.str(); // For example: "<1.23, 1.45>"
                ty.tspan(label_limits).fill_color(y_sty.addlimits_color_).font_size(fy);
              }

              if ((y_sty.df_on_ == true)  // Is wanted.
                    && (dfy != (std::numeric_limits<unsigned short int>::max)()) // and deg_free is defined OK.
                    )
              { // degrees of freedom or number of values -1 used for this estimate.
                std::stringstream label;
                label.precision(4);
                //label.flags(sty.value_ioflags_); // Leave at default.
                label <<"&#x00A0;(" << dfy << ")"; // "123.5"
                std::string label_ydf = label.str();
                ty.tspan(label_ydf).fill_color(y_sty.df_color_).font_size(fy);
              }

              if (y_sty.suffix_ != "")
              { // Want a suffix like "]" or "sec]" or "&#x00A0;sec]"
                ty.tspan(y_sty.suffix_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              }
            }  // Same or newline.

            int udf_font = static_cast<int>(y_sty.values_text_style_.font_size() * reducer);

            std::string label_id = uncx.id_;
            if (x_sty.id_on_ && (label_id.size() != 0)) //
            {  // Add ID or name string (in " quotes).
              label_id = " \"" + label_id + "\" ";
              t.tspan(label_id).fill_color(x_sty.id_color_).font_size(udf_font);
            }

            using boost::posix_time::ptime;
            ptime dt = uncx.time_;
            if (x_sty.datetime_on_ && (dt != boost::posix_time::not_a_date_time))
            {  // Add date and time stamp (if date_time valid).
              std::ostringstream label_dt;
              label_dt << " " << dt;
              t.tspan(label_dt.str()).fill_color(x_sty.datetime_color_).font_size(udf_font);
            }

            int order = uncx.order_;
            if (x_sty.order_on_ && order >= 0) // If order < 0, then no order in sequence number.
            {  // Add order in sequence number.
              std::ostringstream label_order;
              label_order << " #" << order;
              t.tspan(label_order.str()).fill_color(x_sty.order_color_).font_size(udf_font);
            }

            if (x_sty.suffix_ != "")
            { // Add a suffix like "]" or " sec]".
              t.tspan(y_sty.suffix_);
            }
        } // void draw_plot_point_values(double x, double y, g_element& g_ptr, double value)

        // Member functions definitions.
        ////////////////////////////////

          template <class Derived>
          Derived& axis_plot_frame<Derived>::size(int x, int y)
          { //! Set SVG image size (SVG units, default pixels).
            // Check on sanity of these values?
            derived().image_.size(x, y);
            return derived();
          }

          template <class Derived>/*! \tparam Derived plot class in svg_1d_plot or svg_2d_plot or svg_boxplot. */
          template <class T> /*! \tparam T an STL container: array, vector, list, map ...  */
          Derived& axis_plot_frame<Derived>::x_autoscale(const T& begin, const T& end)
          { //! Data series (range accessed using iterators) to use to calculate autoscaled X-axis values.
              scale_axis(begin, end,
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_, derived().autoscale_plusminus_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

            derived().x_autoscale_ = true; //! By default, use these calculated values.
            return derived();
          } // x_autoscale(const T& begin, const T& end)

          template <class Derived>
          template <class T> // T an STL container: @c array, @c vector...
          Derived& axis_plot_frame<Derived>::x_autoscale(const T& container) // Use whole 1D data series.
          { //! Data series (all values) to use to calculate autoscaled X-axis values.
            //scale_axis(container.begin(), container.end(), // All the container.
            scale_axis(container, // All the container.
            &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
            derived().autoscale_check_limits_, derived().autoscale_plusminus_,
            derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

            derived().x_autoscale_ = true; //! By default, use these calculated values.
            return derived();
          } // x_autoscale(const T& container)

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::size()
          { //! \return SVG image size, both horizontal width and vertical height (SVG units, default pixels).
            return derived().image_.xy_sizes();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::image_x_size() //!< Obselete - deprecated.
          { //! \return SVG image X-axis size as horizontal width (SVG units, default pixels).
            return derived().image_.x_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_size(int i)
          { //! Set SVG image X-axis size (SVG units, default pixels).
            derived().image_.x_size(i);
            return derived();
          }
          template <class Derived>
          int axis_plot_frame<Derived>::x_size()
          { //! Get SVG image X-axis size as horizontal width (SVG units, default pixels).
            //! \return SVG image X-axis size as horizontal width (SVG units, default pixels).
            return derived().image_.x_size();
          }
          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_x_size(int i) //!< Obselete - deprecated.
          { //! Set SVG image X-axis size (SVG units, default pixels).
            derived().image_.x_size(i);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::image_y_size() //!< Obselete - deprecated.
          { //! \return SVG image Y-axis size as vertical height (SVG units, default pixels).
            return derived().image_.y_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_y_size(int i) //!< Obselete - deprecated.
          {//! Set SVG image Y-axis size (SVG units, default pixels).
            derived().image_.y_size(i);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::y_size()
          { //! \return SVG image Y-axis size as vertical height (SVG units, default pixels).
            return derived().image_.y_size();
          }

                   template <class Derived>
          Derived& axis_plot_frame<Derived>::y_size(int i)
          {//! Set SVG image Y-axis size (SVG units, default pixels).
            derived().image_.y_size(i);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::background_color()
          { //! \return  plot background color.
            return derived().image_.g(PLOT_BACKGROUND).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::background_color(const svg_color& col)
          { //! Set plot background color.
            derived().image_.g(PLOT_BACKGROUND).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::background_border_color(const svg_color& col)
          { //! Set plot background border color.
            derived().image_.g(PLOT_BACKGROUND).style().stroke_color(col);
            /*!
              background_border_color, for example:
              \verbatim svg_2d_plot my_plot(my_data, "My Data").background_border_color(red).background_color(azure);
              \endverbatim
            */
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::background_border_color()
          { //! \return  plot background border color.
            return derived().image_.g(PLOT_BACKGROUND).style().stroke_color();
          }


          template <class Derived>
          Derived& axis_plot_frame<Derived>::background_border_width(double w)
          { //! Set plot background border width.
            derived().image_.g(PLOT_BACKGROUND).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::background_border_width()
          { //! \return  plot background border width.
            return derived().image_.g(PLOT_BACKGROUND).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::description(const std::string d)
          { /*! Writes description to the SVG document for header, for example:
              \verbatim
                <desc> My Data </desc>
              \endverbatim
          */
            derived().image_.description(d);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::description()
          { //! \return Description of the document for title as \verbatim <desc> ... </desc> \endverbatim
            return derived().image_.description();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::document_title(const std::string d)
          { //! Write document title to the SVG document for title as \verbatim <title> My Title </title>  \endverbatim
            derived().image_.document_title(d);
            return derived();
          }
          template <class Derived>
          std::string axis_plot_frame<Derived>::document_title()
          { //! \return Get document title to the document for title as \verbatim <title> My Title </title> \endverbatim
            return derived().image_.document_title();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::copyright_holder(const std::string d)
          { //! Writes copyright_holder to the document
            //! (as \verbatim <!-- SVG Plot Copyright Paul A. Bristow 2007 --> ) \endverbatim
            //! and as \verbatim metadata: <meta name="copyright" content="Paul A. Bristow" /meta> \endverbatim
            derived().image_.copyright_holder(d);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::copyright_holder()
          { //! \return copyright holder.
            return derived().image_.copyright_holder();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::copyright_date(const std::string d)
          { //! Writes copyright date to the document.
            //! and as \verbatim metadata <meta name="date" content="2007" /> \endverbatim
            derived().image_.copyright_date(d);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::copyright_date()
          { //! \return  copyright_date.
            return derived().image_.copyright_date();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::license(std::string repro,
            std::string distrib,
            std::string attrib,
            std::string commercial,
            std::string derivative)
          { //! Set license conditions for reproduction, attribution, commercial use, and derivative works,
            //! usually "permits", "requires", or "prohibits",
            //! and set license_on == true.
            // Might check these are "permits", "requires", or "prohibits"?
            derived().image_.license(repro, distrib, attrib, commercial, derivative);
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::license_on()
          { //! \return true if license conditions should be included in the SVG document.
            //! \see axis_plot_frame::license
            return derived().image_.license_on();
          }

          template <class Derived>
          Derived&  axis_plot_frame<Derived>::license_on(bool l)
          { //! Set if license conditions should be included in the SVG document.
            //! \see axis_plot_frame::license
            derived().image_.license_on(l);
            return derived();
          }
          template <class Derived>
          bool axis_plot_frame<Derived>::boost_license_on()
          { //! \return  if the Boost license conditions should be included in the SVG document.
            //! To set see axis_plot_frame::boost_license_on(bool).
            return derived().image_.boost_license_one();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::boost_license_on(bool l)
          { //! Set if the Boost license conditions should be included in the SVG document.
            derived().image_.boost_license_on(l);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_reproduction()
          { //! \return  reproduction license conditions, usually "permits", "requires", or "prohibits".
            return derived().image_.reproduction();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_distribution()
          { //! \return  distribution license conditions, usually "permits", "requires", or "prohibits".
            return derived().image_.distribution();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_attribution()
          { //! \return  attribution license conditions, usually "permits", "requires", or "prohibits".
            return derived().image_.attribution();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_commercialuse()
          { //! \return  commercial use license conditions, usually "permits", "requires", or "prohibits".
            return derived().image_.commercialuse();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::coord_precision(int digits)
          { /*! Precision of SVG coordinates in decimal digits (default 3).
              3 decimal digits precision is sufficient for small images.
              4 or 5 decimal digits precision will give higher quality plots,
              especially for larger images, at the expense of larger .svg files,
              particularly if there are very many data points.
           */
            derived().image_.coord_precision(digits);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::coord_precision()
          { //! \return  precision of SVG coordinates in decimal digits.
            return derived().image_.coord_precision();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_precision(int digits)
          { /*! Precision of X tick label values in decimal digits (default 3).
              3 decimal digits precision is sufficient for small images.
              4 or 5 decimal digits precision will give more cluttered plots.
              If the range of labels is very small, then more digits will be essential.
            */

            derived().x_ticks_.value_precision_ = digits;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_value_precision()
          { //! \return  precision of X tick label values in decimal digits
            return derived().x_ticks_.value_precision_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_ioflags(std::ios_base::fmtflags flags)
          { /*! Set iostream std::ios::fmtflags for X value label (default decimal == 0X201).
              Mainly useful for changing to scientific, fixed or hexadecimal format.
              For example: .x_value_ioflags(std::ios::dec | std::ios::scientific)
            */
            derived().x_ticks_.value_ioflags_ = flags;
            return derived();
          }

          template <class Derived>
          std::ios_base::fmtflags axis_plot_frame<Derived>::x_value_ioflags()
          { //! \return  stream ioflags for control of format of X value labels.
            return derived().x_ticks_.value_ioflags_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_labels_strip_e0s(bool cmd)
          { //! Set @c true if want to strip redundant zeros, signs and exponents.
            //! @b Example: reducing "1.2e+000" to "1.2".
            //! This markedly reduces visual clutter, and is the default.
            derived().x_ticks_.strip_e0s_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::y_labels_strip_e0s()
          { //! \return @c true if set to strip redundant zeros, signs and exponents.
            //! @b Example: reducing "1.2e+000" to "1.2".
            return derived().x_ticks_.strip_e0s_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title(const std::string title)
          { /*!
              Set a title for plot.
              The string may include Unicode for greek letter and symbols.
              @b Example:
              A title that includes a greek omega and degree symbols:
              \code
                my_plot.title("Plot of &#x3A9; function (&#x00B0;C)");
              \endcode

              Unicode symbols are at http://unicode.org/charts/symbols.html.

              \note Only set plot title once.
            */
            // Plot title.  TODO
            // new text parent code push_back
            // effectively concatenates with any existing title.
            // So clear the existing string first but doesn't work,
            // so need to clear the whole g_element.
            //derived().image_.g(PLOT_TITLE).clear();
            derived().title_info_.text(title);
            derived().title_on_ = true; // Assume display wanted, if bother to set title.
            return derived(); // Make chainable.
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::title()
          { //! \return  Title for plot,
            // (whose character string may include Unicode value like &#x3A9; for greek letter and symbols).
            //! Example: std::cout << "title \"" << my_plot.title() << "\""<< std::endl;
            //! outputs:
            //! \verbatim title "Plot showing several data point marker shapes &#x26; sizes." \endverbatim
            return derived().title_info_.text();
          }

          template <class Derived>
          text_style& axis_plot_frame<Derived>::title_style()
          { //! \return  Font size for the title (svg units, default pixels).
            //! Example: std::cout << "title style " << my_2d_plot.title_style() << std::endl;
            //! Outputs: title style text_style(10, "Lucida Sans Unicode", "", "normal", "", "", 900)
            return derived().title_info_.textstyle(); // Gets expected title style.
 //           return derived().title_style_;  // Gets default title font size :-(
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_size(int i)
          { //! Sets the font size for the title (svg units, default pixels).
            derived().title_info_.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::title_font_size()
          { //! \return  the font size for the title (svg units, default pixels).
            return derived().title_info_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_family(const std::string& family)
          { //! Set the font family for the title (for example: .title_font_family("Lucida Sans Unicode");
            derived().title_info_.textstyle().font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_family()
          { //! \return  the font family for the title
            return derived().title_info_.textstyle().font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_style(const std::string& style)
          { //! Set the font style for the title (default normal).
            derived().title_info_.textstyle().font_style(style);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_style()
          { //! \return  the font style for the title (default normal).
            return derived().title_info_.textstyle().font_style();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_weight(const std::string& weight)
          { //! Set the font weight for the title (default normal).
            derived().title_info_.textstyle().font_weight(weight);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_weight()
          {//! \return  the font weight for the title.
            return derived().title_info_.textstyle().font_weight();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_stretch(const std::string& stretch)
          { //! Set the font stretch for the title (default normal), wider or narrow.
            derived().title_info_.textstyle().font_stretch(stretch);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_stretch()
          { //! \return the font stretch for the title (default normal), wider or narrow.
            return derived().title_info_.textstyle().font_stretch();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_decoration(const std::string& decoration)
          { //! Set the font decoration for the title (default normal, or underline, overline or strike-thru).
            derived().title_info_.textstyle().font_decoration(decoration);
            return derived();
          }

           template <class Derived>
         const std::string& axis_plot_frame<Derived>::title_font_decoration()
          { //! \return  the font decoration for the title (default normal, or underline, overline or strike-thru).
            return derived().title_info_.textstyle().font_decoration();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_rotation(rotate_style rotate)
          { //! Set the rotation for the title font (degrees, 0 to 360).
            derived().title_info_.rotation(rotate);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::title_font_rotation()
          { //! \return  the rotation for the title font (degrees).
            return derived().title_info_.rotation();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_alignment(align_style alignment)
          { //! Set the alignment for the title.
            //! \sa align_style for options.
            derived().title_info_.alignment(alignment);
            return derived();
          }

          template <class Derived>
          align_style axis_plot_frame<Derived>::title_font_alignment()
          { //! \return  the alignment for the title.
            //! \sa align_style for values.
            return derived().title_info_.alignment();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_text_length(double length)
          { //! If length > 0
            // forces the the estimated text length for the title (svg units, default pixels).
            //! This expands or compresses the text to fill this width exactly,
            //! potentially making it so elongated with spaces between letters,
            //! or compresses so that the glyphs collide so it becomes unreadable.
            //! Normally only used internally.
            //! Example: @c .title_text_length(300) // Force text into an arbitrary chosen fixed width - about equal to 80 chars.
            derived().title_info_.textstyle().text_length(length);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::title_text_length()
          { //! \return the estimated text length for the title (svg units, default pixels).
            //! Default 0
            return derived().title_info_.textstyle().text_length();
          }

          // Legend.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_width(double width)
          { //! Set the width for the legend.
            derived().legend_width_ = width;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::legend_width()
          { //! \return  the width for the legend.
            return derived().legend_width_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_title(const std::string title)
          { //! Set the title for the legend.
            derived().legend_title_.text(title);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::legend_title()
          { //! \return  the title for the legend.
            return derived().legend_title_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_font_weight(const std::string& weight)
          { //! Set the font weight for the legend title.
            derived().legend_title_style_.weight_ = weight;
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_font_weight()
          { //! \return the font weight for the legend title.
            return derived().legend_title_style_.weight_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_font_family(const std::string& family)
          { //! Set the font family for the legend title.
            derived().legend_title_style_.font_family_ = family;
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_font_family()
          { //! \return  the font family for the legend title.
            return derived().legend_title_style_.font_family_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_title_font_size(int size)
          { //! Set the font size for the legend title (svg units, default pixels).
            derived().legend_title_style_.font_size_ = size;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::legend_title_font_size()
          { //! \return  the font size for the legend title (svg units, default pixels).
            return derived().legend_title_style_.font_size_;
          }
   
           template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_title_font_weight(const std::string& weight)
          { //! Set the font weight for the legend title.  Example: @c my_plot.legend_title_font_size(10);
            derived().legend_title_style_.font_weight_ = weight;
            return derived(); // Permit chaining.
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_title_font_weight()
          { //! \return  the font weight for the legend title.
            return derived().legend_title_style_.weight_;
          }

          // Separate title and text font sizes.
          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_text_font_size(int size)
          { //! Set the font size for the legend title (svg units, default pixels).
            derived().legend_text_style_.font_size_ = size;
            return derived(); // Make chainable.
          }


         //template <class Derived>
         // Derived& axis_plot_frame<Derived>::legend_text_font_size(int size)
         // { //! Set Font size for legend text (svg units, default pixels).
         //   derived().legend_text_style_.font_size_ = size;
         //   return derived();
         // }

          template <class Derived>
          int axis_plot_frame<Derived>::legend_text_font_size()
          { //! \return  Font size for the legend text used for marker descriptions (svg units, default pixels).
            return derived().legend_text_style_.font_size_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_text_font_weight(const std::string& weight)
          { //! Set the font weight for the legend text. Example: @c my_plot.legend_text_font_size(20);
            derived().legend_text_style_.font_weight_ = weight;
            return derived(); // Permit chaining.
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_text_font_weight()
          { //! \return  the font weight for the legend text.
            return derived().legend_text_style_.weight_;
          }


          //template <class Derived>
          //Derived& axis_plot_frame<Derived>::legend_text_color(const svg_color& color)
          //{ //! Set the stroke font color for the legend text.
          ////! Example: @c my_plot.legend_text_color(red);
          //  derived().legend_text_style_.stroke_color_ = color;
          //  return derived(); // Permit chaining.
          //}

          //template <class Derived>
          //svg_color axis_plot_frame<Derived>::legend_text_color()
          //{ //! \return  Font stroke color for the legend text.
          //  return derived().legend_text_style_.stroke_color_;
          //}


          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_top_left(double x, double y)
          { //! Set position of top left of legend box (svg coordinates, default pixels).
            //! Bottom right is controlled by contents, so the user cannot set it.
            if((x < 0) || (x > derived().image_.x_size()) || (y < 0) || (y > derived().image_.y_size()))
            {
              throw std::runtime_error("Legend box position outside image!");
            }
            derived().legend_left_ = x;
            derived().legend_top_ = y;
            return derived();
          }

          template <class Derived>
          const std::pair<double, double> axis_plot_frame<Derived>::legend_top_left()
          { //! \return  svg coordinate (default pixels) of top left of legend box.
            std::pair<double, double> r;
            r.first = derived().legend_left_;
            r.second = derived().legend_top_;
            return r;
          }

          template <class Derived>
          const std::pair<double, double> axis_plot_frame<Derived>::legend_bottom_right()
          { //! \return  svg coordinate (default pixels) of Bottom right of legend box.
            std::pair<double, double> r;
            r.first = derived().legend_right_;
            r.second = derived().legend_bottom_;
            return r;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_lines(bool is)
          { //! Set true if legend should include samples of the lines joining data points.
            //! \details This allows different series of data points to be distinguished by different color and/or width.
            //! This is especially useful to show plots of different functions and/or different parameters in different colors.
            derived().legend_lines_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_lines()
          {//! \return  true if legend should include samples of the lines joining data points.
            return derived().legend_lines_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_on(bool cmd)
          { //! Set true if a legend is wanted.
            derived().legend_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_on()
          { //! \return  true if a legend is wanted.
            return derived().legend_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_vertical(double fraction)
          { //! Set vertical position of X-axis for 1D as fraction of plot window.
            if((fraction < 0.0) || (fraction > 1.0))
            {
              std::cout << "Vertical position of X-axis must be a fraction of plot window 0 < value < 1!"
              << std::endl;
            }
            else
            {
              derived().x_axis_vertical_ = fraction;
            }
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_axis_vertical()
          { //! \return vertical position of X-axis for 1D as fraction of plot window.
            return derived().x_axis_vertical_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_place(legend_places l)
          { //! Set the position of the legend, \see  boost::svg::legend_places
            derived().legend_place_ = l;
            return derived();
          }

          template <class Derived>
          legend_places axis_plot_frame<Derived>::legend_place()
          { //! \return  the position of the legend, \see boost::svg::legend_places
            return derived().legend_place_;
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_outside()
          { //! \return  if the legend should be outside the plot area.
            return derived().outside_legend_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_window_on(bool cmd)
          { //! Set true if a plot window is wanted (or false if the whole image is to be used).
            derived().plot_window_on_ = cmd;
            if(cmd)
            { // Set plot window.
              derived().image_.g(detail::PLOT_WINDOW_BACKGROUND).style()
                .fill_color(derived().plot_window_border_.fill_) // background color and
                .stroke_color(derived().plot_window_border_.stroke_); // border color.
            }
            //derived().legend_place_ = outside_right;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::plot_window_on()
          {//! \return  true if a plot window is wanted (or false if the whole image is to be used).
            return derived().plot_window_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_border_color(const svg_color& col)
          { //! Set the color for the plot window background. Example: @c .plot_border_color(lightgoldenrodyellow)
            derived().plot_window_border_.stroke_ = col;
            derived().image_.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::plot_border_color()
          { //! \return  the color for the plot window background.
            return derived().image_.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::plot_border_width()
          { //! \return  the width for the plot window border (svg units, default pixels).
            return derived().image_.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_border_width(double w)
          { //! Set the width for the plot window border (svg units, default pixels).
            derived().plot_window_border_.border_width_ = w;
            derived().image_.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_border_margin(double w)
          { //! Set the margin around the plot window border (svg units, default pixels).
            //! \details This prevents the plot window getting too close to other elements of the plot.
            derived().image_border_.margin_ = w;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::image_border_margin()
          { //! \return  the margin around the plot window border (svg units, default pixels).
            return derived().image_border_.margin_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_border_width(double w)
          { //! Set the svg image border width (svg units, default pixels).
            derived().image_border_.border_width_ = w;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::image_border_width()
          { //! \return  the svg image border width (svg units, default pixels).
            return derived().image_border_.border_width_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_window_x(double min_x, double max_x)
          { //! Set the minimum and maximum (cartesian data units) for the plot window X axis.
            //! This is normally calculated from other plot values.
            if(max_x <= min_x)
            {
              throw std::runtime_error("plot_window X: x_max_ <= x_min_");
            }
            if((max_x - min_x) < std::numeric_limits<double>::epsilon() * 1000)
            { // Range too small to display.
              throw std::runtime_error("plot_window X range too small!" );
            }
            derived().plot_left_ = min_x;
            derived().plot_right_ = max_x;
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_window_y(double min_y, double max_y)
          { //! Set the minimum and maximum (cartesian data units) for the plot window Y axis.
            //! This is normally calculated from other plot values.

            if(max_y <= min_y)
            {
              throw std::runtime_error("plot_window Y: y_max_ <= x_min_");
            }
            if(max_y <= min_y)
            {
              throw std::runtime_error("plot_window Y range too small!");
            }
            derived().plot_top_ = min_y;
            derived().plot_bottom_ = max_y;
            return derived();
          }

          // Get the minimum and maximum (cartesian data units) for the plot window axes.

          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_x_left()
          { //! \return  left of the plot window.
            return derived().plot_left_;
          }
          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_x_right()
          { //! \return  right of the plot window.
            return derived().plot_right_;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_y_top()
          { //! \return  top of the plot window.
            return derived().plot_top_;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_y_bottom()
          { //! \return Top of the plot window.
            return derived().plot_bottom_;
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::plot_window_x()
          { //! \return  both the left and right (X axis) of the plot window.
            std::pair<double, double> r;
            r.first = derived().plot_left_;
            r.second = derived().plot_right_;
            return r;
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::plot_window_y()
          { //! \return  both the top and bottom (Y axis) of the plot window.
            std::pair<double, double> r;
            r.first = derived().plot_top_;
            r.second = derived().plot_bottom_;
            return r;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_interval()
          { //! \return  interval between X minor ticks.
            return derived().x_ticks_.minor_interval_;  // interval
          }

          template <class Derived>
          double axis_plot_frame<Derived>::y_minor_interval()
          { //! \return  interval between Y minor ticks.
            return derived().y_ticks_.minor_interval_;  // interval
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_up_on(bool cmd)
          { //! Set true if X major ticks should mark upwards.
            derived().x_ticks_.up_ticks_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_ticks_up_on()
          { //! \return  true if X major ticks should mark upwards.
            return derived().x_ticks_.up_ticks_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_down_on(bool cmd)
          { //! Set true if X major ticks should mark downwards.
            derived().x_ticks_.down_ticks_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_ticks_down_on()
          { //! \return  true if X major ticks should mark downwards.
            return derived().x_ticks_.down_ticks_on_;
          }
          // Only need y_ticks_left_on & y_ticks_right_on in 2D, so not defined here.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_on(bool cmd)
          { //! Set true if want to show X-axis label text.
            //! \details Also switched on by setting label text.
            //! (on the assumption that if label text is set, display is also wanted,
            //! but can be switched off if \b not required).
            derived().x_axis_.label_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_label_on()
          { //! \return  true if want to show X-axis label text.
            return derived().x_axis_.label_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_font_size(int i)
          { //! Set X axis label font size (svg units, default pixels).
            derived().x_label_info_.textstyle().font_size(i);
            // Also duplicated at
            // derived().x_axis_label_style_.font_size(i);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_label_font_size()
          { //! \return  X axis label font size (svg units, default pixels).
            return derived().x_label_info_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_font_size(int i)
          { //! Set X tick value label font size (svg units, default pixels).
            derived().x_value_value.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_value_font_size()
          { //! \return  X tick value label font size (svg units, default pixels).
            return derived().x_value_value_style_font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_font_family(const std::string& family)
          { //! Set X tick value label font family.
            derived().x_label_info_.textstyle().font_family_ = family;
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::x_label_font_family()
          { //! \return  X tick value label font family.
            return derived().x_label_info_.textstyle().font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_label_color(const svg_color& col)
          { //! Set X axis label color.
            derived().image_.g(detail::PLOT_X_LABEL).style().fill_color(col);
            //derived().image_.g(detail::PLOT_X_LABEL).style().stroke_color(col);
            // Setting the stroke color produces fuzzy characters :-(
            // Set BOTH stroke and fill to the same color?
           return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_axis_label_color()
          { //! \return  X axis label color.
            // But only return the fill color.
            return derived().image_.g(detail::PLOT_X_LABEL).style().fill_color();
          }

          // X-axis tick label style.
          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_values_color(const svg_color& col)
          { //! Set X axis tick value label color.
            // Set BOTH stroke and fill to the same color.
            derived().image_.g(detail::PLOT_X_TICKS_VALUES).style().fill_color(col);
            //derived().image_.g(detail::PLOT_X_TICK_VALUE_LABELS).style().stroke_color(col);
            // Setting the stroke color produces fuzzy characters :-(
            //derived().x_ticks_.color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_ticks_values_color()
          { //! \return  X-axis ticks value label color.
            // But only return the fill color.
            return derived().image_.g(detail::PLOT_X_TICKS_VALUES).style().fill_color();
            //return x_ticks_.color_ ;
          }

          // ticks_labels_style  x_ticks
          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_values_precision(int p)
          { //! Set iostream decimal digits precision of data point X values near data points markers.
            derived().x_ticks_.value_precision_ = p;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_ticks_values_precision()
          { //! \return  iostream decimal digits precision of data point X values near data points markers.
            return derived().x_ticks_.value_precision_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_values_ioflags(std::ios_base::fmtflags f)
          { //! Set iostream format flags of data point X values near data points markers.
            //! Useful to set hexadecimal, fixed and scientific, (std::ios::scientific).
            derived().x_ticks_.value_ioflags_ = f;
            return derived();
          }

          template <class Derived>
          std::ios_base::fmtflags axis_plot_frame<Derived>::x_ticks_values_ioflags()
          { //! \return  iostream format flags of data point X values near data points markers.
            //! Might be used to set hexadecimal, fixed and scientific, (std::ios::scientific).
            return derived().x_ticks_.value_ioflags_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_values_font_size(int size)
          { //! Set X ticks value label font size (svg units, default pixels).
            //derived().x_ticks_.value_label_style_.font_size(i);
            derived().x_value_label_info_.textstyle().font_size_ = size;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_ticks_values_font_size()
          { //! \return  X ticks value label font size (svg units, default pixels).
            // return derived().x_ticks_.value_label_style_.font_size();
            //       return derived().x_value_label_info_style_.font_size();

            return derived().x_value_label_info_.textstyle().font_size_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_values_font_family(const std::string& family)
          { //! Set X ticks value label font family.
            //derived().x_ticks_.value_label_style_.font_family(family); // is effect same as:
            derived().x_value_label_info_.textstyle().font_family_ = family;
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::x_ticks_values_font_family()
          { //! \return  X ticks value label font family.
            return derived().x_ticks_.value_label_style_.font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_on_window_or_axis(int cmd)
          { /*!  Set  X ticks on window or axis
              \arg cmd -1 bottom of plot window,
              \arg cmd 0 on X axis.
              \arg cmd +1 top of plot window.
            */
            derived().x_ticks_.ticks_on_window_or_on_axis_ = cmd;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_ticks_on_window_or_axis()
          { //! \return if X axis ticks wanted on the window (rather than on axis).\n
            //! -1 bottom of plot window, 0 on X axis, +1 top of plot window.
            return derived().x_ticks_.ticks_on_window_or_on_axis_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_units_on(bool cmd)
          { //! Set true if want X axis label to include units (as well as label like "length").
            //! \see x_label_units which also sets true.
            derived().x_axis_.label_units_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_label_units_on()
          { //! Set true if want X axis label to include units (as well as label like "length").
            return derived().x_axis_.label_units_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_labels_side(int place)
          { /*! Position of labels for major ticks on horizontal X-axis line.
               \arg @c place > 0 labels to left of Y-axis line )@c left_side) (default),\n
               \arg @c place = 0 (false) no major tick labels on Y-axis. \n
               \arg @c place > 0 labels to right of Y-axis line (@c right_side).
               \sa enum boost::svg:place for named
            */
            derived().x_ticks_.major_value_labels_side_ = place;
            return derived(); //! \return reference to svg_2d_plot to make chainable.
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_major_labels_side()
          { //! \return The side for X ticks major value labels (see `enum boost::svg::side`).
            return derived().x_ticks_.major_value_labels_side_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_label_rotation(rotate_style rot)
          { /*! Set rotation for X ticks major value labels. (Default horizontal).
               \see rotate_style
            */
            derived().x_ticks_.label_rotation_ = rot;
            return derived();
          }

          template <class Derived>
          rotate_style axis_plot_frame<Derived>::x_major_label_rotation()
          { /*! \return rotation for X ticks major value labels.
                \see rotate_style
             */
            return derived().x_ticks_.label_rotation_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_on(bool cmd)
          { //! If set true, show a title for the plot.
            derived().title_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::title_on()
          {//! If true, will show a title for the plot.
            return derived().title_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_grid_on(bool is)
          { //! If set true, will include a major X-axis grid.
            derived().x_ticks_.major_grid_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_major_grid_on()
          { //! If true, will include a major X-axis grid.
            return derived().x_ticks_.major_grid_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_grid_on(bool is)
          { //! If set true, will include a minor X-axis grid.
            derived().x_ticks_.minor_grid_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_minor_grid_on()
          {//! If true, will include a minor X-axis grid.
            return derived().x_ticks_.minor_grid_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::axes_on(bool is)
          { //! If set true, draw \b both x and y axes (note plural axes).
            derived().x_axis_.axis_line_on_ = is;
            derived().y_axis_.axis_line_on_ = is; // No meaning for 1D, but set anyway?
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::axes_on()
          { //! \return true if \b both x and y axis on.
            return derived().x_axis_.axis_line_on_ && derived().y_axis_.axis_line_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_on(bool is)
          { //! If set true, draw a horizontal X-axis line.
            derived().x_axis_.axis_line_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_axis_on()
          { //! If true, draw a horizontal X-axis line.
          // Use X in preference to Y for 1D.
            return derived().x_axis_.axis_line_on_;
          }

          /////////////////////////////////////////////////////////////////////////////////////
          // Y axis settings.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_axis_on(bool is)
          { //! If set true, draw a vertical Y-axis line.
            derived().y_axis_.axis_line_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::y_axis_on()
          { //! If true, draw a vertical Y-axis line.
            // Should be always false for 1D.
            return derived().y_axis_.axis_line_on_;
          }

          // enums like PLOT_TITLE provide a std:string like "title"
          // colors .stroke_color, .stroke_width and font are set in the appropriate g_element.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_color(const svg_color& col)
          { //! Set the color of any title of the plot.
            // Function title_color could set both fill (middle) and stroke (outside),
            // but just setting fill if simplest,
            // but does not allow separate inside & outside colors.
            derived().image_.g(PLOT_TITLE).style().fill_color(col);
            //derived().image_.g(PLOT_TITLE).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::title_color()
          { //! \return the color of any title of the plot.
            // Function title_color could get either fill and stroke,
            // return derived().image_.g(PLOT_TITLE).style().stroke_color();
            return derived().image_.g(PLOT_TITLE).style().fill_color();
          }

          //Derived& title_font_width(double width)
          //{ // Set the width of the font for the title of the plot.
          // width of text is effectively the boldness.
          //  // Not useful with current browsers, setting this may cause lower quality graphic fonts
          //  // perhaps because the font is created using graphics rather than a built-in font.
          //  derived().image_.g(PLOT_TITLE).style().stroke_width(width);
          //  return derived();
          //}

         //Derived& legend_font_width(double width)
          //{ //! \return  the width of the font for the title of the plot.
            // width of text is effectively the boldness.
          //  derived().image_.g(PLOT_LEGEND_TEXT).style().stroke_width(width);
          //  return derived();
          //}

          //double legend_font_width()
          //{ // Set the width of the font for the title of the legend.
          // Probably not useful at present (se above).
          //  return derived().image_.g(PLOT_LEGEND_TEXT).style().stroke_width();
          //}
          //double legend_font_width()
          //{ //! \return  the width of the font for the title of the legend.
          //  return derived().image_.g(PLOT_TITLE).style().stroke_width();
          //}

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_color(const svg_color& col)
          { //! Set the color of the title of the legend.
            // derived().image_.g(PLOT_LEGEND_TEXT).style().stroke_color(col);
            derived().image_.g(PLOT_LEGEND_TEXT).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::legend_color()
          { //! \return  the color of the title of the legend.
            // Function legend_color sets only stroke, assuming that 'filled' text is not being used.
            // (It produces much lower quality fonts on some browsers).
            return derived().image_.g(PLOT_LEGEND_TEXT).style().fill_color();
            // return derived().image_.g(PLOT_LEGEND_TEXT).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_background_color(const svg_color& col)
          { //! Set the background fill color of the legend box.//
            derived().legend_box_.fill(col);
            derived().image_.g(PLOT_LEGEND_BACKGROUND).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::legend_background_color()
          { //! \return  the background fill color of the legend box.
            return derived().image_.g(PLOT_LEGEND_BACKGROUND).style().fill_color();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_box_fill_on()
          { //! \return true if legend box has a background fill color.
            return derived().legend_box_.fill_on();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_border_color(const svg_color& col)
          { //! Set the border stroke color of the legend box.
            derived().legend_box_.stroke(col);
            derived().image_.g(PLOT_LEGEND_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::legend_border_color()
          { //! \return  the border stroke color of the legend box.
            return  derived().legend_box_.stroke();
            // return derived().image_.g(PLOT_LEGEND_BACKGROUND).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_background_color(const svg_color& col)
          { //! Set the fill color of the plot window background.
            derived().image_.g(PLOT_WINDOW_BACKGROUND).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::plot_background_color()
          { //! \return  the fill color of the plot window background.
            return derived().image_.g(PLOT_WINDOW_BACKGROUND).style().fill_color();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_axis_position()
          { //! \return  the position (or intersection with Y-axis) of the X-axis.
            switch(derived().x_axis_position_)
            {
            case top:
              return "x_axis_position top (all Y values < 0)"; break;
            case x_intersects_y:
              return "x_axis_position intersects Y axis (Y range includes zero)"; break;
            case bottom:
              return "x_axis_position bottom (all Y values > 0)"; break;
            default:
              return "?"; break;
            }
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_color(const svg_color& col)
          { //! Set the color of the X-axis line.
            // Note only stroke color is set.
            derived().image_.g(PLOT_X_AXIS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_axis_color()
          { //! \return  the color of the X-axis line.
            return derived().image_.g(PLOT_X_AXIS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_axis_color(const svg_color& col)
          { //! Set the color of the Y-axis line.
            derived().image_.g(PLOT_Y_AXIS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::y_axis_color()
          { //! \return  The color of the Y-axis line.
            return derived().image_.g(PLOT_Y_AXIS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_color(const svg_color& col)
          { //! Set the color of X-axis label (including any units).
            derived().image_.g(PLOT_X_LABEL).style().fill_color(col);
            derived().image_.g(PLOT_X_LABEL).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_label_color()
          { //! \return  the color of X-axis label (including any units).
            return derived().image_.g(PLOT_X_LABEL).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_width(double width)
          { //! Set the width (boldness) of X-axis label (including any units).
            //! (not recommended until browsers implement better).
            // width of text is effectively the boldness.
            derived().image_.g(PLOT_X_LABEL).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_label_width()
          { //! \return  The width (boldness) of X-axis label (including any units).
            return derived().image_.g(PLOT_X_LABEL).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_label_color(const svg_color& col)
          { //! Set the color of Y-axis label (including any units).
            derived().image_.g(PLOT_Y_LABEL).style().fill_color(col);
            derived().image_.g(PLOT_Y_LABEL).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::y_label_color()
          { //! \return  the color of Y-axis label (including any units).
            return derived().image_.g(PLOT_Y_LABEL).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick_color(const svg_color& col)
          { //! Set the color of X-axis major ticks.
            derived().image_.g(PLOT_X_MAJOR_TICKS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_major_tick_color()
          { //! \return  the color of X-axis major ticks.
            return derived().image_.g(PLOT_X_MAJOR_TICKS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_tick_color(const svg_color& col)
          { //! Set the color of X-axis minor ticks.
            derived().image_.g(PLOT_X_MINOR_TICKS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_minor_tick_color()
          { //! \return  the color of X-axis minor ticks.
            return derived().image_.g(PLOT_X_MINOR_TICKS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_grid_color(const svg_color& col)
          { //! Set the color of X-axis major grid lines.
            derived().image_.g(PLOT_X_MAJOR_GRID).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_major_grid_color()
          { //! \return  the color of X-axis major grid lines.
            return derived().image_.g(PLOT_X_MAJOR_GRID).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_grid_width(double w)
          { //! Set the width of X-axis major grid lines.
            derived().image_.g(PLOT_X_MAJOR_GRID).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_grid_width()
          { //! \return  the color of X-axis major grid lines.
            return derived().image_.g(PLOT_X_MAJOR_GRID).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_grid_color(const svg_color& col)
          { //! Set the color of X-axis minor grid lines.
            derived().image_.g(PLOT_X_MINOR_GRID).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_minor_grid_color()
          { //! \return  the color of X-axis minor grid lines.
            return derived().image_.g(PLOT_X_MINOR_GRID).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_grid_width(double w)
          { //! Set the width of X-axis minor grid lines.
            derived().image_.g(PLOT_X_MINOR_GRID).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_grid_width()
          { //! \return  the width of X-axis minor grid lines.
            return derived().image_.g(PLOT_X_MINOR_GRID).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_width(double width)
          { //! Set the width of X-axis lines.
            derived().image_.g(PLOT_X_AXIS).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_axis_width()
          { //! \return  the width of X-axis lines.
            return derived().image_.g(PLOT_X_AXIS).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::data_lines_width(double width)
          { //! Set the width of lines joining data points.
            derived().image_.g(PLOT_DATA_LINES).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::data_lines_width()
          { //! \return  the width of lines joining data points.
            return derived().image_.g(PLOT_DATA_LINES).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label(const std::string& str)
          { //! Set the text to label the X-axis (and set x_label_on(true)).
            derived().x_label_info_.text(str);
            derived().x_axis_.label_on_ = true; // Assume want x_label string displayed.
            // Might switch label_on false if null string?
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::x_label()
          { //! \return  the text to label the X-axis.
            return derived().x_label_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_units(const std::string& str)
          { //! Set the text to add units to the X-axis label.
            derived().x_units_info_.text(str);
            derived().x_axis_.label_on_ = true; // Assume want x_label string displayed.
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::x_label_units()
          { //! \return  the text to add units to the X-axis label.
            //! The label will only be shown if  x_label_on() == true.
            return derived().x_units_info_.text();
          }

          // y_label not needed in 1D.
          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_label(const std::string& str)
          { //! Set the text for the Y-axis label (and set y_label_on(true)).
            derived().y_label_info_.text(str);
            derived().y_axis_.label_on_ = true; // Assume want y_label string displayed.
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::y_label()
          { //! \return  the text for the Y-axis label.
            //! The label will only be shown if  y_label_on() == true.
            return derived().y_label_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_label_units(const std::string& str)
          { //! Set the text to add units to the Y-axis label.
            derived().y_units_info_.text(str);
            derived().y_axis_.label_on_ = true; // Assume want y_label string displayed.
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::y_label_units()
          { //! \return  the text to add units to the X-axis label.
            return derived().y_units_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_on(bool b)
          { //! \return true if values of X data points are shown (for example: 1.23).
            // (Want override xy_values_on that would otherwise cause overwriting).
            // So the last values_on setting will prevail.
            // But this is only defined in 2D
            //if(derived().xy_values_on())
            //{ // Would be overwritten by XY pair.
            //  derived().xy_values_on(false);
            //}
            derived().x_values_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_values_on()
          { //! If true, show data point values near data points markers.
            return derived().x_values_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_font_size(int i)
          { //! Set font size of data point X values near data points markers.
            derived().x_values_style_.values_text_style_.font_size(i);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_values_font_size()
          { //! \return  font size of data point X values near data points markers.
            return derived().x_values_style_.values_text_style_.font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_font_family(const std::string& family)
          { //! Set font family of data point X values near data points markers.
            derived().x_values_style_.values_text_style_.font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::x_values_font_family()
          { //! Set font family of data point X values near data points markers.
            return derived().x_values_style_.values_text_style_.font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_interval(double inter)
          { //! Set the interval between X-axis major ticks.
            derived().x_ticks_.major_interval_ = inter;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_interval()
          { //! \return  the interval between X-axis major ticks.
            return derived().x_ticks_.major_interval_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_color(const svg_color& col)
          { //! Set the color of data point X values near data points markers.
            // Function could set both fill (middle) and stroke (outside),
            // but just setting fill is simplest,
            // but does not allow separate inside & outside colors.
            // Might be better to set in x_values_style
            derived().image_.g(PLOT_X_POINT_VALUES).style().fill_color(col);
            //derived().image_.g(PLOT_X_POINT_VALUES).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_values_color()
          { //! \return  the color of data point X values near data points markers.
            // Function could get either fill and stroke,
            // return derived().image_.g(PLOT_X_POINT_VALUES).style().stroke_color();
            return derived().image_.g(PLOT_X_POINT_VALUES).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_rotation(rotate_style rotate)
          { //! \return  the rotation (rotate_style) of data point X values near data points markers.
            //! (Degrees: 0 to 360 in 45 steps).
            derived().x_values_style_.value_label_rotation_ = rotate;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_values_rotation()
          { //! \return  the rotation of data point X values near data points markers.
            return derived().x_values_style_.value_label_rotation_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_precision(int p)
          { //! Set iostream decimal digits precision of data point X values near data points markers.
            derived().x_values_style_.value_precision_ = p;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_values_precision()
          { //! \return  iostream decimal digits precision of data point X values near data points markers.
            //!
            return derived().x_values_style_.value_precision_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_ioflags(std::ios_base::fmtflags f)
          { //! Set @c std iostream format flags of data point X values near data points markers.
            //! Useful to set hexadecimal, fixed and scientific, (std::ios::scientific).
            derived().x_values_style_.value_ioflags_ = f;
            return derived();
          }

          template <class Derived>
          std::ios_base::fmtflags axis_plot_frame<Derived>::x_values_ioflags()
          { //! \return  iostream format flags of data point X values near data points markers.
            //! Might be used to set hexadecimal, fixed and scientific, (std::ios::scientific).
            return derived().x_values_style_.value_ioflags_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_plusminus_on(bool b)
          { //! Set if to append std_dev estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            derived().x_values_style_.plusminus_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_plusminus_on()
          { //! \return  if to append std_dev estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.plusminus_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_plusminus_color(const svg_color& col)
          { //! Set the color of X std_dev of value, for example, the color of 0.02 in "1.23 +-0.02 (9)".
            derived().x_values_style_.plusminus_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_plusminus_color()
          { //! Get the color of X std_dev of value, for example, the color of 0.02 in "1.23 +-0.02 (9)".
            return derived().x_values_style_.plusminus_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_addlimits_on(bool b)
          { //! Set if to append confidence limits to data point X values near data points markers.
            //! (May not be implemented yet).
            derived().x_values_style_.addlimits_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_addlimits_on()
          { //! \return  if to append confidence limits to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.addlimits_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_addlimits_color(const svg_color& col)
          { //! Set the color of X confidence limits value, for example, the color of "<1.23 , 1.34>".
            derived().x_values_style_.addlimits_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_addlimits_color()
          { //! Get the color of X confidence limits of value, for example, the color of "<1.23 , 1.34>"".
            return derived().x_values_style_.addlimits_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_df_on(bool b)
          { //! Set true if to append a degrees of freedom estimate to data point X values near data points markers.
            derived().x_values_style_.df_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_df_on()
          { //! \return true if to append a degrees of freedom estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.df_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_df_color(const svg_color& col)
          { //! Set the color of X degrees of freedom, for example, the color of 9 in "1.23 +-0.02 (9)".
            derived().x_values_style_.df_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_df_color()
          { //! Get the color of X degrees of freedom, for example, the color of 9 in "1.23 +-0.02 (9)".
            return derived().x_values_style_.df_color_;
          }

          // Additional for Meas

         template <class Derived>
          Derived& axis_plot_frame<Derived>::x_id_on(bool b)
          { //! Set true if to append a id or name to data point X values near data points markers.
            //! (May not be implemented yet).
            derived().x_values_style_.id_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_id_on()
          { //! \return true if to append an ID or name to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.id_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_id_color(const svg_color& col)
          { //! Set the color of X ID or name, for example, the color of text in "My_id".
            derived().x_values_style_.id_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_id_color()
          { //! Get the color of X ID or name, for example, the color of text in "My_id".
            return derived().x_values_style_.id_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_datetime_on(bool b)
          { //! Set true if to append a datetime to data point X values near data points markers.
            //! (May not be implemented yet).
            derived().x_values_style_.datetime_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_datetime_on()
          { //! \return true if to append a ID or name to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.datetime_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_datetime_color(const svg_color& col)
          { //! Set the color of X point datetime, for example, the color of text in "2004-Jan-1 05:21:33.20".
            derived().x_values_style_.datetime_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_datetime_color()
          { //! Get the color of X point date time, for example, the color of text in "2004-Jan-1 05:21:33.20".
            return derived().x_values_style_.datetime_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_order_on(bool b)
          { //! Set true if to append an order # to data point X values near data points markers.
            derived().x_values_style_.order_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_order_on()
          { //! \return true if to append an order # to data point X values near data points markers.
            return derived().x_values_style_.order_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_order_color(const svg_color& col)
          { //! Set the color of X point order in sequence, for example, #3.
            derived().x_values_style_.order_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_order_color()
          { //! Get the color of X point order in sequence, for example, #3.
            return derived().x_values_style_.order_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_decor(const std::string& pre, const std::string& sep, const std::string& suf)
          { /*! Set prefix, separator and suffix for x_style
            \note if you want a space, you must use a Unicode space "\&#x00A0;",
            for example, ",\&#x00A0;" rather than just ", ".
            */
            derived().x_values_style_.prefix_ = pre;
            derived().x_values_style_.separator_ = sep;
            derived().x_values_style_.suffix_ = suf;
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_prefix()
          { //! Get the prefix (only used if separator != "")
            return derived().x_values_style_.prefix_;
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_suffix()
          { //! Get the suffix (only used if separator != "")
            return derived().x_values_style_.suffix_;
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_separator()
          { //! Get separator (also controls use of the prefix & suffix - they are only used if separator != "").
            //! Note For a space, you must use a Unicode space "\&#x00A0;",
            //! for example, ",\&#x00A0;" rather than ", ".
            return derived().x_values_style_.separator_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick_length(double length)
          { //! Set length of X major ticks.
            derived().x_ticks_.major_tick_length_ = length;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_tick_length()
          {//! \return  length of X major ticks.
            return derived().x_ticks_.major_tick_length_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick_width(double width)
          { //! Set width of X major ticks.
            derived().x_ticks_.major_tick_width_ = width; // Redundant?
            derived().image_.g(PLOT_X_MAJOR_TICKS).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_tick_width()
          {//! \return  width of X major ticks.
            return derived().image_.g(PLOT_X_MAJOR_TICKS).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_tick_length(double length)
          { //! Set length of X minor ticks.
            derived().x_ticks_.minor_tick_length_ = length;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_tick_length()
          { //! \return  length of X minor ticks.
            return derived().x_ticks_.minor_tick_length_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_tick_width(double width)
          { //! Set width of X minor ticks.
            derived().x_ticks_.minor_tick_width_ = width;
            derived().image_.g(PLOT_X_MINOR_TICKS).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_tick_width()
          { //! \return  width of X minor ticks.
            // return derived().x_minor_tick_width_; // should be the same but store in stroke_width is definitive.
            return derived().image_.g(PLOT_X_MINOR_TICKS).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick(double d)
          { //! Set interval (Cartesian units) between major ticks.
            derived().x_ticks_.major_interval_ = d;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_tick()
          { //! \return  interval (Cartesian units) between major ticks.
            return derived().x_ticks_.major_interval_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_interval(double interval)
          { //! Set interval between X-axis minor ticks.
            // aka x_minor_tick
            derived().x_ticks_.minor_interval_ = interval;
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_num_minor_ticks(int num)
          { //! Set number of X-axis minor ticks between major ticks.
            derived().x_ticks_.num_minor_ticks_ = num;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_num_minor_ticks()
          { //! \return  number of X-axis minor ticks between major ticks.
            //! Note: NOT float or double!
            return derived().x_ticks_.num_minor_ticks_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_range(double min_x, double max_x)
          { //! Set the range of values on the X-axis.
            //! The minimum and maximum values must be finite and not too near
            //! to the minima or maxima that can be represented by floating point doubles,
            //! and the range must not be too small.
            if (!boost::math::isfinite(min_x))
            {
              throw std::runtime_error("X range: min not finite!");
            }
            if (!boost::math::isfinite(max_x))
            {
              throw std::runtime_error("X range: max not finite!");
            }
            if(max_x <= min_x)
            { // max_x <= min_x.
              std::stringstream message("X range: max <= min! ");
              message << max_x << " <= " << min_x << std::ends;
              throw std::runtime_error(message.str());
              //throw std::runtime_error("X range: max <= min!");
            }
            if( (std::abs<double>(max_x - min_x) < std::numeric_limits<double>::epsilon() * 1000 * std::abs<double>(max_x))
              || (std::abs<double>(max_x - min_x) < std::numeric_limits<double>::epsilon() * 1000 * std::abs<double>(min_x))
              )
            { // Range too small to display.
              throw std::runtime_error("X range too small!" );
            }
            derived().x_axis_.min_ = min_x;
            derived().x_axis_.max_ = max_x;
            //derived().x_ticks_.max_ = min_x;
            //derived().y_ticks_.max_ = max_x;
            // done in calculate_plot_window, so need to duplicate here.
            derived().x_autoscale_ = false; // Because explicit range has just been set.
            return derived();
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::x_range()
          { //! \return  the range of values on the X-axis.
            //! (Need to use boost::svg::detail::operator<< to display this).
            std::pair<double, double> r;
            r.first = derived().x_axis_.min_;
            r.second = derived().x_axis_.max_;
            return r;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_min(double min_x)
          { //! Set the minimum value on the X-axis.
            // Not useful to check here that x_max_ > x_min_ because may not have set x_min_ yet.
            derived().x_axis_.min_ = min_x;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_min()
          { //! \return  the minimum value on the X-axis.
            //! (Can also get both minimum and maximum as a std::pair).
            return derived().x_axis_.min_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_max(double x)
          { //! Set the maximum value on the X-axis.
            // Not useful to check here that x_max_ > x_min_ because may not have set x_min_ yet.
            derived().x_axis_.max_ = x;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_max()
          { //! \return  the maximum value on the X-axis.
            //! (Can also get both minimum and maximum as a std::pair).
            return derived().x_axis_.max_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::autoscale_check_limits(bool b)
          { //! Set to check that values used for autoscale are within limits.
            //! Default is true, but can switch off checks for speed.
            derived().autoscale_check_limits_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::autoscale_check_limits()
          { //! \return  to check that values used for autoscale are within limits.
            //! Default is true, but can switch off checks for speed.
            return derived().autoscale_check_limits_;
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_autoscale()
          { //! \return  true if to use autoscale value for X-axis.
           return derived().x_autoscale_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_autoscale(bool b)
          { //! Set @c true if to use autoscaled values for X-axis.
             if (b && derived().x_auto_tick_interval_ < 0)
             { // No autoscale values have been calculated, so not safe to make x_autoscale true.
                throw std::runtime_error("X autoscale has not been calculated yet!" );
             }
            derived().x_autoscale_ = b;
            return derived(); //! \return Reference to caller to make chainable.
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::autoscale()
          { //! \return true if to use autoscale values for X-axis.
            //! autoscale() is same as x_autoscale.
           return derived().x_autoscale_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::autoscale(bool b)
          { //! Set whether to use X autoscaled values.
            //! Same as x_autoscale, and used by boxplot too.
             if (derived().x_auto_tick_interval_ < 0)
             { // No autoscale values have been calculated, so not safe to make x_autoscale true.
                throw std::runtime_error("X-axis autoscale has not been calculated yet!" );
             }
            derived().x_autoscale_ = b;
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::autoscale_plusminus(double pm)
          { //! Set how many std_dev or standard deviation to allow for ellipse when autoscaling.
            //! Default is 3 for 99% confidence.
            derived().autoscale_plusminus_ = pm;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::autoscale_plusminus()
          { //! \return  how many std_dev or standard deviation to allow for ellipse when autoscaling.
            //! Default is 3 for 99% confidence.
            return derived().autoscale_plusminus_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::confidence(double alpha)
          { //! Set alpha for displaying confidence intervals.
            //! Default is 0.05 for 95% confidence.
            if (alpha <= 0.)
            { // Warn and leave alpha_ unchanged.
               std::cout << "alpha must be > 0." << std::endl;
            }
            else if (alpha > 0.5)
            { // Warn and leave alpha_ unchanged.
               std::cout << "alpha must be fraction < 0.5 (for example, 0.05 for 95% confidence)" << std::endl;
            }
            else
            {
              derived().alpha_ = alpha;
            }
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::confidence()
          { //! \return  alpha for displaying confidence intervals.
            //! Default is 0.05.
            return derived().alpha_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_autoscale(std::pair<double, double> p)
          { //! Set to use X min & max pair of double values to autoscale X-axis.
            scale_axis(p.first, p.second, // double min and max from pair.
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_, derived().autoscale_plusminus_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);
            derived().x_autoscale_ = true; // Default to use any calculated values?
            return derived();
          } // autoscale(pair<double, double> p)

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_with_zero(bool b)
          { //! Set X-axis autoscale to include zero (default = false).
            //! Must preceed x_autoscale(data) call.
            derived().x_include_zero_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_with_zero()
          { //! \return  true if X-axis autoscale to include zero (default = false).
           return derived().x_include_zero_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_min_ticks(int min_ticks)
          { //! Set X-axis autoscale to include at least minimum number of ticks (default = 6).
            //! Must preceed x_autoscale(data) call.
            derived().x_min_ticks_ = min_ticks;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_min_ticks()
          { //! \return  X-axis autoscale minimum number of ticks.
           return derived().x_min_ticks_;
          } // x_min_ticks()

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_steps(int steps)
          { /*! Set autoscale to set ticks in steps multiples of:\n
             2,4,6,8,10, if 2\n
             or 1,5,10 if 5\n
             or 2,5,10 if 10.\n
             default = 0 (none).
             \note: Must \b preceed x_autoscale(data) call).
             */
            derived().x_steps_ = steps;
            return derived();
          } // x_steps(int steps)

          template <class Derived>
          int axis_plot_frame<Derived>::x_steps()
          { //! \return  autoscale to set ticks in steps.
           return derived().x_steps_;
          } // x_steps()

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_tight(double tight)
          { //! Set tolerance to autoscale to permit data points slightly outside both end ticks.
            //! default 0.
            //! Must preceed x_autoscale(data) call.
            derived().x_tight_ = tight;
            return derived();
          }

         template <class Derived>
         double axis_plot_frame<Derived>::x_tight()
         { //! \return  tolerance given to autoscale to permit data points slightly outside both end ticks.
           return derived().x_tight_;
         }

         template <class Derived>
         double axis_plot_frame<Derived>::x_auto_min_value()
         { //! \return  the X-axis minimum value computed by autoscale.
           return derived().x_auto_min_value_;
         }

         template <class Derived>
         double axis_plot_frame<Derived>::x_auto_max_value()
         { //! \return  the X-axis maximum value computed by autoscale.
           return derived().x_auto_max_value_;
         }

         template <class Derived>
         double axis_plot_frame<Derived>::x_auto_tick_interval()
         { //! \return  the X-axis major tick interval computed by autoscale.
           return derived().x_auto_tick_interval_;
         }

        template <class Derived>
        int axis_plot_frame<Derived>::x_auto_ticks()
        { //! \return  the X-axis number of major ticks computed by autoscale.
         return derived().x_auto_ticks_;
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::nan_limit_color(const svg_color& col)
        { //! Set the color for NaN point stroke color.
          derived().nan_point_style_.stroke_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::nan_limit_color()
        { //! \return  the color for the NaN point stroke color.
          return derived().nan_point_style_.stroke_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::nan_limit_size(int limit_marker_size)
        { //! Set the size for NaN marker(s).
          derived().nan_point_style_.size(limit_marker_size);
          return derived();
        }

        template <class Derived>
        int axis_plot_frame<Derived>::nan_limit_size()
        { //! \return The font size for the NaN point(s) marker shape or symbol.
          return derived().nan_point_style_.size_;
        }


        template <class Derived>
        Derived& axis_plot_frame<Derived>::plus_inf_limit_color(const svg_color& col)
        { //! Set the color for +infinity  point stroke color.
          derived().plus_inf_point_style_.stroke_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::plus_inf_limit_color()
        { //! \return  the color for the +infinity  point stroke color.
          return derived().plus_inf_point_style_.stroke_color();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::plus_inf_limit_fill_color()
        { //! \return  the color for the +infinity  point strokefill color.
          return derived().plus_inf_point_style_.fill_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::plus_inf_limit_size(int limit_marker_size)
        { //! Set the size for +infinity marker(s).
          derived().plus_inf_point_style_.size(limit_marker_size);
          return derived();
        }

        template <class Derived>
        int axis_plot_frame<Derived>::plus_inf_limit_size()
        { //! \return The font size for the NaN point(s) marker shape or symbol.
          return derived().plus_inf_point_style_.size_;
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::minus_inf_limit_color(const svg_color& col)
        { //! Set the color for -minus infinity point stroke color.
          derived().minus_inf_point_style_.stroke_color(col);
          return derived();
        }
        template <class Derived>
        Derived& axis_plot_frame<Derived>::minus_inf_limit_fill_color(const svg_color& col)
        { //! Set the color for -minus infinity point fill color.
          derived().minus_inf_point_style_.fill_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::minus_inf_limit_fill_color()
        { //! \return  the color for the +infinity  point strokefill color.
          return derived().minus_inf_point_style_.fill_color();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::minus_inf_limit_color()
        { //! \return  the color for the -minus infinity  point stroke color.
          return derived().minus_inf_point_style_.stroke_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::minus_inf_limit_size(int limit_marker_size)
        { //! Set the size for -minus infinity marker(s).
          derived().minus_inf_point_style_.size(limit_marker_size);
          return derived();
        }

        template <class Derived>
        int axis_plot_frame<Derived>::minus_inf_limit_size()
        { //! \return The font size for the -infinity point(s) marker shape or symbol.
          return derived().minus_inf_point_style_.size_;
        }

         template <class Derived>
        Derived& axis_plot_frame<Derived>::nan_limit_fill_color(const svg_color& col)
        { //! Set the color for 'at limit' point fill color.
          derived().nan_point_style_.fill_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::nan_limit_fill_color()
        { //! \return  the color for the 'at limit' point fill color.
          return derived().image_.g(detail::PLOT_LIMIT_POINTS).style().fill_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::one_sd_color(const svg_color& col)
        { //! Set the color for the one standard deviation (~67% confidence) ellipse fill.
          derived().image_.g(detail::PLOT_DATA_UNC1).style().fill_on(true);
          derived().image_.g(detail::PLOT_DATA_UNC1).style().fill_color(col);
          derived().image_.g(detail::PLOT_DATA_UNC1).style().stroke_color(blank);
          return derived();
        }

       template <class Derived>
       svg_color axis_plot_frame<Derived>::one_sd_color()
       { //! \return Color for the one standard deviation (~67% confidence) ellipse fill.
         return derived().image_.g(detail::PLOT_DATA_UNC1).style().fill_color();
       }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::two_sd_color(const svg_color& col)
        { //! Set the color for two standard deviation (~95% confidence) ellipse fill.
          derived().image_.g(detail::PLOT_DATA_UNC2).style().fill_on(true);
          derived().image_.g(detail::PLOT_DATA_UNC2).style().fill_color(col);
          derived().image_.g(detail::PLOT_DATA_UNC2).style().stroke_color(blank);
          return derived();
        }

       template <class Derived>
       svg_color axis_plot_frame<Derived>::two_sd_color()
       { //! \return Color for two standard deviation (~95% confidence) ellipse fill.
         return derived().image_.g(detail::PLOT_DATA_UNC2).style().fill_color();
       }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::three_sd_color(const svg_color& col)
        { //! Set the color for three standard deviation (~99% confidence) ellipse fill.
          derived().image_.g(detail::PLOT_DATA_UNC3).style().fill_on(true);
          derived().image_.g(detail::PLOT_DATA_UNC3).style().fill_color(col);
          derived().image_.g(detail::PLOT_DATA_UNC3).style().stroke_color(blank);
          return derived();
        }

       template <class Derived>
       svg_color axis_plot_frame<Derived>::three_sd_color()
       { //! \return Color for three standard deviation (~99% confidence) ellipse fill.
         return derived().image_.g(detail::PLOT_DATA_UNC3).style().fill_color();
       }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::draw_note(double x, double y, std::string note, rotate_style rot /*= horizontal*/, align_style al/* = center_align*/, const svg_color& col /* black */, text_style& tsty/* = no_style*/)
        { /*! \brief Annotate plot with a  text string (perhaps including Unicode), putting note at SVG Coordinates X, Y.
            \details Defaults color black, rotation horizontal and align = center_align
            Using center_align is recommended as it will ensure that will center correctly
            (even if original string is made much longer because it contains Unicode,
            for example Greek or math symbols, taking about 6 characters per symbol)
            because the render engine does the centering.
          */
           g_element* g = &(derived()).image_.add_g_element(); // New group.
           g->style().fill_color(col); // Set its color
           g->push_back(new text_element(x, y, note, tsty, al, rot)); // Add to document image.
          // No checks on X or Y - leave to SVG to not draw outside image.
          // Could warn if X and/or Y outside - but even if OK, then text could still stray outside image.
          return derived();
        } // void draw_note()

        template <class Derived>
        Derived& axis_plot_frame<Derived>::draw_line(double x1, double y1, double x2, double y2, const svg_color& col /* black */)
        { /*! \brief Annotate plot with a line from SVG Coordinates X1, Y1 to X2, Y2.
              \details Default color black.
          */
          g_element* g = &(derived()).image_.add_g_element(); // New group.
          g->style().stroke_color(col);
          //g->style().width(w); // todo
          g->push_back(new line_element(x1, y1, x2, y2));
          // No checks on X or Y - leave to SVG to not draw outside image.
          // Could warn if X and/or Y outside ?
          return derived();
        } // void draw_line()

        template <class Derived>
        Derived& axis_plot_frame<Derived>::draw_plot_line(double x1, double y1, double x2, double y2, const svg_color& col /* black */)
        { /*! \brief Annotate plot with a line from user's Cartesian Coordinates X1, Y1 to X2, Y2.
              \details For example, -10, -10, +10, +10, Default color black.
          */
          derived().calculate_plot_window(); // To ensure the scale and shift are setup for transform.
          // It would be better to store the line (and curve and text) information like plot data series to
          // ensure that transform works correctly.
          // This assumes that the notes, lines and curves are the last item before the write.
          transform_point(x1, y1);
          transform_point(x2, y2);
          g_element* g = &(derived()).image_.add_g_element(); // New group.
          g->style().stroke_color(col);
          g->push_back(new line_element(x1, y1, x2, y2));
          // No checks on X or Y - leave to SVG to not draw outside image.
          // Actually we want to use clip_path for the plot area.
          // Could warn if X and/or Y outside ?
          return derived();
        } // void draw_plot_line()

        template <class Derived>
        Derived& axis_plot_frame<Derived>::draw_plot_curve(double x1, double y1, double x2, double y2, double x3, double y3, const svg_color& col /* black */)
        { /*! \brief Annotate plot with a line from user's Cartesian Coordinates X1, Y1 via X2, Y2 to X3, Y3.
              \details For example, -10, -10, +10, +10, Default color black.
          */
          derived().calculate_plot_window(); // To ensure the scale and shift are setup for transform.
          // It would be better to store the line (and curve and text) information like plot data series to
          // ensure that transform works correctly.
          // This assumes that the notes, lines and curves are the last item before the write.
          transform_point(x1, y1);
          transform_point(x2, y2);
          transform_point(x3, y3);
          g_element* g = &(derived()).image_.add_g_element(); // New group.
          g->style().stroke_color(col);
         // g_inner_ptr->style().stroke_width(derived().serieses_[i].line_style_.width_);

          g->push_back(new qurve_element(x1, y1, x2, y2, x3, y3));
          // No checks on X or Y - leave to SVG to not draw outside image.
          // Actually we want to use clip_path for the plot area.
          // Could warn if X and/or Y outside ?
          return derived();
        } // void draw_plot_curve

//! \endcond // DETAIL

      } // detail
    } // svg
  } // boost

#endif // BOOST_SVG_AXIS_PLOT_FRAME_HPP
