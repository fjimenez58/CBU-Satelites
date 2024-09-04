#include <TinyGPS++.h>  // Esta librería nos ayuda a trabajar con el GPS
#include <HardwareSerial.h>  // Esta librería nos permite usar más puertos seriales en la ESP32

// Crear el objeto que manejará el GPS y configurar el segundo puerto serial de la ESP32
TinyGPSPlus gps;  // Creamos un objeto para manejar el GPS
HardwareSerial ss(1);  // Usamos el segundo puerto serial de la ESP32 (UART1)

// Función que se ejecuta una vez al inicio
void setup() {
  // Iniciar la comunicación serial para ver información en la computadora
  Serial.begin(115200);  // Velocidad de 115200 baudios para la depuración en el Monitor Serial

  // Configurar la comunicación con el GPS
  ss.begin(9600, SERIAL_8N1, 16, 17);  // Conectamos el GPS a 9600 baudios, con RX en el pin 16 y TX en el pin 17

  // Enviar un mensaje al Monitor Serial para saber que todo empezó bien
  Serial.println("ESP32 con GPS GT-U7 iniciado");
}

// Función que se repite una y otra vez
void loop() {
  // Verificamos si hay datos nuevos del GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());  // Leemos y procesamos los datos que vienen del GPS
  }

  // Si la ubicación se ha actualizado, mostramos la información
  if (gps.location.isUpdated()) {
    // Guardamos la latitud y longitud
    double latitude = gps.location.lat();  // Guardar la latitud
    double longitude = gps.location.lng();  // Guardar la longitud

    // Mostrar la latitud en la pantalla de la computadora
    Serial.print("Latitud: ");
    Serial.println(latitude, 6);  // Mostramos la latitud con 6 decimales

    // Mostrar la longitud en la pantalla de la computadora
    Serial.print("Longitud: ");
    Serial.println(longitude, 6);  // Mostramos la longitud con 6 decimales

    // Mostrar la altitud (qué tan alto estamos sobre el nivel del mar)
    Serial.print("Altitud: ");
    Serial.println(gps.altitude.meters());  // La altitud en metros

    // Mostrar cuántos satélites están conectados al GPS
    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());  // Número de satélites conectados

    // Mostrar el HDOP, que nos dice qué tan precisa es nuestra ubicación
    Serial.print("HDOP: ");
    Serial.println(gps.hdop.value());  // El valor de precisión

    // Crear un enlace para ver la ubicación en Google Maps
    Serial.print("Google Maps: https://www.google.com/maps/search/?api=1&query=");
    Serial.print(latitude, 6);  // Añadimos la latitud al enlace
    Serial.print(",");  
    Serial.println(longitude, 6);  // Añadimos la longitud al enlace
  } else {
    // Si no tenemos una buena señal del GPS, mostramos un mensaje
    Serial.println("Esperando señal GPS ...");
  }

  delay(1000);  // Esperar un segundo antes de intentar leer de nuevo
}
