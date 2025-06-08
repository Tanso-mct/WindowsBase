#pragma once

#include "wb_utility/include/dll_config.h"

#include <utility>

namespace wb
{

/***********************************************************************************************************************
 * PairHashとPairEqual
 * std::pairをキーとして使用するためのハッシュ関数と等価比較関数
 * これらは、std::unordered_mapやstd::unordered_setで使用されることを想定している
 * 
 * 使い方は以下の通り:
 * std::unordered_map<std::pair<int, int>, ValueType, PairHash, PairEqual> myMap;
 * std::pair<int, int> myPair = {1, 2};
 * myMap.at(myPair) = value; // 値を設定
/**********************************************************************************************************************/

struct PairHash 
{
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const 
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct PairEqual 
{
    template <typename T1, typename T2>
    bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const 
    {
        return lhs == rhs;
    }
};

} // namespace wb


