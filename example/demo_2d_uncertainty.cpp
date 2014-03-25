/*! \file demo_2d_uncertainty.cpp
    \brief Demonstration of some 2D plot features.
    \details including showing values with uncertainty information as 'plus minus' and degrees of freedom estimates.
    \author Jacob Voytko and Paul A. Bristow
    \date Mar 2009, 2012
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4100) // unreferenced formal parameter
#endif

//[demo_2d_uncertainty_1

/*`First we need some includes to use Boost.Plot and C++ Standard Library:
*/

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <boost/svg_plot/show_2d_settings.hpp> // Only needed for showing which settings in use.
//using boost::svg::show_2d_plot_settings;
//void boost::svg::show_2d_plot_settings(svg_2d_plot&);

#include <boost/svg_plot/detail/pair.hpp>
// using boost::svg::detail::pair; operator<<

#include <boost/quan/unc.hpp>
//#include <boost/quan/unc_init.hpp>

#include <boost/quan/meas.hpp>

//#include <algorithm>
//#include <functional>

#include <map>
  using std::map;
  using std::multimap;

#include <utility>
  using std::pair;
  using std::make_pair;

#include <vector>
   using std::vector;

#include <cmath>
   using std::sqrt;

#include <iostream>
   using std::cout;
   using std::endl;
   using std::scientific;
   using std::hex;
   using std::ios;
   using std::boolalpha;

#include <iterator>
   using std::ostream_iterator;
//] [/demo_2d_uncertainty_1]

  enum side
  { //! \enum side Where axis labels go.
    left_side = -1,
    on_axis = 0,
    right_side = +1,
    bottom_side = -1,
    top_side = +1,
  };

int main()
{
  using std::pair;
  using std::make_pair;

  using boost::svg::detail::operator<<;

  setUncDefaults(std::cout);

  try
  {
//[demo_2d_uncertainty_2

/*`STL map is used as the container for our two data series,
and pairs of values and their uncertainty information
(approximately standard deviation and degrees of freedom)
are inserted using push_back. Since this is a 2-D plot
the order of data values is important.
*/
  typedef unc<false> uncun; // Uncertain Uncorrelated (the normal case).

  using boost::svg::detail::operator<<;

  // Check pair for double.
  pair<double, double> double_pair; // double X and Y
  double_pair = make_pair(double(-2.234), double(-8.76));
  cout << "make_pair(double(-2.234), double(-8.76)) = " << double_pair << endl;

  uncun u1(1.23, 0.56F, 7); // For an X value.
  cout << "u1 = " << u1 << endl; // u1 = 1.23+-0.056 (7)
  uncun u2(3.45, 0.67F, 9); // For a Y value.
  pair<uncun, uncun > mp1 = make_pair(u1, u2); // XY pair of values.
  cout << mp1 << endl; // 1.23+-0.056 (7), 2.345+-0.067 (9)

  map<uncun, uncun > data1; // Container for XY pair of points.
  data1.insert(mp1); // u1, u2 = 1.23+-0.056 (7), 2.345+-0.067 (9)
  data1.insert(make_pair(uncun(4.1, 0.4F, 7), uncun(3.1, 0.3F, 18))); //
  data1.insert(make_pair(uncun(-2.234, 0.03F, 7), uncun(-8.76, 0.9F, 9)));

 /*
`Make very sure you don't forget either uncun(...) like this
`data1.insert(make_pair((-2.234, 0.12F, 7),(-8.76, 0.56F, 9)));`
because, by the bizarre operation of the comma operator, the result will be an integer!
So you will astonished to find that the values will be the *pair of degrees of freedom, (7, 9)*
and the other parts of uncun will be undefined!

Echo the values input:
  */
  cout << data1.size() << " XY data pairs:" << endl;
  std::copy(data1.begin(), data1.end(), ostream_iterator<pair<uncun, uncun > >(cout, "\n"));
  cout << endl;

  svg_2d_plot my_plot;
  /*`If you can be confident that the data set(s) only contains normal, valid data,
  so none are 'at limits' - too big or too small to be meaningful, infinite or NaN (NotANumber),
  then these checks can be skipped (for speed).
  An instrument or operator input might be known to provide only normal data.
  For this example, we know this is true, so override the default autoscale_check_limits(true).
  */
  my_plot.autoscale_check_limits(false);
  /*`The default is autoscale_plusminus(3.) so that confidence ellipses
  at 1, 2 and 3 (uncertainty nominally standard deviations)
  are all within the plot window,
  but if you are less interested in seeing the 2 and 3 ellipses,
  you could risk the outer edges spilling over the borders
  by reducing autoscale_plusminus, for example, to 1.5, down to zero.
  */
  my_plot.autoscale_plusminus(1.5); // default is 3.
  my_plot.confidence(0.01);  // Change from default 0.05 to 0.01 for 99% confidence.

  /*`Use data set `data` to autoscale (you can use a different data set to scale from the one you chose to plot).
  */
  my_plot.xy_autoscale(data1);

  my_plot
    .x_label("times (sec)")
    .x_range(-3, +10)
    .xy_values_on(true) // Show X and Y values next to each point.

     //! \note Essential use of Unicode space in all strings - ANSI space has no effect!
    //.x_decor("", ",&#x00A0;") // Keep all on one line using separator NOT starting with a newline.
    .x_decor("","\n") // Split onto two lines because X separator does start with newline.
    .y_decor("&#x00A0;&#x00A0;&#x00A0;", "&#x00A0;time =", "&#x00A0;sec")
     // Note: a few padding spaces are used to get Y values to lie more nearly under X values.
     // This is only necessary when label are not horizontal.
    .x_values_rotation(slopeup)
    .x_values_font_size(16)
    .x_plusminus_on(true)
    .x_plusminus_color(cyan)

    .x_addlimits_on(true)
    .x_addlimits_color(purple)

    .x_df_on(true)
    .x_df_color(magenta)
    .x_values_font_family("Times New Roman")

    .y_label("distance (km)")
    .y_range(-10., +10.)
    .y_values_rotation(uphill)
    .y_values_font_family("Arial") // Different from X just to show effect.
    .y_plusminus_on(true)
    .y_plusminus_color(red)

    .y_addlimits_on(true)
    .y_addlimits_color(darkgreen)

    .y_df_on(true)
    .y_df_color(green)

  /*`The default uncertainty ellipse colors (that apply to both X and Y axes)
  can be changed thus:
  */
    .one_sd_color(lightblue)
    .two_sd_color(svg_color(200, 230, 255))
    .three_sd_color(svg_color(230, 240, 255))
   ; // my_plot

  my_plot.plot(data1, "data1").shape(unc_ellipse);

  my_plot.write("./demo_2d_uncertainty");

  show_2d_plot_settings(my_plot);

 //] [/demo_2d_uncertainty_2]

  }
  catch (const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

Output :

   ------ Build started: Project: demo_2d_uncertainty, Configuration: Debug Win32 ------
  unc.cpp
  unc_print.cpp
  unc_read.cpp
  demo_2d_uncertainty.cpp
  Generating Code...
  demo_2d_uncertainty.vcxproj -> J:\Cpp\SVG\Debug\demo_2d_uncertainty.exe
  make_pair(double(-2.234), double(-8.76)) = -2.234, -8.76
   make_pair(double(-2.234), double(-8.76)) = -2.234, -8.76
  u1 = 1.2
  1.2, 3.5
  3 XY data pairs:
  -2.23, -8.8
  1.2, 3.5
  4.10, 3.100

  No limits checks: x_min = -2.234, x_max = 4.1, y_min = -8.76, y_max = 3.45


  axes_on true
  background_border_width 2
  background_border_color RGB(255,255,0)
  background_color RGB(255,255,255)
  image_border_margin() 3
  image_border_width() 2
  coord_precision 3
  copyright_date
  copyright_holder
  description
  document_title ""
  x_size 500
  image y_size 400
  image_filename
  legend_on false
  legend_place 2
  legend_top_left -1, -1, legend_bottom_right -1, -1
  legend_background_color blank
  legend_border_color RGB(255,255,0)
  legend_color blank
  legend_title ""
  legend_title_font_size 14
  legend_font_weight
  legend_width 0
  legend_lines true
  limit points stroke color RGB(119,136,153)
  limit points fill color RGB(250,235,215)
  license_on false
  license_reproduction permits
  license_distribution permits
  license_attribution requires
  license_commercialuse permits
  plot_background_color RGB(255,255,255)
  plot_border_color RGB(119,136,153)
  plot_border_width 2
  plot_window_on true
  plot_window_x 92.6, 474
  plot_window_x_left 92.6
  plot_window_x_right 474
  plot_window_y 8, 341
  plot_window_y_top 8
  plot_window_y_bottom 341
  title_on false
  title ""
  title_color blank
  title_font_alignment 2
  title_font_decoration
  title_font_family Verdana
  title_font_rotation 0
  title_font_size 18
  title_font_stretch
  title_font_style
  title_font_weight
  x_values_on false
  x_values_font_size 16
  x_values_font_family Times New Roman
  x_values_precision 3
  x_values_ioflags 200 IOS format flags (0x200) dec.
  y_values_precision 3
  y_values_font_size() 3
  y_values_ioflags 200 IOS format flags (0x200) dec.
  y_values_color blank
  y_values_font_family() Arial
  y_values_font_size() 12
  x_max 5
  x_min -3
  x_autoscale true
  y_autoscale true
  xy_autoscale true
  x_autoscale_check_limits false
  x_axis_on true
  x_axis_color() RGB(0,0,0)
  x_axis_label_color blank
  x_values_color blank
  x_axis_width 1
  x_label_on true
  x_label "times (sec)"
  x_label_color blank
  x_label_font_family Verdana
  x_label_font_size 14
  x_label_units
  x_label_units_on false
  x_major_labels_side left
  x_major_label_rotation 0
  x_major_grid_color RGB(200,220,255)
  x_major_grid_on false
  x_major_grid_width 1
  x_major_interval 1
  x_major_tick 1
  x_major_tick_color RGB(0,0,0)
  x_major_tick_length 5
  x_major_tick_width 2
  x_minor_interval 0
  x_minor_tick_color RGB(0,0,0)
  x_minor_tick_length 2
  x_minor_tick_width 1
  x_minor_grid_on false
  x_minor_grid_color RGB(200,220,255)
  x_minor_grid_width 0.5
  x_range() -3, 5
  x_num_minor_ticks 4
  x_ticks_down_on true
  x_ticks_up_on false
  x_ticks_on_window_or_axis bottom
  y_axis_position y_axis_position intersects X axis (X range includes zero)
  x_axis_position x_axis_position intersects Y axis (Y range includes zero)
  x_plusminus_on true
  x_plusminus_color RGB(0,255,255)
  x_df_on true
  x_df_color RGB(255,0,255)
  x_prefix
  x_separator
  x_suffix
  xy_values_on true
  y_label_on "true"
  y_label_axis distance (km)
  y_axis_color RGB(0,0,0)
  y_axis_label_color blank
  y_axis_on true
  axes_on true
  y_axis_value_color RGB(0,0,0)
  y_axis_width 1
  y_label distance (km)
  y_label_color blank
  y_label_font_family Verdana
  y_label_font_size 14
  y_label_on true
  y_label_units
  y_label_units_on false
  y_label_width 0
  y_major_grid_on false
  y_major_grid_color RGB(200,220,255)
  y_major_grid_width 1
  y_major_interval 2.5
  y_major_labels_side bottom
  y_major_label_rotation 0
  y_major_tick_color RGB(0,0,0)
  y_major_tick_length  5
  y_major_tick_width  2
  y_minor_grid_on false
  y_minor_grid_color  RGB(200,220,255)
  y_minor_grid_width 0.5
  y_minor_interval 0
  y_minor_tick_color RGB(0,0,0)
  y_minor_tick_length 2
  y_minor_tick_width 1
  y_range() -10, 5
  y_num_minor_ticks
  y_ticks_left_on true
  y_ticks_right_on false
  y_ticks_on_window_or_axis left
  y_max 5
  y_min -10
  y_values_on false
  y_plusminus_on true
  y_plusminus_color RGB(255,0,0)
  y_df_on true
  y_df_color RGB(0,128,0)
  y_prefix "&#x00A0;&#x00A0;&#x00A0;"
  y_separator "time = "
  y_suffix "&#x00A0;sec"
  data lines width 2
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========







*/
