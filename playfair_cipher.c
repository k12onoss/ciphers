#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARACTER_SPACE 25 // 'a' - 'z', 'i' and 'j' are considered equal

char* process_message(const char*);

char* generate_matrix(const char*);

char* encipher(const char*, const char*);

char* decipher(const char*, const char*);

int is_valid(const char*);

int index_of(char);

char char_at(int);

int main(void) {
    int message_size;
    printf("Enter message size: ");
    scanf_s("%d", &message_size);

    char* message = malloc(message_size * sizeof(char));
    if (message == NULL) {
        printf("Memory allocation for message failed.");
        return 1;
    }

    printf("Enter the message: ");
    while (getchar() != '\n'); // consume the newline character(s) left in the input buffer, if any
    fgets(message, message_size+1, stdin);

    // consume leftover input stream, if any
    char* leftover = strchr(message, '\n');
    if (leftover == NULL) {
        scanf_s("%*[^\n]");
        scanf_s("%*c");
    } else {
        *leftover = '\0';
    }

    // check if the plain-text contains any invalid characters
    if (!is_valid(message)) {
        printf("The entered message is not supported.");
        free(message);
        return 1;
    }

    char* new_message = process_message(message);
    printf("Message after processing: ");
    fputs(new_message, stdout);

    int key_size;
    printf("\nEnter key size: ");
    scanf_s("%d", &key_size);

    char* key = malloc(key_size * sizeof(char));
    if (key == NULL) {
        printf("Memory allocation for key failed.");
        free(message);
        free(new_message);
        return 1;
    }

    printf("Enter the key: ");
    while (getchar() != '\n'); // consume the newline character(s) left in the input buffer, if any
    fgets(key, key_size+1, stdin);

    // consume leftover input stream, if any
    leftover = strchr(key, '\n');
    if (leftover == NULL) {
        scanf_s("%*[^\n]");
        scanf_s("%*c");
    } else {
        *leftover = '\0';
    }

    // check if the key contains any invalid characters
    if (!is_valid(key)) {
        printf("The entered key is not supported.");
        free(key);
        free(new_message);
        free(message);
        return 1;
    }

    char* matrix = generate_matrix(key);

    // print matrix
    printf("The generated matrix is as follows:");
    for (int i = 0; matrix[i] != '\0'; i++) {
        if (i%5 == 0) printf("\n");
        printf("%c ", matrix[i]);
    }

    int operations;
    printf("\nSelect one of the following operations.\nType 1 for encryption, 2 for decryption: ");
    scanf_s("%d", &operations);

    if (operations == 1) {
        char* ciphertext = encipher(new_message, matrix);
        printf("The cipher-text is: ");
        fputs(ciphertext, stdout);

        free(ciphertext);
    } else if (operations == 2) {
        char* plaintext = decipher(new_message, matrix);
        printf("The plain-text is: ");
        fputs(plaintext, stdout);

        free(plaintext);
    } else {
        printf("Invalid operation selected. Try again.");
    }

    free(message);
    free(new_message);
    free(key);
    free(matrix);

    return 0;
}

char* process_message(const char* message) {
    char* new_message = (char*) malloc(2 * strlen(message) * sizeof(char));
    if (new_message == NULL) {
        printf("Memory allocation failed.");
        exit(1);
    }

    // if there are any repeated characters, add an 'x' (or 'z') in between
    int j = 1;
    for (int i = 1; message[i] != '\0'; i += 2, j += 2) {
        if (message[i-1] == message[i]) {
            new_message[j-1] = (message[i-1] == 'j') ? 'i' : message[i-1];
            new_message[j] = (message[i] == 'x') ? 'z' : 'x';
            new_message[j+1] = (message[i] == 'j') ? 'i' : message[i];
            j++;
        } else {
            new_message[j-1] = (message[i-1] == 'j') ? 'i' : message[i-1];
            new_message[j] = (message[i] == 'j') ? 'i' : message[i];
        }
    }
    
    // add an 'z' (or 'x') in the end if the legth of the new_plaintext is odd
    if ((j-1) % 2 != 0) {
        new_message[j-1] = (new_message[j-2] == 'z') ? 'x' : 'z';
        new_message[j] = '\0';
    } else {
        new_message[j-1] = '\0';
    }

    return new_message;
}

char* generate_matrix(const char* key) {
    // to keep track of characters that have already appeared
    int space[CHARACTER_SPACE];
    for (int i = 0; i < CHARACTER_SPACE; i++) {
        space[i] = 0;
    }

    char* matrix = (char*) malloc(CHARACTER_SPACE * sizeof(char));

    // include all the unique characters from the key
    int j = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        if (space[index_of(key[i])] == 0) {
            matrix[j] = key[i] == 'j' ? 'i' : key[i];
            j++;
            space[index_of(key[i])] = 1;
        }
    }

    // include the rest of the characters
    for (int i = 0; i < CHARACTER_SPACE; i++) {
        if (space[i] == 0) {
            matrix[j] = char_at(i);
            j++;
        }
    }
    matrix[j] = '\0';

    return matrix;
}

char* encipher(const char* plaintext, const char* matrix) {
    int space[CHARACTER_SPACE];
    for (int i = 0; i < CHARACTER_SPACE; i++) {
        space[i] = -1;
    }

    // mark which characters appear in plaintext
    for (int i = 0; plaintext[i] != '\0'; i++) {
        space[index_of(plaintext[i])] = 1;
    }

    // find the position of those characters in the matrix
    for (int i = 0; matrix[i] != '\0'; i++) {
        if (space[index_of(matrix[i])] == 1) {
            space[index_of(matrix[i])] = i;
        }
    }

    char* ciphertext = (char*) malloc(strlen(plaintext) * sizeof(char));
    if (ciphertext == NULL) {
        printf("Memory allocation failed for cipher-text.");
        exit(1);
    }

    // use the playfair rule to replace (encrypt) characters
    int i = 0;
    for ( ; plaintext[i] != '\0'; i+=2) {
        int i1 = space[index_of(plaintext[i])];
        int i2 = space[index_of(plaintext[i+1])];

        int row1 = i1/5, row2 = i2/5, col1 = i1%5, col2 = i2%5;

        int pos1, pos2;
        if (row1 == row2) { // in same row
            pos1 = row1*5 + (i1 + 1)%5;
            pos2 = row2*5 + (i2 + 1)%5;
        } else if (col1 == col2) { // in same column
            pos1 = (i1 + 5) % CHARACTER_SPACE;
            pos2 = (i2 + 5) % CHARACTER_SPACE;
        } else {
            pos1 = row1*5 + col2;
            pos2 = row2*5 + col1;
        }

        ciphertext[i] = matrix[pos1];
        ciphertext[i+1] = matrix[pos2];
    }
    ciphertext[i] = '\0';

    return ciphertext;
}

char* decipher(const char* ciphertext, const char* matrix) {
    int space[CHARACTER_SPACE];
    for (int i = 0; i < CHARACTER_SPACE; i++) {
        space[i] = -1;
    }

    // mark which characters appear in ciphertext
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        space[index_of(ciphertext[i])] = 1;
    }

    // find the position of those characters in the matrix
    for (int i = 0; matrix[i] != '\0'; i++) {
        if (space[index_of(matrix[i])] == 1) {
            space[index_of(matrix[i])] = i;
        }
    }

    char* decrypted = (char*) malloc(strlen(ciphertext) * sizeof(char));
    if (decrypted == NULL) {
        printf("Memory allocation failed for cipher-text.");
        exit(1);
    }

    int i = 0;
    for ( ; ciphertext[i] != '\0'; i+=2) {
        int i1 = space[index_of(ciphertext[i])];
        int i2 = space[index_of(ciphertext[i+1])];

        int row1 = i1/5, row2 = i2/5, col1 = i1%5, col2 = i2%5;

        int pos1, pos2;
        if (row1 == row2) { // in same row
            int p1 = (i1 - 1) % 5;
            if (p1 < 0) p1 = 4;
            int p2 = (i2 - 1) % 5;
            if (p2 < 0) p2 = 4;
            pos1 = row1*5 + p1;
            pos2 = row2*5 + p2;
        } else if (col1 == col2) { // in same column
            pos1 = (i1 - 5) % CHARACTER_SPACE;
            if (pos1 < 0) pos1 += CHARACTER_SPACE;
            pos2 = (i2 - 5) % CHARACTER_SPACE;
            if (pos2 < 0) pos2 += CHARACTER_SPACE;
        } else {
            pos1 = row1*5 + col2;
            pos2 = row2*5 + col1;
        }

        decrypted[i] = matrix[pos1];
        decrypted[i+1] = matrix[pos2];
    }
    decrypted[i] = '\0';

    char* plaintext = (char*) malloc(strlen(decrypted) * sizeof(char));
    if (plaintext == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }

    // remove the extra 'x' (or 'z') that we added while processing the plaintext
    int b = 0;
    for (int a = 0; decrypted[a] != '\0'; ) {
        if ((decrypted[a] == 'x' || decrypted[a] == 'z') && decrypted[a+1] == decrypted[a-1]) {
            a++;
        } else {
            plaintext[b] = decrypted[a];
            a++;
            b++;
        }
    }
    if (plaintext[b-1] == 'x' && plaintext[b-2] == 'z') {
        plaintext[b-1] = '\0';
    } else {
        plaintext[b] = '\0';
    }

    return plaintext;
}

int is_valid(const char* string) {
    for (int i = 0; string[i] != '\0'; i++) {
        int index = index_of(string[i]);
        if (index < 0 || index > 25) {
            return 0;
        }
    }

    return 1;
}

int index_of(char c) {
    if (c > 'i') {
        return c - 'a' - 1;
    } else {
        return c - 'a';
    }
}

char char_at(int i) {
    if (i > 8) {
        return i + 'a' + 1;
    } else {
        return i + 'a';
    }
}
