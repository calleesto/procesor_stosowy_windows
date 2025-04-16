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
    else if (head->next == nullptr && head != nullptr) { // second list
        List* newList = new List();
        newList->next = nullptr;
        newList->prev = tail;
        newList->prev = head;
        tail->next = newList;
        head->next = newList;
        tail = newList;
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


void popChar(List* list) { //in the same order that we push chars so from the head
    // make next char the head
    Char* temp = list->charHead;
    list->charHead = list->charHead->next;

    // if theres still chars in the list
    if (list->charHead != nullptr) {
        list->charHead->prev = nullptr;
    }
    else {
        list->charTail = nullptr;
    }
    delete temp;
}






void popChars(List* list, Char* currentChar) { // we pass chartail here
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
    pushChar(listTail, currentChar->ch);

    if (currentChar->prev != nullptr) {
        copyChars(listHead, listTail, currentChar->prev);
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

void setIndices(List* list, int index) {
    if (list != nullptr) {
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
        copyChars(listHead, listTail, originalList->charTail);
    }
}

void switchListPlaces(List*& listTail) {
    if (listTail->prev != nullptr) { //if more than one list
        if (listTail->prev->prev == nullptr) { // if two lists
            List* prevList = listTail->prev;

            prevList->next = nullptr;
            prevList->prev = listTail;

            listTail->next = prevList;
            listTail->prev = nullptr;

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

int convertListToInt(List* listTail, Char* currentChar, int result) { // we pass the head char here
    if (currentChar != nullptr) {
        result = result * 10 + (currentChar->ch - '0');
        return convertListToInt(listTail, currentChar->prev, result);
    }
    else return result;
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
        copyChars(listHead, listTail, someList->charTail);
    }
}


void manageInput(List*& listHead, List*& listTail, char ch) {
    int nLists;
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
        switchListPlaces(listTail);
        break;
    case '~':
        nLists = countLists(listTail, 0);
        setIndices(listHead, nLists - 1);
        pushChar(listTail, '0' + (listTail->index));
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
        manageInput(listHead, listTail, input[i]);
    }

    //stack cleanup
    if (listTail != nullptr) {
        popStack(listHead, listTail);
    }

    return 0;
}