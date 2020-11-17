/*! \file show_2d_settings.hpp

  \brief Shows settings and options for 2D Plot.
  Example:
  \code
  #include <boost/svg_plot/show_2d_settings.hpp>

  my_plot.show_2d_plot_settings();
  \endcode

  \see show_1d_settings.cpp for 1D plot.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2009, 2012

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SHOW_2D_SETTINGS_HPP
#define BOOST_SVG_SHOW_2D_SETTINGS_HPP

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/svg_plot/detail/pair.hpp> // Output operator<< local version.
#include <boost/quan/xiostream.hpp>

#include <iostream> // std::cout...

namespace boost
{
namespace svg
{

// Declaration (defined below).
//! \cond DETAIL
  void show_2d_plot_settings(svg_2d_plot& plot, std::ostream& os);
//! \endcond

const std::string l_or_r(int i)
{
  return ((i < 0) ? "left" : ((i == 0) ? "none" : "right"));
}

const std::string t_or_b(int i)
{
  return ((i < 0) ? "bottom" : ((i == 0) ? "none" : "top"));
}

void show_2d_plot_settings(svg_2d_plot& plot, std::ostream& os = std::cout)
{ /*! Diagnostic display of all settings of a 2D plot.
   \details  Outputs a long list of over 100 plot parameter settings, default to @c std::cout.
    This list is invaluable if the plot does not look as expected.
   \warning This creates about 100 lines of output, so should be used sparingly!
   \param plot Name of 2D-plot.
   \param os Output stream for diagnostic output, default @ std::cout.
  */

  using boost::svg::detail::operator<<;
  //std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p); //! Output pair of doubles to ostream.

  std::ios_base::fmtflags iostate = std::cout.flags(); // Save all format flags to restore on exit.
  std::streamsize precision = std::cout.precision(); // Save precision to restore on exit.
  os.precision(4);
  os << std::dec << std::boolalpha << std::endl;

  os <<"\nSVG 2-D plot settings" 
              "\n(default units pixels)"
              "\n========================"
    << std::endl;

  os << "axes_on " << plot.axes_on() << std::endl;
  os << "background_border_width " << plot.background_border_width() << std::endl;
  os << "background_border_color " << plot.background_border_color() << std::endl;
  os << "background_color " << plot.background_color() << std::endl;
  os << "image_border_margin() " << plot.image_border_margin() << std::endl;
  os << "image_border_width() " << plot.image_border_width() << std::endl;
  os << "coord_precision " << plot.coord_precision() << std::endl;
  os << "copyright_date  " << plot.copyright_date() << std::endl;
  os << "copyright_holder " << plot.copyright_holder() << std::endl;
  os << "description " << plot.description() << std::endl;
  os << "document_title "  << '"' << plot.document_title()  << '"' << std::endl;
  //os << plot.draw_bezier_lines() << std::endl;
  os << "image x_size " << plot.x_size() << std::endl;
  os << "image y_size " << plot.y_size() << std::endl;
  //os << "image x & y sizes " << plot.size() << std::endl;
  os << "image_filename " << plot.image_.image_filename() << std::endl;
  os << "legend_on " << plot.legend_on() << std::endl;
  std::pair<double, double> lt = plot.legend_top_left();
  std::pair<double, double> rb = plot.legend_bottom_right();
  os << "legend_place " << plot.legend_place() << std::endl;
  os << "legend_top_left " << lt << ", legend_bottom_right " << rb << std::endl;
  os << "legend_background_color " << plot.legend_background_color() << std::endl;
  os << "legend_border_color " << plot.legend_border_color() << std::endl;
  os << "legend_color " << plot.legend_color() << std::endl;
  os << "legend_title "  << '"' << plot.legend_title()  << '"' << std::endl;
  os << "legend_text_font_size " << plot.legend_text_font_size() << std::endl; // Font for marker text.
  os << "legend_title_font_size " << plot.legend_title_font_size() << std::endl; // Font for legend title.
  os << "legend_font_weight " << plot.legend_font_weight() << std::endl; // normal or might be bold.
  os << "legend_text_font_size " << plot.legend_text_font_size() << std::endl;
  os << "legend_title_font_size " << plot.legend_title_font_size() << std::endl;
  os << "legend_title_font_weight " << plot.legend_title_font_weight() << std::endl; // normal or might be bold?
  os << "legend_text_font_weight " << plot.legend_text_font_weight() << std::endl; // normal or might be bold?
  os << "legend_text_style " << plot.legend_text_style_ << std::endl;
  os << "legend_title_style " << plot.legend_title_style_ << std::endl;
  os << "legend_title_text_length " << plot.legend_title_text_length() << std::endl;
  os << "legend_lines " << plot.legend_lines() << std::endl;
  os << "legend_width " << plot.legend_width() << std::endl;
  // Not useful at present, so not longer implemented.
  // os << "legend_font_width " << plot.legend_font_width() << std::endl;
  os << "NaN limit points stroke color " << plot.nan_limit_color() << std::endl;
  os << "NaN limit points fill color " << plot.nan_limit_fill_color() << std::endl;
  os << "license_on " << plot.license_on() << std::endl;
  os << "license_reproduction " << plot.license_reproduction() << std::endl;
  os << "license_distribution " << plot.license_distribution() << std::endl;
  os << "license_attribution " << plot.license_attribution() << std::endl;
  os << "license_commercialuse " << plot.license_commercialuse() << std::endl;
  os << "one_sd_color " << plot.one_sd_color() << std::endl;
  os << "two_sd_color " << plot.two_sd_color() << std::endl;
  os << "three_sd_color " << plot.three_sd_color() << std::endl;
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
  os << "title_on " << plot.title_on() << std::endl; // true
  os << "title \"" << plot.title() << "\""<< std::endl; // Expect "My plot title" but get ""
  os << "title_color " << plot.title_color() << std::endl;
  os << "title_font_alignment " << plot.title_font_alignment() << std::endl;
  os << "title_font_decoration " << plot.title_font_decoration() << std::endl;
  os << "title_font_family " << plot.title_font_family() << std::endl;
  os << "title_font_rotation " << plot.title_font_rotation() << std::endl;
  os << "title_font_size " << plot.title_font_size() << std::endl;
  os << "title_font_stretch " << plot.title_font_stretch() << std::endl;
  os << "title_font_style " << plot.title_font_style() << std::endl;
  os << "title_font_weight " << plot.title_font_weight() << std::endl;
  os << "title_style " << plot.title_style_ << std::endl;
  os << "title_text_length " << plot.title_text_length() << std::endl;
  os << "x_values_on "  << plot.x_values_on() << std::endl;
  os << "x_values_font_size "  << plot.x_values_font_size() << std::endl;
  os << "x_values_font_family "  << plot.x_values_font_family() << std::endl;
  os << "x_values_precision " << plot.x_values_precision() << std::endl;
  os << "x_values_ioflags " << std::hex << plot.x_values_ioflags() << std::dec << ' ';
  outFmtFlags(plot.x_value_ioflags(), os,  ".\n");
  os << "y_values_precision " << plot.y_values_precision() << std::endl;
  os << "y_values_font_size() " << plot.y_value_precision() << std::endl; // duplicate????
  os << "y_values_ioflags " << std::hex << plot.y_values_ioflags() << std::dec << ' ';
  outFmtFlags(plot.y_values_ioflags(), os,  ".\n");
  os << "y_values_color " << plot.y_values_color() << std::endl;
  os << "y_values_font_family() " << plot.y_values_font_family() << std::endl;
  os << "y_values_font_size() " << plot.y_values_font_size() << std::endl;
  os << "x_max " << plot.x_max() << std::endl;
  os << "x_min " << plot.x_min() << std::endl;
  os << "x_autoscale " << plot.x_autoscale() << std::endl;
  os << "y_autoscale " << plot.y_autoscale() << std::endl;
  os << "xy_autoscale " << plot.xy_autoscale() << std::endl;
  os << "x_autoscale_check_limits " << plot.autoscale_check_limits() << std::endl;
  os << "x_axis_on " << plot.x_axis_on() << std::endl;
  os << "x_axis_color() " << plot.x_axis_color() << std::endl;
  os << "x_axis_label_color " << plot.x_axis_label_color() << std::endl;
  os << "x_values_color " << plot.x_values_color() << std::endl;
  os << "x_axis_width " << plot.x_axis_width() << std::endl;
  os << "x_label_on " << plot.x_label_on() << std::endl;
  os << "x_label " << '"' << plot.x_label() << '"' << std::endl;
  os << "x_label_color " << plot.x_label_color() << std::endl;
  os << "x_label_font_family " << plot.x_label_font_family() << std::endl;
  os << "x_label_font_size " << plot.x_label_font_size() << std::endl;
  os << "x_label_units " << plot.x_label_units() << std::endl;
  os << "x_label_units_on " << plot.x_label_units_on() << std::endl;
  //os << "x_label_width " << plot.x_label_width() << std::endl;
  // Not useful at present, so not longer implemented.
  os << "x_major_labels_side " << l_or_r(plot.x_major_labels_side()) << std::endl;
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
  os << "y_axis_position " << plot.y_axis_position() << std::endl;
  os << "x_axis_position " << plot.x_axis_position() << std::endl;
  os << "x_plusminus_on " << plot.x_plusminus_on() << std::endl;
  os << "x_plusminus_color " << plot.x_plusminus_color() << std::endl;
  os << "x_df_on " << plot.x_df_on() << std::endl;
  os << "x_df_color " << plot.x_df_color() << std::endl;
  os << "x_prefix " << plot.x_prefix() << std::endl;
  os << "x_separator " << plot.x_separator() << std::endl;
  os << "x_suffix " << plot.x_suffix() << std::endl;
  os << "xy_values_on "  << plot.xy_values_on() << std::endl;
  os << "y_label_on "  << '"' << plot.y_label_on() << '"' << std::endl;
  os << "y_label_axis " << plot.y_label_axis() << std::endl;
  os << "y_axis_color " << plot.y_axis_color() << std::endl;
  os << "y_axis_label_color " << plot.y_axis_label_color() << std::endl;
  os << "y_axis_on " << plot.y_axis_on() << std::endl;
  os << "axes_on " << plot.axes_on() << std::endl; // Both x and y axes on
  os << "y_axis_value_color " << plot.y_axis_value_color() << std::endl;
  os << "y_axis_width " << plot.y_axis_width() << std::endl;
  os << "y_label " << plot.y_label() << std::endl;
  os << "y_label_color " << plot.y_label_color() << std::endl;//
  os << "y_label_font_family " << plot.y_label_font_family() << std::endl;
  os << "y_label_font_size " << plot.y_label_font_size() << std::endl;
  os << "y_label_on " << plot.y_label_on() << std::endl;
  os << "y_label_units " << plot.y_label_units() << std::endl;
  os << "y_label_units_on " << plot.y_label_units_on() << std::endl;
  os << "y_label_width " << plot.y_label_width() << std::endl;
  os << "y_major_grid_on "  << plot.y_major_grid_on() << std::endl;
  os << "y_major_grid_color " << plot.y_major_grid_color() << std::endl;
  os << "y_major_grid_width " << plot.y_major_grid_width() << std::endl;
  os << "y_major_interval " << plot.y_major_interval() << std::endl;
  os << "y_major_labels_side " << t_or_b(plot.y_major_labels_side()) << std::endl;
  os << "y_major_label_rotation " << plot.y_major_label_rotation() << std::endl;
  os << "y_major_tick_color " << plot.y_major_tick_color() << std::endl;
  os << "y_major_tick_length  " << plot.y_major_tick_length() << std::endl;
  os << "y_major_tick_width  " << plot.y_major_tick_width() << std::endl;
  os << "y_minor_grid_on " << plot.y_minor_grid_on() << std::endl;
  os << "y_minor_grid_color  " << plot.y_minor_grid_color() << std::endl;
  os << "y_minor_grid_width " << plot.y_minor_grid_width() << std::endl;
  os << "y_minor_interval " << plot.y_minor_interval() << std::endl;
  //os << "y_minor_labels_on " << plot.y_minor_labels_on() << std::endl; // Not implemented yet.
  os << "y_minor_tick_color " << plot.y_minor_tick_color() << std::endl;
  os << "y_minor_tick_length " << plot.y_minor_tick_length() << std::endl;
  os << "y_minor_tick_width " << plot.y_minor_tick_width() << std::endl;
  os << "y_range() " << plot.y_range() << std::endl;
  os << "y_num_minor_ticks " << std::endl;
  os << "y_ticks_left_on " << plot.y_ticks_left_on() << std::endl;
  os << "y_ticks_right_on " << plot.y_ticks_right_on() << std::endl;
  os << "y_ticks_on_window_or_axis " << l_or_r(plot.y_ticks_on_window_or_axis()) << std::endl;
  os << "y_max " << plot.y_max() << std::endl;
  os << "y_min " << plot.y_min() << std::endl;
  os << "y_values_on "  << plot.y_values_on() << std::endl;
  os << "y_plusminus_on " << plot.y_plusminus_on() << std::endl;
  os << "y_plusminus_color " << plot.y_plusminus_color() << std::endl;
  os << "x_addlimits_on " << plot.x_addlimits_on() << std::endl;
  os << "x_addlimits_color " << plot.x_addlimits_color() << std::endl;
  os << "y_df_on " << plot.y_df_on() << std::endl;
  os << "y_df_color " << plot.y_df_color() << std::endl;
  os << "y_prefix \"" << plot.y_prefix()  << '"' << std::endl;
  os << "y_separator \"" << plot.y_separator()  << '"' << std::endl;
  os << "y_suffix \"" << plot.y_suffix()  << '"' << std::endl;
  os << "confidence alpha " << plot.confidence() << std::endl;
  os << "data lines width " << plot.data_lines_width() << std::endl;
  // Infinite, NaN and out of plot window markers.
  os << "NaN limit points stroke color " << plot.nan_limit_color() << std::endl;
  os << "NaN limit points fill color " << plot.nan_limit_fill_color() << std::endl;
  os << "NaN limit points size " << plot.nan_limit_size() << std::endl;
  os << "+infinity limit points stroke color " << plot.plus_inf_limit_color() << std::endl;
  os << "+infinity limit points fill color " << plot.plus_inf_limit_fill_color() << std::endl;
  os << "+infinity limit points size " << plot.plus_inf_limit_size() << std::endl;
  os << "-infinity limit points stroke color " << plot.minus_inf_limit_color() << std::endl;
  os << "-infinity limit points fill color " << plot.minus_inf_limit_fill_color() << std::endl;
  os << "-infinity limit points size " << plot.minus_inf_limit_size() << std::endl;
  os.flags(iostate); // Restore saved format flags.
  os.precision(precision);  // Restore precision.
} // void show_2d_plot_settings(svg_2d_plot& plot, std::ostream& os = std::cout)

} // svg
} // boost

#endif // BOOST_SVG_SHOW_2D_SETTINGS_HPP
