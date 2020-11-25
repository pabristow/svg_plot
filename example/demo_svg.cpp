/*!  \file demo_svg.cpp
    \brief Demonstration of a few facets of using the SVG interface directly.
    \details This only demonstrates a very few of the possible features
      because most users will only need to use the plot interfaces.
      But this might provide a little guidance for producing other diagrams and drawings.
      This module is to demonstrate features of the svg class.
      It is entirely contrived and has no other conceivable use!
*/
// Copyright Paul A Bristow 2007, 2012, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdlib.hpp> // for exit codes 

// include <boost/test/included/unit_test.hpp >

#include <boost/svg_plot/svg.hpp>
  // using boost::svg::svg;
  // using boost::svg::g_element;
  // using boost::svg::rect_element;

#include <boost/svg_plot/svg_fwd.hpp>
  // using namespace boost::svg;
  // Is most convenient because otherwise all the colors used must be specified thus:
  // using boost::svg::black;
  // using boost::svg::white;
  // using boost::svg::red;
  // using boost::svg::blue;
  // ... which may get tedious!

#include <iostream>
  //using std::cout;
  //using std::endl;
  //using std::boolalpha;

#include <vector>
// using std::vector;

#include <string>
// using std::string;

#include <boost/array.hpp>
// using boost::array;

using namespace boost::svg;

void symb(boost::array<const poly_path_point, 6> shape, const poly_path_point position, const poly_path_point size, g_element& g)
{
  std::vector<poly_path_point> shaped;
  for (boost::array<const poly_path_point, 6>::iterator i = shape.begin(); i != shape.end(); ++i)
  {
    poly_path_point p = (*i);
    p.x *= size.x;
    p.y *= size.y;
    p.x += position.x;
    p.y += position.y;
    std::cout << p << std::endl;
    shaped.push_back(p);
  }
  std::cout << std::endl;
  // Can't return ref to shape because goes out of scope :-(
  g.push_back(new polygon_element(shaped, true));
  return; // reference to g_element just added.
} // vector<poly_path_point>& symb

int main()
{
  try
  {

  // Is most convenient because otherwise all the colors used must be specified thus:
  using boost::svg::black;
  using boost::svg::white;
  using boost::svg::red;
  using boost::svg::blue;
  // ... which may get tedious!

  using boost::svg::svg;
  using boost::svg::g_element;
  using boost::svg::rect_element;
  using boost::svg::text_element;

  using boost::array;

  svg my_svg;

  std::cout << "Initial my_svg.document_size() " << my_svg.document_size() << std::endl; // == 0

  // Check default and change image size.
  std::cout << "my_svg.x_size() = " << my_svg.x_size() << ", my_svg.y_size() = " << my_svg.y_size() << std::endl;
  // my_svg.x_size() = 400, my_svg.y_size() = 400
  my_svg.size(500, 600);
  std::cout << "my_svg.x_size() = " << my_svg.x_size() << ", my_svg.y_size() = " << my_svg.y_size() << std::endl;
  // my_svg.x_size() = 500, my_svg.y_size() = 600

  boost::svg::text_element t;
  t.textstyle().font_family("arial");
  std::cout << t.textstyle().font_family() << std::endl; // Echos "arial".

  // Exercise a few document metadata items.
  my_svg.document_title("Demo SVG document title");

  my_svg.license("permits", "permits", "permits", "permits");
  my_svg.license_on(true);
  std::cout << std::boolalpha << "my_svg.license_on() " << my_svg.license_on() << std::endl;

  my_svg.boost_license_on(true);
  std::cout << "my_svg.boost_license_on() " << my_svg.boost_license_on() << std::endl;

  my_svg.image_filename("demo_svg"); // File into which SVG XML will be written.
  std::cout << "my_svg.image_filename() " << my_svg.image_filename() << std::endl;

  // Default background color is black, rgb(0,0,0)
  // Draw background. detail::PLOT_BACKGROUND == 0
  // my_svg.gs(0).push_back(new rect_element(0, 0, my_svg.x_size(),  my_svg.y_size() ) );
  // background 

  my_svg.add_g_element(); // Add first (zeroth) new element to the document,

  std::cout << "After one add_g_element my_svg.document_size() = " << my_svg.document_size() << std::endl;

  g_element& g0 = my_svg.gs(0); // so index is zero.
  std::cout << "my_svg.document_size() = number of g_elements = " << my_svg.document_size() << std::endl; // == 1
  my_svg.gs(0).push_back(new rect_element(0, 0, my_svg.x_size(), my_svg.y_size())); // border to image.
  g0.id("group element 0"); // Add an ID to this group.
  std::cout << "g0.id() " << g0.id() << std::endl; // echo it.

  // Show the default colors of the group element g0 added above.
  std::cout << "fill color = " << g0.style().fill_color() << std::endl; // fill color = RGB(0,0,0) == black
  std::cout << "fill on " << std::boolalpha << g0.style().fill_on() << std::endl; // false
  std::cout << "stroke color = " << g0.style().stroke_color() << std::endl; // stroke color = RGB(0,0,0)
  std::cout << "stroke on " << std::boolalpha << g0.style().stroke_on() << std::endl; // stroke on false
  std::cout << "stroke width " << std::boolalpha << g0.style().stroke_width() << std::endl;
  // Change a svg style - color and width, and fill.
  g0.style().stroke_on(true); // stroke on true
  g0.style().stroke_color(red); // 
  g0.style().stroke_width(5);
  std::cout << "width = " << g0.style().stroke_width() << std::endl;
  g0.style().fill_on(true); // stroke on true
  g0.style().fill_color(azure); // 
  std::cout << "fill color = " << g0.style().fill_color() << std::endl; // fill color = RGB(0,0,0) == black
  std::cout << "fill on " << std::boolalpha << g0.style().fill_on() << std::endl; // false
  std::cout << "stroke color = " << g0.style().stroke_color() << std::endl; // stroke color = RGB(0,0,0)
  std::cout << "stroke on " << std::boolalpha << g0.style().stroke_on() << std::endl; // stroke on true
  std::cout << "width on " << std::boolalpha << g0.style().width_on() << std::endl; // width on true
  g0.style().stroke_on(true); // stroke on true
  // This might generate svg xml like this:
  //<g stroke="rgb(255,0,0)" fill="rgb(255,255,255)" stroke-width="10"><rect x="0" y="0"  width="500"  height="600"/></g>

  rect_element r(30, 30, 40, 40);
  std::cout << "rect_element r(30, 30, 40, 40);  " << r << std::endl; // rect(30, 30, 40, 40)

  g0.push_back(new line_element(100, 50, 50, 100)); // red border width 10 white fill.
  // <g stroke="rgb(255,0,0)" fill="rgb(0,0,255)" stroke-width="10"><line x1="100" y1="50" x2="50" y2="100"/></g>

  std::cout << "After adding a line my_svg.document_size() = number of g_elements = " << my_svg.document_size() << std::endl; // == 1

  g0.push_back(new rect_element(20, 20, 50, 50)); // 
  g0.push_back(new polygon_element(30, 40, true));
  g0.push_back(new circle_element(100, 200, 10)); // 
  g0.push_back(new ellipse_element(300, 300, 5, 3)); // 
  std::cout << "After adding several shape elements my_svg.document_size() = number of g_elements = " << my_svg.document_size() << std::endl; // == 1
  std::cout << "After adding several shape elements g0.size() = number of child elements = " << g0.size() << std::endl; // == 6

  my_svg.ellipse(300, 300, 50, 30); // has similar effect, but since not in group, is black stroke & fill.

  //Note: my_svg.line(100, 50,  50, 100); // only generates <line x1="100" y1="50" x2="50" y2="100"/>
  // which doesn't show anything!
  // Because is lacking stroke-width and stroke (color), whereas
  // <line stroke-width="5" stroke="red" x1="100" y1="300" x2="300" y2="100"/>

  poly_path_point p0(100, 200);
  std::cout << " poly_path_point p0(100, 200); " << p0 << std::endl; // Outputs: (100, 200)

  path_element my_path;
  my_path.M(1, 2).L(3, 4).L(5, 6).z();
  //path_element& my_path2 = my_svg.gs(0).path();
  //path_element& my_path0 = g0.path();
  // my_path.M(1, 2).L(3, 6).L(5, 4).z(); // <path d="M1,2 L3,4 L5,6 Z " />
  //path_element& path = g0.path(); // OK

  std::cout << "After adding path_element my_svg.document_size() " << my_svg.document_size() << std::endl; // == 2

  polygon_element& pp = g0.polygon(); // 'empty' polygon.
  polygon_element& ppp(pp); // copy constructor.
  pp.P(400, 500); // Add a single vertex.
  pp.P(200, 300).P(100, 400).P(100, 100); // add several more vertices.
  std::cout << "After adding  polygon my_svg.document_size() " << my_svg.document_size() << std::endl; // 2

  my_svg.triangle(400, 20, 300, 100, 450, 50, false);
  std::cout << "After adding  triangle my_svg.document_size() " << my_svg.document_size() << std::endl; // 3
  my_svg.triangle(200, 20, 350, 100, 250, 100);
  std::cout << "After adding  triangle my_svg.document_size() " << my_svg.document_size() << std::endl; // 4
  my_svg.rhombus(10, 500, 10, 550, 450, 550, 300, 500, true);
  my_svg.pentagon(100, 10, 120, 10, 130, 30, 110, 50, 110, 30, true);
  my_svg.hexagon(300, 10, 420, 10, 330, 130, 350, 150, 210, 30, 250, 60, true);

  array<const poly_path_point, 12> hexagram =
  { // six point star 6/2 ("Star of David")
    poly_path_point(0., -4.), poly_path_point(+1., -2.),
    poly_path_point(+3., -2.), poly_path_point(+2, 0),
    poly_path_point(+3, +2),  poly_path_point(+1, +2),
    poly_path_point(0, 4), poly_path_point(-1, +2),
    poly_path_point(-3, +2), poly_path_point(-2, 0),
    poly_path_point(-3, -2), poly_path_point(-1, -2)
  };

  std::vector<poly_path_point> star;
  poly_path_point offset(300., 300);
  poly_path_point size(50., 50);
  for (array<const poly_path_point, 12>::iterator i = hexagram.begin(); i != hexagram.end(); ++i)
  {
    poly_path_point p = (*i);
    p.write(std::cout);
    p.x *= size.x;
    p.y *= size.y;
    p.x += offset.x;
    p.y += offset.y;
    star.push_back(p);
  }
  std::cout << std::endl;

  for (std::vector<poly_path_point>::iterator i = star.begin(); i != star.end(); ++i)
  {
    poly_path_point p = (*i);
    p.write(std::cout);
  }
  std::cout << std::endl;

  g0.push_back(new polygon_element(star, true));

  std::vector<poly_path_point> my_p;
  my_p.push_back(poly_path_point(10, 20));
  my_p.push_back(poly_path_point(70, 80));
  my_p.push_back(poly_path_point(50, 60));
  my_p.push_back(poly_path_point(90, 100));
  my_p.push_back(poly_path_point(30, 40));
  my_p.push_back(poly_path_point(110, 120));

  for (std::vector<poly_path_point>::iterator i = my_p.begin(); i != my_p.end(); ++i)
  {
    poly_path_point p = (*i);
    p.write(std::cout);
  }
  std::cout << std::endl;

  g0.push_back(new polygon_element(my_p, true));
  my_svg.polygon(my_p, true); // 'filled' polygon.

  //polygon_element& my_poly = my_svg.polygon(10, 20, true);
  //my_poly.p(100, 100); // just one point
  //my_poly.p(200, 100).p(300, 200).p(400,300); // <polygon points=" 11,22 33,44 55,66"/>
  //my_svg.add_g_element(); // Add 2nd new element,

  // This line aborts:
  //polyline_element& pl = g1.polyline(); // 'empty' line.
  //  my_poly.P(123.456789, 123.456789); // just one point
  //  my_poly.P(123.456789, 123.456789, 12., 34.); // just one point
  //pl.P(234.5679, 123.456789).P(300.5678, 200.12345).P(400,400); // <polyline points=" 100,100 200,100 300,100 400,0" fill = "none"/>

  // 5-point Star (values from from http://www.w3.org/TR/SVG/shapes.html#PolygonElement)
  std::vector<poly_path_point> star_5;
  // Push_back the 10 path points.
  star_5.push_back(poly_path_point(350, 75));
  star_5.push_back(poly_path_point(379, 161));
  star_5.push_back(poly_path_point(469, 161));
  star_5.push_back(poly_path_point(397, 215));
  star_5.push_back(poly_path_point(423, 301));
  star_5.push_back(poly_path_point(350, 250));
  star_5.push_back(poly_path_point(277, 301));
  star_5.push_back(poly_path_point(303, 215));
  star_5.push_back(poly_path_point(231, 161));
  star_5.push_back(poly_path_point(321, 161));

  //g0.polygon(star_5, true);
  polygon_element pstar(star_5, true); // OK - construct from vector
  g0.push_back(new polygon_element(star_5, true));

  // Hexagon

  //std::cout.precision(17);
  //double pi = 3.14159265359;
  //std::cout <<  cos(pi/6) << std::endl; // == cos(30) == 0.86602540378442139;

  array<const poly_path_point, 6> hexup =
  { // Regular point-up hexagon!
    poly_path_point(0, -1),
    poly_path_point(0.86602540378442139, -0.5),
    poly_path_point(0.86602540378442139, +0.5),
    poly_path_point(0., +1.),
    poly_path_point(-0.86602540378442139, +0.5),
    poly_path_point(-0.86602540378442139, -0.5)
  };

  for (array<const poly_path_point, 6>::iterator i = hexup.begin(); i != hexup.end(); ++i)
  {
    std::cout << (*i) << std::endl;
  }
  std::cout << std::endl;

  //g0.push_back(new polygon_element(hex, true) );

  {
    symb(hexup, poly_path_point(75., 450.), poly_path_point(10., 10.), g0); // OK construct from array of consts
  }

  std::cout << "my_svg.document_size() " << my_svg.document_size() << std::endl; // 8

  // Adding a 2nd new group element.
  g_element& g1 = my_svg.gs(1); // so its index is now one.

  g1.id("element 1"); // Add an ID
  std::cout << "g1.id()  " << g1.id() << std::endl; // Outputs: element 1
  std::cout << "my_svg.document_size() " << my_svg.document_size() << std::endl; // 8 ???

  std::cout << "my_svg.add_g_element().size() " << my_svg.add_g_element().size() << std::endl; // 0
  // Nothing added to group yet.

  my_svg.write("demo_svg.svg");

   return boost::exit_success;
  }
  catch (std::exception& ex)
  {
    std::cout << "svg_plot exception " << ex.what() << std::endl;
   return boost::exit_failure;
  }
} // int main()

/*



*/

