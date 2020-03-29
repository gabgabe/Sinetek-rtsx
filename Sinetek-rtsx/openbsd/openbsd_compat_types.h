#ifndef SINETEK_RTSX_OPENBSD_OPENBSD_COMPAT_TYPES_H
#define SINETEK_RTSX_OPENBSD_OPENBSD_COMPAT_TYPES_H

#include <libkern/libkern.h> // size_t

typedef u_long bus_addr_t;
typedef void*  bus_dmamap_callback_t; // don't know the type yet!
//typedef void*  bus_dmasync_op_t; // don't know the type yet!
//typedef void*  bus_dma_segment_t; // don't know the type yet!
typedef void*  bus_dma_tag_t; // don't know the type yet!
typedef u_long bus_size_t; // don't know the type yet!
typedef void*  bus_space_handle_t; // don't know the type yet!
typedef void*  bus_space_tag_t; // don't know the type yet!

struct rwlock {
    int dummy;
};

struct IOBufferMemoryDescriptor; // forward declaration
typedef struct {
    // PUBLIC MEMBERS
    uint64_t                      ds_addr;
    uint64_t                      ds_len;
} bus_dma_segment_t;

typedef struct {
    /*
     * PRIVATE MEMBERS: not for use by machine-independent code.
     */
    bus_size_t    _dm_size;    /* largest DMA transfer mappable */
    int           _dm_flags;    /* misc. flags */
    int           _dm_segcnt;    /* number of segs this map can map */
    bus_size_t    _dm_maxsegsz;    /* largest possible segment */
    bus_size_t    _dm_boundary;    /* don't cross this */

    void        * _dm_cookie;    /* cookie for bus-specific functions */

    /*
     * PUBLIC MEMBERS: these are used by machine-independent code.
     */
    bus_size_t        dm_mapsize;    /* size of the mapping */
    int               dm_nsegs;    /* # valid segments in mapping */
    bus_dma_segment_t dm_segs[1];    /* segments; variable length */
} bus_dmamap, *bus_dmamap_t;



#endif /* SINETEK_RTSX_OPENBSD_OPENBSD_COMPAT_TYPES_H */