#pragma once
#include "Board.hpp"

class NextStateBoard {
public:
  int from{};
  int to{};
  int numberOfCards{};
  int fromCardIndex{};
  int toCardIndex{};
  bool isNewMove;
  Board board;

  explicit NextStateBoard(int from, int to, int numberOfCards, int fromCardIndex, int toCardIndex, Board board, bool isNewMove):
    from(from),
    to(to),
    numberOfCards(numberOfCards),
    fromCardIndex(fromCardIndex),
    toCardIndex(toCardIndex),
    isNewMove(isNewMove),
    board(std::move(board))
    { }
};