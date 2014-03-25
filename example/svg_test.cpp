/*! \file svg_test.cpp
   \brief demonstrate boxplot of common functions.
   \details This shows the distribution of the function from the median, quartiles etc.
    This is now obselete - see demo_boxplot.cpp.

  \author Jake Voytko
  \date Mar 2009
*/

// Copyright Jake Voytko 2007
// Copyright Paul A. Bristow 2009

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#  pragma warning (disable : 4224) // warning C4224: nonstandard extension used : formal parameter 'function_ptr' was previously defined as a type
// in boost/function/function_template.hpp.  Should be fixed eventually.
#endif

#include <boost/svg_plot/svg.hpp>
  using boost::svg::svg;

#include <boost/svg_plot/svg_1d_plot.hpp>
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/svg_boxplot.hpp>
  using boost::svg::svg_boxplot;

#include <vector>
  using std::vector;
#include <iostream>
  using std::cout;
  using std::cin;
  using std::endl;
#include <cmath>

double h(double x)
{
    return 5 / (x);
}

double f(double x)
{
    return 40 + 25 * sin(x * 50);
}

int main()
{
    using namespace boost::svg;
    std::vector<double> data1, data2;
    for(double i= 0.1; i < 3; i += .1)
    {
      data1.push_back(h(i));
      data2.push_back(f(i));
    }

    svg_1d_plot my_1d_plot;

    my_1d_plot.title("Boxplots of Common Functions")
      .x_autoscale(data1);
    my_1d_plot.plot(data1, "[50 / x]");
    my_1d_plot.plot(data2, "[40 + 25 * sin(50x)]").stroke_color(red);

    my_1d_plot.write("./svg_test.svg");

    svg_boxplot my_boxplot;

    //my_boxplot.y_range(0, 100)  // Y-Axis information.
    //.y_minor_tick_length(10)
    //.y_major_interval(20);

    my_boxplot.y_autoscale(data2);

    my_boxplot.plot(data1, "[50 / x]");
    my_boxplot.plot(data2, "[40 + 25 * sin(50x)]");
    my_boxplot.write("./svg_test_box.svg");

    return 0;
} // int main()

/*

Output:

svg_test.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\Debug\svg_test.exe

*/
