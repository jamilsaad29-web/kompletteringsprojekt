# ESP-IDF-projektmall med CI och Wokwi-simulering

Här finns en kort beskrivning av hur projektets GitHub Actions-workflow är uppbyggt och hur du kan anpassa det för bygge, enhetstester och Wokwi-simulering.

## Projektets Struktur

Det här repositoryt följer projektstrukturen som beskrivs i avsnittet [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) i ESP-IDF Programming Guide.

Exempel på struktur:

```sh
├── .github
│   └── workflows
├── components    # komponenter i applikationen
├── main          # huvudkomponent i applikationen
├── test_app      # separat testapplikation
└── CMakeLists.txt
```

Applikationskoden ligger i [components](../components/) och [main](../main). Tester körs via [test_app](../test_app/), som bygger de komponenter som ska testas.

I [test_app/CMakeLists.txt](../test_app/CMakeLists.txt) anges vilka komponenter som ska ingå i testbygget:

```cmake
set(EXTRA_COMPONENT_DIRS ../components)
set(TEST_COMPONENTS "greenhouse_monitor;adc_hal;adc_moisture_sensor;adc_temperature_sensor;fake_adc_moisture_sensor;fake_adc_temperature_sensor" CACHE STRING "List of components to test" FORCE)
```

`EXTRA_COMPONENT_DIRS` gör komponenterna i projektet tillgängliga för testapplikationen. `TEST_COMPONENTS` väljer vilka komponenters `test`-mappar som ska byggas och köras.

## Huvudworkflow (`ci_workflow.yml`)

Workflowet som styr CI-flödet finns i `.github/workflows/ci_workflow.yml`.

Det kan startas på tre sätt:

- schemalagt, en gång per dag
- vid Pull Requests
- manuellt via GitHub Actions

Workflowet består av tre delar:

```yml
jobs:
  build_project:
    name: Build Project
    uses: ./.github/workflows/build_esp_app.yml

  test_in_wokwi:
    name: Test Project (Wokwi Simulation)
    uses: ./.github/workflows/test_in_wokwi.yml
    needs: build_project

  build_firmware_artifact:
    name: Build ESP32-C6 Firmware Artifact
    uses: ./.github/workflows/build_firmware_artifact.yml
    needs: test_in_wokwi
```

`build_project` bygger testapplikationen. `test_in_wokwi` kör testerna i Wokwi-simulering. `build_firmware_artifact` bygger den vanliga firmware-binären efter att Wokwi-testet har lyckats.

Det finns också separata workflows för pre-commit och PR-kontroller:

- `.github/workflows/pre-commit.yml` kör `pre-commit run --all-files`.
- `.github/workflows/dangerjs.yml` kör en PR-linter.

## Bygg Testapplikationen (`build_esp_app.yml`)

Workflowet `.github/workflows/build_esp_app.yml` bygger [test_app](../test_app/) med ESP-IDF.

Det använder [esp-idf-ci-action](https://github.com/espressif/esp-idf-ci-action):

```yml
- name: Build Test Application with ESP-IDF
  uses: espressif/esp-idf-ci-action@v1
  with:
    esp_idf_version: v6.0
    target: ${{ matrix.espidf_target }}
    path: 'test_app'
```

I det här projektet används `esp32c6` som target och version 6.0 som ESP-IDF-version. Om projektet ska byggas för en annan ESP-IDF-version eller ett annat chip ändras `esp_idf_version` i `build_esp_app.yml` och `build_firmware_artifact.yml`, samt target-inställningarna i respektive workflow.

Byggjobbet laddar upp testapplikationens binärer som artifacts. De används sedan av Wokwi-testjobbet.

## Kör Tester I Wokwi (`test_in_wokwi.yml`)

Workflowet `.github/workflows/test_in_wokwi.yml` laddar ner de byggda testbinärerna och kör dem i Wokwi.

För att det ska fungera behöver repositoryt ha en secret med namnet `WOKWI_CLI_TOKEN`. Tokenen skapas i [Wokwi CI Dashboard](https://wokwi.com/dashboard/ci) och läggs in i GitHub under:

`Settings` -> `Secrets and Variables` -> `Actions` -> `Repository secrets`

Testjobbet installerar Wokwi CLI och Python-beroenden:

```yml
- name: Install the Wokwi CLI
  run: curl -L https://wokwi.com/ci/install.sh | sh

- name: Install Python packages for PyTest
  run: pip install -r test_app/requirements.txt
```

Sedan körs pytest med Wokwi som embedded service:

```yml
pytest \
  --junit-xml=./test_app_results_${{ matrix.espidf_target }}.xml \
  --embedded-services idf,wokwi \
  --target=${{ matrix.espidf_target }}
```

Testresultaten laddas upp som artifacts och publiceras i GitHub Actions med [Publish Unit Test Result Action](https://github.com/EnricoMi/publish-unit-test-result-action).

## Bygg Firmware Artifact (`build_firmware_artifact.yml`)

Workflowet `.github/workflows/build_firmware_artifact.yml` bygger den vanliga applikationen i repositoryts rot, inte testapplikationen.

```yml
- name: Build firmware with ESP-IDF
  uses: espressif/esp-idf-ci-action@v1
  with:
    esp_idf_version: v6.0
    target: esp32c6
    path: .
```

När bygget lyckas laddas firmware-binären upp som artifact:

```yml
name: firmware-esp32c6-${{ github.sha }}
path: build/example_project.bin
```

Det gör att varje lyckad körning sparar en byggd firmwarefil som kan laddas ner från GitHub Actions.
