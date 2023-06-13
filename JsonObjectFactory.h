#pragma once
#include "JsonNodeFactory.h"

class JsonObjectFactory : public JsonNodeFactory {
public:
    JsonNode* create(std::ifstream& in) const override;
};
