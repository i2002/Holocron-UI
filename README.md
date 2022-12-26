# Holocron UI
Simple UI toolkit powered by SDL.

## Usage

## Class list

## Tasklist
### Tema 0

- [x] Nume proiect (îl puteți schimba ulterior)
- [x] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
- [x] definirea a minim 3-4 clase folosind compunere
- [x] constructori de inițializare
- [x] pentru o clasă: constructor de copiere, `operator=` de copiere, destructor
<!-- - [ ] pentru o altă clasă: constructor de mutare, `operator=` de mutare, destructor -->
<!-- - [ ] pentru o altă clasă: toate cele 5 funcții membru speciale -->
- [x] `operator<<` pentru toate clasele
- [x] cât mai multe `const`
- [x] implementarea a minim 3 funcții membru publice pentru funcționalități specifice temei alese
- [x] scenariu de utilizare a claselor definite: crearea de obiecte și apelarea funcțiilor membru publice în main
- [x] tag de `git`: de exemplu `v0.1`
- [x] serviciu de integrare continuă (CI); exemplu: GitHub Actions

### Tema 2

#### Cerințe
- [x] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [x] moșteniri
  - [x] funcții virtuale (pure) apelate prin pointeri de bază, constructori virtuali (clone)
  - [x] apelarea constructorului din clasa de bază 
  - [x] smart pointers
  - [x] `dynamic_cast`
- [x] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
- [x] excepții
  - [x] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`
  - [x] utilizare cu sens: de exemplu, `throw` în constructor, `try`/`catch` în `main`
- [x] funcții și atribute `static`
- [x] STL
- [x] cât mai multe `const`
- [x] la sfârșit: commit separat cu adăugarea unei noi derivate
- [x] tag de `git`: de exemplu `v0.2`

### Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] o clasă șablon cu sens; minim 2 instanțieri
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git`: de exemplu `v0.3` sau `v1.0`

## Resurse
- https://github.com/albin-johansson/centurion (MIT)
- https://github.com/libsdl-org/SDL (Zlib)
- https://github.com/libsdl-org/SDL_image (Zlib)
- https://github.com/libsdl-org/SDL_mixer (Zlib)
- https://github.com/libsdl-org/SDL_ttf (Zlib)
- https://www.fontsquirrel.com/fonts/open-sans (Apache)
- adăugați trimiteri către resursele externe care v-au ajutat sau pe care le-ați folosit
