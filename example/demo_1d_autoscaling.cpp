/*! \file demo_1d_autoscaling.cpp
    \brief An example to demonstrate simple 1D settings, including auto-scaling.
    \details
      See auto_1d_containers.cpp for an example autoscaling with multiple data series.
      See also demo_1d_plot.cpp for a wider range of use.
*/
// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2009, 2012

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_autoscaling_1

/*`First some includes to use Boost.Plot
(and some others only needed for this example).
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/show_1d_settings.hpp>
// Only needed for showing which settings in use.
// void boost::svg::show_1d_plot_settings(svg_1d_plot&);
 // (Also provides operator<< for std::pair).

#include <boost/algorithm/minmax.hpp>
 using boost::minmax;
#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;

#include <iostream> // for debugging.
  //using std::cout;
  //using std::endl;
  //using std::boolalpha;

#include <limits>
//  using std::numeric_limits;

#include <vector>
//  using std::vector;
#include <set>
//  using std::multiset;

#include <utility>
//  using std::pair;

#include <boost/svg_plot/detail/auto_axes.hpp>
  using boost::svg::show; //! A single STL container.
  using boost::svg::show_all; //! Multiple STL containers.
  using boost::svg::range_mx; //! Find min and max of a single STL container.
  using boost::svg::range_all;  //! Find min and max of multiple STL containers.
//] [/demo_1d_autoscaling_1]

const double tol100eps = 1000 * std::numeric_limits<double>::epsilon(); // A suitable value for tight.

int main()
{
  //[demo_1d_autoscaling_2
  /*`This example uses a few types of containers to demonstrate autoscaling.
  Autoscaling can inspect the container in order to find axis ranges that will be suitable.
  First we create a container and fill with some fictional data, and display the values.
  */

  try
  {
    std::vector<double> my_data;
    // Initialize my_data with some entirely fictional data.
    my_data.push_back(0.2);
    my_data.push_back(1.1); // [1]
    my_data.push_back(4.2); // [2]
    my_data.push_back(3.3); // [3]
    my_data.push_back(5.4); // [4]
    my_data.push_back(6.5); // [5]
    show(my_data); // Show entire container contents,
    // 6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5

    /*`Construct a plot , and add some data to the plot. */

    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.
    my_1d_plot.title("Demo 1D autoscaling").x_label("X values"); // Add a title and X axis label.
    my_1d_plot.plot(my_data, "Auto 1D my_data"); // Add whole data series from my_data.

    /*`Use `x_autoscale` to scale the axis, in this most common and simplest case, using all the values.*/
    my_1d_plot.x_autoscale(my_data);

    /*` and finally write the SVG to a file.*/
    my_1d_plot.write("demo_1d_autoscaling_1.svg"); // Write the plot to file.
//] [/demo_1d_autoscaling_2]


//[demo_1d_autoscaling_3
    /*`In a second example, we use a different type of container, a set,
    and use autoscale in a more advanced way.*/
    std::multiset<double> my_set;
    // Initialize my_set with some entirely fictional data.
    my_set.insert(1.2);
    my_set.insert(2.3);
    my_set.insert(3.4);
    my_set.insert(4.5);
    my_set.insert(5.6);
    my_set.insert(6.7);
    my_set.insert(7.8);
    my_set.insert(8.9);
    // Show the set.
    std::multiset<double>::const_iterator si;
    show(my_set); // for two different types of container.
    // 8 values in container: 1.2 2.3 3.4 4.5 5.6 6.7 7.8 8.9

    svg_1d_plot my_1d_plot_2; // Construct a plot with all the default constructor values.

    /*`and also override the default controls of `scale_axis` function used by autoscaling.*/
    // Set some autoscaling parameters:
    my_1d_plot_2.x_with_zero(true); // Always include zero in the axis range, even if the data values don't.
    my_1d_plot_2.x_min_ticks(10); // Ensure more than the default minimum number of ticks.
    my_1d_plot_2.x_steps(0); // Permits axis ticks in steps of 1,2,4,6,8 or 1, 5, 10, or 1, 2, 5, 10.
    my_1d_plot_2.x_tight(0.01); // Prevent values that are only 1% outside the last tick requiring another tick.

    // Show the flags just set.
    std::cout << (my_1d_plot_2.x_with_zero() ? "x_with_zero, " : "not x_with_zero, ") // x_with_zero
      << my_1d_plot_2.x_min_ticks() << " x_min_ticks, " // 10
      << my_1d_plot_2.x_steps() << " x_steps, "  // 0
      << my_1d_plot_2.x_tight() << " tightness." << std::endl; // 0.001

    /*`Purely to show the possibilities with autoscale, we don't use the whole container,
    but use the two-iterators version of autoscale to *not use the first nor the last values* for autoscaling.
    (Remember values in set are sorted).
    */
    my_1d_plot_2.x_autoscale(++my_set.begin(),--my_set.end());
    /*`This also sets `autoscale(true)`, but note that `x_range()` is still not updated.
    If we want, we can display the ranges chosen by autoscale:
    */
    std::cout << " x_auto_min_value " << my_1d_plot_2.x_auto_min_value()
      << ", x_auto_max _value " << my_1d_plot_2.x_auto_max_value()
      << ", x_auto_tick_interval " << my_1d_plot_2.x_auto_tick_interval() << std::endl;

    /*`As before, we add the data set to the plot, and write to SVG XML to file.
    If you inspect the plot, you will see that the
    lowest data value 1.2 and highest data value 8.9 are no longer shown,
    because it is now outside the plot window.
    This is as if we had only written part of the data series.
    */
     my_1d_plot_2.plot(++my_set.begin(),--my_set.end(), "Auto 1D my_set"); // Add 'top and tailed' data series from my_set.
   //my_1d_plot.plot(my_set, "Auto 1D my_set"); // Add whole data series from my_set.
     my_1d_plot_2.write("demo_1d_autoscaling_2.svg"); // Write the plot to file.

    /*`If we want, we can check the autoscale range used, noting that zero *is* included because we demanded it.*/
    using boost::svg::detail::operator<<;
    std::cout << "x_range() " << my_1d_plot_2.x_range() << std::endl; // x_range() 0, 8

    //show_1d_plot_settings(my_1d_plot_2); // If required.
  }
  /*`try'n'catch blocks are very useful to display any plot error messages.
  Otherwise any exceptions thrown will just terminate - silently and most unhelpfully.
  */
  catch(const std::exception& e)
  {
    std::cout <<"\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
 //] [/demo_1d_autoscaling_3]
  return 0;
} // int main()
/*

//[demo_1d_autoscaling_output

demo_1d_autoscaling.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_autoscaling.exe"
6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
8 values in container: 1.2 2.3 3.4 4.5 5.6 6.7 7.8 8.9
x_with_zero, 10 x_min_ticks, 0 x_steps, 0.01 tightness.
 x_auto_min_value 0, x_auto_max _value 8, x_auto_tick_interval 0.5
x_range() 0, 8
Build Time 0:04

//] [/demo_1d_autoscaling_output]

*/
