#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <sys/random.h>

#define PASSWORD_BUFFER_SIZE 100

bool check_words(int choice);
bool check_symbols(int choice);
bool check_digits(int choice);

int generate_password(
    int length,
    bool words,
    bool symbols,
    bool digits,
    char *password
);

int secure_random_bytes(void *buffer, size_t size);
int secure_random_number(uint32_t max, uint32_t *result);
void shuffle_password(char *password, int length);

int main(void)
{
    char password[PASSWORD_BUFFER_SIZE];

    int length;
    int choice;

    bool words = false;
    bool symbols = false;
    bool digits = false;

    printf("Генератор паролей\n");

    printf("Укажите кол-во символов: ");

    if (scanf("%d", &length) != 1) {
        printf("Ошибка: необходимо ввести число.\n");
        return 1;
    }

    printf("Использовать буквы? (Ответ 0 или 1): ");

    if (scanf("%d", &choice) != 1 ||
        (choice != 0 && choice != 1))
    {
        printf("Ошибка: необходимо ввести 0 или 1.\n");
        return 1;
    }

    words = check_words(choice);

    printf("Использовать цифры? (Ответ 0 или 1): ");

    if (scanf("%d", &choice) != 1 ||
        (choice != 0 && choice != 1))
    {
        printf("Ошибка: необходимо ввести 0 или 1.\n");
        return 1;
    }

    digits = check_digits(choice);

    printf("Использовать спец. знаки? (Ответ 0 или 1): ");

    if (scanf("%d", &choice) != 1 ||
        (choice != 0 && choice != 1))
    {
        printf("Ошибка: необходимо ввести 0 или 1.\n");
        return 1;
    }

    symbols = check_symbols(choice);

    int result = generate_password(
        length,
        words,
        symbols,
        digits,
        password
    );

    if (result == 0) {
        printf(
            "\nВаш сгенерированный пароль: %s\n",
            password
        );
    }

    return result == 0 ? 0 : 1;
}

bool check_words(int choice)
{
    return choice == 1;
}

bool check_symbols(int choice)
{
    return choice == 1;
}

bool check_digits(int choice)
{
    return choice == 1;
}

int secure_random_bytes(void *buffer, size_t size)
{
    unsigned char *ptr = buffer;
    size_t received = 0;

    while (received < size) {
        ssize_t result = getrandom(
            ptr + received,
            size - received,
            0
        );

        if (result > 0) {
            received += (size_t)result;
            continue;
        }

        if (result == -1 && errno == EINTR) {
            continue;
        }

        perror("getrandom");
        return -1;
    }

    return 0;
}

int secure_random_number(uint32_t max, uint32_t *result)
{
    if (max == 0) {
        return -1;
    }

    uint32_t value;
    uint32_t threshold = (uint32_t)(-max) % max;

    do {
        if (secure_random_bytes(
                &value,
                sizeof(value)
            ) != 0)
        {
            return -1;
        }

    } while (value < threshold);

    *result = value % max;

    return 0;
}

void shuffle_password(char *password, int length)
{
    for (int i = length - 1; i > 0; i--) {
        uint32_t random_index;

        if (secure_random_number(
                (uint32_t)(i + 1),
                &random_index
            ) != 0)
        {
            return;
        }

        char temp = password[i];

        password[i] = password[random_index];
        password[random_index] = temp;
    }
}

int generate_password(
    int length,
    bool words,
    bool symbols,
    bool digits,
    char *password
)
{
    const char lowercase[] =
        "abcdefghijklmnopqrstuvwxyz";

    const char uppercase[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    const char digits_list[] =
        "0123456789";

    const char symbols_list[] =
        "!@#$%^&*()_+-=~`[]{}";

    if (length <= 0) {
        printf("Пароль не может быть пустым.\n");
        return -1;
    }

    if (length >= PASSWORD_BUFFER_SIZE) {
        printf(
            "Пароль не может быть длиннее 99 символов.\n"
        );
        return -1;
    }

    if (length < 6) {
        printf(
            "Пароль должен быть не менее 6 символов.\n"
        );
        return -1;
    }

    if (!words && !symbols && !digits) {
        printf(
            "Ошибка: Вы не выбрали ни один тип символов!\n"
        );
        return -1;
    }

    char charset[128];
    size_t charset_length = 0;

    if (words) {
        for (size_t i = 0; lowercase[i] != '\0'; i++) {
            charset[charset_length++] = lowercase[i];
        }

        for (size_t i = 0; uppercase[i] != '\0'; i++) {
            charset[charset_length++] = uppercase[i];
        }
    }

    if (digits) {
        for (size_t i = 0; digits_list[i] != '\0'; i++) {
            charset[charset_length++] = digits_list[i];
        }
    }

    if (symbols) {
        for (size_t i = 0; symbols_list[i] != '\0'; i++) {
            charset[charset_length++] = symbols_list[i];
        }
    }

    int position = 0;
    uint32_t index;

    if (words) {
        if (secure_random_number(2, &index) != 0) {
            return -1;
        }

        if (index == 0) {
            if (secure_random_number(26, &index) != 0) {
                return -1;
            }

            password[position++] = lowercase[index];
        } else {
            if (secure_random_number(26, &index) != 0) {
                return -1;
            }

            password[position++] = uppercase[index];
        }
    }

    if (digits) {
        if (secure_random_number(10, &index) != 0) {
            return -1;
        }

        password[position++] = digits_list[index];
    }

    if (symbols) {
        size_t symbols_count = sizeof(symbols_list) - 1;

        if (secure_random_number(
                (uint32_t)symbols_count,
                &index
            ) != 0)
        {
            return -1;
        }

        password[position++] = symbols_list[index];
    }

    while (position < length) {
        if (secure_random_number(
                (uint32_t)charset_length,
                &index
            ) != 0)
        {
            return -1;
        }

        password[position++] = charset[index];
    }

    shuffle_password(password, length);

    password[length] = '\0';

    return 0;
}
