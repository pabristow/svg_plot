/*! \file 2d_limit.cpp

  \brief Simple 2D plot show 1/x function values at limit.

  \author Jacob Voytko and Paul A. Bristow

  \date Jul 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009
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
  using namespace boost::svg; // For colors.
  std::map<double, double> data1;

  double inter = 0.5;
  for(double i = -10; i <= 10.; i += inter)
  {
    data1[i] = f(i);
  }

  svg_2d_plot my_plot;

  // Image size & ranges settings.
  my_plot.size(500, 350)
         .x_range(-10.5, 10.5)
         .y_range(-1.1, 1.1);

  // Text settings.
  my_plot.title("Plot of 1 / x")
         .x_label("X Axis Units")
         .y_label("F(x)")
         .y_major_labels_side(1) // Top
         .y_major_grid_on(true)
         .plot_window_on(true)
         .x_major_labels_side(1); // Top

  // X-axis settings.
  my_plot.x_major_interval(2)
         .x_major_tick_length(14)
         .x_major_tick_width(1)
         .x_minor_tick_length(7)
         .x_minor_tick_width(1)
         .x_num_minor_ticks(3)

  // Y-axis settings.
         .y_major_interval(20)
         .y_num_minor_ticks(4);

  // Legend-box settings.
  my_plot.legend_title_font_size(15);

  my_plot.minus_inf_limit_color(lime).plus_inf_limit_color(pink); // Limit value at x = 0 is +infinity.

  my_plot.plot(data1, "1 / x").shape(square).size (5).line_on(false);

  my_plot.write("./2d_limit.svg");

  std::cout << "" << my_plot.plus_inf_limit_color() << std::endl;
  std::cout << "" << my_plot.plus_inf_limit_color() << std::endl;

  return 0;
} // int main()

