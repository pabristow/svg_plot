/*! \file
   \brief Boost.Plot SVG plot Implementation details.
   \details See svg.hpp etc for user functions.
      svg_tag.hpp defines all classes that can occur in the SVG parse tree.
*/

// svg_tag.hpp

// Copyright Jacob Voytko 2007, 2008
// Copyright Paul A Bristow 2007, 2008, 2009, 2012, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_TAG_HPP
#define BOOST_SVG_TAG_HPP

#include <boost/ptr_container/ptr_container.hpp>
// using boost::vec_ptr;
#include <boost/array.hpp>
// using boost::array;

#include "../svg_style.hpp"
#include "svg_style_detail.hpp" // 

#include <ostream>
// using std::ostream;
#include <string>
// using std::string;
#include <vector>
// using std::vector;

namespace boost
{
namespace svg
{
  // Forward declarations of classes defined in this module.
  // Also copied to svg_fwd.hpp.

  class svg_element; // svg_element is base class for:
  // g_element (group element),
  // rect_element, circle_element, line_element, text_element,
  // polyline_element, polygon_element, path_element, clip_path_element,
  class text_parent; // Ancestor to both tspan and strings for the text_element class.
  class text_element_text;
  class tspan_element; // Within a text_element, adjust text and font properties.
  class text_element; // text with position, size, font, (& styles) & orientation.
  class rect_element; // clipping path restricts the region to which paint can be applied.
  class circle_element; // Represents a single circle.
  class ellipse_element; // Represents a single ellipse.
  class line_element; // Represents a single line.
  struct path_point; // Base class for m_path, z_path, q_path, h_path, v_path, c_path, s_path.
  struct poly_path_point; // for polyline & polygon
  class polygon_element; // closed shape consisting of a set of connected straight line segments.
  class polyline_element; // a set of connected straight line segments.
  class path_element; // d= moveto, lineto...
  class clip_path_element; // Restricts the region to which paint can be applied.
  struct m_path; // moveto coordinates (x, y), outputs "M1.2,3.4"
  struct l_path; // lineto coordinates (x, y).
  struct z_path; // z indicates a closepath.
  struct h_path; // Draws a horizontal line from the current point (cpx, cpy) to (x, cpy).
  struct v_path; // Draws a vertical line from the current point (cpx, cpy) to (cpx, y).
  struct c_path; // Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1).
  struct q_path; // Draws a quadratic Bezier curve from the current point to (x,y).
  struct s_path; // Draws a cubic Bezier curve from the current point to (x,y).
  struct t_path; // Draws a quadratic Bezier curve from the current point to (x,y).
  struct a_path; // Draws a elliptical arc from the current point to (x,y).
  struct P_path; // Adds another (absolute) point to a polyline or polygon.
  class g_element; /* 
  'g' element is a container element, for grouping together related graphics elements, for example:
   <g stroke="rgb(255,0,0)" <rect x="0" y="0"  width="500"  height="600"/> </g>
   */

  class svg_element
  { /*! \class boost::svg::svg_element
       \brief svg_element is base class for all the leaf elements @c g_element, @c rect_element.. .
       \details
       \code g_element, rect_element, circle_element, line_element, polygon_element, polyline_element, path_element, clip_path_element, text_element, tspan_element.\n
       \endcode
     */
  public:
    svg_style svg_style_; //!< Colors fill, stroke, width; set and get by function @c style().
    std::string id_name_; //!< SVG id name, set & get by function id.
    std::string class_name_; //!< SVG class name, set & get by function class id.
    std::string clip_name_; //!< SVG clip path name, set & get by function clip_id.

    void write_attributes(std::ostream& os)
    { //! Output group_element id and clip-path.
      //! Example: <clipPath id="plot_window"><rect x="53.6" y="40.5" width="339" height="328"/></clipPath>
      if(id_name_.size() != 0)
      { // Is an id_name, so output.
        os << " id=\"" << id_name_ << "\""; // Prefix with space.
      // Example: id_name ="imageBackground"
      }
      if(class_name_.size() != 0)
      { // Is a class_name, so output.
        os << " class=\"" << class_name_ << "\"";
      }
      if(clip_name_.size() != 0)
      { // Is a clip_name, so output.
        os << " clip-path=\"url(#" << clip_name_ << ")\""; // Prefix with space.
        // Might be nicer to suffix with newline - but after the >
      }
      // should transform be here allow translate and rotate?
      /*! \details
        Classes inherited from svg_element add other references, see section 5.3.1, like color, fill, stroke, gradients...
        */
      /*
        \verbatim
          Example id: <g id="yMinorGrid" ></g>
          Example class: <g class="grid_style"></g>
          Example URI: fill="url(#Gradient01) // local URL
        \endverbatim
      */
    } // void write_attributes(std::ostream& os)

    svg_element(const svg_style& style_info,
                const std::string& id_name = "",
                const std::string& class_name = "",
                const std::string& clip_name = "")
                :
                svg_style_(style_info),
                id_name_(id_name),
                class_name_(class_name),
                clip_name_(clip_name)
    { //! Constructor with some defaults.
    }

    svg_element()
    { //! Default constructor.
    }

    virtual void write(std::ostream& os) = 0; //!< Write function outputs appropriate SVG commands for a leaf or child element.

    virtual ~svg_element()
    { //! Destructor.
    }

    bool operator==(const svg_element& lhs)
    { //! Compare svg_elements, useful for Boost.Test.
      return lhs.id_name_ == id_name_;
    }

    bool operator!=(const svg_element& lhs)
    { //! Compare svg_elements for inequality, useful for Boost.Test.
      return lhs.id_name_ != id_name_;
    }

    // Set and get member functions.
    svg_style& style()
    { //! \return  Reference to @c svg_style to provide indirect access to colors & width 
      //! via style().stroke_color(), fill_color(), width() etc.
      return svg_style_;
    }

    // const version.
    const svg_style& style() const
    { //! \return Reference to @c const @c svg_style to provide indirect access to colors & width
      //! via style().stroke_color(), fill_color(), width() (const version).
      return svg_style_;
    }

    void id(const std::string& id)
    { //! Provide a unique name for an element. Example: <g id="plotBackground"

      /*! \details
        See http://www.w3.org/TR/SVG/struct.html#IDAttribute
        5.10.1 Attributes common to all elements: id and xml:base
        The id and xml:base attributes are available on all SVG elements:
        Attribute definitions:
        id = "name"
        Standard XML attribute for assigning a unique name to an element.
        Refer to the "Extensible Markup Language (XML) 1.0" Recommendation [XML10].
        xml:base = "<uri>"
        Specifies a base URI other than the base URI of the document or external entity.
        Refer to the "XML Base" specification [XML-BASE].
        A group of elements, as well as individual objects can be given a name using the id attribute.
        Named groups are needed for several purposes such as animation and re-usable objects.
      */
      id_name_ = id;
   }

    std::string id()
    { //! \return  The unique name for an element, for example id() ="plotBackground".
      return id_name_;
    }

    void class_id(const std::string& class_id)
    { //! Set @c class_id, non-unique string identifier for a Class Attribute.
      /*! \details
        http://www.w3.org/TR/2001/REC-SVG-20010904/styling.html#ClassAttribute
        6.12 Attributes common to all elements: id and xml:base
        Example: class_id == "info"
      */
      class_name_ = class_id;
    }

    std::string class_id()
    { //! \return Class id, non-unique string identifier for an element.
      return class_name_;
    }

    void clip_id(const std::string& id)
    { //! Set name of a clip path, for example: g_ptr.clip_id(plot_window_clip_);
      clip_name_ = id;
    }

    std::string clip_id()
    { //! \return  Name of a clip path, for example: \code g_ptr.clip_id(plot_window_clip_); \endcode
      return clip_name_;
    }
  }; // class svg_element

  // Derived elements whose write member functions
  // output SVG XML for line, circle, rectangle, text...
  // Reminder: Within a literal C string, \"  is needed to output a " ;-)

  // -----------------------------------------------------------------
  // Represents a straight line
  // -----------------------------------------------------------------
  class line_element: public svg_element
  { /*! \class boost::svg::line_element
        \brief Line from (x1, y1) to (x2, y2).
        /details Straight line from SVG location (x1, y1) to (x2, y2).

    */
  public:
    double x1_; //!< Line from (x1_, x2_) to (y1_, y2_)
    double y1_; //!< Line from (x1_, x2_) to (y1_, y2_)
    double x2_; //!< Line from (x1_, x2_) to (y1_, y2_)
    double y2_; //!< Line from (x1_, x2_) to (y1_, y2_)

    line_element(double x1, double y1, double x2,  double y2)
      :   x1_(x1), x2_(x2),  y1_(y1), y2_(y2)
    { //! Constructor assigning all line_element private data.
    }

    line_element(double x1, double y1,
                 double x2, double y2,
                 // Inherited from svg_element:
                 const svg_style& style_info, // No default?
                 const std::string& id_name = "",
                 const std::string& class_name = "",
                 const std::string& clip_name = "")
                : x1_(x1), y1_(y1), x2_(x2),y2_(y2),
                  svg_element(style_info, id_name, class_name, clip_name)
    { //! Constructor assigning all line_elementdata,
      //! and also inherited svg_element data.
    }

    void write(std::ostream& rhs)
    { /*! output line from (x1_, y1_) to (x2_, y2_) by
       writing XML SVG command to draw a straight line.
       */
      /* \verbatim Example: <line x1="5" y1="185" x2="340" y2="185"/> \endverbatim */
      rhs << "\t"  "<line x1=\"" << x1_ << "\" y1=\"" << y1_
          << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>" "\n";
    }
  }; // class line_element


  std::ostream& operator<< (std::ostream& os, const line_element& l)
  { //! Diagnostic output of line coordinates.
    //! Example: line_element l(20, 20, 50, 50);  std::cout << l << std::endl;
    //! Outputs:  rect(20, 20, 50, 50)
    os << "rect(" << l.x1_ << ", " << l.y1_
      << ", " << l.x2_ << ", " << l.y2_ << ")";
    return os;
  } // std::ostream& operator<<

  // Represents a curve (quadratic)
  class qurve_element: public svg_element
  { /*! \class boost::svg::qurve_element
        \brief Quadratic Bezier curved line from (x1, y1) control point (x2, y2) to (x3, y3).
        \details Note x2 is the Bezier control point - the curve will \b not pass thru this point.
    */
  public: 
    double x1_; //!< Quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (y3_, y3_).
    double x2_; //!< Quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (y3_, y3_).
    double y1_; //!< Quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (y3_, y3_).
    double y2_; //!< Quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (y3_, y3_).
    double x3_; //!< Quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (y3_, y3_).
    double y3_; //!< Quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (y3_, y3_).

    // bool fill; now inherited from parent svg class.
    qurve_element(double x1, double y1, double x2,  double y2, double x3,  double y3)
      :   x1_(x1), y1_(y1), x2_(x2), y2_(y2),  x3_(x3), y3_(y3)
    { //!< Quadratic curved line constructor (info inherited from parent svg_element class).
    }

    qurve_element(double x1, double y1,
                 double x2, double y2, // Control point - will not pass thru this point.
                 double x3, double y3,
                 // Inherited from svg_element:
                 const svg_style& style_info,
                 const std::string& id_name="",
                 const std::string& class_name="",
                 const std::string& clip_name = "")
                : x1_(x1),x2_(x2),  y1_(y1), y2_(y2), x3_(x3), y3_(y3),
                  svg_element(style_info, id_name, class_name, clip_name)
    { //!< Quadratic curved line constructor, including svg_element info.
    }

    void write(std::ostream& os)
    { /*! output quadratic curved line from (x1_, y1_) control point (x2_, y2_) to (x3_, y3_)
       \details
          \verbatim Example:
          \endverbatim
      */
      os << "<path d=\"M" << x1_ << "," << y1_
          << " Q" << x2_ << "," << y2_ << " " // Control point - will not pass thru this point.
          //<< x1_ << "," << y1_ << " "
          //<< x2_ << "," << y2_ << " "
          << x3_ << "," << y3_
          <<"\"";
      if(svg_style_.fill_on() == false)
      {
        os << " fill = \"none\"";
      }
      os<<"/>";
    }
  }; // class qurve_element

  class rect_element : public svg_element
  { /*! \class boost::svg::rect_element
        \brief Rectangle from top left coordinate, width and height.
        \details
         Represents a single rectangle.
         http://www.w3.org/TR/SVG/shapes.html#RectElement
    */
    friend bool operator==(const rect_element&, const rect_element&);
    friend bool operator!=(const rect_element&, const rect_element&);
  public: 
    double x_; //!< X-axis coordinate of the side of the rectangle which has the smaller x-axis coordinate value.
    double y_; //!< Y-axis coordinate of the side of the rectangle which has the smaller y-axis coordinate value.
    //!< So (0, 0) is top left corner of rectangle.
    double width_; //!< x_ + width_ is top right.
    double height_; //!< y_ + height_ is bottom left.
    //!< x_ + width_ and y_ + height_ is bottom right.
  public:

    rect_element(double x, double y, double w, double h)
      : x_(x), y_(y), width_(w), height_(h)
    { //! Constructor defines all private data (no defaults).
    }

    rect_element(double x, double y, double w, double h,
                // Inherited from svg_element:
                 const svg_style& style_info,
                 const std::string& id_name,
                 const std::string& class_name,
                 const std::string& clip_name)
      :
        svg_element(style_info, id_name, class_name, clip_name),
        x_(x), y_(y), width_(w), height_(h)
    { //! Constructor defines all private data (inherites info from svg_element).
    }

    double x() const
    { //! x-axis coordinate of the side of the rectangle which has the smaller x-axis coordinate value.
      return x_;
    }

    double y() const
    { //! y-axis coordinate of the side of the rectangle which has the smaller y-axis coordinate value.
      return y_;
    }

    double width() const
    { //! x + width is top right.
      return width_;
    }

    double height() const
    { //! y + height is bottom left.
      return height_;
    }

    bool operator==(const rect_element& lhs)
    { //! Comparison (useful for Boost.Test).
      return (lhs.x() == x_) && (lhs.y() == y_) &&  (lhs.width() == width_) && (lhs.height() == height_);
    }
    bool operator!=(const rect_element& lhs)
    { //!< Comparison rect_elements (useful for Boost.Test).
      return (lhs.x() != x_) || (lhs.y() != y_) ||  (lhs.width() != width_) || (lhs.height() != height_);
    }

    void write(std::ostream& os)
    { /*!
      Output SVG XML commands to draw a rectangle.
      \verbatim
        For example: <rect  x="0" y="0"  width="500"  height="350"/>
      \endverbatim
      Example: \code <rect x="0" y="0" width="500" height="600"/> \endcode
      */
      os << "\t<rect";
      write_attributes(os); // id (& clip_path)
      os << " x=\"" << x_ << "\""
        << " y=\"" << y_ << "\""
        << " width=\"" << width_ << "\""
        << " height=\"" << height_ << "\"/>"
        "\n";
    }
  }; // class rect_element

  // Not sure why these are free functions and not class members?  TODO
  bool operator==(const rect_element& lhs, const rect_element& rhs)
  { //! Compare equality of two SVG rect_elements.
    return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y()) && (lhs.width() == rhs.width()) && (lhs.height() == rhs.height());
  }

  bool operator!=(const rect_element& lhs, const rect_element& rhs)
  { //! Compare inequality of two SVG rect_elements.
    return (lhs.x() != rhs.x()) || (lhs.y() == rhs.y()) || (lhs.width() == rhs.width()) || (lhs.height() == rhs.height());
  }

  std::ostream& operator<< (std::ostream& os, const rect_element& r)
  { //! Diagnostic output of rectangle coordinates and dimensions.
    //! Example: rect_element r(20, 20, 50, 50);  std::cout << r << std::endl;
    //! Outputs:  rect(20, 20, 50, 50)
      os << "rect(" << r.x() << ", " << r.y()
         << ", " << r.width() << ", " << r.height() << ")" ;
    return os;
  } // std::ostream& operator<<

  // -----------------------------------------------------------------
  // class circle_element  Represents a single circle.
  // -----------------------------------------------------------------
  class circle_element : public svg_element
  {/*! \class boost::svg::circle_element
        \brief Circle from coordinates of center, and radius.
        \details
         Represents a single circle.
         \sa http://www.w3.org/TR/SVG/shapes.html#CircleElement
    */
  public:
    double x_;
    double y_;
    double radius_;
    circle_element(double x,  //!< coordinate X of center of ellipse.
      double y, //!< coordinate Y of center of ellipse.
      double radius = 5) //!< radius of ellipse.
      : x_(x), y_(y), radius_(radius)
    { //! Constructor defines all private data (default radius only).
    }

    circle_element(double x, double y, double radius,
                 const svg_style& style_info,
                 const std::string& id_name="",
                 const std::string& class_name="",
                 const std::string& clip_name=""
                 )
      :
        x_(x), y_(y),
        svg_element(style_info, id_name, class_name, clip_name), radius_(radius)
    { //! Constructor defines all private data.
    }

    void write(std::ostream& os)
    { /*! Output SVG XML commands to draw a circle:
    \verbatim
       Example: <circle cx="9.78571" cy="185" r="5"/>
    \endverbatim
    */
      os << "\t<circle";
      write_attributes(os);
      os << " cx=\"" << x_ << "\" cy=\"" << y_ << "\" r=\"" << radius_ << "\"/>" "\n";
    }
  }; // class circle_element

  std::ostream& operator<< (std::ostream& os, const circle_element& c)
  { //! Diagnostic output of circle center X and Y coordinates and radius.
    //! Example: circle_element c(20, 20, 40);  std::cout << c << std::endl;
    //! Outputs:  
    os << "circle(" << c.x_ << ", " << c.y_
      << ", " << c.radius_ << ")" "\n";
    return os;
  } // std::ostream& operator<<

  class ellipse_element : public svg_element
  { /*! \class boost::svg::ellipse_element
        \brief Ellipse from center coordinate, and radius.
        \details
        Represents a single ellipse.
        http://www.w3.org/TR/SVG/shapes.html#EllipseElement
        9.4 The 'ellipse'  element.
        Default is 'horizontal' but can be rotated.
        */
  public:
    double cx_; //!< coordinate x of center of ellipse, default 0.
    double cy_; //!< coordinate y, default 0.
    double rx_; //!< radius x, default 4 pixels.
    double ry_; //!< radius y, default 8 pixels.
    double rotate_; //! rotation in degrees from horizontal (default 0.).
    // Only hacked in - should be in attributes?
    ellipse_element(double cx, //!< coordinate X of center of ellipse.
      double cy, //!< coordinate Y  of center of ellipse.
      double rx = 4, //!< X radius default.
      double ry = 8) //!< Y radius default.
      : cx_(cx), cy_(cy), rx_(rx), ry_(ry), rotate_(0.)
    { //!< Constructor defines all private data (with default radii).
    }

    ellipse_element(double cx, //!< coordinate X of center of ellipse.
      double cy, //!< coordinate Y of center of ellipse.
      double rx, //!< radius X.
      double ry, //!< radius Y.
      const svg_style& style_info, //!< style (color and width)
      const std::string& id_name="", //!< ID of group, for example: PLOT_X_TICKS_VALUES.
      const std::string& class_name="", //!< Name of SVG class, for example: "grid_style".
      const std::string& clip_name="") //!< name of clip path.
      :
        svg_element(style_info, id_name, class_name, clip_name),
          cx_(cx), cy_(cy), rx_(rx), ry_(ry), rotate_(0.)
    { //!< Constructor sets ellipse and its style (defaults define all private data).
    }
    ellipse_element(
        double cx, //!< coordinate X of center of ellipse.
        double cy, //!< coordinate Y of center of ellipse.
        const svg_style& style_info, //!< Style - Colors & widths.
        const std::string& id_name = "",  //!< ID of group, for example: PLOT_X_TICKS_VALUES.
        const std::string& class_name = "", //!< Name of SVG class."grid_style"
        const std::string& clip_name = "") //!< name of clip path.
      : cx_(cx), cy_(cy), rx_(4), ry_(8), // 4 and 8 are the same defaults used above.

        svg_element(style_info, id_name, class_name, clip_name),
        rotate_(0.) // Horizontal.
    {  //!< Constructor that also includes style, id, class and clip.
    }

    void write(std::ostream& os)
    { /*!
        Output SVG XML commands to draw an ellipse.
        Example: \<ellipse rx="250" ry="100" fill="red"  /\>
     */
      os << "\t<ellipse";
      write_attributes(os);
      if(rotate_ != 0)
      { // Should this be in atttributes?
        os << " transform= \"" << " rotate=(" << rotate_ << ")\"";
      }
      os << " cx=\"" << cx_ << "\" cy=\"" << cy_ << "\""
          << " rx=\"" << rx_ << "\" ry=\"" << ry_  << "\"/>" "\n";
    }
  }; // class ellipse_element

  enum class align_style
  { //! \enum align_style Represents a single block of text, with font & alignment.
    left_align, //!< Align text to left.
    right_align, //!< Align text to right.
    center_align //!< Center align text.
  };

  std::ostream& operator<< (std::ostream& os, align_style al)
  { //! Outputs: alignment (useful for diagnosis).
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
    else os << "???" << std::endl;
    return os;
  } //   std::ostream& operator<< (std::ostream& os, align_style al)

class text_parent
{ /*! \class boost::svg::text_parent
    \brief An ancestor to both tspan and strings for the text_element class.
    \details This allows an array of both types to be stored in @c text_element.
  */
  public:
    std::string text_; //!< Actual text string for SVG text.
    virtual ~text_parent() {}
    virtual void write(std::ostream& /* os */)
    { //! @c write functions output SVG commands to @c ostream.
    }
    text_parent(const std::string& text): text_(text)
    { //! Construct from string text.
      //! Used by text_element and tspan_element to store tex to be output.
    }
    text_parent(const text_parent& rhs): text_(rhs.text_)
    { //! Copy constructor.
    }
}; // class text_parent

class text_element_text : public text_parent
{ /*! \class boost::svg::text_element_text
  \brief text string to be stored in text_parent.
  \details See 10.4 text element http://www.w3.org/TR/SVG/text.html#TextElement
  */
public:
  text_element_text(const std::string& text): text_parent(text)
  { //! Construct from text string.
  }
  text_element_text(const text_element_text& rhs): text_parent(rhs)
  { //! Copy construct text element.
  }
  void write(std::ostream& os)
  { //! write text to stream.
    os << text_;
  }
}; // class text_element_text

std::ostream& operator<< (std::ostream& os, text_element_text& t)
{ //! Outputs: text & style (useful for diagnosis).
  //! Usage: text_element_text t("Some text);  std::cout << t << std::endl;
  os << "text_element_text(\"" << t.text_ << "\") " << std::endl;
  return os;
} // std::ostream& operator<<


class tspan_element : public text_parent, public svg_element
{ /*! \class boost::svg::tspan_element
    \brief tspan (not text_element) to be stored in text_parent.
    \details See 10.5 tspan element http://www.w3.org/TR/SVG/text.html#TSpanElement 
    Example: 

    */
public:
  double x_;  //!< Absolute X position.
  double y_;  //!< Absolute Y position.
  double dx_; //!< Relative X position of a 1st single character of text.
  double dy_; //!< Relative Y position of a 1st single character of text.
  int rotate_; //!< Rotation of a 1st single character of text.
  //! (A list of shifts or rotations for several characters is not yet implemented).
  double text_length_;  //!< Allows the author to provide exact alignment.
  //! dx_, dy_, and rotate_ can all be omitted, usually meaning no shift or rotation,
  //! but see http://www.w3.org/TR/SVG/text.html#TSpanElement for ancestor rules.
  //! (@c text_length_ only used if > 0).
  //! but x_, y_, need a flag?
  bool use_x_; //!> If true then use X absolute position.
  bool use_y_; //!> If true then use Y absolute position.
  //bool use_text_length_; //!< If true then use user calculated length rather than SVG (not used).
  text_style text_style_; //!< font variants.

  tspan_element(const std::string& text, //!< Text string to display.
    const text_style& textstyle = not_a_text_style) //!< Default text textstyle (font).
  :
    use_x_(false), use_y_(false), 
    text_parent(text), text_style_(textstyle),
    x_(0), y_(0), dx_(0), dy_(0), // X & Y coordinates, relative and absolute.
    rotate_(0),
    text_length_(0) // If text_length_ > 0 then compress or expand to this specified length.
  { //! Construct tspan element (with all defaults except text string).
  }

  tspan_element(const tspan_element& rhs); //!< Copy constructor.
    // TODO all may need refactoring to separate declaration from definition - as example below.

  //tspan_element(const tspan_element& rhs)   //!< Copy constructor.
  //  :
  //  x_(rhs.x_), y_(rhs.y_), dx_(rhs.dx_), dy_(rhs.dy_), rotate_(rhs.rotate_),
  //  text_length_(rhs.text_length_), use_x_(rhs.use_x_), use_y_(rhs.use_y_),
  //  use_text_length_(rhs.use_text_length_), text_style_(rhs.text_style_),
  //  text_parent(rhs)
  //{ // Copy all parameters
  //}

  // All setters (all chainable).
  //tspan_element(const std::string& text, const text_style& textstyle);
  //tspan_element(const tspan_element&); // Default textstyle.
  //tspan_element& text(const std::string& text);
  //tspan_element& dx(double dx);
  //tspan_element& dy(double dy);
  //tspan_element& rotation(int rotation);
  //tspan_element& x(double x);
  //tspan_element& y(double y);
  //tspan_element& text_length(double text_length);
  //tspan_element& text_style(const text_style& textstyle)

  tspan_element& text(const std::string& text)
  { //! Set text string to use with SVG tspan command.
    text_=text;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& dx(double dx)
  { //! Relative X position of a 1st single character of text string to use with SVG tspan command.
    dx_ = dx;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& dy(double dy)
  { //! Relative Y position of a 1st single character of text string to use with SVG tspan command.
    dy_ = dy;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& rotation(int rotation)
  { //!< Note implementation so far only rotates the 1st character in string.
    //!< text_element rotation rotates the whole text string, so it *much* more useful.
    rotate_ = rotation;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& x(double x)
  { //! Absolute X position of a 1st single character of text string to use with SVG tspan command.
    x_ = x;
    use_x_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& y(double y)
  {//! Absolute Y position of a 1st single character of text string to use with SVG tspan command.
    y_ = y;
    use_y_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& text_length(double text_length)
  { //! Set user estimate of text length (see http://www.w3.org/TR/SVG/text.html#TSpanElement TSPAN SVG Specification).
    text_length_ = text_length;
   // use_text_length_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& font_size(int size)
  { //! font size of 1st single character of text string to use with SVG tspan command.
    text_style_.font_size(size);
  //  use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& font_family(const std::string& family)
  {//! font family of 1st single character of text string to use with SVG tspan command.
    text_style_.font_family(family);
  //  use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& font_style(const std::string& textstyle)
  { //! font style of 1st single character of text string to use with SVG tspan command.
    //! font-style: normal | bold | italic | oblique
    //! Examples: "italic"
    //! http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-02-t.svg
    text_style_.font_style(textstyle);
  //  use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& font_weight(const std::string& w)
  { //! font weight of 1st single character of text string to use with SVG tspan command.
  //! svg font-weight: normal | bold | bolder | lighter | 100 | 200 .. 900
    //! Examples: "bold", "normal"
    //! http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-02-t.svg
    //! tests conformance.  Only two weights are supported by Firefox, Opera, Inkscape.
    text_style_.font_weight(w);
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& fill_color(const svg_color& color)
  { //! Set fill color for a tspan element.
    svg_style_.fill_color(color);  // svg_element svg_style style_info
    svg_style_.fill_on(true);
 //   use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& stroke_color(const svg_color& color)
  { //! Set stroke color for a tspan element.
    svg_style_.stroke_color(color);
    svg_style_.stroke_on(true);
 //   use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  tspan_element& stroke_width(double width)
  { //! Set stroke width for a tspan element.
    svg_style_.stroke_width(width);
    svg_style_.stroke_on(true);
 //   use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }


  tspan_element& textstyle(const text_style& textstyle)
  { //! Set text style (font) for a @c tspan_element.
    text_style_ = textstyle;
 //   use_style_ = true;
    return *this; //! \return @c tspan_element& to make chainable.
  }

  // All get functions.
  //tspan_element::std::string text();
  //double x();
  //double y();
  //double dx();
  //double dy();
  //int rotation();
  //double text_length();
  //text_style& font_style();
  //const text_style& text_style() const;

  // These class tspan member functions 
 text_style& text_style()
  { //! \return  Reference to @c text_style& to permit access to font family, size ...
    return text_style_;
  }

  //Const version won't compile - name clash?
  //const text_style& text_style() const
  //{ //! \return Reference to @c text_style& to permit access to font family, size (const version).
  //  return text_style_;
  //}

  svg_style& style()
  { //! \return  Reference to @c svg_style to provide indirect access to colors & width 
    //! via style().stroke_color(), .fill_color(), .width() etc.
    return svg_style_;
  }

  std::string text()
  { //! Get text from a tspan element.
    return text_;
  }
  double x()
  { //! Get absolute X position for tspan element.
    return x_;
  }

  double y()
  { //! Get absolute Y position for tspan element.
    return y_;
  }
  double dx()
  { //! Get relative X position for tspan element.
    return dx_;
  }

  double dy()
  { //! Get relative Y position for tspan element.
    return dy_;
  }

  int rotation()
  {  //! Get rotation for the next character for tspan element.
    return rotate_;
  }

  int font_size()
  { //! Get the font size for tspan element (from its text_style).
    return text_style_.font_size();
  }

  const std::string& font_family()
  { //! Get the font family for tspan element (from its text_style).
    return text_style_.font_family();
  }

  const std::string& font_weight() const
  { //! Get the font weight for tspan element (from its text_style).
    return text_style_.font_weight();
  }

  const std::string&  font_style()
  { //! Get the font style for tspan element (from its text_style).
    return text_style_.font_style();
  }

  const std::string&  font_style() const
  { //! Get the font style for tspan element (from its text_style). const version.
    return text_style_.font_style();
  }

  svg_color fill_color()
  { //! Get the fill color for tspan element from SVG style svg_element.
    return svg_style_.fill_color();
  }

  bool fill_on()
  { //! Get true if to use stroke color for tspan element from SVG style svg_element.
    return svg_style_.fill_on();
  }

  svg_color stroke_color()
  { //! Get the stroke color for tspan element from SVG style svg_element.
    return svg_style_.stroke_color();
  }

  bool stroke_on()
  { //! Get true if to use stroke color for tspan element from SVG style svg_element.
    return svg_style_.stroke_on();
  }

  bool width_on()
  { //! Get true if to use stroke width for tspan element from SVG style svg_element.
    return svg_style_.width_on();
  }

  double stroke_width()
  { //! Get stroke width for tspan element from SVG style svg_element.
    return svg_style_.stroke_width();
  }

  double text_length()
  { //! Get user's estimated length for a text string.
    //! This length may be used to expand or contract the SVG text to fit into this width,
    //! if text_length_ == 0, then has no effect as is not output by @c write below.
    //! If < 0 would be an error if output like textLength=-1
    return text_style_.text_length_;
  }
  
  void write(std::ostream& os)
  { //! Output SVG XML for a tspan_element.
    os << "\t" "<tspan";
    write_attributes(os); // id & clip_path
    svg_style_.write(os); // fill, stroke, width...

    // All of the conditional writes within tspan_element.

    // First, all elements that can be tested based on their value.
    if(rotate_ != 0)
    {
      os << " rotate=\"" << rotate_ << "\"";
    }
    if(dx_!= 0)
    {
      os << " dx=\"" << dx_ << "\"";
    }
    if(dy_!= 0)
    {
      os << " dy=\"" << dy_ << "\"";
    }

    // Now, add all elements that can be tested with the flags.
    if(use_x_ == true)
    {
      os << " x=\"" << x_ << "\"";
    }
    if(use_y_  == true)
    {
      os << " y=\"" << y_ << "\"";
    }
    // https://www.w3.org/TR/SVG11/text.html#FontPropertiesUsedBySVG
    // 10.10 Font selection properties
    // Text_style
    if (text_style_ != not_a_text_style)
    { // Want to output (repeat) text style info.
      if (text_style_.font_size() != 0)
      {
        os << " font-size=\"" << text_style_.font_size() << "\"";
      }
      if (text_style_.font_family() != "")
      { // Examples: Arial, Times New Roman.
        os << " font-family=\"" << text_style_.font_family() << "\"";
      }
      if (text_style_.font_style().size() != 0)
      { // Examples: 	normal | italic | oblique | inherit
        os << " font-textstyle=\"" << text_style_.font_style() << "\"";
      }
      if (text_style_.font_weight().size() != 0)
      { // Examples: normal | bold | bolder | lighter | 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900 |
        os << " font-weight=\"" << text_style_.font_weight() << "\"";
      }
      if (text_style_.font_stretch().size() != 0)
      { // Examples: normal | wider | narrower | ultra-condensed | extra-condensed | condensed | semi-condensed | semi-expanded | expanded | extra-expanded | ultra-expanded 
        os << " font-stretch=\"" << text_style_.font_stretch() << "\"";
      }
      // 10.12 Text decoration https://www.w3.org/TR/SVG11/text.html#TextDecorationProperties  
      if (text_style_.font_decoration().size() != 0)
      {
        // none | [ underline || overline || line-through || blink ] | inherit
        os << " text-decoration=\"" << text_style_.font_decoration() << "\"";
      }
      if (text_length_ > 0)
      { // Use estimated text length to expand or compress to the this SVG length.
        os << " textLength=\"" << text_length_ << "\"";
      }
    }
    os << ">" << text_ << "</tspan>" "\n";  // The actual text string.
  } //   void write(std::ostream& os)

  }; // class tspan_element

std::ostream& operator<< (std::ostream& os, const tspan_element& t)
{ //! Diagnostic output of tspan coordinates info, and text_style font info.
  //! Example:   std::cout << r << std::endl;
  //! Outputs:  
  //! \verbatim ts = tspan(0, 0, 10, 20, 0, 0, false, false, text_style(40, "Arial", "", "bold", "", "")) \endverbatim
  os << std::boolalpha << "tspan("
    << t.x_ << ", " << t.y_ 
    << ", " << t.dx_ << ", " << t.dy_
    << ", " << t.rotate_ << ", " << t.text_length_ 
    << ", " << (t.use_x_ ? "absolute" : "relative")
    << ", " << (t.use_y_ ? "absolute" : "relative") // 
    << ", " << t.text_style_  // text_style(40, "Arial", "", "bold", "", "")
    << ", " << t.svg_style_ // Inherited from svg_element.  (Why doesn't t.style() work?)
 //   << ", " << t.style() // Inherited from svg_element.  (Why doesn't t.style() work?)
    << ")";
  return os;
} // std::ostream& operator<<


//! Copy constructor.
tspan_element::tspan_element(const tspan_element& rhs)
    :
    text_length_(rhs.text_length_), use_x_(rhs.use_x_), use_y_(rhs.use_y_),
    text_parent(rhs), text_style_(rhs.text_style_),
    x_(rhs.x_), y_(rhs.y_), dx_(rhs.dx_), dy_(rhs.dy_), rotate_(rhs.rotate_)
  { // Separately defined constructor.
  } // tspan_element::tspan_element

/*! \class boost::svg::text_element
      \brief Holds text with position, size, font, (& styles) & orientation.
      \details
      SVG Coordinates of 1st character EM box, see http://www.w3.org/TR/SVG/text.html#TextElement 10.2.

      So any text with y coordinate == 0 shows only any roman lower case descenders!\n

      \verbatim
        (Text may contain embedded XML Unicode characters for Greek, math, emoji etc, for example: &#x3A9;).
      \endverbatim

      \n
      size; // Size (default pixels) of font to use for text. Diagnostics show: " font-size = "12".\n
      \sa http://www.w3.org/TR/SVG/text.html#CharactersAndGlyphs
      param std::string font;  // font-family: "Arial" | "Times New Roman" | "Verdana" | "Lucida Sans Unicode"
      "sans", "serif", "times"
      http://www.w3.org/TR/SVG/text.html#FontFamilyProperty 10.10 Font selection properties\n

      std::string style; // font-style: normal | bold | italic | oblique
      std::string weight; // font-weight: normal | bold | bolder | lighter | 100 | 200 .. 900
      std::string stretch; // font-stretch: normal | wider | narrower ...
      std::string decoration; // // "underline" | "overline" | "line-through"
      Example:
      \verbatim
        <text x="250" y="219.5" text-anchor="middle"  font-family="verdana" font-size="12">0 </text>
      \endverbatim
  */
class text_element : public svg_element
{
public:
  double x_; //!< Left edge of character.
  double y_; //!< Bottom of Roman capital character.
  ptr_vector<text_parent> data_; //!< Stores all of the containing text string data.
  text_style text_style_; //!< font variants.
  align_style align_; //!< Alignment: left_align, right_align, center_align.
  rotate_style rotate_; //!< Rotation: horizontal, upward, downward, upsidedown.

  void generate_text(std::ostream& os)
  { // Output SVG 
    for(ptr_vector<text_parent>::iterator i = data_.begin(); i != data_.end(); ++i)
    {
      (*i).write(os);
    }
  }
  // Set member functions.
  // void alignment(align_style a);
  // void rotation(rotate_style rot);
  // void x(double x);
  // void y(double y);
  // void text(const std::string& t);
  // tspan_element& tspan(const std::string& t);
  // text_element(double x, double y, const std::string text,text_style ts,align_style align, rotate_style rotate);
  // text_element(const text_element& rhs);

  // Get member functions.
  // text_style& textstyle();
  // const text_style& textstyle() const;
  // align_style alignment();
  // rotate_style rotation() const;
  // double x() const;
  // double y() const;

  text_style& textstyle()
  { //! Get @c text_element textstyle for font size, family, decoration ...
    return text_style_;
  }

  const text_style& textstyle() const
  { //! Get  @c text_element textstyle for font size, family, decoration ...
    return text_style_;
  }

  text_element& textstyle(text_style& ts)
  { //! Set  @c text_element text textstyle for font size, family, decoration ...
    text_style_ = ts;
    return *this; //! \return text_element& to make chainable.
  }

  text_element& alignment(align_style a) 
  { //! Set alignment: @c left_align, @c right_align, @c center_align.
    align_ = a;
    return *this; //! \return text_element& to make chainable.
  }

  align_style alignment()
  { //! left_align, right_align, center_align
    return align_;
  }

  text_element& rotation(rotate_style rot)
  { //! Degrees: horizontal  = 0, upward = -90, downward, upsidedown
    //! Generates: transform = "rotate(-45 100 100 )"
    rotate_ = rot;
    return *this; //! \return text_element& to make chainable.
  }

  rotate_style rotation() const
  { //! \return rotation of text element (degrees).
    return rotate_;
  }

  // All set functions now return *this to be chainable, for example:
  // my_text_element.textstyle(no_text_style).x(999).y(555).alignment(right_align).rotation(vertical);

  text_element& x(double x)
  { //! X coordinate of text to write.
    x_ = x;
    return *this; //! \return @c text_element& to make chainable.
  }

  double x() const
  { //! X coordinate of text to write.
    return x_;
  }

  text_element& y(double y)
  { //! Y coordinate of text to write.
    y_ = y;
    return *this; //! \return  @c text_element& to make chainable.
  }

  double y() const
  { //! Y coordinate of text to write.
    return y_;
  }

  void text(const std::string& t)
  { //! push_back tspan text-string to write.
    data_.push_back(new text_element_text(t));
  }

  tspan_element& tspan(const std::string& t)
  { //! Add text span element (using current text_style text_style_).
    //! data_ in member of text_parent.
    data_.push_back(new tspan_element(t, text_style_));
    return *(static_cast<tspan_element*>(&data_[data_.size()-1]));
  }

  // 
  tspan_element& tspan(const std::string& t, const text_style& textstyle)
  { //! Add text span element (with specified text_style textstyle).
    //! data_ in member of text_parent.
    data_.push_back(new tspan_element(t, textstyle));
    return *(static_cast<tspan_element*>(&data_[data_.size()-1]));
  }

  text_element(
    //!< Constructor, setting all data elements.
    //! X & Y-Coordinates of 1st character EM box, see
    //!< \sa http://www.w3.org/TR/SVG/text.html#TextElement 10.2
    double x = 0., //!< X = Left edge.
    double y = 0., //!< Y =  Bottom left of (western) character (roman capital).
    //!< So any text with Y coordinate = 0 shows only the descenders of (roman?) lower case !
    //!< One must increase Y to allow for the height (font size) of the character.
    const std::string text = "", //!< Text string to output (may include Unicode string like "&#x221A;" for square root symbol.
    text_style ts = no_text_style, //!< Text font text style, default left to SVG defaults.
    align_style align = align_style ::left_align, //!< Alighment of text, left, center or right, default left_align.
    rotate_style rotate = horizontal) //!< orientation of text, default horizontal.
    : // Constructor.
    x_(x), y_(y), // location.
    data_(ptr_vector<text_parent>()),
    text_style_(ts), // Uses copy constructor.
    //size_(size), font_(font), text_style_(textstyle), weight_(weight), stretch_(stretch), decoration_(decoration), text_length_(text_length)
    align_(align),
    rotate_(rotate)
  { //! text_element Default Constructor defines defaults for all class members.
    data_.push_back(new text_element_text(text)); // Adds new text string.
  }

  text_element(const text_element& rhs)
  :
    x_(rhs.x_), y_(rhs.y_), text_style_(rhs.text_style_), align_(rhs.align_), rotate_(rhs.rotate_)
  { //! Copy constructor.
     data_ = (const_cast<text_element&>(rhs)).data_.release();
  }

  text_element& operator=(const text_element& rhs)
  { //! Assignment operator.
    x_ = rhs.x_;
    y_ = rhs.y_;
    data_.clear(); // Copy data_
    data_.insert(data_.end(), rhs.data_.begin(), rhs.data_.end());
    text_style_ = rhs.text_style_; // font_size, family...
    align_ = rhs.align_;
    rotate_ = rhs.rotate_;
    return *this; //! \return @c text_element& to make chainable.
  }

  std::string text()
  { //! \return text @c std::string of a @c text_element.
    std::ostringstream os;
    generate_text(os);
    return os.str();
  }

  text_style text_style()
  {
    return text_style_; 
  }

   //! Output SVG @c text_element, style & attributes to a @c std::ostream.
   // Changing back to OLD convention on spaces:
   // NO trailing space, but *start* each item with a tab and end with newline.
   // 
   // Should now use easier to read SVG XML layout.
   // Example: \verbatim os << " <text x=\"" << x_ << "\" y=\"" << y_ << "\""; \endverbatim
  void write(std::ostream& os)
  {
    os << "\t<text x=\"" << x_ << "\" y=\"" << y_ << "\"";
    // ??? Test 
    std::string anchor;
    switch(align_)
    {
    case align_style::left_align :
      // anchor = "start"; // This is the initial == default.
      // so should be possible to reduce file size this by:
      anchor = "";
      break;
    case align_style::right_align :
      anchor = "end";
      break;
    case align_style::center_align :
      anchor = "middle";
      break;
    default:
      anchor = "";
      break;
    }
    if(anchor != "")
    {
      os << " text-anchor=\"" << anchor << "\"";
    }
    if(rotate_ != 0)
    { // Only show rotation info if not normal horizontal writing.
      os << " transform=\"rotate("
        << rotate_ << " "
        << x_ << " "
        << y_ << ")\"";
    }
    //if (text_style_.font_size() > 0 )
    //{ // 
      if (text_style_.font_size() > 0)
      { // Example output: font-size="10" 
        os << " font-size=\"" << text_style_.font_size() << "\"";
      }
      if (text_style_.font_family() != "")
      { // Example: Arial, Verdana, Times New Roman ...
        // Example output: font-family="serif"
        os << " font-family=\"" << text_style_.font_family() << "\"";
      }
      if (text_style_.font_style().size() != 0)
      { // Example: italic.
        os << " font-textstyle=\"" << text_style_.font_style() << "\"";
      }
      if (text_style_.font_weight().size() != 0)
      { // Example: bold.
      os << " font-weight=\"" << text_style_.font_weight() << "\"";
      }
      if (text_style_.font_stretch().size() != 0)
      {
      os << " font-stretch=\"" << text_style_.font_stretch() << "\"";
      }
      if (text_style_.font_decoration().size() != 0)
      {
      os << " text-decoration=\"" << text_style_.font_decoration() << "\"";
      }
      if (text_style_.text_length() > 0)
      {
        os << " textLength=\"" << text_style_.text_length() << "\"";
      }
 //   }
    os << ">" ;
    generate_text(os); 
    os << "\t" "</text>" "\n";
    // Example: <text x="67.5" y="462" text-anchor="middle" font-size="12" font-family="Lucida Sans Unicode">my_text!</text>
  } // void write(std::ostream& os)
}; // class text_element_

  std::ostream& operator<< (std::ostream& os, text_element& t)
  { //! Outputs: text & textstyle (useful for diagnosis).
    //! Example: 
    //! \code
    //!   text_element& t = my_doc.text(100, 100, "My Text", my_text_style, center_align, uphill);
    //    std::cout << "text_element = " << t << std::endl;
    //! \endcode
    // Outputs:  text_element = text_element(100, 100, text_style(20, "serif", "bold", "", "", ""), center, -45)
      os << "text_element(" << t.x_ << ", " << t.y_
        //<< ", "  << t.data_
        << ", " << t.text_style_ 
        << ", " << t.align_
        << ", " << t.rotate_ << ")" 
        << std::endl;
      ;
    return os;
  } // std::ostream& operator<<

  class clip_path_element: public svg_element
  {  /*! \class boost::svg::clip_path_element
      \brief The clipping path restricts the region to which paint can be applied.
      \details 14.3 Clipping paths http://www.w3.org/TR/SVG/masking.html#ClipPathProperty.
    */
  public: 
//  private:
    std::string element_id; //!< SVG element id.
    rect_element rect; //!< Clipping rectangle.

  public:

    clip_path_element(const std::string& id, const rect_element& rect)
      : element_id(id), rect(rect)
      { //!< Constructor defines all member variables.
      }

      void write(std::ostream& os)
      { //!< Write clip path to ostream.
        os << "<clipPath id=\"" << element_id << "\">";
        rect.write(os);
        os  << "</clipPath>" << std::endl;
      }
  }; // class clip_path_element

  struct path_point
  { /*! \struct boost::svg::path_point
      \brief Base class for m_path, z_path, q_path, h_path, v_path, c_path, s_path.
      \details Paths represent the outline of a shape which can be
      filled, stroked, used as a clipping path, or any combination of the three.
     */
    bool relative; //!< If true relative else absolute.

    virtual void write(std::ostream& rhs) = 0; //!< write functions output SVG commands like "M1.2, 3.4",
    virtual ~path_point()
    { //! Destructor.
    }

    path_point(bool relative) : relative(relative)
    { //!< Constructor defines all member variables.
    }
  }; // struct path_point


  struct m_path: public path_point
  { /*! \struct boost::svg::m_path
      \brief Move to coordinates (x, y)
     \details See 8.3.2 The "moveto" commands.
     */
    double x; //!< End of move SVG X coordinate.
    double y; //!< End of move SVG Y coordinate.

    void write(std::ostream& os)
    { //! Write moveto X and Y coordinates to stream, for example: "M52.8571,180 "
      if(relative)
      {
        os << "m";
      }
      else
      { // absolute
        os << "M";
      }
      os << x << "," << y << " "; // separator changed to comma for clarity when reading XML source.

    } // void write(std::ostream& os)

    m_path(double x, double y, bool relative = false)
      : path_point(relative),
        x(x), y(y)
    { //! Construct a move to
    }
  }; // struct m_path

  struct z_path: public path_point
  { /*! \struct boost::svg::z_path
       \brief Close current path.
       \details
       http://www.w3.org/TR/SVG/paths.html#PathElement
       8.3.1 General information about path data.
       Close the current subpath by drawing a straight line
       from the current point to current subpath's initial point.
    */
    void write(std::ostream& os)
    { //! Write close current path SVG command.
      os << "Z";
    }

    z_path() : path_point(false)
    { //! Constructor defines all member variables.
    }
  }; // struct z_path

  // 8.3.4 The "lineto" commands L, H & V.
  struct l_path: public path_point
  {  /*! \struct boost::svg::l_path
      \brief Draw a line from the current point to the given (x,y) coordinate
       which becomes the new current point.
    */
    double x; //!< End of line SVG X coordinate.
    double y; //!< End of line SVG Y coordinate.

    void write(std::ostream& os)
    { //! Write line to SVG command.
      if(relative)
      {
        os << "l";
      }
      else
      { // Absolute.
        os << "L";
      }
      os << x << "," << y << " " "\n";
    }

    l_path(double x, double y, bool relative = false)
      : x(x), path_point(relative), y(y)
    { //! Constructor defines all member variables.
    }
  }; // struct l_path

  struct h_path: public path_point
  { /*! \struct boost::svg::h_path
      \brief  Draws a horizontal line from the current point (cpx, cpy) to (x, cpy).
       which becomes the new current point. No y needed, start from current point y.
    */
    double x; //!< x horizontal SVG coordinate.
    void write(std::ostream& os)
    { //! Write horizontal line SVG command.
      if(relative)
      {
        os << "h";
      }
      else
      { // Absolute.
        os << "H";
      }
      os << x << " ";
    }

    h_path(double x, bool relative = false)
    :  path_point(relative), x(x)
    { //!< Constructor defines all member variables.
    }
  }; // struct h_path

  struct v_path: public path_point
  { /*! \struct boost::svg::v_path
        \brief Draws a vertical line from the current point (cpx, cpy) to (cpx, y).
        No x coordinate needed - use current point x.
    */
    double y; //!< Y vertical line SVG coordinate.
    void write(std::ostream& os)
    { //! Write vertical line SVG command.
      if(relative)
      {
        os << "v";
      }
      else
      { // Absolute.
        os << "V";
      }
      os << y << " ";
    }

    v_path(double y, bool relative = false)
      :  path_point(relative), y(y)
    { //!< Constructor (defines all member variables).
    }
  }; // struct v_path


  struct c_path: public path_point
  { /*! \struct boost::svg::c_path
     \brief Draws a cubic Bezier curve from the current point to (x, y) using (x1, y1).
     \details 8.3.5 The curve commands: C, Q & A.
    */
    double x1; //!< Middle of curve.
    double y1; //!< Middle of curve.
    double x2; //!< End point.
    double y2; //!< End point.
    double x; //!< Current (start point).
    double y; //!< Current (start point).

    void write(std::ostream& os)
    { //!< Write a cubic Bezier curve SVG XML to ostream.
      if(relative)
      {
        os << "c";
      }
      else
      { // Absolute.
        os<<"C";
      }
      os << x1 << "," << y1 << " " << x2 << "," << y2 << " "
        << x << "," << y << " ";
    } // void write(ostream&)

    c_path(double x1, double y1, double x2, double y2,
            double x, double y, bool relative = false)
      : path_point(relative), x1(x1), y1(y1), x2(x2), y2(y2), x(x), y(y)
    { //!< Constructor defines all member variables.
    }
  }; // struct c_path

  struct q_path: public path_point
  { /*! \struct boost::svg::q_path
      \brief Draws a quadratic Bezier curve from the current point to (x,y).
       using (x1,y1) as the control point.
    */
    double x1; //!< quadratic Bezier curve control X coordinate.
    double y1; //!< quadratic Bezier curve control Y coordinate.
    double x; //!< quadratic Bezier curve end X coordinate.
    double y; //!< quadratic Bezier curve end Y coordinate.

    void write(std::ostream& os)
    { //!< Write a quadratic Bezier curve SVG XML to ostream.
      if(relative)
      {
        os << "q";
      }
      else
      { // Absolute.
        os << "Q";
      }
      os << x1 << " " << y1 << " " << x << " " << y << " ";
    }

    q_path(double x1, double y1, double x, double y, bool relative = false)
      : path_point(relative), x1(x1), y1(y1), x(x), y(y)
    { //! Constructor quadratic Bezier curve.
    }
  }; //struct q_path

  struct s_path : public path_point
  { /*! \struct boost::svg::s_path
      \brief Draws a cubic Bezier curve from the current point to (x,y).
      \details see also t_path for a quadratic Bezier curve.

    */
    double x1; //!< Cubic Bezier curve control X coordinate.
    double y1; //!< Cubic Bezier curve control Y coordinate.
    double x; //!< Cubic Bezier curve end X coordinate.
    double y; //!< Cubic Bezier curve end Y coordinate.
    void write(std::ostream& os)
    { //! Write SVG Cubic Bezier curve command.
      if(relative)
      {
        os << "s";
      }
      else
      { // Absolute.
        os << "S";
      }
      os << x1 << "," << y1 << " "
        << x << "," << y << " ";
    }

    s_path(double x1, double y1, double x, double y, bool relative = false)
      : x1(x1), y1(y1), x(x), y(y), path_point(relative)
    { //! Constructor Cubic Bezier curve.
    }
  }; // struct s_path

  struct t_path: public path_point
  { /*! \struct boost::svg::t_path
      \brief Draws a quadratic Bezier curve from the current point to (x,y).
      \details see also s_path for a cubic Bezier curve.
    */
    double x; //!< SVG X coordinate.
    double y; //!< SVG Y coordinate.

    void write(std::ostream& os)
    { //! Write SVG command for a cubic Bezier curve.
      if(relative)
      {
        os << "t";
      }
      else
      { // Absolute.
        os << "T";
      }
      os << x << "," << y << " ";
    }

    t_path(double x, double y, bool relative = false)
      : x(x), y(y), path_point(relative)
    { //! Constructor of path that draws a quadratic Bezier curve from the current point to (x,y)
    }
  }; // struct t_path

  struct a_path : public path_point
  { /*! \struct boost::svg::a_path
      \brief Draws a elliptical arc from the current point to (x,y),
        using two radii, axis rotation, and two control flags.
      \details See 8.3.8 The elliptical arc curve commands.!
        Useful for pie charts, etc.
     */
    double x; //!< X End of arc from current point.
    double y; //!< Y End of arc from current point.
    double rx; //!< X radius
    double ry; //!< Y radius
    double x_axis_rotation; //!< Any rotation of the X axis.
    bool large_arc; //!< true if arc >= 180 degrees wanted.
    bool sweep; //!< true if to draw in positive-angle direction

    void write(std::ostream& os)
    { //! Write elliptical arc path XML to ostream.
      if(relative)
      {
        os << "a";
      }
      else
      { // Absolute.
        os << "A";
      }
      os << rx << "," << ry << " " << x_axis_rotation
        << ((large_arc) ? 1 : 0) << "," << ((sweep) ? 1 : 0) << " "
        << x << "," << y << " ";
    }

    //! Construct elliptic arc path.
    a_path(double x, double y, double rx, double ry, double x_axis_rotation, bool large_arc = false, bool sweep = false, bool relative = false)
      : x(x), y(y), rx(rx), ry(ry), x_axis_rotation(x_axis_rotation), large_arc(large_arc), sweep(sweep), path_point(relative)
    { // Constructor
    }
  }; // struct a_path

  class path_element: public svg_element
  {  /*! \class boost::svg::path_element
     \brief Path element holds places on a path used by move, line ...
     \details
     \sa http://www.w3.org/TR/SVG/paths.html#PathElement
     8.3.1 General information about path data.
     A path is defined by including a 'path' element
     which contains a d="(path data)"  attribute,
     where the d attribute contains the moveto, line, curve
     (both cubic and quadratic Beziers), arc and closepath instructions.
     */
  public:
    // bool fill; now inherited from parent svg class.

    ptr_vector<path_point> path; //!< All the (x, y) coordinate pairs,
    //!< filled by repeated chained calls of functions m, M, l , L... that are push_backed.

    path_element(const path_element& rhs)
    { //! Copy constructor.
      path = (const_cast<path_element&>(rhs)).path.release();
    }

    path_element(const svg_style& style_info,
      const std::string& id_name="",
      const std::string& class_name="",
      const std::string& clip_name="")
      :
      svg_element(style_info, id_name, class_name, clip_name)
    { //! Construct empty path element.
    }

    path_element()
    { //! Construct an empty path element.
      // fill now got from the parent svg fill color.
    }

    path_element& fill_on(bool on_)
    { //! Set area fill, on or off.
      svg_style_.fill_on(on_);
      return *this; //! \return path_element& to make chainable.
    }

    bool fill_on()
    { //! \return area fill, on or off.
      return svg_style_.fill_on();
    }
    // Note 1: return of path_element& permits chaining calls like
    // my_path.M(3, 3).l(150, 150).l(200, 200)...;

    // Note 2: By convention:
    // lower case (like m call m_path(x, y, true) for relative,
    // but upper case (like M) calls m_path(x, y, false) for absolute.

    path_element& m(double x, double y)
    { //! Move relative by x and y.
      path.push_back(new m_path(x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& M(double x, double y)
    { //! Move to absolute x and y.
      path.push_back(new m_path(x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& z()
    { //! Path end. Note lower case z, see path_element& Z() below.
      path.push_back(new z_path());
      return *this; //! \return path_element& to make chainable.
    }

    path_element& Z()
    { //! Path end. Note Upper case Z also provided for compatibility with
      //! http://www.w3.org/TR/SVG/paths.html#PathDataClosePathCommand 8.3.3 
      //! which allows either case.
      path.push_back(new z_path());
      return *this; //! \return path_element& to make chainable.
    }

    path_element& l(double x, double y)
    { //! Line to (relative).
      path.push_back(new l_path(x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& L(double x, double y)
    { //! Line to (absolute).
      path.push_back(new l_path(x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& h(double x)
    { //! Line horizontal (relative).
      path.push_back(new h_path(x, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& H(double x)
    { //! Line horizontal (absolute).
      path.push_back(new h_path(x, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& v(double y)
    { //! Line vertical (relative).
      path.push_back(new v_path(y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& V(double y)
    {//! Line vertical (absolute).
      path.push_back(new v_path(y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& c(double x1, double y1, double x2, double y2, double x, double y)
    { //! Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1).(relative).
      path.push_back(new c_path(x1, y1, x2, y2, x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& C(double x1, double y1, double x2, double y2, double x, double y)
    { //! Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1).(absolute).
      path.push_back(new c_path(x1, y1, x2, y2, x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& q(double x1, double y1, double x, double y)
    {  //! Quadratic Curve Bezier (relative).
      path.push_back(new q_path(x1, y1, x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& Q(double x1, double y1, double x, double y)
    { //! Quadratic Curve Bezier (absolute).
      path.push_back(new q_path(x1, y1, x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& s(double x1, double y1, double x, double y)
    { //! Draws a cubic Bezier curve from the current point to (x,y) (relative).
      path.push_back(new s_path(x1, y1, x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& S(double x1, double y1, double x, double y)
    { //! Draws a cubic Bezier curve from the current point to (x,y) (absolute).
      path.push_back(new s_path(x1, y1, x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& t(double x, double y)
    { //! Draws a quadratic Bezier curve from the current point to (x,y)(relative).
      path.push_back(new t_path(x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& T(double x, double y)
    { //! Draws a quadratic Bezier curve from the current point to (x,y)(absolute).
      path.push_back(new t_path(x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    void write(std::ostream& os)
    { //! Write a SVG path command to an @c std::ostream.
      //! Example: \verbatim <path d="M5,175 L5,195 M148.571,175" /> \endverbatim
      if (path.begin() != path.end() )
      { // Is some path info (trying to avoid useless <path d=""/>"
        // TODO or would this omit useful style & attributes?
        os << "\t" "<path d=\"";
        for(ptr_vector<path_point>::iterator i = path.begin(); i != path.end(); ++i)
        {
          (*i).write(os); // M1,2
        }
        os << "\"";

        write_attributes(os); // id & clip_path.
        svg_style_.write(os); // fill, stroke, width...

        // line above should write fill = "none" that
        // seems to be needed for reasons unclear.
        // Even when g_element does not specify a fill, it seems to be interpreted as black fill.
        if(!fill_on())
        {
          os << " fill=\"none\"";
        }
        os<<"/>" "\n"; // closing to match <path d=
      }
    } // void write(std::ostream& os)
  }; // class path_element

  struct poly_path_point
  { /*! \struct boost::svg::poly_path_point
      \brief polyline or polygon point coordinates (x, y)
      \details  9.6 polyline & 9.7 The 'polygon' element.
      */
    double x; //!< polygon or polyline path point X SVG coordinate.
    double y; //!< polygon or polyline path point Y SVG coordinate.
    // Polygon & polyline points are @b always absolute, never relative,
    // and values have no preceeding letter like M or L,
    // So @c poly_path_point is NOT derived from @c path_point.

    void write(std::ostream& os)
    { //! Output poly_path_point as SVG XML,
      //! Example: " 250,180"
      //! Leading space is redundant for 1st after "points= ",
      //! but others are separators, and arkward to know which is 1st.
      os << " " << x << "," << y; // x, y separator comma for clarity.
    } // void write(std::ostream& os)

    poly_path_point(double x, double y)
      : x(x), y(y)
    { //! Construct a polygon or polyline path point from X and Y SVG coordinate.
    }

    poly_path_point() : x(0.), y(0.)
    { //! Default constructor places point at SVG origin (top left).
    }

  }; // struct poly_path_point

  std::ostream& operator<< (std::ostream& os, const poly_path_point& p)
  { //! Output may be useful for diagnosis and Boost.Test.
    //! Usage:  poly_path_point p0(100, 200);
    //! cout << p0 << endl;
    //! Outputs: (100, 200)
     os << "(" << p.x << ", " << p.y  << ")";
    return os;
  } // std::ostream& operator<<

  class polygon_element: public svg_element
  {  /*! \struct boost::svg::polygon_element
     \brief The 'polygon' element defines a closed shape
     consisting of a set of connected straight line segments.

     \details http://www.w3.org/TR/SVG/shapes.html#PolygonElement
     The 'polygon'  element 9.9.7.
     A polygon is defined by including a 'path'  element
     which contains a points="(path data)"  attribute,
     where the d attribute contains the x, y coordinate pairs.
    */
    friend std::ostream& operator<< (std::ostream&, const polygon_element&);
    friend std::ostream& operator<< (std::ostream&, polygon_element&);

  public: //temporary for experimental gil
//  private:
    //using boost::ptr_vector;
    ptr_vector<poly_path_point> poly_points; //!< All the x, y coordinate pairs,
    //!< push_backed by calls of p_path(x, y).
    bool fill; //!< polygon to have fill color.

    polygon_element(const polygon_element& rhs)
    { //! Copy constructor.
      poly_points = (const_cast<polygon_element&>(rhs)).poly_points.release();
      // 'empty' the vector of points.
    }

    polygon_element() : fill(true)
    { //! Default constructor empty polygon (with fill on).
    }

    polygon_element (double x, double y, bool f = true) : fill(f)
    { //! Constructor - One absolute (x, y) point only.
      //! Can add more path points using member function P.
      poly_points.push_back(new poly_path_point(x, y));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, bool f = true)
      :
      fill(f)
    { //! Constructor - Absolute (x, y) only. Used by triangle.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, bool f = true)
      :
      fill(f)
    { //! Constructor - Absolute (x, y) only. Used by rhombus.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
      poly_points.push_back(new poly_path_point(x4, y4));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, bool f = true)
      :
      fill(f)
    { //! Constructor - Absolute (x, y) only. Used by pentagon.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
      poly_points.push_back(new poly_path_point(x4, y4));
      poly_points.push_back(new poly_path_point(x5, y5));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, bool f = true)
      :
      fill(f)
    { //! Constructor - Six absolute (x, y) only. Used by hexagon.
      // Might be done more efficiently with fixed size boost::array?
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
      poly_points.push_back(new poly_path_point(x4, y4));
      poly_points.push_back(new poly_path_point(x5, y5));
      poly_points.push_back(new poly_path_point(x6, y6));
    }

    polygon_element (std::vector<poly_path_point>& points, bool f = true)
      :
      fill(f)
    { //! Constructor from vector of path points.
      poly_points.reserve(points.size()); // Since we know how many will be pushed.
      for(std::vector<poly_path_point>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }
/*
//! Add a polygon from a fixed length @c boost::array
//! \tparam n Number of points in the poly_path
    template<int n>
    polygon_element (boost::array<const poly_path_point, n>& points, bool f = true)
      :
      fill(f)
    { // Constructor from const array of path points.
      poly_points.reserve(n); // Since we know how many will be pushed.
      //poly_points.reserve(points.size()); // Since we know how many will be pushed.
      for(array<const poly_path_point, n>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }

    // And non-const poly_path_point version, in case needed.
    template<int n>
    polygon_element (boost::array<poly_path_point, n>& points, bool f = true)
      :
      fill(f)
    { // Constructor from const array of path points.
      poly_points.reserve(n); // Since we know how many will be pushed.
      // == poly_points.reserve(points.size()); // Since we know how many will be pushed.
      for(array<poly_path_point, n>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }
*/
    // Member function to add more points to polygon.
    polygon_element& P(double x, double y)
    { //! Add another point (x, y) - absolute only.
      poly_points.push_back(new poly_path_point(x, y));
      return *this; //! \return polygon_element& to make chainable.
    }

    void write(std::ostream& os)
    {  /*! \verbatim SVG XML:
            Example: <polygon fill="lime" stroke="blue" stroke-width="10"
            points="850,75  958,137.5 958,262.5 850,325 742,262.6 742,137.5" />
         \endverbatim
       */
      os << "\t" "<polygon points=\"";
      for(ptr_vector<poly_path_point>::iterator i = poly_points.begin(); i != poly_points.end(); ++i)
      {
        (*i).write(os); //  x, y coordinates as " 1,2"
      }
      os << "\"";
      write_attributes(os);
      svg_style_.write(os);
      if(!fill)
      {
        os << " fill = \"none\"";
      }
      os<<"/>" "\n";
    } // void write(std::ostream& os)

    std::ostream& operator<< (std::ostream& os)
    { /*! Output polygon info. (May be useful for diagnosis and Boost.Test).
         using os << "(" << p.x << ", " << p.y  << ")" ;
         Usage:  polygon_element p(1, 2, 3, 4, 5, 6);
           my_polygon.operator << (cout);
         (But NOT cout << my_polygon << endl;)
         Outputs: (1, 2)(3, 4)(5, 6)
      */
      for(ptr_vector<poly_path_point>::iterator i = poly_points.begin(); i != poly_points.end(); ++i)
      {
        os << (*i); //  x, y coordinates as " (1, 2)"
      }
      return os;
    } // std::ostream& operator<<

  }; // class polygon_element

  std::ostream& operator<< (std::ostream& os, polygon_element& p)
  { /*! Output all poly_path_ points (May be useful for Boost.Test and diagnosis).
        ptr_vector<poly_path_point> poly_points; All the x, y coordinate pairs,
        Example: \code polygon_element p(1, 2, 3, 4, 5, 6);
        std::cout << p << std::endl;
        /endcode
        Outputs: (1, 2)(3, 4)(5, 6)
    */
    for(ptr_vector<poly_path_point>::iterator i = p.poly_points.begin(); i != p.poly_points.end(); ++i)
    {
      os << (*i); //  x, y coordinates as " (1, 2)(3, 4)..."
      // Uses \code  os << "(" << p.x << ", " << p.y  << ")" ; \endcode
    }
    return os;
  } // std::ostream& operator<<

  class polyline_element: public svg_element
  { /*! \class boost::svg::polyline_element
     \brief The 'polyline'  element: defines a set of connected straight line segments.
     \details
      http://www.w3.org/TR/SVG/shapes.html#PolylineElement
     9.6 The polyline  element: defines a set of connected straight line segments.
     Typically, polyline elements define open shapes.
     A polyline is defined by including a 'path'  element
     which contains a points="(path data)"  attribute,
     where the points attribute contains the x, y coordinate pairs.
     * perform an absolute moveto operation
       to the first coordinate pair in the list of points
     * for each subsequent coordinate pair,
       perform an absolute lineto operation to that coordinate pair.
     The advantage of polyline is in reducing file size,
     avoiding M and repeated L before x & y coordinate pairs.
     */
  friend std::ostream& operator<< (std::ostream&, polyline_element&);

   public: 
// private: // has little advantage?
    ptr_vector<poly_path_point> poly_points; //!< All the (x, y) coordinate pairs,
    // push_back by calls of p_path(x, y).
    //bool fill; // not needed for polyline, unlike polygon.

    polyline_element(const polyline_element& rhs)
    { //! Copy constructor.
      poly_points = (const_cast<polyline_element&>(rhs)).poly_points.release();
    }

    polyline_element()
    { //! Construct an 'empty' line.
      //! Can new line path points add using polyline_element member function P(x, y).
    }

    polyline_element (double x1, double y1)
    { //! Constructor from one (x, y) path point, absolute only.
      poly_points.push_back(new poly_path_point(x1, y1));
    }

    polyline_element (double x1, double y1, double x2, double y2)
    { //! Constructor from  Two (x, y) path points, absolute only.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
    }

    polyline_element (std::vector<poly_path_point>& points)
    { //! Constructor from vector of path points.
      for(std::vector<poly_path_point>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }

    // Member function to add new points to existing line.
    polyline_element& P(double x, double y)
    { //! Absolute (x, y) only, so Capital letter P.
      poly_points.push_back(new poly_path_point(x, y));
      return *this; //! \return polyline_element& to make chainable.
    }

    void write(std::ostream& os)
    { /*! \verbatim
          Output polyline info (useful for Boost.Test).
          Example: <polyline points=" 100,100 200,100 300,200 400,400"/>
          \endverbatim
      */
      os << "\t" "<polyline points=\"";
      for(ptr_vector<poly_path_point>::iterator i = poly_points.begin(); i!= poly_points.end(); ++i)
      {
        (*i).write(os); //  x, y coordinates as " 1,2"
      }
      os << "\"";
      write_attributes(os);
      svg_style_.write(os);
      os<<"/>" "\n";
    } // void write(std::ostream& os)

  }; // class polyline_element

  std::ostream& operator<< (std::ostream& os, polyline_element& p)
  { /*! 
     Output polyline info (useful for Boost.Test and diagnosis).
      \verbatim
         Example: <polyline points=" 100,100  200,100  300,200  400,400"/>
         ptr_vector<poly_path_point> poly_points; // All the x, y coordinate pairs.
      \endverbatim
    */
    for(ptr_vector<poly_path_point>::iterator i = p.poly_points.begin(); i != p.poly_points.end(); ++i)
    {
      os << (*i); //  x, y coordinates as " (1, 2)(3, 4)..."
      // using os << "(" << p.x << ", " << p.y  << ")" ;
    }
    // Usage:  polyline_element p(1, 2, 3, 4, 5, 6);
    // std::cout << p << std::endl;
    // Outputs: (1, 2)(3, 4)(5, 6)
    return os;
  } // std::ostream& operator<<

  class g_element: public svg_element
  { /*! \class boost::svg::g_element
      \brief g_element (group element) is the node element of our document tree.

     g_element is a container element for grouping together related graphics elements).\n
     See https://www.w3.org/TR/SVG/struct.html#Groups 5.2.1 Overview.
     Container element is an element which can have graphics elements and other container elements as child elements. 
     Container elements are: a, clipPath, defs, @b g, marker, mask, pattern, svg, switch, symbol, and unknown.

     A group of elements, as well as individual objects, can be given a name using the @b id attribute.
     All g_elements should include an id as a string like id="background", or a macro defined in 
       ../include/boost/svg_plot/detail/svg_style_detail.hpp
      to index @c std::string @c document_ids_ 

      For example, an image background rectangle with border and fill:
      \verbatim 
         <g id="background" fill="rgb(255,255,255)">  
           <rect width="500" height="350"/>
         </g>
      \endverbatim
   */
  public: // Simplest, private has little benefit.

    ptr_vector<svg_element> children; /*!< Children of this group element node,
      containg graphics elements like text, rect, circle, line, polyline... */

    std::string clip_name;  //!< Name of clip path.
    bool clip_on; //!< @c true if to clip anything outside the clip path, often the plot window
   //!< so that data-point markers do not overlap axis tick values on the outside of the window.

    g_element() : clip_on(false)
    { //! Construct g_element (with no clipping).
    }

    svg_element& operator[](unsigned int i)
    { //! \return Child @c svg_element ith node.
      return children[i];
    }

    size_t size()
    { //! \return Number of child (leaf) nodes.
      return children.size();
    }

    void write(std::ostream& os)
    { /*! Output all children (leafs) of a group element.
        \verbatim
          Example:
           <g fill="rgb(255,255,255)" id="background"><rect x="0" y="0" width="500" height="350"/></g>
        \endverbatim
      */

      if (children.size() > 0)
      { /*! Do not output anything if no child leafs to avoid useless output like: 
          \verbatim
           <g id="legendBackground"> </g>
          \endverbatim
        */
        os << "\n" "<g"; // Do NOT need space if convention is to start following item with space or tab or newline.
        write_attributes(os); // id="background" (or clip_path)
        svg_style_.write(os); // SVG style info like stroke="rgb(0,0,0)" fill= "rgb(255,0,0)" ...
        os << ">" 
          "\n"; // Newline after the g_element id and style is easier to read.
        for(unsigned int i = 0; i < children.size(); ++i)
        {
          children[i].write(os);  // Using each element's version of write function.
          // Using tab to indent also makes easy to read.
        }
        os << "</g>" "\n"  ; // 
      }
    } // void write(std::ostream& rhs)

    g_element& gs(int i)
    { //! i is index of children nodes (first is zero).
      //! Example:   \code g_element& g1 = my_svg.gs(1); // index is one. \endcode
      return *(static_cast<g_element*>(&children[i])); // \return reference to this child node @c g_element.
    }

    g_element& add_g_element()
    { //! Add a new group element.
      //! \return Reference to the new child node just created.
      //! Example: \code  g_element& g0 = my_svg.add_g_element(); // Add first (zeroth) new element to the document. \endcode
      children.push_back(new g_element());
      return *(static_cast<g_element*>(&children[children.size()-1]));
    }

    line_element& line(double x1, double y1, double x2, double y2)
    { //! Add a new line element.
      //! \return A reference to the new child node just created.
      children.push_back(new line_element(x1, y1, x2, y2));
      return *(static_cast<line_element*>(&children[children.size()-1]));
    }

    rect_element& rect(double x1, double y1, double x2, double y2)
    { //! Add a new rect element.
      //! \return A reference to the new child node just created.
      children.push_back(new rect_element(x1, y1, x2, y2));
      return *(static_cast<rect_element*>(&children[children.size()-1]));
    }

    circle_element& circle(double x, double y, double radius = 5.)
    { //! Add a new circle element.
      //! \return A reference to the new child node just created.
      children.push_back(new circle_element(x, y, radius));
      return *(static_cast<circle_element*>(&children[children.size()-1]));
    }

    ellipse_element& ellipse(double rx, double ry, double cx, double cy)
    { //! Add a new ellipse element.
      //! \return A reference to the new child node just created.
      children.push_back(new ellipse_element(rx, ry, cx, cy));
      return *(static_cast<ellipse_element*>(&children[children.size()-1]));
    }

    // svg::text constructor with defaults. 
    text_element& text(double x = 0., double y = 0., // Location.
    const std::string& text = "", // Text string to display.
    const text_style& textstyle = no_text_style, // Default to use SVG implementation's defaults for font family, size.
    const align_style& align = align_style::left_align,
    const rotate_style& rotate = horizontal)
    { //! Add a new text element.
      //! \return A reference to the new child (leaf) node just created.
      children.push_back(new text_element(x, y, text, textstyle, align, rotate));
      return *(static_cast<text_element*>(&children[children.size()-1]));
    }

    // Polygon for shapes with many vertices.
    polygon_element& polygon(double x, double y, bool f = true)
    { //! Add a new polygon element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element(x, y, f));
      return *(static_cast<polygon_element*>(&children[children.size()-1]));
    }

    polygon_element& polygon(std::vector<poly_path_point>& v, bool f = true)
    { //! Add a new complete polygon element.
      //! \return A reference to the new child node just created.
      // push_back a complete many-sided polygon to the document.
      children.push_back(new polygon_element(v, f));
      return *(static_cast<polygon_element*>(&children[children.size()-1]));
    }

    polyline_element& polyline(std::vector<poly_path_point>& v)
    {  //! Add a new complete polyline.
       //! \return A reference to the new child node just created.
      children.push_back(new polyline_element(v));
      return *(static_cast<polyline_element*>(&children[children.size()-1]));
    }

    polyline_element& polyline(double x, double y)
    { //! Add a new polyline element, but 1st point only, add others later with .P(x, y)...
      //! \return A reference to the new child node just created.
      children.push_back(new polyline_element(x, y));
      return *(static_cast<polyline_element*>(&children[children.size()-1]));
    }

    polygon_element& triangle(double x1, double y1, double x2, double y2, double x3, double y3, bool f = true)
    { //! Add a new triangle element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, f));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& rhombus(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, bool f = true)
    { //! Add a new rhombus element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, f = true));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& pentagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, bool f = true)
    { //! Add a new pentagon element.
      //! \return A reference to the new child node just created.
      // push_back a complete pentagon to the document.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, f));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& hexagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, bool f = true)
    { //! Add a new hexagon element.
      //! \return A reference to the new child node just created.
      // push_back a complete 6-sided star to the document.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, f));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& polygon()
    { //! Add a new polygon element.
      //! \return A reference to the new polygon element just created.
      children.push_back(new polygon_element()); // Empty polygon,
      // to which poly_path_points can be added later using member function P.
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polyline_element& polyline()
    { //! Add a new polyline element.
      //! \return A reference to the new polyline element just created.
      children.push_back(new polyline_element()); // Empty polyline.
      return *(static_cast<polyline_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    path_element& path()
    { //! Add a new path element.
      //! \return A reference to the new path just created.
      children.push_back(new path_element()); // Empty path.
      return *(static_cast<path_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    void push_back(svg_element* g)
    { //! Add a new child node g_element.
      children.push_back(g);
    }

    void clear()
    { //! Remove all the child nodes.
      children.clear();
    }
  }; // class g_element

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_TAG_HPP
