#pragma once
#include <fstream>
#include "JsonNode.h"

class JsonNodeFactory {
public:
    virtual ~JsonNodeFactory() = default;

    virtual JsonNode* create(std::ifstream& in) const = 0;
};
