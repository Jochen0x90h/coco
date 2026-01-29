#pragma once


#ifdef __GNUC__
#define COCO_FORCE_INLINE inline __attribute__((always_inline))
#define COCO_PACK_BEGIN
#define COCO_PACK_END __attribute__((__packed__)) ;
#endif
