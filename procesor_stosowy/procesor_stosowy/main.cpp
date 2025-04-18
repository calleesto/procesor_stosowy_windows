#include <iostream>

using namespace std;

#define MAX_INPUT 20000

struct Char {
    char ch;
    Char* next;
    Char* prev;
};

struct List {
    int index;
    Char* charHead;
    Char* charTail;
    List* next;
    List* prev;
};

void pushList(List*& head, List*& tail) { // from the tail
    // head list case
    if (tail == nullptr) {
        tail = new List();
        tail->next = nullptr;
        tail->prev = nullptr;
        head = tail;
        head->next = nullptr;
        head->prev = nullptr;
    }
    else { // next lists
        List* newList = new List();
        newList->next = nullptr;
        newList->prev = tail;
        tail->next = newList;
        tail = newList;
    }
    //initialize head char and tail char of the list
    tail->charHead = nullptr;
    tail->charTail = nullptr;
}

void pushChar(List* list, char ch) { // from the head
    // head char case
    if (list->charHead == nullptr) {
        list->charHead = new Char();
        list->charHead->next = nullptr;
        list->charHead->prev = nullptr;
        list->charHead->ch = ch;
        list->charTail = list->charHead;
    }
    else { // next chars
        Char* newChar = new Char();
        newChar->prev = nullptr;
        newChar->next = list->charHead;
        list->charHead->prev = newChar;
        list->charHead = newChar;
        newChar->ch = ch;
    }
}

void popCharHead(List* list) {
    if (list->charHead != nullptr) {
        Char* temp = list->charHead;
        list->charHead = list->charHead->next;

        if (list->charHead != nullptr) {
            list->charHead->prev = nullptr;
        }
        else {
            list->charTail = nullptr; // list is now empty
        }

        delete temp;
    }
}

void popCharTail(List* list) {
    if (list->charTail != nullptr) {
        Char* temp = list->charTail;
        list->charTail = list->charTail->prev;

        if (list->charTail != nullptr) {
            list->charTail->next = nullptr;
        }
        else {
            list->charHead = nullptr; // list is empty
        }

        delete temp;
    }
}

void pushCharTail(List* list, char ch) {
    if (list->charTail == nullptr) {
        list->charTail = new Char();
        list->charTail->next = nullptr;
        list->charTail->prev = nullptr;
        list->charTail->ch = ch;
        list->charHead = list->charTail;
    }
    else {
        Char* newChar = new Char();
        newChar->ch = ch;
        newChar->next = nullptr;
        newChar->prev = list->charTail;
        list->charTail->next = newChar;
        list->charTail = newChar;
    }
}

void popChars(List* list, Char* currentChar) { // we pass chartail here
    if (currentChar == nullptr) { return; }
    Char* temp = currentChar->prev;
    delete currentChar;
    if (temp != nullptr) {
        popChars(list, temp);
    }
    else {
        list->charHead = nullptr;
        list->charTail = nullptr;
    }
}

void popList(List*& listHead, List*& listTail) { // in the same order that we push lists so from the tail
    popChars(listTail, listTail->charTail);
    //make previous list the tail
    List* temp = listTail;
    listTail = listTail->prev;

    //if theres still lists in the stack
    if (listTail != nullptr) {
        listTail->next = nullptr;
    }
    else {
        listHead = nullptr;
    }

    delete temp;
}

void popStack(List*& listHead, List*& currentList) {
    if (currentList != nullptr) {
        List* temp = currentList->prev;
        popList(listHead, currentList);
        if (temp != nullptr) {
            popStack(listHead, temp);
        }
    }
}

void copyChars(List*& listHead, List*& listTail, Char* currentChar) {
    //currentChar is the chartail of the og list
    pushCharTail(listTail, currentChar->ch);

    if (currentChar->next != nullptr) {
        copyChars(listHead, listTail, currentChar->next);
    }
}

void printChars(List* list, Char* currentChar) {
    if (currentChar != nullptr) {
        cout << currentChar->ch;
        if (currentChar->next != nullptr) {
            printChars(list, currentChar->next);
        }
        else return;
    }
}

void printReverse(List* list, Char* ch) {
    //print first character if it exists
    cout << list->index << ": ";
    printChars(list, ch);
    cout << endl;
    if (list->next != nullptr) {
        printReverse(list->next, list->next->charHead);
    }
    else return;
}

int countLists(List* list, int counter) {
    if (list->prev != nullptr) {
        return countLists(list->prev, counter + 1);
    }
    else return counter + 1;
}

void setIndices(List*& list, int index) {
    if (list != nullptr && index >= 0) {
        list->index = index;
        setIndices(list->next, index - 1);
    }
    else return;
}

void pushTailListCopy(List*& listHead, List*& listTail) {
    //save pointer to og list
    List* originalList = listTail;

    // push new tail list
    pushList(listHead, listTail);

    // copy chars from og list to new tail list
    if (originalList->charTail != nullptr) {
        copyChars(listHead, listTail, originalList->charHead);
    }
}

void switchListPlaces(List*& listHead, List*& listTail) {
    if (listTail->prev != nullptr) { //if more than one list
        if (listTail->prev->prev == nullptr) { // if two lists
            List* prevList = listTail->prev;

            prevList->next = nullptr;
            prevList->prev = listTail;

            listTail->next = prevList;
            listTail->prev = nullptr;

            listHead = listTail;
            listTail = prevList;
        }
        else { // if three or more lists
            List* prevList = listTail->prev;
            List* prevPrevList = listTail->prev->prev;

            prevPrevList->next = listTail;

            listTail->prev = prevPrevList;
            listTail->next = prevList;

            prevList->prev = listTail;
            prevList->next = nullptr;


            listTail = prevList;
        }
    }
}

int convertListToInt(List* listTail, Char* currentChar, int result) { // we pass the tail char here
    if (currentChar != nullptr) {
        result = result * 10 + (currentChar->ch - '0');
        return convertListToInt(listTail, currentChar->prev, result);
    }
    else return result;
}

int listToSignedInt(List* listTail, Char* currentChar, int result) {
    if (currentChar == nullptr) {
        return result;
    }
    if (currentChar->ch == '-') {
        return -convertListToInt(listTail, currentChar->prev, result);
    }
    else if (currentChar->ch >= '0' && currentChar->ch <= '9') {
        result = result * 10 + (currentChar->ch - '0');
        return convertListToInt(listTail, currentChar->prev, result);
    }
}

List* findList(List* currentList, int index) {
    if (currentList != nullptr) {
        if (index != currentList->index) {
            return findList(currentList->prev, index);
        }
        else return currentList;
    }
    return nullptr;
}

void pickList(List*& listHead, List*& listTail, Char* currentChar) {
    //read tail list number so we know which index we need to target
    int result = convertListToInt(listTail, currentChar, 0);

    popList(listHead, listTail);

    int nLists = countLists(listTail, 0);
    setIndices(listHead, nLists - 1);

    //save pointer to the list we want copied
    List* someList = findList(listTail, result);

    // push new tail list
    pushList(listHead, listTail);

    // copy chars from A placed list to new tail list
    if (someList->charTail != nullptr) {
        copyChars(listHead, listTail, someList->charHead);
    }
}


void pushAsciiDigits(List*& listTail, int value) {
    if (value >= 10) {
        pushAsciiDigits(listTail, value / 10);
    }
    pushChar(listTail, '0' + (value % 10));
}

void pushAsciiCharFromNumber(List*& listHead, List*& listTail) {
    int asciiCode = convertListToInt(listTail, listTail->charTail, 0);

    popList(listHead, listTail);

    pushList(listHead, listTail);
    pushChar(listTail, static_cast<char>(asciiCode));
}

void logicalNot(List*& listHead, List*& listTail) {
    int result = convertListToInt(listTail, listTail->charTail, 0);
    if (result == 0) { //list empty or contains 0
        popList(listHead, listTail);
        pushList(listHead, listTail);
        pushChar(listTail, '1');
    }
    else {
        popList(listHead, listTail);
        pushList(listHead, listTail);
        pushChar(listTail, '0');
    }
}

void trimZeros(List*& listHead, List*& listTail, Char* currentChar, bool isNegative) {
    if (!currentChar || !currentChar->prev) return;

    if (currentChar->ch == '-' && !isNegative && currentChar->prev->ch == '0') {
        isNegative = true;
        Char* prev = currentChar->prev;
        popCharTail(listTail);
        trimZeros(listHead, listTail, prev, isNegative);
        return;
    }

    if (currentChar->ch == '0' && currentChar->prev->ch == '0') {
        Char* prev = currentChar->prev;
        popCharTail(listTail);
        trimZeros(listHead, listTail, prev, isNegative);
        return;
    }

    if (currentChar->ch == '0' && currentChar->prev->ch != '0') {
        popCharTail(listTail);
        if (isNegative) {
            pushCharTail(listTail, '-');
        }
        return;
    }
}

bool isEqual(Char* currentChar1, Char* currentChar2) {
    if (currentChar1 != nullptr) {
        if (currentChar1->ch == currentChar2->ch) {
            return isEqual(currentChar1->prev, currentChar2->prev);
        }
        else return false;
    }
    else return true;
}

bool isLessThan(Char* currentChar1, Char* currentChar2) {
    if (currentChar1 != nullptr) {
        if (currentChar1->ch == currentChar2->ch) {
            return isLessThan(currentChar1->prev, currentChar2->prev);
        }
        else if (currentChar2->ch < currentChar1->ch) {
            return true; 
        }
        else {
            return false;
        }
    }
    else return false;
}



void lessThanSign(List*& listHead, List*& listTail) {
    if (listTail->prev->charTail->ch != '0' && listTail->charTail->ch != '0' && listTail->prev->charTail->ch != '-' && listTail->charTail->ch != '-') {
        int result = isLessThan(listTail->charTail, listTail->prev->charTail);
        popList(listHead, listTail);
        popList(listHead, listTail);
        pushList(listHead, listTail);
        if (result) {
            pushChar(listTail, '1');
        }
        else {
            pushChar(listTail, '0');
        }
    }
    else {
        trimZeros(listHead, listTail, listTail->charTail, false);
        int a = listToSignedInt(listTail, listTail->charTail, 0);
        popList(listHead, listTail);
        trimZeros(listHead, listTail, listTail->charTail, false);
        int b = listToSignedInt(listTail, listTail->charTail, 0);
        popList(listHead, listTail);
        pushList(listHead, listTail);
        if (b < a) {
            pushChar(listTail, '1');
        }
        else {
            pushChar(listTail, '0');
        }
    }
}

void equalSign(List*& listHead, List*& listTail) {
    if (listTail->prev->charTail->ch != '0' && listTail->charTail->ch != '0' && listTail->prev->charTail->ch != '-' && listTail->charTail->ch != '-') {
        int result = isEqual(listTail->charTail, listTail->prev->charTail);
        popList(listHead, listTail);
        popList(listHead, listTail);
        pushList(listHead, listTail);
        if (result) {
            pushChar(listTail, '1');
        }
        else {
            pushChar(listTail, '0');
        }
    }
    else {
        trimZeros(listHead, listTail, listTail->charTail, false);
        int a = convertListToInt(listTail, listTail->charTail, 0);
        popList(listHead, listTail);
        trimZeros(listHead, listTail, listTail->charTail, false);
        int b = convertListToInt(listTail, listTail->charTail, 0);
        popList(listHead, listTail);
        pushList(listHead, listTail);
        if (b == a) {
            pushChar(listTail, '1');
        }
        else {
            pushChar(listTail, '0');
        }
    }
}

void addLists() {
    
}


void manageInput(List*& listHead, List*& listTail, char ch, int i) {
    int nLists, asciiValue;
    char headChar;
    switch (ch) {
    case '\'':
        pushList(listHead, listTail);
        break;
    case ',':
        popList(listHead, listTail);
        break;
    case '@':
        pickList(listHead, listTail, listTail->charTail);
        break;
    case '&':
        if (listTail != nullptr) {
            nLists = countLists(listTail, 0);
            setIndices(listHead, nLists - 1);
            printReverse(listHead, listHead->charHead);
        }
        break;
    case ':':
        pushTailListCopy(listHead, listTail);
        break;
    case ';':
        switchListPlaces(listHead, listTail);
        break;
    case '~':
        pushList(listHead, listTail);
        pushAsciiDigits(listTail, i);
        break;
    case '.':
        char inputChar;
        cin >> inputChar;
        pushChar(listTail, inputChar);
        break;
    case '>':
        cout << listTail->charHead->ch;
        popList(listHead, listTail);
        break;
    case '-':
        if (listTail->charHead != nullptr && listTail->charTail->ch == '-') {
            popCharTail(listTail);
        }
        else {
            pushCharTail(listTail, '-');
        }
        break;
    case '^':
        if (listTail->charHead != nullptr && listTail->charTail->ch == '-') {
            popCharTail(listTail);
        }
        break;
    case '[':
        asciiValue = static_cast<int>(listTail->charHead->ch);
        popList(listHead, listTail);
        pushList(listHead, listTail);
        pushAsciiDigits(listTail, asciiValue);
        break;
    case ']':
        pushAsciiCharFromNumber(listHead, listTail);
        break;
    case '$':
        headChar = listTail->charHead->ch;
        popCharHead(listTail);
        pushList(listHead, listTail);
        pushChar(listTail, headChar);
        break;
    case '#':
        if (listTail->charTail != nullptr) {
            copyChars(listHead, listTail->prev, listTail->charHead);
        }
        popList(listHead, listTail);
        break;
    case '=':
        equalSign(listHead, listTail);
        break;
    case '<':
        lessThanSign(listHead, listTail);
        break;
    case '!':
        logicalNot(listHead, listTail);
        break;
    case '?':
        //
        break;
    default:
        pushChar(listTail, ch);
        break;
    }
}

int main() {
    char input[MAX_INPUT];

    //reads first line of input and puts it into array
    cin.getline(input, MAX_INPUT);

    //initialize head list and tail list of the stack
    List* listHead = nullptr;
    List* listTail = nullptr;

    //throws each input array char at the manageInput function
    for (int i = 0; input[i] != '\0'; i++) {
        manageInput(listHead, listTail, input[i], i);
    }

    //stack cleanup
    if (listTail != nullptr) {
        popStack(listHead, listTail);
    }

    return 0;
}