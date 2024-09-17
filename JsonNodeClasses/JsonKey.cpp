#include "JsonKey.h"
#include "InvalidJsonSyntax.h"

void JsonKey::assertKey(const String& key) {

    if(key.getCharCount('\"') != 0 || key.getCharCount('\n') != 0 || key.getCharCount('\t') != 0
       || key.getCharCount(COMMAND_PARAMETER_STARTING_DELIMITER) != 0 || key.getCharCount(COMMAND_PARAMETER_ENDING_DELIMITER) != 0) {

        throw InvalidJsonSyntax("Disallowed character in given string");
    }
}

JsonKey::JsonKey(const String& key) {
    assertKey(key);

    _key = key;
}

JsonKey::JsonKey(String&& key) {
    assertKey(key);

    _key = std::move(key);
}

const String& JsonKey::getKeyString() const {
    return _key;
}

String& JsonKey::accessKeyString() {
    return _key;
}

void JsonKey::setKeyString(const String& key) {
    assertKey(key);

    _key = key;
}

void JsonKey::setKeyString(String&& key) {
    assertKey(key);

    _key = std::move(key);
}
