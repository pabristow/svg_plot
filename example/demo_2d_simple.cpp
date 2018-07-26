/*! \file demo_2d_simple.cpp
    \brief Demonstration of some simple 2D plot features.
    \details Uses some simple math functions to generate curves.
    This demonstrates plotting some simple math functions with most of the 2-D defaults,
    just changing a few typical details.
    The detailed output shows the plot settings for each plot.
    See default_2d_plot.cpp for using \b all defaults.
    See also demo_2d_plot.cpp for use of some of the very many options.
    \date Feb 2009
    \author Jacob Voytko and Paul A. Bristow 
*/

// demo_2d_simple.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2012, 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp> // For plot package.
  using boost::svg::svg_2d_plot;

#include <boost/svg_plot/show_2d_settings.hpp>
// Only needed for showing which settings in use.
// Use this when the plot doesn't look as you want it to be.

#include <boost/quan/unc_init.hpp>  // for setUncDefaults

#include <iostream>
  using std::cout;
  using std::endl;
  using std::boolalpha;
#include <map>
  using std::map;
#include <cmath>
  using std::sqrt;
#include <string>
  using std::string;

// using namespace boost::svg;
// may be *very convenient* if using any SVG named colors,
// to avoid writing
  using boost::svg::red;
  using boost::svg::yellow;
  using boost::svg::orange;
  using boost::svg::blue;
  // for every color used.
  
  // and other enum options used:
  using boost::svg::square;
  using boost::svg::circlet;

// Some example of functions to plot.
double f(double x)
{
  return sqrt(x);
}

double g(double x)
{
  return -2 + x*x;
}

double h(double x)
{
  return -1 + 2 * x;
}

int main()
{
  std::cout << "Demonstration of a simple 2D plot showing data points with markers and with lines joining points." << std::endl;
  try
  {
    // Some containers for (sorted) sample data.
    map<double, double> data1;
    map<double, double> data2;
    map<double, double> data3;

    for(double i = -5; i <= 10.; i += 1.)
    { // Several data points for each function.
      data1[i] = f(i);
      data2[i] = g(i);
      data3[i] = h(i);
      // List if desired:
      // cout << i << ' '<< data1[i] << ' ' << data2[i] << ' '<< data3[i] << endl;
    }

    setUncDefaults(std::cout);  // Set the uncertain class defaults.
    svg_2d_plot my_plot; // Class to hold the plot settings.
    // Uses most defaults, but scale settings are usually sensible.

    // Add the data series to the plot:
    my_plot.title("demo_2d_simple");
    cout << " my_plot.title() " << my_plot.title() << endl;
    my_plot.x_label("X-axis").y_label("Y-axis"); // Note chaining, the easy way to add lots of options.

    std::string s = my_plot.title(); 

    my_plot.plot(data1, "Sqrt(x)").fill_color(red);
    my_plot.plot(data2, "-2 + x^2").fill_color(orange).size(5);
    my_plot.plot(data3, "-1 + 2x").fill_color(yellow).bezier_on(true).line_color(blue).shape(square);
    cout << " my_plot.title() " << my_plot.title() << endl;

    my_plot.write("./demo_2d_simple.svg");
    cout << " my_plot.title() " << my_plot.title() << endl;

    show_2d_plot_settings(my_plot);
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n  " << e.what() << std::endl;
  }
  return 0;
} // int main()


/*

Output:

Demonstration of a simple 2D plot showing data points with markers and with lines joining points.
my_plot.title() demo_2d_simple
my_plot.title() demo_2d_simple
font size 12
x value_font_space 24
x left-right border_margin = 24
plot_left before margin 30
plot_left after margin 54
plot_right before margin 498
plot_right after margin 474

y value_font_space 24
y top-botton border_margin = 24
plot_top before margin 54
plot_top after margin 54
plot_bottom before margin 474
plot_bottom after margin 474
my_plot.title()


SVG 2-D plot settings
(default units pixels)========================
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
image x_size 500
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
plot_window_x 84.2, 474
plot_window_x_left 84.2
plot_window_x_right 474
plot_window_y 71, 323
plot_window_y_top 71
plot_window_y_bottom 323
title_on true
title ""
title_color blank
title_font_alignment 2
title_font_decoration
title_font_family Lucida Sans Unicode
title_font_rotation 0
title_font_size 18
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family Lucida Sans Unicode
x_values_precision 3
x_values_ioflags 200 iosFormatFlags (0x200) dec.
y_values_precision 3
y_values_font_size() 3
y_values_ioflags 200 iosFormatFlags (0x200) dec.
y_values_color blank
y_values_font_family() Lucida Sans Unicode
y_values_font_size() 12
x_max 10
x_min -10
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label "X-axis"
x_label_color blank
x_label_font_family Lucida Sans Unicode
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on false
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
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
x_range() -10, 10
x_num_minor_ticks 4
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis bottom
y_axis_position y_axis_position intersects X-axis (X range includes zero)
x_axis_position x_axis_position intersects Y axis (Y range includes zero)
x_plusminus_on false
x_plusminus_color blank
x_df_on false
x_df_color RGB(0,0,0)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on "true"
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Lucida Sans Unicode
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on false
y_major_grid_color RGB(200,220,255)
y_major_grid_width 1
y_major_interval 2
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
y_range() -10, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis left
y_max 10
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color blank
x_addlimits_on false
x_addlimits_color RGB(0,0,0)
y_df_on false
y_df_color RGB(0,0,0)
y_prefix ""
y_separator ""
y_suffix ""
confidence alpha 0.05
data lines width 2
Press any key to continue . . .



*/
