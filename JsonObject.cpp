#include "JsonObject.h"
#include "JsonValue.h"
#include "JsonArray.h"
#include "fileFunctions.h"

JsonObject::JsonObject(std::ifstream& in, unsigned nestingLevel) {
    _nestingLevel = nestingLevel;

    while(true) {
        while(in.peek() != '\"') {
            in.get();
        }

        _correspondingKeys.pushBack(parseValue(in));

        while(true) {
            char currentChar = (char)in.peek();

            if (currentChar == '\"') {

                JsonValue matchingValue(in, true);
                _jsonNodeCollection.pushBack(matchingValue.clone());

                in.get(currentChar);

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == '}') {
                    return;
                }
            }

            if (currentChar == '-' || (currentChar >= '0' && currentChar <= '9')
                || currentChar == 't' || currentChar == 'f' || currentChar == 'n') {

                JsonValue matchingValue(in, false);
                _jsonNodeCollection.pushBack(matchingValue.clone());

                in.get(currentChar);

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == '}') {
                    return;
                }
            }

            if (currentChar == '{') {
                in.get();

                JsonObject matchingObject(in, _nestingLevel + 1);
                _jsonNodeCollection.pushBack(matchingObject.clone());

                while(true) {
                    currentChar = (char)in.get();

                    if(currentChar == ',') {
                        break;
                    }
                    if(currentChar == '}') {
                        return;
                    }
                }
                break;
            }

            if (currentChar == '[') {
                in.get();

                JsonArray matchingArray(in, _nestingLevel + 1);
                _jsonNodeCollection.pushBack(matchingArray.clone());

                while(true) {
                    currentChar = (char)in.get();

                    if(currentChar == ',') {
                        break;
                    }
                    if(currentChar == '}') {
                        return;
                    }
                }
                break;
            }

            in.get();
        }
    }
}

JsonObject::JsonObject(const JsonObject& other) : JsonNode(other), _jsonNodeCollection(other._jsonNodeCollection),
                                                  _correspondingKeys(other._correspondingKeys) {
    copyFrom(other);
}

JsonObject::JsonObject(JsonObject&& other) noexcept : JsonNode(other), _jsonNodeCollection(std::move(other._jsonNodeCollection)),
                                                      _correspondingKeys(std::move(other._correspondingKeys)) {
    moveFrom(std::move(other));
}

JsonObject& JsonObject::operator=(const JsonObject& other) {
    if(this != &other) {
        JsonNode::operator=(other);
        _jsonNodeCollection = other._jsonNodeCollection;
        _correspondingKeys = other._correspondingKeys;
        copyFrom(other);
        free();
    }

    return *this;
}

JsonObject& JsonObject::operator=(JsonObject&& other) noexcept {
    if(this != &other) {
        JsonNode::operator=(other);
        _jsonNodeCollection = std::move(other._jsonNodeCollection);
        _correspondingKeys = std::move(other._correspondingKeys);
        moveFrom(std::move(other));
        free();
    }

    return *this;
}

JsonObject::~JsonObject() {
    free();
}

void JsonObject::copyFrom(const JsonObject& other) {
    this->_nestingLevel = other._nestingLevel;

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        this->_jsonNodeCollection[i] = other._jsonNodeCollection[i]->clone();
    }
}

void JsonObject::moveFrom(JsonObject&& other) {
    this->_nestingLevel = other._nestingLevel;

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        this->_jsonNodeCollection[i] = other._jsonNodeCollection[i];
        other._jsonNodeCollection[i] = nullptr;
    }
}

void JsonObject::free() {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        delete _jsonNodeCollection[i];
    }
}

void JsonObject::print() const {
    size_t numberOfPairsInObject = _correspondingKeys.getSize();

    std::cout << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        printIndentation(_nestingLevel + 1);

        std::cout << '\"' << _correspondingKeys[i] << '\"' << ':' << ' ';

        _jsonNodeCollection[i]->print();

        if(i == numberOfPairsInObject - 1) {
            std::cout << '\n';

            printIndentation(_nestingLevel);

            std::cout << '}';
            return;
        }

        std::cout << ',' << '\n';
    }
}

JsonNode* JsonObject::clone() const {
    return new JsonObject(*this);
}
