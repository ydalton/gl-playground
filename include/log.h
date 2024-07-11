#ifndef __LOG_H_
#define __LOG_H_

#include <stdio.h>

#define LOG(...)			printf(__VA_ARGS__)
#define WARN(...)			LOG("warning: " __VA_ARGS__)

#endif
