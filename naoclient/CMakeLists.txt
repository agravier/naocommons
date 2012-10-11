cmake_minimum_required(VERSION 2.8)
project(naoclient)

find_package(qibuild)

# build the RTP server
add_subdirectory(rtsp_server)

# NAOqi library source files
set(_srcs 
    fgrab.cpp
    fgrab.h
    naoclient.cpp 
    naoclient.h 
    naoqi_module_init.cpp)

# Build the library in the subfolder lib/naoqi
qi_create_lib(naoclient SHARED ${_srcs} SUBFOLDER naoqi)
qi_use_lib(naoclient ALCOMMON ALVISION BOOST) # rtsp_server)
qi_stage_lib(naoclient)

# Create a executable named start_naoclient using start_naoclient.cpp
qi_create_bin(start_naoclient start_naoclient.cpp)
qi_use_lib(start_naoclient ALCOMMON)

# # Add a simple test:
# enable_testing()
# qi_create_test(test_naoclient "test.cpp")
