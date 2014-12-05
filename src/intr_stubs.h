#pragma once

#define STUB(num) extern void intr_stub_ ## num ()

STUB(0);
STUB(1);
STUB(2);
STUB(3);
STUB(4);
STUB(5);
STUB(6);
STUB(7);
STUB(8);
STUB(9);
STUB(10);
STUB(11);
STUB(12);
STUB(13);
STUB(14);
STUB(15);
STUB(16);
STUB(17);
STUB(18);

STUB(32);
STUB(33);

STUB(48);

#undef STUB