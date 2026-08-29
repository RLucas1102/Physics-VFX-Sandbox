#ifndef FIELDINTERFACE_H
#define FIELDINTERFACE_H

#include <memory>

#include "Volume.h"

namespace lux {

    template <typename T>
    std::shared_ptr<Volume<T>> add(const std::shared_ptr<Volume<T>> &a, 
                                   const std::shared_ptr<Volume<T>> &b) 
    {
        return std::make_shared<AddField<T>>(a,b);
    }

    template <typename T>
    std::shared_ptr<Volume<T>> operator+(const std::shared_ptr<Volume<T>> &a, 
                                         const std::shared_ptr<Volume<T>> &b)
    {
        return add(a,b);
    }
}

#endif