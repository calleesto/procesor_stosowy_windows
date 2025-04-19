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

void pushHeadList(List*& head, List*& tail) {
    // head list case
    if (head == nullptr) {
        head = new List();
        head->next = nullptr;
        head->prev = nullptr;
        tail = head;
        tail->next = nullptr;
        tail->prev = nullptr;
    }
    else { // next lists
        List* newList = new List();
        newList->next = head;
        newList->prev = nullptr;
        head->prev = newList;
        head = newList;
    }
    //initialize head char and tail char of the list
    head->charHead = nullptr;
    head->charTail = nullptr;
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
    return result;
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
    if (value < 0) {
        pushChar(listTail, '-');
        value = -value;
    }
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

void conditionalJump(List*& listHead, List*& listTail, int& i) {
    int targetPosition = convertListToInt(listTail, listTail->charTail, 0);
    popList(listHead, listTail);

    bool condition = false;
    if (listTail != nullptr) {
        if (listTail->charTail != nullptr) {
            if (!(listTail->charHead == listTail->charTail && listTail->charHead->ch == '0')) {
                condition = true;
            }
        }
    }

    popList(listHead, listTail);

    if (condition) {
        i = targetPosition - 1;
    }
}

void addChars(List*& listHead, Char* currentChar1, Char* currentChar2, int remainder) { // char1 is listTail, char2 is listtail->prev
    if (currentChar1 && currentChar1->ch == '-') currentChar1 = currentChar1->next;
    if (currentChar2 && currentChar2->ch == '-') currentChar2 = currentChar2->next;

    int digit1 = currentChar1 != nullptr ? (currentChar1->ch - '0') : 0;
    int digit2 = currentChar2 != nullptr ? (currentChar2->ch - '0') : 0;
    int sum = digit1 + digit2 + remainder;

    if (currentChar1 != nullptr || currentChar2 != nullptr || remainder > 0) {
        if (sum < 10) {
            pushCharTail(listHead, '0' + sum);
            // Move to next digits, handling null pointers
            Char* nextChar1 = currentChar1 != nullptr ? currentChar1->next : nullptr;
            Char* nextChar2 = currentChar2 != nullptr ? currentChar2->next : nullptr;
            addChars(listHead, nextChar1, nextChar2, 0);
        }
        else {
            pushCharTail(listHead, '0' + (sum % 10));
            // Move to next digits, handling null pointers
            Char* nextChar1 = currentChar1 != nullptr ? currentChar1->next : nullptr;
            Char* nextChar2 = currentChar2 != nullptr ? currentChar2->next : nullptr;
            addChars(listHead, nextChar1, nextChar2, 1);
        }
    }
    else if (remainder > 0) {
        pushChar(listHead, '0' + remainder);
    }
}


void addLargeLists(List*& listHead, List*& listTail) {
    pushHeadList(listHead, listTail);

    addChars(listHead, listTail->charHead, listTail->prev->charHead, 0);

    popList(listHead, listTail);
    popList(listHead, listTail);
}



// B - A
void subtractAFromB(List*& listHead, List*& listTail, Char* a, Char* b, int borrow) {
    int digit1 = a ? a->ch - '0' : 0;
    int digit2 = b ? b->ch - '0' : 0;
    int diff = digit2 - digit1 - borrow;


    if (diff < 0) {
        diff += 10;
        borrow = 1;

    }
    else {
        borrow = 0;

    }
    pushCharTail(listHead, '0' + (diff));
    Char* nextChar1 = a ? a->next : nullptr;
    Char* nextChar2 = b ? b->next : nullptr;
    if (nextChar1 || nextChar2) {
        subtractAFromB(listHead, listTail, nextChar1, nextChar2, borrow);
    }
}

bool aIsBigger(Char* a, Char* b, int result) {
    if (a->ch >= b->ch) {
        result = true;
    }
    else {
        result = false;
    }

    if ((a->next == nullptr) && (b->next == nullptr)) {
        return result;
    }
    else if ((a->next != nullptr && b->next == nullptr)) {
        return true;
    }
    else if ((b->next != nullptr && a->next == nullptr)) {
        return false;
    }
    else {
        return aIsBigger(a->next, b->next, result);
    }
    
}

void addLists(List*& listHead, List*& listTail) {
    char a = listTail->charTail->ch;
    char b = listTail->prev->charTail->ch;
    Char* aLink = listTail->charHead;
    Char* bLink = listTail->prev->charHead;

    bool isNegA = false;
    bool isNegB = false;

    trimZeros(listHead, listTail, listTail->charTail, false);
    trimZeros(listHead, listTail->prev, listTail->prev->charTail, false);


    if (a == '-' && b == '-') { // two negatives
        // add large lists and add negative at the end
        addLargeLists(listHead, listTail);
        pushCharTail(listTail, '-');
    }
    else if (a != '-' && b != '-') { //two positives
        // add large lists
        addLargeLists(listHead, listTail);
    }
    else {
        pushHeadList(listHead, listTail);

        if (a == '-') {
            popCharTail(listTail);
            isNegA = true;
        }
        else if (b == '-') {
            popCharTail(listTail->prev);
            isNegB = true;
        }
        if (aIsBigger(aLink, bLink, false)) {
            subtractAFromB(listHead, listTail, bLink, aLink, 0);
            if (isNegA && listHead->charHead->ch != '0') {
                pushCharTail(listHead, '-');
            }
        }
        else {
            subtractAFromB(listHead, listTail, aLink, bLink, 0);
            if (isNegB && listHead->charHead->ch != '0') {
                pushCharTail(listHead, '-');
            }
        }
        popList(listHead, listTail);
        popList(listHead, listTail);
        trimZeros(listHead, listTail, listHead->charTail, false);
    }
}

void manageInput(List*& listHead, List*& listTail, char ch, int& i) {
    int asciiValue;
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
            int nLists = countLists(listTail, 0);
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
        conditionalJump(listHead, listTail, i);
        break;
    case '+':
        addLists(listHead, listTail);
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