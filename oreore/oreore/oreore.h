#ifndef __OREORE_COMMON_HEADER_H__
#define __OREORE_COMMON_HEADER_H__

#include "TimeCounter.h"
#include "Utils.h"
#include "../../../libs/picojson/picojson.h"

#ifndef NOT_USE_COCOS2DX
#   include "./cocos2d/oreore.h"
#endif

#include "dlog.h"

/* step */
#include "./utils/step/Step.h"

// storage
#include "./utils/step/storage/StringStorage.h"

// serializer
#include "./utils/step/serializer/JSON.h"
#include "./utils/step/serializer/msgpack.h"

// process
#include "./utils/step/process/encrypt/Blowfish.h"

#endif

