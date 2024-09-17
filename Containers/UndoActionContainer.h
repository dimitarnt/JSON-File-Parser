#pragma once
#include "ArrayStack.hpp"
#include "SharedPtr.hpp"
#include "JsonCommand.h"
#include "JsonPair.h"

class UndoActionContainer {
private:
    ArrayStack<SharedPtr<JsonCommand>> _undoActions;
    bool _addingIsLocked = false;

    static String getRenamedPath(const String& newKey, const char* path);
    static String getRenamedPath(String&& newKey, const char* path);
    static String getPreviousPath(const char* path);
    static String getBaseKey(const char* path);
    static String addToPath(const char* path, const String& newKey);

public:
    UndoActionContainer() = default;

    void undo();
    void undoAll();

    void lockAdding();
    void unlockAdding();

    void addRenameUndoAction(const String& previousKey, const String& newKey, const char* path);
    void addRenameUndoAction(String&& previousKey, String&& newKey, const char* path);
    void addRenameUndoAction(const String& previousKey, String&& newKey, const char* path);
    void addRenameUndoAction(String&& previousKey, const String& newKey, const char* path);

    void addSetUndoAction(const SharedPtr<JsonNode>& previousNode, const char* path);
    void addSetUndoAction(SharedPtr<JsonNode>&& previousNode, const char* path);

    void addCreateUndoAction(const char* path);

    void addRemoveUndoActionInArray(const SharedPtr<JsonNode>& removedNode, const char* path);
    void addRemoveUndoActionInArray(SharedPtr<JsonNode>&& removedNode, const char* path);
    void addRemoveUndoActionInObject(const JsonPair& removedPair, const char* path);
    void addRemoveUndoActionInObject(JsonPair&& removedPair, const char* path);

    void addMoveToArrayFromArrayUndoAction(const char* previousPathTo, const char* previousPathFrom);
    void addMoveToObjectFromArrayUndoAction(const char* previousPathTo, const char* previousPathFrom);
    void addMoveToArrayFromObjectUndoAction(const char* previousPathTo, const char* previousPathFrom, const char* oldKey);
    void addMoveToObjectFromObjectUndoAction(const char* previousPathTo, const char* previousPathFrom, const char* oldKey);
};
