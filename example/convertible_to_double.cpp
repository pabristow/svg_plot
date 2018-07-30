/*! 
\brief 
Demonstrates plotting various types convertible to double.
*/

//  convertible_to_double.cpp

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

#include <boost/math/constants/constants.hpp> // For pi

// Example of a Boost.Multiprecision type.
#include <boost/multiprecision/cpp_bin_float.hpp>
// using boost::multiprecision:: cpp_bin_float_quad;
// but not convertible to double.

// Example of a User Defined Type - a fixed point type.
#include <boost/fixed_point/fixed_point_negatable.hpp>
#include <boost/fixed_point/fixed_point_negatable_cmath.hpp>
typedef boost::fixed_point::negatable<15, -16> fixed_point_type;
// Can be plotted OK.

// Some functions to generate trig functions.
// template of floating-point type to allow testing of any type.
template <typename T>
T f(T x)
{
  return sin(x);
}

template <typename T>
T g(T x)
{
  return cos(x);
}

template <typename T>
T h(T x)
{
  return tan(x);
}

template <typename T = double>
void trig_plots()
{
  using namespace boost::svg; // Very convenient to allow easy access to colors and other items.

  std::map<T, T> sin_data, cos_data, tan_data, sincos_data;

  T step = boost::math::constants::pi<T>() / 8;  // Interval between function data points.

   // Generate some trigonmetric data to plot.
  for (T i = static_cast<T>(0); i <= static_cast<T>(10); i += step)
  {
    sin_data[i] = f(i); // sin
    cos_data[i] = g(i); // cos
    tan_data[i] = h(i); // tan
    sincos_data[i] = sin(g(i)); // sincos
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
  // Search engines will provide Unicodes by querying "UNicode ampersand"
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
    .size(10) // Size (diameter pixels) of circlet data point marker.
    .fill_color(yellow) // Outline is default black and centre yellow.
                        // Default is no bezier.  Note angularity at the minima and maxima.
    ;

  my_plot.plot(cos_data, "cos(x)")
    .line_color(blue) // Defaults to showing line, but not in legend.
    .line_on(true) // Needed to show in the legend.
    .line_width(1) // thinner line.
    .shape(square) // Center of square has the data point coordinate.
    .size(5)
    .fill_color(red)  // Center of square.
    ;

  my_plot.plot(tan_data, "tan(x)")
    .line_on(false)  // No line joining points.
    .shape(cone) // bottom point of cone has the coordinate of the data point.
    .size(5).fill_color(blue); // Just show data point markers.

  my_plot.plot(sincos_data, "sincos(x)")
    .line_on(true)  // Just line joining points.
    .line_color(purple)
    .line_width(0.5)
    .bezier_on(true) // Note smoother at the minima and maxima.
    .shape(none); // NO data point markers (and only shows a line in the legend).

  my_plot.write("./demo_convertible_to_double.svg"); // Final plot.

} // void trig_plots()

int main()
{

  trig_plots<float>(); // OK
  trig_plots<>(); // default double
  trig_plots<double>(); // OK
  trig_plots<long double>(); // OK
  // OK, no warnings, but range from 
  //   (std::numeric_limits<long double>::max)() to min() is greater than for double.
  // so overflow or underflow on conversion to double is possible.

  //using boost::multiprecision::cpp_bin_float_quad;
  //trig_plots<cpp_bin_float_quad>();

  // i:\modular-boost\boost\svg_plot\detail\functors.hpp(207):
  // error C2338: Uncertain types must be convertible to double!
    // error C2440: 'type cast': cannot convert from 'const boost::multiprecision::cpp_bin_float_quad' to 'Meas'
  // Using map with cpp_bin_float_quad would be very inefficient?

  // Using a fixed-point might also be problematic?
  //typedef boost::fixed_point::negatable<15, -16> fixed_point_type;
  trig_plots<fixed_point_type>(); // OK
  // But some fixed_point types might fail this example because
  // the range from max to min might not be great enough for the data to plot chosen.

  // Hopeless case - but does not provide a very helpful error message.
  // trig_plots<std::string>();

  return 0;
} // int main()


//! Demonstrate 