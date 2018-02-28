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
    const T & Get(const char* resName)
	{
		assert(resName != nullptr);
		gb::utils::string resPath = _Path;
		resPath += resName;

		const_mp_itr itr = _mpRes.find(resPath);
		if (itr != _mpRes.cend())
			return itr->second;
		else
		{
			luatable_mapper mapper(resPath, luastate_mgr::Instance().getconfig_state());
			if (mapper.validate())
			{
				T r;
				r.from_lua(mapper);
				std::pair<mp_itr, bool> ret = _mpRes.insert(std::pair<const gb::utils::string, T>
					(resPath, std::move(r)));
				return ret.first->second;
			}
			else
			{
				logger::Instance().warning(string
				("gb::render::resource::Res::Get can't find specified res @")
					+ resPath);
				// TODO return a default res?
				return _mpRes.begin()->second;
			}
		}
	}

private:
    typedef typename std::unordered_map<const gb::utils::string, T>::const_iterator const_mp_itr;
    typedef typename std::unordered_map<const gb::utils::string, T>::iterator mp_itr;
    std::unordered_map<const gb::utils::string, T> _mpRes;

	GB_PROPERTY(private, Path, gb::utils::string);
};

GB_RENDER_RESOURCE_NS_END
