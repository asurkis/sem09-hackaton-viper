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
  SCALE,
  EXPAND,
  LINE,
  REC,
  PAL,
  PALETTE,
  SHADING,
};

class CommandParser {
  inline static std::unordered_map<std::wstring, Commands> const commandsDict{
      {L"q",      QUIT     },
      {L"w",      SAVE_FILE},
      {L"e",      LOAD_FILE},
      {L"new",    NEW_FILE },
      {L"scale",  SCALE    },
      {L"expand", EXPAND   },
      {L"line",   LINE     },
      {L"rec",    REC      },
      {L"pal",    PAL      },
      {L"p",      PALETTE  },
      {L"r",      SHADING  }
  };
  std::wstring commandName;
  std::vector<std::wstring> commandArgs;

 public:
  int handleCommand(const std::wstring& commandString, Context& context) {
    auto cur_command = parseCommand(commandString);
    switch (cur_command) {
      case QUIT: context.quit(); break;
      case SAVE_FILE: context.saveFile(); break;
      case LOAD_FILE: context.loadFile(ws2s(commandArgs[0])); break;
      case EXPAND: context.expand(stoi(commandArgs[0]), commandArgs[1]); break;
      case NEW_FILE:
        //if given
        if(commandArgs.size()==2) {
          int32_t width  = std::stoi(commandArgs[0]), heigth = std::stoi(commandArgs[1]);
          if(width>0 && heigth>0)
            context.newFile(width, heigth);
        }
        break;
      case PAL:
        //key_type_color_color
        if(commandArgs.size()==5){
            if(commandArgs[0].size()==1 && (commandArgs[1]==L"rgb" || commandArgs[1]==L"hsv")){
              int32_t fst =std::stoi(commandArgs[2]), snd =std::stoi(commandArgs[3]), thd=std::stoi(commandArgs[4]);
              context.putColor(ws2s(commandArgs[0]),ws2s(commandArgs[1]), fst, snd, thd);
            }
        }

    }
    return 0;
  }

  /** for debugging */
  std::wstring s2ws(const std::string& str) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(str);
  }

  std::string ws2s(const std::wstring& wstr) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
  }

  int parseCommand(const std::wstring& commandString) {
    setCommandNameArgs(commandString);
    if(commandsDict.count(commandName)==0)
      return ERROR;
    else
      return commandsDict.at(commandName);
  }

  int setCommandNameArgs(const std::wstring& command) {
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
    return 0;
  }
};

#endif  // VIPER_COMMANDPARSER_H
