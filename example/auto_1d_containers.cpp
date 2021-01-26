/*! \file auto_1d_containers.cpp
 \brief Example to demonstrate autoscaling with *multiple* STL containers.
 \details See also auto_1d_plot.cpp and auto_1d_container.cpp.

 \date Feb 2009
 \author Paul A Bristow
 */

// Copyright Paul A Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[auto_1d_containers_1

/*`First we need a few includes to use Boost.Plot
(and some others only needed for this example).
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/detail/auto_axes.hpp>
  using boost::svg::show; // A single STL container.
  using boost::svg::show_all; // Multiple STL containers.
  //using boost::svg::range; // Find min and max of a single STL container.
  using boost::svg::range_all;// Find min and max of multiple STL containers.
  // Note neither of these check for 'limits' (infinite, NaN) values.

#include <boost/svg_plot/detail/pair.hpp>
//  using boost::svg::detail::operator<<; // Output pair as, for example: 1.23, 4.56

//#include <boost/svg_plot/show_1d_settings.hpp>
// Only needed for showing which settings in use.
//void boost::svg::show_1d_plot_settings(svg_1d_plot&);

#include <boost/algorithm/minmax.hpp>
 using boost::minmax;
#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;

#include <iostream>
  using std::cout;
  using std::endl;
  using std::boolalpha;

#include <limits>
  using std::numeric_limits;

#include <vector>
  using std::vector;

#include <utility>
  using std::pair;

#include <algorithm>
  using std::multiplies;
  using std::transform;
  using std::copy;
#include <iterator>
  using std::iterator;
  using std::iterator_traits;
  using std::ostream_iterator;

//] [/auto_1d_containers_1]

int main()
{
//[auto_1d_containers_2
  /*`This example uses two containers to demonstrate autoscaling.
  It is common to plot more than one set of data series together.
  Autoscaling must probably inspect all the containers of these series
  in order to find axis ranges that will be *suitable for all of them*.
  */
  vector<double> my_data_1;
  // Initialize STL container my_data_1 with some entirely fictional data.
  my_data_1.push_back(0.2); // [0]
  my_data_1.push_back(1.1); // [1]
  my_data_1.push_back(4.2); // [2]
  my_data_1.push_back(3.3); // [3]
  my_data_1.push_back(5.4); // [4]
  my_data_1.push_back(6.5); // [5]

  /*`
  We might use some convenient functions to list the container to cout.
  */
  show(my_data_1); // Entire container contents.
  /*`
  Others are easily written, often using std::copy, for example:
  ``
  copy(my_data_1_2.begin(), my_data_1_2.end(), ostream_iterator<double>(cout, " "));
  ``
  */
  /*`
  Now we concoct another equally fictional data series by a transform multiplying by 2.3.
  */

  vector<double> my_data_2; // Create a second data series.
  copy(my_data_1.begin(), my_data_1.end(), back_inserter(my_data_2));
  // Change the values in an entirely arbitrary way (each * 2.3).
  //std::transform(my_data_2.begin(), my_data_2.end(), my_data_2.begin(), std::bind1st(std::multiplies<double>(), 2.3));
  // std::bind1st is deprecated or removed.

  double factor = 2.3456;
  auto m1 = [factor](double& c) { return c * factor; };
  std::transform(my_data_2.begin(), my_data_2.end(), my_data_2.begin(), [factor](double& c) { return c * factor; });
  //  or 
   
    std::transform(my_data_2.begin(), my_data_2.end(), my_data_2.begin(), m1);
  //cout << std::endl << my_data.size() << " values in my_data_2. " << std::endl;

  /*`Next we need a new STL container, vector say, to hold our multiple containers of data series.
    They must all be the same STL container type, in this example, `vector<double>`.
    And we use pushback to add the containers.
  */
  vector<vector<double> > my_containers;

  my_containers.push_back(my_data_1); // Add 1st data series.
  my_containers.push_back(my_data_2); // Add another data series.
  cout << my_containers.size() << " containers." << std::endl;
  show_all(my_containers);

  /*` Finally we can use all the containers to find the minimum of mimimums and maximum of maximums
  ready to feed into the plot autoscale function.
  */
  pair<double, double> mm = range_all(my_containers);
  cout << "Data range: "  << mm << std::endl; // min & max of all data.
  svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

  /*`We could feed the minimum and maximum values separately,*/
  my_1d_plot.x_autoscale(mm.first, mm.second);  // Use minimum and maximum to autoscale.
  /*`
  but usually feeding the pair is more convenient.
  (Perhaps we might want to impose some other minimum and maximum here).
  */
  my_1d_plot.x_autoscale(mm);  // Use overall minimum and maximum to autoscale.

  /*`Finally, we add the data series containers to the plot, and write the SVG out to file.*/
  my_1d_plot.plot(my_data_1, "data_1");
  my_1d_plot.plot(my_data_2, "data_2").stroke_color(red);

  my_1d_plot.write("auto_1d_containers.svg"); // Write the plot to file.

  /*`If we want, we can check the autoscale range used.*/
  using boost::svg::detail::operator<<; // To avoid ambiguity.
  cout << "x_range() " << my_1d_plot.x_range() << std::endl; // x_range() 0, 15
  /*`And even all the (hundreds of) plot settings (useful for diagnosis why your plot doesn't meet your expectations).*/
  //show_1d_plot_settings(my_1d_plot);

//] [/auto_1d_containers_2]

  return 0;
} // int main()

/*

//[auto_1d_containers_output
Compiling...
auto_1d_containers.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\auto_1d_containers.exe"
6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
2 containers.
6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
6 values in container: 0.46 2.53 9.66 7.59 12.42 14.95
Data range: 0.2, 14.9
x_range() 0, 15
Build Time 0:03


with unc class

  Description: Autorun "J:\Cpp\SVG\Debug\auto_1d_containers.exe"
  6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
  2 containers.
  6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
  6 values in container: 0.46 2.53 9.66 7.59 12.42 14.95
  Data range: <0.2, 14.95>
  x_range() 0, 15






//] [/auto_1d_containers_output]

*/

