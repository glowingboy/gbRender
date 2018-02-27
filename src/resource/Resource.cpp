#include "Resource.h"

#include <gbUtils/logger.h>

using namespace gb::render::resource;
using namespace gb;
using namespace gb::utils;

template<typename T>
template <typename str>
const T & Res<T>::Get(str && resName)
{
    assert(szEntityFile != nullptr);
    const_mp_itr itr = _mpRes.find(resName);
    if(itr != _mpRes.cend())
	return itr->second;
    else
    {
	luatable_mapper mapper(resName, luastate_mgr::Instance().getconfig_state());
	if(mapper.validate())
	{
	    T r;
	    r.from_lua(mapper);
	    std::pair<mp_itr, bool> ret =  _mpRes.insert(std::pair<string, T>
							       (std::forward<str>(entityFile), std::move(r)));
	    return ret->first->second;
	}
	else
	{
	    logger.Instance().warning(string
				      ("gb::render::resource::Res::Get can't find specified res @")
				      + resName);
	    // TODO return a default entity?
	    return _mpRes.begin()->second;
	}
    }
}

//template template <typename T> 
//const T & Res<T>::Get<const string & >(const string & resName);
//
//template template <typename T> 
//const T & Res<T>::Get<string && >( string && resName);
