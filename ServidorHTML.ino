// Librerias
#include <WiFi.h>

// Configuramos el servidor web en el puerto 80
WiFiServer server(80);

// Nombre y contraseña de la red WiFi a la que nos vamos a conectar
const char* ssid = "Oficina_AIA"; 
const char* password = "IIND.andes2023"; 

// Variables para manejar el estado de la comunicación
String header;    // Almacena los datos recibidos desde la página web
String estadoSalida = "off";  // Estado de la salida: encendida (on) o apagada (off)
String mensajeSerial = "";  // Mensaje recibido desde el serial
bool nuevoMensaje = false;  // Indica si hay un nuevo mensaje desde el serial
const int salida = 2;   // Pin al que está conectado un LED o dispositivo que se puede encender/apagar

// Página web simple que muestra la información y botones
String pagina = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8' />"
                "<title>CBU - Diseño de Sistemas Satelitales</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; text-align: center; padding: 20px; background-color: #f0f8ff; }"
                "h1 { color: #005b96; }"
                "p { font-size: 20px; }"
                "button { background-color: #4CAF50; color: white; border: none; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px; cursor: pointer; border-radius: 5px; }"
                "button:hover { background-color: #45a049; }"
                "</style>"
                "<script>"
                "function obtenerMensajeSerial() {"
                "  fetch('/serial').then(response => response.text()).then(data => {"
                "    if (data !== '') {"
                "      document.getElementById('mensajeSerial').innerText = data;"
                "    }"
                "  }).catch(error => console.error('Error:', error));"
                "}"
                "setInterval(obtenerMensajeSerial, 2000);" // Verificar cada 2 segundos
                "</script>"
                "</head>"
                "<body onload='obtenerMensajeSerial();'>"
                "<center>"
                "<h1>CBU - Diseño de Sistemas Satelitales</h1>"
                "<p>Laboratorio 2</p>"
                "<p>Grupo 1</p>"
                "<p>Leffer Trochez</p>"
                "<form action='/send' method='POST'>"
                "<input type='text' name='message' placeholder='Ingrese un mensaje'>"
                "<button type='submit'>Enviar</button>"
                "</form>"
                "<p><a href='/on'><button>ENCENDER</button></a></p>"
                "<p><a href='/off'><button>APAGAR</button></a></p>"
                "<h3>Comunicación CubeSat - Estación en Tierra:</h3>"
                "<p id='mensajeSerial'></p>"
                "</center>"
                "</body>"
                "</html>";

void setup() {
  // Inicia la comunicación serial para enviar y recibir datos
  Serial.begin(115200);
  
  // Configura el pin de salida como salida y lo apaga
  pinMode(salida, OUTPUT);
  digitalWrite(salida, LOW);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Muestra puntos en la consola mientras intenta conectarse
  }

  // Una vez conectado, muestra la IP en la consola serial
  Serial.println("----------------------------------------------------");
  Serial.println("WiFi conectado exitosamente");
  Serial.println("INICIA LA COMUNICACIÓN CUBESAT - ESTACIÓN TERRENA");
  Serial.println(WiFi.localIP());

  // Inicia el servidor web
  server.begin();
}

void loop() {
  // Manejo de la conexión con la página web
  WiFiClient client = server.available(); 
  if (client) { 
    String currentLine = "";
    String postData = "";  
    bool requestProcessed = false;

    while (client.connected()) { 
      if (client.available()) { 
        char c = client.read(); 
        header += c;

        if (c == '\n' && currentLine.length() == 0) {
          // Verifica si se ha recibido un comando desde la página web
          if (header.indexOf("GET /on") >= 0) {
            digitalWrite(salida, HIGH);  // Enciende el dispositivo conectado
            estadoSalida = "on";
          } else if (header.indexOf("GET /off") >= 0) {
            digitalWrite(salida, LOW);  // Apaga el dispositivo conectado
            estadoSalida = "off";
          } else if (header.indexOf("POST /send") >= 0) {
            // Manejo de un mensaje enviado desde la página web
            while (client.available()) {
              char p = client.read();
              postData += p;
            }

            int messageIndex = postData.indexOf("message=");
            if (messageIndex >= 0) {
              String message = postData.substring(messageIndex + 8);
              message = urlDecode(message);
              Serial.print("Mensaje recibido desde la página web (estación terrena): ");
              Serial.println(message);
              mensajeSerial = message;
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println(pagina);
            requestProcessed = true;
            break;

          } else if (header.indexOf("GET /serial") >= 0) {
            // Envía el mensaje serial a la página web
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println("Connection: close");
            client.println();
            client.println(mensajeSerial);
            requestProcessed = true;
            break;

          } else {
            // Enviar la página principal
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println(pagina);
            requestProcessed = true;
            break;
          }
        }

        if (c == '\n') {
          currentLine = "";
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    if (!requestProcessed) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println();
      client.println(pagina);
    }

    header = "";

    client.stop();
  }

  // Leer cualquier mensaje recibido desde la consola serial
  if (Serial.available() > 0) {
    mensajeSerial = Serial.readStringUntil('\n');
    nuevoMensaje = true;
    Serial.print("Mensaje enviado desde Serial (Cubesat): ");
    Serial.println(mensajeSerial);
  }
}

// Función para decodificar una cadena URL
String urlDecode(String input) {
  String decoded = "";
  char temp;
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == '+') {
      decoded += ' ';
    } else if (input[i] == '%') {
      temp = (input.substring(i + 1, i + 3)).toInt();
      decoded += char(strtol(input.substring(i + 1, i + 3).c_str(), NULL, 16));
      i += 2;
    } else {
      decoded += input[i];
    }
  }
  return decoded;
}
