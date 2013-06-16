contiki-cc2530eb
================

contiki for cc2530eb. I create this repo for the reason that my cc2530
board is a bit different from the official cc2530dk board and I
couldn't afford the official cc2530dk board.

meanwhile, with the constraintion of 8051 cpu and the difference of
sdcc compiler, the core and the app code may have some change with the
official code if you want to run coap without stackoverflow。

cc2530eb专属contiki。( ° ▽ °)，买不起官方的cc2530dk，所以在淘宝上买的模块，地址http://item.taobao.com/item.htm?spm=a230r.1.14.27.OhPnto&id=10090130879
,我这里没有打广告的意思，但是那个老板挺好人的，以前zstack各种问题麻烦了
它很多次。这模块有1个joystick(它这
里是5个按键)，只要不是同时按2个以上按键的话，用法跟代码跟joystick应该是通用的，但是跟原版的cc2530dk和
cc2530eb还是有一点不同。同时因为8051 架构的cpu还
有sdcc编译器的关系，如果想要跑coap的话，要对app层的代码改动比较大的变
化，有时甚至需要对core层进行修改，来防止堆栈溢出所以特地建了一个repo。
这个工程是基于george oikonomou的contiki-sensinode，地址如下
https://github.com/g-oikonomou/contiki-sensinode ，但他只简单移植了
rpl和tcp/ip，并没有搞coap。另外我自己添加了一个joystick-demo 和
simple-udp-rpl，另外rest-coap已经成功跑起来，重启问题已经搞定，但是不
知道为什么通信不了，搞好之后我就会push上来的，cc2530可以跑coap，没问题，
但是SRAM会使用接近8K，这样子的话，可编程空间就会很小，我会尽量优化的。

first you must follow george's github wiki and build the sdcc
toolchain

and you may need the cc-tool the flash cc2530 see here http://sourceforge.net/projects/cctool/

TO compile the source code 
cd example/cc2530eb/blink-hello
make
cc-tool -v -e -w blink-hello.hex
