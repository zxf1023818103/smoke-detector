#include "smokedetector.h"

char* reverse(char *str, int size) {
    char *i = str, *j = str + size - 1;
    while (i < j) {
        char t = *i;
        *i = *j;
        *j = t;
        i++; j--;
    }
    return str;
}

char* my_itoa(unsigned int value, char *buffer) {
    unsigned int i = 0;
    for (;;) {
        buffer[i++] = value % 10 + '0';
        value /= 10;
        if (value == 0) {
            break;
        }
    }
    buffer[i] = 0;
    return reverse(buffer, i);
}

int is_digit(char c) {
    return '0' <= c && c <= '9';
}

int my_atoi(char *value, unsigned int *result) {
    char *end = value;
    while (*end) {
        end++;
    }
    end--;
    int digit = 1;
    *result = 0;
    while (value <= end) {
        if (is_digit(*end)) {
            *result += digit * (*end - '0');
            digit *= 10;
            end--;
        }
        else {
            return 0;
        }
    }
    return 1;
}

int my_strcmp(const char *str1, const char *str2) {
    char *p1 = (char*)str1, *p2 = (char*)str2;
    while (*p1 == *p2 && *p1) {
        p1++; p2++;
    }
    return *p1 - *p2;
}

char *my_strchr(const char *str, char c) {
    char *p = (char*)str;
    while (*p) {
        if (*p == c) {
            return p;
        }
        p++;
    }
    return 0;
}
