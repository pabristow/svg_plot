/*!
  \file
  \brief Styles for SVG specifying font, sizes, shape, color etc for text, values, lines, axes etc.
  \details SVG style information is fill, stroke, width, line & bezier curve.

   This module provides \code struct plot_point_style & struct plot_line_style \endcode
   and class @c svg_style holding the styles.
   \sa http://www.w3.org/TR/SVG11/styling.html
*/

// svg_style.hpp
// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2009, 2013, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_STYLE_HPP
#define BOOST_SVG_SVG_STYLE_HPP

//#define BOOST_SVG_STYLE_DIAGNOSTICS for diagnostic output.

#include <boost/svg_plot/svg_color.hpp> 
// using boost::svg::svg_color
#include <boost/svg_plot/svg_style.hpp> // Font and other styles. // For not_a_text_style etc
#include <boost/svg_plot/detail/svg_style_detail.hpp> // For enum plot_doc_structure.

#include <iostream>
// using std::ostream;
#include <sstream>
// using std::stringstream;
#include <limits>
// using numeric_limits;

namespace boost
{
namespace svg
{
  typedef double fp_type; 
  //!< Allows a switch between using @c double or @c float to hold
  //!< many floating-point plotable data values as either @c double or @c float.
  //!< (32-bit has sufficient precision for data plots, so using @c float might be faster and/or take less space,
  //!< but with a smaller range of @c (std::numeric_limits<>::max)() to @c (std::numeric_limits<>::min)(), 
  //! about 10^38 compared to 64-bit 10^308).

  static const fp_type aspect_ratio = 0.6;  //!< aspect_ratio is a guess at average height to width of font (0.55 to 0.7?).
  //!< used to estimate the svg length of a title or header string from the font size (height and width of EM - capital M).
  //!< This can only be quite approximate as varies on type of font and variants (upper or lower, narrow or bold).
  //!< and the mix of characters widths (unless monospace font).
  //!< See https://www.w3.org/TR/SVG/text.html#GlyphsMetrics
  //!< https://www.w3.org/TR/SVG/text.html#InterfaceSVGTextContentElement
  //! Can also use textLength to coerce into an estimated length.

  /*! Default font chosen is a Unicode font like ['Lucida Sans Unicode] that
   has the best chance of ['symbols] being rendered corrrectly.
   Used for title, legend, axes ... unless overridden by an explicit font specification.
  */
  const static char* default_font_family("Lucida Sans Unicode"); // TODO make sure used throughout.
    // should be default_font_family

// Forward declarations of classes in svg_style.hpp
class svg_style; // Holds the basic stroke, fill colors and width, and their switches.
class text_style; // Text and tspan element's font family, size ...
class value_style; // data-series point value information, text, color, uncertainty & df, orientation, and textLength.
class plot_point_style; // Shape, color, (optional value & uncertainty) of data-point-markers.
class plot_line_style; // Style of line joining data-series values.
class axis_line_style; // Style of the x and/or y axes lines. But NOT the ticks and value-labels.
class ticks_labels_style; // Style of the x and y axes ticks, grids and tick value-labels.
class box_style; //  Box colors, size and border switches.
class histogram_style;  // Options for histograms.
class bar_style;  // Style of bars.

//! \enum rotate_style Rotation of text (in degrees clockwise from horizontal == 0). (but rotation stored an int degrees).
//! int rotation = static_cast<int>(horizontal) which is really ugly.
//! (Making an enum class caused major clutter.
enum rotate_style
{
  // Also need a no_rotate, = -1;
  horizontal = 0, //!< normal horizontal left to right, centered.
  slopeup = -30, //!< slope up.
  uphill = -45, //!< 45 steep up.
  steepup = -60, //!< up near vertical.
  upward = -90, //!< vertical writing up.
  backup = -135, //!< slope up backwards - upside down!
  leftward= -180, //!< horizontal to left.
  rightward = 360, //!< horizontal to right.
  slopedownhill = 30, //!< 30 gentle slope down.
  downhill = 45, //!< 45 down.
  steepdown = 60, //!<  60 steeply down.
  downward = 90,  //!< vertical writing down.
  backdown = 135, //!< slope down backwards.
  upsidedown = 180 //!< upside down!  (== -180)
};

//! \enum align_style Represents alignment a single @c std::string of chars.
enum class align_style
{ 
  no_align = -1,  // Not aligned.
  left_align, //!< Align text to left. Outputs: \verbatim text-anchor="left" \endverbatim
  right_align, //!< Align text to right. Outputs: \verbatim text-anchor="right" \endverbatim
  center_align //!< Center align text. Outputs: \verbatim text-anchor="middle" \endverbatim
};

std::ostream& operator<< (std::ostream& os, const align_style& al)
{ //! Outputs: alignment type (useful for diagnosis).
  //! Example: 
  //! \code align_style al = align_style::left_align;  std::cout << "Align is " << al << std::endl; \endcode
  //! Outputs: Align is left 
  if (al == align_style::left_align)
  {
    os << "left";
  }
  else if (al == align_style::center_align)
  {
    os << "center";
  }
  else if (al == align_style::right_align)
  {
    os << "right";
  }
  else if (al == align_style::no_align)
  {
    os << "not aligned";
  }
  else os << "??? aligned";
  // os << std::endl; // User should provide any suffix.
  return os;
} //   std::ostream& operator<< (std::ostream& os, align_style al)

//! Outputs: rotation style as words from horizontal(useful for diagnosis).
//! Example:  \code std::cout << "x_values_rotation " << show_rotation(plot.x_values_rotation()) << std::endl; \endcode
//! Outputs: \verbatim rotation is steepup (-60) \endverbatim
std::string show_rotation(int rot)
{ 
  if (rot == 0) {return "horizontal (0)"; }
  else if (rot == -30) { return  "slopeup (-30)"; }
  else if (rot == -45) { return  "uphill (-45)"; }
  else if (rot == -60) { return  "steepup (-60)"; }
  else if (rot == -90) { return  "upward (-90)"; }
  else if (rot == -135) { return  "backup (-135)"; }
  else if (rot == -180) { return  "leftward (-180)"; }
  else if (rot == 360) { return  "rightward (360)"; }
  else if (rot == 30) { return  "slopedownhill (30)"; }
  else if (rot == 45) { return  "downhill (45)"; }
  else if (rot == 60) { return  "steepdown (60)"; }
  else if (rot == 90) { return  "downward (90)"; }
  else if (rot == 135) { return  "backdown (135)"; }
  else if (rot == 180) { return  "upsidedown (180)"; }
  else if (rot < 0) { return  "undefined rotate!"; }
  return "";
} // 

//std::ostream& operator<< (std::ostream& os, rotate_style rot)
//{ //! Outputs: rotation style as words from horizontal (useful for diagnosis).
//  //! Example: 
//  //! \code int rot = static_cast<int>(horizontal);
//  //!    std::cout << "rotation = " << rot << std::endl;
//  //! \endcode
//  //! 
//  //! 
//  // Outputs: \verbatim rot is uphill (-45) \endverbatim
//  if (rot == 0) { os << "horizontal (0)"; }
//  else if (rot == -30) { os << "slopeup (-30)"; }
//  else if (rot == -45) { os << "uphill (-45)"; }
//  else if (rot == -60) { os << "steepup (-60)"; }
//  else if (rot == -90) { os << "upward (-90)"; }
//  else if (rot == -135) { os << "backup (-135)"; }
//  else if (rot == -180) { os << "leftward (-180)"; }
//  else if (rot == 360) { os << "rightward (360)"; }
//  else if (rot == 30) { os << "slopedownhill (30)"; }
//  else if (rot == 45) { os << "downhill (45)"; }
//  else if (rot == 60) { os << "steepdown (60)"; }
//  else if (rot == 90) { os << "downward (90)"; }
//  else if (rot == 135) { os << "backdown (135)"; }
//  else if (rot == 180) { os << "upsidedown (180)"; }
//  else if (rot < 0) { os << "undefined rotate!"; }
//  // else { os << static_cast<int>(rot) << "?? rotate!"; }  // Should not happen.
//  ;
//  return os;
//  } // std::ostream& operator<< (std::ostream& os, int rot)

//! The place for ticks value-labels on the X and/or Y-axes.
enum place
{
  left_side = -1,
  on_axis = 0,
  right_side = +1,
  bottom_side = -1,
  top_side = +1,
};

//! Remove unwanted sign and leading zero(s) in exponent in floating-point decimal digit values
//! to reduce their length.
const std::string strip_e0s(std::string s);

// Estimate length of string in SVG units when appears.
double string_svg_length(const std::string& s, const text_style& style);
/*!
 This is the style information for any group (g) tag.
 This could be expanded to include more data from the SVG standard.

 There are some strange effects for text on some browsers
 (Firefox especially) when only stroke is specified.
 fill is interpreted as black, and the font outline is fuzzy and bolder.
\verbatim
   <g id="title" stroke="rgb(255,0,0)"> ... is red border and black fill.
   (because created as a graphic not a builtin font?)
   <g id="title" fill="rgb(255,0,0)"> ... is red sharp font.
   <g id="title" stroke="rgb(255,0,0)" fill="rgb(255,0,0)"> red and red fill also fuzzy.
   So for text, only specific the fill unless a different outline is really wanted.
   Defaults for text provide a built-in glyph, for example for title:
   <g id="title">
     <text x="250" y="36" text-anchor="middle" font-size="18" font-family="Verdana">
       Plot of data
     </text>
   </g>
   and this is not a graphic.
 \endverbatim
*/

class svg_style
{ //! \class boost::svg::svg_style Holds the basic SVG stroke and fill colors, and width, and switches to control if in use, or not.
  friend std::ostream& operator<< (std::ostream&, const svg_style&);
  // Used for diagnostic output of all values of style and state of switches.

  // Access by set and get member functions below.
  // Data member variables names end with _ to permit use of names for set & get member functions.
  svg_color stroke_; //!< Color of SVG stroke (line or outline).
  svg_color fill_; //!< Color of SVG fill.
  double width_; //!< Width of line.  Only valid if > 0 & width_on_ == true
  // Switches:
  bool stroke_on_; //!< true if stroke is to be specified.
  bool fill_on_; //!< true if fill to be specified.
  bool width_on_; //!< true if width to be specified.

public:
  // Constructors declarations:
  svg_style(); // Default style.
  svg_style(const svg_color& stroke, const svg_color& fill, unsigned int width);

  // Set svg_style member functions to set fill color and stroke color & width , and switches.
  svg_style& stroke_color(const svg_color& col) ;
  svg_style& fill_color(const svg_color& col);
  svg_style& stroke_width(double width);
  // Switches:
  svg_style& fill_on(bool is);
  svg_style& stroke_on(bool is);
  svg_style& width_on(bool is);

  // Get svg_style member functions to return fill color, stroke color & width, and state of switches.
  svg_color fill_color() const;
  svg_color stroke_color() const;
  double stroke_width() const;
  bool fill_on() const;
  bool stroke_on() const;
  bool width_on() const;

  void write(std::ostream& os); // Output svg_style to file or stream.

  // Comparison operators (useful for testing at least).
  bool operator==(svg_style& s);
  bool operator!=(svg_style& s);
}; // class svg_style

// class svg Definitions.
// Constructors.

  svg_style::svg_style(const svg_color& stroke, const svg_color& fill, unsigned int width)
  :
  stroke_(stroke), fill_(fill), width_(width),
  stroke_on_(false), fill_on_(false), width_on_(false)
  { //! Construct svg_style with specified fill and stroke colors, and width.
  }

  svg_style::svg_style()
  : //! Default svg_style has everything off.
  stroke_(svg_color(0, 0, 0)), //! Stroke default is black.
  fill_(blank), //! No fill color.
  width_(0), //! No width specified.
  stroke_on_(false), fill_on_(false), width_on_(false) //! All switches off.
  { // Default constructor initialises all data.
  }

  // Member Functions definitions.
  svg_color svg_style::fill_color() const
  { //! \return SVG fill color.
    return svg_color(fill_);
  }

  svg_color svg_style::stroke_color() const
  { //! \return SVG stroke color.
    //! See svg_color.hpp for @c enum @c svg_color_constant for named colors.
    //! \sa svg_color.hpp
    return svg_color(stroke_);
  }

  double svg_style::stroke_width() const
  { //! \return SVG stroke width.
    return width_;
  }

  svg_style& svg_style::fill_on(bool is)
  { //! Set true if SVG fill is wanted.
    fill_on_ = is;
    return *this; //! \returns svg_style& to make chainable.
  }

  bool svg_style::fill_on() const
  { //! \return true if fill wanted.
    return fill_on_;
  }

  bool svg_style::stroke_on() const
  { //! \return true if SVG stroke is on.
    return stroke_on_;
  }

  svg_style& svg_style::stroke_on(bool is)
  { //! Set true if SVG stroke is wanted.
    stroke_on_ = is;
    return *this; //! \returns svg_style& to make chainable.
  }

  bool svg_style::width_on() const
  { //! \return true if to use SVG stroke width.
    return width_on_;
  }

  svg_style& svg_style::width_on(bool is)
  { //! Set true to use SVG stroke width.
    width_on_ = is;
    return *this; //! \return svg_style& to make chainable.
  }

  // Set svg_style member functions to set fill, stroke & width.
  svg_style& svg_style::stroke_color(const svg_color& col)
  { //! Set stroke color (and set stroke on).
    stroke_ = col;
    stroke_on_ = true; // Assume want a stroke if color is set.
    return *this;  //! \return svg_style& to make chainable.
  }

  svg_style& svg_style::fill_color(const svg_color& col)
  { //! Set fill color (and set fill on true, unless color is blank).
    fill_ = col;
    fill_on_ = ! is_blank(col); // If blank fill is true or "none".
    return *this; //! \return svg_style& to make chainable.
  }

  svg_style& svg_style::stroke_width(double width)
  { //! Set stroke width (and set width on).
      width_ = width;
      width_on_ = ((width > 0) ? true : false);
      return *this; //! \return svg_style& to make chainable.
  }

  bool svg_style::operator==(svg_style& s)
  { //! Compare svg_styles.
     return (s.fill_color() == fill_)
       && (s.stroke_color() == stroke_)
       && (s.stroke_width() == width_)
       && (s.fill_on() == fill_on_)
       && (s.stroke_on() == stroke_on_)
       && (s.width_on() == width_on_);
   }

  bool svg_style::operator!=(svg_style& s)
   {//! Compare svg_styles (for inequality).
     return (s.fill_color() != fill_)
       || (s.stroke_color() != stroke_)
       || (s.stroke_width() != width_)
       || (s.fill_on() != fill_on_)
       || (s.stroke_on() != stroke_on_)
       || (s.width_on() != width_on_);
   }

  std::ostream& operator<< (std::ostream& os, const svg_style& s)
  {  /*! Output a @c std::string description of a @c svg_style.
         Usage: svg_style my_svg_style; std::cout << my_svg_style << std::endl;
         Outputs for default not_a_style, for example:
         \code
           svg_style(RGB(0,0,0), RGB(0,0,0), 0, no fill, no stroke, no width)
         \endcode
         or a style that has been set to black with a green fill
         \code
           svg_style(RGB(255,0,0), RGB(0,128,0), 2, fill_on, stroke_on, width_on)
         \endcode
     */
      os << "svg_style("
         << s.stroke_ << ", "
         << s.fill_ << ", "
         << s.width_ << ", " // 
         << ((s.stroke_on_) ? "stroke_on, " : "no stroke, ")
         << ((s.fill_on_) ? "fill_on, " : "no fill, ")
         << ((s.width_on_) ? "width_on)" : "no width)");
    return os;
  } // std::ostream& operator<<

  void svg_style::write(std::ostream& os)
  { //! \brief Write any stroke, fill colors and/or width info to SVG XML document.
   /*! \details Example output: \<g id="yMinorTicks" stroke="rgb(0,0,0)" stroke-width="1"\>
   */
    if(stroke_on_)
    { // (Note: start with space but no terminating space).
        os << " stroke=\"";
        stroke_.write(os);
        os << "\"";
    }
    if(fill_on_) //  && (fill_ != blank))
    { // Don't add fill info if color is blank.
        os << " fill=\"";
        fill_.write(os);
        os << "\"";
    }
    if(width_on_ && (width_ > 0))
    { // We never want a 0 (or <0) width output?
        os << " stroke-width=\""
            << width_
            << "\"";
    }
 } // void svg_style::write(std::ostream& os)

  // End of svg_style definitions. /////////////////////////////////////////////

 /*! \class boost::svg::text_style
     \brief Font size, font family, font weight, font style, font stretch & font decoration, and text_length.
     \details
     Font size, font family, font weight, font style, font stretch & font decoration, and text_length.\n
     text font-family (for example: "Lucida Sans Unicode", "arial", Times New Roman" ...).
     Available fonts depend on the program rendering the SVG XML, usually a browser.
     The default font (usually "Lucida Sans Unicode") is used
     if a renderer (in a browser or a converter-to-PDF like RenderX)
     does not provide the font specified.
     A Unicode font has a better chance of providing Unicode symbols, for example, specified as @c \&\#x221E;.
     Symbols are used to show data-points and most shapes use Unicode.
     These fonts are probably usable:
     \code
       "arial", "impact", "courier", "lucida console",  "Lucida Sans Unicode", "Verdana", "calibri", "century",
       "lucida calligraphy", "tahoma", "vivaldi", "informal roman", "lucida handwriting", "lucida bright", "helvetica"
     \endcode

     http://www.fileformat.info/info/unicode/font/index.htm provdes a fuller listing of support by many fonts;
     those awarded 4 or 5 stars are probably most useful.

     text_length Estimate of SVG length of text used to force compress or expand to fit exactly into this width.\n 
     https://www.w3.org/TR/css-fonts-3/#font-stretch-prop 
     */

class text_style
{
  friend std::ostream& operator<< (std::ostream&, const text_style&);
  // Output as text, for example: legend_title_style text_style(14, "Lucida Sans Unicode", "", "normal", "", "", 0)
  friend bool operator== (const text_style&, const text_style&);
  friend bool operator!= (const text_style&, const text_style&);

  public: // 
  int font_size_; //!< Font size (SVG units, default pixels).
  std::string font_family_; //!< Font family, examples: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode".
  std::string weight_; //!< Font weight examples: "bold", "normal".
  std::string font_style_; //!< Font style, examples: normal | bold | italic | oblique.
  std::string stretch_; //!< Font stretch, examples: normal | wider | narrower. (Not supported by all browsers).
  std::string decoration_; //!< Font decoration, examples: "underline" | "overline" | "line-through".
  double text_length_; //!< Estimate of SVG length of text used to force compress or expand into exactly this width. \n
  //!< Only actually used if text_length_ > 0.

  //! Default Constructor initializes all class member data to default values.
  text_style( 
    int font_size = 0, //!< Default font size (12 pixels).  NOT const because it might be changed during (re-)sizing.
    const std::string& font = "", //!< Font (family) name. Examples: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode"...
    const std::string& weight = "", //!< Font weight. Examples: "bold", "normal".
    const std::string& style = "", //!< Font style. Examples: normal | bold | italic | oblique.
    const std::string& stretch = "", //!< Font stretch. Examples: normal | wider | narrower ...
    // ultra-condensed, extra-condensed, condensed, semi-condensed, normal, semi-expanded, expanded, extra-expanded, ultra-expanded
    const std::string& decoration = "", //!< Font decoration examples: "underline" | "overline" | "line-through".
    double text_length = 0); //!< Estimated length of text string. Default = 0 means do not attempt to compress or expand to fit to length.

  text_style(const text_style & rhs); // Copy constructor.  NOT const because can be changed during (re-)sizing.

  // Set and get functions are being replaced by direct access to the now public class members data items, shown by ending with an underscore.
  // text_style Setters.
  text_style& font_size(int i);
  text_style& font_family(const std::string& s);
  text_style& font_weight(const std::string& s);
  text_style& font_style(const std::string& s);
  text_style& font_stretch(const std::string& s);
  text_style& font_decoration(const std::string& s);
  // text_style& font_variant(const std::string& s); // Not implemented, nor are others.
  text_style& text_length(double); // Force renderer to contract or expand to this length.

  // text_style Getters.
  int font_size() const;
  const std::string& font_family() const;
  const std::string& font_weight() const;
  const std::string& font_style() const;
  const std::string& font_stretch() const;
  const std::string& font_decoration() const;
  double text_length() const;

  // Comparison operators useful for testing at least.
  bool operator==(const text_style& ts);
  bool operator!=(const text_style& ts);
  // bool operator==(const text_style& lhs, const text_style& rhs);
  text_style& operator=(const text_style& rhs);

  void write(std::ostream&); // Output text_element SVG.

}; //   class text_style

// class text_style function *Definitions*.

//  3.7.Shorthand font property: the font property https://www.w3.org/TR/css-fonts-3/#propdef-font 
// The font property is, except as described below, a shorthand property for setting
// font-style, font-variant, font-weight, font-stretch, font-size, line-height, font-family
// at the same place in the stylesheet.

text_style::text_style( //!< Constructor to allow all text style font parameters 
  //! (font size, family, bold, italic, condensed, underline ...) to be set.
  int size, //!< Font size.   ( \sa https://www.w3.org/TR/css-fonts-3/#propdef-font-size).
  // https://www.w3.org/TR/css-fonts-3/#propdef-font-size-adjust  not implemented)
  const std::string& font, //!< Default for browser is sans with Firefox & IE but serif with Opera.
  const std::string& weight, //!< font weight "normal" | "bold" | "light" (\sa https://www.w3.org/TR/css-fonts-3/#propdef-font-weight) 
  const std::string& style, //!< font-style: normal | italic | oblique (\sa https://www.w3.org/TR/css-fonts-3/#propdef-font-style) 
  const std::string& stretch, //!< font-stretch: normal | condensed | expanded (\sa https://www.w3.org/TR/css-fonts-3/#font-stretch-prop)
  const std::string& decoration, //!< Default is No decoration.  underline | overline | strike-through (or moe than one)
  //!< text-decoration="line-through" "underline" "overline" 
  //!< \sa https://www.w3.org/TR/SVG/text.html#TextDecorationProperties
  double text_length //!< Force compression or expansion to this length (if text_length >).
  ) 
  : // Constructor.
  font_size_(size),
  font_family_(font),
  weight_(weight),
  font_style_(style),
  stretch_(stretch),
  decoration_(decoration),
  text_length_(text_length)
  { // text_style default constructor, defines defaults for all members.
  }

//! text_style Copy constructor (copies all member data items).
text_style::text_style(const text_style& rhs)
  :
  font_size_(rhs.font_size_),
  font_family_(rhs.font_family_),
  weight_(rhs.weight_),
  font_style_(rhs.font_style_),
  stretch_(rhs.stretch_),
  decoration_(rhs.decoration_),
  text_length_(rhs.text_length_)
{
} //

text_style& boost::svg::text_style::operator=(const text_style& rhs)
{ //! Assignment @c operator= (copies all member data items).
  font_size_ = rhs.font_size_;
  font_family_ = rhs.font_family_;
  weight_ = rhs.weight_;
  font_style_ = rhs.font_style_;
  stretch_ = rhs.stretch_;
  decoration_ = rhs.decoration_;
  text_length_ = rhs.text_length_;
  return *this; //! \return text_style& to make chainable.
}

  // Set and get text_style functions.

  text_style& text_style::font_size(int i)
  { //! Set font size (svg units usually pixels) default 10.
    font_size_ = i;
    return *this;
    //! \return reference to @c text_style& to make chainable.
  }

  int text_style::font_size() const
  { //! \return  text_style's font size (svg units, usually pixels).
    return font_size_;
  }

  text_style& text_style::font_family(const std::string& s)
  { //! Set font family, for example: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode".
    /*!
    \details Default for browser is sans with Firefox & IE but serif with Opera.\n
      See also browser conformance test at\n
      http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-01-t.svg\n
      which tests three styles of font, serif, sans-serif and mono-spaced.\n
      \verbatim
        <text font-family="Georgia, 'Minion Web', 'Times New Roman', Times, 'MS PMincho', Heisei-Mincho, serif " x="20" y="80">A serifed face</text>

        <text font-family="Arial, 'Arial Unicode', 'Myriad Web', Geneva, 'Lucida Sans Unicode', 'MS PGothic', Osaka, sans-serif " x="20" y="160">A sans-serif face</text>

        <text font-family="'Lucida Console', 'Courier New', Courier, Monaco, 'MS Gothic', Osaka-Mono, monospace" x="20" y="240">A mono (iW) face</text>
      \endverbatim
    */
    font_family_ = s;
    return *this; //! \return reference to text_style to make chainable.
  }

  const std::string& text_style::font_family() const
  { //! \return @c text_style's font family as @c std::string, for example: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode"..
    return font_family_;
  }

  text_style& text_style::font_style(const std::string& s)
  { /*! Set font style. Example: \code my_text_style.font_style("italic");\n \endcode
    */
    font_style_ = s;
    return *this; //! \return reference to text_style to make chainable.
  }

  const std::string& text_style::font_style() const
  { //! \return  font style, default normal. 
    /*! font-style: normal | bold | italic | oblique.
    Example: "normal" is default font style.
    */
    return font_style_;
  }

   text_style& text_style::font_weight(const std::string& s)
  { //! svg font-weight: "normal" | "bold" | "bolder" | "lighter" | "100" | "200" .. "900".\n
    //! Examples:  \code .font_weight("bold");  \endcode
    //! Only two weights, "bold", "normal", are supported by Firefox, Opera, Inkscape...
    weight_ = s;
    return *this;
    //! \return reference to text_style to make chainable.
  }

   const std::string& text_style::font_weight() const
   {  /*! Set font weight.
      Example: \code  my_text_style.font_style("bold");\n \endcode
      */
     return weight_;
   }

  text_style& text_style::font_stretch(const std::string& s)
  {  //! font-stretch:"normal" | "wider" | "narrower"
     //! Examples: \code .font_stretch("wider") \endcode
     //! \note Implementation by browsers varies.

    stretch_ = s;
    return *this; //! \return reference to text_style to make chainable.
  }

  const std::string& text_style::font_stretch() const
  { //! \return font stretch, for example: "normal" | "wider" | "narrower".
    return stretch_;
  }

  text_style& text_style::font_decoration(const std::string& s)
  { /*! Set font decoration: "underline" | "overline" | "line-through" ...
      http://www.croczilla.com/~alex/conformance_suite/svg/text-deco-01-b.svg
      tests line-through and underline.
      But implementation varies.
      Example @c .font_decoration("underline");
    */
    decoration_ = s;
    return *this; //! \return reference to text_style to make chainable.
  }

  const std::string& text_style::font_decoration() const
  { //! \return  font decoration.
    return decoration_;
  }

  // Font-variant-position normal, sub and super
  // https://www.w3.org/TR/css-fonts-3/#font-variant-position-prop
  // not implemented, and poorly supported by browsers.

  // baseline-shift   baseline | sub | super | <percentage> | <length> | inherit
  // https://www.w3.org/TR/SVG11/text.html#BaselineAlignmentProperties

  // Baseline shift not implemented yet.
  // http://www.croczilla.com/~alex/conformance_suite/svg/text-align-02-b.svg
  // tests for baseline-shifted text.  This is needed for subscript and superscript,
  // vital for nice display of units like m^2 and chemical formulae like H2O
  // IE (Adobe SVG viewer) and Opera conforms but not Firefox (yet).
  // Can also use Unicode symbols like sub and superscript 1,2,3 to get H2O and m2.

  // lengthAdjust = "spacing|spacingAndGlyphs"
  // Indicates the type of adjustments which the user agent shall make to make the rendered length of the text
  // match the value specified on the 'textLength' attribute.
  // default is spacing (don't squeeze the glyphs)
  // spacingAndGlyphs squeezes both.
  // Not used yet.

  /* https://www.w3.org/TR/SVG11/text.html#FontsTablesBaselines
  textLength = "<length>"
    The author's computation of the total sum of all of the advance values
    that correspond to character data within this element,
    including the advance value on the glyph (horizontal or vertical),
    the effect of properties 'kerning', 'letter-spacing' and 'word-spacing' and
    adjustments due to attributes 'dx' and 'dy' on 'tspan' elements.
    This value is used to calibrate the user agent's own calculations with that of the author.
    The purpose of this attribute is to allow the author to achieve exact alignment,
    in visual rendering order after any bidirectional reordering, for the first and last rendered glyphs
    that correspond to this element; thus, for the last rendered character
    (in visual rendering order after any bidirectional reordering),
    any supplemental inter-character spacing beyond normal glyph advances are ignored (in most cases)
    when the user agent determines the appropriate amount to expand/compress the text string to fit
    within a length of 'textLength'.
    A negative value is an error (see Error processing).
    If the attribute is not specified, the effect is as if the author's computation exactly matched
    the value calculated by the user agent;
    thus, no advance adjustments are made.
    */

  inline text_style & text_style::text_length(double length)
  { //! Set text_length to be rendered from an estimate of length from number of characters in string.
    text_length_ = length;
    return *this; //! \return reference to text_style to make chainable.
  }

  double text_style::text_length() const
  {
    return text_length_; //! \return text_length to be rendered from an estimate of length from number of characters in string.
  }

   // operators needed for testing at least.
  bool text_style::operator==(const text_style& ts)
  { //! Compare text_style for equality (needed for testing).
   bool result = (
     (ts.font_size_ == font_size_)  // NOT const as may be changed during sizing.
     && (ts.font_family_ == font_family_)
     && (ts.stretch_ == stretch_)
     && (ts.font_style_ == font_style_)
     && (ts.weight_ == weight_)
     && (ts.decoration_ == decoration_)
     && (ts.text_length_ == text_length_) // NOT const as may be changed during sizing.
     );
   return result;
  } // operator==

  bool text_style::operator!=(const text_style& ts)
  { //! Compare text_style for inequality (needed for testing).
   bool result = (
     (ts.font_size_ != font_size_)
     || (ts.font_family_ != font_family_)
     || (ts.stretch_ != stretch_)
     || (ts.font_style_ != font_style_)
     || (ts.weight_ != weight_)
     || (ts.decoration_ != decoration_)
     || (ts.text_length_ != text_length_)
     );

   return result;
   ;
  } //  operator!=

  bool operator==(const text_style& lhs, const text_style& rhs)
  { //! Compare two text_style for equality
    //! Note operator== and operator << both needed to use Boost.Test.
    //! (But can be avoided with a macro define).
     return (lhs.font_size_ == rhs.font_size_)
       && (lhs.font_family() == rhs.font_family())
       && (lhs.stretch_ ==  rhs.stretch_)
       && (lhs.font_style_ ==  rhs.font_style_)
       && (lhs.weight_ ==  rhs.weight_)
       && (lhs.decoration_ ==  rhs.decoration_)
       && (lhs.text_length_ == rhs.text_length_);
  } //   bool operator==(const text_style& lhs, const text_style& rhs)

  bool operator!= (const text_style& lhs, const text_style& rhs)
  { //! Compare two text_style for equality.
    //! Note operator== and operator << both needed to use Boost.Test.
    //! (But can be avoided with a macro define).
      return (lhs.font_size_ != rhs.font_size_)
       && (lhs.font_family() != rhs.font_family())
       && (lhs.stretch_ !=  rhs.stretch_)
       && (lhs.font_style_ !=  rhs.font_style_)
       && (lhs.weight_ !=  rhs.weight_)
       && (lhs.decoration_ !=  rhs.decoration_)
       && (lhs.text_length_ != rhs.text_length_);
  } //   bool operator!= (const text_style& lhs, const text_style& rhs)

std::ostream& operator<< (std::ostream& os, const text_style& ts)
{ //! Output a text_style as a text string (mainly useful for diagnostic use).
  // Example: 
  os << "text_style("
    << ts.font_size_ << ", \""
    << ts.font_family_ << "\", \""
    << ts.font_style_ << "\", \"" // italic
    << ts.weight_ // bold
    // Options enabled if implemented by rendering programs.
    << "\", \""
    << ts.stretch_ << "\", \""
    << ts.decoration_ << "\"";
    if (ts.text_length_ > 0)
    { // Only show if a text_length is set.
      os << "\", " << ts.text_length_;
    }
    os << ")";
  /*! \details Example:
     text_style ts(12, "Arial", "italic", "bold", "", "", 0);  std::cout << ts << std::endl;
     Outputs:  text_style(12, "Arial", "italic", "bold", "", "")

     text_style ts(12, "Arial", "italic", "bold", "narrower", "underline", 1000);  std::cout << ts << std::endl;
     Outputs:  text_style(12, "Arial", "italic", "bold", "narrower", "underline", 1000)
   */
  return os; // Make chainable.
} // std::ostream& operator<<

void text_style::write(std::ostream& os) 
{
  if (font_size_ > 0)
  {
    os << " font-size=\"" << font_size_ << "\"";
  }
  if (font_family_ != "")
  {// Example: Arial, Verdana, Times New Roman ... 
    // \verbatim Example output: font-family="serif" \endverbatim
    os << " font-family=\"" << font_family_ << "\"";
  }
  if (weight_.size() != 0)
  { // Example: bold.
    os << " font-weight=\"" << weight_ << "\"";
  }
  if (font_style_.size() != 0)  // or != ""
  { // Example: italic.
    os << " font-style=\"" << font_style_ << "\"";
  }
  if (stretch_.size() != 0)
  { // Example: narrower.
    os << " font-stretch=\"" << stretch_ << "\"";
  }
  if (decoration_.size() != 0)
  {
    os << " text-decoration=\"" << decoration_ << "\"";
  }
  if (text_length() > 0)
  {
    os << " textLength=\"" << text_length_ << "\"";
  }
} //   void svg_style::write(std::ostream& os)

// End of class text_style function *Definitions* separated.

text_style no_text_style; //!< Text style that uses all constructor defaults.
static const text_style not_a_text_style (-1, "","","","","",0); //!< Text style that uses null for all text style features 
// (used to signal no writing to output of SVG style required).

class value_style
{ /*! \class boost::svg::value_style
     \brief data-series point value-label information, text, color, orientation, alignment, (uncertainty & df),
     name ID string, order in sequence, time and date.
     \details For example, to output: 5.123 +- 0.01 (19).\n
     Uncertainty and degrees of freedom estimate.
     Prefix, separator and suffix allow X and Y values to be together on one line, for example\n
     [1.23+- 0.01 (3), 4.56 +-0.2 (10)]\n
     Used in draw_plot_point_values (note plural - not used in singular draw_plot_point_value)
     where X value_style is used to provide the prefix and separator, and Y value_style to provide the suffix.
     Prefix, separator and suffix are ignored when X or Y are shown separately using draw_plot_point_value.
     "4.5 +- 0.01 (3) Second #2, 2012-Mar-13 13:01:00"
  */
public:
  int value_label_rotation_; //!< Direction point value-labels written.
  align_style value_label_alignment_; //!< Alignment of label with respect to value, left, centered, or right.\n Example: \code text-anchor="middle" \endcode
  // These are a duplicate of info elsewhere?
  int value_precision_; //!< Decimal digits of precision of value, default 3, for example "1.23".
  std::ios_base::fmtflags value_ioflags_; //!< Control of scientific, fixed, hex etc.
  bool strip_e0s_; //!< If true, then unnecessary zeros and + sign will be stripped to reduce length.
  text_style values_text_style_; //!< Font etc used for data-point value marking.
  // svg_style
  svg_color stroke_color_; //!< Stroke color for value.
  svg_color fill_color_; //!< Fill color for value.
  bool plusminus_on_; /*!< If an uncertainty estimate is to be appended (as + or - value).
     \details See http://en.wikipedia.org/wiki/Plus-minus_sign
    */
  svg_color plusminus_color_; //!< Color for uncertainty, for example: 0.02 in "1.23 +-0.02".
  bool addlimits_on_; //!< If an confidence interval is to be added, for example <4.5, 4.8>.
  svg_color addlimits_color_; //!< Color for confidence interval.
  bool df_on_; //!< If a degrees of freedom estimate is to be appended.
  svg_color df_color_; //!< Color for degrees for freedom, for example: 99 in "1.23 +-0.02 (99)".
  bool id_on_;  //!< If an id or name string to be appended. default == false,
  svg_color id_color_; //!< Color for id or name string".
  bool datetime_on_; //!< If an time and/or date string to be appended. default == false,
  svg_color datetime_color_; //!< Color for time and date string".
  bool order_on_;  //!< If an order in sequence number # to be appended. default == false,
  svg_color order_color_; //!< Color for sequence number #".
  std::string prefix_; //!< Prefix to data-point value, default none, but typically "[".
  std::string separator_; //!< Separator between x and y values, if both on same line (none if only X or only Y, or Y below X).
  std::string suffix_; //!< Suffix to data-point value, default none, but typically "]".
  std::string datetime_format_; //!< Boost.Date-time format for output of timestamp value-label (usually a compact format. default "%Y%b%d_%H:%M") .

  //! Constructor data-point value-label style (provides default color and font).
  //! Separators [,] provide for customising layout, for example: [1.23+-0.01 (3), 4.56 +-0.2 (10)].
  value_style(
    int r = static_cast<int>(horizontal), //!< Label orientation, default horizontal.
    align_style a = align_style::left_align, //!< Label alignment, default left, so value_label is to right of data-point-marker.
    int p = 3, //!< Reduced from default of 6 which is usually too long, default 3.
    std::ios_base::fmtflags f = std::ios::dec, //!< Any std::ios::ioflags, for example, hex, fixed, scientific.
    bool s = true, //!< If true, then unnecessary zeros will be stripped to reduce length.
    text_style ts = no_text_style, //!< All defaults, black etc.
    const svg_color& scol = black, //!< == black.
    const svg_color& fcol = black,  //!< no fill.
    bool pm = false, //!< If uncertainty estimate to be appended.
    const svg_color& plusminus_color = black, //!< Default color for uncertainty of value.
    bool lim = false, //!< If confidence limits to be appended.
    const svg_color& addlimits_color = black, //!< Default color for confidence limits.
    bool df = false,  //!< If a degrees of freedom estimate to be appended.
    const svg_color& df_color = black,//!< Default color for degrees of freedom.
    bool id = false,  //!< If an ID string to be appended.
    const svg_color& id_color = black,//!< Default color for ID string.
    bool dt = false,  //!< If a  date and time to be appended.
    const svg_color& dt_color = black,//!< Default color for  date and time  of value.
    bool ordno = false,  //!< If an order # to be appended, for example: "#13".
    const svg_color& order_color = black,//!< Default color for order #.
    
    std::string pre = "", //!< pre Prefix, for example: "[",
    std::string sep  = "", //!< sep Separator, for example: ,\&\#x00A0;", 
                           //!< \note If put comma space ", " the trailing space seems to be ignored, so add Unicode explicit space.
    std::string suf  = "", //!< suf Suffix, for example: "]")
    std::string format = "%Y%b%d_%H:%M" //!< format for date and time if use to label data-point values. 
  )
    :
  value_label_rotation_(r),
    value_label_alignment_(a),
    value_precision_(p), value_ioflags_(f), strip_e0s_(s),
    values_text_style_(ts), stroke_color_(scol), fill_color_(fcol),
    plusminus_on_(pm), plusminus_color_(plusminus_color),
    addlimits_on_(lim), addlimits_color_(addlimits_color),
    df_on_(df), df_color_(df_color),
    id_on_(id), id_color_(id_color),
    datetime_on_(dt), datetime_color_(dt_color),
    order_on_(ordno), order_color_(order_color),
    prefix_(pre), separator_(sep), suffix_(suf),
    datetime_format_(format)
  { //! Constructor setting parameters with some defaults.
  }
}; // class value_style

////////////////////////////////////////////////////////////////////////////////////////////////////
// 
 //! \enum point_shape used for marking a data point.
//! (Used in draw_plot_point in axis_plot_frame.hpp).
enum point_shape
{
  none = 0, //!< No marker for data point.
  circlet, /*!< Circle. 
    \note Name was changed to round to avoid clash with function named circle,
    but was then found to clash with C++ Standard numeric function round.
    Full qualification `point_shape::round` requires C++11 support to compile, so then changed to circlet.
  */
  square, //!< Square.
  point, //!< Small solid point.
  egg, //!< Ellipsoid. Unicode x2B2D is white horizontal, 2B2F is white vertical.
  unc_ellipse, //!< Ellipse sized using uncertainty estimate of x and y,
  //!< typically about twice standard deviation or 95% confidence interval.
  vertical_line,  //!< Vertical line up & down from axis.
  horizontal_line, //!< Horizontal line left & right from axis.
  vertical_tick, //!< Vertical tick up from axis.
  horizontal_tick, //!< Horizontal line right from axis.
  //!< Note horizontal will not be useful for 1D plot - will be on the axis.
  //!< Triangles https://unicode.org/charts/PDF/U25A0.pdf
  cone,
  cone_point_up, //!< Cone pointing right- 'rightwayup'.  Unicode 2580 is down-cone with white center.
  cone_point_down, //!< Cone pointing down - 'upside down'.  Unicode 2580 is down-cone with white center.
  cone_point_right, //!< Cone pointing right.  Unicode 2580 is down-cone with white center.
  cone_point_left, //!< Cone pointing right.  Unicode 2580 is down-cone with white center.
  outside_window, //! Marker to show that a point lies outside the plotting area, and so is not shown.
  triangle, //!< Triangle pointing UP. Unicode &#25B3; big and 25B5 for small triangle  
  star, //!< Star.
  lozenge, //!< Lozenge or square with corners pointing up and down.
  diamond, //!< Diamond playing card shape.
  heart, //!< Heart playing card shape.
  club, //!< Club playing card shape.
  spade, //!< Spade playing card shape.
  asterisk, //!< Asterix as * symbol
  cross, //!< cross
  symbol /*!< Unicode symbol including letters, digits, greek & 'squiggles'.
  \verbatim
    Default letter "X".\n
    Other examples: "&#x3A9;"= greek omega, "&#x2721;" = Star of David hexagram
    &#2720 Maltese cross & other dingbats, like &#x25BABlack right-pointing pointer\n
    \sa http://en.wikipedia.org/wiki/List_of_Unicode_characters#Basic_Latin geometric shapes
    that may be a better way to make these symbols: &#25A0 black square ...to &#25FF
    Most browsers now implement most of these properly.
    http://jrgraphix.net/r/Unicode/25A0-25FF
    http://unicode.org/charts/ https://unicode.org/charts/PDF/U25A0.pdf
  \endverbatim
  */
}; // enum point_shape

class plot_point_style
{ /*! \class boost::svg::plot_point_style
    \brief Shape, color, and symbol or shape of data-point markers.
    \details (optional x and/or y data-point value(s) & optional uncertainty).
  */
  friend std::ostream& operator<< (std::ostream&, plot_point_style);
public:

  plot_point_style(const plot_point_style&);  // Copy constructor.

  svg_color fill_color_; //!< Fill color of the centre of the shape.
  svg_color stroke_color_; //!< Color of circumference of shape.
  int size_; //!< Diameter of circle, height of square, font_size  ...
  point_shape shape_; //!< shape: round, square, point...
  std::string symbols_; //!< Unicode symbol(s) (letters, digits, squiggles etc).\n
  //! Caution: not all Unicode symbols are rendered by all browsers!\n
  //! Example: U2721 is Star of David or hexagram,
  //! see http://en.wikipedia.org/wiki/Hexagram, symbols("&#x2721;")
  //! Positioning of symbols (especially > 1 symbols) may be imprecise.

  text_style symbols_style_; //!< font, size, decoration of symbols.
  bool show_x_value_; //!< If true, show the X value like "1.2" near the point. (If both true, then show both X and Y as a pair like "1.2, 3.4".)
  bool show_y_value_; //!< If true, show the Y value like "3.4" near the point. (If both true, then show both X and Y as a pair like "1.2, 3.4".)

  //int dist; // from the point to the value.
  //int orient_; // Orientation of the value from the point.  == static_cast<int>(rotate_style o)
  //// Note that this needs to alter the text alignment, center, left or right,
  //// to avoid writing over the point marker.
  //int rotation_; // Rotation of the value text string itself.  == static_cast<int>(rotate_style r)
  //// Note that this also needs to alter the text alignment, center, left or right,
  //// to avoid writing over the point marker.
  //text_style value_style_; // Size, font, color etc of the value.

  plot_point_style( //!< Constructor with all defaults.
    const svg_color& stroke = black,  //!< Color of circumference of shape (default black).
    const svg_color& fill = blank, //!< Fill color of the centre of the shape (default white).
    int size = 5, //!< Diameter of circle, height of square, font_size  ...
    point_shape shape = circlet, //!< shape: circlet, square, point...
    const std::string& symbols = ""); //!< Unicode symbol(s) (letters, digits, squiggles etc), (default letter x).

  plot_point_style& size(int i);
  int size();
  plot_point_style& fill_color(const svg_color& f);
  svg_color& fill_color();
  plot_point_style& stroke_color(const svg_color& f);
  svg_color& stroke_color();
  plot_point_style& shape(point_shape s);
  point_shape shape();
  plot_point_style& symbols(const std::string s);
  std::string& symbols();
  plot_point_style& style(text_style ts);
  text_style& style() const;
}; // struct plot_point_style

// class plot_point_style function Definitions.

// Constructor.
  plot_point_style::plot_point_style( //!< Constructor sets defaults for all data members.
    const svg_color& stroke,  //!< Color of circumference or outline of shape.
    const svg_color& fill, //!< Fill color of the centre of the shape (if possible for symbol).
    int size, //!< Diameter of circle, height of square, font_size of Unicode symbol  ...
    // Also symbols_style_.font_size(i); // Also set Font size, in case using a symbol as marker.
    point_shape shape, //!< shape: round, square, point... or symbol if Unicode symbol(s) to be used.
    const std::string& symbols) //!< Unicode symbol(s) (letters, digits, symbols, emojis etc).
  :
    stroke_color_(stroke), 
    fill_color_(fill), 
    size_(size),
    shape_(shape),
    symbols_(symbols), // Unicode
    show_x_value_(false), show_y_value_(false)
  { // May be best to have a fixed-width font for symbols?
    // But there are always problems centering a symbol at @b exactly the right point.
    symbols_style_.font_family("Lucida Sans Unicode");
    symbols_style_.font_size(size); // Default size = 5
  }

  //! plot_point_style Copy constructor.
  plot_point_style::plot_point_style(const plot_point_style& rhs) : 
    stroke_color_(rhs.stroke_color_),
    fill_color_(rhs.fill_color_),
    size_(rhs.size_),
    shape_(rhs.shape_),
    symbols_(rhs.symbols_),
    show_x_value_(rhs.show_x_value_),
    show_y_value_(rhs.show_y_value_)
  {    // ??? needed to fix font of data-point-markers?
    symbols_style_.font_family("Lucida Sans Unicode");
    symbols_style_.font_size(size_); // 
  } //

// Member Function Definitions.

 plot_point_style& plot_point_style::size(int i)
  { //! Set size of shape or symbol used to mark data-value plot point(s).
    size_ = i;  //! Diameter of shape like circle, height of square, or symbols font_size.
    symbols_style_.font_size(i); // Also set Font size, in case using a symbol as marker.
    return *this;
    //! \return plot_point_style& to make chainable.
  }

  int plot_point_style::size()
  { //! \return Size of shape or symbol used to mark data-value plot point(s).
    return size_;
  }

  plot_point_style& plot_point_style::fill_color(const svg_color& f)
  { //! Set Fill color of shape or symbol used to mark data value plot point(s).
    //! \sa stroke_color. For example: \code .fill_color(red).stroke_color(black) \endcode
    fill_color_ = f;
    return *this;
    //! \return plot_point_style& to make chainable.
  }

  svg_color& plot_point_style::fill_color()
  { //! \return Fill color of shape or symbol used to mark data value plot point(s).
    return fill_color_;
  }

  plot_point_style& plot_point_style::stroke_color(const svg_color& f)
  { //! Set stroke color of shape or symbol used to mark data-value plot-point(s).
    //! Example: \code .stroke_color(blue).fill_color(red) \endcode
    stroke_color_ = f;
    return *this; //! \return plot_point_style& to make chainable.
  }

  svg_color& plot_point_style::stroke_color()
  { //! \return Stroke color of shape or symbol used to mark data-value plot-point(s).
    return stroke_color_;
  }

  plot_point_style& plot_point_style::shape(point_shape s)
  { //! Set shape used to mark data-value plot-point(s).\n
    //! Example: \code.shape(circlet).size(10).stroke_color(green).fill_color(red) \endcode
    //! If shape is a symbol, then a Unicode symbol(s) in @c symbols is used.
    shape_ = s;
    return *this; //! \return plot_point_style& to make chainable.
  }

  point_shape plot_point_style::shape()
  { // Get shape used to mark data-value plot-point(s)
    //! \return @c shape used to mark data-value plot-point(s).
    return shape_;
  }

  plot_point_style& plot_point_style::symbols(const std::string s)
  { //! Override default symbol "x" with a Unicode symbol(s) like square with fill, or emoji like smiley. 
    //! \warning Only effective if @c .shape(symbol) is also set!
    //! \warning Using more than one Unicode symbol will not position the symbols exactly at the correct location on the plot.
    symbols_ = s;
    return *this; //! \return plot_point_style& to make chainable.
  }

  std::string& plot_point_style::symbols()
  { //! \return Plot data-point marking Unicode symbol(s).
    //! \warning Only effective if @c .shape(symbol) is also used!
    return symbols_;
  }

  //! Assign a text_style to data-point marker symbol(s).
  plot_point_style& plot_point_style::style(text_style ts)
  {
    symbols_style_ = ts;
    return *this; //! \return plot_point_style& to make chainable.
  }

  text_style& plot_point_style::style() const
  { 
    return const_cast<text_style&>(symbols_style_); //! \return text_style& Control of symbol font, size, decoration etc.
  }

// End class plot_point_style function *Definitions* separated.

  std::ostream& operator<< (std::ostream& os, plot_point_style p)
{  //! Output description of data-value plot point marker(s) to @c std::ostream.
   //! \param os @c std::ostream for output.
   //! \param p Data plot point marker colors, size, shape, symbol(s), style and location.
  os << "plot_point_style("
     << p.shape_ << ", " // Shape enum value.
     << p.stroke_color_ << ", "
     << p.fill_color_ << ", "
     << p.size_ << ", "
     << "\"" << p.symbols_ << "\", "  // Unicode symbol code as string, for example: "&#x2666;" 
     << p.symbols_style_ << ", "  // 
     << p.show_x_value_ << ", "
     << p.show_y_value_
     << ", symbols_style: " << p.symbols_style_ 
     << ")";
/*! \details Example: plot_point_style p;  std::cout << p << std::endl;
   Outputs: 
   data-series #3 point_style =
     plot_point_style(19, RGB(0,128,0), RGB(255,192,203), 30, , 
     text_style(14, "times new roman", "italic", "bold", "narrow", "underline"),
     0, 0)

   shape is enum 19 (small point)
   size is 30 pixels
   symbols = "" - not using one.
   symbols_style_  is a text_style but size is not same as symbol?
   0, 0 x and y values are not shown.

   Example of marker using a Unicode symbol:
   \code 
     plot_point_style my_diamond(red, green, 20, symbol, "&#x2666;");
      std::cout << "my_diamond is " << my_diamond << std::endl;
   \endcode
   Outputs:
   \code my_diamond is plot_point_style(25, RGB(255,0,0), RGB(0,128,0), 20, "&#x2666;", text_style(20, "Lucida Sans Unicode", "", "", "", ""), 0, 0)
   \endcode
   */
return os;
} // std::ostream& operator<<

//! plot_point_style that uses all the defaults.
plot_point_style default_plot_point_style();
plot_point_style not_a_plot_point_style(blank, blank, -1, none);

class plot_line_style
{ //! \class boost::svg::plot_line_style Style of line joining data-series values.
  // TODO Dotted and dashed line style would be useful for monochrome plots.
  // Use 
public:
  svg_color stroke_color_; //!< Stroke color of line. (no fill color for lines)
  svg_color area_fill_; //!< Fill color from line to axis. == false means color.is_blank = true, or = blank.
  double width_; //!< Width of line joining data-series values.
  bool line_on_; //!< If true, data-points will be joined by straight line(s).
  bool bezier_on_; //!< If true, data-points will be joined by bezier curved line(s).

  //! Constructor to set plot line style, but providing defaults for all member data.
  plot_line_style(const svg_color& col = black, const svg_color& fill_col = blank, double width = 2, bool line_on = true, bool bezier_on = false);

  plot_line_style& width(double w);
  double width();
  plot_line_style& color(const svg_color& f);
  svg_color& color();
  plot_line_style& area_fill(const svg_color& f);
  svg_color& area_fill();
  bool line_on() const;
  plot_line_style& line_on(bool is) ;
  bool bezier_on() const;
  plot_line_style& bezier_on(bool is);
}; // class plot_line_style

// class plot_line_style function Definitions.

  //! Constructor to set plot line style, but providing defaults for all member data.
  plot_line_style::plot_line_style(const svg_color& col, const svg_color& fill_col, double width, bool line_on, bool bezier_on)
    :
    stroke_color_(col), area_fill_(fill_col), width_(width), line_on_(line_on), bezier_on_(bezier_on)
    { // Provides defaults for all data.
    }

// Member Functions.

  plot_line_style& plot_line_style::width(double w)
  { //! Set width of line(s) joining data-points.
    width_ = w;
    return *this; // Make chainable.
    //! \return plot_line_style& to make chainable.
  }

  double plot_line_style::width()
  { //! \return  width of line(s) joining data-points.
    return width_;
  }

  plot_line_style& plot_line_style::color(const svg_color& f)
  { //! Set color of line(s) joining data-points.
    stroke_color_ = f;
    return *this; //! \return plot_line_style& to make chainable.
  }

  svg_color& plot_line_style::color()
  { //! \return color of line(s) joining data-points.
    return stroke_color_;
  }

  plot_line_style& plot_line_style::area_fill(const svg_color& f)
  {//! Set if area under line joining data-points is to be color filled.
    area_fill_ = f;
    return *this; //! \return plot_line_style& to make chainable.
  }

  svg_color& plot_line_style::area_fill()
  { //! \return  if area under line joining data-points is to be color filled.
    return area_fill_;
  }

  bool plot_line_style::line_on() const
  { //! \return True if line(s) will join data-points.
    return line_on_;
  }

  plot_line_style& plot_line_style::line_on(bool is)
  { //! Set true if line(s) are to join data-points.
    line_on_ = is;
    return *this; //! \return plot_line_style& to make chainable.
  }

  bool plot_line_style::bezier_on() const
  { //! \return true if bezier curved line(s) are to join data-points.
    return bezier_on_;
  }

  plot_line_style& plot_line_style::bezier_on(bool is)
  { //! Set true if bezier curved line(s) are to join data-points.
    bezier_on_ = is;
    return *this; //! \return plot_line_style& to make chainable.
  }

// End class plot_line_style function Definitions.

std::ostream& operator<< (std::ostream& os, plot_line_style p)
{ //! Output description of plot_line_style. (mainly useful for diagnosis).
  /*! \details Example Usage: \code plot_line_style p;   std::cout << p << std::endl; \endcode
    \n
    \verbatim Outputs: point_line_style(RGB(0,0,0), blank, line, no bezier)  \endverbatim
  */
  os << "point_line_style("
     << p.stroke_color_ << ", "
     << p.area_fill_ << " area fill, "
     << ((p.line_on_) ? "line, " : "no line, ")
     << ((p.bezier_on_) ? "bezier)" : "no bezier)");
  return os;  // \return reference to @c std::ostream to make chainable.
} // std::ostream& operator<<

// class plot_line_style function Definitions.

// Member Functions.

 //! \enum dim dimension of plot. (Used so that an axis knows what type it is,  X (abscissa) or Y (ordinate), or N (none).
enum dim
{
  N = 0, X = 1, Y = 2
};

class axis_line_style
{ /*! \class boost::svg::axis_line_style
    \brief Style of the X or Y-axes lines.
    \details (But NOT the ticks and value-labels because different styles for X and Y-axes are possible).
  */
public:
  dim dim_; //!< Dimension type : None, X (abscissa) or Y (ordinate).
  double min_; //!< minimum X value (Cartesian units).
  double max_; //!< maximum Y value (Cartesian units).
  // Note that these duplicate the same named in ticks_labels_style,
  // but they might have different uses, so are left pro tem.
  // TODO reconsider the implications of this (largely accidental) decision.
  // double interval_; does NOT duplicate major_interval_ in ticks_label_style.
  svg_color color_; //!< Axis line (stroke) color.
  double axis_width_; //!< Axis line width.
  int axis_position_; /*!< How the axes intersect with values as below:\n
    enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
    enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
    If axes look like an L, then is bottom left.
    If a T then y intersects and X is at bottom.
  */
  bool label_on_; //!< Label axis with text - example: "length".
  bool label_units_on_; //!< Label axis units, example: "cm".
  bool axis_line_on_; //!< Draw an X horizontal or Y vertical axis line.
  double axis_; //!< Depending on value of dim, either X-axis (y = 0) transformed into SVG Y coordinates or Y-axis (x = 0) transformed into SVG X coordinates (-1 if not calculated yet).

  // Used in axis_plot_frame.hpp
  // class axis_line_style default constructor, sets default values for all member data items.
   //! Constructor that provides defaults all axis style items.
  axis_line_style(
    dim d = X, //!< X or Y axis.
    double min = -10., //!< Minimum of axis line.
    double max = +10., //!< Maximum of axis line.
    // See also default in ticks_labels_style.
    const svg_color col = black, //!< Axis line color.
    double width = 1, //!< Axis line width.
    int axis_position = 0, /*!< How the axes intersect with values as below:\n
    enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
    enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
    If axes look like an L, then is bottom left.
    If a T then y intersects and X is at bottom.
  */
    bool label_on = true, //!< Label axis with text - example: "length".
    bool label_units_on = false, //!< Label axis units, example: "cm".
    bool axis_lines_on = true,  //!< Draw an X horizontal or a Y vertical axis line.
    double axis = -1  /*!< Depending on value of dim,
                        either X-axis (y = 0) transformed into SVG Y coordinates,
                        or Y-axis (x = 0) transformed into SVG X coordinates (-1 if not calculated yet). */
    );
  // class axis_line_style member functions Declarations:
  // Set and get member functions.
  axis_line_style& color(const svg_color& color);
  svg_color color();
  axis_line_style& width(double w);
  double width();
  bool label_on() const;
  axis_line_style& label_on(bool is) ;
  bool label_units_on() const;
  axis_line_style& label_units_on(bool is) ;
  axis_line_style& position(int pos);
  double position();
  bool axis_line_on() const;
}; // class axis_line_style

  // class axis_line_style Member Functions Definitions:
  // Default constructor.
  axis_line_style::axis_line_style( //!< Sets all member data items with defaults for all.
    dim d, //!< Dimension (zero if boxplot)
    double min, //!< Minimum of axis line.
    double max, //!< Maximum of axis line.
    // See also default in ticks_labels_style.
    const svg_color col, //!< Line color
    double width, //!< Line width.
    int axis_position, //!< Intersection of axis, if any.
    bool label_on,//!< If to include axis label (default true).
    bool label_units_on, //!< If to include units after axis label (default true).
    bool axis_lines_on, //!< If to draw an axis line.
    double axis) // Depending on value of dim, either X-axis (y = 0) transformed into SVG Y coordinates or Y-axis (x = 0) transformed into SVG X coordinates (-1 if not calculated yet).
    :
    dim_(d), min_(min), max_(max), color_(col), axis_width_(width),
    axis_position_(axis_position),
    label_on_(label_on), // default is to include axis label.
    label_units_on_(label_units_on), // default is to include units after axis label.
    axis_line_on_(axis_lines_on),
    axis_(axis) // -1 means not calculated yet.
  { // Initialize all data.
    if(max_ <= min_)
    { // max_ <= min_.
      throw std::runtime_error("Axis range: max <= min!");
    }
    if((max_ - min_) < std::numeric_limits<double>::epsilon() * 1000)
    { // Range too small to display.
      throw std::runtime_error("Axis range too small!" );
    }
    axis_ = -1; // means not calculated yet - see axis_plot_frame.
  } // axis_line_style constructor

  axis_line_style& axis_line_style::color(const svg_color& color)
  { //! Set color of an axis line.
    color_ = color;
    return *this; //! \return plot_line_style& to make chainable.
  }

  svg_color axis_line_style::color()
  { //! \return  color of an axis line.
    return color_;
  }

  axis_line_style& axis_line_style::width(double w)
  { //! Set width of an axis line.
    axis_width_ = w;
    return *this; //! \return plot_line_style& to make chainable.
  }

  double axis_line_style::width()
  { //! \returns width of an axis line (pixels).
    return axis_width_;
  }

  bool axis_line_style::label_on() const
  { //! If returns true, then axis to be labelled, for example "X axis".
    return label_on_;
  }

  axis_line_style& axis_line_style::label_on(bool is)
  { //! If set true, then axis to be labelled with the label, for example "X axis" (but default "").
    label_on_ = is;
    return *this; //! \return plot_line_style& to make chainable.
  }

  bool axis_line_style::label_units_on() const
  { //! If returns true, then axis to be labelled with unit, for example " (mm)"
    return label_units_on_;
  }

  axis_line_style& axis_line_style::label_units_on(bool is)
  { //! If set true, then axis to be labelled with the units label, for example " (mm)" (but default "").
    label_units_on_ = is;
    return *this;  //! \return plot_line_style& to make chainable.
  }

  axis_line_style& axis_line_style::position(int pos)
  { //! How the axes intersect.
    axis_position_ = pos;
    return *this;  //! \return plot_line_style& to make chainable.
  }

  double axis_line_style::position()
  { //! \return How the axes intersect.\n
    //! enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
    //! enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
    //! If axes look like an L, then is bottom left.
    //! If a T then y intersects and X is at bottom.
    return axis_position_;
  }

  bool axis_line_style::axis_line_on() const
  { //! If returns true, then either an X or a Y axis line to be drawn.
    return label_on_;
  }

// End class axis_line_style member functions definitions:

class ticks_labels_style
{ /*! \class boost::svg::ticks_labels_style
   \brief Style of the X and Y axes major and minor ticks, grids and their tick-value-labels.
   \details
   But NOT the X and Y axes lines.
   These can be either on the axis lines or on the plot window edge(s),
   (because different styles for x and y are possible).
  */
  friend class svg_2d_plot;

public:
    dim dim_; //!< Dimension type X, Y, or None.
    double min_; //!< Minimum x value (Cartesian units).
    double max_; //!< Maximum x value (Cartesian units).
    double minor_interval_; //!< Stride or interval between minor ticks (Cartesian units).
    double major_interval_; //!< Stride or interval between major x ticks (Cartesian units).
      // No set function because x_num_minor_ticks_ used to determine this instead,
      // but one could calculate x_minor_interval_.
    unsigned int num_minor_ticks_; //!< Number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5 (All units in svg units, default pixels).
    svg_color major_tick_color_; //!< Color (stroke) of tick lines.
    double major_tick_width_; //!< Width of major tick lines.
    double major_tick_length_;//!< Length of major tick lines.
    svg_color minor_tick_color_; //!< Color (stroke) of tick lines.
    double minor_tick_width_; //!< Width of minor tick lines.
    double minor_tick_length_; //!< Length of minor tick lines.
    svg_color major_grid_color_; //!< Color of major grid lines.
    double major_grid_width_; //!< Width of major grid lines.
    svg_color minor_grid_color_; //!< color of minor grid lines.
    double minor_grid_width_; //!< Wdith of minor grid lines.

    bool up_ticks_on_; //!< Draw ticks up from horizontal X-axis line.
    bool down_ticks_on_; //!< Draw ticks down from horizontal X-axis line.
    bool left_ticks_on_; //!< Draw ticks left from vertical Y-axis line.
    bool right_ticks_on_; //!< Draw ticks right from vertical Y-axis line.
    // Simplest to have all of these although only one pair (up or down) or (left or right) is used.
    // Unused are always false.
    int major_value_labels_side_; //!< Which side of axis for label values for major ticks.
    // < 0 means to left (for Y) or down (for X) (default),
    // 0 (false) means no ticks value-labels (just ticks),
    // > 0 means to right (for Y) or top(for X).
    int label_rotation_; //!< Rotation direction axis tick-value-labels written. Default 0 means horizontal.
    // assign by static_cast<int>(horizontal)
    //align_style label_alignment_; //< Alignment of tick-value-label using text_anchor. Default center_align for X-axis but right_align for Y-axis.
    //!< This ensures that value-labels center on the tick so that "1.1" aligns the decimal point with the tick.
    //!< Ideally alignment takes rotation into account to get label as close a possible to the tick. 
    bool major_grid_on_;  //!< Draw X grid at major ticks.
    bool minor_grid_on_; //!< Draw X grid at minor ticks.
    svg_color values_color_; //!< Color of tick values labels.
    // (just fill_color for now (stroke makes characters fuzzy.)
    int value_precision_; //!< Precision for tick value-labels, usually 3 will suffice.
    std::ios_base::fmtflags value_ioflags_;  //!< IO formatting flags for the axis default std::ios::dec.
    bool strip_e0s_; //!< If redundant zero, + and e are to be stripped, for example "+1.000e3" to "1e3".
    double label_max_length_;  //!< width (in SVG units, pixels) of longest value-label text on axis.
    double label_max_space_;  //!< Space (SVG units, pixels) needed for value-label adjusted for rotation.
    int ticks_on_window_or_on_axis_; //!< value-labels & ticks on a plot window border (rather than on X or Y-axis).
    //! For Y-axis -1 = left, 0 = false = on X-axis, +1 = right. Default -1 to left of plot window.
    //! For X-axis -1 = bottom, 0 = false = on Y-axis, +1 = top. Default -1 below bottom of plot window.
    //! 0 = false puts the ticks and their labels on the X or Y axis line which may be in the middle of the plot.
    //! For 1D the default overrides the constructor default of -1 below, to tick and value-label the X-axis.
    //! For 2D the default is left at -1, to use bottom and left of plot window to tick and value-label X and Y-axis.

    text_style value_label_style_; //!< text style (font, size...) for value-labels.

     //! Constructor setting several parameters, but providing default values for all member data.
    ticks_labels_style(
    dim d = X, //!< X or Y axis (-1 if not assigned yet).
    const text_style& style = no_text_style, //!< Default text font style.
    double max = 10.,  //!< Maximum x value (Cartesian units).
    double min = -10., //!< Minimum x value (Cartesian units).
    double major_interval = 2., //!< Interval between major ticks.
    unsigned int num_minor_ticks = 4) //!< Number of minor ticks between major ticks.
    : // Constructor.
    // Initialize all data.
    // can be changed by set functions.
    dim_(d), // 1 or 2 D
    min_(min),
    max_(max),
    minor_interval_(0), //!< Calculated from x & y_num_minor_ticks_
    major_interval_(major_interval),
    num_minor_ticks_(num_minor_ticks),
    // These are the plot defaults.
    // major_interval_(2.), // x stride between major ticks & value-label.
    // num_minor_ticks_(4), // suits: major 0, minor 2, 4, 6, 8, major 10
    // but given a value here for safety.
    major_tick_color_(black), // line stroke color.
    major_tick_width_(2),
    major_tick_length_(5),
    minor_tick_color_(black), // line stroke color.
    minor_tick_width_(1),
    minor_tick_length_(2),
    major_grid_color_(svg_color(200, 220, 255)), // greyblue default color.
    major_grid_width_(1.), // pixels.
    minor_grid_color_(svg_color(200, 220, 255)), // greyblue
    minor_grid_width_(0.5), // pixels.

    up_ticks_on_(false), // Draw ticks up from horizontal X-axis line.
    down_ticks_on_(true), // Draw ticks down from horizontal X-axis line.
    left_ticks_on_(true), // Draw ticks left from vertical Y-axis line.
    right_ticks_on_(false), // Draw ticks right from vertical Y-axis line.
    // Simplest to have all of these although only one pair like up or down is used.
    // Unused are always false.
    major_value_labels_side_(-1), // Label values side for major ticks left (right or none).
    label_rotation_(static_cast<int>(horizontal)), // Direction axis value-labels written.
      // label_alignment(?)
    major_grid_on_(false),  // Draw grid at major ticks.
    minor_grid_on_(false),// Draw grid at minor ticks.
    values_color_(black),
    value_precision_(3), // precision for tick value-labels, usually 3 will suffice.
    // 4 might be better to permit thousands without using e format?
    value_ioflags_(std::ios::dec),  // IO formatting flags for the axis.
    // Note that ALL the flags are set, overwriting any defaults, so std::dec is wise.
    // This should give the default 'normal' iosflags with neither fixed, scientific nor showpoint set.
    strip_e0s_(true), // strip superflous zeros and signs.
    label_max_length_(0.), // length (estimated in SVG units) of longest label on axis.
    label_max_space_(0.), // Space (estimated in SVG units) of longest label on axis, adjusted for rotation.
    ticks_on_window_or_on_axis_(-1), // value-labels & ticks on the plot window,
    // rather than on X or Y-axis.
    // Default -1 means left or bottom of plot window.
    value_label_style_(style)
    {
      if(max_ <= min_)
      { // max_ <= min_.
        throw std::runtime_error("Axis ticks & labels range: max <= min!");
      }
      if((max_ - min_) < std::numeric_limits<double>::epsilon() * 1000)
      { // Range too small to display.
        throw std::runtime_error("Axis ticks & labels range too small!" );
      }
  } // ticks_labels_style constructor.

  double label_length(double value)
  { //! Find the SVG length of label (like "1.23E-5") for a value.
    // Needs to know the IO precision & flags for the axis,
    // and if zeros are to be stripped, so can't be a free function.
    std::stringstream label;
    label.precision(value_precision_);
    label.flags(value_ioflags_);
    label << value; // "1.2" or "3.4e+000"...
    double r;
    if (strip_e0s_)
    { // Do want to strip unecessary e, +, & leading exponent zeros.
      std::string stripped = strip_e0s(label.str());
      r = string_svg_length(stripped, value_label_style_);
      // want x_or y_value_label_style_ here!
      return r;
    }
    r = string_svg_length(label.str(), value_label_style_);
    return r;
  } // double label_length

  double longest_label()
  { //! Update label_max_length_ with the longest value-label as SVG units (default pixels),
    //! return the count of digits etc.
    if(major_value_labels_side_ != 0) // ! none
    { // Show values by the tick as "1.2" or "3.4e+000"...
      double longest = 0;

      //axis_line_style& axis = (dim_ = X) ? y_axis() : x_axis(); // The intersecting *other* axis.
      //  || !axis.axis_line_on_ ignore these tests to avoid the above that doesn't work.
      // Zero isn't likely to be the longest value-label.
      // ticks_labels_style& tick_labels = (dim_ = X) ? x_ticks() : y_ticks(); // doesn't work.

      // Check length of label for the ticks on the positive side (right or above zero).
      for(double v = 0.; v <= max_; v += major_interval_)
      {
        if (v != 0. || ticks_on_window_or_on_axis_ != 0)
        { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line,
          // or avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
          // (won't be a Y-axis line for 1-D, where both the zero tick & value-label is always wanted).
          double l = label_length(v);
          if (l > longest)
          {
            longest = l;
          }
        }
      } // for v
      // Check length of label of the ticks on the negative side (left of zero).
      for(double v = 0.; v >= min_; v -= major_interval_)
      {
        if (v != 0. || ticks_on_window_or_on_axis_ != 0)
        { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line.
          // (won't be Y-axis line for 1-D where the zero tick is always wanted).
          // But no tick means no value-label 0 either unless on_plot_window.
          double l = label_length(v);
          if (l > longest)
          {
            longest = l;
          }
        }
      } // for v
      label_max_length_ = longest;
      return longest;
    }
    else
    {
      label_max_length_ = 0;
      return 0;
    }
  } // longest_label()


  bool use_up_ticks() const
  {
    return up_ticks_on_; //! \return true if to draw ticks up from horizontal X-axis line.
  }

  ticks_labels_style& use_up_ticks(bool is)
  { //! Set true to draw ticks up from horizontal X-axis line.
    up_ticks_on_ = is;
    return *this; //! \return ticks_labels_style& to make chainable.
  }

  bool use_down_ticks() const
  {
    return down_ticks_on_; //! \return true if to draw ticks down from horizontal X-axis line.
  }

  ticks_labels_style& use_down_ticks(bool side)
  { //! Set true if to draw ticks down from horizontal X-axis line.
    down_ticks_on_ = side;
    return *this; //! \return ticks_labels_style& to make chainable.
  }

  int major_value_labels_side() const
  { //! \return side for tick value-labels: left (<0), none (==0) or right (>0).
    return major_value_labels_side_;
  }

  ticks_labels_style& major_value_labels_side(int is)
  { //! Set side for tick value-labels: left (<0), none (==0) or right (>0).
    major_value_labels_side_ = is;
    return *this; //! \return ticks_labels_style& to make chainable.
  }
}; // class ticks_labels_style

class box_style
{ //! \class boost::svg::box_style Style of a rectangular box. (Used for boxplot image and plot window).
public:
    svg_color stroke_; //!< Box line (stroke) color.
    svg_color fill_; //!< Box fill color.
    double border_width_; //!< plot border rectangle width.
    double margin_; //!< Marginal (pixels) space around the box (inside or out).
    bool border_on_; //!< Display the border of the box.
    bool fill_on_; //!< Color fill the box.

    //! Constructor to set parameters but provides defaults for all variables.
    box_style(
      const svg_color& scolor = black, //!< stroke color
      const svg_color& fcolor = white, //!< fill color (white = no fill).
      double width = 1., //!< of border.
      double margin = 1., //!< Margin around box (SVG units, default pixels).
      bool border_on = true, //!< Draw a border of specified width.
      bool fill_on = false //!< Apply fill color.
     );
  box_style& stroke(const svg_color& color);
  svg_color stroke();
  box_style& fill(const svg_color& color);
  svg_color fill();
  box_style& width(double w);
  double width();
  box_style& margin(double w);
  double margin();
  bool border_on() const;
  box_style& border_on(bool is) ;
  bool fill_on() const;
  box_style& fill_on(bool is) ;

}; // class box_style

// class box_style Definitions.

//! Constructor to set parameters but provides defaults for all variables.
box_style::box_style(
  const svg_color& scolor, // = black,
  const svg_color& fcolor, // = white, // No fill.
  double width,//  = 1, // of border
  double margin, // = 4., //
  bool border_on,// = true, // Draw a border of width.
  bool fill_on) //= false) // Apply fill color.
    :
  stroke_(scolor), fill_(fcolor), border_width_(width),
  margin_(margin),
  border_on_(border_on),
  fill_on_(fill_on)
  { // Initializes all member data with defaults.
  }

// Member Functions definitions.

  box_style& box_style::stroke(const svg_color& color)
  { //! Set (stroke) color for box outline.
    stroke_ = color;
    return *this; //! \return box_style& to make chainable.
  }

  svg_color box_style::stroke()
  {
    return stroke_; //! \return (stroke) color for box outline.
  }

  box_style& box_style::fill(const svg_color& color)
  { //! Set fill color for box.
    fill_ = color;
    return *this; //! \return box_style& to make chainable.
  }

  svg_color box_style::fill()
  {
    return fill_; //! \return Fill color for box.
  }

  //! Set width for box.
  box_style& box_style::width(double w)
  { 
    border_width_ = w;
    return *this; //! \return box_style& to make chainable.
  }

  double box_style::width()
  {
    return border_width_; //! \return width for box.
  }

  box_style& box_style::margin(double w)
  { //! Set marginal (default pixels) space around the box (inside or out).
    margin_ = w;
    return *this; //! \return box_style& to make chainable.
  }

   double box_style::margin()
  {
    return margin_; //! \return marginal (default pixels) space around the box (inside or out).
  }

  bool box_style::border_on() const
  {
    return border_on_; //! \return If the box border should be shown.
  }

  box_style& box_style::border_on(bool is)
  { //! Set true if the box border should be shown.
    border_on_ = is;
    return *this; //! \return box_style& to make chainable.
  }

  bool box_style::fill_on() const
  {
    return fill_on_; //! \return if the box should be filled.
  }

  box_style& box_style::fill_on(bool is)
  {//! Set true if the box should be filled.
    fill_on_ = is;
    return *this; //! \return box_style& to make chainable.
  }

// End class box_style Definitions.

//! \enum bar_option Options for bar to draw bar charts.
enum bar_option
{ 
  y_block = -2, //!< Rectangular (optionally filled) block style horizontal to Y-axis,
  y_stick = -1, //!< Bar or row line (stroke width) horizontal to Y-axis.
  no_bar = 0, //!< No bar.
  x_stick = +1, //!< Stick or column line (stroke width) vertical to X-axis.
  x_block = +2  //!< Rectangular (optionally filled) block style vertical to X-axis,
  // Other options like cylinders and cones might be added here?
  // x_cyl = +3, x_cone = +4 ...
};

//! \enum histogram_option options for histograms.
enum histogram_option
{ 
  // row = -1, // Row line (stroke width) horizontal to Y-axis. Not implemented.
  // See svg_2d_plot for details of why not.
  no_histogram = 0, //!< No histogram.
  column = +1 //!< Stick or column line (stroke width) vertically to/from X-axis.
  //! Column is the most common histogram style.
};

 /*! \class boost::svg::histogram_style
     \brief Histogram options.
*/
class histogram_style
{
public:
  histogram_option histogram_option_; //!< default bar, no_histogram or column.

  histogram_style(histogram_option opt = no_histogram); //!< Set any histogram option.

  histogram_style& histogram(histogram_option opt); //!< Set any histogram option.

  double histogram(); //!< \return Histogram option.
}; // class histogram_style


// class histogram_style Definitions.

// Constructor.
histogram_style::histogram_style(histogram_option opt)
:
histogram_option_(opt)
{ //! Constructor providing defaults for all data.
  //! Line width and area-fill are taken from the plot_line_style style.
}

// Member Functions Definitions.

histogram_style& histogram_style::histogram(histogram_option opt)
{ //! Histogram to be shown as sticks or bars.
  histogram_option_ = opt;
  return *this; //! \return box_style& to make chainable.
}

double histogram_style::histogram()
{
  return histogram_option_; //!< \return Histogram option.
}
// End class histogram_style Definitions.

class bar_style
{ /*! \class boost::svg::bar_style
    \brief Style (color, width, fill) of histogram bars.
  */
  // TODO should inherit from svg_style?
public:
  svg_color color_; //!< Color of line (stroke) (no fill color for lines).
  svg_color area_fill_; //!< Fill color from line to axis.
  double width_; //!< Width of bar, not enclosing line width.
  bar_option bar_option_; //!< stick or bar.
  bar_style(const svg_color& col = black, const svg_color& acol = true, double width = 2, bar_option opt = no_bar); //!< Construct with defaults for all member variables.
  bar_style& width(double w); //!<
  double width();
  bar_style& color(const svg_color& f);
  svg_color& color();
  bar_style& area_fill(const svg_color& f);
  svg_color& area_fill();
  bar_style& bar_opt(bar_option option);
  double bar_opt();

}; // class bar_style

// class bar_style Definitions.

//! Constructor, setting defaults for all member variables.
bar_style::bar_style(const svg_color& col, const svg_color& acol, double width, bar_option opt)
:
color_(col), area_fill_(acol), width_(width),  bar_option_(opt)
{ // Defaults for all data.
}

// Member Functions Definitions.

bar_style& bar_style::width(double w)
{ // of bar, not the enclosing line (stroke) width.
  width_ = w;
  return *this; //! \return box_style& to make chainable.
}

double bar_style::width()
{
  return width_; //! \return Width of bar, not enclosing line width.
}

bar_style& bar_style::color(const svg_color& f)
{ // Set color of line or enclosing line.
  color_ = f;
  return *this; //! \return box_style& to make chainable.
}

svg_color& bar_style::color()
{ //
  return color_; //! \return  Color of bar line or enclosing line.
}

bar_style& bar_style::area_fill(const svg_color& f)
{ // Set bar rectangle fill color.
  area_fill_ = f;
  return *this; //! \return box_style& to make chainable.
}

svg_color& bar_style::area_fill()
{
  return area_fill_; //! \return bar rectangle fill color.
}

bar_style& bar_style::bar_opt(bar_option option)
{ // Set to use stick or bar for histograms.
  bar_option_ = option;
  return *this; //! \return box_style& to make chainable.
}

double bar_style::bar_opt()
{
  return bar_option_; //! \return If to use stick or bar for histograms.
}

// End class bar_style Member Functions Definitions.

const std::string strip_e0s(std::string s)
{ /* To remove redundant sign and leading zero(s) in exponent, for example, "1.2e+000" becomes "1.2"
    \details Used to work out the longest value-label before calculate_plot_window.
    Should also be useful for values that spill over into exponent format
    'by accident' - when leading zeros are likely.
  */

  // An ugly hack but works...
  //  (Could also do the same for uppercase E cases).
  //  (Considered doing a repeated strip but complicated).

  using std::string;
  size_t j = s.find("e+000");
  if (j != std::string::npos)
  { // Found "e+000".
    s.erase(j, 5); // remove "e+000" completely, leaving no e... at all.
    return s;
  }
  j = s.find("e-000");
  if (j != std::string::npos)
  { //
    s.erase(j, 5); // remove entire "e-000".
    return s;
  }
  j = s.find("e+00");
  if (j != std::string::npos)
  { // Found "e+00", either 1.23e+00 or 2.34e+01 .. e+09
     // From VS2015 and GCC and Clang all follow C++ standard
    // which says use at least two exponent digits (MS was always 3 e+ddd)
    // "e+00" with no following digit means a real zero exponent.
   if (s[j+3] == '0')
    { // is just "e+00" for a zero exponent, so remove the entire string.
     s.erase(j, 4); // remove "e+00" from "e+00", leaving no e... at all.
    }
    else
    {
      s.erase(j + 1, 3); // remove "+00" from "e+009", leave d, so becomes e9.
    }
    return s;
  }

  j = s.find("e-00");
  if (j != std::string::npos)
  {
    s.erase(j+2, 2); // remove "00", leave "-" and any trailing d.
    return s;
  }

  j = s.find("e+0");
  if (j != std::string::npos)
  {
    s.erase(j + 1, 2); // remove "+0", leave "dd"
    return s;
  }
  j = s.find("e-0");
  if (j != std::string::npos)
  {
    s.erase(j+2, 1); // remove "-0", leave "-dd"
  }
  return s; //! \return Trimmed string (perhaps unchanged).
} // const std::string strip(double d)

  /*! \details
  http://www.w3.org/TR/SVG/text.html#FontSizeProperty
  Font size is the height of the text's font, so width = aspect_ratio * font_size.

  Even after reading http://www.w3.org/TR/SVG/fonts.html,\n
  unclear how to determine the exact width of digits, so an
  arbitrary average width height ratio aspect_ratio = 0.6 is used as a good approximation.
  */

double string_svg_length(const std::string& s, const text_style& style)
{
  /*
  \brief Compute svg length of @c std::string.
  \param s @c std::string that may contains embedded Unicode symbols in hex format, for example &#x3A9;
  \param style text_style containing font size, family, weight etc. \sa text_style.
  \verbatim
  To avoid big length and centering misalignments caused by
  a 7 character Unicode hex value counting as 6 characters instead of one actual symbol,
  if possible use an actual length, but probably platform and/or browser-dependent,
  else use average char width, held in \code static const aspect_ratio \endcode
  and deal with Unicode, for example &#x3A9; = greek omega,
  counting each symbol(s) embedded between & and ; as one EM width character.

  Font Support for Unicode Characters is pretty good in 2020, but
  to see what symbols are supported by browsers see http://www.fileformat.info/info/unicode/font/index.htm
  Fonts of interest to Unicode users tab but mainly lists compliance as stars out of 5.

  Font Character Test for Arial Unicode MS has 5 stars
  http://www.fileformat.info/info/unicode/font/arial_unicode_ms/list.htm lists all the fonts,
  and those not supported.

  see the 'tab' at the bottom for 'Grid with examples' and characters NOT supported.
  Supported by blocks shows the 'blocks' like greek, math operators

  http://www.fileformat.info/info/unicode/font/arial_unicode_ms/grid.htm shows all the examples,
  some of special interest like Unicode Character 'DEGREE CELSIUS' (U+2103)
  chess symbols, lots of star symbols, 25b0 triangles, Greek 0410, letter and numbers inside circle 24B0,
  units like mA, uF 3380, math symbols 2200 like Unicode Character 'CIRCLED DOT OPERATOR' (U+2299),

  Unicode Characters in the Miscellaneous Symbols Block like:
  umbrella, snowman, sun, cloud, cross of jerusalem, male female, chess and very many Chinese ideograms.

  Also 5 stars Code2000, but in practise most fonts given 4 star will be useful.

  For example,
  https://www.fileformat.info/info/unicode/font/lucida_sans_unicode/index.htm
  shows Font Character Test for Lucida Sans Unicode.
  tabs of links at the bottom of page:  Unicode characters supported by the Lucida Sans Unicode font

  Esimate of length should also ignore embedded xml like <sub> (not implemented by all browsers yet).
  Uses @c aspect_ratio to estimate width of characters from font_size (height).

  SVG function getComputedTextLength() allows a document to compute the actual SVG length needed
  https://www.w3.org/TR/SVG/text.html#__svg__SVGTextContentElement__getComputedTextLength
  usable from Javascript and MS windows,
  but this is no use in a C++ program without a lookup table of all Unicode chars,
  so the best we can do is to compute an average character width multiplied by number of characters?
  Also, to avoid any overflow (or underflow) of the box, we can instruct the renderer to stretch and squeeze
  to fill exactly the estimated width used to write the box border.

  https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/textLength
  The textLength attribute, available on SVG <text> and <tspan> elements,
  lets you specify the width of the space into which the text will draw.
  \code
    <text y="20" textLength="6em">Small text length</text>
    <text y="40" textLength="120%">Big text length</text>
  \endcode

  \endverbatim
  \returns length of string in SVG units depending on text_style (font size etc).

  \example
    \code double title_svg_length = string_svg_length(derived().title(), derived().title_info_.textstyle());
    \endcode
 */

 int chars = 0; // Actual number of characters in string.
 double width = 0.; // Estimated width of characters in string.
 bool in_esc = false;
 for (std::string::const_iterator i = s.begin(); i != s.end(); i++)
 {
   unsigned char c = *i;
    if (*i == '&')
    { // Start of Unicode 'escape sequence &#XAB12;' 
      in_esc = true;
       while ((*i != ';') // Assume corrctly terminated by ;
         && (i != s.end())) // In case mistakenly not null terminated.
       {
          i++; 
       }
       in_esc = false;
       chars++; // Only count a Unicode string like &#x3A9; as 1 character (greek omega) wide.
       width += aspect_ratio * 1.3;  // Assume all Unicode glyphs are a bit wider? 
    }
    else if (*i == '<')
    { // Ignore embedded XML like <sub> or <super> used in Quickbook for subscript and superscripts.
      // (Users should use Unicode symbols to show correctly, for example in units like m^2)
      in_esc = true;
       while ((*i != '>')
         && (i != s.end())) // In case mistakenly not terminated.
       {
           i++; // Only count <...>; as NO characters wide.
       }
       chars--;  // Ignore the entire <...> sequence.
       in_esc = false;
    }
    else
    { // Actual normal ASCII-ish character.
      chars++;
      width += (isupper(c) ? aspect_ratio * 1.2 : aspect_ratio);  // Upper case chars are wider than lower.
    }
 }
 //double svg_length = chars * (style.font_size() * aspect_ratio);  // Estimated width of svg string.
 double svg_length = width * style.font_size();  // Estimated width of svg string.

 // Would be nice to be able to use
 // https://www.w3.org/TR/SVG/text.html#__svg__SVGTextContentElement__getComputedTextLength
// double svg_computed_length = getComputedTextLength();

#ifdef BOOST_SVG_STYLE_DIAGNOSTICS
  std::cout << "string \"" << s << "\" has " << chars << " Unicode characters"
    ", width = " << width << ", font_size " << style.font_size() << " and svg length is " << svg_length << std::endl;
  std::cout << "style =  " << style << std::endl;
#endif // BOOST_SVG_STYLE_DIAGNOSTICS

  // I:\modular-boost\libs\svg_plot\example\SVG_text_width_height.cpp shows that
  // width can be quite a bit different from different fonts, especially serif versus sans serif.
  // Aspect ratio can vary from 0.4 to 0.55 so estimate of 1 units could increase to 1.4 in worst case, as observed.
  // For example 1000 svg units might take between 180 and 240 random chars.
  // So must use textLength="estimated" to ensure it fits, for example adding
  //   textLength="1e+03"

 return svg_length;
} // double string_svg_length(const std::string& s, const text_style& style)

}//svg
}//boost

#endif // BOOST_SVG_SVG_STYLE_HPP

/*

*/
