#pragma once

#include "ResourceNS.h"
#include <gbUtils/common.h>
#include <unordered_map>
#include <gbUtils/string.h>

GB_RENDER_RESOURCE_NS_BEGIN

template<typename T>
class Res
{
    GB_SINGLETON(Res);
public:
    template <typename str>
    const T & Get(str && resName);
private:
    typedef typename std::unordered_map<gb::utils::string, T>::const_iterator const_mp_itr;
    typedef typename std::unordered_map<gb::utils::string, T>::iterator mp_itr;
    std::unordered_map<gb::utils::string, T> mpRes;
};

GB_RENDER_RESOURCE_NS_END
