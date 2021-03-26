readme.txt

SVG_plot - a data visualization library for C++
===============================================

This library aims to help the user to easily plot data
that is stored in STL containers. You can display data
on one-dimensional, two-dimensional plots, boxplots and histograms.

The plot is written to produce a Scalable Vector Graphic image in XML
(that can also be easily converted to other graph types,
for example, PNG http://en.wikipedia/org/wiki/Portable_Network_Graphics).

SVG graphics are tiny but very high quality
and do not change resolution when expanded or contracted.

The library is a collection of C++ header files
in Boost layout (do not try to build a library).
You will need Boost http:/www.boost.org libraries installed too
(but those that are used by SVG_plot are all header-only too).

To use, you must download the headers in svg_plot.7z,
and also quan.7z for handling uncertainty,
and unzip both to a location of your choice.
For example, you might unzip to folder or partition z:
so header files are in folder z:/svg_plot.

You must ensure that the #included files are visible to your build system.
For Microsoft Visual Studio, this means adding the location
z:/svg_plot to your VC++ Directories, Include files list,
or by adding to Additional Include Directories.

To use, you must get your data values
(and optionally uncertainty information)
into a STL container like vector (for 1D) or map (for 2D).

  vector<double> a;  // Container filled with your data.

  svg_1d_plot my_plot; // Construct a plot.

  // Set title and legend, and X-axis range and label.
           .title("Times")
           .x_label("time (sec)")
           .x_range(0, 10);
  // There are hundreds of other possible options here!

  my_plot.plot(a, "A").stroke_color(blue); // Add your data to the plot.

  my_plot.write("./1d_simple.svg"); // Finally write the plot to a file.

You can then view the SVG plot using any modern browser
(with Microsoft Internet Explorer <= 8, you will need to install an Adobe plugin)

For documentation, examples, samples and tests, download svg_plot_libs.7z

If all else fails, see the extensive Doxygen, PDF and HTML manuals for details.
svg_plot_libs/doc/doxygen/index.html, svg_plot_libs/doc/svg_lot.pdf and  svg_plot_libs/doc/index.html.

Paul A. Bristow
Version 3, 2021

# Copyright Paul A. Bristow 2013, 2014, 2021

# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)








