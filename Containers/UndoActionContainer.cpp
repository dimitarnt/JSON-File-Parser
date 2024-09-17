#include "UndoActionContainer.h"
#include "RenameCommand.h"
#include "SetCommand.h"
#include "CreateInArrayCommand.h"
#include "CreateInObjectCommand.h"
#include "RemoveCommand.h"
#include "MoveToArrayCommand.h"
#include "MoveToObjectCommand.h"

namespace {
    const String DEFAULT_KEY = "Unknown";
}

void UndoActionContainer::undo() {

    _undoActions.peek()->execute();

    _undoActions.pop();
}

void UndoActionContainer::undoAll() {

    for(size_t i = 0; !_undoActions.isEmpty(); ++i) {
        undo();
    }
}

void UndoActionContainer::lockAdding() {

    _addingIsLocked = true;
}

void UndoActionContainer::unlockAdding() {

    _addingIsLocked = false;
}

String UndoActionContainer::getRenamedPath(const String& newKey, const char* path) {
    String pathStr(path);
    size_t pathLength = pathStr.getLength();

    for(size_t i = 0; i < pathLength; ++i) {

        if(path[pathLength - i - 1] == '/') {
            pathStr = pathStr.substring(0, pathLength - i);
            pathStr += newKey;

            return pathStr;
        }
    }

    return newKey;
}

String UndoActionContainer::getRenamedPath(String&& newKey, const char* path) {
    String pathStr(path);
    size_t pathLength = pathStr.getLength();

    for(size_t i = 0; i < pathLength; ++i) {

        if(path[pathLength - i - 1] == '/') {
            pathStr = pathStr.substring(0, pathLength - i);
            pathStr += newKey;

            return pathStr;
        }
    }

    return newKey;
}

String UndoActionContainer::getPreviousPath(const char* path) {
    String pathStr(path);
    size_t pathLength = pathStr.getLength();

    for(size_t i = 0; i < pathLength; ++i) {

        if(pathStr[pathLength - i - 1] == '/') {
            return pathStr.substring(0, pathLength - i - 1);
        }
    }

    return "";
}

String UndoActionContainer::getBaseKey(const char* path) {
    String pathStr(path);
    size_t pathLength = pathStr.getLength();

    size_t startIndex = 0;

    for(size_t i = 0; i < pathLength; ++i) {

        if(pathStr[pathLength - i - 1] == '/') {
            startIndex = pathLength - i - 1;
        }
    }

    if(startIndex == 0) {
        return pathStr;
    }

    return pathStr.substring(startIndex + 1, pathLength - startIndex - 1);
}

String UndoActionContainer::addToPath(const char* path, const String& newKey) {
    String pathStr(path);

    if(pathStr.isEmpty()) {
        return newKey;
    }

    pathStr += '/';
    pathStr += newKey;

    return pathStr;
}

void UndoActionContainer::addRenameUndoAction(const String& previousKey, const String& newKey, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new RenameCommand(previousKey, getRenamedPath(newKey, path)));
}

void UndoActionContainer::addRenameUndoAction(String&& previousKey, String&& newKey, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new RenameCommand(std::move(previousKey), getRenamedPath(std::move(newKey), path)));
}

void UndoActionContainer::addRenameUndoAction(const String& previousKey, String&& newKey, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new RenameCommand(previousKey, getRenamedPath(std::move(newKey), path)));
}

void UndoActionContainer::addRenameUndoAction(String&& previousKey, const String& newKey, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new RenameCommand(std::move(previousKey), getRenamedPath(newKey, path)));
}

void UndoActionContainer::addSetUndoAction(const SharedPtr<JsonNode>& previousNode, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new SetCommand(previousNode, path));
}

void UndoActionContainer::addSetUndoAction(SharedPtr<JsonNode>&& previousNode, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new SetCommand(std::move(previousNode), path));
}

void UndoActionContainer::addCreateUndoAction(const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new RemoveCommand(path));
}

void UndoActionContainer::addRemoveUndoActionInArray(const SharedPtr<JsonNode>& removedNode, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new CreateInArrayCommand(removedNode, path));
}

void UndoActionContainer::addRemoveUndoActionInArray(SharedPtr<JsonNode>&& removedNode, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new CreateInArrayCommand(std::move(removedNode), path));
}

void UndoActionContainer::addRemoveUndoActionInObject(const JsonPair& removedPair, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new CreateInObjectCommand(removedPair, getPreviousPath(path)));
}

void UndoActionContainer::addRemoveUndoActionInObject(JsonPair&& removedPair, const char* path) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new CreateInObjectCommand(std::move(removedPair), getPreviousPath(path)));
}

void UndoActionContainer::addMoveToArrayFromArrayUndoAction(const char* previousPathTo, const char* previousPathFrom) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new MoveToArrayCommand(previousPathFrom, previousPathTo));
}

void UndoActionContainer::addMoveToObjectFromArrayUndoAction(const char* previousPathTo, const char* previousPathFrom) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new MoveToArrayCommand(previousPathFrom, addToPath(previousPathTo, DEFAULT_KEY).getData()));
}

void UndoActionContainer::addMoveToArrayFromObjectUndoAction(const char* previousPathTo, const char* previousPathFrom, const char* oldKey) {
    if(_addingIsLocked) {
        return;
    }

    _undoActions.push(new MoveToObjectCommand(getPreviousPath(previousPathFrom).getData(), previousPathTo, oldKey));
}

void UndoActionContainer::addMoveToObjectFromObjectUndoAction(const char* previousPathTo, const char* previousPathFrom, const char* oldKey) {
    if(_addingIsLocked) {
        return;
    }

    String baseKey = getBaseKey(previousPathFrom);

    _undoActions.push(new MoveToObjectCommand(getPreviousPath(previousPathFrom).getData(), addToPath(previousPathTo, baseKey).getData(), oldKey));
}
