#include <stdint.h>

void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);
void Uart0_Init(void);
uint8_t Uart0_Rcv(void);
void Uart0_Send(uint8_t dado);
uint8_t checkIfInputIsNumber(uint8_t inputValue); // return 0 when valid and -1 when not valid
uint8_t checkIfInputIsOperation(uint8_t inputValue); // return 0 when valid and -1 when not valid
int32_t stringToInt(uint8_t *str);

typedef enum {
  FIRSTNUM,
  OPERATION,
  SECONDNUM,
  RESULT,
} CalcStep;

typedef enum {
  SUM,
  SUBTRACT,
  MULTIPLY,
  DIVIDE
} OperationType;

void sendNextLineToUart() {
  Uart0_Send('\r');
  Uart0_Send('\n');
}

OperationType getOperationType(uint8_t c) {
  switch (c) {
    case '+':
      return SUM;
      break;
    case '-':
      return SUBTRACT;
      break;
    case '*':
      return MULTIPLY;
      break;
    case '/':
      return DIVIDE;
      break;
    default:
      return SUM;
  }
}

int32_t calculate(int32_t valueA, int32_t valueB, OperationType operationType) {
  switch (operationType) {
    case SUM:
      return valueA + valueB;
      break;
    case SUBTRACT:
      return valueA - valueB;
      break;
    case MULTIPLY:
      return valueA * valueB;
      break;
    case DIVIDE:
      return valueA / valueB;
      break;
  }
}

int32_t countDigits(int32_t value) {
  int32_t result = 1;

  while (value/10 != 0) {
    result++;
    value /= 10;
  }

  return result;
}

void printIntToUart(int32_t value) {
  if (value < 0) {
    Uart0_Send('-');
    value *= -1;
  }

  int32_t numberOfDigits = countDigits(value);
  
  uint8_t str[4];

  int32_t aux = value;

  int32_t i;

  for (i = numberOfDigits; i > 0; i--) {
    str[i-1] = (uint8_t)((aux % 10) + 48);
    aux = aux / 10;
  }

  str[numberOfDigits] = '\0';

  i = 0;

  while (str[i] != '\0') {
    Uart0_Send(str[i]);
    i++;
  }
  
  sendNextLineToUart();
}

int main(void) {
  GPIO_Init();
  Uart0_Init();

  int32_t valueA;
  uint8_t valueAStr[4];
  uint8_t valueAStrPosition = 0;

  int32_t valueB = 0;
  uint8_t valueBStr[4];
  uint8_t valueBStrPosition = 0;

  OperationType operationType;
  
  CalcStep calcStep = FIRSTNUM;

  while (1) {
    uint8_t kbInput = Uart0_Rcv();

    switch (calcStep) {
      case FIRSTNUM:
        if (checkIfInputIsNumber(kbInput) == 0) {
          valueAStr[valueAStrPosition] = kbInput;
          valueAStrPosition++;
          Uart0_Send(kbInput);

          if (valueAStrPosition == 3) {
            valueAStr[valueAStrPosition] = '\0';
            calcStep = OPERATION;
          }

          break;
        }

        if (checkIfInputIsOperation(kbInput) == 0) {
          if (kbInput == '-' && valueAStrPosition == 0) {
            valueAStr[valueAStrPosition] = kbInput;
            valueAStrPosition++;
            Uart0_Send(kbInput);
            break;
          }

          if (valueAStrPosition != 0 && valueAStr[valueAStrPosition] != '-') {
            operationType = getOperationType(kbInput);
            valueAStr[valueAStrPosition] = '\0';
            Uart0_Send(kbInput);
            calcStep = SECONDNUM;
          }
        }

        break;

      case OPERATION:
        if (checkIfInputIsOperation(kbInput) == 0) {
          operationType = getOperationType(kbInput);
          Uart0_Send(kbInput);
          calcStep = SECONDNUM;
        }

        break;

      case SECONDNUM:
        if (checkIfInputIsNumber(kbInput) == 0) {
          valueBStr[valueBStrPosition] = kbInput;
          valueBStrPosition++;
          Uart0_Send(kbInput);

          if (valueBStrPosition == 3) {
            valueBStr[valueBStrPosition] = '\0';
            calcStep = RESULT;
          }

          break;
        }

        if (kbInput == '-' && valueBStrPosition == 0) {
            valueBStr[valueBStrPosition] = kbInput;
            valueBStrPosition++;
            Uart0_Send(kbInput);
            break;
          }

        if (
          kbInput == '=' && 
          valueBStrPosition != 0 && 
          valueBStr[0] != '-'
        ) {
          valueBStr[valueBStrPosition] = '\0';
          Uart0_Send(kbInput);
          
          valueA = stringToInt(valueAStr);
          valueB = stringToInt(valueBStr);

          printIntToUart(calculate(valueA, valueB, operationType));

          valueA = 0;
          valueAStr[0] = '0';
          valueAStr[1] = '0';
          valueAStr[2] = '0';
          valueAStr[3] = '\0';
          valueAStrPosition = 0;

          valueB = 0;
          valueBStr[0] = '0';
          valueBStr[1] = '0';
          valueBStr[2] = '0';
          valueBStr[3] = '\0';
          valueBStrPosition = 0;

          calcStep = FIRSTNUM;
        }

        break;
      
      case RESULT:
        if (kbInput == '=') {
          Uart0_Send(kbInput);
          
          valueA = stringToInt(valueAStr);
          valueB = stringToInt(valueBStr);

          printIntToUart(calculate(valueA, valueB, operationType));

          valueA = 0;
          valueAStr[0] = '0';
          valueAStr[1] = '0';
          valueAStr[2] = '0';
          valueAStr[3] = '\0';
          valueAStrPosition = 0;

          valueB = 0;
          valueBStr[0] = '0';
          valueBStr[1] = '0';
          valueBStr[2] = '0';
          valueBStr[3] = '\0';
          valueBStrPosition = 0;

          calcStep = FIRSTNUM;
        }
        break;
    }
  }
}
