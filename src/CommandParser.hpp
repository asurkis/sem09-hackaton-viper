//
// Created by evgeny on 08.11.22.
//
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Context.hpp"
#include "unordered_map"

#ifndef VIPER_COMMANDPARSER_H
#define VIPER_COMMANDPARSER_H

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
    PALATTE,
    SHADING,
};

class CommandParser {
public:
    CommandParser(){
        commandsDict=std::unordered_map<std::wstring, Commands> ({{L"q", QUIT},
                                                                 {L"w", SAVE_FILE},
                                                                 {L"e",LOAD_FILE },
                                                                 {L"new",NEW_FILE},
                                                                 {L"scale",SCALE },
                                                                 {L"expand", EXPAND},
                                                                 {L"line",LINE},
                                                                 {L"rec", REC},
                                                                 {L"p",PALATTE },
                                                                 {L"r",SHADING }});
    }
    std::unordered_map<std::wstring, Commands> commandsDict;
    std::wstring commandName;
    std::vector<std::wstring> commandArgs;

    int handleCommand(const std::wstring& commandString, Context& context) {
      auto cur_command = parseCommand(commandString);
      switch(cur_command){
        case QUIT:
          context.quit();
          break;
        case SAVE_FILE:
          context.saveFile();
          break;
      }
      return 0;
    }

    int parseCommand(const std::wstring& commandString) {
        setCommandNameArgs(commandString);
        if(commandsDict.count(commandName))
          return commandsDict[commandName];
        else
          return ERROR;
    }

    int setCommandNameArgs(const std::wstring & command) {
      commandArgs = {};
      auto spacePos = find(command.begin(), command.end(), ' ');
      commandName = command.substr(0, spacePos - command.begin());

      while(spacePos != command.end() && *spacePos == ' ')
        ++spacePos;

      auto prevPos = spacePos;
      while (prevPos != command.end()) {
        spacePos = find(prevPos, command.end(), ' ');
        commandArgs.push_back(command.substr(prevPos - command.begin(), spacePos - prevPos)); /** second arg is size */
        while(spacePos != command.end() && *spacePos == ' ')
          ++spacePos;
        prevPos = spacePos;
      }

      return 0;
    }
};


#endif  // VIPER_COMMANDPARSER_H