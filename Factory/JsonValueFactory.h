#pragma once
#include "JsonNodeFactory.h"

class JsonValueFactory : public JsonNodeFactory {
public:
    SharedPtr<JsonNode> create(std::ifstream& in) const override;
};
