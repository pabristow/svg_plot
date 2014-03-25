/*! \file demo_boxplot_full.cpp
    \brief Demonstration of more options for Boxplots.
    \details Quickbook markup so can be included in documentation.

    \author Jacob Voytko and Paul A. Bristow 
    \date Feb 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate nearly all boxplot options.
// See also demo_boxplot_simple.cpp and demo_boxplot.cpp for a narrow range of use.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_boxplot_full_1

/*`
Boxplot is a  convenient way of graphically depicting groups of numerical data 
through their five-number summaries.
Show 1st quartile, median and 3rd quartile as a box,
minimum and maximum non-outlier values as whiskers,
and outliers and extreme outliers.

See [@http://en.wikipedia.org/wiki/Boxplot boxplot] and

Some Implementations of the Boxplot
Michael Frigge, David C. Hoaglin and Boris Iglewicz
The American Statistician, Vol. 43, No. 1 (Feb., 1989), pp. 50-54

First we need a few includes to use Boost.Plot.
*/

#include <vector>
using std::vector;
#include <cmath>
using ::sin;
#include <boost/svg_plot/svg_boxplot.hpp>

#include <boost/array.hpp>
  using boost::array;

#include <iostream>
using std::cout;
using std::endl;

/*`Use two functions, 1/x and sin(x), to simulate distributions.
*/

double f(double x)
{ // Effectively 1/x.
  return 50 / x;
}

double g(double x)
{ // Effectively sin(x).
  return 40 + 25 * sin(x * 50);
}
//] [demo_boxplot_full_1]

int main()
{
  using namespace boost::svg;
  try
  {
//[demo_boxplot_full_2]
/*`10 values are computed and stored in two std:: vectors.
*/
  std::vector<double> data1;
  std::vector<double> data2;

  cout.precision(2);
  for(double i = 0.1; i < 10; i += 0.1)
  {   // Fill our vectors with 100 values:
    double fv = f(i);
    double gv = g(i);
    // cout << i << ' ' << fv << ' ' << gv << endl;
    data1.push_back(fv);
    data2.push_back(gv);
  }

  /*`Other containers, for example array, can be used too:
  */

  //const boost::array<double, 0> data0;
  //const boost::array<double, 10> data3 = {20., 30., 40., 45., 47., 50., 55., 60., 70, 80.};

/*`A new boxplot is contructed and very many settings added.
This is only to show their use and is intended to be visible, if totally tasteless!
*/
  svg_boxplot my_boxplot;

  //my_boxplot.plot(data0, "data0"); // Produces warning: "Message from thrown exception was: Data series data0 is empty!"

  //my_boxplot.plot(data3); // Produces warning: "Data series has no title!"

  my_boxplot.background_border_color(darkblue);
  my_boxplot.background_color(azure);

  my_boxplot  // Title and axes labels.
    .title("Boxplots of 1/x and sin(x) Functions")
    .x_label("Functions")
    .y_label("Population Size");

  my_boxplot.y_range(0, 100)  // Y-Axis information.
    .y_minor_tick_length(2)
    .y_major_interval(20);

/*`Many attributes of boxplots can be changed from the 'built-in' defaults, for example: */

   my_boxplot.whisker_length(25.).box_width(10)
     .box_fill(lime)
     .box_border(blue)
     .box_fill(lightblue)
     .median_color(red). median_width(2)
     .axis_color(orange).axis_width(4)
     .outlier_color(red)
     .outlier_fill(yellow)
     .outlier_shape(square)
     .outlier_size(5)
     .median_values_on(true)
     .outlier_values_on(true)
     .extreme_outlier_values_on(true) 
     .extreme_outlier_color(brown)
     .extreme_outlier_shape(diamond)
     .extreme_outlier_size(10)
    ;

   cout << my_boxplot.outlier_color() << endl; // red
   cout << my_boxplot.outlier_size() << endl; // size 10
   cout << my_boxplot.outlier_shape() << endl; // square

   //cout << my_boxplot.outlier_style.size() << endl; // doesn't work???


/*` Applies to all boxplots, unless changed for any individual plots, for example, change colors for data1 only:*/

   my_boxplot.plot(data1, "data1")
     .whisker_length(50.)
     .min_whisker_width(4).min_whisker_color(red)
     .max_whisker_width(7).max_whisker_color(green)
     .box_width(10)
     .box_fill(yellow)
     .box_border(magenta)
     .median_color(blue). median_width(5)
     .axis_color(lime). axis_width(1)
     .outlier_color(blue)
     .outlier_fill(yellow)
     .outlier_shape(cone)
     .outlier_size(10)
     .extreme_outlier_color(red)
     .extreme_outlier_fill(green)
     .extreme_outlier_shape(circlet)
     .extreme_outlier_size(10)
     //.extreme_outlier_values_on(true) not implemented.
;



   // my_boxplot.plot(data1, "test").box_style().fill_color(pink).stroke_color(green);
   // Once box_style() has been used to chain box styles, one can no longer chain to other non-box items, which is limiting.
   // So convenience functions are provided for many (but not all) features like: .box_fill(pink), box_border(green)...
   // Similar restrictions follow
   // my_boxplot.plot(data1, "test").box_width(10).whisker_length(5).median_style().stroke_color(purple);


/*`Add the two data series containers, and their labels, to the plot.
*/

  my_boxplot.plot(data1, "[50/x]");
  my_boxplot.plot(data2, "[sin(x*50)]");

/* 
  cout << "my_boxplot.title " << my_boxplot.title() << endl;
  cout << "my_boxplot.x_label_text "<< my_boxplot.x_label_text() << endl;
  cout << "my_boxplot.y_label_text " << my_boxplot.y_label_text() << endl; 

  cout << "my_boxplot.background_color " << my_boxplot.background_color() << endl;
  cout << "my_boxplot.background_border_color " << my_boxplot.background_border_color() << endl;
  cout << "my_boxplot.plot_background_color " << my_boxplot.plot_background_color() << endl;
  cout << "my_boxplot.plot_border_color " << my_boxplot.plot_border_color() << endl;
 */

/*`Finally write the SVG plot to a file.
*/
  my_boxplot.write("demo_boxplot_full.svg");

/*`You can view the plot (in all its 'glory') at demo_boxplot_full.svg."
*/

//] [demo_boxplot_full_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n  " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

Output:
demo_boxplot_full.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_boxplot_full.exe"
RGB(255,0,0)
5
2
Build Time 0:03

*/

