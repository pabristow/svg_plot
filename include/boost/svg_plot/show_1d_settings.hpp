/*! \file

  \brief Shows settings and options for 1D Plot.

  \author Paul A. Bristow
  \date Mar 2009
  \see show_2d_settings.cpp for 2-D plot.
 */
//  show_1d_settings.hpp
// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2009, 2011

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SHOW_1D_SETTINGS_HPP
#define BOOST_SVG_SHOW_1D_SETTINGS_HPP

#include <boost/svg_plot/detail/pair.hpp>
//  using boost::svg::detail::operator<<;

#include <iostream>

namespace boost
{
namespace svg
{

  // Declaration, defined below.
  //! \cond DETAIL
  void show_1d_plot_settings(svg_1d_plot&, std::ostream&);
  //! \endcond

//! Strings describing each bit in @c std::ios_base::fmtflags.
const char* fmtFlagWords[16] =
{
  "skipws", "unitbuf", "uppercase","showbase","showpoint","showpos","left","right",
    "internal","dec","oct","hex","scientific","fixed","boolalpha", "?"
    /*
    enum _Fmtflags
    { // Show action when bit set == 1
    skipws = 0x0001,  //   Skip white space on input.
    unitbuf = 0x0002, // Output each char - unbuffered.
    uppercase = 0x0004,// Display uppercase A through F for hexadecimal values and E for scientific values
    showbase = 0x0008, // Display numeric constants in a format that can be read by the C++ compiler.
    showpoint = 0x0010, // Show decimal point and trailing zeros for floating-point values.
    showpos = 0x0020, // Show plus signs (+) for positive values.
    left = 0x0040, // Left-align values; pad on the right with the fill character.
    right = 0x0080,// Right-align values; pad on the left with the fill character (default alignment).
    internal = 0x0100, // Add fill characters after any leading sign or base indication, but before the value.
    dec = 0x0200, // Format numeric values as base 10 (decimal) (default radix).
    oct = 0x0400, // Format numeric values as base 8 (octal).
    hex = 0x0800, // Format numeric values as base 16 (hexadecimal).
    scientific = 0x1000,// Display floating-point numbers in scientific format.
    fixed = 0x2000, // Display floating-point numbers in fixed format.
    boolalpha = 0x4000, // Show bool as word true or false.
    // 0x8000 un-used. // show as ?
    adjustfield = 0x01c0,
    basefield = 0x0e00,
    floatfield = 0x3000,
    _Fmtmask = 0x7fff,
    _Fmtzero = 0
    };
    */
}; // const char* fmtFlagWords

void outFmtFlags(std::ios_base::fmtflags fmtFlags, std::ostream& os, const char* term)
{ //! Output strings describing all bits in @c std::ios_base::fmtflags.
  /*! @b Usage: @c outFmtFlags();
     For example, by default outputs to @c std::cerr \n
     \verbatim
     FormatFlags: skipws showbase right dec."
     \endverbatim
     \n
     Default parameter values are:\n
     \code
       void outFmtFlags(fmtflags fmtFlags = cout.flags(), ostream& os = cerr, const char* term = ".\n");
     \endcode
   */
  const int up = 16; // Words across page.
  const int count = 16;  // 16 because using unsigned short int (must be at least 16 bits).
  std::ios_base::fmtflags flags = os.flags(); // Save to restore.
  fmtFlags &= static_cast<std::ios_base::fmtflags>(0x7FFF);  // _Fmtmask // clear un-used bits.
  os << "IOS format flags (" << std::showbase << std::hex << fmtFlags << std::dec << ")" ; // hex value.
  if (fmtFlags != 0)
  {
    for(int i = 0, j = 1, hit = 0; i < count; ++i)
    {
      if ((fmtFlags & j) != 0)
      {
        if (hit == 0)
        { // First word no separator.
          os << ' ';
        }
        else
        {
          os << (( (hit % up) == 0) ? ",\n" : ", ");
        }
        os  << fmtFlagWords[i];
        ++hit; // Count to add space or newline as appropriate.
      }
      j <<= 1;
    }
  }
  os << term; // eg "\n" or ". "
  os.setf(flags);  // Restore.
}  // outFmtFlags

//namespace detail
//{
//
//  //std::ostream& operator<< (std::ostream&, const std::pair<double, double>&);
//  //template<class T1, class T2> std::ostream& operator<< (std::ostream&, std::pair<T1, T1>&);
//
//  template<class T1, class T2>
//  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
//  { // Output a pair of values.
//         os << p.first << ", " << p.second;
//      // Outputs:  1.2, 3.4
//      return os;
//  } // std::ostream& operator<<
//
//  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p)
//  { // Output a pair of values.
//      int precision = os.precision(3); // Save & use rather than default precision(6)
//      os << p.first << ", " << p.second;
//      // Outputs:  1.2, 3.4
//      os.precision(precision); // Restore.
//      return os;
//  } // std::ostream& operator<<
//
//  // Maybe better as:
//  //template<typename charT, typename traits, typename T1, typename T2>
//  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
//  //{
//  //  return os << p.first << " " << p.second;
//  //}
//  //
//  //// Explicit double, double.
//  //template<typename charT, typename traits>
//  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
//  //{
//  //  return os << p.first << " " << p.second;
//  //}
//  // but OK for this purpose.
//} // namespace detail

//! Outputs strings to show horizontal orientation: left, right or none.
const std::string l_or_r(int i)
{
 return ((i < 0) ? "left" : ((i == 0) ? "none" : "right"));
}

//! Outputs strings to show vertical orientation: top, bottom, or none.
const std::string t_or_b(int i)
{
 return ((i < 0) ? "bottom" : ((i == 0) ? "none" : "top"));
}

void show_1d_plot_settings(svg_1d_plot& plot, std::ostream& os = std::cout)
{ /*! \brief Diagnostic display of all settings of a 1D plot.
     Outputs a long list of about hundred of plot parameter settings, by default to @c std::cout.
    This list is invaluable if the plot does not look as expected.
    \warning This creates about 100 lines of output, so should be used sparingly!
    \param plot Name of svg_1d_plot.
    \param os @c std::ostream& for output, default @c std::cout.
  */

  using boost::svg::detail::operator<<;
  // std::ostream& operator<< (std::ostream&, const std::pair<double, double>&);
  // defined above.
  // Save std::cout state in case use the default.
  std::ios_base::fmtflags flags = std::cout.flags(); // Save format flags to restore on exit.
  std::streamsize prec = std::cout.precision(); // Save precision to restore on exit.

  os << std::boolalpha << std::endl;
  os << "\nShow 1d_plot_settings:" << std::endl;
  os << "axes_on " << plot.axes_on() << std::endl;
  os << "background_border_width " << plot.background_border_width() << std::endl;
  os << "background_border_color " << plot.background_border_color() << std::endl;
  os << "background_color " << plot.background_color() << std::endl;
  os << "image_border_margin() " << plot.image_border_margin() << std::endl;
  os << "image_border_width() " << plot.image_border_width() << std::endl;
  os << "coord_precision " << plot.coord_precision() << std::endl;
  os << "copyright_date  " << plot.copyright_date() << std::endl;
  os << "copyright_holder " << plot.copyright_holder() << std::endl;
  os << "description "<< plot.description() << std::endl;
  os << "document_title \"" << plot.document_title()  << '"' << std::endl;
  //os << plot.draw_bezier_lines() << std::endl;
  os << "image x_size " << plot.x_size() << std::endl;
  os << "image y_size " << plot.y_size() << std::endl;
  os << "image_size " << plot.size() << std::endl;
  os << "image_filename " << plot.image_.image_filename() << std::endl;
  os << "legend_on " << plot.legend_on() << std::endl;
  std::pair<double, double> lt = plot.legend_top_left();
  std::pair<double, double> rb = plot.legend_bottom_right();
  os << "legend_place " << plot.legend_place() << std::endl;
  os << "legend_top_left " << lt << ", legend_bottom_right " << rb << std::endl;
  os << "legend_background_color " << plot.legend_background_color() << std::endl;
  os << "legend_border_color " << plot.legend_border_color() << std::endl;
  os << "legend_color " << plot.legend_color() << std::endl;
  os << "legend_title \"" << plot.legend_title()  << '"' << std::endl;
  os << "legend_title_font_size " << plot.legend_title_font_size() << std::endl;
  os << "legend_font_size " << plot.legend_title_font_size() << std::endl;  // Legend text.

  // Not implemented yet.
  //os << "legend_font_weight " << plot.legend_font_weight() << std::endl;
  //os << "legend_font_width " << plot.legend_font_width() << std::endl;
  os << "legend_width " << plot.legend_width() << std::endl;
  os << "legend_lines " << plot.legend_lines() << std::endl;

  os << "NaN limit points stroke color " << plot.nan_limit_color() << std::endl;
  os << "NaN limit points fill color " << plot.nan_limit_fill_color() << std::endl;

  os << "license_on " << plot.license_on() << std::endl;
  os << "license_reproduction " << plot.license_reproduction() << std::endl;
  os << "license_distribution " << plot.license_distribution() << std::endl;
  os << "license_attribution " << plot.license_attribution() << std::endl;
  os << "license_commercial_use " << plot.license_commercialuse() << std::endl;
  os << "plot_background_color " << plot.plot_background_color() << std::endl;
  os << "plot_border_color " << plot.plot_border_color() << std::endl;
  os << "plot_border_width " << plot.plot_border_width() << std::endl;
  os << "plot_window_on " << plot.plot_window_on() << std::endl;
  os << "plot_window_x " << plot.plot_window_x() << std::endl;
  os << "plot_window_x_left " << plot.plot_window_x_left() << std::endl;
  os << "plot_window_x_right " << plot.plot_window_x_right() << std::endl;
  os << "plot_window_y " << plot.plot_window_y() << std::endl;
  os << "plot_window_y_top " << plot.plot_window_y_top() << std::endl;
  os << "plot_window_y_bottom " << plot.plot_window_y_bottom() << std::endl;
  os << "title_on " << plot.title_on() << std::endl;
  os << "title \"" << plot.title() << "\""<< std::endl;
  os << "title_color " << plot.title_color() << std::endl;
  os << "title_font_alignment " << plot.title_font_alignment() << std::endl;
  os << "title_font_decoration " << plot.title_font_decoration() << std::endl;
  os << "title_font_family " << plot.title_font_family() << std::endl;
  os << "title_font_rotation " << plot.title_font_rotation() << std::endl;
  os << "title_font_size " << plot.title_font_size() << std::endl;
  os << "title_font_stretch " << plot.title_font_stretch() << std::endl;
  os << "title_font_style " << plot.title_font_style() << std::endl;
  // Not implemented yet.
  //os << "title_font_weight " << plot.title_font_weight() << std::endl;
  //os << "title_font_width " << plot.title_font_width() << std::endl;
  os << "x_value_precision " << plot.x_value_precision() << std::endl;
  os << "x_value_ioflags " << std::hex << plot.x_value_ioflags() << std::dec << ' ';
  outFmtFlags(plot.x_value_ioflags(), os,  ".\n");
  os << "x_plusminus_on " << plot.x_plusminus_on() << std::endl;
  os << "x_plusminus_color " << plot.x_plusminus_color() << std::endl;
  os << "x_addlimits_on " << plot.x_addlimits_on() << std::endl;
  os << "x_addlimits_color " << plot.x_addlimits_color() << std::endl;
  os << "x_df_on " << plot.x_df_on() << std::endl;
  os << "x_df_color " << plot.x_df_color() << std::endl;

  os << "x_id_on " << plot.x_id_on() << std::endl;
  os << "x_id_color " << plot.x_id_color() << std::endl;

  os << "x_datetime_on " << plot.x_datetime_on() << std::endl;
  os << "x_datetime_color " << plot.x_datetime_color() << std::endl;

  os << "x_order_on " << plot.x_df_on() << std::endl;
  os << "x_order_color " << plot.x_df_color() << std::endl;

  os << "x_prefix \"" << plot.x_prefix()  << '"' << std::endl;
  os << "x_separator \"" << plot.x_separator()  << '"' << std::endl;
  os << "x_suffix \"" << plot.x_suffix()  << '"' << std::endl;

  // Not applicable to 1D
  //os << "y_value_precision " << plot.y_value_precision() << std::endl;
  //os << "y_value_ioflags " << std::hex << plot.y_value_ioflags() << dec << ' ';
  //outFmtFlags(plot.y_value_ioflags(), os,  ".\n");
  os << "x_max " << plot.x_max() << std::endl;
  os << "x_min " << plot.x_min() << std::endl;
  os << "x_axis_on " << plot.x_axis_on() << std::endl;
  os << "x_axis_color() " << plot.x_axis_color() << std::endl;
  os << "x_axis_label_color " << plot.x_axis_label_color() << std::endl;
  os << "x_values_color " << plot.x_values_color() << std::endl;
  os << "x_axis_width " << plot.x_axis_width() << std::endl;
  os << "x_label_on " << plot.x_label_on() << std::endl;
  os << "x_label \"" << plot.x_label()  << '"' << std::endl;
  os << "x_label_color " << plot.x_label_color() << std::endl;
  os << "x_label_font_family " << plot.x_label_font_family() << std::endl;
  os << "x_label_font_size " << plot.x_label_font_size() << std::endl;
  os << "x_label_units " << plot.x_label_units() << std::endl;
  os << "x_label_units_on " << plot.x_label_units_on() << std::endl;
  // Not implemented yet.
  //os << "x_label_width " << plot.x_label_width() << std::endl;
  os << "x_major_labels_side " << l_or_r(plot.x_major_labels_side()) << std::endl;
  os << "x_values_font_size " << plot.x_values_font_size() << std::endl;
  os << "x_values_color " << plot.x_values_color() << std::endl;
  os << "x_values_precision " << plot.x_values_precision() << std::endl;
  os << "x_values_ioflags " << plot.x_values_ioflags() << std::endl;
  os << "x_major_label_rotation " << plot.x_major_label_rotation() << std::endl;
  os << "x_major_grid_color " << plot.x_major_grid_color() << std::endl;
  os << "x_major_grid_on " << plot.x_major_grid_on() << std::endl;
  os << "x_major_grid_width " << plot.x_major_grid_width() << std::endl;
  os << "x_major_interval " << plot.x_major_interval() << std::endl;
  os << "x_major_tick " << plot.x_major_tick() << std::endl;
  os << "x_major_tick_color " << plot.x_major_tick_color() << std::endl;
  os << "x_major_tick_length " << plot.x_major_tick_length() << std::endl;
  os << "x_major_tick_width " << plot.x_major_tick_width() << std::endl;
  os << "x_minor_interval " << plot.x_minor_interval() << std::endl;
  os << "x_minor_tick_color " << plot.x_minor_tick_color() << std::endl;
  os << "x_minor_tick_length " << plot.x_minor_tick_length() << std::endl;
  os << "x_minor_tick_width " << plot.x_minor_tick_width() << std::endl;
  os << "x_minor_grid_on " << plot.x_minor_grid_on() << std::endl;
  os << "x_minor_grid_color " << plot.x_minor_grid_color()<< std::endl;
  os << "x_minor_grid_width " << plot.x_minor_grid_width() << std::endl;
  os << "x_range() " << plot.x_range() << std::endl;
  os << "x_num_minor_ticks " << plot.x_num_minor_ticks() << std::endl;
  os << "x_ticks_down_on " << plot.x_ticks_down_on() << std::endl;
  os << "x_ticks_up_on " << plot.x_ticks_up_on() << std::endl;
  os << "x_ticks_on_window_or_axis " << t_or_b(plot.x_ticks_on_window_or_axis()) << std::endl;
  os << "x_axis_position " << plot.x_axis_position() << std::endl;
  os << "x_autoscale " << plot.x_autoscale() << std::endl;
  os << "x_autoscale_check_limits " << plot.autoscale_check_limits() << std::endl;
  os << "confidence alpha " << plot.confidence() << std::endl;
  os << "data lines width " << plot.data_lines_width() << std::endl;
  os.flags(flags); // Restore.
  os.precision(prec);
} // void show_1d_plot_settings(svg_1d_plot& plot, std::ostream& = std::cout)

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_SHOW_1D_SETTINGS_HPP
