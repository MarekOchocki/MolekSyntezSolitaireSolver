#pragma once
#include <string>
#include <vector>
#include "CardColumn.hpp"

class NextStateBoard;

class Board {
public:
  std::vector<CardsColumn> columns;

  static Board fromString(const std::vector<std::vector<std::string>>& columns);
  bool isSolved() const;
  std::vector<NextStateBoard> makeNextMoves() const;
  std::string toString() const;

private:
  Board makeMove(int from, int to, int numberOfCards) const;
};