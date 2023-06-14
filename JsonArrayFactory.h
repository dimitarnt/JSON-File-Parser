#pragma once
#include "JsonNodeFactory.h"

class JsonArrayFactory : public JsonNodeFactory {
public:
    SharedPtr<JsonNode> create(std::ifstream& in) const override;
};
