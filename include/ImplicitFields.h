#ifndef IMPLICITFIELDS_H
#define IMPLICITFIELDS_H

#include "Volume.h"

namespace lux {

    template<typename T>
    class ConstantVolume : public Volume<T> {

        ConstantVolume();
        ~ConstantVolume = default;

        const volumeDataType eval(T v) const override

    };

} 

#endif