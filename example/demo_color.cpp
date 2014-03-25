/*! \file demo_color.cpp
  \brief Demonstration of some SVG color features.
  \details This produces a sample SVG file.
  \author Paul A Bristow 
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This module is entirely contrived examples of using svg_colors.
// see also test_svg.cpp for tests using Boost.Test.

#ifdef _MSC_VER
#  pragma warning(disable : 4127) //  conditional expression is constant
#  pragma warning(disable : 4512) //  assignment operator could not be generated
#  pragma warning(disable : 4224) //  nonstandard extension used : formal parameter 'function_ptr' was previously defined as a type
#endif

#include <boost/svg_plot/svg.hpp>
  using boost::svg::svg;

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <iostream>
  using std::cout;
  using std::endl;
  using std::boolalpha;

#include <vector>
  using std::vector;

  using namespace std;

int main()
{

  svg_1d_plot my_colors;

  my_colors.title("Demo SVG colors")
    .legend_on(true)
    .legend_title("Legend title")
    .x_label_on(true)
    .x_label("axis") // This doesn't show unless .x_label_on() == true!
    // x_label is far too far below the axis.
    .x_axis_color(blue)
    .x_label_color(magenta)
    .x_range(0., 7.);

  vector<double> my_data;
  my_data.push_back(1.);
  my_data.push_back(2.);
  my_data.push_back(3.);

  my_colors.plot(my_data, "My Data");

  my_colors.write("demo_colors.svg");

  {
    svg_color my_blank(false);
    cout << "svg_color my_blank(false);" << boolalpha << endl;
    cout << "my_blank.is_blank() " << my_blank.is_blank() << endl;
    cout << "my_blank.red() " << my_blank.red() << endl;
    cout << "my_blank.green() " << my_blank.green() << endl;
    cout << "my_blank.blue() " << my_blank.blue() << endl;
    my_blank.write(cout); cout << endl;
  }
  {
    svg_color my_blank(true);
    cout << "svg_color my_blank(true)" << boolalpha << endl;
    cout << "my_blank.is_blank is " << my_blank.is_blank() << endl;
    cout << "my_blank.r my_blank.g my_blank.b = "
      << (unsigned int)my_blank.red() << ' ' 
      << (unsigned int)my_blank.green() << ' ' 
      << (unsigned int)my_blank.blue() << endl;
    cout << "my_blank " << my_blank << endl; // my_blank RGB(0,0,0) == default color.
    my_blank.write(cout); cout << endl;
  }
  {
    svg_color my_blank(blank);
    cout << "svg_color my_blank(blank)" << boolalpha << endl;
    cout << "my_blank.is_blank is " << my_blank.is_blank() << endl;
    cout << "my_blank.red() my_blank.green() my_blank.blue() = "
      << (unsigned int)my_blank.red() << ' ' 
      << (unsigned int)my_blank.green() << ' ' 
      << (unsigned int)my_blank.blue() << endl;
    cout << "my_blank " << my_blank << endl; // my_blank RGB(0,0,0) == default color.
    my_blank.write(cout); cout << endl;
  }

  svg_color my_red(255, 0, 0); // 
  cout << "red " << red << endl; // Output: red 119 - because red is an enum!  Caution!
  cout << "red " << svg_color(red) << endl; // Output: red RGB(255,0,0)
  if (my_red == svg_color(red)) 
  {
    cout << "my_red == svg_color(red)" << endl;
  }
  if (my_red == red) 
  {
    cout << "my_red == red" << endl;
  }
  if (my_red != black) 
  {
    cout << "my_red != black" << endl;
  }
  if (white != black) 
  {
    cout << "my_red != black" << endl;
  }

  svg_color my_color(127, 255, 212); // Specify a 'custom' color, not in the svg colors list.
  svg_color my_same_color(127, 255, 212);
  my_color.write(cout); cout << endl;
  cout << "my_color " << my_color << endl;

  svg_color my_color2(magenta);
  my_color2.write(cout);
  cout << endl;
  cout << "magenta " << magenta << endl; // magenta 85 because magenta is an enum!
  cout << "magenta " << svg_color(magenta) << endl; // magenta RGB(255,0,255)

  cout << "(my_color == my_color2) is " <<  (my_color == my_color2) << endl;
  cout << "(my_color == my_same_color) is "<<  (my_color == my_same_color) << endl;
  svg_color my_blank(blank);
  cout << "(my_blank == my_blank) is "<<  (my_blank == my_blank) << endl;
  svg_color my_blank2(true);
  cout << "(my_blank == my_blank2) is "<<  (my_blank == my_blank2) << endl;

  my_colors.write("demo_color.svg");

  return 0;
} // int main()

/*

Output:

Compiling...
demo_color.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_color.exe"
svg_color my_blank(false);
my_blank.is_blank is true
my_blank.r my_blank.g my_blank.b = 0 0 0
my_blank blank
none
svg_color my_blank(true)
my_blank.is_blank is false
my_blank.r my_blank.g my_blank.b = 0 0 0
my_blank RGB(0,0,0)
rgb(0,0,0)
svg_color my_blank(blank)
my_blank.is_blank is true
my_blank.r my_blank.g my_blank.b = 255 255 255
my_blank blank
none
red 119
red RGB(255,0,0)
my_red == svg_color(red)
my_red == red
my_red != black
my_red != black
rgb(127,255,212)
my_color RGB(127,255,212)
rgb(255,0,255)
magenta 85
magenta RGB(255,0,255)
(my_color == my_color2) is false
(my_color == my_same_color) is true
(my_blank == my_blank) is true
(my_blank == my_blank2) is false
Build Time 0:03



*/

