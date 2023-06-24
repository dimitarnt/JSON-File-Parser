#pragma once
#include "JsonCommand.h"

class SearchCommand : public JsonCommand {
public:
    SearchCommand() = default;
    void execute() const override;
};
