#pragma once
#ifdef HYPERSTREAM_LOG
#include <iostream>
#define $(value) std::cout << (#value) << (value) << std::endl
#endif