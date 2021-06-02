
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


File myFile;

void setup()
{

  Serial.begin(9600);
  Serial.print("Iniciando SD ...");
  SPI.begin(18,19,23,5);

  
  if (!SD.begin(5)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
 
 //Comrpovamos si hay un fichero en la tarjeta
 if (SD.exists("/archivo.txt")) {
    Serial.println("example.txt exists.");
  } 

  else {
    Serial.println("archivo.txt doesn't exist.");
  }
  // Abrimos el fichero para escribir en el, si existe no habrá problema, si no existe lo creará.
  myFile = SD.open("/archivo.txt", FILE_WRITE);
  myFile.close();

  if (SD.exists("/archivo.txt")) {

    
    Serial.println("archivo.txt exists.");
  } 
  else {
    Serial.println("archivo.txt doesn't exist.");
  }

  //Escribimos en el fichero
  myFile=SD.open("/archivo.txt",FILE_WRITE);
  myFile.println("Hola mundo");
  myFile.close();
  //Volvemos a abrir el fichero para mostrar lo que hay dentro
  myFile=SD.open("/archivo.txt");
  if (myFile) {
    Serial.println("archivo.txt:");
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    myFile.close(); //cerramos el archivo
  }
  
  else {
    Serial.println("Error al abrir el archivo");
  }
}

void loop()
{
  
}