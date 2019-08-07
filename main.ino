#include <EEPROMAnything.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>  
#include <FS.h>
#include <ESP8266mDNS.h>

File fsUploadFile;
#define AP_CONTROL_PIN D0;

//////////////////////////////////////

String dns = "9999";
int ssidAP = 9999;
int passwordAP = 99999999;

/////////////////////////////////////
uint32_t ssid;
uint32_t password;
uint32_t ssidFromDevices[5];
uint32_t passwordsFromDevices[5];

uint8_t valueOfDevices;
ESP8266WebServer HTTP(80);
String nesPage;
char* a[8];
char* b[8];

//String getContentType(String filename){                                 // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
//  if (HTTP.hasArg("download")) return "application/octet-stream";       // Если HTTP имеет аргументом "download", то отдаем содержимое файла для скачивания и завершаем выполнение функции 
//  else if (filename.endsWith(".html")) return "text/html";               // Если файл заканчивается на ".htm", то возвращаем заголовок "text/html" и завершаем выполнение функции
//  else if (filename.endsWith(".html")) return "text/html";              // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
//  else if (filename.endsWith(".css")) return "text/css";                // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
//  else if (filename.endsWith(".js")) return "application/javascript";   // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
//  else if (filename.endsWith(".png")) return "image/png";               // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
//  else if (filename.endsWith(".gif")) return "image/gif";               // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
//  else if (filename.endsWith(".jpg")) return "image/jpeg";              // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpeg" и завершаем выполнение функции
//  else if (filename.endsWith(".ico")) return "image/x-icon";            // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
//  else if (filename.endsWith(".xml")) return "text/xml";                // Если файл заканчивается на ".xml", то возвращаем заголовок "text/xml" и завершаем выполнение функции
//  else if (filename.endsWith(".pdf")) return "application/x-pdf";       // Если файл заканчивается на ".pdf", то возвращаем заголовок "application/x-pdf" и завершаем выполнение функции
//  else if (filename.endsWith(".zip")) return "application/x-zip";       // Если файл заканчивается на ".zip", то возвращаем заголовок "application/x-zip" и завершаем выполнение функции
//  else if (filename.endsWith(".gz")) return "application/x-gzip";       // Если файл заканчивается на ".gz", то возвращаем заголовок "application/x-gzip" и завершаем выполнение функции
//  return "text/plain";                                                  // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
//}
//
//bool handleFileRead(String path){                       // Функция работы с файловой системой
//  if(path.endsWith("/")) path += nesPage;           // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
//  String contentType = getContentType(path);            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
//  String pathWithGz = path + ".gz";                     // Заводим еще один путь с адресом заархивированной версии файла из адреса обращения
//  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){ // Если в файловой системе существует заархивированный или простой файл по адресу обращения
//    if(SPIFFS.exists(pathWithGz))                       //  Если существует заархивированный файл,
//      path += ".gz";                                    //    то добавляем в адрес обращения окончание, указывающее на архив
//    File file = SPIFFS.open(path, "r");                 //  Открываем файл для чтения
//    size_t sent = HTTP.streamFile(file, contentType);   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
//    file.close();                                       //  Закрываем файл
//    return true;                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
//  }
//  return false;                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
//}
void HTTP_init() {
  HTTP.on("/init", writeDevices);
  HTTP.on("/brightness ", brightnessControl); 
  HTTP.begin();
}

void brightnessControl() {
  analogWrite(A0, HTTP.arg("brightness").toInt());
}

void writeDevices() {
  EEPROM.begin(512);
  EEPROM_writeAnything(1, HTTP.arg("password").toInt());
  EEPROM_writeAnything(6, HTTP.arg("ssid").toInt());
  EEPROM.end();
  int len = HTTP.arg("len").toInt();
  for (int i = 0; i < len; i++) {
    EEPROM.begin(512);
    EEPROM_writeAnything(100 + i * 5, HTTP.arg("device" + String(i)).toInt());
    EEPROM_writeAnything(150 + i * 5, HTTP.arg("password" + String(i)).toInt());
    EEPROM.end();
  }
}
void ApMode() {
  SPIFFS.begin();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(itoa(ssidAP, a[8], DEC), itoa(passwordAP, b[8], DEC));
  nesPage
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder started");
  }
  HTTP_init();
}

void clientMode () {
    EEPROM.begin(512);
    EEPROM_readAnything(100, ssid);
    EEPROM_readAnything(102, password);
    EEPROM.end();
    WiFi.mode(WIFI_STA);
    WiFi.begin(itoa(ssid, a[8], DEC), itoa(password, b[8], DEC));
    while(WiFi.status() != WL_CONNECTED) {
      Serial.println('.');
    }
    HTTP_init();
}

void apOrClient() {
  if (digitalRead(AP_CONTROL_PIN)) {
    ApMode();
  } else {
    clientMode();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(AP_CONTROL_PIN, INPUT);
  pinMode(A0, OUTPUT);
  apOrClient;
}

void loop() {
  
}

