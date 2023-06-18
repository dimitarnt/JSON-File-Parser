#pragma once
#include "JsonNodeFactory.h"

class JsonObjectFactory : public JsonNodeFactory {
public:
    SharedPtr<JsonNode> create(std::ifstream& in) const override;
};
