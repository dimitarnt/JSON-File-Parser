#pragma once
#include <fstream>
#include "SharedPtr.hpp"
#include "JsonNode.h"

class JsonNodeFactory {
public:
    virtual ~JsonNodeFactory() = default;

    virtual SharedPtr<JsonNode> create(std::ifstream& in) const = 0;
};
