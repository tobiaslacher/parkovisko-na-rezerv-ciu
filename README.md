Inteligentné parkovisko

Terézia Tomášová: hardwér

Jakub Zádrapa: softvér

Tobias Lacher: dokumentácia

Cieľ projektu: Cieľom projektu je zefektívniť parkovanie, znížiť čas potrebný na hľadanie voľného miesta a zlepšiť organizáciu dopravy. 
Inteligentné parkovisko je systém navrhnutý na monitorovanie a správu parkovacích miest v reálnom čase. Systém poskytuje používateľovi 
prehľad o aktuálnom stave parkoviska, napríklad:

 - koľko áut sa momentálne nachádza na parkovisku,
 - koľko áut prešlo vstupnou/výstupnou rampou,
 - koľko parkovacích miest je voľných,
 - aktuálnu obsadenosť parkoviska.

Použitá platforma: ESP32, arduino rozhranie

Použité komponenty: ESP32,  nepájivé pole, prepojovacie vodiče, senzor pohybu, OLED displej, LED diody, Servo, IR senzory infračervené

Princíp činnosti: 
Systém je riadený mikrokontrolérom ESP32, ktorý je pripojený na Wi-Fi sieť a neustále monitoruje IR senzory umiestnené pri vstupe a výstupe parkoviska.

Pri zaznamenaní vozidla vstupným senzorom systém vyhodnotí dostupnosť voľných parkovacích miest. Ak je miesto voľné, ESP32 vyšle signál servomotoru, ktorý otvorí rampu. Zároveň sa zníži počet voľných miest, aktualizuje sa údaj na OLED displeji a zmena sa odošle na webovú stránku, kde sa zobrazí aktuálny stav parkoviska v reálnom čase.

Pri odchode vozidla výstupný senzor zaznamená pohyb, rampa sa opäť otvorí, počet voľných miest sa zvýši a systém aktualizuje všetky zobrazené informácie.

Po uplynutí nastaveného časového intervalu sa rampa automaticky zatvorí. Celý proces prebieha automaticky bez potreby manuálneho zásahu používateľa.

Význam projektu: Projekt inteligentného parkoviska má praktické využitie v moderných mestách, obchodných centrách, univerzitách či administratívnych budovách.

Jeho význam spočíva v:

znížení dopravných zápch pri hľadaní parkovania,
úspore času vodičov,
efektívnejšom využívaní parkovacích kapacít,
podpore konceptu smart city,
automatizácii a digitalizácii parkovacích systémov.

Projekt zároveň demonštruje možnosti využitia IoT technológií v reálnych podmienkach.

Možnosti rozšírenia: 
1. Rezervácia na konkrétny časový interval
2. Mapa voľných miest v reálnom čase
3. Automatické prideľovanie najlepšieho miesta
4. Automatické rozpoznávanie ŠPZ
