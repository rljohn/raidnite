#pragma once

#include "engine/types.h"

#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <xhash>
#include <algorithm>
#include <utility>

#undef CreateEvent

#include "engine/entity/component.h"
#include "engine/entity/entity.h"
#include "engine/system/log/logging.h"
#include "engine/system/log/log_channels.h"
#include "engine/unit/buff.h"
#include "engine/unit/power.h"
#include "engine/system/stringutil.h"
