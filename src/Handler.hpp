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
  MODE_VISUAL,
  MODE_PRE_EDIT_ONE,
  MODE_EDIT_ONE,
  MODE_PRE_EDIT,
  MODE_EDIT,
  MODE_COMMAND,
};

class Handler {
  std::wstring command;
  Mode currentMode = MODE_NORMAL;
  Mode prevMode = MODE_NORMAL;

 public:
  void handleKey(Context& context, sf::Event::KeyEvent const& evt) {}

  void handleCharacter(Context& context, sf::Uint32 c) {
    switch (currentMode) {
      case MODE_NORMAL:
      case MODE_VISUAL:
        switch (c) {
          case ESCAPE:
            prevMode = MODE_NORMAL;
            currentMode = MODE_NORMAL;
            break;

          case 'h':
            context.moveCursor(-1, 0);
            if (currentMode == MODE_NORMAL) {
              context.dropSelection();
            }
            break;

          case 'j':
            context.moveCursor(0, 1);
            if (currentMode == MODE_NORMAL) {
              context.dropSelection();
            }
            break;

          case 'k':
            context.moveCursor(0, -1);
            if (currentMode == MODE_NORMAL) {
              context.dropSelection();
            }
            break;

          case 'l':
            context.moveCursor(1, 0);
            if (currentMode == MODE_NORMAL) {
              context.dropSelection();
            }
            break;

          case 'r':
            prevMode = currentMode;
            currentMode = MODE_PRE_EDIT_ONE;
            break;

          case 'c':
            prevMode = currentMode;
            currentMode = MODE_PRE_EDIT;
            break;

          case 'v':
            if (currentMode == MODE_NORMAL) {
              currentMode = MODE_VISUAL;
            } else {
              currentMode = MODE_NORMAL;
            }
            break;

          case ':':
            command = L":";
            currentMode = MODE_COMMAND;
            break;
        }
        break;

      case MODE_PRE_EDIT_ONE:
        if (c == ESCAPE) {
        } else if ('0' <= c && c <= '9') {
          context.replaceColor(c - '0');
        } else if ('a' <= c && c <= 'z') {
          context.replaceColor(10 + c - 'a');
        }
        currentMode = prevMode;
        break;

      case MODE_EDIT_ONE:
        // TODO: insert actual mode
        currentMode = MODE_NORMAL;
        break;

      case MODE_PRE_EDIT:
        switch (c) {
          case ESCAPE:
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

      case MODE_EDIT:
        // TODO: insert actual mode
        currentMode = MODE_NORMAL;
        break;

      case MODE_COMMAND:
        if (c == '\n') {
          handleCommand(context);
          command.clear();
          currentMode = MODE_NORMAL;
        } else if (c == ESCAPE) {
          command.clear();
          currentMode = MODE_NORMAL;
        } else if (c == '\b') {
          if (!command.empty()) {
          }
        } else {
          command.push_back(c);
        }
        break;
    }
  }

  void handleCommand(Context& context) {}
};

#endif  // Handler_hpp_INCLUDED
