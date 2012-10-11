include_directories(include ../UsageEnvironment/include)

qi_create_lib(groupsock SRC GroupsockHelper.cpp GroupEId.cpp inet.c Groupsock.cpp NetInterface.cpp NetAddress.cpp IOHandlers.cpp SHARED)
qi_use_lib(groupsock)
qi_stage_lib(groupsock)

qi_install_header(include/Groupsock.hh)
