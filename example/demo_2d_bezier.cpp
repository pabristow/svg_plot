/*! \file demo_2d_bezier.cpp
\brief Simple 2D plot of trig functions,
contrived to show optionally
  using Bezier for curves,
  data points markers,
  lines joining data points,
and 
  legend identifying point marks and/or lines.

  A few color, widths and shapes options are demonstrated,
  to produce a somewhat lurid effect.

  Demonstrates that the legend marks line only shown if a dataset is plotted with a line joining points,
  and a data-point value marker is only shown if marker shapes are used,
  and not if the shape == none.
*/

// Copyright Paul A. Bristow 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp>
// using namespace boost::svg;
// Very convenient to allow easy access to colors and other items.

#include <map>
// using std::map;
#include <cmath>
//using std::sin;
//using std::cos;
//using std::tan;

double f(double x)
{
  return std::sin(x);
}

double g(double x)
{
  return std::cos(x);
}

double h(double x)
{
  return std::tan(x);
}

int main()
{

  using namespace boost::svg; // Very convenient to allow easy access to colors and many other items.

  std::map<double, double> sin_data, cos_data, tan_data, sincos_data;

  double step = 3.14159265 / 8.;  // Interval between function data points.

  // Generate some trigonmetric data to plot.
  for(double i = 0; i <= 10.; i += step)
  {
    sin_data[i] = f(i); // sin
    cos_data[i] = g(i); // cos
    tan_data[i] = h(i); // tan
    sincos_data[i] = std::sin(g(i)); // sincos
 } // for i

  svg_2d_plot my_plot; // Data structure to hold the plot.

  // Size/scale settings.
  my_plot.size(700, 500) // SVG image size (pixel).
    .x_range(-0.5, 10.5) // Range of x and y axes,  
    .y_range(-1.1, 1.1); // chosen to ensure that the maxima and minimax 
                         // are not just on the edge of the plot window.

  // Text settings.
  my_plot.title("Plot of sin, cos &#x26; tan and sincos functions")
    // Note: for ampersand must use Unicode &#x26; because it is a reserved symbol in SVG XML.
    // Search engines will provide Unicodes by querying "Unicode ampersand"
    // at sites like https://unicode.org/, 
    // http://www.fileformat.info/info/unicode/char/0026/index.htm and others.
    .title_font_size(28)
    .x_label("x Axis Units")
    .y_major_labels_side(left)
    .y_major_grid_on(true);

  // Layout options:
  my_plot.legend_on(true) // Want a legend box.
    .plot_window_on(true) // want a plot window with axis labels etc outside.
    .x_label_on(true) // Label X-axis ticks with their values.
    //.y_label_on(false)  // false is default.
    ;

  // Plot color settings.
  // (Note use of chaining to add settings).
  my_plot
    .background_color(darkgreen)
    .legend_background_color(lightgray)
    .legend_border_color(black)
    .plot_background_color(lightgoldenrodyellow)
    .title_color(white)
    .y_major_grid_color(black);

  // X axis settings.
  my_plot.x_major_interval(2)
    .x_major_tick_length(14)
    .x_major_tick_width(1)
    .x_minor_tick_length(7)
    .x_minor_tick_width(1)
    .x_num_minor_ticks(3)

    // Y axis settings.
    .y_major_interval(25)
    .y_num_minor_ticks(5);

  // Legend settings.
  my_plot.legend_title_font_size(15)
    .legend_title("Legend");

  my_plot.plot(sin_data, "sin(x)")
    .line_on(true) // Line joining data points, using default color black.
    .shape(circlet) // and circle marker showing data points.
    .size(10) // Size (diameter pixels) of circlet data-point marker.
    .fill_color(yellow) // Outline is default black and centre yellow.
  // Default is no bezier.  Note angularity at the minima and maxima.
    ;

  my_plot.plot(cos_data, "cos(x)")
    .line_color(blue) // Defaults to showing line, but not in legend.
    .line_on(true) // Needed to show in the legend.
    .line_width(1) // thinner line.
    .shape(square) // Center of square has the data-point coordinate.
    .size(5)
    .fill_color(red)  // Center of square.
    ;

  my_plot.plot(tan_data, "tan(x)")
    .line_on(false)  // No line joining points.
    .shape(cone) // bottom point of cone has the coordinate of the data point.
    .size(5).fill_color(blue); // Just show data-point markers.

  my_plot.plot(sincos_data, "sincos(x)")
    .line_on(true)  // Just line joining points.
    .line_color(purple)
    .line_width(0.5)
    .bezier_on(true) // Note smoother at the minima and maxima.
    .shape(none) ; // NO data-point markers (and only shows a line in the legend).

  my_plot.write("demo_2d_bezier.svg"); // Final plot.

  std::cout << "demo_2d_bezier plot written to " << "demo_2d_bezier.svg" << std::endl;
  return 0;
} // int main()

