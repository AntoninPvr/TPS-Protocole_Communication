// index.h
/* Lors du démarrage du microcontroleur une procédure copie 
 * les variables utiles de la mémoire programme vers la RAM. 
 * Afin d'économiser la place dans la RAM on peut utiliser 
 * le mot clef PROGMEM afin d'inciter la procédure deboot à 
 * garder en mémoire programme les variables ne changeant pas
 * 
 * exemple pour une array de character:
 * const char MAIN_page[] PROGMEM = R"=====( )=====";
 */
 
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <body>    
        <h1>ESP32 Sensor Station Server</h1>
        <p> Binome: Antonin Pivard </p>
        <p>Heure: XXX </p>
        <p> Nombre de capteur: 2</p>
        <h2> Pages disponibles </h2>
        <br><a href="ADC"> ADC Esp32 </a>
        <br><a href="MCP0"> MCP9808_0 </a>
        <br><a href="MCP1"> MCP9808_1 </a>
    </body>
</html>
)=====";

const char MesureADC[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <body>    
        <h1>ESP32 Sensor Station Server</h1>
        <p> Binome: Antonin Pivard </p>
        <p>Capteur: MCP9808</p>
        <p> Nombre de capteur: 2</p>
        <h2> Mesure ADC </h2>
        <br> ADC: <span class="reading"></span> </br>

        <script>
        setInterval( function(){getData();}, 2000);
        function getData()
        {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200)
        {
        var count;
        var num_an = this.responseXML.getElementsByTagName('reading').length;
        for (count = 0; count < num_an; count++)
        {
        document.getElementsByClassName("reading")[count].innerHTML =
        this.responseXML.getElementsByTagName('reading')[count].childNodes[0].nodeValue;
        }
        }
        };
        xhttp.open("GET", "readADC", true);
        xhttp.send();
        }
    </script>
    </body>
</html>
)=====";

const char MesureMCP0[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <body>    
        <h1>ESP32 Sensor Station Server</h1>
        <p> Binome: Antonin Pivard </p>
        <p>Capteur: MCP9808</p>
        <p> Nombre de capteur: 2</p>
        <h2> Mesure MCP9808 </h2>
        <p> ADC_0: XX </p>
    </body>
</html>
)=====";

const char MesureMCP1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <body>    
        <h1>ESP32 Sensor Station Server</h1>
        <p> Binome: Antonin Pivard </p>
        <p>Capteur: MCP9808</p>
        <p> Nombre de capteur: 2</p>
        <h2> Mesure MCP9808 </h2>
        <p> ADC_1: XX </p>
    </body>
</html>
)=====";