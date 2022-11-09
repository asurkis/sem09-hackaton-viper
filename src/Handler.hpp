#ifndef Handler_hpp_INCLUDED
#define Handler_hpp_INCLUDED

#include <SFML/Window.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include "CommandParser.hpp"
#include "Context.hpp"

sf::Uint32 constexpr ESCAPE = 27;

enum Mode {
  MODE_NORMAL = 0,
  MODE_SELECTION,
  MODE_PRE_EDIT_ONE,
  MODE_PRE_EDIT,
  MODE_EDIT,
  MODE_COMMAND,
  MODE_PICK_UP,
};

inline std::wstring_view contextPrefix(Mode mode) {
  switch (mode) {
    case MODE_NORMAL: return L"";
    case MODE_SELECTION: return L"Selection";
    case MODE_PRE_EDIT_ONE: return L"r";
    case MODE_PRE_EDIT: return L"c";
    case MODE_EDIT: return L"EDIT";
    case MODE_COMMAND: return L":";
    case MODE_PICK_UP: return L"p";
  }
  return L"";
}

class Handler {
  std::wstring command;
  Mode currentMode = MODE_NORMAL;
  Mode prevMode    = MODE_NORMAL;
  CommandParser parser;

 public:
  void handleCharacter(Context& context, sf::Uint32 c) {
    switch (currentMode) {
      case MODE_NORMAL:
      case MODE_SELECTION:
        switch (c) {
          case ESCAPE:
            prevMode    = MODE_NORMAL;
            currentMode = MODE_NORMAL;
            context.dropSelection();
            break;

          case 'h':
          case 'j':
          case 'k':
          case 'l':
            switch (c) {
              case 'h': context.moveCursor(-1, 0); break;
              case 'j': context.moveCursor(0, 1); break;
              case 'k': context.moveCursor(0, -1); break;
              case 'l': context.moveCursor(1, 0); break;
            }
            if (currentMode == MODE_NORMAL) {
              context.dropSelection();
            } else {
              context.updateSelection();
            }
            break;

          case 'd': context.deleteColor(); break;

          case 'r':
            prevMode    = currentMode;
            currentMode = MODE_PRE_EDIT_ONE;
            break;

          case 'c':
            prevMode    = currentMode;
            currentMode = MODE_PRE_EDIT;
            break;

          case 's':
            prevMode = currentMode;
            if (currentMode == MODE_SELECTION) {
              currentMode = MODE_NORMAL;
              context.dropSelection();
            } else {
              currentMode = MODE_SELECTION;
            }
            break;

          case 'a': context.swapCursor(); break;

          case 'R':
            currentMode = MODE_SELECTION;
            context.setSelectionType(ST_RECTANGLE);
            break;

          case 'T':
            currentMode = MODE_SELECTION;
            context.setSelectionType(ST_LINE);
            break;

          case ':':
            command.clear();
            currentMode = MODE_COMMAND;
            break;
          case 'p':
            prevMode    = currentMode;
            currentMode = MODE_PICK_UP;
            break;
          case 'f': context.replacePrevColor(); break;
        }
        break;

      case MODE_PRE_EDIT_ONE:
        if (c == ESCAPE) {
        } else if (isPaletteKey(c)) {
          context.replaceColor(c);
        }
        currentMode = prevMode;
        break;

      case MODE_PRE_EDIT:
        switch (c) {
          case ESCAPE: currentMode = MODE_NORMAL;

          case 'h': break;
          case 'j': break;
          case 'k': break;
          case 'l': break;
        }
        break;

      case MODE_EDIT:
        // TODO: insert actual mode
        currentMode = MODE_NORMAL;
        break;

      case MODE_COMMAND:
        if (c == L'\r') {
          parser.handleCommand(command, context);
          command.clear();
          currentMode = prevMode;
        } else if (c == ESCAPE) {
          command.clear();
          currentMode = prevMode;
        } else if (c == L'\b') {
          if (!command.empty()) {
            command.pop_back();
          }
        } else {
          command.push_back(c);
        }
        break;

      case MODE_PICK_UP:
        context.pickUpColor(c);
        currentMode = prevMode;
        break;
    }

    context.statusLinePrefix = contextPrefix(currentMode);
    if (currentMode == MODE_COMMAND) {
      context.statusLine = command;
    } else {
      context.statusLine = std::wstring_view();
    }
  }
};

#endif  // Handler_hpp_INCLUDED
