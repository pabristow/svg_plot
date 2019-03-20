#          Copyright Michael Tesch 2019.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

foreach (BASE 
    1d_full_layout
    1d_simple_style
    1d_x_external
    1d_x_grid
    2d_area_fill
    2d_bezier
    2d_full
    2d_limit
    2d_simple
    2d_y_grid
    auto_1d_containers
    auto_1d_plot
    auto_2d_plot
    auto_boxplot
    boxplot_full
    boxplot_simple
    container_minmax
    demo_1d_autoscaling
    demo_1d_axis_scaling
    demo_1d_containers
    demo_1d_heat_flow_data
    demo_1d_limits
    demo_1d_meas
    demo_1d_plot
    demo_1d_simple
    demo_1d_tick_values
    demo_1d_uncertainty
    demo_1d_values
    demo_1d_vector
    demo_1d_x_external
    demo_2d_area_fill
    demo_2d_autoscaling
    demo_2d_bad_function
    demo_2d_bars
    demo_2d_fonts
    demo_2d_histogram
    demo_2d_limits
    demo_2d_lines
    demo_2d_plot
    demo_2d_simple
    demo_2d_tick_values
    demo_2d_uncertainty
    demo_2d_values
    demo_2d_weather
    demo_annotation
    demo_boxplot
    demo_boxplot_full
    demo_boxplot_simple
    demo_color
    #demo_FP_compare
    demo_functions_boxplot
    demo_Hoaglin
    demo_point_markers
    demo_rounds
    demo_svg
    demo_svg_tspan
    svg_colors
    svg_test_boxplot
    svg_test
    )

  add_executable (${BASE} ${BASE}.cpp)
  add_dependencies (${BASE} quanX)
  add_test (${BASE} ${BASE})

endforeach()