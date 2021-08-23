#include "ResourceManager.h"
#include "../Libs/Sdl.h"

namespace tei::internal::resources
{

	ResourceManager::Service Resources {};

	ResourceManager::ResourceManager(fs::path defaultPath)
		: m_DefaultPath{ defaultPath }
	{
		METRICS_TIMEBLOCK;

		fs::current_path(defaultPath);
	}

	ResourceManager::~ResourceManager()
	{}

	fs::path ResourceManager::GetExisting(fs::path const& path) const
	{
		if (std::filesystem::exists(path))
			return std::filesystem::absolute(path);

		else if (auto newpath = m_DefaultPath / path; std::filesystem::exists(newpath))
			return newpath;

		else throw utility::TeiRuntimeError{ "Could not find valid path: " + path.string() };
	}

}