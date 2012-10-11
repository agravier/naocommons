include_directories(include ../UsageEnvironment/include ../groupsock/include)

qi_create_lib(BasicUsageEnvironment SRC BasicUsageEnvironment.cpp  BasicUsageEnvironment0.cpp BasicTaskScheduler0.cpp BasicTaskScheduler.cpp DelayQueue.cpp BasicHashTable.cpp include/BasicUsageEnvironment.hh SHARED)
qi_use_lib(BasicUsageEnvironment)
qi_stage_lib(BasicUsageEnvironment)

qi_install_header(include/BasicUsageEnvironment.hh)
