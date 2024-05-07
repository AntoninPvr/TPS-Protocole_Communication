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
        <p>Heure: XXX </p>
        <br><a href="/hello"> Lien </a>
    </body>
</html>
)=====";
