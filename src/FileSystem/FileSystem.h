#pragma once

#include "Log.h"

class FileSystem
{
public:
    static std::string ReadFile(const std::string& filepath);
};