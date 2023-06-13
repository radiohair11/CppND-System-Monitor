#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <unistd.h>

int main() {
  System system;
  NCursesDisplay::Display(system);
}