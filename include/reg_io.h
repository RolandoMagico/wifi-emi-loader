#ifndef __REG_IO_H__
#define __REG_IO_H__

#ifndef VXWORKS
#include <linux/io.h>
#include "var_typedef.h"

// ---------------------------------------------------------------------------
//  Register Manipulations
// ---------------------------------------------------------------------------
#if 1
#define IO_R_32(reg) \
	readl((VOID *)reg)

#define IO_W_32(reg, val) \
	writel(val, (VOID *)reg)

#define IO_R_16(reg) \
	readw((VOID *)reg)

#define IO_W_16(reg, val) \
	writew(val, (VOID *)reg)

#define IO_R_8(reg) \
	readb((VOID *)reg)

#define IO_W_8(reg, val) \
	writeb(val, (VOID *)reg)
#else
#define IO_R_32(reg) \
    (*(volatile UINT32 * const)(reg))

#define IO_W_32(reg, val) \
    (*(volatile UINT32 * const)(reg)) = (val)

#define IO_R_16(reg) \
    (*(volatile UINT16 * const)(reg))

#define IO_W_16(reg, val) \
    (*(volatile UINT16 * const)(reg)) = (val)

#define IO_R_8(reg) \
    (*(volatile UINT8 * const)(reg))

#define IO_W_8(reg, val) \
    (*(volatile UINT8 * const)(reg)) = (val)
#endif
#define IO_CLR_BIT_32(reg, val) IO_W_32(reg, (IO_R_32(reg) & (~(val))))
#define IO_SET_BIT_32(reg, val) IO_W_32(reg, (IO_R_32(reg) | (val)))
#endif
#ifdef VXWORKS
//TODO:Need to check Endian?
#define IO_R_32(reg) \
	(*((UINT32 *)(reg)))

#define IO_W_32(reg, val) \
	(*((UINT32 *)(reg)) = (val))

#define IO_CLR_BIT_32(reg, val) IO_W_32(reg, (IO_R_32(reg) & (~(val))))
#define IO_SET_BIT_32(reg, val) IO_W_32(reg, (IO_R_32(reg) | (val)))
#endif

#endif /* __REG_IO_H__ */
