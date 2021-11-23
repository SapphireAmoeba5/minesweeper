#pragma once
#include <iostream>

// Quickly enable to disable logging while building for performance reasons
#define LOGGING

#ifdef LOGGING

#define DEBUG_INFO(x)   std::cout<<"\033[32m[INFO] \033[0m"<<x<<std::endl
#define DEBUG_WARN(x)   std::cout<<"\033[93m[WARN] \033[0m"<<x<<std::endl
#define DEBUG_ERROR(x)  std::cout<<"\033[31m[ERROR] \033[0m"<<x<<std::endl

#else

#define DEBUG_INFO(x) 
#define DEBUG_WARN(x) 
#define DEBUG_ERROR(x)

#endif