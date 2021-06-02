# **PRACTICA 5 : Buses de comunicación II (SPI)**

El objetivo de esta práctica es comprender los buses de comunicación SPI. Para ello vamos a usar uno de los componentes que suelen usar este tipo de bus, como por ejemplo la lectura y escritura de tarjetas de memoria SD.

El código usado para la práctica lo tuve que alargar un poco más para poder comprovar el funcionamiento del código.

## ** 1) void setup() **

```cs

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

```
Lo primero que hacemos es importar dos librerías para facilitar esta comunicación que deseamos establecer.
-- SPI.h  
-- SD.h
Lo siguiente que hacemos es crear un objeto de la clase *File* llamado *myFile* con este vamos a poder abrir y cerrar el fichero de la SD para poder leerlo o escribir en él.

Ahora si que entramos dentro de la función setup y establecemos el baud rate a 9600 en mi caso.
Imprimimos por pantalla que se esta inciando la SD.
Ahora inicializamos el us SPI con la función SPI.begin(), en este begin le ponemos según este orden los puertos usados:
*begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)*
Si no le pasaramos unos parámetros válidos, si vamos a la función podemos ver que se establecen unos por defecto, los cuáles son los más recomendados .

``` cs
void SPIClass::begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
{
    if(_spi) {
        return;
    }

    if(!_div) {
        _div = spiFrequencyToClockDiv(_freq);
    }

    _spi = spiStartBus(_spi_num, _div, SPI_MODE0, SPI_MSBFIRST);
    if(!_spi) {
        return;
    }

    if(sck == -1 && miso == -1 && mosi == -1 && ss == -1) {
        _sck = (_spi_num == VSPI) ? SCK : 14;
        _miso = (_spi_num == VSPI) ? MISO : 12;
        _mosi = (_spi_num == VSPI) ? MOSI : 13;
        _ss = (_spi_num == VSPI) ? SS : 15;
    } else {
        _sck = sck;
        _miso = miso;
        _mosi = mosi;
        _ss = ss;
    }

    spiAttachSCK(_spi, _sck);
    spiAttachMISO(_spi, _miso);
    spiAttachMOSI(_spi, _mosi);

}

```
Vemos que si todos los pines pasados por parámetro fueran *-1* les asignaría él mismo su valor.
Si no es así, al ser un constructor, le pondría el valor pasado a las variables de la clase.

Lo siguiente que hacemos es incicializar la clase SD.
La función para entender lo que hace haría falta meterse muy a dentro de la clase SD y SPI, por lo que yo no lo voy a explicar pero si cabe decir que es una función booleana que retorna false o true en función de si ha podido montar e inicializar la sd. Si no fuera así nos retornaría un *false* y por eso en el void setup hacemos ese condicional de si nos retornara un false que avise sobre que no se ha podido inicializar la SD.

  Una vez inicializada miramos si se encuentra algún fichero en la tarjeta con la función SD.exists('/nombre_archivo'). 
  Ahora que ya todo funciona y se ha podido localizar el archivo, procedemos a abrir y leerlo.
    
    Primero de todo lo que hago está por si el archivo no existiera, si ya se tiene un archivo guardado no haría falta hacer lo siguiente:
```cs
    myFile=SD.open("/archivo.txt",FILE_WRITE);
    myFile.println("Hola mundo");
    myFile.close();

```
Primero lo abrimos y ponemos que vamos a escribir en él. Escribimos de una forma muy conocida, una función llamada prinln("message").
Y finalmente lo cerramos.

Ahora nos interesa leer lo que hay dentro de él:
Primero lo abrimos, comprovamos que todo va bien y mientras esté disponible, vamos a escribir en el monitor serial lo que haya en el fichero con Serial.write(myFile.read()).
Vemos que son funciones muy cómodas ya que se asemejan mucho a otras usadas para leer, escribir y mirar si hay información para leer mediante el puerto serial.
Finalmente una vez leído, myFile.available() se vuelve false por lo que salimos del bucle y lo cerramos.
  
  En este caso no vamos a usar el void loop(), ya que no queremos leer sin parar el archivo de la SD.

  ## **CONCLUSIONES**
Para hacer esta práctica tuve algunos problemas a causa de la tarjeta SD. Tuve que hacer este código de crear un fichero ya que cuando lo creaba desde Windows no siempre me lo lograba leer a causa de la incopatibilidad de formato de archivos.
Tuve que resetear varias veces la tarjeta SD ya que windows cuando intenta formatear no siempre lo hace del todo si no que deja cosas en la memoria.
Finalmente tras algunos intentos pude conseguir leer el archivo y mostrarlo por pantalla.

### **MEJORAS**
Una vez hecho esta práctica me puse a investigar un poco más sobre esta librería y vi que se pueden hacer muchas mas cosas que no solamente leer y escribir ficheros, si no que se puede crear directorios  y renombrar los archivos... Esto mezclado con otras librerías como por ejemplo la de anteriores prácticas para hacer un servidor con la misma ESP32, podría ser útil para crear un servidor de ficheros para poder acceder desde la misma red local.
Son cosas a plantearse para futuros proyectos!
