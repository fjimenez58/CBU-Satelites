// Variables globales
float escalarVoltaje = 2.0;  // Escala el voltaje medido
float error = 0.33;          // Corrección del valor de voltaje
float voltajeMaximo = 4.2;   // Voltaje máximo (100% batería)
float voltajeMinimo = 3.0;   // Voltaje mínimo (0% batería)

void setup() {
  // Inicia comunicación serial
  Serial.begin(115200);
  
  // Configura el pin 34 como entrada de la ESP32
  pinMode(34, INPUT);
}

void loop() {
  // Lee valor analógico del pin 34
  int valorPin34 = analogRead(34);
  
  // Convierte el valor a voltaje (0 - 3.3V), que es un voltaje adecuado para el pin 34
  float voltaje = (valorPin34 * 3.3) / 4095.0;
  
  // Escala el voltaje y corrige el error para obtener el volatje real
  float voltajeEscalado = (voltaje * escalarVoltaje) + error;

  // Muestra el voltaje en el monitor serial
  Serial.print("Voltaje de la batería: ");
  Serial.println(voltajeEscalado);

  // Espera 500 ms
  delay(500);
}
