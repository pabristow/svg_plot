 /*!  \file
  \brief SVG Plot functions common to 1D, 2D and Boxplots.
  \details Functions are derived from base class @c axis_plot_frame.
  \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009, 2012, 2013

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

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
         void size_legend_box();
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
         void clear_y_axis(); //!< Clear the Y axis layer of the SVG plot.
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

          Derived& size(unsigned int x, unsigned int y); //!<  Set SVG image size (SVG units, default pixels).
          std::pair<double, double> size(); //!<  \return SVG image size, both horizontal width and vertical height (SVG units, default pixels).

          Derived& x_size(unsigned int i); //!< Set SVG image X-axis size (SVG units, default pixels).
          unsigned int x_size(); //!<  \return SVG image X-axis size as horizontal width (SVG units, default pixels).
          unsigned int image_x_size(); //!<  Obselete - deprecated use x_size()
          Derived& image_x_size(unsigned int i); //!<  Obselete - deprecated - use x_size().

          unsigned int y_size(); //!<  \return SVG image Y-axis size as vertical height (SVG units, default pixels).
          Derived& y_size(unsigned int i); //!<  Set SVG image Y-axis size (SVG units, default pixels).
          unsigned int image_y_size(); //!<  Obselete - deprecated - use y_size()
          Derived& image_y_size(unsigned int i); //!<  Obselete - deprecated - use y_size()

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
              A title that includes a greek omega and degree symbols:
              \code
                my_plot.title("Plot of &#x3A9; function (&#x00B0;C)");
              \endcode

              Unicode symbols are at http://unicode.org/charts/symbols.html.
            */
          Derived& title(const std::string title);
          const std::string title(); //!<  \return Title for plot (whose string may include Unicode for greek letter and symbols).
          Derived& title_font_size(unsigned int i); //!<  Sets the font size for the title (SVG units, default pixels).
          unsigned int title_font_size(); //!<  \return Font size for the title (SVG units, default pixels).
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
          // Legend.
          Derived& legend_width(double width); //!<  Set the width for the legend box.
          double legend_width(); //!<  \return Width for the legend box.
          Derived& legend_title(const std::string title); //!<  Set the title for the legend.
          const std::string legend_title(); //!<  \return Title for the legend.
          Derived& legend_font_weight(const std::string& weight); //!<  Set the font weight for the legend title.
          const std::string& legend_font_weight(); //!<  \return  Font weight for the legend title.
          Derived& legend_font_family(const std::string& family); //!<  Set the font family for the legend title.
          const std::string& legend_font_family(); //!<  \return  the font family for the legend title.
          Derived& legend_title_font_size(unsigned int size); //!<  \return Font family for the legend title.
          unsigned int legend_title_font_size(); //!<  \return Font size for the legend title (svg units, default pixels).
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
          Derived& legend_header_font_size(int size); //!<  Set legend header font size (svg units, default pixels).
          int legend_header_font_size(); //!<\return  legend header font size (svg units, default pixels).
          Derived& plot_window_on(bool cmd); //!<Set true if a plot window is wanted (or false if the whole image is to be used).
          bool plot_window_on();//!<\return  true if a plot window is wanted (or false if the whole image is to be used).
          Derived& plot_border_color(const svg_color& col); //!<Set the color for the plot window background.
          svg_color plot_border_color(); //!<\return  the color for the plot window background.
          Derived& plot_border_width(double w); //!<Set the margin around the plot window border (svg units, default pixels).
            //! \details This prevents the plot window getting too close to other elements of the plot.
          double plot_border_width(); //!<\return  the width for the plot window border (svg units, default pixels).
          Derived& image_border_margin(double w); //!<Set the margin around the plot window border (svg units, default pixels).
            //! \details This prevents the plot window getting too close to other elements of the plot.
          double image_border_margin(); //!<\return  the margin around the plot window border (svg units, default pixels).
          Derived& image_border_width(double w); //!<Set the svg image border width (svg units, default pixels).
          double image_border_width(); //!<\return  the svg image border width (svg units, default pixels).
          Derived& plot_window_x(double min_x, double max_x);//!<Set the minimum and maximum (cartesian data units) for the plot window X axis.
            //! This is normally calculated from other plot values.
          Derived& plot_window_y(double min_y, double max_y);//!<Set the minimum and maximum (cartesian data units) for the plot window Y axis.
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
          Derived& x_label_font_size(unsigned int i); //!<Set X axis label font size (svg units, default pixels).
          unsigned int x_label_font_size(); //!<\return  X axis label font size (svg units, default pixels).
          Derived& x_value_font_size(unsigned int i); //!<Set X tick value label font size (svg units, default pixels).
          unsigned int x_value_font_size(); //!<\return  X tick value label font size (svg units, default pixels).
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
          Derived& x_ticks_values_font_size(unsigned int i); //!<Set X ticks value label font size (svg units, default pixels).
          unsigned int x_ticks_values_font_size(); //!<Set X ticks value label font size (svg units, default pixels).

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
          Derived& x_values_font_size(unsigned int i); //!<Set font size of data point X values near data points markers.
          unsigned int x_values_font_size(); //!<\return  font size of data point X values near data points markers.
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
          Derived& x_num_minor_ticks(unsigned int num); //!<Set number of X-axis minor ticks between major ticks.
          unsigned int x_num_minor_ticks(); //!<\return  number of X-axis minor ticks between major ticks.
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
          Derived& limit_color(const svg_color&); //!<Set the color for 'at limit' point stroke color.
          svg_color limit_color(); //!<\return  the color for the 'at limit' point stroke color.
          Derived& limit_fill_color(const svg_color&); //!<Set the color for 'at limit' point fill color.
          svg_color limit_fill_color(); //!<\return  the color for the 'at limit' point fill color.
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

        // class axis_plot_frame Member function Definitions (for .ipp file):


//          template <typename Derived>/*! \tparam Derived plot class, svg_1d_plot or svg_2d_plot or svg_boxplot. */
//          template <typename T> /*! \tparam T an STL container: array, vector, list, map ...  */
//          Derived& axis_plot_frame<Derived>::x_autoscale(const T& begin, const T& end);
//
//          template <class Derived>
//          template <class T> // T an STL container: array, vector...
//          Derived& axis_plot_frame<Derived>::x_autoscale(const T& container); // Use whole 1D data series.
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::size(unsigned int x, unsigned int y);
//
//          template <class Derived>
//          std::pair<double, double> axis_plot_frame<Derived>::size();
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::image_x_size(); //!< Obselete - deprecated.
//
//           template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_size(unsigned int i);
//
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::x_size();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::image_x_size(unsigned int i); //!< Obselete - deprecated.
//
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::image_y_size(); //!< Obselete - deprecated.
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::image_y_size(unsigned int i); //!< Obselete - deprecated.
//
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::y_size();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::y_size(unsigned int i);
//                    template <class Derived>
//          svg_color axis_plot_frame<Derived>::background_color();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::background_color(const svg_color& col);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::background_border_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::background_border_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::background_border_width(double w);
//          template <class Derived>
//          double axis_plot_frame<Derived>::background_border_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::description(const std::string d);
//
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::description();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::document_title(const std::string d);
//          template <class Derived>
//          std::string axis_plot_frame<Derived>::document_title();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::copyright_holder(const std::string d);
//
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::copyright_holder();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::copyright_date(const std::string d);
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::copyright_date();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::license(std::string repro,
//            std::string distrib,
//            std::string attrib,
//            std::string commercial,
//            std::string derivative);
//
//          template <class Derived>
//          bool axis_plot_frame<Derived>::license_on();
//          template <class Derived>
//          Derived&  axis_plot_frame<Derived>::license_on(bool l);
//
//          template <class Derived>
//          bool axis_plot_frame<Derived>::boost_license_on();
//
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::license_reproduction();
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::license_distribution();
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::license_attribution();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::coord_precision(int digits);
//          template <class Derived>
//          int axis_plot_frame<Derived>::coord_precision();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_value_precision(int digits);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_value_precision();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_value_ioflags(std::ios_base::fmtflags flags);
//          template <class Derived>
//          std::ios_base::fmtflags axis_plot_frame<Derived>::x_value_ioflags();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_labels_strip_e0s(bool cmd);
//
//          template <class Derived>
//          bool axis_plot_frame<Derived>::y_labels_strip_e0s();
//
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::title();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_size(unsigned int i);
//
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::title_font_size();
//
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_family(const std::string& family);
//
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::title_font_family();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_style(const std::string& style);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::title_font_style();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_weight(const std::string& weight);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::title_font_weight();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_stretch(const std::string& stretch);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::title_font_stretch();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_decoration(const std::string& decoration);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::title_font_decoration();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_rotation(rotate_style rotate);
//          template <class Derived>
//          int axis_plot_frame<Derived>::title_font_rotation();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_font_alignment(align_style alignment);
//          template <class Derived>
//          align_style axis_plot_frame<Derived>::title_font_alignment();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_width(double width);
//          template <class Derived>
//          double axis_plot_frame<Derived>::legend_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_title(const std::string title);
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::legend_title();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_font_weight(const std::string& weight);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::legend_font_weight();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_font_family(const std::string& family);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::legend_font_family();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_title_font_size(unsigned int size);
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::legend_title_font_size();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_top_left(double x, double y);
//          template <class Derived>
//          const std::pair<double, double> axis_plot_frame<Derived>::legend_top_left();
//          template <class Derived>
//          const std::pair<double, double> axis_plot_frame<Derived>::legend_bottom_right();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_lines(bool is);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::legend_lines();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::legend_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_axis_vertical(double fraction);
//
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_axis_vertical();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_place(legend_places l);
//          template <class Derived>
//          legend_places axis_plot_frame<Derived>::legend_place();
//          template <class Derived>
//          bool axis_plot_frame<Derived>::legend_outside();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_header_font_size(int size);
//          template <class Derived>
//          int axis_plot_frame<Derived>::legend_header_font_size();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::plot_window_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::plot_window_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::plot_border_color(const svg_color& col);
//          template <class Derived>
//          double axis_plot_frame<Derived>::plot_border_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::plot_border_width(double w);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::image_border_margin(double w);
//           template <class Derived>
//          double axis_plot_frame<Derived>::image_border_margin();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::image_border_width(double w);
//          template <class Derived>
//          double axis_plot_frame<Derived>::image_border_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::plot_window_x(double min_x, double max_x);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::plot_window_y(double min_y, double max_y);
//
//          template <class Derived>
//          double axis_plot_frame<Derived>::plot_window_x_left();
//
//          template <class Derived>
//          double axis_plot_frame<Derived>::plot_window_x_right();
//          template <class Derived>
//          double axis_plot_frame<Derived>::plot_window_y_top();
//          template <class Derived>
//          double axis_plot_frame<Derived>::plot_window_y_bottom();
//
//          template <class Derived>
//          std::pair<double, double> axis_plot_frame<Derived>::plot_window_x();
//          template <class Derived>
//          std::pair<double, double> axis_plot_frame<Derived>::plot_window_y();
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_minor_interval();
//          template <class Derived>
//          double axis_plot_frame<Derived>::y_minor_interval();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_up_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_ticks_up_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_down_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_ticks_down_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_label_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_font_size(unsigned int i);
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::x_label_font_size();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_value_font_size(unsigned int i);
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::x_value_font_size();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_font_family(const std::string& family);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::x_label_font_family();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_axis_label_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_axis_label_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_values_color(const svg_color& col);
//
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_ticks_values_color();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_values_precision(int p);
//
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_ticks_values_precision();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_values_ioflags(std::ios_base::fmtflags f);
//          template <class Derived>
//          std::ios_base::fmtflags axis_plot_frame<Derived>::x_ticks_values_ioflags();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_values_font_size(unsigned int i);
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::x_ticks_values_font_size();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_values_font_family(const std::string& family);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::x_ticks_values_font_family();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_ticks_on_window_or_axis(int cmd);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_ticks_on_window_or_axis();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_units_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_label_units_on();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_labels_side(int place);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_major_labels_side();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_label_rotation(rotate_style rot);
//
//          template <class Derived>
//          rotate_style axis_plot_frame<Derived>::x_major_label_rotation();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_on(bool cmd);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::title_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_grid_on(bool is);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_major_grid_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_grid_on(bool is);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_minor_grid_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::axes_on(bool is);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::axes_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_axis_on(bool is);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_axis_on();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::background_border_width(double w);
//          template <class Derived>
//          double axis_plot_frame<Derived>::background_border_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::description(const std::string d);
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::description();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::y_axis_on(bool is);
//
//          template <class Derived>
//          bool axis_plot_frame<Derived>::y_axis_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::title_color(const svg_color& col);
//
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::title_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::legend_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_background_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::legend_background_color();
//          template <class Derived>
//          bool axis_plot_frame<Derived>::legend_box_fill_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::legend_border_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::legend_border_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::plot_background_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::plot_background_color();
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::x_axis_position();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_axis_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_axis_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::y_axis_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::y_axis_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_label_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_width(double width);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_label_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::y_label_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::y_label_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_tick_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_major_tick_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_tick_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_minor_tick_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_grid_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_minor_tick_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_grid_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_major_grid_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_grid_width(double w);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_major_grid_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_grid_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_minor_grid_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_grid_width(double w);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_minor_grid_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_axis_width(double width);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_axis_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::data_lines_width(double width);
//          template <class Derived>
//          double axis_plot_frame<Derived>::data_lines_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label(const std::string& str);
//          template <class Derived>
//          std::string axis_plot_frame<Derived>::x_label();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_label_units(const std::string& str);
//          template <class Derived>
//          std::string axis_plot_frame<Derived>::x_label_units();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::y_label(const std::string& str);
//          template <class Derived>
//          std::string axis_plot_frame<Derived>::y_label();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::y_label_units(const std::string& str);
//          template <class Derived>
//          std::string axis_plot_frame<Derived>::y_label_units();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_values_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_font_size(unsigned int i);
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_font_family(const std::string& family);
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::x_values_font_size();
//
//          template <class Derived>
//          const std::string& axis_plot_frame<Derived>::x_values_font_family();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_interval(double inter);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_major_interval();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_values_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_rotation(rotate_style rotate);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_values_rotation();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_precision(int p);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_values_precision();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_values_ioflags(std::ios_base::fmtflags f);
//          template <class Derived>
//          std::ios_base::fmtflags axis_plot_frame<Derived>::x_values_ioflags();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_plusminus_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_plusminus_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_plusminus_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_plusminus_color();
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_addlimits_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_addlimits_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_addlimits_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_addlimits_color();
//
//
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_df_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_df_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_df_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_df_color();
//         template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_id_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_id_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_id_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_id_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_datetime_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_datetime_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_datetime_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_datetime_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_order_on(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_order_on();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_order_color(const svg_color& col);
//          template <class Derived>
//          svg_color axis_plot_frame<Derived>::x_order_color();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_decor(const std::string& pre, const std::string& sep, const std::string& suf);
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::x_prefix();
//          template <class Derived>
//          const std::string axis_plot_frame<Derived>::x_suffix();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_tick_length(double length);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_major_tick_length();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_tick_width(double width);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_major_tick_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_tick_length(double length);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_minor_tick_length();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_tick_width(double width);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_minor_tick_width();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_major_tick(double d);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_major_tick();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_minor_interval(double interval);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_num_minor_ticks(unsigned int num);
//          template <class Derived>
//          unsigned int axis_plot_frame<Derived>::x_num_minor_ticks();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_range(double min_x, double max_x);
//          template <class Derived>
//          double axis_plot_frame<Derived>::x_max();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::autoscale_check_limits(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::autoscale_check_limits();
//          template <class Derived>
//          bool axis_plot_frame<Derived>::x_autoscale();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_autoscale(bool b);
//          template <class Derived>
//          bool axis_plot_frame<Derived>::autoscale();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::autoscale_plusminus(double pm);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::autoscale(bool b);
//          template <class Derived>
//          double axis_plot_frame<Derived>::autoscale_plusminus();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_autoscale(std::pair<double, double> p);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_with_zero(bool b);
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_min_ticks(int min_ticks);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_min_ticks();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_steps(int steps);
//          template <class Derived>
//          int axis_plot_frame<Derived>::x_steps();
//          template <class Derived>
//          Derived& axis_plot_frame<Derived>::x_tight(double tight);
//         template <class Derived>
//         double axis_plot_frame<Derived>::x_tight();
//         template <class Derived>
//         double axis_plot_frame<Derived>::x_auto_min_value();
//         template <class Derived>
//         double axis_plot_frame<Derived>::x_auto_max_value();
//         template <class Derived>
//         double axis_plot_frame<Derived>::x_auto_tick_interval();
//        template <class Derived>
//        int axis_plot_frame<Derived>::x_auto_ticks();
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::limit_color(const svg_color& col);
//        template <class Derived>
//        svg_color axis_plot_frame<Derived>::limit_color();
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::limit_fill_color(const svg_color& col);
//        template <class Derived>
//        svg_color axis_plot_frame<Derived>::limit_fill_color();
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::one_sd_color(const svg_color& col);
//       template <class Derived>
//       svg_color axis_plot_frame<Derived>::one_sd_color();
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::two_sd_color(const svg_color& col);
//       template <class Derived>
//       svg_color axis_plot_frame<Derived>::two_sd_color();
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::three_sd_color(const svg_color& col);
//       template <class Derived>
//       svg_color axis_plot_frame<Derived>::three_sd_color();
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::draw_note(double x, double y, std::string note, rotate_style rot /*= horizontal*/, align_style al/* = center_align*/, const svg_color& col /* black */, text_style& tsty/* = no_style*/);
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::draw_line(double x1, double y1, double x2, double y2, const svg_color& col /* black */);
//        template <class Derived>
//        Derived& axis_plot_frame<Derived>::draw_plot_line(double x1, double y1, double x2, double y2, const svg_color& col /* black */);

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
          if(derived().x_ticks_.minor_grid_on_)
          {
            if(!derived().plot_window_on_)
            { // Use whole image.
              // Make space for title and X-axis labels.
              if(derived().title_on_)
              { // Allow text_margin_ * font_size around text (pixels).
                y_bottom += derived().title_info_.textstyle().font_size() * derived().text_margin_;
              }
              if(derived().x_axis_.label_on_)
              {
                y_top -= derived().x_label_info_.textstyle().font_size() * derived().text_margin_;
              }
            }
            else
            { // plot_window_on_ == true.
              y_bottom = derived().plot_top_ + derived().plot_window_border_.width_; // Top.
              y_top = derived().plot_bottom_ - derived().plot_window_border_.width_; // Bottom. Ensure *inside* window.
            }
            // Make sure that we are drawing inside the allowed window.
            if((x >= derived().plot_left_) && (x <= derived().plot_right_)) // allow = too?
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
          if(derived().x_ticks_.up_ticks_on_)
          {
            y_bottom -= derived().x_ticks_.minor_tick_length_; // up
          }
          if (derived().x_ticks_.down_ticks_on_)
          {
            y_top += derived().x_ticks_.minor_tick_length_; // down.
          }
         // Make sure that we are drawing inside the allowed window.
          if((x >= derived().plot_left_) && (x <= derived().plot_right_)) // TODO allow < or <=
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
          if((x < derived().plot_left_ - 0.01) || (x > derived().plot_right_ + 0.01))
          // Allow a bit extra to allow for round-off errors.
          { // tick value is way outside plot window, so nothing to do.
            //std::cout << derived().plot_left_ << ' '<< x << std::endl;
            // This *was* displayed for a plot.
            return;
          }
          double y_up(0.); // upper end of tick.
          double y_down(derived().image_.x_size()); // y_down = lower end of tick.
          if(derived().x_ticks_.major_grid_on_)
          { // Draw major grid vertical line.
            if(!derived().plot_window_on_)
            { // Allow a modest margin around text of title and X-axis labels, if in use.
              if(derived().title_on_)
              {
                y_up += derived().title_info_.textstyle().font_size() * derived().text_margin_;
              }
              if(derived().x_ticks_.major_value_labels_side_ != 0)
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
            if(derived().x_ticks_.ticks_on_window_or_on_axis_ < 0)
            { // Put the ticks on the plot window border (was external).
              y_up = derived().plot_bottom_; // on the window line.
              y_down = derived().plot_bottom_; // y_up = upper, y_down = lower.
              if(derived().x_ticks_.up_ticks_on_)
              {
                y_up -= x_tick_length; // up
              }
              if (derived().x_ticks_.down_ticks_on_)
              {
                y_down += x_tick_length; // down.
              }
            }
            else if(derived().x_ticks_.ticks_on_window_or_on_axis_ > 0)
            { // Put the ticks on the plot window border (was external).
              y_up = derived().plot_top_; // on the window line.
              y_down = derived().plot_top_; // y_up = upper, y_down = lower.
              if(derived().x_ticks_.up_ticks_on_)
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
              if(derived().x_ticks_.up_ticks_on_)
              {
                y_up -=  x_tick_length; // up
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

            if(derived().x_ticks_.major_value_labels_side_ != 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
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
              if(derived().x_ticks_.ticks_on_window_or_on_axis_ != 0)
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
          { // Outside plot window - so do nothing?  Warning?
            //std::cerr << "Writing draw_x_major_tick OUTside plot window: "
            //  "x = " << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
          }
        } // draw_x_major_tick

        template <class Derived>
        void axis_plot_frame<Derived>::draw_x_axis()
        { //! Draw horizontal X-axis line & plot window line to hold, and ticks and grids.
          if(derived().x_axis_.axis_line_on_)
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
          for(double x = 0.; x <= derived().x_axis_.max_; x += derived().x_ticks_.major_interval_)
          {
            for(double j = x + x_minor_jump;
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
          for(double x = 0.; x >= derived().x_axis_.min_; // ?? * (1. + 2 * std::numeric_limits<double>::epsilon());
            x -= derived().x_ticks_.major_interval_) // Want a close to test here?
          {
            // Draw minor ticks.
            for(double j = x - x_minor_jump;
              j > (x - derived().x_ticks_.major_interval_ + x_minor_jump) * (1. + 2 * std::numeric_limits<double>::epsilon());
              // Increase test value by a few bits to avoid accumulated rounding error
              // that intermittently puts minor ticks *at same value as* major ticks.
              j -= x_minor_jump)
            {
              if ((j != 0. || !derived().y_axis_.axis_line_on_)  || (derived().x_ticks_.ticks_on_window_or_on_axis_ != 0))
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
              else if ((derived().x_ticks_.label_rotation_ == uphill)  || (derived().x_ticks_.label_rotation_ == downhill))
              { // sloping 45 degrees up or down.
                y += derived().x_ticks_.label_max_space_ * sin45; // Move down from end of tick.
                if (derived().x_ticks_.down_ticks_on_ == true)
                {  // Move down for any downward ticks.
                  y += 1.1 * (std::max)(derived().x_ticks_.major_tick_length_, derived().x_ticks_.minor_tick_length_); // And avoid macro max trap!
                  // and a small space.
                  y += 0.7 * (derived().x_label_info_.textstyle().font_size() + derived().x_value_label_info_.textstyle().font_size()); // best compromise?
                }
              }
              else if ((derived().x_ticks_.label_rotation_ == slopeup)  || (derived().x_ticks_.label_rotation_ == slopedownhill))
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
                  y += derived().x_value_label_info_.textstyle().font_size() ;
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
            if(detail::limit_max(x))
            {
              x = derived().plot_right_;
            }
            if(detail::limit_max(y))
            {
              y = derived().plot_top_;
            }
            if(detail::limit_min(x))
            {
              x = derived().plot_left_;
            }
            if(detail::limit_min(y))
            {
              y = derived().plot_top_;
            }
            // If value is NaN, use zero instead.
            // TODO Do we want/get a different color or shape for NaNs?
            if(detail::limit_NaN(x))
            {
              x = 0.;
              transform_x(x);
            }
            if(detail::limit_NaN(y))
            {
              y = 0.;
              transform_y(y);
            }
          } // void adjust_limits


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
          derived().title_info_.x(derived().image_.x_size() / 2.); // Center of image.
          double y;
          y = derived().title_info_.textstyle().font_size() * derived().text_margin_; // Leave a linespace above.
          derived().title_info_.y(y);
          derived().image_.g(PLOT_TITLE).push_back(new text_element(derived().title_info_));
        } // void draw_title()

        template <class Derived>
        void axis_plot_frame<Derived>::size_legend_box()
        { //! Calculate how big the legend box needs to be.
          if(derived().legend_on_ == false)
          { // No legend wanted, so set values to show legend positions invalid?
            //derived().legend_left_ = -1.;
            //derived().legend_right_ = -1.;
            //derived().legend_top_ = -1.;
            //derived().legend_bottom_ = -1.;
            derived().legend_height_ = 0.; // At least set the size to zero.
            derived().legend_width_ = 0.;
            return;
          }
          else
          { // legend_on_ == true
            // Work out the size the legend box needs to be to hold the
            // header, markers, lines & text.
            size_t num_series = derived().serieses_.size(); // How many data series.
            int font_size = derived().legend_header_.textstyle().font_size();
            int point_size =  derived().serieses_[0].point_style_.size();
            // Use height of whichever is the biggest of point marker and font.
            double spacing = (std::max)(font_size, point_size);
            // std::cout << spacing <<  ' ' << font_size << ' ' << point_size << std::endl;
            bool is_header = (derived().legend_header_.text() != "");
            //text_element legend_header_; // legend box header or title (if any).
            //text_style legend_style_;
            double longest = string_svg_length(derived().legend_header_.text(), derived().legend_style_);
            //std::cout << "\nLegend header " << longest << " svg units." << std::endl;
            derived().legend_width_ = 2 * (derived().legend_box_.margin() * derived().legend_box_.width());
            // Don't plan to write on either side border, or on the 'forbidden' margins of the box.
            for(size_t i = 0; i < num_series; ++i)
            { // Find the longest text (if longer than header) in all the data series.
              std::string s = derived().serieses_[i].title_;
              double siz = string_svg_length(s, derived().legend_style_);
              if (siz > longest)
              {
                longest = siz;
              }
            } // for
            // std::cout << "\nLongest legend header or data descriptor " << longest << " svg units." << std::endl;
            derived().legend_width_ += longest * 0.8; // Space for longest text.
            // Kludge factor to allow for not knowing the real length.

            // Allow width for a leading space, and trailing
              derived().legend_width_ += spacing * 2.5;

            // & trailing space before box margin.
            // if (derived().serieses_[i].line_style_.line_on_) // line joining points.
            if (derived().legend_lines_)
            { // Add for colored line marker in legend.
              derived().legend_width_ += spacing * 1.5;
            }
            if(derived().serieses_[0].point_style_.shape() != none)
            { // Add for any colored data point marker, cross, round... & space.
              derived().legend_width_ += 1.5 * derived().serieses_[0].point_style_.size();
            }
            // else no point marker.

            // legend_height must be *at least* enough for
            // any legend header and text_margin(s) around it
            // (if any) plus a text_margin_ top and bottom.
            // Add more height depending on the number of lines of text.
            derived().legend_height_ = spacing; // At top
            if ( (is_header) // is a legend header line.
              && (derived().legend_header_.text() != "") )
            {
              derived().legend_height_ += 2 * font_size; // text & space after.
            }
            derived().legend_height_ += num_series * spacing * 2; // Space for the data point symbols & text.
          } // legend_on_ == true

         //std::cout << "Legend width " << derived().legend_width_ << ", height " << derived().legend_height_ << std::endl;
        } //  void size_legend_box()

        template <class Derived>
        void axis_plot_frame<Derived>::place_legend_box()
        { //! Place legend box (if required).
          if(derived().legend_on_ == true) // Legend box required.
          {
            derived().outside_legend_on_ = true; // Unless proves to be inside.
            //double spacing = derived().legend_box_.margin(); // Might be better to use this, but needs redoing.
            double spacing = derived().y_axis_label_style_.font_size() * 1.; // Around any legend box - beyond any border.
            switch (derived().legend_place_)
            {
            case nowhere:
              return; // Actually places it at (0, 0), probably overwriting the plot.
            case somewhere:
              // Assume legend_top_left will place it somewhere where there is nothing else.
               //derived().legend_left_; set by legend_top_left
               //derived().legend_top_;
               derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
               derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
               break;
            case inside:
              derived().outside_legend_on_ = false;
              if (derived().legend_left_ == -1)
              { // Legend box position NOT been set by legend_top_left.
                // Default inside position is top left level with plot window.
              derived().legend_left_ = derived().image_border_.width_ + derived().image_border_.margin_; // left image edge + space.
              derived().plot_left_ += derived().legend_width_ + spacing; // Push plot window right same amount to make room,
              derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
              derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
              derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
            }
             else
             { // Legend position has been specified by legend_top_left.
                break;
             }
             break;
                // If outside then reserve space for legend by reducing plot window.
            case outside_right: // Default legend position is outside_right,
              // so that it isn't too close to the image edge or the plot window.
              derived().plot_right_ -= derived().legend_width_ + spacing; // Narrow plot window from right.
              derived().legend_left_ = derived().plot_right_  + spacing; // plot + border.
              derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
              derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
              derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
              break;
            case outside_left:
              derived().plot_left_ += derived().legend_width_ + spacing /2 ; // Push plot window right same amount to make room,
              derived().legend_left_ = derived().image_border_.width_ + derived().image_border_.margin_; // left image edge + space.
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
               derived().legend_bottom_ -= (derived().image_border_.width_ + derived().image_border_.margin_); // up
               derived().legend_top_ = derived().legend_bottom_ - derived().legend_height_;
               derived().legend_left_ = derived().image_.x_size()/  2. - derived().legend_width_ / 2; // Center.
               derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
               derived().plot_bottom_ = derived().legend_top_;
               derived().plot_bottom_ -= 2 * spacing;
              break;
              } // switch

            //std::cout << "Legend: left " << derived().legend_left_
            //    << ", right " << derived().legend_right_
            //    << ", top " << derived().legend_top_
            //    << ", bottom " << derived().legend_bottom_
            //    << std::endl;

              // Check if the location requested will fit,
              // now that we know the size of box needed.
              if ( (derived().legend_left_ < 0) || (derived().legend_left_ > derived().image_.x_size()))
              { // left outside image?
                std::cout << "Legend top left " << derived().legend_left_
                  << " is outside image size = " << derived().image_.x_size() << std::endl;
              }
              if ((derived().legend_right_ < 0) || (derived().legend_right_ > derived().image_.x_size()))
              { // right outside image?
                std::cout << "Legend top right " << derived().legend_right_
                  << " is outside image size = " << derived().image_.x_size() << std::endl;
              }
              if ((derived().legend_top_ < 0) || (derived().legend_top_ > derived().image_.y_size()))
              { // top outside image?
                std::cout << "Legend top " << derived().legend_top_
                  << " outside image!" << derived().image_.y_size() << std::endl;
              }
              if ((derived().legend_bottom_  < 0 ) || (derived().legend_bottom_ > derived().image_.y_size()))
              { // bottom outside image?
                std::cout << "Legend bottom " << derived().legend_bottom_
                  << " outside " << derived().image_.y_size() << std::endl;
              }

               derived().image_.g(detail::PLOT_LEGEND_BACKGROUND)
              .style().fill_color(derived().legend_box_.fill()) //
              .stroke_color(derived().legend_box_.stroke())
              .stroke_width(derived().legend_box_.width())
              .stroke_on(derived().legend_box_.border_on())
              ;

              // Draw border box round legend.
              g_element* g_ptr = &(derived().image_.g(PLOT_LEGEND_BACKGROUND));
              g_ptr->push_back(new
                rect_element(derived().legend_left_, derived().legend_top_, derived().legend_width_, derived().legend_height_));
            } // if legend_on_
          } //  void calculate_legend_box()

          template <class Derived>
          void axis_plot_frame<Derived>::draw_legend()
          { //! Draw the legend border, text header (if any) 
            //! and data point marker lines and/or shapes.
            // size_t num_points = derived().series.size();
            // cout << derived().legend_box_.width() <<  ' ' << derived().legend_box_.margin() << endl;

            int font_size = derived().legend_header_.textstyle().font_size();
            int point_size =  derived().serieses_[0].point_style_.size();
            // Use whichever is the biggest of point marker and font.
            double spacing = (std::max)(font_size, point_size);
            // std::cerr << spacing <<  ' ' << font_size << ' ' << point_size << endl;
            bool is_header = (derived().legend_header_.text() != "");

           // Assume legend box position has already been sized and positioned by function calculate_legend_box.
            double legend_x_start = derived().legend_left_; // Saved box location.
            double legend_width = derived().legend_width_;
            double legend_y_start = derived().legend_top_;
            double legend_height = derived().legend_height_;

            // Draw border box round legend.
            g_element* g_ptr = &(derived().image_.g(PLOT_LEGEND_BACKGROUND));

            g_ptr->push_back(new
              rect_element(legend_x_start, legend_y_start, legend_width, legend_height));

            double legend_y_pos = legend_y_start + derived().text_margin_ * spacing;
            if (is_header)
            { // Draw the legend header text for example: "My Plot Legend".
              derived().legend_header_.x(legend_x_start + legend_width / 2.); // / 2. to center in legend box.
              // Might be better to use center_align here because will fail if legend contains symbols in Unicode.
              derived().legend_header_.y(legend_y_pos);
              derived().image_.g(PLOT_LEGEND_TEXT).push_back(new text_element(derived().legend_header_));
              legend_y_pos += 2 * spacing; // Might be 1.5? - useful if many series makes the box too tall.
            }

            g_ptr = &(derived().image_.g(PLOT_LEGEND_POINTS));
            g_element* g_inner_ptr = g_ptr;
            g_inner_ptr = &(derived().image_.g(PLOT_LEGEND_TEXT));

            for(unsigned int i = 0; i < derived().serieses_.size(); ++i)
            { // Show point marker, perhaps line, & text info for each of the data series.

              // cout << "Series " << i << endl;
              // derived().serieses_[i].point_style_ << endl;
              // cout << derived().serieses_[i].line_style_ << endl;

              double legend_x_pos = legend_x_start;
              legend_x_pos += spacing; // space before point marker and/or line & text.
              g_inner_ptr = &(g_ptr->add_g_element());
              // Use both stroke & fill colors from the points' style.
              // Applies to both shape AND line.
              g_inner_ptr->style().stroke_color(derived().serieses_[i].point_style_.stroke_color_);
              g_inner_ptr->style().fill_color(derived().serieses_[i].point_style_.fill_color_);
              g_inner_ptr->style().stroke_width(derived().serieses_[i].line_style_.width_);

              //cout << "g_inner_ptr.style().stroke_color() " << g_inner_ptr->style() << endl;
              plot_point_style& sty = derived().serieses_[i].point_style_;

              if(sty.shape_ != none)
              { // Is some data point marker shape to show in legend box.

                // ellipse is special case to show uncertainty of data point.
                bool was_unc_ellipse = false;
                if (sty.shape_ == unc_ellipse)
                {  // Problem here with unc_ellipse with calculation of a suitable size
                   // and also, more fundamentally, the legend box overwrites the PLOT_DATA_UNC layers,
                  sty.shape_ =  circlet; // so as a hack, use a round or circlet instead.
                  was_unc_ellipse = true; // Note so can restore after showing circle.
                }

                draw_plot_point( // Show a plot point like circlet (==round), square, vertical bar...
                  legend_x_pos,
                  legend_y_pos,
                  *g_inner_ptr,
                  sty, 
                  unc<false>(), unc<false>());  // X and Y position.
                  //was derived().serieses_[i].point_style_, unc(0.), unc(0.));
                legend_x_pos += 1.5 * spacing;
                if (was_unc_ellipse)
                { // Restore (or the data points won't use the unc_ellipse!
                  sty.shape_ = unc_ellipse;
                }
              }

              // Line markers are only really useful for 2-D lines and curves showing functions.
              if (derived().legend_lines_)
              { // Need to draw a short line to show color for that data series.
                  // Line joining points option is true.
                  if (derived().serieses_[i].line_style_.line_on_ || derived().serieses_[i].line_style_.bezier_on_)
                  { // Use stroke color from line style.
                     g_inner_ptr->style().stroke_color(derived().serieses_[i].line_style_.stroke_color_);
                  }
                  else
                  { // Use point stroke color instead.
                    g_inner_ptr->style().stroke_color(derived().serieses_[i].point_style_.stroke_color_); // OK with 1D
                  }
                  //std::cout << "line g_inner_ptr->style().stroke_color() " << g_inner_ptr->style().stroke_color() << std::endl;

                  g_inner_ptr->push_back(new line_element( // Draw horizontal lines with appropriate color.
                    legend_x_pos,
                    legend_y_pos,
                    legend_x_pos + spacing, // Line sample is one char long.
                    legend_y_pos));
                  legend_x_pos += 1.5 * spacing; // Total is short line & a space.
              } // legend_lines_

              // Legend text for each Data Series added to the plot.
              g_inner_ptr = &(derived().image_.g(PLOT_LEGEND_TEXT));
              g_inner_ptr->push_back(new text_element(
                legend_x_pos, // allow space for the marker.
                legend_y_pos,
                derived().serieses_[i].title_, // Text for this data series.
                derived().legend_header_.textstyle(),
                left_align));
              legend_y_pos += 2 * spacing;
            } // for
          } // void draw_legend()

          template <class Derived>
          void axis_plot_frame<Derived>::draw_plot_point(double x, double y, // X and Y values (in SVG coordinates).
            g_element& g_ptr,
            plot_point_style& sty,
            unc<false> ux, unc<false> uy) // Default unc ux = 0. and uy = 0. ?
          { /*! Draw a plot data point marker shape
              whose size and stroke and fill colors are specified in plot_point_style,
              possibly including uncertainty ellipses showing multiples of standard deviation.
            */
            /*
              For 1-D plots, the points do not *need* to be centered on the X-axis,
              and putting them just above, or sitting on, the X-axis is much clearer.
              For 2-D plots, the symbol center should, of course, be centered exactly on x, y.
              circle and ellipse are naturally centered on the point.
              for rectangle x and y half_size offset centers square on the point.
              But symbols are in a rectangular box and the offset is different for x & y
              even assuming that the symbol is centered in the rectangle.
              the vertical and horizontal ticks are deliberately offset above the axes.
              TODO Not sure this is fully resolved.
            */
            int size = sty.size_;
            double half_size = size / 2.;
            //cout << "point style() "<< sty.style() << endl;
            // Whatever shape, text or line, want to use the point style.
            g_ptr.style().stroke_color(sty.stroke_color_);
            g_ptr.style().fill_color(sty.fill_color_);

            //cout << "g_ptr.style() " << g_ptr.style() << endl;

            switch(sty.shape_) // from enum point_shape none, round, square, point, egg
            {
            case none:
              break;

            case circlet:
              g_ptr.circle(x, y, (int)half_size);
              break;

            case point:
              g_ptr.circle(x, y, 1); // Fixed size round.
              break;

            case square:
              g_ptr.rect(x - half_size, y - half_size, size, size);
              break;

            case egg:
              g_ptr.ellipse(x, y, half_size, size * 2.); // Tall thin egg!
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

            case vertical_tick: // Especially neat for 1-D points.
              g_ptr.line(x, y, x , y - size); // tick up from axis.
              break;
            case vertical_line:
              g_ptr.line(x, y + size, x , y - size); // line up & down from axis.
              break;
            case horizontal_tick:
              // horizontal_tick is pretty useless for 1-D because the horizontal line is on the X-axis.
              g_ptr.line(x, y, x + size, y ); // tick right from axis.
              break;
            case horizontal_line:
              g_ptr.line(x, y - size, x + size, y ); // line left & right from axis.
              // horizontal_line is pretty useless for 1-D because the horizontal line is on the X-axis.
              break;
            case symbol:
              g_ptr.text(x, y + half_size, sty.symbols(), sty.style(), center_align, horizontal); // symbol(s), size and center.

              // Unicode symbols that work on most browsers are listed at
              // boost\math\libs\math\doc\sf_and_dist\html4_symbols.qbk,
              // http://www.htmlhelp.com/reference/html40/entities/symbols.html
              // and  http://www.alanwood.net/demos/ent4_frame.html
              // Geometric shapes http://www.unicode.org/charts/PDF/Unicode-3.2/U32-25A0.pdf
              // Misc symbols http://www.unicode.org/charts/PDF/U2600.pdf
              // The Unicode value in decimal 9830 or hex x2666 must be prefixed with & and terminated with ;
              // for example &x2666; for xml
              // and then enveloped with "" to convert to a std::string, for example: "&#x2666;" for diamond.

              break;
            case diamond:
              g_ptr.text(x, y, "&#x2666;", sty.symbols_style_, center_align, horizontal);
              // size / 4. puts bottom tip on the X-axis,
              // size / 2. put center above the X-axis
              // x, y, put on the X-axis - probably what is needed for 2-D plots.
              // diamond, spades, clubs & hearts fill with expected fill_color.
              break;
            case asterisk:
              g_ptr.text(x, y - size / 3., "&#x2217;", sty.symbols_style_, center_align, horizontal);
              // asterisk is black filled.
              // size /3 puts the bottom tip on the X-axis.
              break;
            case lozenge:
              g_ptr.text(x, y - size / 3., "&#x25CA;", sty.symbols_style_, center_align, horizontal);
              // size / 3 to get tip of lozenge just on the X-axis.
              // lozenge seems not to fill?
              break;
            case club:
              g_ptr.text(x, y, "&#x2663;", sty.symbols_style_, center_align, horizontal);
              // x, y, puts club just on the X-axis
              break;
            case spade:
              g_ptr.text(x, y, "&#x2660;", sty.symbols_style_, center_align, horizontal);
              //
              break;
            case heart:
              g_ptr.text(x, y , "&#x2665;", sty.symbols_style_, center_align, horizontal);
              //
              break;
            case cone: // Pointing down triangle.
              {
                bool fill = (sty.fill_color() != blank);
                g_ptr.triangle(x - half_size, y - size, x + half_size, y - size, x, y, fill);
              // Last point puts the bottom tip of the triangle on the X-axis (may not be wanted for 2-D).
              }
              break;
            case triangle: // Pointing up triangle.
               g_ptr.text(x, y , "&#x25B2;", sty.symbols_style_, center_align, horizontal);
                 // Also could use &#x25BC for pointing down triangle, and
                 // &#x25B4 for small up-pointing triangle and &#x25BE for small down triangle.
               break;
             case star:
               g_ptr.text(x, y , "&#x2605;", sty.symbols_style_, center_align, horizontal);
               break;

            case cross: // Not X.
              g_ptr.line(x, y + size, x , y - size); // line up & down from axis,
              g_ptr.line(x, y - size, x + size, y ); // & line left & right from axis.
              // Cross is pretty useless for 1-D because the horizontal line is on the X-axis.
              break;
              // TODO Other point_shapes do nothing yet.
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
          Derived& axis_plot_frame<Derived>::size(unsigned int x, unsigned int y)
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
          unsigned int axis_plot_frame<Derived>::image_x_size() //!< Obselete - deprecated.
          { //! \return SVG image X-axis size as horizontal width (SVG units, default pixels).
            return derived().image_.x_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_size(unsigned int i)
          { //! Set SVG image X-axis size (SVG units, default pixels).
            derived().image_.x_size(i);
            return derived();
          }
          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_size()
          { //! Get SVG image X-axis size as horizontal width (SVG units, default pixels).
            //! \return SVG image X-axis size as horizontal width (SVG units, default pixels).
            return derived().image_.x_size();
          }
          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_x_size(unsigned int i) //!< Obselete - deprecated.
          { //! Set SVG image X-axis size (SVG units, default pixels).
            // Can't this be x_size(unsigned int i)
            derived().image_.x_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::image_y_size() //!< Obselete - deprecated.
          { //! \return SVG image Y-axis size as vertical height (SVG units, default pixels).
            return derived().image_.y_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_y_size(unsigned int i) //!< Obselete - deprecated.
          {//! Set SVG image Y-axis size (SVG units, default pixels).
            derived().image_.y_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::y_size()
          { //! \return SVG image Y-axis size as vertical height (SVG units, default pixels).
            return derived().image_.y_size();
          }

                   template <class Derived>
          Derived& axis_plot_frame<Derived>::y_size(unsigned int i)
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
          { /*! Writes description to the document for header, for example:
              \verbatim
                <desc> My Data </desc>
              \endverbatim
          */
            derived().image_.description(d);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::description()
          { //! \return  description of the document for header as \verbatim <desc> ... </desc> \endverbatim
            return derived().image_.description();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::document_title(const std::string d)
          { //! Write document title to the SVG document for header as \verbatim <title> My Title </title>  \endverbatim
            derived().image_.document_title(d);
            return derived();
          }
          template <class Derived>
          std::string axis_plot_frame<Derived>::document_title()
          { //! \return  document title to the document for header as \verbatim <title> My Title </title> \endverbatim
            return derived().image_.document_title();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::copyright_holder(const std::string d)
          { //! Writes copyright_holder to the document
            //! (for header as \verbatim <!-- SVG Plot Copyright Paul A. Bristow 2007 --> ) \endverbatim
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
          { //! Set if to strip redundant zeros, signs and exponents, for example, reducing "1.2e+000" to "1.2"
            //! This markedly reduces visual clutter, and is the default.
            derived().x_ticks_.strip_e0s_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::y_labels_strip_e0s()
          { //! \return  if to strip redundant zeros, signs and exponents, for example, reducing "1.2e+000" to "1.2"
            return derived().x_ticks_.strip_e0s_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title(const std::string title)
          { /*!
              Set a title for plot.
              The string may include Unicode for greek letter and symbols.
              @b example:
              A title that includes a greek omega and degree symbols:
              \code
                my_plot.title("Plot of &#x3A9; function (&#x00B0;C)");
              \endcode

              Unicode symbols are at http://unicode.org/charts/symbols.html.
            */
            // Plot title.  TODO
            // new text parent code push_back
            // effectively concatenates with any existing title.
            // So clear the existing string first but doesn't work,
            // so need to clear the whole g_element.
            //derived().image_.g(PLOT_TITLE).clear();
            derived().title_info_.text(title);
            derived().title_on_ = true; // Assume display wanted, if bother to set title.
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::title()
          { //! \return  a title for plot, whose string may include Unicode for greek letter and symbols.
            return derived().title_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_size(unsigned int i)
          { //! Sets the font size for the title (svg units, default pixels).
            derived().title_info_.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::title_font_size()
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
          { //! \return  the font stretch for the title.
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
            derived().title_info_.alignment(alignment);
            return derived();
          }

          template <class Derived>
          align_style axis_plot_frame<Derived>::title_font_alignment()
          { //! \return  the alignment for the title.
            return derived().title_info_.alignment();
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
            derived().legend_header_.text(title);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::legend_title()
          { //! \return  the title for the legend.
            return derived().legend_header_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_font_weight(const std::string& weight)
          { //! Set the font weight for the legend title.
            derived().legend_header_.textstyle().font_weight(weight);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_font_weight()
          { //! \return  the font weight for the legend title.
            return derived().legend_header_.textstyle().font_weight();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_font_family(const std::string& family)
          { //! Set the font family for the legend title.
            derived().legend_header_.textstyle().font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_font_family()
          { //! \return  the font family for the legend title.
            return derived().legend_header_.textstyle().font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_title_font_size(unsigned int size)
          { //! Set the font size for the legend title (svg units, default pixels).
            derived().legend_header_.textstyle().font_size(size);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::legend_title_font_size()
          { //! \return  the font size for the legend title (svg units, default pixels).
            return derived().legend_header_.textstyle().font_size();
          }

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
          { //! \return  the position of the legend, \see  boost::svg::legend_places
            return derived().legend_place_;
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_outside()
          { //! \return  if the legend should be outside the plot area.
            return derived().outside_legend_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_header_font_size(int size)
          { //! Set legend header font size (svg units, default pixels).
            derived().legend_header_.textstyle().font_size(size);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::legend_header_font_size()
          { //! \return  legend header font size (svg units, default pixels).
            return derived().legend_header_.textstyle().font_size();
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
          { //! Set the color for the plot window background.
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
            derived().plot_window_border_.width_ = w;
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
            derived().image_border_.width_ = w;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::image_border_width()
          { //! \return  the svg image border width (svg units, default pixels).
            return derived().image_border_.width_;
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
          Derived& axis_plot_frame<Derived>::x_label_font_size(unsigned int i)
          { //! Set X axis label font size (svg units, default pixels).
            derived().x_label_info_.textstyle().font_size(i);
            // Also duplicated at
            // derived().x_axis_label_style_.font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_label_font_size()
          { //! \return  X axis label font size (svg units, default pixels).
            return derived().x_label_info_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_font_size(unsigned int i)
          { //! Set X tick value label font size (svg units, default pixels).
            derived().x_value_value.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_value_font_size()
          { //! \return  X tick value label font size (svg units, default pixels).
            return derived().x_value_value.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_font_family(const std::string& family)
          { //! Set X tick value label font family.
            derived().x_label_info_.textstyle().font_family(family);
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
          Derived& axis_plot_frame<Derived>::x_ticks_values_font_size(unsigned int i)
          { //! Set X ticks value label font size (svg units, default pixels).
            //derived().x_ticks_.value_label_style_.font_size(i);
            derived().x_value_label_info_.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_ticks_values_font_size()
          { //! \return  X ticks value label font size (svg units, default pixels).
            // return derived().x_ticks_.value_label_style_.font_size();
                   return     derived().x_value_label_info_.textstyle().font_size();

            //return derived().x_ticks_.value_label_info_.font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_values_font_family(const std::string& family)
          { //! Set X ticks value label font family.
            //derived().x_ticks_.value_label_style_.font_family(family); // is effect same as:
            derived().x_value_label_info_.textstyle().font_family(family);
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



          ////////////////////////////////



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
          { //! Set the background fill color of the legend box.
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
          { //! \return  the color of the Y-axis line.
            return derived().image_.g(PLOT_Y_AXIS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_color(const svg_color& col)
          { //! Set the color of X-axis label (including any units).
            // add fill as well PAB Oct 07
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
          { //! \return  the width (boldness) of X-axis label (including any units).
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
          Derived& axis_plot_frame<Derived>::x_values_font_size(unsigned int i)
          { //! Set font size of data point X values near data points markers.
            derived().x_values_style_.values_text_style_.font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_values_font_size()
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
            return derived().x_values_style_.value_precision_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_ioflags(std::ios_base::fmtflags f)
          { //! Set iostream format flags of data point X values near data points markers.
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
          Derived& axis_plot_frame<Derived>::x_num_minor_ticks(unsigned int num)
          { //! Set number of X-axis minor ticks between major ticks.
            derived().x_ticks_.num_minor_ticks_ = num;
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_num_minor_ticks()
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
          }

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
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_steps()
          { //! \return  autoscale to set ticks in steps.
           return derived().x_steps_;
          }

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
        Derived& axis_plot_frame<Derived>::limit_color(const svg_color& col)
        { //! Set the color for 'at limit' point stroke color.
          // Need to set the series
          derived().image_.g(detail::PLOT_LIMIT_POINTS).style().stroke_color(col);
          // derived().serieses_[0].limit_point_color(col); // Would require to add some data first!
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::limit_color()
        { //! \return  the color for the 'at limit' point stroke color.
          return derived().image_.g(detail::PLOT_LIMIT_POINTS).style().stroke_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::limit_fill_color(const svg_color& col)
        { //! Set the color for 'at limit' point fill color.
          derived().image_.g(detail::PLOT_LIMIT_POINTS).style().fill_on(true);
          derived().image_.g(detail::PLOT_LIMIT_POINTS).style().fill_color(col);
          //derived().serieses_[0].limit_point_style_.fill_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::limit_fill_color()
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
