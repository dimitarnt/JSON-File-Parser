#include "JsonValueFactory.h"
#include "JsonValue.h"

JsonNode* JsonValueFactory::create(std::ifstream& in) const {
    return new JsonValue(in);
}
