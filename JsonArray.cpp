#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"
#include "fileFunctions.h"

JsonArray::JsonArray(std::ifstream& in, unsigned nestingLevel) {
    _nestingLevel = nestingLevel;

    while(true) {
        char currentChar = (char)in.peek();

        if(currentChar == '\"') {

            _jsonNodeCollection.addJsonString(in);

            currentChar = (char)in.peek();

            if(currentChar == ']') {
                in.get();
                return;
            }
        }

        if(currentChar == '-' || (currentChar >= '0' && currentChar <= '9')
           || currentChar == 't' || currentChar == 'f' || currentChar == 'n') {

            _jsonNodeCollection.addJsonValue(in);

            currentChar = (char)in.peek();

            if(currentChar == ']') {
                in.get();
                return;
            }
        }

        if(currentChar == '{') {
            in.get();

            _jsonNodeCollection.addJsonObject(in, _nestingLevel + 1);

            while(true) {
                currentChar = (char)in.get();

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == ']') {
                    return;
                }
            }
            continue;
        }

        if(currentChar == '[') {
            in.get();

           _jsonNodeCollection.addJsonArray(in, _nestingLevel + 1);

            while(true) {
                currentChar = (char)in.get();

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == ']') {
                    return;
                }
            }
            continue;
        }

        in.get();
    }
}

void JsonArray::print() const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    std::cout << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        printIndentation(_nestingLevel + 1);

        _jsonNodeCollection[i]->print();

        if(i == numberOfElementsInArray - 1) {
            std::cout << '\n';

            printIndentation(_nestingLevel);

            std::cout << ']';
            return;
        }

        std::cout << ',' << '\n';
    }
}

JsonNode* JsonArray::clone() const {
    return new JsonArray(*this);
}
