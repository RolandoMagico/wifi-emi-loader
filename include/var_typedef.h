#ifndef __VAR_TYPEDEF_H__
#define __VAR_TYPEDEF_H__

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define FUNC_STATUS_SUCCESS            0
#define FUNC_STATUS_FAILURE            -1
#define FUNC_STATUS_NOT_SUPPORT        1

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned long long ULONGLONG;

typedef signed char CHAR;
typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef signed long long INT64;
typedef signed short SHORT;
typedef signed int INT;
typedef signed long LONG;
typedef signed long long LONGLONG;

typedef unsigned char BOOLEAN;
typedef void VOID;

typedef unsigned char *PUINT8;
typedef unsigned short *PUINT16;
typedef unsigned int *PUINT32;
typedef unsigned long long *PUINT64;
typedef signed int *PINT32;
typedef signed long long *PINT64;
typedef signed char *PCHAR;
typedef void *PVOID;

#endif /* __VAR_TYPEDEF_H__ */
