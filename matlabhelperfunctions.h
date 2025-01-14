#ifndef MATLABHELPERFUNCTIONS_H
#define MATLABHELPERFUNCTIONS_H
#include <QLabel>
#include <QCheckBox>
#include <vector>
#include <string_view>
#include "datapath.h"

void addArrayToArgs(std::string &args, const std::vector<std::string> &cellArray, const bool isCharArray, const std::string &prependedString=std::string(","), const std::string &arrayChars=std::string("{}"),const bool isMcc=false);

void addCharArrayToArgs(std::string &args, const std::string &charArray, const std::string &prependedString=std::string(","), const bool isMcc=false, const bool addEscapeChar=false);

void addScalarToArgs(std::string &args, const std::string &scalar, const std::string &prependedString=std::string(","));

void addBoolToArgs(std::string &args, const bool mBool, const std::string &prependedString=std::string(","));

int addDataPathsToArgs(std::string &args, const std::string &prependedString=std::string(","), std::vector<dataPath> dataPaths=std::vector<dataPath>(),const bool isMcc=false);

void addChannelPatternsToArgs(std::string &args, std::vector<std::pair<QLabel*,QCheckBox*>> channelWidgets, bool customPatterns, QString patternLine, const std::string &prependedString,const bool isMcc=false,const std::string &arrayChars=std::string("{}"));

std::string btosM(const bool val);

bool endsWith(std::string_view str, std::string_view suffix);

#endif // MATLABHELPERFUNCTIONS_H
