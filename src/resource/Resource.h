#pragma once

#include "ResourceNS.h"
#include <gbUtils/common.h>
#include <unordered_map>
#include <gbUtils/logger.h>
#include <gbUtils/string.h>
#include <gbUtils/luatable.h>
#include <algorithm>
#include "../data/Shader.h"
#include "../data/Font.h"
#include "../file/Font.h"

#define GB_RENDER_RESOURCE_CFG_SHADER "Shader"
#define GB_RENDER_RESOURCE_CFG_MATERIAL "Material"
#define GB_RENDER_RESOURCE_CFG_MESH "Mesh"
#define GB_RENDER_RESOURCE_CFG_ENTITY "Entity"
#define GB_RENDER_RESOURCE_CFG_FONT "Font"


#define GB_RENDER_RESOURCE_CFG_KEY_RESROOT "ResRoot"
#define GB_RENDER_RESOURCE_CFG_KEY_BASEFILES "BaseFiles"
#define GB_RENDER_RESOURCE_CFG_KEY_DEFAULTRES "DefaultRes"


GB_RENDER_RESOURCE_NS_BEGIN

struct _Res_cfg
{
	_Res_cfg() :
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
class _Res_base
{
protected:
	_Res_base() :
		_Initialized(false)
	{}
	~_Res_base()
	{
		std::for_each(_mpRes.begin(), _mpRes.end(), [](std::pair<const gb::utils::string, T*>& res)
		{
			GB_SAFE_DELETE(res.second);
		});
	}
public:
	bool Initialize(const char* configFile)
	{
		assert(configFile != nullptr);

		gb::utils::luatable_mapper mapper(_LuaStates[0]);
		if (mapper.map_file(configFile))
		{
			_Res_cfg cfg = mapper.get_table<_Res_cfg>();
			mapper.unmap();

			SetResRoot(filesystem::Instance().get_absolute_path(cfg.resRoot));

			std::for_each(cfg.baseFiles.begin(), cfg.baseFiles.end(), [this](const gb::utils::string& bf)
			{
				_LuaStates.dofile(filesystem::Instance().get_absolute_path(bf));
			});

			_DefaultRes = filesystem::Instance().get_absolute_path(cfg.defaultRes);

			T* res = _load_res(_DefaultRes);
			if (res)
			{
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

	T* Get(const char* resName)
	{
		assert(resName != nullptr && _Initialized);

		gb::utils::string resPath = _ResRoot;
		resPath += resName;

		const_mp_itr itr = _mpRes.find(resPath);
		if (itr != _mpRes.cend())
			return itr->second;
		else
		{
			T* res = _load_res(resPath);
			if (res != nullptr)
				return res;
			else
			{
				gb::utils::logger::Instance().warning(gb::utils::string
				("gb::render::resource::Res::Get can't find specified res @")
					+ resPath);
				return _mpRes.at(_DefaultRes);
			}

		}
	}
private:
	virtual T* _load_res(const char* data) = 0;
protected:
	typedef typename std::unordered_map<const gb::utils::string, T*>::const_iterator const_mp_itr;
	typedef typename std::unordered_map<const gb::utils::string, T*>::iterator mp_itr;
	std::unordered_map<const gb::utils::string, T*> _mpRes;

	GB_PROPERTY(protected, ResRoot, gb::utils::string);
	GB_PROPERTY_R(protected, LuaStates, gb::utils::luastate_mt);
	GB_PROPERTY_R(protected, Initialized, bool);
	GB_PROPERTY_R(protected, DefaultRes, gb::utils::string)
};


template<typename T>
class Res : public _Res_base<T>
{
	GB_SINGLETON(Res);
private:
	virtual T* _load_res(const char* data) override
	{
		gb::utils::luatable_mapper mapper(_LuaStates[0]);
		if (mapper.map_file(data))
		{
			T* r = new T;
			r->from_lua(mapper);
			mapper.unmap();

			std::pair<mp_itr, bool> ret = _mpRes.insert(std::make_pair(data, r));
			assert(ret.second);
			return r;
		}
		else
			return nullptr;
	}
};

//specialization for Shader
template <>
class Res <gb::render::data::Shader> : public _Res_base<gb::render::data::Shader>
{
	GB_SINGLETON(Res);
private:
	virtual  gb::render::data::Shader* _load_res(const char* data) override
	{
		gb::utils::luatable_mapper mapper(_LuaStates[0]);

		gb::render::data::Shader* r = new gb::render::data::Shader;
		if (r->from_lua(mapper, data))
		{
			std::pair<mp_itr, bool> ret = _mpRes.insert(std::make_pair(data, r));

			assert(ret.second);

			return r;
		}
		else
		{
			GB_SAFE_DELETE(r);
			return nullptr;
		}

	}
};

//Font
template <>
class Res <gb::render::data::Font> : public _Res_base<gb::render::data::Font>
{
	GB_SINGLETON(Res);
private:
	virtual  gb::render::data::Font* _load_res(const char* data) override
	{
		data::Font* font = gb::render::file::Font::Instance().ParseFromFile(data);
		if (font != nullptr)
		{
			auto ret = _mpRes.insert(std::make_pair(data, font));

			assert(ret.second);

			return ret.first->second;
		}
		else
			return nullptr;
	}

};

//texture
template <>
class Res < gb::render::GLTexture> : public _Res_base<gb::render::GLTexture>
{
	GB_SINGLETON(Res);

public:
	virtual  gb::render::GLTexture* _load_res(const char* data) override
	{
		//TODO:
		return nullptr;
	}
};
GB_RENDER_RESOURCE_NS_END
