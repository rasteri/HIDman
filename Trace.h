
#ifndef _TRACE_H_
#define _TRACE_H_

#ifdef DEBUG
#include <stdio.h>

#define TRACE(x)  printf(x)
#define TRACE1(fmt, x)  printf(fmt, x)
#define TRACE2(fmt, x1, x2) printf(fmt, x1, x2)
#define TRACE3(fmt, x1, x2, x3) printf(fmt, x1, x2, x3)

#else
#define TRACE(x)  
#define TRACE1(fmt, x)  
#define TRACE2(fmt, x1, x2)
#define TRACE3(fmt, x1, x2, x3)

#endif

#endif

