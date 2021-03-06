#ifndef SINETEK_RTSX_UTIL_H
#define SINETEK_RTSX_UTIL_H

#include <os/log.h>

#include <IOKit/IOLib.h> // IOSleep
#include <IOKit/IOMemoryDescriptor.h> // IOMemoryDescriptor

#ifndef UTL_THIS_CLASS
#error UTL_THIS_CLASS must be defined before including this file (i.e.: #define UTL_THIS_CLASS "SDDisk::").
#endif

#define UTL_ERR(fmt, ...) \
os_log_fault(OS_LOG_DEFAULT, "rtsx: %12s%-22s: " fmt "\n", UTL_THIS_CLASS, __func__, ##__VA_ARGS__); \

#define UTL_LOG(fmt, ...) \
os_log(OS_LOG_DEFAULT, "rtsx: %12s%-22s: " fmt "\n", UTL_THIS_CLASS, __func__, ##__VA_ARGS__); \

#define UTL_CHK_SUCCESS(expr) \
({ \
	auto sinetek_rtsx_utl_chk_success = expr; \
	if (sinetek_rtsx_utl_chk_success != kIOReturnSuccess) { \
		UTL_ERR("%s returns error %d", #expr, sinetek_rtsx_utl_chk_success); \
	} \
	sinetek_rtsx_utl_chk_success; \
})

#if DEBUG
//#define UTL_DEBUG_LEVEL 2
#ifndef UTL_DEBUG_LEVEL
#	define UTL_DEBUG_LEVEL 1 // only 0-level messages
#endif
#define UTL_DEBUG(lvl, fmt, ...) \
do { \
	if (lvl < UTL_DEBUG_LEVEL) { \
		os_log(OS_LOG_DEFAULT, "rtsx: %12s%-22s: " fmt "\n", UTL_THIS_CLASS, __func__, ##__VA_ARGS__); \
		IOSleep(50); /* Wait for log to appear... */ \
	} \
} while (0)
#else // DEBUG
#define UTL_DEBUG(lvl, fmt, ...) do { } while (0)
#endif // DEBUG

#if DEBUG || SDMMC_DEBUG
static inline const char *mmcCmd2str(uint16_t mmcCmd) {
	switch (mmcCmd) {
		case 0: return "MMC_GO_IDLE_STATE";
		case 1: return "MMC_SEND_OP_COND";
		case 2: return "MMC_ALL_SEND_CID";
		case 3: return "*_RELATIVE_ADDR";
		case 6: return "*_SWITCH*/SD_APP_BUS_WIDTH";
		case 7: return "MMC_SELECT_CARD";
		case 8: return "MMC_SEND_EXT_CSD/SD_SEND_IF_COND";
		case 9: return "MMC_SEND_CSD";
		case 12: return "MMC_STOP_TRANSMISSION";
		case 13: return "MMC_SEND_STATUS";
		case 16: return "MMC_SET_BLOCKLEN";
		case 17: return "MMC_READ_BLOCK_SINGLE";
		case 18: return "MMC_READ_BLOCK_MULTIPLE";
		case 23: return "MMC_SET_BLOCK_COUNT";
		case 24: return "MMC_WRITE_BLOCK_SINGLE";
		case 25: return "MMC_WRITE_BLOCK_MULTIPLE";
		case 55: return "MMC_APP_CMD";

		case 41: return "SD_APP_OP_COND";
		case 51: return "SD_APP_SEND_SCR";
		default: return "?";
	}
}
#endif // DEBUG || SDMMC_DEBUG

#define UTL_CHK_PTR(ptr, ret) do { \
	if (!(ptr)) { \
		UTL_ERR("null pointer (%s) found!!!", #ptr); \
		return ret; \
	} \
} while (0)

#if RTSX_USE_IOMALLOC
#define UTL_MALLOC(TYPE) (TYPE *) UTLMalloc(#TYPE, sizeof(TYPE))
static inline void *UTLMalloc(const char *type, size_t sz) {
	UTL_DEBUG(0, "Allocating a %s (%u bytes).", type, (unsigned) sz);
	return IOMalloc(sz);
}
#define UTL_FREE(ptr, TYPE) \
do { \
	UTL_DEBUG(0, "Freeing a %s (%u bytes).", #TYPE, (unsigned) sizeof(TYPE)); \
	IOFree(ptr, sizeof(TYPE)); \
} while (0)
#else // RTSX_USE_IOMALLOC
#define UTL_MALLOC(TYPE) new TYPE
#define UTL_FREE(ptr, TYPE) do { \
	delete ptr; \
	ptr = nullptr; \
} while (0)
#endif // RTSX_USE_IOMALLOC

static inline AbsoluteTime timo2AbsoluteTimeDeadline(int timo) {
	extern int hz;
	uint64_t nsDelay = (uint64_t) timo / hz * 1000000000LL;
	AbsoluteTime absInterval, deadline;
	nanoseconds_to_absolutetime(nsDelay, &absInterval);
	clock_absolutetime_interval_to_deadline(absInterval, &deadline);
	return deadline;
}

/// Only valid between prepare() and complete()
static inline size_t bufferNSegments(IOMemoryDescriptor *md) {
	size_t ret = 0;
#if DEBUG
	uint64_t len = md->getLength();

	uint64_t thisOffset = 0;
	while (thisOffset < len) {
		ret++;
		IOByteCount thisSegLen;
		if (!md->getPhysicalSegment(thisOffset, &thisSegLen, kIOMemoryMapperNone)) return 0;
		thisOffset += thisSegLen;
	}
#endif // DEBUG
	return ret;
}

/// Only valid between prepare() and complete()
static inline void dumpBuffer(IOMemoryDescriptor *md) {
#if DEBUG
	auto len = md->getLength();

	uint64_t thisOffset = 0;
	while (thisOffset < len) {
		uint64_t thisSegLen;
		auto addr = md->getPhysicalSegment(thisOffset, &thisSegLen, kIOMemoryMapperNone | md->getDirection());
		UTL_DEBUG(0, " - Segment: Addr: 0x%016llx Len: %llu", addr, thisSegLen);
		if (!addr) return;
		thisOffset += thisSegLen;
	}
#endif // DEBUG
}

#define RTSX_PTR_FMT "0x%08x%08x"
#define RTSX_PTR_FMT_VAR(ptr) (uint32_t) ((uintptr_t) ptr >> 32), (uint32_t) (uintptr_t) ptr

#endif /* SINETEK_RTSX_UTIL_H */
