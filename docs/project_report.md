# Projektrapport – Växthussystem

## 1. Projektöversikt

Projektet är ett inbyggt växthussystem utvecklat med ESP-IDF. Systemet läser temperatur och markfuktighet och använder mätvärdena för att avgöra om växthuset ska vara inaktivt, vattna eller larma.

Projektet innehöll redan en grundläggande implementation. Målet med kompletteringsuppgiften var att komplettera delar av logiken, skriva enhetstester och använda GitHub Actions för kontinuerlig integration.

Jag valde att implementera följande två funktioner:

- `greenhouse_temperature_status()`
- `greenhouse_should_water()`

Jag kompletterade även projektet med tester för funktionerna.

## 2. Arbetssätt

Arbetet genomfördes med ett agilt arbetssätt där uppgiften delades upp i mindre delar. Varje funktion utvecklades separat i en egen feature-branch.

Arbetsflödet var:

1. Skapa en feature-branch från `main`.
2. Implementera funktionen.
3. Skriva enhetstester.
4. Bygga testapplikationen.
5. Göra commit och pusha till GitHub.
6. Skapa en Pull Request.
7. Kontrollera GitHub Actions.
8. Merga ändringen till `main`.

Detta gjorde det möjligt att utveckla och kontrollera en funktion i taget.

## 3. Sprint 0 – Planering

Under Sprint 0 analyserades projektets struktur och uppgiftens krav.

Följande aktiviteter planerades:

- Installera och konfigurera ESP-IDF 6.0.
- Bekanta mig med projektets komponentstruktur.
- Identifiera vilka funktioner som skulle implementeras.
- Undersöka befintliga tester.
- Kontrollera hur GitHub Actions och Wokwi används.
- Planera implementation och tester.

Jag valde att arbeta med `greenhouse_temperature_status()` och `greenhouse_should_water()` eftersom de är tydliga logiska funktioner som kan testas oberoende av fysisk hårdvara.

Målet var att stegvis utveckla den befintliga lösningen och efter varje del kontrollera att projektet fortfarande kunde byggas och integreras.

## 4. Genomförande

### Vecka 1 – Projektstart och temperaturstatus

Under den första veckan började jag med att gå igenom projektets struktur, uppgiftens krav och de befintliga testerna. Utvecklingsmiljön konfigurerades med ESP-IDF 6.0.3 och projektet byggdes för att kontrollera att miljön fungerade.

Därefter implementerades `greenhouse_temperature_status()`.

Funktionen klassificerar temperaturen enligt följande:

- Under -40 °C eller över 85 °C: `INVALID`
- Under 15 °C: `LOW`
- 15–27 °C: `OK`
- 28–34 °C: `HIGH`
- 35–85 °C: `CRITICAL`

Tre enhetstester skapades för att kontrollera låga och normala temperaturer, höga och kritiska temperaturer samt ogiltiga temperaturvärden.

Arbetet genomfördes i branchen `feature/temperature-status`. Testapplikationen byggdes och ändringarna pushades därefter till GitHub. En Pull Request skapades och ändringarna integrerades till `main`.

### Vecka 2 – Bevattningslogik, CI och slutförande

Under den andra veckan implementerades `greenhouse_should_water()`.

Funktionen kontrollerar först att mätvärdet är giltigt. Om mätvärdet är ogiltigt returneras `false`. Om mätvärdet är giltigt används `greenhouse_soil_status()` för att kontrollera markfuktigheten. Funktionen returnerar `true` endast när marken klassificeras som torr.

Tre enhetstester skapades:

- torr mark returnerar `true`
- mark som inte är torr returnerar `false`
- ogiltigt mätvärde returnerar `false`

Arbetet genomfördes i branchen `feature/should-water`. Testapplikationen byggdes lokalt med ESP-IDF 6.0.3 och därefter pushades ändringarna till GitHub.

En Pull Request skapades och GitHub Actions användes för att kontrollera ändringarna innan de mergades till `main`.

Den sista delen av arbetet bestod av att konfigurera och verifiera projektets fullständiga CI-flöde. En Wokwi CI-token lades till som en GitHub Repository Secret och hela GitHub Actions-workflowet kördes på `main`.

Slutresultatet blev att projektet byggdes korrekt och samtliga 32 tester godkändes i Wokwi.

## 5. Planerat jämfört med genomfört

Den ursprungliga planen var att implementera två av de funktioner som angavs i uppgiften och skriva 2–3 tester per funktion.

Det planerade arbetet genomfördes:

| Planerat arbete | Resultat |
|---|---|
| Konfigurera utvecklingsmiljön | Genomfört |
| Undersöka befintlig kod och tester | Genomfört |
| Implementera temperaturstatus | Genomfört |
| Skriva 2–3 tester för temperaturstatus | 3 tester skapades |
| Implementera bevattningslogik | Genomfört |
| Skriva 2–3 tester för bevattningslogik | 3 tester skapades |
| Kontrollera befintliga soil-status-tester | Genomfört |
| Bygga testapplikationen lokalt | Genomfört |
| Arbeta med feature-branches | Genomfört |
| Använda Pull Requests | Genomfört |
| Verifiera projektet med CI | Genomfört |
| Köra tester i Wokwi | 32 av 32 godkända |

Under arbetet behövde utvecklingsmiljön konfigureras för ESP-IDF 6.0. Projektet byggdes lokalt med ESP-IDF 6.0.3.

GitHub Actions aktiverades även i det forkade repositoryt. För att kunna köra Wokwi-simuleringen behövde en Wokwi CI-token konfigureras som en GitHub Repository Secret.

Efter konfigurationen kunde hela CI-flödet genomföras.

## 6. MVP-utveckling

Projektets första MVP var den befintliga versionen av växthussystemet där grundläggande sensoravläsning och beslutslogik redan fanns.

MVP:n utvecklades stegvis:

**MVP 1:** Befintligt växthussystem med grundläggande sensor- och beslutslogik.

**MVP 2:** Temperaturklassificering implementerades och testades.

**MVP 3:** Bevattningslogiken implementerades och testades.

**MVP 4:** Projektet verifierades genom hela CI-flödet med bygge, Wokwi-simulering och firmwarebygge.

På detta sätt utvecklades lösningen stegvis i stället för att alla förändringar gjordes samtidigt.

## 7. Testning

Projektet använder Unity för enhetstester och en separat ESP-IDF-testapplikation i `test_app`.

För de två funktioner som jag implementerade skapades totalt sex nya testfall.

De befintliga testerna för `greenhouse_soil_status()` kontrollerar bland annat gränsvärden för torr, normal och våt mark samt ogiltiga fuktvärden.

### Testmatris

| Funktion | Test | Förväntat resultat |
|---|---|---|
| `greenhouse_temperature_status()` | 10 °C | LOW |
| `greenhouse_temperature_status()` | 22 °C | OK |
| `greenhouse_temperature_status()` | 28 °C | HIGH |
| `greenhouse_temperature_status()` | 35 °C | CRITICAL |
| `greenhouse_temperature_status()` | -41 °C | INVALID |
| `greenhouse_temperature_status()` | 86 °C | INVALID |
| `greenhouse_should_water()` | 22 °C, 20 % fukt | true |
| `greenhouse_should_water()` | 22 °C, 50 % fukt | false |
| `greenhouse_should_water()` | 22 °C, -1 % fukt | false |
| `greenhouse_soil_status()` | 0 och 34 % | DRY |
| `greenhouse_soil_status()` | 35 och 70 % | OK |
| `greenhouse_soil_status()` | 71 och 100 % | WET |
| `greenhouse_soil_status()` | -1 och 101 % | INVALID |

De två nya funktionerna arbetar med redan tillgängliga värden och behöver därför inte direkt använda projektets fake ADC-komponenter. Fake ADC-komponenterna används däremot av projektets befintliga tester för sensoravläsning.

## 8. CI och GitHub Actions

Projektet använder GitHub Actions för automatisk verifiering.

CI-flödet består av:

- bygge av testapplikationen
- körning av tester i Wokwi-simulering
- publicering av testresultat
- bygge av ESP32-C6 firmware

Efter att Wokwi CI hade konfigurerats genomfördes hela workflowet på `main`.

Resultatet blev:

- Build Project: godkänd
- Wokwi Simulation: godkänd
- Firmware Build: godkänd
- 32 av 32 tester godkända
- 0 misslyckade tester

Det visar att den slutliga versionen både kunde byggas och klara projektets automatiserade tester.

## 9. Problem och lösningar

Under projektet uppstod några problem som behövde lösas.

Ett problem var att Wokwi-testningen först inte kunde starta eftersom `WOKWI_CLI_TOKEN` saknades.

Problemet löstes genom att skapa en Wokwi CI-token och lägga till den som en Repository Secret i GitHub.

Efter detta kördes hela CI-workflowet igen och samtliga 32 tester blev godkända.

Arbetet med separata feature-branches gjorde det också möjligt att kontrollera förändringar innan de integrerades i `main`.

## 10. Slutsats

Projektets mål uppnåddes genom att två av de efterfrågade funktionerna implementerades och testades.

Totalt skapades sex nya testfall för de två implementerade funktionerna. De befintliga testerna för `greenhouse_soil_status()` behölls och ingick tillsammans med projektets övriga tester i den slutliga testkörningen.

Arbetet delades upp över två arbetsveckor och genomfördes stegvis med feature-branches och Pull Requests.

Den slutliga versionen verifierades genom GitHub Actions och Wokwi. Resultatet blev 32 av 32 godkända tester och ett godkänt firmwarebygge.
