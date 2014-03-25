/*! \file demo_1d_x_external.cpp
  \brief 1D plot from two vectors of doubles, showing axis label variation.
  \author Jacob Voytko and Paul A. Bristow
  \date 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_1d_plot.hpp>
#include <vector>

using std::vector;
using namespace boost::svg;

//enum x_axis_intersect
//{ //! \enum x_axis_intersect If and how the X axes intersects Y axis.
//  bottom = -1, //!< X-axis free below bottom of end of Y-axis (case of all Y definitely < 0).
//  x_intersects_y = 0, //!< x_intersects_y when Y values include zero, so X intersects the Y axis.
//  top = +1 //!< X-axis free above top of X-axis (case of all Y definitely > 0).
//  };

int main()
{
//[demo_1d_x_external_1

/*`Following previous examples, we set up two containers for two data series.
*/
  vector<double> dan_times;
  vector<double> elaine_times;

  dan_times.push_back(3.1);
  dan_times.push_back(4.2);
  elaine_times.push_back(2.1);
  elaine_times.push_back(7.8);

  svg_1d_plot my_plot;

  // Adding some generic settings.
  my_plot.background_border_color(black)
         .legend_on(true)
         .plot_window_on(true)
         .title("Race Times")
         .x_range(-1, 10);

/*`We add tastelessly color the grids for both major and minor ticks, and switch both grids on.
*/
  my_plot.x_major_grid_color(pink)
         .x_minor_grid_color(lightgray);

  my_plot.x_major_grid_on(true)
         .x_minor_grid_on(true);

/*`Also we specify the position of the labelling of the X-axis.
It can be controlled using values in the `enum x_axis_intersect`.
``
  enum x_axis_intersect
  { //! \enum x_axis_intersect
    bottom = -1, // On the bottom of the plot window.
    x_intersects_y = 0, // On the Y axis (in the middle of the plot window).
    top = +1 // On the top of the plot window.
  };
``
For this example, we choose to show the X axis and tick value labels at the top of the plot window.
*/
  my_plot.x_ticks_on_window_or_axis(top); // on top, not on axis.

  // Write to plot.
  my_plot.plot(dan_times, "Dan").stroke_color(blue);
  my_plot.plot(elaine_times, "Elaine").stroke_color(orange);

  // Write to file.
  my_plot.write("./demo_1d_x_external.svg");
  return 0;
} // int main()
//] [/demo_1d_x_external_1]

/*
Output:

Compiling...
demo_1d_x_external.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_x_external.exe"
*/

