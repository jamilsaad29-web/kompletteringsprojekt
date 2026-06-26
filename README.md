# ESP-IDF-projektmall med CI och Wokwi-simulering

Det här är en projektmall som har anpassats från referensen för ESP-IDF-projekt med GitHub CI.

## Konfiguration (Wokwi-simulering)

Gå till [Wokwi CI Dashboard](https://wokwi.com/dashboard/ci) och skapa en ny CI-token. Lägg sedan till tokenen som en secret i ditt GitHub-repository med namnet `WOKWI_CLI_TOKEN` under "Settings" -> "Secrets and Variables" -> "Actions" -> "Repository secrets".

## Pre-commit hooks

Eftersom vi har lagt in linting via pre-commit hooks i GitHub Actions-flödet är det en bra idé att se till att samma kontroller också körs lokalt på våra datorer. Poängen med (våra) pre-commit hooks är att genomföra linting av koden och säkerställa att den följer en bestämd kodstruktur. Om de här kontrollerna misslyckas när du skapar en Pull Request kommer resten av testerna inte att köras förrän du har pushat upp rättad kod. Genom att använda samma pre-commit hooks lokalt kan vi hitta och automatiskt rätta småfel innan de ens commitas lokalt.

Börja med att installera Python-skriptet `pre-commit`. Om du kör kommandot direkt i mappen kan du få meddelandet *"This environment is externally managed ..."*. I så fall kan du antingen skapa och aktivera en virtuell miljö själv (`python -m venv .venv` följt av `source .venv/bin/activate`) eller öppna en ESP-IDF Terminal, som bör öppna en virtuell miljö *åt dig*:

`python -m pip install pre-commit`

Sedan använder vi pre-commit för att installera hooks:

`pre-commit install --install-hooks`

Efter det bör varje ny commit automatiskt köra pre-commit hooks när du försöker commita. Du kan också köra dem manuellt så här:

`pre-commit run --all-files`

## License

Innehållet i detta repository är släppt till public domain eller [CC0-licensierat](LICENSE), efter ditt eget val.
