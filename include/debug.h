#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DFT_TAG "[WIFI-EMI]"

#define DBG_LOG_LOUD 4
#define DBG_LOG_DBG  3
#define DBG_LOG_INFO 2
#define DBG_LOG_WARN 1
#define DBG_LOG_ERR  0

extern UINT32 gDbgLvl;

#define RED(_text)  "\033[1;31m"_text"\033[0m"
#define GRN(_text)  "\033[1;32m"_text"\033[0m"
#define YLW(_text)  "\033[1;33m"_text"\033[0m"
#define BLUE(_text) "\033[1;36m"_text"\033[0m"


#define DBG_LOUD_FUNC(fmt, arg...) \
do { \
	if (gDbgLvl >= DBG_LOG_LOUD) \
		printk(GRN(DFT_TAG"[L]%s:"  fmt), __func__, ##arg); \
} while (0)

#define DBG_DBG_FUNC(fmt, arg...) \
do { \
	if (gDbgLvl >= DBG_LOG_DBG) \
		printk(YLW(DFT_TAG"[D]%s:"  fmt), __func__, ##arg); \
} while (0)

#define DBG_INFO_FUNC(fmt, arg...) \
do { \
	if (gDbgLvl >= DBG_LOG_INFO) \
		printk(YLW(DFT_TAG"[I]%s:"  fmt), __func__, ##arg); \
} while (0)

#define DBG_WARN_FUNC(fmt, arg...) \
do { \
	if (gDbgLvl >= DBG_LOG_WARN) \
		printk(RED(DFT_TAG"[W]%s(%d):"  fmt), __func__, __LINE__, ##arg); \
} while (0)

#define DBG_ERR_FUNC(fmt, arg...) \
do { \
	if (gDbgLvl >= DBG_LOG_ERR) \
		printk(RED(DFT_TAG"[E]%s(%d):"  fmt), __func__, __LINE__, ##arg); \
} while (0)

#endif /* __DEBUG_H__ */
