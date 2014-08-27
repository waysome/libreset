#ifndef __MACROS_H__
#define __MACROS_H__

#define MAX(x,y)                    \
       ({ __typeof__ (x) _x = (x);  \
          __typeof__ (y) _y = (y);  \
          _x > _y ? _x : _y; })

#define MIN(x,y)                    \
       ({ __typeof__ (x) _x = (x);  \
          __typeof__ (y) _y = (y);  \
          _x < _y ? _x : _y; })

#endif //__MACROS_H__
