//
// Created by evgeny on 08.11.22.
//
#ifndef VIPER_COMMANDPARSER_H
#define VIPER_COMMANDPARSER_H

#include <codecvt>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <unordered_map>
#include "Context.hpp"

enum Commands {
  ERROR = -1,
  QUIT,
  SAVE_FILE,
  LOAD_FILE,
  NEW_FILE,
  EXPAND,
  PAL,
  SHADING,
  SET
};

class CommandParser {
  inline static std::unordered_map<std::wstring, Commands> const commandsDict{
      {L"q",      QUIT     },
      {L"w",      SAVE_FILE},
      {L"e",      LOAD_FILE},
      {L"new",    NEW_FILE },
      {L"expand", EXPAND   },
      {L"pal",    PAL      },
      {L"r",      SHADING  },
      {L"set", SET}
  };
  std::wstring commandName;
  std::vector<std::wstring> commandArgs;

  std::wstring s2ws(const std::string& str) const {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(str);
  }

  std::string ws2s(const std::wstring& wstr) const {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
  }

 public:
  int handleCommand(const std::wstring& commandString, Context& context) {
    setCommandNameArgs(commandString);
    auto cur_command = parseCommand(commandString);
    switch (cur_command) {
      case QUIT: context.quit(); break;
      case SAVE_FILE:
        if (commandArgs.size() == 1) {
          context.saveFile(ws2s(commandArgs[0]));
        } else {
          context.saveFile();
        }
        break;
      case LOAD_FILE:
        if (commandArgs.size() == 1) {
          context.loadFile(ws2s(commandArgs[0]));
        }
        break;
      case EXPAND:
        context.expand(commandArgs[0], std::stoi(commandArgs[1]));
        break;
      case NEW_FILE:
        // if given
        if (commandArgs.size() == 2) {
          int32_t width  = std::stoi(commandArgs[0]),
                  height = std::stoi(commandArgs[1]);
          if (width > 0 && height > 0) {
            context.newFile(width, height);
          }
        }
        break;
      case PAL:
        // key_type_color_color
        if (commandArgs.size() == 5) {
          if (commandArgs[0].size() == 1 &&
              (commandArgs[1] == L"rgb" || commandArgs[1] == L"hsv")) {
            int32_t fst = std::stoi(commandArgs[2]),
                    snd = std::stoi(commandArgs[3]),
                    thd = std::stoi(commandArgs[4]);
            if (commandArgs[1] == L"rgb") {
              context.changePalette(commandArgs[0][0],
                                    sf::Color(fst, snd, thd));
            } else if (commandArgs[1] == L"hsv") {
              context.changePalette(commandArgs[0][0],
                                    hsv(fst, 0.01f * snd, 0.01f * thd));
            }
          }
        }
      case SET:
        if (commandArgs[0] == L"paletteScale") {
          context.rescalePalette(std::stoi(ws2s(commandArgs[1])));
        } else if (commandArgs[0] == L"fontSize") {
          context.setFontSize(std::stoi(commandArgs[1]));
        } else if (commandArgs[0] == L"gridStep") {
          context.setGridStep(std::stoi(commandArgs[1]));
        } else if (commandArgs[0] == L"previewScale") {
          context.setPreviewScale(std::stoi(commandArgs[1]));
        }
        break;
    }
    return 0;
  }

  void setCommandNameArgs(const std::wstring& command) {
    commandArgs   = {};
    auto spacePos = find(command.begin(), command.end(), ' ');
    commandName   = command.substr(0, spacePos - command.begin());

    while (spacePos != command.end() && *spacePos == ' ') {
      ++spacePos;
    }

    auto prevPos = spacePos;
    while (prevPos != command.end()) {
      spacePos = find(prevPos, command.end(), ' ');
      commandArgs.push_back(
          command.substr(prevPos - command.begin(),
                         spacePos - prevPos)); /** second arg is size */
      while (spacePos != command.end() && *spacePos == ' ') {
        ++spacePos;
      }
      prevPos = spacePos;
    }
  }

  int parseCommand(const std::wstring& commandString) const {
    if (commandsDict.count(commandName) == 0) {
      return ERROR;
    } else {
      return commandsDict.at(commandName);
    }
  }
};

#endif  // VIPER_COMMANDPARSER_H
