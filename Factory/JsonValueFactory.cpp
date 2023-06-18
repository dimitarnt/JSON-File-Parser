#include "JsonValueFactory.h"
#include "JsonValue.h"

SharedPtr<JsonNode> JsonValueFactory::create(std::ifstream& in) const {
    return new JsonValue(in);
}
