#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <Windows.h>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string_view>

/***********************************************************************************************************************
 * Utility Includes
/**********************************************************************************************************************/

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/dx_helpers.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/functor.h"
#include "wb_utility/include/type_cast.h"
#include "wb_utility/include/window_helpers.h"
