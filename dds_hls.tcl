open_project -reset proj_dds_hls

# Add design files
add_files dds_hls.cpp
add_files -tb test_dds_hls.cpp

# Set the top-level function
set_top dds_hls

# ########################################################
# Create a solution
open_solution -reset solution1 -flow_target vivado

# Define technology and clock rate
set_part {xczu48dr-ffvg1517-2-e}
create_clock -period 3.0
set_clock_uncertainty 0.2
config_rtl -reset all

csynth_design
export_design -format ip_catalog

exit
