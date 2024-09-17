#pragma once
#include "JsonCommand.h"

class SearchCommand : public JsonCommand {
private:
    String _searchTerm;

public:
    explicit SearchCommand(const String& searchTerm);
    explicit SearchCommand(String&& searchTerm);

    void execute() const override;
};
