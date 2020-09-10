/*! \file 1d_full_layout.cpp
    \brief Example of 1 D plot of 3 different STL container types using several layout features.
    \details Creates file 1d_full_layout.svg
    \author Jacob Voytko
*/

// Copyright Jacob Voytko 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_1d_plot.hpp>
// for using namespace boost::svg; // Convenient to access colors and many other svg_plot items.
#include <boost/svg_plot/svg_color.hpp>
// for boost::svg::svg_plot::svg_color

#include <boost/svg_plot/show_1d_settings.hpp>

#include <array> //   using std::array;
#include <vector> // using std::vector;
#include <deque> // using std::deque;
#include <cmath> //  using std::sqrt;

// Some random functions to create the data to plot.
double f(double x)
{
  return std::sqrt(x);
}

double g(double x)
{
  return -2 + x*x;
}

double h(double x)
{
  return -1 + 2*x;
}

int main()
{
  using namespace boost::svg; // Convenient to access colors and many other svg_plot items.
  using boost::svg::svg_color;

  try
  {
    // Three containers of different types (just for show):
    std::vector<double> data1;
    std::deque<double> data2;
    std::array<double, 10> data3;

    // Fill the three containers with some randomish data :
    int j = 0;
    for (double i = 0; i < 9.5; i += 1.)
    {
      data1.push_back(f(i));
      data2.push_front(g(i));
      data3[j++] = h(i);
    }

    svg_1d_plot my_plot;

    // Size/scale settings for the plot.
    my_plot.size(700, 350)
      .x_range(-3, 10);

    // Text settings (note chaining 2nd and 3rd settings).
    my_plot.title("Animal Lives")
      .title_font_size(29)
      .x_label("life-time (months)");

    // Commands.
    my_plot.legend_on(true)
      .plot_window_on(true)
      .x_label_on(true)
      .x_major_labels_side(true);

    // Color settings.
    my_plot.background_color(darkslategray) // == svg_color(47 , 79 , 79 )
      // or .background_color(svg_color(47, 79, 79)) //  darkslategray in svg_color.hpp
      .legend_background_color(azure)
      .legend_border_color(gold)
      .plot_background_color(lightgoldenrodyellow)
      .title_color(white);

    // Axis settings.
    my_plot.x_major_interval(2)
      .x_major_tick_length(14)
      .x_major_tick_width(1)
      .x_minor_tick_length(7)
      .x_minor_tick_width(1)
      .x_num_minor_ticks(3);

    // Legend settings.
    my_plot.legend_title("Animal")
      .legend_title_font_size(15)
      ;

    // Add the data to the plot:
    my_plot.plot(data1, "Lions").stroke_color(blue);
    my_plot.plot(data2, "Tigers").stroke_color(orange);
    my_plot.plot(data3, "Bears").stroke_color(red);

    // Write the final plot in SVG format.
    my_plot.write("./1d_full_layout.svg");

    using boost::svg::show_1d_plot_settings;
    show_1d_plot_settings(my_plot);
  }
  catch (std::exception& ex)
  { // Report about any exceptions. 
    std::cout << "std::exception thown " << ex.what() << std::endl;
  }
  return 0;
} // int main()

// 1d_full_layout.cpp
