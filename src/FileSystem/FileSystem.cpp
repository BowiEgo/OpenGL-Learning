#include "FileSystem.h"

#include <iostream>

std::string FileSystem::ReadFile(const std::string &filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        CORE_ASSERT("Could not open file '{0}'", filepath);
    }

    return result;
}
