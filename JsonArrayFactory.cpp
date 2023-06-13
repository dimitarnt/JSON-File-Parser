#include "JsonArrayFactory.h"
#include "JsonArray.h"

JsonNode* JsonArrayFactory::create(std::ifstream& in) const {
    return new JsonArray(in);
}
