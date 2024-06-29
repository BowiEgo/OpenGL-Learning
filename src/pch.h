#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <variant>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>

#include "Log.h"

template<typename T, size_t N>
constexpr size_t ArrayLength(const T (&)[N]) {
    return N;
}

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;