/*! \file demo_2d_uncertainty.cpp
    \brief Demonstration of some 2D plot features.
    \details including showing values with uncertainty information as 'plus minus', confidence intervals and degrees of freedom estimates.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_2d_uncertainty_1

/*`First we need some includes to use Boost.SVG_Plot, and C++ Standard Library:
*/

#include <boost/svg_plot/svg_2d_plot.hpp>
// using namespace boost::svg;

#include <boost/svg_plot/show_2d_settings.hpp> // Only needed for showing ALL the many settings used.
//using boost::svg::show_2d_plot_settings;
//void boost::svg::show_2d_plot_settings(svg_2d_plot&);

#include <boost/svg_plot/detail/pair.hpp>
// using boost::svg::detail::pair; operator<<

#include <boost/quan/unc.hpp>
//#include <boost/quan/unc_init.hpp>

#include <boost/quan/meas.hpp>

#include <algorithm>
//  using std::copy;
//#include <functional>

#include <map>
  //using std::map;
  //using std::multimap;

#include <utility>
  //using std::pair;
  //using std::make_pair;

#include <vector>
//  using std::vector;

#include <cmath>
//   using std::sqrt;

#include <iostream>
   //using std::cout;
   //using std::endl;
   //using std::scientific;
   //using std::hex;
   //using std::ios;
   //using std::boolalpha;
#include <iomanip>
// Using std::setw;
// using std::setprecision;

#include <iterator>
//   using std::ostream_iterator;

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
  using std::vector;
  using std::map;
  using std::multimap;
  using std::cout;
  using std::endl;
  using std::scientific;
  using std::hex;
  using std::ios;
  using std::boolalpha;
  using std::sqrt;

  using namespace boost::svg;
  using boost::svg::detail::operator<<;


  try
  {
//[demo_2d_uncertainty_2

/*`STL map is used as the container for our two data-series,
and pairs of values and their uncertainty information
(approximately standard deviation and degrees of freedom)
are inserted using push_back. Since this is a 2-D plot
the order of data values is important.
*/
  typedef unc<false> uncun; // Uncertain Uncorrelated (the normal case).

  using boost::svg::detail::operator<<;
  using std::ostream_iterator;

  setUncDefaults(std::cout);  // Initialize for uncertain types.

  // Create pair for fundamental type double that is implicitly exact with no uncertainty information.
  pair<double, double> double_pair; // double X and Y pair.
  double_pair = make_pair(double(-2.234), double(-8.76)); // Construct and then echo the data values thus:
  std::cout << double_pair.first << ", " << double_pair.second << std::endl; //  make_pair(double(-2.234), double(-8.76)) = -2.234, -8.76
  // But it is more convenient to use a specialization of operator<< for `std::pair` to show both:
  std::cout << "make_pair(double(-2.234), double(-8.76)) = " << double_pair << std::endl;
  // make_pair(double(-2.234), double(-8.76)) = -2.234, -8.76

  // Or, more informatively, use an uncertain type `uncun` that holds explicit uncertainty information
  // (standard-deviation, degrees of freedom and so can compute confidence internals).

  uncun ux(1.03, 0.56F, 7); // For an X-value.
  // Using the  `uncun operator<<` provided we can output all the details of the uncertain value.
  std::cout << scientific << plusminus << addlimits << adddegfree << std::setw(20)  << std::left
    << "ux = " << ux << std::endl; // 1.2 +/-0.56 <0.82, 1.64> (7)

  uncun uy(3.45, 0.67F, 9); // For a Y-value.
  std::cout << "uy = " << uy << std::endl; // 3.5 +/-0.67 <3.01, 3.89> (9)

  // Now we make a pair of X and Y uncertain values:
  pair<uncun, uncun > mp1 = make_pair(ux, uy); // X & Y pair of two uncertain values.
  // Echo both X and Y-values and add all the uncertainty information, standard deviation and degrees of freedom :
  std::cout << "mp1 = "<< mp1 << std::endl; // 1.2 +/-0.56 <0.82, 1.64> (7), 3.5 +/-0.67 <3.01, 3.89> (9)

  std::map<uncun, uncun > data1; // Container for X & Y pairs of data-point values.
  data1.insert(mp1); // Insert 1st pair of X & Y.
  data1.insert(make_pair(uncun(3.9, 0.01F, 8), uncun(1.1, 0.1F, 18))); // and add another X&Y-pair
  data1.insert(make_pair(uncun(-2.234, 0.3F, 7), uncun(-8.76, 0.9F, 9))); // and a third pair.

 /*
`Make very sure you don't forget either uncun(...) like this
`data1.insert(make_pair((-2.234, 0.12F, 7),(-8.76, 0.56F, 9)));`
because, by the bizarre operation of the comma operator, the result will be an integer!
So you will astonished to find that the values will be the *pair of degrees of freedom, (7, 9)*
and the other parts of uncun will be undefined!

Echo the values input, correctly rounded using the uncertainy and degrees of freedom:
  */
  std::cout << data1.size() << " XY data pairs:" << std::endl;
  std::copy(data1.begin(), data1.end(), ostream_iterator<pair<uncun, uncun> >(std::cout, "\n"));
  std::cout << std::endl;

  /*
  3 XY data pairs:
   -2.23 +/-0.030 <-2.26, -2.21> (7), -8.8 +/-0.90 <-9.35, -8.17> (9)
   1.2 +/-0.56 <0.82, 1.64> (7), 3.5 +/-0.67 <3.01, 3.89> (9)
   4.1 +/-0.40 <3.82, 4.38> (8), 3.1 +/-0.30 <2.96, 3.24> (18)
  */

  svg_2d_plot my_plot;  // Construct an empty plot.

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
  //my_plot.autoscale_plusminus(1.5); // default is 3.
  // my_plot.confidence(0.01);  // Change from default 0.05 to 0.01 for 99% confidence.

  /*`Use data set `data` to autoscale. (You can use a different data set to scale from the one you chose to plot).
  */
  //my_plot.xy_autoscale(data1);

  my_plot
    // X values settings:
    .x_label("times (sec)")
    .x_range(-3, +10)
    .x_values_on(true) // Show X-values next to each point.

     //! \note Essential use of Unicode space &\#x00A0; in all strings - ANSI space has no effect!
  //  .x_decor("t ", ", ", "sec") // Keep all on one line using a separator NOT starting with a newline.
    .x_decor("t ", "\n_", "sec") // Split X and Y onto two lines because X separator *does* start with newline.
    .x_values_rotation(uphill)
    .x_values_font_size(10) // Bigger than default.
    .x_values_font_family("Times New Roman") // Serif font to show difference from sans serif ued for Y value-labels.
    .x_major_grid_on(true)
    .y_major_grid_on(true)

    // Y values settings:
    .y_label("distance (km)")
    .y_range(-10., +10.) // But may be over-written by x and or  y-autoscale below.
    .y_values_on(true) // Show Y values next to each point.
    .y_values_rotation(uphill)
    .y_values_font_family("Arial") // Sans serif different from X-values font just to show effect.
    .y_values_font_size(8) // Smaller than default.
    .y_decor("&#x00A0;&#x00A0;&#x00A0; d &#x00A0;", "&#x00A0;", "&#x00A0;km")
//    .y_decor("&#x00A0;&#x00A0; time = ", "&#x00A0;", "&#x00A0;sec")
     // Note: a few padding spaces are used to get Y-value-labels to lie more nearly under X-value-labels.
     // This is only necessary when value-labels are not horizontal.
     // y_prefix "&#x00A0;&#x00A0;&#x00A0;"   3 spaces.
     // y_separator "&#x00A0;time ="  space before word time.
     // y_suffix "&#x00A0;sec" space before word sec.

    .y_plusminus_on(true) // Show plus/minus +/- uncertainty with data-point value-labels, for example "2.1 +/- 0.001"
    .y_plusminus_color(red) // Show plus/minus +/- uncertainty in red.

    .y_addlimits_on(true)  // Show plus/minus +/- confidence limits for data-point value-labels.
    .y_addlimits_color(darkgreen) // Show +/- in darkgreen, for example: "+/- 0.03".

    .y_df_on(true) // Show degrees of freedom (usually observations -1) for data-points.
    .y_df_color(green) // Show degrees of freedom in green, for examples: "11").

     .xy_values_on(true) // Show both X-values and Y-values next to each point.
     // .xy_autoscale(data1) // may result in value-labels running off the plot and image.

  /*`The default uncertainty ellipse colors (that apply to both X and Y axes)
  can be changed thus:
  */
    .one_sd_color(lightblue) // Color of ellipse for one standard deviation (about 66% probability).
    .two_sd_color(svg_color(200, 230, 255)) // Color of ellipse for two standard deviation (~95%).
    .three_sd_color(svg_color(230, 240, 255)) // Color of ellipse for two standard deviation (~99%).
   ; // my_plot

//  my_plot.plot(data1, "data1").shape(unc_ellipse).fill_color(lightyellow).stroke_color(magenta);
  my_plot.plot(data1, "data1").shape(unc_ellipse).fill_color(blue).stroke_color(magenta);
  // TODO the uncertainty ellipses are now not showing anything useful for 2D but OK for 1D??? :-(

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
  ux = 1.2
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
