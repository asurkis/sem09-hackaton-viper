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
    std::wstring commandArgs;

    int parseCommand(const std::wstring& commandString) {
        setCommandNameArgs(commandString);
        if(commandsDict.count(commandName))
          return commandsDict[commandName];
        else
          return ERROR;
    }

    int setCommandNameArgs(const std::wstring & command) {
        auto spacePos = find(command.begin(), command.end(), L' ');
        if (spacePos == command.end()) {
            commandName = command;
            commandArgs = L"";
            return 0;
        }
        auto spaceIndex = spacePos - command.begin();
        commandName = command.substr(0, spaceIndex);
        commandArgs = command.substr(spaceIndex + 1, command.size());
        return 0;
    }


};


#endif  // VIPER_COMMANDPARSER_H