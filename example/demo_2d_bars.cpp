/*! \file demo_2d_bars.cpp
    \brief Demonstrate the bars (& columns or sticks).
    \details 
    \date 11 Feb 2009
    \author Paul A. Bristow
*/

// Copyright Paul A. Bristow 2008

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <map>

using std::map;
using namespace boost::svg;

int main()
{
  map<double, double> map1; // Two sample data series.
  map<double, double> map2;

  // This is random data used purely for example purposes.  
  map1[1.] = 3.2; // map[x] = y
  map1[2.] = 5.4;
  map1[7.3] = 9.1;
  map1[8.] = 7.3;
  map1[5.5] = 5.4;
  map1[9.3] = 9.9;

  map2[-3.1] = -6.1;
  map2[5.4] = 4.7;

  svg_2d_plot my_plot1;

  my_plot1.title("X Times")
         .legend_on(true)
         .x_range(-1, 11)
         .background_border_color(black);

  my_plot1.plot(map1, "Series 1").fill_color(blue).shape(none).line_on(false).bar_opt(x_stick).bar_width(5).bar_color(red);
  my_plot1.write("./demo_2d_bar_x_stick.svg");
  my_plot1.plot(map1, "Series 1").shape(none).line_on(false).bar_opt(x_block).bar_width(10);
  my_plot1.write("./demo_2d_bar_x_block.svg");

  svg_2d_plot my_plot2;

  my_plot2.title("Y Times")
         .legend_on(true)
         .x_range(-10, 10)
         .background_border_color(blue);

  my_plot2.plot(map2, "Series 2").fill_color(green).shape(none).line_on(false).bar_opt(y_stick).bar_width(5).bar_color(cyan);
  my_plot2.write("./demo_2d_bar_y_stick.svg");
  my_plot2.plot(map2, "Series 2").shape(none).line_on(false).bar_opt(y_block).bar_width(10).bar_color(green).bar_area_fill(magenta);
  my_plot2.write("./demo_2d_bar_y_block.svg");

  return 0;
} // int main()

/* Output:

demo_2d_bars.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_2d_bars.exe"

*/
