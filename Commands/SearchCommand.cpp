#include "SearchCommand.h"
#include "JsonParser.h"

SearchCommand::SearchCommand(const String& searchTerm) : JsonCommand(JsonCommandType::SEARCH_COMMAND), _searchTerm(searchTerm) {}

SearchCommand::SearchCommand(String&& searchTerm) : JsonCommand(JsonCommandType::SEARCH_COMMAND), _searchTerm(std::move(searchTerm)) {}

void SearchCommand::execute() const {
    JsonParser::getInstance()->search(_searchTerm.getData());

    std::cout << std::endl << std::endl;
}
