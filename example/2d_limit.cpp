/*! \file 2d_limit.cpp
  \brief Simple 2D plot show 1/x function values at limit.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009, 2021
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_2d_plot.hpp>
// boost::svg::svg_2d_plot
#include <limits>
// infinity
#include <map>
//using std::map;

double f(double x)
{ // Function to plot.
  return 1. / x;
}

int main()
{
  using namespace boost::svg; // For SVG named colors.
  std::map<double, double> data1;

  const double interval = 0.5;
  for(double i = -10; i <= 10.; i += interval)
  {
    data1[i] = f(i);
  }

  svg_2d_plot my_plot;

  // Image size & ranges settings.
  my_plot.size(500, 350) // SVG image in pixels.
         .x_range(-10.5, 10.5) // Offset by 0.5 so that +10 and -10 markers are visible.
         .y_range(-1.1, 1.1); // Offset by 1 so that +10 and -10 markers are visible.

  // Text settings.
  my_plot.title("Plot of 1 / x")
    .x_label("X Axis Units")
    .y_label("F(x)")
    .y_major_labels_side(-1) // Left.
    .plot_window_on(true);

  // X-axis settings.
  my_plot.x_major_interval(2)
    .x_major_tick_length(14)
    .x_major_tick_width(1)
    .x_minor_tick_length(7)
    .x_minor_tick_width(1)
    .x_num_minor_ticks(1)
    .x_major_labels_side(1) // Top of X-axis line (but zero collides with vertical x == 0 line).

  // Y-axis settings.
         .y_major_interval(1)
         .y_num_minor_ticks(4);

  // Legend-box settings.
  my_plot.legend_title_font_size(15);

  //my_plot.minus_inf_limit_color(red).plus_inf_limit_color(green); // Limit value at x = 0 is +infinity.
  // TODO - the 'at-limit' infinity point cone pointing-down shows as default color pink, not the changed color(s).

  my_plot.plot(data1, "1 / x").shape(square).size (5).line_on(false);

  my_plot.write("./2d_limit.svg");

  // Sets colors correctly, but not used.
  // std::cout << "" << my_plot.plus_inf_limit_color() << std::endl; // RGB(0,128,0)
   //std::cout << "" << my_plot.minus_inf_limit_color() << std::endl; // RGB(255,0,0)

  return 0;
} // int main()

