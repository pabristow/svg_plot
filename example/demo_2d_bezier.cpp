/*! \file demo_2d_bezier.cpp
\brief Simple 2D plot using Bezier curves,
data points markers, lines joining data points,
and legend identifying point marks and lines.
*/

// Copyright Paul A. Bristow 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

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
  std::map<double, double> sin_data, cos_data, tan_data;

  double inter = 3.14159265 / 8.;

  // Generate some data to plot.
  for(double i = 0; i <= 10.; i += inter)
  {
    sin_data[i] = f(i); // sin
    cos_data[i] = g(i); // cos
    tan_data[i] = h(i); // tan
  }

  svg_2d_plot my_plot; // To hold the plot.

  // Size/scale settings.
  my_plot.size(700, 500) // SVG image size (pixel).
    .x_range(-0.5, 10.5) // Range of x and y, 
    .y_range(-1.1, 1.1); // chosen to ensure that the maxima and minimax 
                         // are not on the edge of the plot window.

  // Text settings.
  my_plot.title("Plot of sin, cos &#x26; tan functions")
    // Note: for ampersand must use Unicode &#x26; because it is a reserved symbol in SVG XML.
    // Search engines will provide Unicodes by querying "UNicode ampersand"
    // at sites like https://unicode.org/, 
    // http://www.fileformat.info/info/unicode/char/0026/index.htm and others.
    .title_font_size(28)
    .x_label("x Axis Units")
    .y_major_labels_side(left)
    .y_major_grid_on(true);

  // Commands.
  my_plot.legend_on(true) // Want a legend box.
    .plot_window_on(true) // want a plot window with axis lables etc outside.
    .x_label_on(true);

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

  // legend settings.
  my_plot.legend_title_font_size(15)
    .legend_title("Legend");

  my_plot.plot(sin_data, "sin(x)")
    .line_on(true) // Line joining data points, default color black.
    .shape(circlet) // and circle showing data points.
    .size(10) // of circle
    .fill_color(yellow) // Outline is default black and centre yellow.
  // Default is no bezier.  Note angularity at the minima and maxima.
    ;

  my_plot.plot(cos_data, "cos(x)")
    .line_color(blue) 
    .bezier_on(true) // Note smooth at the minima and maxima.
    .shape(square).size(5).fill_color(red);

  my_plot.plot(tan_data, "tan(x)")
    .line_on(false)  // No line joining points.
    .shape(cone).size(5).fill_color(blue); // Just show points.

  my_plot.write("./demo_2d_bezier.svg");

  return 0;
} // int main()

