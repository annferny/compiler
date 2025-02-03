# PL0-Compiler für eine 64-Bit virtuelle Maschine

Dieses Projekt implementiert einen Compiler für die Programmiersprache PL0. Der Compiler erzeugt Code für eine 64-Bit virtuelle Maschine.

## Dateien

| Datei                      | Funktionalität                     |
|----------------------------|------------------------------------|
| `lex.h`, `lexframe.c`      | Lexikalische Analyse (Scanner)     |
| `parser.h`, `parser.c`     | Syntaxanalyse (Parser)             |
| `CodeGen.h`, `CodeGen.c`   | Codegenerierung                    |
| `namelist.h`, `namelist.c` | Verwaltung der Symboltabelle       |
| `list.h`, `list.c`         | Implementierung verketteter Listen |
| `stack.h`, `stack.c`       | Stack für Label                    |
| `structs.h`                | Datenstrukturen                    |
| `tests.h`, `tests.c`       | Testfunktionen                     |
| `lexTest.c`                | Mainfunktion                       |
| `code.h`                   | Enum für Kellerbefehle             |
| `compiler64`               | Das kompilierte Program            |
