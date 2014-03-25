/*! \file 2d_simple.cpp
  \brief Simple 2D plot.
  \author Jacob Voytko
  \date 2007
*/

// Copyright Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <vector>

using std::map;
using namespace boost::svg;

int main()
{
  map<double, double> map1;
  map<double, double> map2;

  // This is random data used purely for example.
  map1[1.] = 3.2;
  map1[2.] = 5.4;
  map1[7.3] = 9.1;

  map2[3.1] = 6.1;
  map2[5.4] = 7.;

  svg_2d_plot my_plot;

  my_plot.title("Race Times")
         .legend_on(true)
         .x_range(-1, 11)
         .background_border_color(black);

  my_plot.plot(map1, "Series 1").stroke_color(blue);
  my_plot.plot(map2, "Series 2").stroke_color(orange);

    my_plot.write("./2d_simple.svg");
  return 0;
} // int main()
