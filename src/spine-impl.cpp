#pragma once
#include <spine/spine.h>

namespace spine
{
    SpineExtension *getDefaultExtension()
    {
        return new DefaultSpineExtension();
    }
}
