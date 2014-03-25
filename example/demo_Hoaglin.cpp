/*! \file demo_Hoaglin.cpp
  \brief Demonstration of Boxplot quartile options. Boxplots appear different depending the choice of definition for the quartile.

  \details
    "Some Implementations of the Boxplot"
    Michael Frigge, David C. Hoaglin and Boris Iglewicz
    The American Statistician, Vol. 43, No. 1 (Feb., 1989), pp. 50-54
    discusses the design of the boxplot.

    However the plot of their example data shown below shows the considerable variation in the appearance of the same data,
    using different definitions of quartiles used in various popular statistics packages.

    One obvious conclusion is that you should not expect boxplots to look the same when using more than one program.

    Boost.Plot provides 5 popular definitions for the quartiles.
    This should allow the user to produce plots that look similar to boxplots from most statistics plotting program.
    To confuse matter further, most have their own default definition *and* options to chose other definitions:
    these options are shown below as type, method, PCTLDEF.

    The interquartile range is calculated using the 1st \& 3rd sample quartiles,
    but there are various ways to calculate those quartiles, summarised in
    Rob J. Hyndman and Yanan Fan, 1996, "Sample Quantiles in Statistical Packages",
    The American Statistician 50(4):361-365, (1996).

    The interquartile range, often called IQR is quartile 3 (p = 3/4) - quartile 1 (1/4).
    The median is the 2nd quartile (p = 2/4 = 1/2).

    Five of Hyndman and Fan's sample quantile definitions have a particularly simple common form
    selected according to which definition of m is chosen in function quantiles.
    This is implemented in function quantiles by parameter `HF_definition`:

      double quantile(vector<double>& data, double p, int HF_definition = 8);

    The default definition is that recommended by Hyndman and Fan, or
    users can select which definition is used for all boxplots, or individual data series as shown in the example below.

       my_boxplot.quartile_definition(5); // All plots

       my_boxplot.plot.quartile_definition(7); // Just this data series plot.

    Hyndman and Fan definitions 4 to 8 are used by the following packages:

    * #4 SAS (PCTLDEF=1), R (type=4), Maple (method=3)
    * #5 R (type=5), Maple (method=4), Wolfram Mathematica quartiles.
    * #6 Minitab, SPSS, BMDP, JMP, SAS (PCTLDEF=4), R(type=6), Maple (method=5).
    * #7 Excel, S-Plus, R (type=7[default]), Maxima, Maple (method=6).
    * #8 H&F 8: R (type=8), Maple (method=7[default]).

    Some observations on the various options are:

    * #4 Often a moderate interquartile range.

    * #5 Symmetric linear interpolation: a common choice when the data represent a sample
    from a continuous distribution and you want an unbiased estimate of the quartiles of that distribution.

    * #6 This "half" sample excludes the sample median (k observations) for odd n (=2*k+1).
    This will tend to be a better estimate for the population quartiles,
    but will tend to give quartile estimates that are a bit too far
    from the center of the whole sample (too wide an interquartile range).

    * #7 Smallest interquartile range, so flags most outliers.
    For a continuous distribution,
    this will tend to give too narrow an interquartile range,
    since there will tend to be a small fraction of the population beyond the extreme
    sample observations. In particular, for odd n (=2*k+1), Excel calculates the
    1st (3rd) quartile as the median of the lower (upper) "half" of the sample
    including the sample median (k+1 observations).

    * #8 recommended by H&F because it is
    approximately median-unbaised estimate regardless of distribution
    and thus suitable for continuous and discrete distributions.
    which gives quartiles between those reported by Minitab and Excel.
    This approach is approximately median unbiased for continuous distributions.
    Slightly higher interquartile range than definition 7.

    The 'fences' beyond which points are regarded as outliers, or extreme outliers,
    are a multiplying factor, usually called k, and usually 1.5 * interquartile range,
    and 3 * interquartile range as recommended by Hoaglin et al.

  \author Paul A Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate the implementation of boxplot.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_Hoaglin_1

/*`

"Some Implementations of the Boxplot"
Michael Frigge, David C. Hoaglin and Boris Iglewicz
The American Statistician, Vol. 43, No. 1 (Feb., 1989), pp. 50-54
discusses the design of the boxplot.

However the plot of their example data shown below shows the considerable variation in the appearance of the same data,
using different definitions of quartiles used in various popular statistics packages.

One obvious conclusion is that you should not expect boxplots to look the same when using more than one program.

Boost.Plot provides 5 popular definitions for the quartiles.
This should allow the user to produce plots that look similar to boxplots from most statistics plotting program.
To confuse matter further, most have their own default definition *and* options to chose other definitions:
these options are shown below as type, method, PCTLDEF.

The interquartile range is calculated using the 1st & 3rd sample quartiles,
but there are various ways to calculate those quartiles, summarised in
Rob J. Hyndman and Yanan Fan, 1996, "Sample Quantiles in Statistical Packages",
The American Statistician 50(4):361-365, (1996).

The interquartile range, often called IQR is quartile 3 (p = 3/4) - quartile 1 (1/4).
The median is the 2nd quartile (p = 2/4 = 1/2).

Five of Hyndman and Fan's sample quantile definitions have a particularly simple common form
selected according to which definition of m is chosen in function quantiles.
This is implemented in function quantiles by parameter `HF_definition`:

  double quantile(vector<double>& data, double p, int HF_definition = 8);

The default definition is that recommended by Hyndman and Fan, or
users can select which definition is used for all boxplots, or individual data series as shown in the example below.

   my_boxplot.quartile_definition(5); // All plots

   my_boxplot.plot.quartile_definition(7); // Just this data series plot.

Hyndman and Fan definitions 4 to 8 are used by the following packages:

* #4 SAS (PCTLDEF=1), R (type=4), Maple (method=3)
* #5 R (type=5), Maple (method=4), Wolfram Mathematica quartiles.
* #6 Minitab, SPSS, BMDP, JMP, SAS (PCTLDEF=4), R(type=6), Maple (method=5).
* #7 Excel, S-Plus, R (type=7[default]), Maxima, Maple (method=6).
* #8 H&F 8: R (type=8), Maple (method=7[default]).

Some observations on the various options are:

* #4 Often a moderate interquartile range.

* #5 Symmetric linear interpolation: a common choice when the data represent a sample
from a continuous distribution and you want an unbiased estimate of the quartiles of that distribution.

* #6 This "half" sample excludes the sample median (k observations) for odd n (=2*k+1).
This will tend to be a better estimate for the population quartiles,
but will tend to give quartile estimates that are a bit too far
from the center of the whole sample (too wide an interquartile range).

* #7 Smallest interquartile range, so flags most outliers.
For a continuous distribution,
this will tend to give too narrow an interquartile range,
since there will tend to be a small fraction of the population beyond the extreme
sample observations. In particular, for odd n (=2*k+1), Excel calculates the
1st (3rd) quartile as the median of the lower (upper) "half" of the sample
including the sample median (k+1 observations).

* #8 recommended by H&F because it is
approximately median-unbaised estimate regardless of distribution
and thus suitable for continuous and discrete distributions.
which gives quartiles between those reported by Minitab and Excel.
This approach is approximately median unbiased for continuous distributions.
Slightly higher interquartile range than definition 7.

The 'fences' beyond which points are regarded as outliers, or extreme outliers,
are a multiplying factor, usually called k, and usually 1.5 * interquartile range,
and 3 * interquartile range as recommended by Hoaglin et al.

*/

#include <vector>
using std::vector;
#include <cmath>
using ::sin;
//#include <boost/assert.hpp> // for BOOST_ASSERT
#include <boost/svg_plot/svg_boxplot.hpp>

#include <boost/svg_plot/quantile.hpp>
using boost::svg::quantile;

// double boost::svg::quantile(vector<double>& data, double p, int HF_definition);
// Estimate pth quantile of data using one of 5 definitions.
// Default HF_definition is the recommendation of Hyndman and Fan, definition #8.

#include <boost/array.hpp>
  using boost::array;

#include <iostream>
using std::cout;
using std::endl;
//] [demo_Hoaglin_1]

int main()
{
  using namespace boost::svg;
  try
  {
//[demo_Hoaglin_2]
  // 11 values from Hoaglin et al page 50.
  const boost::array<double, 11> Hoaglin_data = {53., 56., 75., 81., 82., 85., 87., 89., 95., 99., 100.};
  //                                                       q1           median           q3

  vector<double> Hoaglin(Hoaglin_data.begin(), Hoaglin_data.end());
  for (int def = 4; def <= 8; def++)
  { // All the F&Y definitions of quartiles.
    double q1 = quantile(Hoaglin, 0.25, def); // 75
    double q2 = quantile(Hoaglin, 0.5, def); // 85
    double q3 = quantile(Hoaglin, 0.75, def); // 95
    cout << "Hoaglin definition #" << def << ", q1 " << q1
      << ", q2 " << q2 << ", q3 " << q3 << ", IQR " << q3 - q1 << endl;
  } // for

  // Same data copied for different data series.
  vector<double> Hoaglin4(Hoaglin_data.begin(), Hoaglin_data.end());
  vector<double> Hoaglin5(Hoaglin_data.begin(), Hoaglin_data.end());
  vector<double> Hoaglin6(Hoaglin_data.begin(), Hoaglin_data.end());
  vector<double> Hoaglin7(Hoaglin_data.begin(), Hoaglin_data.end());
  vector<double> Hoaglin8(Hoaglin_data.begin(), Hoaglin_data.end());

  svg_boxplot H_boxplot;

  /*`Show the quartile definition default.
*/
    cout << "Default boxplot.quartile_definition() = " << H_boxplot.quartile_definition() << endl; // 8

/*` Add title, labels, range etc to the whole boxplot:
*/
  H_boxplot  // Title and axes labels.
    .title("Hoaglin Example Data")
    .x_label("Boxplot")
    .y_label("Value")
    .y_range(45, 115)  // Y-Axis range.
    .y_minor_tick_length(2)
    .y_major_interval(10);

/*`Add a few setting to the plot including setting quartile definition (though is actually same as the default 8),
and show that the value is stored.
*/
    svg_boxplot& b = H_boxplot.median_values_on(true)
    .outlier_values_on(true)
    .extreme_outlier_values_on(true)
    .quartile_definition(8);
/*`Show the quartile definition just assigned:
*/
    cout << "boxplot.quartile_definition() = " << b.quartile_definition() << endl; // 8

/*`Add a data series container, and labels, to the plot using the whole boxplot quartile definition set.
*/
    H_boxplot.plot(Hoaglin_data, "default_8");

/*`Add another data series container, and the labels, to the plot, and select a *different* quartile definition.
*/

    svg_boxplot_series& d4 =
    H_boxplot.plot(Hoaglin4, "def #4")
    .whisker_length(4.)
    .quartile_definition(4);

/*`Show the quartile definition just assigned to the this data series.
*/
  cout << "boxplot_series.quartile_definition() = " << d4.quartile_definition() << endl; // 4

/*`Add yet more data series container, and the labels, to the plot, and select a *different* quartile definition for each.
*/    H_boxplot.plot(Hoaglin5, "def #5")
    .whisker_length(5.)
    .quartile_definition(5);

    H_boxplot.plot(Hoaglin6, "def #6")
    .whisker_length(6.)
    .quartile_definition(6);

    H_boxplot.plot(Hoaglin6, "def #7")
    .whisker_length(7.)
    .quartile_definition(7);

    H_boxplot.plot(Hoaglin6, "def #8")
    .whisker_length(8.)
    .quartile_definition(8);

/*`Write the entire SVG plot to a file.
*/
  H_boxplot.write("demo_Hoaglin.svg");
//] [demo_Hoaglin_2]
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

//[demo_Hoaglin_output

``Autorun "j:\Cpp\SVG\debug\demo_Hoaglin.exe"
Hoaglin definition #4, q1 70.25, q2 83.5, q3 90.5, IQR 20.25
Hoaglin definition #5, q1 76.5, q2 85, q3 93.5, IQR 17
Hoaglin definition #6, q1 75, q2 85, q3 95, IQR 20
Hoaglin definition #7, q1 78, q2 85, q3 92, IQR 14
Hoaglin definition #8, q1 76, q2 85, q3 94, IQR 18
Default boxplot.quartile_definition() = 8
boxplot.quartile_definition() = 8
boxplot_series.quartile_definition() = 4
``
//] [demo_Hoaglin_output]


*/

