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
uint32_t power(uint32_t valueA, uint32_t valueB) {
  if (valueB == 0) {
    return 1;
  }

  uint32_t result = valueA;

  for (uint32_t i = 1; i < valueB; i++) {
    result *= valueA;
  }

  return result;
}

uint32_t countCharacters(uint8_t *str) {
  uint32_t result = 0;
  uint8_t aux = str[0];
  uint32_t i = 1;

  while (aux != '\0') {
    result++;
    aux = str[i];
    i++;
  }

  return result;
}

// error when return value is -1
uint32_t charToInt(uint8_t c) {
  uint32_t asciiValue = (uint32_t)c;

  if (asciiValue > 57 || asciiValue < 48) {
    return -1;
  }

  return asciiValue - 48;
}

uint32_t stringToInt(uint8_t *str) {
  uint32_t result = 0;
  uint8_t isNegative = 0; // 0 == false, 1 == true
  uint32_t numberOfCharacters = countCharacters(str);

  uint32_t i = 0;

  if (str[0] == '-') {
    isNegative = 1;
    i = 1;
  }

  for (i = 0; i < numberOfCharacters; i++) {
    uint32_t intValue = charToInt(str[i]);

    if (intValue == -1) {
      *error = 1;
      return -1;
    }

    result += intValue * power(10, (numberOfCharacters-1 - i));
  }

  if (isNegative == 1) {
    result *= -1;
  }

  return result;
}