#include "asset_paths.hpp"

#include <raylib.h>

#include <string>

const char* AssetPaths::Resolve(const char* relativePath)
{
    static std::string resolved;

    if (FileExists(relativePath))
    {
        return relativePath;
    }

    resolved = std::string("../") + relativePath;
    if (FileExists(resolved.c_str()))
    {
        return resolved.c_str();
    }

    resolved = std::string(GetApplicationDirectory()) + relativePath;
    if (FileExists(resolved.c_str()))
    {
        return resolved.c_str();
    }

    resolved = std::string(GetApplicationDirectory()) + "../" + relativePath;
    if (FileExists(resolved.c_str()))
    {
        return resolved.c_str();
    }

    return relativePath;
}