#pragma once

#include "ResourceNS.h"
#include <gbUtils/common.h>
#include <unordered_map>
#include <gbUtils/string.h>
#include <gbUtils/luatable.h>

#define GB_RENDER_RESOURCE_CFG_KEY_RESROOT "ResRoot"
#define GB_RENDER_RESOURCE_CFG_KEY_BASEFILES "BaseFiles"
#define GB_RENDER_RESOURCE_CFG_KEY_DEFAULTRES "DefaultRes"


GB_RENDER_RESOURCE_NS_BEGIN

struct _Res_Cfg
{
	_Res_Cfg() :
		resRoot("./")
	{}
	void from_lua(const gb::utils::luatable_mapper& mapper)
	{
		if (mapper.has_key(GB_RENDER_RESOURCE_CFG_KEY_RESROOT))
			resRoot = mapper.get_string_by_key(GB_RENDER_RESOURCE_CFG_KEY_RESROOT);

		baseFiles = mapper.get_strings_by_key(GB_RENDER_RESOURCE_CFG_KEY_BASEFILES);

		defaultRes = mapper.get_string_by_key(GB_RENDER_RESOURCE_CFG_KEY_DEFAULTRES);
	}

	gb::utils::string resRoot;
	std::vector<gb::utils::string> baseFiles;
	gb::utils::string defaultRes;
};

template<typename T>
class Res
{
    GB_SINGLETON_NO_CTORDCLR(Res);
	Res():
		_Initialized(false)
	{}
public:

public:
	bool Initialize(const char* configFile)
	{
		assert(configFile != nullptr);

		gb::utils::luatable_mapper mapper(configFile, _LuaStates[0]);
		if (mapper.validate())
		{
			_Res_Cfg cfg = mapper.get_table<_Res_Cfg>();

			SetResRoot(filesystem::Instance().get_absolute_path(cfg.resRoot));

			std::for_each(cfg.baseFiles.begin(), cfg.baseFiles.end(), [this](const gb::utils::string& bf)
			{
				_LuaStates.dofile(filesystem::Instance().get_absolute_path(bf));
			});

			_DefaultRes = filesystem::Instance().get_absolute_path(cfg.defaultRes);

			gb::utils::luatable_mapper dmapper(_DefaultRes, _LuaStates[0]);
			if (dmapper.validate())
			{
				T r;
				r.from_lua(dmapper);
				std::pair<mp_itr, bool> ret = _mpRes.insert(std::pair<const gb::utils::string, T>
					(_DefaultRes, std::move(r)));

				_Initialized = true;

				return true;
			}
			else
			{
				logger::Instance().error(string
				("gb::render::resource::Res::Initialize no available default res found, configFile@")
					+ configFile);
				// TODO return a default res?
				return false;
			}
		}
		else
			return false;
	}
    const T& Get(const char* resName)
	{
		assert(resName != nullptr && _Initialized);

		gb::utils::string resPath = _ResRoot;
		resPath += resName;

		const_mp_itr itr = _mpRes.find(resPath);
		if (itr != _mpRes.cend())
			return itr->second;
		else
		{
			gb::utils::luatable_mapper mapper(resPath, _LuaStates[0]);
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
				return _mpRes.at(_DefaultRes);
			}
		}
	}

private:
    typedef typename std::unordered_map<const gb::utils::string, T>::const_iterator const_mp_itr;
    typedef typename std::unordered_map<const gb::utils::string, T>::iterator mp_itr;
    std::unordered_map<const gb::utils::string, T> _mpRes;

	GB_PROPERTY(private, ResRoot, gb::utils::string);
	GB_PROPERTY_R(private, LuaStates, gb::utils::luastate_mt);
	GB_PROPERTY_R(private, Initialized, bool);
	GB_PROPERTY_R(private, DefaultRes, gb::utils::string)
};

GB_RENDER_RESOURCE_NS_END
