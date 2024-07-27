#include "Board.hpp"
#include "CardColumn.hpp"
#include "NextStateBoard.hpp"
#include "utils.hpp"

Board Board::fromString(const std::vector<std::vector<std::string>>& columns) {
  Board board;
  board.columns = map<std::vector<std::string>, CardsColumn>(columns, [](const std::vector<std::string>& column) { return CardsColumn::fromString(column); });
  return board;
}

bool Board::isSolved() const {
  int numberOfNonEmptyColumns = 0;
  for(const auto& column : columns) {
    numberOfNonEmptyColumns += column.getLength() == 0 ? 0 : 1;
  }
  if(numberOfNonEmptyColumns != 4) return false;

  for(const auto& column : columns) {
    if(column.getLength() == 0) continue;
    if(!column.isSolved()) return false;
  }
  return true;
}

std::vector<NextStateBoard> Board::makeNextMoves() const {
  std::vector<NextStateBoard> result;
  for(std::size_t from = 0; from < columns.size(); from++) {
    for(std::size_t to = 0; to < columns.size(); to++) {
      if(from == to) continue;

      for(int numberOfCards = 1; numberOfCards <= columns[from].getLength(); numberOfCards++) {
        const auto cardFrom = columns[from].getCardFromBack(numberOfCards - 1);
        if(!cardFrom.has_value()) break;
        const auto nextCardFrom = columns[from].getCardFromBack(numberOfCards);
        if(nextCardFrom.has_value() && cardFrom.value().canBePlacedOn(nextCardFrom.value())) {
          continue;
        }

        const auto cardTo = columns[to].getLastCard();

        if(numberOfCards == columns[from].getLength() && !cardTo.has_value()) {
          break;
        }

        int fromCardIndex = (int)columns[from].getLength() - numberOfCards;
        int toCardIndex = (int)columns[to].getLength() + 1;

        if(!cardTo.has_value() || cardFrom.value().canBePlacedOn(cardTo.value())) {
          result.emplace_back((int)from, (int)to, numberOfCards, fromCardIndex, toCardIndex, makeMove((int)from, (int)to, numberOfCards), false);
        }
        break;
      }
    }
  }
  return result;
}

std::string Board::toString() const {
  if(columns.empty()) return "";
  std::vector<std::string> columnsAsStrings = map<CardsColumn, std::string>(columns, [](const CardsColumn& column) { return column.toString(); });
  auto stateAsString = reduce<std::string>(columnsAsStrings, [](const std::string& prev, const std::string& curr) { return prev + "\n" + curr; });
  return stateAsString;
}

Board Board::makeMove(int from, int to, int numberOfCards) const {
  Board newBoard = *this;
  auto movedCards = newBoard.columns[from].popCards(numberOfCards);
  newBoard.columns[to].pushCards(movedCards);
  return newBoard;
}
