#pragma once


#ifdef _MSC_VER
#define COCO_NODISCARD [[nodiscard]]
#define COCO_FORCE_INLINE __forceinline
#define COCO_PACK_BEGIN __pragma(pack(push, 1))
#define COCO_PACK_END ; __pragma(pack(pop))
#endif

#ifdef __GNUC__
#define COCO_NODISCARD [[gnu::warn_unused_result]] [[nodiscard]]
#define COCO_FORCE_INLINE inline __attribute__((always_inline))
#define COCO_PACK_BEGIN
#define COCO_PACK_END __attribute__((__packed__)) ;
#endif

// byte order: use std::endian
