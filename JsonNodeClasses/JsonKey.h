#pragma once
#include "String.h"

class JsonKey {
private:
    String _key;

    static void assertKey(const String& key);

public:
    JsonKey() = default;
    JsonKey(const String& key);
    JsonKey(String&& key);

    const String& getKeyString() const;
    String& accessKeyString();

    void setKeyString(const String& key);
    void setKeyString(String&& key);
};
