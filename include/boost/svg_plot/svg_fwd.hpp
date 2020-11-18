/*!
  \file svg_fwd.hpp
  \author Jacob Voytko & Paul A. Bristow

  \brief Forward declarations of SVG classes and functions.
  \details Doxygen descriptions are in the corresponding .hpp files.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2012

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_FWD_HPP
#define BOOST_SVG_FWD_HPP

namespace boost
{
namespace svg
{
//! @cond DETAIL

// svg_color.hpp contains definitions.
class svg_color;
enum svg_color_constant;
void constant_to_rgb(svg_color_constant c, unsigned char &r,
                     unsigned char &g, unsigned char &b);
std::ostream& operator<< (std::ostream&, const svg_color&);
enum svg_color_constant;

// svg_style.hpp contains definitions.

enum point_shape;
class plot_point_style;
class plot_line_style;
class text_style;

class svg_style;
svg_color stroke_color();
svg_color fill_color();
unsigned int stroke_width();
bool fill_on();
bool stroke_on();
int width_on();
svg_style& stroke_color(const svg_color& col);
svg_style& fill_color(const svg_color& col);
svg_style& stroke_width(unsigned int width);

//void write(std::ostream& rhs);
// \detail\svg_style.hpp

using boost::svg::detail::plot_doc_structure;

// enum plot_doc_structure; // ISO C++ forbids forward references to 'enum' types

// svg.hpp
// Chainable set and get member functions for:
// Image size x and y, and both.
svg& x_size(unsigned int x);
unsigned int x_size();
svg& y_size(unsigned int y);
unsigned int y_size();
svg& size(unsigned int x, unsigned int y);

// Information about the image file: description, title, copyright and date.
svg& description(const std::string d);
std::string& description();
svg& document_title(const std::string d);
const std::string document_title();
svg& author(const std::string);
const std::string author();

svg& copyright_holder(const std::string d);
const std::string copyright_holder();
svg& copyright_date(const std::string d);
const std::string copyright_date();
svg& coord_precison(int);
int coord_precison();
svg& license(std::string repro, std::string distrib, std::string attrib, std::string commercial);
svg& license(bool);
bool is_license();
svg& boost_license(bool);
bool boost_license();

// Write information about svg shapes:
svg& line(double x1, double y1, double x2, double y2);
svg& rect(double x1, double y1, double x2, double y2);
svg& circle(double x, double y, unsigned int radius);
svg& ellipse(double rx, double ry, double cx, double cy);

svg& text(double x, double y, const std::string& text, int text_size,
const std::string& font,
const std::string& style, const std::string& weight,
const std::string& stretch, const std::string& decoration, align_style align,  int rotate);

// Write image out to ostream and file:
svg& write(std::ostream& s_out);
svg& write(const std::string& file);

// Load a stylesheet into string css from an input file.
svg& load_stylesheet(const std::string& input);

// svg_style.hpp
enum point_shape; // {none, round, square, point...};
class plot_point_style;
class plot_line_style;
class svg_style;
svg_color fill_color();
svg_color stroke_color();
unsigned int stroke_width();
svg_style& fill_color(const svg_color& col);
svg_style& stroke_color(const svg_color& col);
svg_style& stroke_width(unsigned int width);
// void svg_style::write(std::ostream& rhs);

// svg_tag.hpp

class svg_element; //svg_element is base class for
  // rect_element, circle_element, line_element, text_element, path_element, clip_path_element, g_element
// Set and get member functions.
svg_style& style();
//const svg_style& style();
void id(const std::string& id);
std::string id( );
void clip_id(const std::string& id);
enum class align_style; // left_align, right_align, center_align
enum rotate_style; // Rotation in degrees from horizontal.
//    horizontal = 0, // normal left to right.
//    upward = -90, // vertical writing up.
//    downward = 90,  // vertical writing down.
//    upsidedown = 180 //

class text_element; // text with position, size, font, (& styles) & orientation.
class rect_element; // clipping path restricts the region to which paint can be applied.
class circle_element; // Represents a single circle.
class ellipse_element; // Represents a single ellipse.
class line_element; // Represents a single line.
class clip_path_element; // Restricts the region to which paint can be applied.
struct path_point; // Base class for m_path, z_path, q_path, h_path, v_path, c_path, s_path & point_path.
struct point_path; // for polyline & polygon
class polygon_element; // closed shape consisting of a set of connected straight line segments.
class polyline_element; // a set of connected straight line segments.
class path_element; // d= moveto, lineto...
struct m_path; // moveto coordinates (x, y).
struct l_path; // lineto coordinates (x, y).
struct z_path; // z indicates a closepath.
struct h_path; // Draws a horizontal line from the current point (cpx, cpy) to (x, cpy).
struct v_path; // Draws a vertical line from the current point (cpx, cpy) to (cpx, y).
struct c_path; // Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1).
struct q_path; // Draws a quadratic Bezier curve from the current point to (x,y).
struct s_path; // Draws a cubic Bezier curve from the current point to (x,y).
struct t_path; // Draws a quadratic Bezier curve from the current point to (x,y).
struct a_path; // Draws a elliptical arc from the current point to (x,y).
class g_element; // 'g' element is a container element for grouping together related graphics elements.

// Note don't try to use accent in Bezier! - causes failure in autodoc .xml files - not UTF-8.

// svg_1d_plot.hpp
struct svg_plot_series;
class svg_1d_plot;

//! \endcond // DETAIL

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_FWD_HPP


