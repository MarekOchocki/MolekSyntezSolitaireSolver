#include "Controller.hpp"
#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>

namespace {
  std::vector cardsUIData = {
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{0,1,1,1,1,1,0,0,0},
          std::array<int, 9>{0,1,1,1,1,1,1,1,0}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{1,1,1,1,1,1,1,0,0},
          std::array<int, 9>{1,1,0,0,0,0,0,0,0}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{0,1,1,1,1,1,0,0,0},
          std::array<int, 9>{0,1,1,1,0,1,1,1,0}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{0,1,1,1,1,1,0,0,0},
          std::array<int, 9>{0,1,1,1,0,0,0,1,0}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{1,1,1,0,0,1,1,1,1},
          std::array<int, 9>{1,0,0,0,0,0,0,0,0}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{1,1,0,0,0,0,1,1,0},
          std::array<int, 9>{1,1,1,1,1,1,0,0,0}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{1,1,1,1,1,0,0,0,0},
          std::array<int, 9>{1,1,1,1,1,1,1,1,1}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{1,1,0,0,0,0,1,1,0},
          std::array<int, 9>{1,1,1,1,1,1,1,1,1}
      },
      std::array<std::array<int, 9>, 2>{
          std::array<int, 9>{1,1,1,1,1,1,1,1,0},
          std::array<int, 9>{1,1,0,0,0,0,0,0,0}
      }
  };
}

void Controller::run() {
  while(true) {
    if(isPressed('Q'))
      return;
    if(isPressed('S')) {
      refresh();
      isPaused = false;
    }
    if(isPressed('P')) {
      isPaused = true;
    }
    if(!isPaused) {
      doOneStep();
    }
  }
}

bool Controller::isPressed(char key) {
  return GetKeyState(key) & 0x8000;
}

void Controller::doOneStep() {
  if(steps.empty()) {
    addInitialSteps();
  }
  const auto& currentStep = steps.front();
  steps.pop();
  switch(currentStep.type) {
    case StartNewGame:
      std::cout << "Start a new game" << std::endl;
      startNewGame();
      break;
    case Wait:
      std::cout << "Wait 200ms" << std::endl;
      wait(currentStep.waitTimeMs);
      break;
    case SolveFromInitState:
      std::cout << "Solve current board state" << std::endl;
      solveFromInitState();
      break;
    case MoveCards:
      std::cout << "move cards from column " << currentStep.fromColumn + 1 << " to column " << currentStep.toColumn << std::endl;
      moveCards(currentStep);
      break;
  }
}

void Controller::addInitialSteps() {
  steps.push(ControllerStep::createStartNewGame());
  for(int i = 0; i < 25; i++)
    steps.push(ControllerStep::createWait(200));
  steps.push(ControllerStep::createSolveFromInitState());
}

void Controller::refresh() {
  gameWindow.refresh();
}

void Controller::wait(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Controller::startNewGame() {
  gameWindow.clickLMB(480, 520);
}

void Controller::solveFromInitState() {
  gameWindow.refresh();
  Board initBoard = readInitBoardState();
  visitedStates.clear();
  const auto solutionSteps = solve(initBoard);
  if(!solutionSteps.has_value()) {
    addInitialSteps();
    std::cout << "Solution without cheat moves doesn't exist" << std::endl;
    return;
  }
  std::cout << "Found a solution" << std::endl;

  for(const auto& step : solutionSteps.value()) {
    steps.push(ControllerStep::createMoveCards(step.from, step.to, step.fromCardIndex, step.toCardIndex));
  }

  for(int i = 0; i < 2; i++)
    steps.push(ControllerStep::createWait(200));
}

void Controller::moveCards(const ControllerStep& step) {
  int fromX = getColumnCenterX(step.fromColumn);
  int fromY = getCardCenterY(step.fromColumnCardIndex);
  int toX = getColumnCenterX(step.toColumn);
  int toY = getCardCenterY(step.toColumnCardIndex);

  gameWindow.clickLMB(fromX, fromY);
  gameWindow.clickLMB(toX, toY);
}


int Controller::getColumnCenterX(int columnIndex) {
  return firstCardMiddleX + cardDiffX * columnIndex;
}

int Controller::getCardCenterY(int cardIndex) {
  return firstCardMiddleY + cardDiffY * cardIndex;
}

Board Controller::readInitBoardState() const {
  std::vector<std::vector<std::string>> initCardsAsStrings;
  for(int columnIndex = 0; columnIndex < 6; columnIndex++) {
    initCardsAsStrings.emplace_back();
    for(int cardIndex = 0; cardIndex < 6; cardIndex++) {
      initCardsAsStrings.back().push_back(readCard(columnIndex, cardIndex));
    }
  }
  return Board::fromString(initCardsAsStrings);
}

std::string Controller::readCard(int columnIndex, int cardIndex) const {
  int top = firstCardLetterY + cardDiffY * cardIndex;
  int left = firstCardLetterX + cardDiffX * columnIndex;

  int cardNumber = -1;
  for(int i = 0; i < 9; i++) {
    if(doPixelsMatch(top, left, i)) {
      cardNumber = i;
      break;
    }
  }
  if(cardNumber == -1) {
    // TODO: error handling
    std::cout << "Couldn't recognize any card" << std::endl;
  }

  return Card(cardNumber).toString();
}


bool Controller::doPixelsMatch(int top, int left, int cardNumber) const {
  auto cardData = cardsUIData[cardNumber];
  for(int i = 0; i < 9; i++) {
    Pixel pixel = gameWindow.getPixel(left + i, top);
    unsigned char expectedValue = cardData[0][i] == 1 ? 255 : 0;
    if(pixel.r != pixel.g || pixel.g != pixel.b || pixel.b != expectedValue) {
      return false;
    }
  }
  for(int i = 0; i < 9; i++) {
    Pixel pixel = gameWindow.getPixel(left, top + i);
    unsigned char expectedValue = cardData[1][i] == 1 ? 255 : 0;
    if(pixel.r != pixel.g || pixel.g != pixel.b || pixel.b != expectedValue) {
      return false;
    }
  }
  return true;
}

std::optional<std::vector<NextStateBoard>> Controller::solve(const Board& board) const {
  const std::string boardAsString = board.toString();
  if(visitedStates.find(boardAsString) != visitedStates.end()) {
    return {};
  }
  visitedStates.emplace(boardAsString);
  const auto nextMoves = board.makeNextMoves();
  for(const auto& nextMove : nextMoves) {
    if(nextMove.board.isSolved()) {
      return {std::vector{nextMove}};
    }
    const auto solutionTry = solve(nextMove.board);
    if(solutionTry.has_value()) {
      std::vector first{nextMove};
      first.insert(first.end(), solutionTry.value().begin(), solutionTry.value().end());
      return {first};
    }
  }
  return {};
}
