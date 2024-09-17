#include "SaveAsCommand.h"
#include "JsonParser.h"

SaveAsCommand::SaveAsCommand(const String& fileName, const String& path) : JsonCommand(JsonCommandType::SAVE_AS_COMMAND),
                                                                           _fileName(fileName), _path(path) {}

SaveAsCommand::SaveAsCommand(String&& fileName, String&& path) : JsonCommand(JsonCommandType::SAVE_AS_COMMAND),
                                                                 _fileName(std::move(fileName)), _path(std::move(path)) {}

SaveAsCommand::SaveAsCommand(const String& fileName, String&& path) : JsonCommand(JsonCommandType::SAVE_AS_COMMAND),
                                                                      _fileName(fileName), _path(std::move(path)) {}

SaveAsCommand::SaveAsCommand(String&& fileName, const String& path) : JsonCommand(JsonCommandType::SAVE_AS_COMMAND),
                                                                      _fileName(std::move(fileName)), _path(path) {}

void SaveAsCommand::execute() const {
    JsonParser::getInstance()->saveAs(_fileName.getData(), _path.getData());

    std::cout << "File saved." << std::endl << std::endl;
}
