/*!
  \file svg_color.hpp
  \brief SVG standard names of colors, and functions to create and output colors.
  \date 9 Feb 2009
  \author Jacob Voytko & Paul A. Bristow
*/
// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2009
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_COLOR_HPP
#define BOOST_SVG_SVG_COLOR_HPP

#include <boost/assert.hpp>

#include <ostream>
// using std::ostream

namespace boost
{
namespace svg
{
  /*!
    \brief Colors that have SVG standard special names.
    \details  The reason that the underscore separator convention does not match
    the normal Boost format is that these names that are specified by the SVG standard.
    http://www.w3.org/TR/SVG/types.html#ColorKeywords
    color "tan" is also renamed to "tanned" to avoid clash with global function name tan in math.h.
  */
  enum svg_color_constant
  { //! \enum svg_color_constant SVG standard names for some colors.
    //! See http://www.w3.org/TR/SVG/types.html#ColorKeywords
    aliceblue, antiquewhite, aqua, aquamarine, azure, beige,
    bisque, black, blanchedalmond, blue, blueviolet, brown,
    burlywood, cadetblue, chartreuse, chocolate, coral,
    cornflowerblue, cornsilk, crimson, cyan, darkblue, darkcyan,
    darkgoldenrod, darkgray, darkgreen, darkgrey, darkkhaki,
    darkmagenta, darkolivegreen, darkorange, darkorchid, darkred,
    darksalmon, darkseagreen, darkslateblue, darkslategray,
    darkslategrey, darkturquoise, darkviolet, deeppink,
    deepskyblue, dimgray, dimgrey, dodgerblue, firebrick,
    floralwhite, forestgreen, fuchsia, gainsboro, ghostwhite, gold,
    goldenrod, gray, grey, green, greenyellow, honeydew, hotpink,
    indianred, indigo, ivory, khaki, lavender, lavenderblush,
    lawngreen, lemonchiffon, lightblue, lightcoral, lightcyan,
    lightgoldenrodyellow, lightgray, lightgreen, lightgrey,
    lightpink, lightsalmon, lightseagreen, lightskyblue,
    lightslategray, lightslategrey, lightsteelblue, lightyellow,
    lime, limegreen, linen, magenta, maroon, mediumaquamarine,
    mediumblue, mediumorchid, mediumpurple, mediumseagreen,
    mediumslateblue, mediumspringgreen, mediumturquoise,
    mediumvioletred, midnightblue, mintcream, mistyrose, moccasin,
    navajowhite, navy, oldlace, olive, olivedrab, orange,
    orangered, orchid, palegoldenrod, palegreen, paleturquoise,
    palevioletred, papayawhip, peachpuff, peru, pink, plum,
    powderblue, purple, red, rosybrown, royalblue, saddlebrown,
    salmon, sandybrown, seagreen, seashell, sienna, silver,
    skyblue, slateblue, slategray, slategrey, snow, springgreen,
    steelblue, tanned,
    // tan, // Note that tan would clash with geometric tan in math.h!
    teal, thistle, tomato, turquoise, violet,
    wheat, white, whitesmoke, yellow, yellowgreen,
    blank // 'NotAColor' == 147
  }; // enum svg_color_constant

  // Forward declarations in this module (see also svg_fwd):

  class svg_color;
  void constant_to_rgb(svg_color_constant c, unsigned char& r, unsigned char& g, unsigned char& b);
  std::ostream& operator<< (std::ostream&, const svg_color&);

  class svg_color
  /*! \brief SVG standard colors, see also enum svg_color_constant
      \details svg_color is the struct that contains information about RGB colors.
      For the constructor, the SVG standard specifies that numbers
      outside the normal rgb range are to be accepted,
      but are constrained to acceptable range of integer values [0, 255].
  */
  {
    friend std::ostream& operator<<(std::ostream& os, const svg_color& color);
    friend bool operator== (const svg_color& lhs, const svg_color& rhs);
    friend bool operator!= (const svg_color& lhs, const svg_color& rhs);
    friend void constant_to_rgb(svg_color_constant c, unsigned char& r, unsigned char& g, unsigned char& b);
    friend svg_color constant_to_rgb(svg_color_constant c);
    friend bool is_blank(const svg_color& col);

  public: //temporary for experimental gil
//  private:
    unsigned char r_; //!< red unsigned char provides range [0 to 255].
    unsigned char g_; //!< green unsigned char provides range [0 to 255].
    unsigned char b_; //!< blue unsigned char provides range [0 to 255].
    bool is_blank_; //!< true means "Not to be displayed" a 'pseudo-color'. If is_blank_ == true should write output to SVG XML file as "none".

  public:

    svg_color(int red, int green, int blue) : is_blank_(false)
    {  /*! \brief Construct an SVG color from RGB values.
       \details Constrain rgb to [0 .. 255].
       Default is to construct a 'pseudo-color' blank.
       */
      red = ( red < 0 ) ? 0 : red;
      green = ( green < 0 ) ? 0 : green;
      blue = ( blue < 0 ) ? 0 : blue;
      r_ = (unsigned char)(( red > 255 ) ? 255 : red);
      g_ = (unsigned char)(( green > 255 ) ? 255 : green);
      b_ = (unsigned char)(( blue > 255 ) ? 255 : blue);
    } // svg_color(int red, int green, int blue)

    svg_color(bool is) : is_blank_(!is)
    { //! Constructor from bool permits svg_color my_blank(false) as a (non-)color.
      /*! \details with same effect as svg_color my_blank(blank);
      svg_color(true) means default (black?)
      svg_color(false) means blank.
      For example:
      plot.area_fill(false) will be a blank == no fill.
      plot.area_fill(true) will be a default(black) fill.
      */
      r_ = 0; // Safer to assign *some* value to rgb: zero, or 255 or something,
      g_ = 0; // rather than leaving them random.
      b_ = 0; // Default 'blank' color 0,0,0 is black.
    }  //  svg_color(bool is)

    svg_color(svg_color_constant col)
    { //! Set a color (including blank) using the SVG 'standard' colors defined in enum boost::svg::svg_color_constant
      if (col == blank)
      { // NotAColor.
        is_blank_ = true;
        r_ = 255; // Safer to assign *some* value to rgb: zero, or 255 or something,
        g_ = 255; // rather than leaving them random.
        b_ = 255; // Default 'blank' color here is white.
      }
      else
      { // Proper color.
        is_blank_ = false;
        constant_to_rgb(col, r_, g_, b_);
      }
    }

    void write(std::ostream& os)
    { //! Write to ostream a color in svg format.
      //! \details Usage: my_color.write(cout); Outputs: rgb(127,255,212)
      if(!is_blank_)
      {
        os << "rgb(" //! Note lower case (whereas operator<< uses uppercase).
          << (unsigned int)r_ << ","
          << (unsigned int) g_ << ","
          << (unsigned int)b_ << ")" ;
      }
      else
      {
        os << "none";
      }
    } // void write(std::ostream& os)

    bool operator== (const svg_color& rhs)
    { //! Compare colors (for equal).
      if ((is_blank_) && (rhs.is_blank_ == true))
      { // Both blank
        return true;
      }
      return (r_ == rhs.r_) && (g_ == rhs.g_) && (b_ == rhs.b_);
    }

    bool operator!= (const svg_color& rhs)
    { //! Compare colors (for not equal).
      if ((is_blank_) || (rhs.is_blank_ == true))
      {
        return true;
      }
      return (r_ != rhs.r_) || (g_ != rhs.g_) || (b_ != rhs.b_);
    }

    bool is_blank() const
    { //! \return true if color is blank.
      return is_blank_;
    }

    unsigned int red() const
    { //! return red component of color [0, 255]
      return r_;
    }

    unsigned int green() const
    { //! return green component of color [0, 255]
      return g_;
    }

    unsigned int blue() const
    { //! return blue component of color [0, 255]
      return b_;
    }

  }; // class svg_color

  // Note operator== and operator<< are both needed to use Boost.Test.
  bool operator== (const svg_color& lhs, const svg_color& rhs)
  { //! Compare colors (for equal).
    // Note operator== and operator << both needed to use Boost.Test.
    if ((rhs.is_blank_ == true) && (rhs.is_blank_ == true))
    { // Both blank.
      return true;
    }
    return (lhs.r_ == rhs.r_) && (lhs.g_ == rhs.g_) && (lhs.b_ == rhs.b_);
  }

  bool operator!= (const svg_color& lhs, const svg_color& rhs)
  { //! Compare colors (for not equal).
    // Note operator== and operator << both needed to use Boost.Test.
    if ((rhs.is_blank_ == true) || (rhs.is_blank_ == true))
    { // Either blank.
      return true;
    }
    return (lhs.r_ == rhs.r_) || (lhs.g_ == rhs.g_) || (lhs.b_ == rhs.b_);
  }

  bool is_blank(const svg_color& col)
  { //! \return true if color is blank.
    return col.is_blank_;
  }

  std::ostream& operator<<(std::ostream& os, const svg_color& color)
  { /*!
      \brief Output color to stream as RGB. See boost::svg::svg_color_constant
      \details for example: "RGB(138, 43 , 226)" for blueviolet.
      This comment does not appear - for reasons entirely unclear.
    */
    if(!color.is_blank_)
    {
      os << "RGB(" // Note deliberate uppercase to show difference between write and operator<<
        << (unsigned int)color.r_ << ","
        << (unsigned int)color.g_ << ","
        << (unsigned int)color.b_ << ")";
    }
    else
    {
      os << "blank";
    }
    /*! \details Usage: svg_color my_color(127, 255, 212); cout << "my_color " << my_color << endl;
         Outputs: my_color RGB(127,255,212)       cout << "magenta " << svg_color(magenta) << endl;
         but caution! cout << magenta << endl; outputs 85 because magenta is an enum boost::svg::svg_color_constant !
     */
    return os;
  } // std::ostream& operator<<

  //! SVG standard colors, \see svg_color_constant
  svg_color color_array[] =
  {
    svg_color(240, 248, 255), // aliceblue
    svg_color(250, 235, 215), // antiquewhite
    svg_color(0  , 255, 255), // aqua
    svg_color(127, 255, 212), // aquamarine [4]
    svg_color(240, 255, 255), // azure
    svg_color(245, 245, 220), // beige
    svg_color(255, 228, 196), // bisque
    svg_color(0  , 0  , 0  ), // black
    svg_color(255, 235, 205), // blanchedalmond
    svg_color(0  , 0  , 255), // blue
    svg_color(138, 43 , 226), // blueviolet
    svg_color(165, 42 , 42 ), // brown
    svg_color(222, 184, 135), // burlywood
    svg_color(95 , 158, 160), // cadetblue
    svg_color(127, 255, 0  ), // chartreuse
    svg_color(210, 105, 30 ), // chocolate
    svg_color(255, 127, 80 ), // coral
    svg_color(100, 149, 237), // cornflowerblue
    svg_color(255, 248, 220), // cornsilk
    svg_color(220, 20 , 60 ), // crimson
    svg_color(0  , 255, 255), // cyan
    svg_color(0  , 0  , 139), // darkblue
    svg_color(0  , 139, 139), // darkcyan
    svg_color(184, 134, 11 ), // darkgoldenrod
    svg_color(169, 169, 169), // darkgray
    svg_color(0  , 100, 0  ), // darkgreen
    svg_color(169, 169, 169), // darkgrey
    svg_color(189, 183, 107), // darkkhaki
    svg_color(139, 0  , 139), // darkmagenta
    svg_color(85 , 107, 47 ), // darkolivegreen
    svg_color(255, 140, 0  ), // darkorange
    svg_color(153, 50 , 204), // darkorchid
    svg_color(139, 0  , 0  ), // darkred
    svg_color(233, 150, 122), // darksalmon
    svg_color(143, 188, 143), // darkseagreen
    svg_color(72 , 61 , 139), // darkslateblue
    svg_color(47 , 79 , 79 ), // darkslategray
    svg_color(47 , 79 , 79 ), // darkslategrey
    svg_color(0  , 206, 209), // darkturquoise
    svg_color(148, 0  , 211), // darkviolet
    svg_color(255, 20 , 147), // deeppink
    svg_color(0  , 191, 255), // deepskyblue
    svg_color(105, 105, 105), // dimgray
    svg_color(105, 105, 105), // dimgrey
    svg_color(30 , 144, 255), // dodgerblue
    svg_color(178, 34 , 34 ), // firebrick
    svg_color(255, 250, 240), // floralwhite
    svg_color(34 , 139, 34 ), // forestgreen
    svg_color(255, 0  , 255), // fuchsia
    svg_color(220, 220, 220), // gainsboro
    svg_color(248, 248, 255), // ghostwhite
    svg_color(255, 215, 0  ), // gold
    svg_color(218, 165, 32 ), // goldenrod
    svg_color(128, 128, 128), // gray
    svg_color(128, 128, 128), // grey
    svg_color(0  , 128, 0  ), // green
    svg_color(173, 255, 47 ), // greenyellow
    svg_color(240, 255, 240), // honeydew
    svg_color(255, 105, 180), // hotpink
    svg_color(205, 92 , 92 ), // indianred
    svg_color(75 , 0  , 130), // indigo
    svg_color(255, 255, 240), // ivory
    svg_color(240, 230, 140), // khaki
    svg_color(230, 230, 250), // lavender
    svg_color(255, 240, 245), // lavenderblush
    svg_color(124, 252, 0  ), // lawngreen
    svg_color(255, 250, 205), // lemonchiffon
    svg_color(173, 216, 230), // lightblue
    svg_color(240, 128, 128), // lightcoral
    svg_color(224, 255, 255), // lightcyan
    svg_color(250, 250, 210), // lightgoldenrodyellow
    svg_color(211, 211, 211), // lightgray
    svg_color(144, 238, 144), // lightgreen
    svg_color(211, 211, 211), // lightgrey
    svg_color(255, 182, 193), // lightpink
    svg_color(255, 160, 122), // lightsalmon
    svg_color(32 , 178, 170), // lightseagreen
    svg_color(135, 206, 250), // lightskyblue
    svg_color(119, 136, 153), // lightslategray
    svg_color(119, 136, 153), // lightslategrey
    svg_color(176, 196, 222), // lightsteelblue
    svg_color(255, 255, 224), // lightyellow
    svg_color(0  , 255, 0  ), // lime
    svg_color(50 , 205, 50 ), // limegreen
    svg_color(250, 240, 230), // linen
    svg_color(255, 0  , 255), // magenta
    svg_color(128, 0  , 0  ), // maroon
    svg_color(102, 205, 170), // mediumaquamarine
    svg_color(0  , 0  , 205), // mediumblue
    svg_color(186, 85 , 211), // mediumorchid
    svg_color(147, 112, 219), // mediumpurple
    svg_color(60 , 179, 113), // mediumseagreen
    svg_color(123, 104, 238), // mediumslateblue
    svg_color(0  , 250, 154), // mediumspringgreen
    svg_color(72 , 209, 204), // mediumturquoise
    svg_color(199, 21 , 133), // mediumvioletred
    svg_color(25 , 25 , 112), // midnightblue
    svg_color(245, 255, 250), // mintcream
    svg_color(255, 228, 225), // mistyrose
    svg_color(255, 228, 181), // moccasin
    svg_color(255, 222, 173), // navajowhite
    svg_color(0  , 0  , 128), // navy
    svg_color(253, 245, 230), // oldlace
    svg_color(128, 128, 0  ), // olive
    svg_color(107, 142, 35 ), // olivedrab
    svg_color(255, 165, 0  ), // orange
    svg_color(255, 69 , 0  ), // orangered
    svg_color(218, 112, 214), // orchid
    svg_color(238, 232, 170), // palegoldenrod
    svg_color(152, 251, 152), // palegreen
    svg_color(175, 238, 238), // paleturquose
    svg_color(219, 112, 147), // palevioletred
    svg_color(255, 239, 213), // papayawhip
    svg_color(255, 218, 185), // peachpuff
    svg_color(205, 133, 63 ), // peru
    svg_color(255, 192, 203), // pink
    svg_color(221, 160, 221), // plum
    svg_color(176, 224, 230), // powderblue
    svg_color(128, 0  , 128), // purple
    svg_color(255, 0  , 0  ), // red
    svg_color(188, 143, 143), // rosybrown
    svg_color(65 , 105, 225), // royalblue
    svg_color(139, 69 , 19 ), // saddlebrown
    svg_color(250, 128, 114), // salmon
    svg_color(244, 164, 96 ), // sandybrown
    svg_color(46 , 139, 87 ), // seagreen
    svg_color(255, 245, 238), // seashell
    svg_color(160, 82 , 45 ), // sienna
    svg_color(192, 192, 192), // silver
    svg_color(135, 206, 235), // skyblue
    svg_color(106, 90 , 205), // slateblue
    svg_color(112, 128, 144), // slategray
    svg_color(112, 128, 144), // slategrey
    svg_color(255, 250, 250), // snow
    svg_color(0  , 255, 127), // springgreen
    svg_color(70 , 130, 180), // steelblue
    svg_color(210, 180, 140), // tanned
    svg_color(0  , 128, 128), // teal
    svg_color(216, 191, 216), // thistle
    svg_color(255, 99 , 71 ), // tomato
    svg_color(64 , 224, 208), // turquoise
    svg_color(238, 130, 238), // violet
    svg_color(245, 222, 179), // wheat
    svg_color(255, 255, 255), // white
    svg_color(245, 245, 245), // whitesmoke
    svg_color(255, 255, 0  ), // yellow
    svg_color(154, 205, 50 ), // yellowgreen
    svg_color(true)          // blank - "Not to be displayed" pseudo-color.
  }; // svg_color color_array[]

  void constant_to_rgb(svg_color_constant c, unsigned char& r, unsigned char& g, unsigned char& b)
  { /*! Convert a named SVG standard color, see enum boost::svg::svg_color_constant
      to update three variables (r, g, b) holding red, green and blue values.
      Asserts that c NOT the blank color.
    */
    BOOST_ASSERT(c != blank);
    svg_color color(color_array[c]);
    r = color.r_;
    g = color.g_;
    b = color.b_;
  } // void constant_to_rgb

  svg_color constant_to_rgb(svg_color_constant c)
  { /*! Convert a svg color constant enum boost::svg::svg_color_constant to a svg_color.
    \return svg_color
      Example:
      constant_to_rgb(4) or constant_to_rgb(aquamarine)
      gives svg_color(127, 255, 212) // aquamarine.
    */
    // This comment appears OK.
    return color_array[c];
  } // svg_color constant_to_rgb(svg_color_constant c)

} // svg
} // boost

#endif // BOOST_SVG_SVG_COLOR_HPP
