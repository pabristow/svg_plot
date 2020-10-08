/*! 
\brief 
Demonstrates actual length of text displayed as SVG.

Shows warning from too much compression using text_length.

And also shows use of text_length can undercompress to space out glyphs to become unreadable.

Font Support for Unicode Characters

https://www.fileformat.info/info/unicode/font/index.htm

Shows aspect ratio for font size 10  varys from 0.55 to 0.4
no of chars that fit the 1000 image varies from 

120 letter M units (em width) - the widest
340 letter i - the narrowest
random letters from 180 to 240
aspect ratios
lucida sans unicode 0.49
verdana 0.48
arial 0.42
Tme New Roman 0.4

in svg_style.hpp 
  static const fp_type aspect_ratio = 0.6;  //!< aspect_ratio is a guess at average height to width of font.

  Examples of using function plot.title_text_length(1000); so squeeze or expand title.
*/

// svg_text_width_height.cpp

// Copyright Paul A. Bristow 2018, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/svg_plot/svg_style.hpp>
#include <boost/svg_plot/svg_2d_plot.hpp>
// using namespace boost::svg;

#include <boost/svg_plot/show_2d_settings.hpp>

#include <iostream>
#include <string>

int main()
{
  using namespace boost::svg;
  // Very convenient to allow easy access to colors, data point marker shapes and other svg_plot items.

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.

    svg_2d_plot my_2d_plot; // Construct a plot with all the default constructor values.

    // Containers to hold some data.
    std::map<const double, double> my_data_0;
    my_data_0[0.0] = 0.0;
    my_data_0[10.0] = 10.0;

    std::string a_title(340, 'i'); // 

    my_2d_plot // Nearly all default settings.
      .size(1000, 200)
      .title("&#x3A9;") // 116 l fill 1000 exactly 465 ll
       .title("&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;") // 116 &#x3A9; fill 1000 exactly 465 &#x3A9;&#x3A9;
//     .title("&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;&#x3A9;") // 116 l fill 1000 exactly 465 ll
    //  .title("llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll") // 116 l fill 1000 exactly 465 ll
     // .title("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM") // 116 M fill 1000 exactly 465 mm
      // So for font width = 20 (type normal) width of an M is 1000/
 //     .title("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM") // 100 M width 400 mm
 //     .title("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii") // 116 i width 160 mm      .title_on(true)
 //     .title(a_title) // std::string a_title(116, 'W');  Fills 1000 completely like 'M'
 //     .title("The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog.The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog.")
 //      181 'mixed' chars 10 font size almost fills (435 mm) 1000 width so each char is 1000 / 181 = 5.5 svg unit aspect ratio =  0.55
      // and triggers warning message:
//         "width 1092 (SVG units) may overflow plot image!
 //        (Reduce font size from 10, or number of characters from 182, or increase image size from 1000)."
 //     .title("The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog.The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog. The quick b")
        // Fills exactly char count 194 = 470 mm 1000/194 = 5.15, aspect ratio = 10/5.16 = 0.516
        // so a string of 194 * 0.516 = 1000 
       // .title(a_title) // std::string a_title(340, 'i'); // will exactly, char count 340
      //.title_font_family("Lucida sans Unicode")
      //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is")
      // default Lucida sans Unicode 204 chars fill 1000 pixels, so aspect ratio = 1000 /204 = 4.9 div 10 = 0.49

    //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is time time for all good men to come to the")
     //But 204 chars Times New Roman is only 370 mm long
    //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid")
    //// 249 chars fit.  So aspect ratio = 1000/250 = 4, so for 10 point aspect ratio = 0.4
    //.title_font_family("Arial")
    //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men")
//  Arial 10 point 240 chars so aspect ratio = 1000/240 = 0.42

    //.title_font_family("verdana")
   //  verdana 10 point 208 chars so aspect ratio = 1/10 * 1000/208 = 0.48
     // Example of using all the text styles:
 //     .title("Now is the time for all good men to come to the aid of the party.")
      .title_font_size(10)
      //.title_font_family("Arial")
      .title_font_family("Times new roman")
  //    .title_font_style("italic")
   //   .title_font_weight("bold")
   //   .title_font_stretch("narrower") // May have no effect here.
     // .title_font_decoration("underline")
      .title_text_length(1500) // Force into an arbitrary chosen fixed width 1500 = more than full width of image so spaced out.
      // and overflowing chars are lost at both end.
      .title_text_length(1000) // Force into an arbitrary chosen fixed width 1000 = full width of image.
     // .title_text_length(800) // Force into an arbitrary chosen fixed width very tight so letter M just touching.
      // This is very long title test:
      // .title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the ")
      // if no text_length() then get warning
      // font size = 5 is far too small and stretch to fit
      // font size = 20 makes all the letters on top of each other.
      // font size = 12 is just readable with text_length 1000.
      // font size = 13 is too close and some glyphs collide with text_length 1000.
      /*  Error message example:
      title style text_style(13, "Arial", "italic", "bold", "", "underline", 1000), text_length = 1000
      Title "Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the "
        estimated width 1630.2 (SVG units) may overflow plot image or or over-compress text!
        (Reduce font size from 13, or number of characters from 209, or increase image size from 1000).
      */
      // Squash Factor 1.6 chosen on this basis, but might be different for other fonts?
    
      .plot(my_data_0)

      ;
    // Show just couple of text styles
      //std::cout << "title family " << my_2d_plot.title_font_family() << ", size " << my_2d_plot.title_font_size() << std::endl;
      // title family Lucida Sans Unicode, size 10

    std::cout << "title style " << my_2d_plot.title_style()  // title style text_style(12, "Arial", "italic", "bold", "narrower", "underline", 1000)
      << ", text_length = " << my_2d_plot.title_text_length()  // text_length = 1000 
      << std::endl;

    // Title text "Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the "
    //  with an estimated width 1630.2 (SVG units) may overflow plot space 1000 or over-compress text with compression ratio 1.6302.
    //  Reduce font size from 13, or number of characters from 209, or increase image size from 1000?.
    my_2d_plot.write("./svg_text_width_height.svg"); // Plot output to file.
    // Output contains for the title:
    //<g id="title">
    //  <text x="500" y="18" text-anchor="middle" font-size="12" 
    //font-family="Arial" font-style="italic" font-weight="bold" font-stretch="narrower" 
    // text-decoration="underline" 
    // textLength="1e+03">    <<<<<<<<  note how is forced to use the exact (estimated) width.
    //  Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the 
    //  </text>
    //</g>
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()
