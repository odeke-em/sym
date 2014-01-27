#ifndef _ERRORS_H
#define _ERRORS_H
  #include <stdio.h>

  typedef enum {
    TypeError, ValueError, IndexError, SyntaxError, BufferOverFlow,
    AssertionError, NullPointerException, IndexOutOfBoundsException,
    ZeroDivisionException, CorruptedDataException
  } Exception;
  
  #define raiseWarning(fmt, args){\
    fprintf(stderr,"%s::%s\033[91m ",__FILE__, __func__);\
    fprintf(stderr, fmt, args);\
    fprintf(stderr, "\033[00m on line %d\n", __LINE__);\
  }

  #define throwException(errCode,errMsg){\
    if (errMsg != NULL){\
      fprintf(stderr, "%s\n", #errMsg);\
    }\
    raiseWarning(#errCode, "");\
    exit(-1);\
  }
 
  #define assert(validExpression){\
    if (! validExpression)\
      raiseError(validExpression);\
  }

  #define raiseError(args) {\
    raiseWarning("Traceback most recent call: ", args);\
    exit(-2);\
  }

  #define raiseExceptionIfNull(expression){\
    if (! expression)\
      throwException(NullPointerException, expression);\
  }
#endif
