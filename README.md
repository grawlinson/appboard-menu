# AppBoard Menu

The purpose of the menu is to allow the user to interact with the various peripherals on the application board, using only the 4-way rotary button pad (up, down, left & right) and the LCD.

![Image of the AppBoard controls][img-controls]

The 4 peripherals are the:

* Motor (0-100% speed control)
* Lamp (0-100% brightness contro)l
* Speaker (0-25kHz tone frequency control)
* RGB LED (0-32mA control of each LED)

## Menu Structure

The menu will include a flashing cursor to indicate which line is selected as well as multiple sub-menus which can be entered/exited using the buttons.

## TODO

The code works as-is, so odds are I won't actually get around to refactoring the code. There are a few things that can definitely be improved, such as:

* Motor, lamp, speaker & rgb functions into separate file titled peripherals.
* Use structs & change the ways that functions access variables.
* RGB functions look quite similar. e.g. increase{Red,Green,Blue} function can probably be merged into one. void increaseRGB(char selectedColor)

## LICENSE

The MIT License (MIT)

Copyright (c) 2015 George Rawlinson <mailto:george@rawlinson.net.nz>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

[img-controls]: images/controls.jpg
