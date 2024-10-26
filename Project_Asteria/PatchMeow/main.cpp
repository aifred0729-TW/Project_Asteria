#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>

void win() {
    printf("YOU WIN !!\n");
    return;
}

void checkDeeper(int value) {
    if (value == 0xc8763) {
        printf("BAD MEOW :(\n");
        exit(1);
    }
    return win();
}

bool checkInput(int value, int answer) {

    printf("Initial Check : %d\n", value);
    
    if (value == answer) return true;

    printf("Failed to pass stage 1 check :(\n");
    return false;
}

int main() {
    int value = 0;
    int answer = 0xc8763;

    scanf("%d", &value);
    
    if (checkInput(value, answer)) {
        checkDeeper(value);
    }

    return 0;
}