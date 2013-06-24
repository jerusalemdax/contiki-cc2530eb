contiki-cc2530eb
================

This repo is base on the foundation of george oikonomou's
contiki-sensinode project, address here:https://github.com/g-oikonomou/contiki-sensinode
I create this repo for the reason that my cc2530
board is different from the official cc2530dk board and I
couldn't afford the official cc2530dk board.

I call my board cc2530eb and I bought it here. http://item.taobao.com/item.htm?spm=a230r.1.14.27.OhPnto&id=10090130879
I'm not making ads here, it just because that the seller is friendly
to me when I learning zstack.

the cc2530eb use five buttons instead of joystick, so the joystick
 may be used different with the official joystick. And I couldn't
 promise that the code could run with the joystick

meanwhile, with the constraintion of 8051 cpu and the difference of
sdcc compiler, the core and the app code may change lots of code with the
official code if you want to run coap without stackoverflow。

first you must follow george's github wiki and build the sdcc
toolchain

and you may need the cc-tool the flash cc2530 see here http://sourceforge.net/projects/cctool/

TO compile the source code 
cd example/cc2530eb/blink-hello

make

cc-tool -v -e -w blink-hello.hex

* TODO
* how to use the sniffer example ? 
* the rest CoAP server couldn't get message correctly although it has no autorestart problem now
* joystick example still have bugs, always show two buttons result when I press it, maybe this is the difference between the official joystick
* temperory could not run neither coap3 or coap7, I think it's the hardware reason, But I will try it.
