/*!
  \file svg.hpp
  \brief Scalable Vector Graphic (SVG) format elements.
  \details Provides classes and methods to create the basic SVG graph elements.
  Graph elements, point, path, line, circle, rect and polygon, text are used by the 1D, 2D and Boxplot functions,
  but could also be used for generating other graphics in SVG format.

  \author Jacob Voytko & Paul A. Bristow
 */

/*!
  \mainpage Scalable Vector Graphic (SVG) Plot Package

  \section intro_sec Introduction

  Humans have a fantastic capacity for visual understanding, and merely looking
  at data organized in one, two, or three dimensions allows us to see relations
  not otherwise visible in a list of numbers.

  Computers, however, deal with information numerically, and C++ and the
  \htmlonly
    <a href="http://en.wikipedia.org/wiki/C%2B%2B#Standard_library"> Standard Template Library (STL) </a>
  \endhtmlonly
  do not currently offer a way to bridge the gap.

  This library helps the user to easily plot data stored in STL containers.

  This project is focused on using STL containers in order to graph data on
  one-dimensional (1D) and two-dimensional (2D) plots.

  The plot will currently be written to a
  \htmlonly
    <a href="http://en.wikipedia.org/wiki/Scalable_Vector_Graphics"> Scalable Vector Graphic image </a>
  \endhtmlonly
  \htmlonly
    <a href="http://www.w3.org/TR/SVG11/"> Scalable Vector Graphics (SVG) </a>
  \endhtmlonly
  \htmlonly
    <a href="http://www.w3.org/TR/REC-xml/#sec-comments"> XML specification</a>
  \endhtmlonly

  and file format for describing two-dimensional vector graphics.

  SVG files (.svg) can be viewed with any modern internet browser:

  - Mozilla Firefox (newer versions),

  - \htmlonly
      <a href="http://www.adobe.com/products/acrobat/readstep2.htmlAdobe"> Adobe Acrobat Reader</a>
    \endhtmlonly

  - \htmlonly
      <a href="http://www.adobe.com/products/illustrator"> Adobe Illustrator </a>
    \endhtmlonly

  - \htmlonly
      <a href="www.opera.com"> Opera</a>
    \endhtmlonly
    has
    \htmlonly
      <a href="http://www.opera.com/docs/specs/svg/"> SVG support</a>
    \endhtmlonly.

  - Microsoft Internet Explorer 9 and later, (or earlier versions \b provided a suitable
    \htmlonly
      <a href="http://www.adobe.com/svg/"> Adobe SVG Viewer plug-in for SVG files</a>
    \endhtmlonly
    is installed).  Sadly, rendering quality is so poor that IE9 cannot be recommended.
    IE11 offers good rendering compliance with the SVG standard.

  - \htmlonly
      <a href="http://www.inkscape.org"> Inkscape</a>
    \endhtmlonly
    , a fine Open Source SVG editor with excellent rendering, full scaling and other editing features,
    including conversion to wide variety of other graphics formats
    like Portable Network Graphics .png.

    Inkscape is the 'gold standard' for viewing SVG files.

    To do this, you will need to download and install the Inkscape package from http://www.inkscape.org.
    usually to \verbatim C:\Program Files\Inkscape\  \endverbatim
    You will need to use the Explorer, Tools, Folder Options,
    and add extension .svg if necessary, and then associate svg files with \c inkscape.exe.

    You may also find it convenient to add \verbatim "c:\Program Files\inkscape" \endverbatim to your Path using
    Control Panel, System, Advanced system settings, Environment variables, Path, and edit
    to append the folder (needs restart of comand window).
    You can then use commands like "inkscape my_picture.svg" from any command window.

    Inkscape allows one to open many svg files as a slideshow
    and move forward and back with the cursor keys.

    This works as expected using a command window,
    for example after changing to the directory containing your SVG files,
    and running "inkview *.svg" to display all the images in the directory as a slideshow.

    Sadly, by default, if you select many files (but not more than 15!) in Windows Explorer,
    multiple windows (instances of inkview.exe) are opened instead of a slideshow.
    If you know how to change this 'by design' feature, please tell me.

    You may instead find it useful to create a batch file, for example called view_my_plots.bat containing
    \verbatim START "Inkscape" /MIN inkscape.exe  C:\Users\Paul\Desktop\My_Plots\*.svg \endverbatim
    that will display all the svg images in folder My_Plots in turn as the cursor keys are pressed.
    Of course, you can also omit the folder specification, and just have `*.svg` when the location of the
    batch file will determine which svg files are being viewed.

  - Google www.google.com/chrome Chrome,

  - And by Many other graphics programs, for example
      \htmlonly
        <a href="http://svg.software.informer.com/software/"> Most popular SVG software</a>
      \endhtmlonly

  The goals of the project are:

  - To let users produce simple plots with minimal intervention by using sane defaults.
  - To allow users to easily customize plots but allow very fine-grained control of appearance.
  - To allow the user to talk to the plot classes using coordinate units rather than pixels or other arbitrary measures.
  - To produce and transfer plots quickly enough to be useful in real-time.
  - To represent uncertainty visually and numerically by showing only significant digits,
  and optionally adding uncertainty and degrees of freedom estimates.
  - To create the backbone of a `svg` class that can be extended to fully support the standard.
  - Compliance with the  \htmlonly <a href="http://www.w3.org/TR/SVG11/"> W3C Scalable vector Graph standard.</a> \endhtmlonly
  - Validation using W3C Markup Validation Service.
  - Copyright and license conditions built into the SVG file.

  \section why_svg Why SVG format?

  SVG provides very high quality images that display well on small screens like handheld devices,
  conventional monitors of all sizes,
  and on big screens (for example, projected on lecture theatre screens),
  and especially when printed, but files are tiny
  (compared to images produced by spreadsheets like Microsoft Excel).

  SVG files are microscopic when compressed using a zip program
  to convert to types like .zip, or the specific compressed SVG file type (.svgz).

  SVG files can be added to other documents like Adobe PDF, and display well, without bloat.

  \section plot_types_sec 1D, 2D and Boxplot

  Many types of plots can be produced:

  - 1D plots are to show the spread of repeated observations of a single variable,
  for example, the measured length of an object
  (where the object's length is assumed to stay exactly the same but the measurements have some uncertainty)
  or observations of a single attribute, for example,
  the heights of boys in a school
  (where the measurement is assumed exact but the heights really do vary).

  - 2D plots allow X versus Y variables to be displayed.  You can show values,
  and optionally their uncertainty (the deprecated term is 'error bars').

  - Boxplots can be produced, optionally with values and outliers.

  - Bar charts can also be produced.

  This page generated from file svg.hpp.

*/
// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2007, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_HPP
#define BOOST_SVG_SVG_HPP

#include <string>
#include <ostream>
#include <fstream>
#include <exception>
#include <vector>

//#include "stylesheet.hpp" // TODO better to be called svg_stylesheet.hpp?
#include "detail/svg_tag.hpp" // element class definitions.
#include "svg_style.hpp"
//#include "svg_fwd.hpp" // Could be used to check declarations and definitions match correctly.

// SVG stands for Scalable Vector Graphics,
// an XML grammar for stylable graphics, usable as an XML namespace.

// http://www.adobe.com/svg/basics/intro.html SVG Workflow on optimising SVG
// Gzip compression - can give files that are 1/10th size of gif or jpeg.
// Use default values whenever possible rather than defining all attributes and properties explicitly.
// Take advantage of the path data compaction facilities so use relative coordinates.
// Use h and v for horizontal and vertical lines.
// Use s or t for cubic and quadratic Bezier segments whenever possible.
// Eliminate extraneous white space and separators.
// Use symbols if the same graphic appears multiple times in the document.
// Use CSS property inheritance and selectors to consolidate commonly used properties into named styles
// or to assign the properties to a parent group element.
// Use filter effects to help construct graphics via client-side graphics operations.

namespace boost
{
namespace svg
{ //! \namespace boost \brief www.Boost.org.
  //! \namespace boost::math \brief Boost.Math library at www.boost.org.
  //! \namespace boost::math::detail \brief Boost.Math library at www.boost.org implementation details.
  //! \namespace svg \brief Scalable Vector Graph plot functions, classes and data.
  //! \namespace boost::svg \brief SVG classes and functions in Boost.Plot.
  //! \namespace boost::svg::boxplot \brief SVG classes and functions specific to boxplots.
  //! \namespace boost::svg::detail \brief SVG classes and functions implementation details (not recommended for normal use).

  /*! Inserted as a SVG comment, for example\n
   "<!-- Demo of 1D plot features. -->"\n
   and also as a\n \<desc\>http://www.w3.org/TR/SVG/struct.html#DescriptionAndTitleElements\n
   Section 5.4 The 'desc' and 'title' elements.\n\n

   Default XML comment is:\n\n
\verbatim
      <!-- SVG plot written using Boost.Plot program (Creator Jacob Voytko) -->
\endverbatim
\n
\verbatim

      <!-- Use, modification and distribution of Boost.Plot subject to the -->
\endverbatim
\n
\verbatim

      <!-- Boost Software License, Version 1.0.-->
\endverbatim
\n
\verbatim

      <!-- (See accompanying file LICENSE_1_0.txt -->
\endverbatim
\n
\verbatim

      <!-- or copy at http://www.boost.org/LICENSE_1_0.txt) -->

\endverbatim

*/

// Note problem here caused by -- being read as an en dash!


    static const std::string package_info = //!< Default SVG package information about this program that produced the SVG image (not the image itself).
      "<!-- SVG plot written using Boost.Plot program (Creator Jacob Voytko) --> \n"
      "<!-- Use, modification and distribution of Boost.Plot subject to the --> \n"
      "<!-- Boost Software License, Version 1.0.--> \n"
      "<!-- (See accompanying file LICENSE_1_0.txt --> \n"
      "<!-- or copy at http://www.boost.org/LICENSE_1_0.txt) --> \n";

  /*
     Copyright notice to be inserted into plot image produced by this program.
     Note: can have more than one copyright date, or a range.
     Produces a copyright notice as an SVG comment like this:
     "<!-- Copyright Paul A. Bristow, 2007  --> \n"
     and as a meta item:
     \verbatim
       <meta name="copyright" content="Paul A. Bristow" />
     \endverbatim
   */

    /*

    Another popular option is Creative Commons Attribution-ShareAlike 4.0 International License
          https://creativecommons.org/licenses/by-sa/4.0/

    <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
    
    */

//! \cond DETAIL
// Forward declaration of Class to output Scalable Vector Graph XML graph elements: point, path, line, circle, rect, polygon and text.
class svg;
// See svg_fwd.hpp for forward declarations.
// Public member functions:
//   svg() constructor, and copy constructor.
// Chainable set and get functions for image size, description, title, copyright and date.
// image size x and y, and both.
// svg& x_size(unsigned int x)
// unsigned int x_size()
// svg& y_size(unsigned int y)
// unsigned int y_size()
// svg& size(unsigned int x, unsigned int y)
// information about the image file.
// svg& description(std::string d)
// std::string& description()
// svg& document_title(std::string d)
// std::string document_title()
// svg& copyright_holder(std::string d)
// svg& copyright_date(std::string d)
// const std::string copyright_date()

// Write information about svg shapes:
// svg& line(double x1, double y1, double x2, double y2)
// svg& rect(double x1, double y1, double x2, double y2)
// svg& circle(double x, double y, unsigned int radius = 5)
// svg& ellipse(double rx, double ry, double cx, double cy)

// write image out to ostream and file:
// svg& write(std::ostream& s_out)
// svg& write(const std::string& file)

// Possibles for license strings:
// static const std::string permit("permits");
// static const std::string require("requires");
// static const std::string prohibit("prohibits");
//! \endcond

class svg
{ /*! \class boost::svg::svg
  \brief Class to output Scalable Vector Graph XML graph elements: point, path, line, circle, rect, polygon and text.
  \details  Class to add basic Scalable Vector Graph XML graph elements:
  point, path, line, circle, rect, polygon and text to SVG images,
  including metadata like author, copyright and license.
  Finally output the final image as SVG XML to a @c std::stream or file of type .svg (by default).
*/
protected:
  unsigned int x_size_; //!< SVG image X-axis size (in SVG units (default pixels).
  unsigned int y_size_; //!< SVG image Y-axis size (in SVG units (default pixels).

  g_element document; //!< To hold all group elements of the svg document.
  std::vector<clip_path_element> clip_paths; //!< Points on clip path (used for plot window).
  // Document metadata:
  std::string title_document_; //!< SVG document title (appears in the SVG file header as \verbatim <title> ... </title> \endverbatim).
  std::string image_desc_; //!< SVG image description (appears in the SVG file header as \verbatim <desc> ... </desc> \endverbatim).
  std::string holder_copyright_; //!< SVG info on holder of copyright (probably == author, but could be an institution).
  std::string date_copyright_; //!< SVG info on date of copyright claimed.
  std::string css_; //!< Cascading Style Sheet.
  std::string filename_; //!< file SVG XML written to.
  std::string author_; //!< Author(s) name. (Probably == copyright holder).
  bool is_boost_license_; //!< If true, to include the Boost license as a XML comment.
  bool is_license_; //!< If true, to include Creative Commons license as metadata:
  std::string reproduction_; //!< License requirements for reproduction: "permits", "requires", or "prohibits".
  std::string attribution_; //!< License requirements for attribution: "permits", "requires", or "prohibits".
  std::string commercialuse_; //!< License requirements for commerical use: "permits", "requires", or "prohibits".
  std::string distribution_; //!< License requirements for distribution: "permits", "requires", or "prohibits".
  std::string derivative_works_; //!< License requirements for derivative: "permits", "requires", or "prohibits".

  int coord_precision_; //!< Number of decimal digits precision for output of X and Y coordinates to SVG XML.
  // Not sure this is the best place for this?

private:
//! \cond DETAIL // Dxygen document this section only if DETAIL defined.
  void write_header(std::ostream& s_out)
  { //! Output the DTD SVG 1.1 header into the SVG g_element document.
    s_out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
      //<< "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
      //<< "\"http://www.w3.org/graphics/svg/1.1/dtd/svg11.dtd\">"
      // Doctype removed to avoid the need to reference this file (repeatedly!)
      // using RenderX to produce pdf with embedded svg.
      // Might also provide a local copy of the dtd to avoid need to refer to it.
      << std::endl;
    // TODO IE6 with Adobe does not recognise this DOCTYPE, but displays OK.
    // http://jwatt.org/svg/authoring/#namespace-binding recommends NO DOCTYPE!
    // But does recommend other info - see write()
    // Inkscape does NOT provide a doctype.
    // s_out << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
    //         "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20000303 Stylable//EN\"
    //         \"http://www.w3.org/TR/2000/03/WD-SVG-20000303/DTD/svg-20000303-stylable.dtd\">\n"
    // This IS out of date? version 1.0 TR 2000, but the Adobe SVG viewer complains
    // if it is missing - but ignored.
    // But Firefox says document does not contain style.
    // http://www.adobe.com/svg/basics/getstarted.html

    // Inkscape uses encoding=\"UTF-8\" Unicode
    // (encoding=\"iso-8859-1\" == Latin now obselete)

    // example of style command:
    // style="font-size:20px;font-weight:bold;text-decoration: line-through;fill:aqua"
    // font-family:Arial Narrow

    // Inkscape uses: xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg"
  }

  void write_css(std::ostream& s_out)
  { //! Output CSS (Cascading Style Sheet) - (not yet used or implemented).
    if (css_.size() != 0) // css != ""
    { // TODO confirm that this isn't useful if css is "".
      // [CDATA[ ... ]] enclosing the style information
      // is a standard XML construct for hiding information
      // necessary since CSS style sheets can include characters,
      // such as ">", which conflict with XML parsers.
      s_out << "<defs><style type=\"text/css\"><![CDATA[" << css_
        << "]]></style></defs>" << std::endl;
      // CSS inline style can be declared within a style attribute in SVG
      // by specifying a semicolon-separated list of property declarations,
      // where each property declaration has the form "name: value".
      // For example a style: style="fill:red; stroke:blue; stroke-width:3"
      // class=
      // Multiple class names must be separated by whitespace.

      // Example: <defs><style type="text/css"><![CDATA[]]>
      // .axes { fill:none;stroke:#333333;stroke-width:1.6 }
      // .title{ font-size:20px;font-weight:bold;font-style:italic;fill:magenta }
      // .legend_title{ font-size:16px;font-weight:bold;fill:darkblue;text-anchor:middle }
      // .legend_item{ font-size:16px;font-weight:normal;fill:blue }
      // .x_axis_value{ font-size:12px;font-weight:normal;fill:black }
      //   </style></defs>
    }
  } // void write_css(std::ostream& s_out)

  void write_document(std::ostream& s_out)
  { //! \brief Output all of the image to the svg document (Internal function)
    /*! \details Output all clip paths that define a region of the output device
      to which paint can be applied.
     */
    for(size_t i = 0; i < clip_paths.size(); ++i)
    {
      clip_paths[(unsigned int)i].write(s_out);
    }
    // Write all visual group elements.
    for(size_t i = 0; i < document.size(); ++i)
    { // plot_background, grids, axes ... title
      document[(unsigned int)i].write(s_out);
    }
  } // write_document
//! \endcond // DETAIL
public:
  svg() //! Define clas svg default constructor.
    :
    x_size_(400), //!< X-axis of the whole SVG image (default SVG units, default pixels).
    y_size_(400), //!< Y-axis of the whole SVG image (default SVG units, default pixels).
    title_document_(""),  //!< This is a SVG document title, not a plot title.
    image_desc_(""), //!< Information about the SVG image, for example, the program that created it.
    holder_copyright_(""),  //!< Name of copyright holder.
    date_copyright_(""), //!<  Date of copyright claim.
    author_(""), //!< Author of image (defaults to the copyright holder).
    css_(""), //!< Stylesheet filename, if any.
    filename_(""), //!< Name of file to which SVG XML has been written is embedded in the file as an XML comment (if written only to an ostream, filename will not appear in comment).
    is_boost_license_(false), //!< If true, Boost license text is written as comment in SVG XML. (Default is no license). Suggested strings for license permission are "permits", "requires", or "prohibits", or "".
    is_license_(false), //!< If true, license text is written as comment in SVG XML. (Default is no license).
    reproduction_("permits"), //!< Default license permits reproduction.
    attribution_("requires"), //!< Default license requires attribution.
    commercialuse_("permits"), //<! Default license permits commerical use.
    distribution_("permits"), //!< Default license permits distribution.
    derivative_works_("permits"), //!< Default license permits derivative works.
    coord_precision_(3) //!< 3 decimal digits precision is enough for 1 in 1000 resolution: suits small image use. Higher precision (4, 5 or 6) will be needed for larger images, but increase the SVG XML file size, especially if there are very many data values.
  { // Default constructor.
  }

  svg(const svg& rhs) : x_size_(rhs.x_size_), y_size_(rhs.y_size_)
  { //! Copy constructor copies ONLY X and Y image sizes.
    // TODO Other member data items are NOT copied.  OK? Unused and untested, and perhaps not useful.
    // I think this means that in practice one can't copy an existing customised SVG?
  }

  // Set & get functions for x_size_ and y_size_
  void x_size(unsigned int x)
  { //! Set X-axis (horizontal) image size in SVG units (default pixels).
    x_size_ = x;
  }

  void y_size(unsigned int y)
  { //! Set Y-axis (vertical) image size in SVG units (default pixels).
    y_size_ = y;
  }

  unsigned int x_size()
  { //! \return  X-axis (horizontal width) SVG image size  in SVG units (default pixels).
    return x_size_;
  }

  unsigned int y_size()
  { //! \return  Y-axis (vertical height) SVG image size in SVG units (default pixels).
    return y_size_;
  }

  std::pair<double, double> xy_sizes()
  { //! \return Both X and Y sizes (horizontal width and vertical height) of the SVG image in SVG units (default pixels).
    std::pair<double, double> r;
    r.first = x_size_;
    r.second = y_size_;
    return r;
  }

  unsigned int document_size()
  { //! \return How many group elements groups have been added to the document.
    return static_cast<unsigned int>(document.size());
  }

  void coord_precision(int digits)
  { //! \brief Set decimal digits to be output for X and Y coordinates.
    /*! \details Default stream precision 6 decimal digits is probably excessive.\n

      4.1 Basic data types, integer or float in decimal or scientific (using e format).
      3 or 4 probably enough if image size is under 1000 x 1000.
      This will reduce .svg file sizes significantly for curves represented with many data points.\n

      For example, if a curve is shown using 100 points,
      reducing to precision(3) from 6 will reduce file size by 300 bytes.
      So a default of 3 is used in the default constructor above,
      but can be changed using this function.
      Used in @c svg.write below and so applies to all the entire @c svg document.
     */
    coord_precision_ = digits;
  }

  int coord_precision()
  { //! \return  Decimal digits precision to be output for X and Y coordinates.
    return coord_precision_;
  }

  /*! \brief Write whole .svg 'file' contents to file.

  \details @c svg.write() also has two flavors, a file and an ostream.
  The file version opens an ostream, and calls the stream version.

  The stream version first clears all unnecessary data from the graph,
  builds the document tree, and then calls the write function for the root
  document node, which calls all other nodes through the Visitor pattern.

  TODO provide a filtered-stream version that writes in zipped format type .svgz ?
  http://lists.w3.org/Archives/Public/www-svg/2005Dec/0308.html
  recommends MUST have  correct Content-Encoding headers.
  */
  // --------------------------------------------------------------------------------

  void write(const std::string& filename)
  {
    std::string file(filename); // Copy to avoid problems with const if need to append.
    if (file.find('.') == std::string::npos)
    { // No file type suffix, so provide the default .svg.
      file.append(".svg");
    }
    std::ofstream f_out(file.c_str());
    if(f_out.fail())
    {
      filename_ = ""; // Erase any previous filename,
      // so does not appear in comment if later written just to stream.
      throw std::runtime_error("Unable to open file " + file);
    }
    filename_ = file; // Note so that can embed into file as comment.
    write(f_out);
  }

  void write(std::ostream& s_out)
  { //! Write whole .svg 'file' contents to stream (perhaps a file).
    write_header(s_out); // "<?xml version=...
    // write svg document, begin <svg tag.
    // <svg xml:space="preserve" width="5.5in" height=".5in">

    s_out << "<svg width=\"" << x_size_ << "\" height =\"" << y_size_
      << "\" version=\"1.1\"\n" // http://www.w3.org/TR/SVG11/

      // 1.2 draft specification at http://www.w3.org/TR/SVG12/
      "xmlns:svg =\"http://www.w3.org/2000/svg\"\n"
      "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
      "xmlns:cc=\"http://web.resource.org/cc/\"\n"
      "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
      "xmlns =\"http://www.w3.org/2000/svg\"\n"

      // xml namespace containing svg shapes rect, circle...
      // so can write rect or circle avoiding need for qualification svg:rect, svg:circle...
      // This site isn't visited, but if missing Firefox, at least, will fail to render.

      // Might also need xlink and ev,
      // but Inkscape doesn't provide it, so we don't until required.
      //   "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
      // tells that elements and attributes which are prefixed by "xlink:"
      // are a part of the xlink specification http://www.w3.org/1999/xlink.
      // Need to use xlink:href to refer to xlink.
      //  "xmlns:ev=\"http://www.w3.org/2001/xml-events\"\n"
      << '>' << std::endl;

    // Bind the required namespaces, see http://jwatt.org/svg/authoring/#namespace-binding
    //
    // << " baseProfile = \"full\"\n"
    // is recommended and might be needed, but defaults to baseProfile = "none"
    // according to
    // http://www.w3.org/TR/SVG/struct.html#SVGElementBaseProfileAttribute

    s_out << package_info << std::endl;

    if (author_ == "")
    {
      author_ = holder_copyright_;
    }
    else
    {
      if (holder_copyright_ == "")
      {
        holder_copyright_ = author_;
      }
      else
      { // Copyright has been assigned to another, so list separately.
        s_out << "<!-- " << author_ << " --> "<< std::endl;
      }
    }
    if (holder_copyright_ != "")
    { // Output copyright & date as both comment and meta data.
      s_out << "<!-- SVG Plot Copyright " << holder_copyright_ << " " << date_copyright_ << " --> "<< std::endl;
      s_out << "<meta name=\"copyright\" content=\"" << holder_copyright_ << "\" />" << std::endl;
      //  Example:  \verbatim <meta name="copyright" content="Paul A. Bristow" /> \endverbatim
      s_out << "<meta name=\"date\" content=\"" << date_copyright_ << "\" />" << std::endl;
      // Example: \verbatim <meta name="Date" content="20071101"> \endverbatim
    }
    if (image_desc_ != "")
    {
      s_out << "<!-- " << image_desc_ << " --> "<< std::endl;
      s_out << "<desc>" << image_desc_ << "</desc>" << std::endl;
    }
    if (title_document_ != "")
    {
       s_out << "<title>" << title_document_ << "</title>" << std::endl;
    }
    if (filename_ != "")
    { // Example: <!-- File demo_1d_plot.svg -->
      s_out << "<!-- File " << filename_ << " --> "<< std::endl;
    }
    s_out.precision(coord_precision());

    if (is_boost_license_ == true)
    {
      s_out <<
        "<!-- Use, modification and distribution of this Scalable Vector Graphic file -->"
        "\n<!-- are subject to the Boost Software License, Version 1.0. -->"
        "\n<!-- (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) -->\n"
        << std::endl;
    } // is_boost_license
    if (is_license_ == true)
    { // Add license information to the file.
      // http://dublincore.org/documents/2000/07/16/usageguide/
      // http://dublincore.org/documents/2000/07/16/usageguide/sectc.shtml#creator
      s_out <<
        "<metadata id = \"id0\">\n"
          "<rdf:RDF>\n"
             "<cc:Work rdf:about=\"" << filename_ << "\">\n" // Presumably .svg (or svgz?)
               "<dc:format>image/svg+xml</dc:format>\n"
               "<dc:type rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
               "<dc:title> " << (title_document_ != "" ? title_document_ : filename_) << "</dc:title>\n"
               "<dc:creator> <cc:Agent> <dc:title>Boost.Plot</dc:title> </cc:Agent></dc:creator>\n"
               "<dc:author><cc:Agent><dc:title>" << author_ << " </dc:title> </cc:Agent> </dc:author>\n"
               "<dc:rights><cc:Agent><dc:title>" << holder_copyright_ << "</dc:title></cc:Agent></dc:rights>\n"
               "<dc:date>" << date_copyright_ << "</dc:date>\n"
               "<dc:identifier>" << filename_ << "</dc:identifier>\n" // URI for this svg document.
               "<dc:source>" << "Boost.plot 0.5" << "</dc:source>\n"
               "<dc:relation>" << "" << "</dc:relation>\n" // URI to a related document, perhaps user source program.
               "<dc:publisher><cc:Agent><dc:title>" << holder_copyright_ << "</dc:title></cc:Agent></dc:publisher>\n"
               // publisher could be different from copyright holder.
               "<dc:language>en_US</dc:language>\n" // Could be changed to suit, en-GB for example ;-)
               "<dc:description>" << image_desc_ << "</dc:description>\n"
               "<dc:contributor><cc:Agent><dc:title>" << author_ << "</dc:title></cc:Agent></dc:contributor>\n"
               "<dc:subject><rdf:Bag><rdf:li>Boost svg plot keyword</rdf:li></rdf:Bag></dc:subject>\n"
               // Could add keywords string here.
               // License conditions URI: /by/ or /by_na/ ..
               "<cc:license rdf:resource=\"http://creativecommons.org/licenses/\" />\n"
               // Might instead select a specific license like http://creativecommons.org/licenses/by/3.0/
               // rather than a fully fexible combination as below.  Inkscape does this, for example.
             "</cc:Work>\n"
             "<cc:License rdf:about=\"http://creativecommons.org/licenses/\">\n"
               "<cc:" << reproduction_ << " rdf:resource=\"http://web.resource.org/cc/Reproduction\"/>\n"
               "<cc:" << distribution_ << " rdf:resource=\"http://web.resource.org/cc/Distribution\"/>\n"
               "<cc:requires rdf:resource=\"http://web.resource.org/cc/Notice\"/>\n"
               "<cc:" << attribution_ << " rdf:resource=\"http://web.resource.org/cc/Attribution\"/>\n"
               "<cc:" << commercialuse_ << " rdf:resource=\"http://web.resource.org/cc/CommercialUse\"/>\n"
               // Including commercialuse means doesn't display correctly in Inkscape metadata,
               // but the license URI, for example, http://creativecommons.org/licenses/by/3.0/ is shown correctly.
               "<cc:" << derivative_works_ << " rdf:resource=\"http://web.resource.org/cc/DerivativeWorks\"/>\n"
             "</cc:License>\n"
          "</rdf:RDF>\n"
         "</metadata>"
       << std::endl;
    } // is_license
    write_css(s_out);// stylesheet, if any.
    write_document(s_out); // write clip paths and all document elements.
    s_out << "</svg>" << std::endl;   // close off svg tag.
  }

  void license(
    const std::string reproduction = "permits",
    const std::string distribution = "permits",
    const std::string attribution = "requires",
    const std::string commercialuse = "permits",
    const std::string derivative = "permits")
  {  /*! Set several license requirements for the svg document.
       If any are set, then a license is wanted, so @c svg#is_license is set @c true.
       This can be changed using function @c license_on().
     */
    reproduction_ = reproduction;
    distribution_ = distribution;
    attribution_ = attribution;
    commercialuse_ = commercialuse;
    derivative_works_ = derivative;
    is_license_ = true;  // Assume want this if set any of these requirements.
  } // void license

  void license_on(bool l)
  { /*! Set (or not) license using all requirements (default permits).\n
     Implicitly set by setting any license requirement using @c license function.
     */
    is_license_ = l;
  }

  bool license_on()
  { //! Return true if a license has been requested for @c svg header metatadata.
    return is_license_;
  }

  void boost_license_on(bool l)
  { //! Set (or not) to include Boost license text in @c svg header as comment.
    is_boost_license_ = l;
  }

  bool boost_license_on()
  { //! Return true if a boost license has been requested in the @c svg header as comment.
    return is_boost_license_;
  }

  const std::string& reproduction()
  { //! \return  License reproduction requirement.
    return reproduction_;
  }

  const std::string& distribution()
  { //! \return  License distribution requirement.
    return distribution_;
  }
  const std::string& attribution()
  { //! \return  License attribution requirement.
    return attribution_;
  }
  const std::string& commercialuse()
  { //! \return  License commercial use requirement.
    return commercialuse_;
  }

  // -------------------------------------------------------
  // Writes the information about the image to the document.
  // TODO: allow other unit identifiers.
  // -------------------------------------------------------
  void size(unsigned int x, unsigned int y)
  { //! Set both X and Y image size (SVG units, default pixels).
    x_size_ = x;
    y_size_ = y;
  }

  void description(const std::string d)
  { //! Write description to the SVG document (for header as \<desc\> ... \</desc\>).
    image_desc_ = d;
  }

  const std::string& description()
  { //! \return  description of the SVG document (for header as \<desc\>).
    return image_desc_;
  }

  void author(const std::string a)
  { //!  Set author for the SVG document (default is \<copyright_holder\>).
    author_ = a;
  }

  const std::string& author()
  { //!  \return  author of the SVG document (for header as \<author\>).
    return author_;
  }

  void document_title(const std::string title)
  { //!  Set document title for the SVG document (for header as \<title\> ... \</title\>).
    title_document_ = title;
  }

  const std::string document_title()
  { //!   \return document title for the SVG document (for header as \<title\>).
    return title_document_;
  }

  void copyright_holder(const std::string copyright_holder)
  { //!  Set document title for the SVG document (for header as  \<copyright_holder\>).
    holder_copyright_ = copyright_holder;
  }

  const std::string copyright_holder()
  { //!  \return  document title for the SVG document (for header as  \<copyright_holder\> ).
    return holder_copyright_;
  }

  void copyright_date(const std::string copyright_date)
  { //!  Set copyright date for the SVG document (for header as \<copyright_date\>).
    date_copyright_ = copyright_date;
  }

  const std::string copyright_date()
  { //!  \return copyright date for the SVG document (for header as \<copyright_date\>).
    return date_copyright_;
  }

  void image_filename(const std::string filename)
  { //!  Set image filename for the SVG document (for header as \<filename\>).
    filename_ = filename;
  }

  const std::string image_filename()
  { //! \return  image filename for the SVG document (for header as \<filename\>).
    return filename_;
  }

  // ------------------------------------------------------------------------
  // Add (push_back) information about line, rec, circle & ellipse to the document.
  // ------------------------------------------------------------------------

  line_element& line(double x1, double y1, double x2, double y2)
  { //! Add (push_back) information about a line to the document.
    //! 'line' element defines a line segment that starts at one point (x1, y1) and ends at another (x2, y2).
    return document.line(x1, y1, x2, y2);
  }

  rect_element& rect(double x1, double y1, double x2, double y2)
  { //! push_back information about a rectangle to the document.
    //! 'Rect' element defines a rect segment with one point (x1, y1) and opposite vertex is (x2, y2).
    return document.rect(x1, y1, x2, y2);
  }

  circle_element& circle(double x, double y, unsigned int radius = 5)
  { //! push_back information about a circle to the document.
    //! 'circle' element defines a circle centered at (x1, y1) and its radius.
    return document.circle(x, y, radius);
  }

  ellipse_element& ellipse(double rx, double ry, double cx, double cy)
  { //! push_back information about a ellipse to the document.
    //! 'ellipse' element defines a ellipse centered at point (x1, y1) and its two radii.
    return document.ellipse(rx, ry, cx, cy);
  }

  text_element& text(double x, double y, const std::string& text,
    const text_style& style, // font size, font family etc., and any text_length estimate.
    align_style align, rotate_style rotate)
  { //! push_back information about text to the document, with location, style (font size, family etc and text_length), alignment & rotation.
    return document.text(x, y, text, style, align, rotate); // see svg_tag.hpp 2137 for definition.
  }
  /*  text_length  SVG XML textLength = "<length>"
  The author's (this program) computation of the total sum of all of 
  the advance values that correspond to character data within this element,
  including the advance value on the glyph (horizontal or vertical),
  the effect of properties 'kerning', 'letter-spacing' and 'word-spacing'
  and adjustments due to attributes 'dx' and 'dy' on 'tspan' elements. 
  
  This value is used to calibrate the user agent's (renderer) own calculations with that of the author.
  The purpose of this attribute is to allow the author to achieve exact alignment, 
  in visual rendering order after any bidirectional reordering, 
  for the first and last rendered glyphs that correspond to this element;
  thus, for the last rendered character (in visual rendering order after any bidirectional reordering), 
  any supplemental inter-character spacing beyond normal glyph advances are ignored (in most cases) 
  when the user agent (this program) determines the appropriate amount 
  to expand/compress the text string to fit within a length of 'textLength'.
  */

  // \sa https://www.w3.org/TR/SVG11/text.html#TextElementTextLengthAttribute

  // Polygon for shapes with many vertices.
  polygon_element& polygon(double x, double y, bool f = true) // 1st point only, add others later with .P(x, y).
  { //! push_back info about 1st point of a polygon shape (add others later with .P(x, y)).
    return document.polygon(x, y, f);
  }

  //JVTODO: Replace with STL container template version.

  polygon_element& polygon(std::vector<poly_path_point>& v, bool f = true)
  { //! push_back a complete many-sided polygon to the document with vertices specified as a vector of path_points.
    return document.polygon(v, f);
  }

  // Specific polygon shapes: triangle, rhombus, pentagon & hexagon. (not in SVG standard but convenient).
  polygon_element& triangle(double x1, double y1, double x2, double y2, double x3, double y3, bool f = true)
  { //! push_back a complete triangle to the document.
    return document.polygon(x1, y1, f).P(x2, y2).P(x3, y3);
  }

  polygon_element& rhombus(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, bool f = true)
  { //! push_back the four coordinate of a complete rhombus to the document.
    return document.polygon(x1, y1, f).P(x2, y2).P(x3, y3).P(x4, y4);
  }

  polygon_element& pentagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, bool f = true)
  { //! push_back the five coordinates complete pentagon to the document.
    return document.polygon(x1, y1, f).P(x2, y2).P(x3, y3).P(x4, y4).P(x5, y5);
  }

  polygon_element& hexagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, bool f = true)
  { //! push_back the coordinate of the points of a complete hexagon to the document.
    return document.polygon(x1, y1, f).P(x2, y2).P(x3, y3).P(x4, y4).P(x5, y5).P(x6, y6);
  }

  polyline_element& polyline(double x, double y)
  { //! push_back info about the 1st point of a polyline (add others later with .P(x, y)).

    return document.polyline(x, y);
  }

  polyline_element& polyline(double x1, double y1, double x2, double y2)
  { //! push_back info about the 1st & 2nd point of a polyline (add others later with .P(x, y)).
    return document.polyline(x1, y1).P(x2, y2);
  }

  polyline_element& polyline(std::vector<poly_path_point>& v)
  { //! push_back a complete many-sided polygon to the document, from a vector of path_points.
    return document.polyline(v);
  }

  // Add information about path, clip_path to the document.

  path_element& path()
  { //! Construct an empty path, ready for additions with chainable functions M., L., ...
    return document.path(); //! \return reference to @c path_element.
  }

  clip_path_element& clip_path(const rect_element& rect, const std::string& id)
  { //! Rectangle outside which 'painting' is 'clipped' so doesn't show.
    clip_paths.push_back(clip_path_element(id, rect));
    return clip_paths[clip_paths.size()-1]; //! \return Reference to @c clip_path_element.
  }

  g_element& add_g_element()
  { //! Add information about a group element to the document.
    //! Increments the size of the array of g_elements, returned by g_element.size().
    return document.add_g_element(); //! \return reference to the added group element @c add_g_element.
  }

  g_element& g(int i)
  { //! from array of g_elements, indexed by group type, PLOT_BACKGROUND, PLOT_WINDOW_BACKGROUND, ... SVG_PLOT_DOC_CHILDREN,
    return document.g(i); //! \return reference to the ith group element.
  }

  //// -------------------------------------------------------------
  //// Load stylesheet
  //// -------------------------------------------------------------

  //void load_stylesheet(const std::string& input)
  //{ // Load a stylesheet into string css from an input file.
  //  std::ifstream if_str(input.c_str());

  //  if(if_str.fail())
  //  {
  //    throw std::runtime_error("Error opening file " + input);
  //  }
  //  if(!validate_stylesheet(if_str))
  //  {
  //    throw std::runtime_error("Error loading stylesheet!");
  //  }

  //  if_str.clear();
  //  if_str.seekg(0);

  //  std::string tmp;
  //  css = "";

  //  while(std::getline(if_str, tmp))
  //  {
  //    css += tmp;
  //  }
  //} // svg& load_stylesheet
}; // class svg

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_HPP
