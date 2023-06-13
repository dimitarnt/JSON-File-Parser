#include "JsonObjectFactory.h"
#include "JsonObject.h"

JsonNode* JsonObjectFactory::create(std::ifstream& in) const {
    return new JsonObject(in);
}
