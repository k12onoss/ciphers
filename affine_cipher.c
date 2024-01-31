#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gcd(int a);

int a_inverse(int a);

char* encrypt(const char* plaintext, int a, int b, int buffer_size);

char* decrypt(const char* ciphertext, int a, int b, int buffer_size);

int main(void) {
    int buffer_size = 100;
    
    printf("Enter the size of (or number of characters in) the message: ");
    scanf("%d", &buffer_size);

    char* message = malloc(buffer_size * sizeof(char));
    if (message == NULL) {
        printf("Memory allocation failed for message.");
        return 1;
    }

    printf("Enter message: \n");
    getchar(); // to consume the newline character left in input buffer.
    fgets(message, buffer_size + 1, stdin);

    if (strlen(message) < buffer_size) {
        message[strlen(message) - 1] = '\0'; // to null terminate at appropriate position. This is done incase the buffer_size is greater than the length of the entered message.
    }

    printf("Enter the value of a between 0 to 25 such that gcd of a and 26 is 1: ");
    int a;
    scanf("%d", &a);

    if (a >= 0 && a < 26 && gcd(a) != 1) {
        printf("The value entered for a is incorrect.");
        return 1;
    }

    printf("Enter the value of b between 0 to 25 (inclusive): ");
    int b;
    scanf("%d", &b);

    if (b < 0 || b > 25) {
        printf("The value entered for b is incorrect.");
        return 1;
    }

    int operations;
    printf("Select one of the following operations.\nType 1 for encryption, 2 for decryption: ");
    scanf("%d", &operations);

    if (operations ==  1) {
        char* encryptedText = encrypt(message, a, b, 26);

        printf("Encrypted text:\n%s", encryptedText);

        free(encryptedText);
    } else if (operations == 2) {
        char* decryptedText = decrypt(message, a, b, buffer_size);

        printf("Decrypted text:\n%s", decryptedText);

        free(decryptedText);
    } else {
        printf("Please select the appropriate operation.");
        return 1;
    }

    free(message);
    return 0;
}

int gcd(int a) {
    int m = 26;
    while (m != 0) {
        int temp = m;
        m = a % m;
        a = temp;
    }
    return a;
}

char* encrypt(const char* plaintext, int a, int b, int buffer_size) {
    char* encrypted = malloc(buffer_size * sizeof(char));
    if (encrypted == NULL) {
        printf("Memory allocation failed for encrypted text.");
        exit(1);
    }

    int i = 0;
    for (; *(plaintext + i) != '\0'; i++) {
        char c = *(plaintext + i);
        char newC = ' ';
        if (c != ' ') {
            int x = c - 'a';
            newC = (a*x + b)%26 + 'a';
        }
        *(encrypted + i) = newC;
    }

    *(encrypted + i) = '\0';
    return encrypted;
}

int a_inverse(int a) {
    for (int x = 1; x < 26; x++) {
        int temp = (a*x)%26;
        if (temp == 1) {
            return x;
        }
    }
    return 0;
}

char* decrypt(const char* ciphertext, int a, int b, int buffer_size) {
    int a_inv = a_inverse(a);
    char* decrypted = malloc(buffer_size * sizeof(char));
    if (decrypted == NULL) {
        printf("Memory allocation failed for decrypted text.");
        exit(1);
    }

    int i = 0;
    for (; *(ciphertext + i) != '\0'; i++) {
        char c = *(ciphertext + i);
        char newC = ' ';
        if (c != ' ') {
            int x = c - 'a';
            int temp = (a_inv*(x - b)) % 26;
            if (temp < 0) {
                temp += 26;
            }
            newC = temp + 'a';
        }
        *(decrypted + i) = newC;
    }

    *(decrypted + i) = '\0';
    return decrypted;
}
