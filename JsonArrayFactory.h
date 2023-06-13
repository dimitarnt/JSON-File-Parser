#pragma once
#include "JsonNodeFactory.h"

class JsonArrayFactory : public JsonNodeFactory {
public:
    JsonNode* create(std::ifstream& in) const override;
};
