svg_plot
========

Plot data in SVG format using C++ library.

With just a few lines of C++ code, the SVG_plot C++ library allows plotting of data (held in STL containers like vector and map), to be output as Scalable Vector Graphic files that can be viewed by all modern browsers.

1D, 2D and Boxplots can be produced, and there are a myriad of options to control appearance.

The plots are very high quality and can be expanded without loss of quality (no pixelation!) but are tiny, (compared to a Microsoft Excel plot, for example) and are easily and highly compressible, making them suitable for real-time transmission and update.

The SVG_plot project was originally written by Jake Voytko in 2007 as a Boost-sponsored Google Summer of Code project in 2007. It has been maintained and enhanced since then in Boost Sandbox, but is judged unsuitable for a Boost Library, so now being made more widely available here.

Version 2 adds representation of uncertainty estimates as numbers and zones.

[Boost License](http://www.boost.org/LICENSE_1_0.txt).

Documentation
=============

Online documentation can be found here: https://pabristow.github.io/doc/html/index.html

Building the demos & examples using CMake
=========================================

```
cmake . -Bbuild
cmake --build build
```

Some example images
===================
<a href="./example/1d_full_layout.cpp">./examples/1d_full_layout.cpp

<img src="./doc/images/1d_full_layout.svg"/></a>

<a href="./example/1d_x_grid.cpp">./examples/1d_x_grid.cpp

<img src="./doc/images/1d_x_grid.svg"/></a>

<a href="./example/2d_bezier.cpp">./examples/2d_bezier.cpp

<img src="./doc/images/2d_bezier.svg"/></a>

<a href="./example/2d_full.cpp">./examples/2d_full.cpp

<img src="./doc/images/2d_full.svg"/></a>

<a href="./example/2d_limit.cpp">./examples/2d_limit.cpp

<img src="./doc/images/2d_limit.svg"/></a>

<a href="./example/2d_simple.cpp">./examples/2d_simple.cpp

<img src="./doc/images/2d_simple.svg"/></a>

<a href="./example/2d_y_grid.cpp">./examples/2d_y_grid.cpp

<img src="./doc/images/2d_y_grid.svg"/></a>

<a href="./example/auto_1d_containers.cpp">./examples/auto_1d_containers.cpp

<img src="./doc/images/auto_1d_containers.svg"/></a>

<a href="./example/auto_1d_plot.cpp">./examples/auto_1d_plot.cpp

<img src="./doc/images/auto_1d_plot.svg"/></a>

<a href="./example/auto_2d_plot.cpp">./examples/auto_2d_plot.cpp

<img src="./doc/images/auto_2d_plot.svg"/></a>

<a href="./example/auto_boxplot.cpp">./examples/auto_boxplot.cpp

<img src="./doc/images/auto_boxplot.svg"/></a>

<a href="./example/boxplot_simple.cpp">./examples/boxplot_simple.cpp

<img src="./doc/images/boxplot_simple.svg"/></a>

<a href="./example/demo_1d_autoscaling.cpp">./examples/demo_1d_autoscaling.cpp

<img src="./doc/images/demo_1d_autoscaling.svg"/></a>

<a href="./example/demo_1d_limits.cpp">./examples/demo_1d_limits.cpp

<img src="./doc/images/demo_1d_limits.svg"/></a>

<a href="./example/demo_1d_plot.cpp">./examples/demo_1d_plot.cpp

<img src="./doc/images/demo_1d_plot.svg"/></a>

<a href="./example/demo_1d_simple.cpp">./examples/demo_1d_simple.cpp

<img src="./doc/images/demo_1d_simple.svg"/></a>

<a href="./example/demo_1d_uncertainty.cpp">./examples/demo_1d_uncertainty.cpp

<img src="./doc/images/demo_1d_uncertainty.svg"/></a>

<a href="./example/demo_1d_values.cpp">./examples/demo_1d_values.cpp

<img src="./doc/images/demo_1d_values.svg"/></a>

<a href="./example/demo_1d_vector.cpp">./examples/demo_1d_vector.cpp

<img src="./doc/images/demo_1d_vector.svg"/></a>

<a href="./example/demo_1d_x_external.cpp">./examples/demo_1d_x_external.cpp

<img src="./doc/images/demo_1d_x_external.svg"/></a>

<a href="./example/demo_2d_fonts.cpp">./examples/demo_2d_fonts.cpp

<img src="./doc/images/demo_2d_fonts.svg"/></a>

<a href="./example/demo_2d_limits.cpp">./examples/demo_2d_limits.cpp

<img src="./doc/images/demo_2d_limits.svg"/></a>

<a href="./example/demo_2d_lines.cpp">./examples/demo_2d_lines.cpp

<img src="./doc/images/demo_2d_lines.svg"/></a>

<a href="./example/demo_2d_simple.cpp">./examples/demo_2d_simple.cpp

<img src="./doc/images/demo_2d_simple.svg"/></a>

<a href="./example/demo_2d_tick_values.cpp">./examples/demo_2d_tick_values.cpp

<img src="./doc/images/demo_2d_tick_values.svg"/></a>

<a href="./example/demo_2d_uncertainty.cpp">./examples/demo_2d_uncertainty.cpp

<img src="./doc/images/demo_2d_uncertainty.svg"/></a>

<a href="./example/demo_2d_values.cpp">./examples/demo_2d_values.cpp

<img src="./doc/images/demo_2d_values.svg"/></a>

<a href="./example/demo_boxplot.cpp">./examples/demo_boxplot.cpp

<img src="./doc/images/demo_boxplot.svg"/></a>

<a href="./example/demo_color.cpp">./examples/demo_color.cpp

<img src="./doc/images/demo_color.svg"/></a>

<a href="./example/demo_Hoaglin.cpp">./examples/demo_Hoaglin.cpp

<img src="./doc/images/demo_Hoaglin.svg"/></a>

<a href="./example/demo_point_markers.cpp">./examples/demo_point_markers.cpp

<img src="./doc/images/demo_point_markers.svg"/></a>

<a href="./example/demo_svg.cpp">./examples/demo_svg.cpp

<img src="./doc/images/demo_svg.svg"/></a>

<a href="./example/demo_svg_tspan.cpp">./examples/demo_svg_tspan.cpp

<img src="./doc/images/demo_svg_tspan.svg"/></a>

<a href="./example/svg_colors.cpp">./examples/svg_colors.cpp

<img src="./doc/images/svg_colors.svg"/></a>

<a href="./example/svg_test_boxplot.cpp">./examples/svg_test_boxplot.cpp

<img src="./doc/images/svg_test_boxplot.svg"/></a>

 
