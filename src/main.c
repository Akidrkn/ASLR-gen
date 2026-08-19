#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool check_words(int choice);
bool check_symbols(int choice);
bool check_digits(int choice);
int generate_password(int length, bool words, bool symbols, bool digits, char *password);

int main(void) {
    char password[100];
    int length;
    int choice; 
    bool words = false, symbols = false, digits = false;

    printf("Генератор паролей\n");
    printf("Укажите кол-во символов: ");
    scanf("%d", &length); 

    printf("Использовать буквы? (Ответ 0 или 1): ");
    scanf("%d", &choice);
    words = check_words(choice);

    printf("Использовать цифры? (Ответ 0 или 1): ");
    scanf("%d", &choice);
    digits = check_digits(choice);

    printf("Использовать спец. знаки? (Ответ 0 или 1): ");
    scanf("%d", &choice);
    symbols = check_symbols(choice);
    
    int result = generate_password(length, words, symbols, digits, password);
    
    if (result == 0) {
        printf("\nВаш сгенерированный пароль: %s\n", password);
    }
    
    return 0;
}

bool check_words(int choice) {
    return choice == 1;
}

bool check_symbols(int choice) {
    return choice == 1;
}

bool check_digits(int choice) {
    return choice == 1;
}

int generate_password(int length, bool words, bool symbols, bool digits, char *password) {
    char symbols_list[] = "!@#$^&*()_+-=~`";
    int symbols_count = 15; 

    if (length <= 0) {
        printf("Пароль не может быть пустым.\n");
        return -1;
    } else if (length >= 100) {
        printf("Пароль не может быть длиннее 100 символов.\n");
        return -1;
    } else if (length < 6) {
        printf("Пароль должен быть не менее 6 символов.\n");
        return -1;
    }
    
    if (!words && !symbols && !digits) {
        printf("Ошибка: Вы не выбрали ни один тип символов!\n");
        return -1;
    }

    int local_var_1 = 42;
    int local_var_2 = 100;
    uintptr_t addr1 = (uintptr_t)&local_var_1;
    uintptr_t addr2 = (uintptr_t)&local_var_2;
    uintptr_t addr3 = (uintptr_t)&length;
    uintptr_t seed = addr1 ^ (addr2 << 7) ^ (addr3 >> 3);

    for (int i = 0; i < length; i++) {
        while (1) {
            seed = seed * 1103515245 + 12345;
            int type = (seed / 65536) % 4;

            if ((type == 0 || type == 1) && words) {
                if (type == 0) password[i] = (char)('a' + ((seed / 65536) % 26));
                if (type == 1) password[i] = (char)('A' + ((seed / 65536) % 26));
                break;
            } 
            else if (type == 2 && symbols) {
                int idx = (seed / 65536) % symbols_count;
                password[i] = symbols_list[idx];
                break;
            }
            else if (type == 3 && digits) {
                password[i] = (char)('0' + ((seed / 65536) % 10));
                break;
            }
        }
    }
    
    password[length] = '\0'; 
    
    return 0;
}
