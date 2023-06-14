#include "JsonArrayFactory.h"
#include "JsonArray.h"

SharedPtr<JsonNode> JsonArrayFactory::create(std::ifstream& in) const {
    return new JsonArray(in);
}
