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
    COMMAND1 = 0,
    COMMAND2 = 1,
    NUM_COMMANDS,
};

class CommandParser {
public:
    CommandParser() {
        //set dict
    }
    std::unordered_map<std::string, int> commandsDict;
    std::string commandName;
    std::string commandArgs;

    int parseCommand(Context& context) {
        std::string commandString = "extract from Context"; //command extract from Context
        setCommandNameArgs(commandString);

        auto id = getCommandId(commandName);
        if (NUM_COMMANDS <= id) {
            return -1; // ERROR
        }
        return id;
    }

    int setCommandNameArgs(const std::string & command) {
        auto spacePos = find(command.begin(), command.end(), ' ');
        if (spacePos == command.end()) {
            commandName = command;
            commandArgs = "";
        }
        auto spaceIndex = spacePos - command.begin();
        commandName = command.substr(0, spaceIndex);
        commandArgs = command.substr(spaceIndex + 1, command.size());
        return 0;
    }

    int getCommandId(const sf::String& commandName) {
        auto it = commandsDict.find(commandName);
        if (it != commandsDict.end()) {
            return it->second;
        }
        return NUM_COMMANDS;
    }

};


#endif  // VIPER_COMMANDPARSER_H