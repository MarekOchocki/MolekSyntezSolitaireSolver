#pragma once
#include "GameWindow.hpp"
#include "Board.hpp"
#include "NextStateBoard.hpp"
#include <queue>
#include <vector>
#include <optional>
#include <unordered_set>

enum ControllerStepType {
  StartNewGame,
  Wait,
  SolveFromInitState,
  MoveCards
};

class ControllerStep {
public:
  ControllerStepType type = StartNewGame;
  int waitTimeMs{};
  int fromColumn{};
  int toColumn{};
  int fromColumnCardIndex{};
  int toColumnCardIndex{};

  static ControllerStep createStartNewGame() {
    return {};
  }

  static ControllerStep createWait(int ms) {
    ControllerStep result;
    result.type = Wait;
    result.waitTimeMs = ms;
    return result;
  }

  static ControllerStep createSolveFromInitState() {
    ControllerStep result;
    result.type = SolveFromInitState;
    return result;
  }

  static ControllerStep createMoveCards(int from, int to, int fromCardIndex, int toCardIndex) {
    ControllerStep result;
    result.type = MoveCards;
    result.fromColumn = from;
    result.toColumn = to;
    result.fromColumnCardIndex = fromCardIndex;
    result.toColumnCardIndex = toCardIndex;
    return result;
  }
};

class Controller {
private:
  static constexpr int cardDiffX = 82;
  static constexpr int cardDiffY = 16;

  static constexpr int firstCardMiddleX = 275;
  static constexpr int firstCardMiddleY = 153;

  static constexpr int firstCardLetterX = 244;
  static constexpr int firstCardLetterY = 149;

  bool isPaused = true;
  GameWindow gameWindow;
  std::queue<ControllerStep> steps;

  mutable std::unordered_set<std::string> visitedStates;

public:
  void run();

private:
  static bool isPressed(char key) ;
  void doOneStep();
  void refresh();

  void addInitialSteps();

  static void wait(int ms) ;
  void startNewGame();
  void solveFromInitState();
  void moveCards(const ControllerStep& step);

  Board readInitBoardState() const;
  std::optional<std::vector<NextStateBoard>> solve(const Board& board) const;

  static int getColumnCenterX(int columnIndex) ;
  static int getCardCenterY(int cardIndex) ;

  std::string readCard(int columnIndex, int cardIndex) const;
  bool doPixelsMatch(int top, int left, int cardNumber) const;
};
