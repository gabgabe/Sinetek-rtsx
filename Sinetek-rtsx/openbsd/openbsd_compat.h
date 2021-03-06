// This file aims to make code imported from OpenBSD compile under macOS with the minimum changes.

#ifndef SINETEK_RTSX_OPENBSD_OPENBSD_COMPAT_H
#define SINETEK_RTSX_OPENBSD_OPENBSD_COMPAT_H

// include necessary headers
#include <sys/cdefs.h> // __BEGIN_DECLS, __END_DECLS
__BEGIN_DECLS
#include <sys/systm.h> // MIN, EINVAL, ENOMEM, etc...
__END_DECLS

#include <IOKit/IOTimerEventSource.h>

// other headers from OpenBSD
#include "sdmmc_ioreg.h"
#include "sdmmcdevs.h"
#include "sdmmcvar.h"
#include "rtsxreg.h"
#include "rtsxvar.h"
#include "Sinetek_rtsx.hpp"
#define UTL_THIS_CLASS "" // there is no class, since it is c code
#include "util.h"

#if RTSX_USE_IOLOCK
#define splsdmmc(...) UTLsplsdmmc(sc->splsdmmc_rec_lock)
inline int UTLsplsdmmc(IORecursiveLock *l) {
	IORecursiveLockLock(l);
	/* UTL_DEBUG(2, "Locked splsdmmc_lock"); */
	return 0;
}

#define splx(n) do { \
	/* UTL_DEBUG(2, "Unlocking splsdmmc_lock"); */ \
	IORecursiveLockUnlock(sc->splsdmmc_rec_lock); \
} while (0)
#endif

// disable execcisve logging from OpenBSD code
#if DEBUG
#define printf(...) UTL_DEBUG(1, __VA_ARGS__)
#else
#define printf(...) do {} while (0)
#endif

extern int hz;

#endif // SINETEK_RTSX_OPENBSD_OPENBSD_COMPAT_H
