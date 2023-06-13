#pragma once
#include "JsonNodeFactory.h"

class JsonValueFactory : public JsonNodeFactory {
public:
    JsonNode* create(std::ifstream& in) const override;
};
