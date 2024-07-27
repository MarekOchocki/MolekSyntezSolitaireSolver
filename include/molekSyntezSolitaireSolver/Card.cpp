#include "utils.hpp"
#include "Card.hpp"

Card::Card(int number): number(number) {}

Card Card::fromString(const std::string& str) {
  const auto index = findIndex(asString, str);
  return Card(static_cast<int>(index));
}

bool Card::canBePlacedOn(const Card& other) const {
  return other.number - number == 1;
}

std::string Card::toString() const {
  if(number >= asString.size()) {
    return "unknown card";
  }
  return asString[number];
}