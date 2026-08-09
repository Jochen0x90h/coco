#pragma once


#ifdef __GNUC__
#define COCO_NODISCARD [[gnu::warn_unused_result]] [[nodiscard]]
#define COCO_FORCE_INLINE inline __attribute__((always_inline))
#define COCO_PACK_BEGIN
#define COCO_PACK_END __attribute__((__packed__)) ;
#endif

// byte order: use std::endian
