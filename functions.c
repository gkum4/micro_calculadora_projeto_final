#include <stdint.h>

// return 0 when valid and -1 when not valid
uint8_t checkIfInputIsNumber(uint8_t inputValue) {
  if (inputValue >= 48 && inputValue <= 57) {
    return 0;
  }

  return -1;
}

// return 0 when valid and -1 when not valid
uint8_t checkIfInputIsOperation(uint8_t inputValue) {
  if (  
      inputValue == '+' ||
      inputValue == '-' ||
      inputValue == '*' ||
      inputValue == '/'
    ) {
    return 0;
  }

  return -1;
}

// only positive values for valueB
int32_t power(int32_t valueA, int32_t valueB) {
  if (valueB == 0) {
    return 1;
  }

  int32_t result = valueA;

  for (int32_t i = 1; i < valueB; i++) {
    result *= valueA;
  }

  return result;
}

int32_t countCharacters(uint8_t *str) {
  int32_t result = 0;
  uint8_t aux = str[0];
  int32_t i = 1;

  while (aux != '\0') {
    result++;
    aux = str[i];
    i++;
  }

  return result;
}

// error when return value is -1
int32_t charToInt(uint8_t c) {
  int32_t asciiValue = (int32_t)c;

  if (asciiValue > 57 || asciiValue < 48) {
    return -1;
  }

  return asciiValue - 48;
}

int32_t stringToInt(uint8_t *str) {
  int32_t result = 0;
  int8_t isNegative = 0; // 0 == false, 1 == true
  int32_t numberOfCharacters = countCharacters(str);

  int32_t i = 0;

  if (str[0] == '-') {
    isNegative = 1;
    i = 1;
  }

  for (i = 0; i < numberOfCharacters; i++) {
    int32_t intValue = charToInt(str[i]);

    if (intValue == -1) {
      return -1;
    }

    result += intValue * power(10, (numberOfCharacters-1 - i));
  }

  if (isNegative == 1) {
    result *= -1;
  }

  return result;
}