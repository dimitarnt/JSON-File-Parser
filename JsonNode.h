#pragma once
#include "String.h"

class JsonNode {
public:
    JsonNode() = default;
    virtual ~JsonNode() = default;

    virtual JsonNode* clone() const = 0;

    String parseValue(std::ifstream& in) const;
};
