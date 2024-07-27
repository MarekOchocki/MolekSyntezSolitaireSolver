#pragma once
#include <vector>
#include <optional>
#include "Card.hpp"

class CardsColumn {
  std::vector<Card> state;

public:
  CardsColumn() = default;
  explicit CardsColumn(const std::vector<Card>& state);
  static CardsColumn fromString(const std::vector<std::string>& columns);

  bool isSolved() const;

  std::vector<Card> popCards(std::size_t numberOfCards);
  void pushCards(const std::vector<Card>& cards);
  std::size_t getLength() const;
  std::optional<Card> getCardFromBack(int reverseIndex) const;
  std::optional<Card> getLastCard() const;
  std::string toString() const;
};