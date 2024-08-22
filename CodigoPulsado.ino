// La función setup() se ejecuta una vez, al inicio, cuando se enciende o reinicia la placa ESP32.
// Aquí es donde configuramos los pines (conexiones) de la placa.
void setup() {
  // Configuramos el pin 0 del Arduino como entrada (INPUT) mediante la funcion pinMode().
  // Esto significa que vamos a conectar un botón a este pin y leer si está presionado o no.
  pinMode(0, INPUT);
  // Configuramos el pin 2 del Arduino como salida (OUTPUT).
  // Esto significa que vamos a conectar un LED a este pin y controlarlo (encenderlo o apagarlo).
  pinMode(2, OUTPUT);
}
// La función loop() se ejecuta repetidamente, una y otra vez, mientras la placa esté encendida.
// Es el "cerebro" del programa que revisa el estado del botón y enciende o apaga el LED según corresponda.
void loop() {
  // Declaramos una variable llamada Push_button_state para almacenar el estado del botón.
  // Luego, leemos el valor del pin 0 (donde está conectado el botón) y lo guardamos en Push_button_state.
  // Si el botón está presionado, el valor será LOW.
  // Si el botón no está presionado, el valor será HIGH.
  int Push_button_state = digitalRead(0);
  // Ahora, comprobamos si el botón está presionado.
  if (Push_button_state == HIGH) {
    // Si el botón está presionado, encendemos el LED.
    // Lo hacemos enviando un valor LOW (alto, 0) al pin 2.
    digitalWrite(2, LOW);
  } else {
    // Si el botón no está presionado, apagamos el LED.
    // Lo hacemos enviando un valor HIGH (bajo, 1) al pin 2, que controla el LED.
    digitalWrite(2, HIGH);
  }
}
