#ifndef _DEBUG_H_
#define _DEBUG_H_


#define DEBUG           1


#if DEBUG
#   include "out.h"
#   define DBG(expr)    { \
                            Out::dbg() << __FILE__ ":"  << __LINE__ \
                                    << ": " #expr " == "; \
                            __typeof__(expr) _expr_val = (expr); \
                            Out::dbg() << HEX << _expr_val \
                                     << " (" << DEC << _expr_val << ")\n"; \
                        }
#   define ASSERT(expr) if (0 == (expr)) \
                        { \
                            Out::dbg() << __FILE__ ":" << __LINE__ \
                                    << ": FAILED ASSERT(" #expr ")\n"; \
                            kernel_stop(); \
                        }
#else
#   define DBG(expr)
#   define ASSERT(expr)
#endif

#endif // _DEBUG_H_
