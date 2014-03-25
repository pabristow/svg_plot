/*! \file demo_1d_vector.cpp
    \brief Simple plot of vector of 1D data.
    \details An example to demonstrate simple 1D plot using two vectors,
     see also demo_1d_containers for examples using other STL containers.
    \author Jacob Voytko & Paul A. Bristow
    \date Feb 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_vector_1

/*`First we need a few includes to use Boost.Plot:
*/
#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
#include <vector>
  using std::vector;
//] [/demo_1d_vector_1]

int main()
{
//[demo_1d_vector_2

/*`STL vector is used as the container for our two data series,
and values are inserted using push_back.  Since this is a 1-D plot
the order of data values is not important.
*/
  vector<double> dan_times;
  dan_times.push_back(3.1);
  dan_times.push_back(4.2);

  vector<double> elaine_times;
  elaine_times.push_back(2.1);
  elaine_times.push_back(7.8);

/*`The constructor initializes a new 1D plot, called `my_plot`, and also sets all the very many defaults for axes, width, colors, etc. 
*/
  svg_1d_plot my_plot;

/*`A few (member) functions that set are fairly self-explanatory:

* title provides a title at the top for the whole plot,
* `legend_on(true)` will mean that titles of data series and markers will display in the legend box.
* `x_range(-1, 11)` sets the axis limits from -1 to +11 (instead of the default -10 to +10).
* `background_border_color(blue)` sets just one of the very many options.
*/

  my_plot.background_border_color(blue)
    .legend_on(true)
    .title("Race Times")
    .x_range(-1, 11);

  my_plot.legend_lines(true);

/*`The syntax `my_plot.title("Hello").legend_on(true)...` may appear unfamiliar,
but is a convenient way of specifying many parameters in any order. It is equivalent to:
``
  my_plot.title("Race Times");
  my_plot.legend_on(true);
  my_plot.x_range(-1, 11);
  my_plot.background_border_color(blue);
``
Chaining thus allows you to avoid repeatedly typing "`myplot.`"
and easily group related settings like plot window, axes ... together.
A fixed order would clearly become impracticable with
hundreds of possible arguments needed to set all the myriad plot options.

Within all of the plot classes, 'chaining' works the same way,
by returning a reference to the calling object thus `return  *this;`

Then we need to add our data series,
and add optional (but very helpful) data series titles
if we want them to show on the legend.
*/

  my_plot.plot(dan_times, "Dan").shape(circlet).size(10).stroke_color(red).fill_color(green);
  my_plot.plot(elaine_times, "Elaine").shape(vertical_line).stroke_color(blue);


/*`Finally, we can write the SVG to a file of our choice.
*/

  my_plot.write("./demo_1d_vector.svg");
//] [/demo_1d_vector_2]

  return 0;
} // int main()

/*

Output:
//[demo_1d_vector_output

Compiling...
demo_1d_vector.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_vector.exe"
Build Time 0:04
//] [/demo_1d_vector_output]
*/
