/* You should not use pre-compiled header because this file is recursive. */

#pragma once
using namespace std;

#include <iostream>
#include <map>
#include <vector>

// C compatible headers
#include <cstdio>
#include <cstring>
#include <cmath>

#include <gtkmm-2.4/gtkmm.h>


#include "./cyg_def.hpp"
#include "./cyg_class.hpp"
#include "./cyg_function.hpp"

#ifdef DEBUG
extern char *__unused_tmpval;
#define UNUSED(a,...)	((void)(0 && printf(__unused_tmpval,a,__VA_ARGS__+0)))
#else
#define UNUSED(a,...)
#endif

