# Heptaboard

Heptaboard is a tool to use the reactive programming language [heptagon](https://gitlab.inria.fr/synchrone/heptagon) on AVR Arduino boards. It allows to use the Arduino lib inside the heptagon code and to compile this code into an arduino executable and send it to the Arduino board with one single command. It was only tested on MacOS with an Arduino Uno board.

## Installation

You need to install [heptagon](https://opam.ocaml.org/packages/heptagon/) (which requires OCaml), [avr-gcc](https://gcc.gnu.org/wiki/avr-gcc) and [avrdude](https://www.nongnu.org/avrdude/). The [arduino-cli](https://github.com/arduino/arduino-cli) can be usefull to find the Arduino board location on your system.

Download the repository, and use `make`.

```bash
git clone https://github.com/webalorn/heptaboard/tree/main
cd heptaboard
make
make install
```

## Usage

```bash
# Compile one example and send it to the automatically detected Arduino, executing the main loop 4 time per second
heptaboard -s -f 4 examples/setup.ept examples/led_blink.ept

# Send the code to the board located at /dev/arduinodevic, executing it every 200 milliseconds, with a setup file written in C
heptaboard -c examples/setup.c -b /dev/arduinodevice -s -d 200 examples/two_leds.ept

## Higher frequency
heptaboard -c examples/setup.c -s -f 6000 examples/setup.ept examples/detector.ept
```

You can use `heptaboard --help` to display all the available options.

## License
[MIT](https://github.com/webalorn/heptaboard/blob/main/LICENSE)