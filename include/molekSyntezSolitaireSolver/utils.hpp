#pragma once
#include <vector>
#include <array>
#include <functional>
#include <stdexcept>
#include <string>

template<typename T>
int findIndex(const std::vector<T>& v, T value)
{
  auto it = find(v.begin(), v.end(), value);
  return it != v.end() ? it - v.begin() : -1;
}

template<typename T>
std::vector<T> splice(std::vector<T>& vec, std::size_t start)
{
  auto startIt = vec.begin() + start;
  auto endIt = vec.end();

  std::vector<T> result;
  result.reserve(endIt - startIt + 1);
  while(startIt != endIt) {
    result.push_back(*startIt);
    startIt++;
  }
  vec.resize(start);
  return result;
}

template<typename T, typename R>
std::vector<R> map(const std::vector<T>& vec, std::function<R(const T&)> mapCallback)
{
  std::vector<R> result;
  result.reserve(vec.size());
  for(std::size_t i = 0; i < vec.size(); i++) {
    result.push_back(mapCallback(vec[i]));
  }
  return result;
}

template<typename T>
T reduce(const std::vector<T>& vec, std::function<T(const T&, const T&)> reduceCallback)
{
  if(vec.empty()) {
    throw std::runtime_error("reduce called with empty vector");
  }

  T currentVal = vec[0];
  for(std::size_t i = 1; i < vec.size(); i++) {
    currentVal = reduceCallback(currentVal, vec[i]);
  }
  return currentVal;
}

template<typename T, std::size_t size>
std::size_t findIndex(const std::array<T, size>& v, T value)
{
  auto it = find(v.begin(), v.end(), value);
  return it != v.end() ? it - v.begin() : -1;
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter);