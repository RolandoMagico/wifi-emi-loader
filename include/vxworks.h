#ifndef __VXWORKS_H__
#define __VXWORKS_H__

#include "include/var_typedef.h"
typedef unsigned int size_t;
#define NULL ((void *)0)

struct platform_device {
	void *dev;
};
#define platform_set_drvdata(_handle, _data) 

#define devm_kzalloc(_handle, _size, _flags) \
	({void *_ptr = NULL; _ptr = malloc(_size); if (_ptr) memset(_ptr, 0, _size); _ptr;})
#define devm_kfree(_handle, _ptr) free(_ptr)
extern void *malloc(size_t);
extern void free(void *);
extern void *memset(void *, int, size_t);
extern void *memcpy(void *, const void *, size_t);
extern void *memmove(void *, const void *, size_t);

#define printf my_printf
#define printk my_printf
extern int my_printf(const char * fmt, ...);
extern int snprintf (char *, size_t, const char *, ...);

#ifndef ENOMEM
#define	ENOMEM		(-30005) /* no memory when allocate memory in Wi-Fi driver */
#endif

extern void sysUDelay(int us);
extern void sysMsDelay(int us);
#define mdelay sysMsDelay
#define udelay sysUDelay

#define __iomem

#define	_INSTRUCTION_CACHE 	0	/* Instruction Cache(s) */
#define	_DATA_CACHE			1	/* Data Cache(s) */

typedef enum				/* CACHE_TYPE */
{
    INSTRUCTION_CACHE = _INSTRUCTION_CACHE,
    DATA_CACHE        = _DATA_CACHE
} CACHE_TYPE;

int cacheInvalidate (CACHE_TYPE, void *, size_t);
int cacheFlush (CACHE_TYPE, void *, size_t);


//TODO:
#ifndef PHY_TO_K1
#define PHY_TO_K1(_addr) ((((UINT32)(_addr)) & 0x0FFFFFFF) | 0xA0000000)
#endif

static inline void *ioremap_nocache(UINT32 phyAddr, UINT32 size)
{
	//TODO:We just do ioremap for ddr address, not io register address
	if ((phyAddr & 0xF0000000) == 0x40000000)
	{
		cacheFlush(_DATA_CACHE, (void *)phyAddr, size);
		return (void *)PHY_TO_K1(phyAddr);
	}
	else
	{
		return (void *)phyAddr;
	}
}

#define iounmap(_addr)

#endif /* __VXWORKS_H__ */
