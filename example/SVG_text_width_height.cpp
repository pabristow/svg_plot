/*! 
\brief 
Demonstrates actual length of text displayed as SVG.

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


*/

// svg_text_width_height.cpp

// Copyright Paul A. Bristow 2018

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

    //     Legend Example 0
    svg_2d_plot my_2d_plot; // Construct a plot with all the default constructor values.

    // Containers to hold some data.
    std::map<const double, double> my_data_0;
    my_data_0[0.0] = 0.0;
    my_data_0[10.0] = 10.0;

    std::string a_title(340, 'i'); // 

    my_2d_plot // Nearly all default settings.
      .size(1000, 100)
 //     .title("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM") // 116 M fill 1000 exactly 465 mm
 //     .title("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM") // 100 M width 400 mm
 //     .title("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii") // 116 i width 160 mm      .title_on(true)
 //     .title(a_title) // std::string a_title(116, 'W');  Fills 1000 completely like 'M'
 //     .title("The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog.The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog.")
 //      181 'mixed' chars 10 font size almost fills (435 mm) 1000 width so each char is 1000 / 181 = 5.5 svg unit   aspect ratio =  0.55
      // and triggers warning message:
//         "width 1092 (SVG units) may overflow plot image!
 //        (Reduce font size from 10, or number of characters from 182, or increase image size from 1000)."
 //     .title("The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog.The Quick brown Fox jumped over the lazy dog. The Quick brown Fox jumped over the lazy dog. The quick b")
        // Fills exactly char count 194 = 470 mm 1000/194 = 5.15, aspect ratio = 10/5.16 = 0.516
        // so a string of 194 * 0.516 = 1000 
       // .title(a_title) // std::string a_title(340, 'i'); // will exactly, char count 340
      //.title_font_family("Lucida sans Unicode")
      //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is")
      // default Lucida sans Unicode 204 chars fill 1000 pixels. , so aspect ration = 1000 /204 = 4.9 div 10 = 0.49

    //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is time time for all good men to come to the")
     //But 204 chars Times New Roman is only 370 mm long
    //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid")
    //// 249 chars fit.  So aspect ratio = 1000/250 = 4, so for 10 point aspect ration = 0.4
    //.title_font_family("Arial")
    //.title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men")
//  Arial 10 point 240 chars so aspect ratio = 1000/240 = 0.42

    //.title_font_family("verdana")
   //  verdana 10 point 208 chars so aspect ratio = 1/10 * 1000/208 = 0.48
     // Example of using all the text styles:
      .title_font_size(12)
      .title_font_family("Arial")
      .title_font_style("italic")
      .title_font_weight("bold")
      .title_font_stretch("narrower") // May have no effect here.
      .title_font_decoration("underline")
      .title_text_length(1000) // Force into 900 width.
      .title("Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the ")
      // font size = 5 is far too small and stretch to fit
      // font size = 20 makes all the letters on top of each other.
      // font size = 12 is too close to be easily readable with text_length 900
      // if no text_length() then get warning
      // even if .title_text_length(1000)  then get message but fits and is readable.

      /*
      Title "Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the time for all good men to come to the aid of the party. Now is the "
        estimated width 1504.8 (SVG units) may overflow plot image!
        (Reduce font size from 12, or number of characters from 209, or increase image size from 1000).
      */
      
      .plot(my_data_0)
      ;
      //std::cout << "title family " << my_2d_plot.title_font_family() << ", size " << my_2d_plot.title_font_size() << std::endl;
      // title family Lucida Sans Unicode, size 10

    std::cout << "title style " << my_2d_plot.title_style() << ", text_length = " << my_2d_plot.title_text_length() << std::endl;
    // title style text_style(10, "Arial", "italic", "bold", "narrower", "underline", 900), text_length = 900

    my_2d_plot.write("./svg_ext_width_height.svg"); // Plot output to file.
    // Output contains for the title:
    // <text x="500" y="15" text-anchor="middle" font-size="10" font-family="Arial" font-style="italic" font-weight="bold" font-stretch="narrower" text-decoration="underline" textLength="900">Now ..
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()
