include_directories(include ../groupsock/include)

qi_create_lib(UsageEnvironment SRC UsageEnvironment.cpp HashTable.cpp strDup.cpp SHARED)
qi_use_lib(UsageEnvironment)
qi_stage_lib(UsageEnvironment)

qi_install_header(include/UsageEnvironment.hh)
