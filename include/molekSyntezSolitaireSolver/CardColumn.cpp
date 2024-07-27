#include "CardColumn.hpp"
#include "utils.hpp"

CardsColumn::CardsColumn(const std::vector<Card>& state): state(state) {}

CardsColumn CardsColumn::fromString(const std::vector<std::string>& columns) {
  std::vector<Card> cards;
  cards.reserve(columns.size());
  for(const auto& column : columns) {
    cards.push_back(Card::fromString(column));
  }
  return CardsColumn(cards);
}

bool CardsColumn::isSolved() const {
  if(state.size() != 9) return false;
  for(int i = 1; i < state.size(); i++) {
    if(!state[i].canBePlacedOn(state[i-1])) {
      return false;
    }
  }
  return true;
}

std::vector<Card> CardsColumn::popCards(std::size_t numberOfCards) {
  return splice(state, state.size() - numberOfCards);
}

void CardsColumn::pushCards(const std::vector<Card>& cards) {
  state.insert(state.end(), cards.begin(), cards.end());
}

std::size_t CardsColumn::getLength() const {
  return state.size();
}

std::optional<Card> CardsColumn::getCardFromBack(int reverseIndex) const {
  if(reverseIndex >= state.size()) return {};
  return {state[state.size() - reverseIndex - 1]};
}

std::optional<Card> CardsColumn::getLastCard() const {
  return getCardFromBack(0);
}

std::string CardsColumn::toString() const {
  if(state.empty()) return "[]";
  std::vector<std::string> cardsAsStrings = map<Card, std::string>(state, [](const Card& card) { return card.toString(); });
  auto stateAsString = reduce<std::string>(cardsAsStrings, [](const std::string& prev, const std::string& curr) { return prev + ", " + curr; });
  return "[" + stateAsString + "]";
}