/*! \file demo_boxplot_simple.cpp
    \brief An example to demonstrate simplest use of boxplot. See also boxplot_full.cpp for a wider range of use.

    \author Jacob Voytko and Paul A. Bristow
    \date 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[boxplot_simple_1

/*`
Boxplot is a  convenient way of graphically depicting groups of numerical data
through their five-number summaries.
Show 1st quartile, median and 3rd quartile as a box,
95% confidence interval as whiskers,
outliers and extreme outliers.

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
//] [boxplot_simple_1]

int main()
{
  using namespace boost::svg;
  try
  {
//[boxplot_simple_2]
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

/*`A new boxplot is constructed and a few settings added.
*/
  svg_boxplot my_boxplot;

  my_boxplot  // Title and axes labels.
    .title("Boxplots of 1/x and sin(x) Functions")
    .x_label("Functions")
    .y_label("Population Size");

  my_boxplot.y_range(0, 100)  // Axis information.
    .y_major_interval(20);

/*`Add the two data series containers, and their labels, to the plot.
*/

  my_boxplot.plot(data1, "[50 / x]");
  my_boxplot.plot(data2, "[40 + 25 * sin(x * 50)]");

/*`Finally write the SVG plot to a file.
*/
  my_boxplot.write("boxplot_simple.svg");
/*`You can view the plot at boxplot_simple.svg."
*/

//] [boxplot_simple_2]
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

Autorun "j:\Cpp\SVG\debug\demo_boxplot_simple.exe"

Build Time 0:00
Build log was saved at "file://j:\Cpp\SVG\demo_boxplot_simple\Debug\BuildLog.htm"


*/

