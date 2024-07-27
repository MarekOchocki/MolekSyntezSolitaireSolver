#pragma once
#include <array>
#include <string>

class Card {
  inline static const std::array<std::string, 9> asString = {
      "6", "7", "8", "9", "10", "V", "D", "K", "T"
  };
  int number = 0;

public:
  Card() = default;
  explicit Card(int number);
  static Card fromString(const std::string& str);

  bool canBePlacedOn(const Card& other) const;
  std::string toString() const;
};
