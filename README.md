# Holocron UI
Simple UI toolkit powered by SDL.

This library was built as final project for the Object-Oriented Programming course at Faculty of Mathematics and Informatics, University of Bucharest.

## Usage
All classes are included in ...
Link dependencies
### Installation

### Dependencies

### Sample application

## Documentation
### Widget tree
The core concept of this library is the widget tree. The user interface is built as a tree of widgets, with the root widget being a window.
The widget tree handles basic tasks such as sizing, rendering and event propagation.

### Class hierarchy
- `Widget`
- `Container`
- `GridContainer`
- `TextBox`
- `Window`

### Widget sizing and positioning

### Event system
The base `Widget` class is responsible for receiving and propagating GUI events applied to the window.
Client code can register additional event handlers for custom functionality.
In addition to that, each class inherited from `Widget` may have its own events to which client code can subscribe.

In order to subscribe to an event it's necessary to specify the event type and the base class which emits it (by default it's Widget).
```cpp
main_window.add_event_handler<std::string, Window>([](const std::string &event) {
    std::cout << "Window clicked: " << event << "\n";
    return false;
});
```

The event handler can be a function, member function bound to `this` or a lambda function that receives the event as the first parameter and
an optional data parameter which is passed at handler registration.

## Known issues

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
- [x] o funcție șablon (template) cu sens; minim 2 instanțieri
- [x] o clasă șablon cu sens; minim 2 instanțieri
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
