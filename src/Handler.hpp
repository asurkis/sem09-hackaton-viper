#ifndef Handler_hpp_INCLUDED
#define Handler_hpp_INCLUDED

#include "Context.hpp"

#include <SFML/Window.hpp>
#include <iostream>
#include <ostream>
#include <string>

sf::Uint32 constexpr ESCAPE = 27;

enum Mode {
  MODE_NORMAL = 0,
  MODE_PRE_CORRECT,
  MODE_CORRECT,
  MODE_COMMAND,
  MODE_VISUAL
};

class Handler {
  std::wstring command;
  Mode currentMode = MODE_NORMAL;

 public:
  void handleKey(Context& context, sf::Event::KeyEvent const& evt) {}

  void handleCharacter(Context& context, sf::Uint32 c) {
    switch (currentMode) {
      case MODE_VISUAL:
      case MODE_NORMAL:
        switch (c) {
          case 'h':
            context.moveCursor(-1, 0);
            break;

          case 'j':
            context.moveCursor(0, 1);
            break;

          case 'k':
            context.moveCursor(0, -1);
            break;

          case 'l':
            context.moveCursor(1, 0);
            break;

          case 'v':
            if (currentMode == MODE_NORMAL) {
              currentMode = MODE_VISUAL;
            } else {
              currentMode = MODE_NORMAL;
            }

          case ':':
            command = L":";
            currentMode = MODE_COMMAND;
            break;
        }
        break;

      case MODE_PRE_CORRECT:
        switch (c) {
          case 27:
            currentMode = MODE_NORMAL;

          case 'h':
            break;
          case 'j':
            break;
          case 'k':
            break;
          case 'l':
            break;
        }
        break;

      case MODE_CORRECT:
        break;

      case MODE_COMMAND:
        if (c == '\n') {
        } else if (c == 27) {
        }
        command.push_back(c);
        break;
    }
  }
};

#endif  // Handler_hpp_INCLUDED
