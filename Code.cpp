// Объявление используемых пинов
const int leftSensorPin = 5;    // Пин левого фоторезистора
const int rightSensorPin = 4;   // Пин правого фоторезистора
const int servoPin = 11;        // Пин для подключения сервопривода

// Переменные для хранения значений с фоторезисторов и ошибки
int leftSensorValue = 0;
int rightSensorValue = 0;
int error = 0;
int errorAverage = 0;

// Параметры для настройки сервопривода
const int minPosition = 5;
const int maxPosition = 150;
const int initialPosition = 45;

// Объект для управления сервоприводом
Servo servo;

void setup() {
  // Настройка порта Serial для вывода отладочной информации
  Serial.begin(9600);

  // Подключение сервопривода к соответствующему пину
  servo.attach(servoPin);

  // Установка сервопривода в начальную позицию для калибровки
  Serial.println("Калибровка сервопривода");
  servo.write(initialPosition);
  delay(5000); // Пауза для завершения калибровки

  // Вывод информации о готовности к работе
  Serial.println("Готов к работе");
}

void loop() {
  // Чтение значений с фоторезисторов
  leftSensorValue = analogRead(leftSensorPin);
  rightSensorValue = analogRead(rightSensorPin);

  // Расчет ошибки
  error = leftSensorValue - rightSensorValue;
  errorAverage = (errorAverage + error) / 2;

  // Вычисление новой позиции сервопривода
  int newOutput = initialPosition + getTravel();

  // Проверка на выход за границы диапазона движения сервопривода
  if (newOutput > maxPosition) {
    newOutput = maxPosition;
  } else if (newOutput < minPosition) {
    newOutput = minPosition;
  }

  // Управление сервоприводом
  servo.write(newOutput);

  // Пауза для стабилизации и уменьшения нагрузки на процессор
  delay(100);
}

// Функция вычисления направления движения сервопривода
int getTravel() {
  // Возвращает -1 для поворота влево, 1 для поворота вправо, 0 для остановки

  if (errorAverage < (-1 * deadband)) {
    return 1;
  } else if (errorAverage > deadband) {
    return -1;
  } else {
    return 0;
  }
}
