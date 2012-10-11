# In case of compilation issues with some versions of GCC, try adding the -Wno-deprecated flag to CMAKE_CXX_FLAGS

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-O2 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64")
endif()

set(LIVE555_DIR live)

add_subdirectory(${LIVE555_DIR}/UsageEnvironment)

add_subdirectory(${LIVE555_DIR}/BasicUsageEnvironment)

add_subdirectory(${LIVE555_DIR}/liveMedia)

add_subdirectory(${LIVE555_DIR}/groupsock)

qi_create_bin(testOnDemandRTSPServer testOnDemandRTSPServer.cpp)
qi_use_lib(testOnDemandRTSPServer liveMedia groupsock BasicUsageEnvironment UsageEnvironment)

# qi_create_lib(rtsp_server rtsp_server.cpp)
# qi_use_lib(rtsp_server liveMedia groupsock BasicUsageEnvironment UsageEnvironment)
# qi_stage_lib(rtsp_server)

