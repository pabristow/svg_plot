/*! \file test_svg_xml.cpp
    \brief Test actual SVG in XML by comparing with expected XML.
*/

// Copyright Paul A. Bristow 2007, 2009, 2013, 2016, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_LIB_DIAGNOSTIC "on"
#define BOOST_TEST_MAIN
// Note define BOOST_TEST_MAIN must come BEFORE this include.
#include <boost/test/unit_test.hpp>

#include <boost/test/tools/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <boost/svg_plot/svg_color.hpp>
using namespace boost::svg;

#include <iostream>
  using std::cout;

#include <map>
  using std::multimap;
#include <cmath>
  using ::sin;

BOOST_AUTO_TEST_CASE(test_styles)
{
  BOOST_CHECK_EQUAL(aliceblue, 0); // Is 1st array element, index zero.
  BOOST_CHECK_EQUAL(yellowgreen, 146); // Last array element, index 146.
  BOOST_CHECK_EQUAL(blank, 147); // Is last NotAColor array element, index 146.
}

BOOST_AUTO_TEST_CASE(test_svg_xml)
{
  using namespace boost::svg;
  svg_2d_plot my_2d_plot;

   my_2d_plot
    .background_border_color(black)
    .title("Y axis label experiment")
    .x_label_on(true)
    .y_label_on(true)
    .x_major_labels_side(bottom_side)
    .y_major_labels_side(left_side)
    .plot_window_on(true)
   ;
   if (true)
  { // (Re-)Write the expected pattern file.
    my_2d_plot.write("test_svg_xml.svg");

  /*
   output_test_stream( const_string pattern_file_name, bool match_or_save  = true,
                                        bool  text_or_binary    = true );
    result_type     is_empty( bool flush_stream = true );
    result_type     check_length( std::size_t length, bool flush_stream = true );
    result_type     is_equal( const_string arg_, bool flush_stream = true );
    result_type     match_pattern( bool flush_stream = true );
  */

    // Writing to stream is different from writing file - because the latter adds filename?
    //  void write(const std::string& filename)
    //  void write(std::ostream& s_out)
    //     if (filename_ != "")
    //{ // Example: <!-- File demo_1d_plot.svg -->
    //  s_out << "<!-- File " << filename_ << " --> "<< std::endl;
    //}

    // So output_test_stream seems to behave like a file?
  }

 // Check
  output_test_stream output("test_svg_xml.svg");

  my_2d_plot.write(output);

  BOOST_CHECK(output.match_pattern());
} // BOOST_AUTO_TEST_CASE(test_svg_xml)
