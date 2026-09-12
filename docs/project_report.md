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

## 4. Genomförande

### Del 1 – Temperaturstatus

Den första delen var implementationen av `greenhouse_temperature_status()`.

Funktionen klassificerar temperatur enligt följande:

- Under -40 °C eller över 85 °C: `INVALID`
- Under 15 °C: `LOW`
- 15–27 °C: `OK`
- 28–34 °C: `HIGH`
- 35–85 °C: `CRITICAL`

Tre enhetstester skapades för att kontrollera låga och normala temperaturer, höga och kritiska temperaturer samt ogiltiga temperaturvärden.

Arbetet gjordes i branchen `feature/temperature-status` och mergades till `main` genom en Pull Request.

### Del 2 – Bevattningslogik

Den andra delen var implementationen av `greenhouse_should_water()`.

Funktionen returnerar `true` när:

- mätvärdet är giltigt
- markfuktigheten klassificeras som torr

Funktionen returnerar `false` om mätvärdet är ogiltigt eller om marken inte är torr.

Tre enhetstester skapades:

- torr mark returnerar `true`
- mark som inte är torr returnerar `false`
- ogiltigt mätvärde returnerar `false`

Arbetet gjordes i branchen `feature/should-water` och mergades därefter till `main` genom en Pull Request.

## 5. Planerat jämfört med genomfört

Den ursprungliga planen var att implementera två av de funktioner som angavs i uppgiften och skriva 2–3 tester per funktion.

Detta genomfördes enligt plan.

Under arbetet behövde även utvecklingsmiljön konfigureras för ESP-IDF 6.0. Projektet byggdes lokalt med ESP-IDF 6.0.3.

GitHub Actions aktiverades även i det forkade repositoryt. För att kunna köra Wokwi-simuleringen behövde en Wokwi CI-token konfigureras som en GitHub Repository Secret.

Efter konfigurationen kunde hela CI-flödet genomföras.

## 6. MVP-utveckling

Projektets första MVP var den befintliga versionen av växthussystemet där grundläggande sensoravläsning och beslutslogik redan fanns.

MVP:n utvecklades stegvis:

**MVP 1:** Befintligt växthussystem med grundläggande sensor- och beslutslogik.

**MVP 2:** Temperaturklassificering implementerades och testades.

**MVP 3:** Bevattningslogiken implementerades och testades.

**MVP 4:** Projektet verifierades genom hela CI-flödet med bygge, Wokwi-simulering och firmwarebygge.

## 7. Testning

Projektet använder Unity för enhetstester och en separat ESP-IDF-testapplikation i `test_app`.

För de två implementerade funktionerna skapades totalt sex nya testfall.

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

## 8. CI och GitHub Actions

Projektet använder GitHub Actions för automatisk verifiering.

CI-flödet består av:

- bygge av testapplikationen
- körning av tester i Wokwi-simulering
- publicering av testresultat
- bygge av ESP32-C6 firmware

Pre-commit-kontrollen användes även i samband med Pull Requests.

Efter att Wokwi CI hade konfigurerats genomfördes hela workflowet på `main`.

Resultatet blev:

- Build Project: godkänd
- Wokwi Simulation: godkänd
- Firmware Build: godkänd
- 32 av 32 tester godkända
- 0 misslyckade tester

Det visar att den slutliga versionen både kunde byggas och klara projektets automatiserade tester.

## 9. Problem och lösningar

Ett problem under arbetet var att Wokwi-testningen först misslyckades eftersom `WOKWI_CLI_TOKEN` saknades.

Problemet löstes genom att skapa en Wokwi CI-token och lägga till den som en Repository Secret i GitHub.

Efter detta kördes hela CI-workflowet igen och samtliga 32 tester blev godkända.

## 10. Slutsats

Projektets mål uppnåddes genom att två av de efterfrågade funktionerna implementerades och testades.

Arbetet delades upp i feature-branches och ändringarna integrerades genom Pull Requests. Detta gav ett tydligt arbetsflöde där varje del kunde kontrolleras innan den mergades till `main`.

Den slutliga versionen verifierades med GitHub Actions och Wokwi, där samtliga 32 tester godkändes.