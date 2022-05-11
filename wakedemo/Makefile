# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:wakedemo-c.elf wakedemo-s.elf

#additional rules for files
wakedemo-c.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o buzzer.o switch-interrupt-handler-c.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load-c: wakedemo-c.elf
	msp430loader.sh $^

wakedemo-s.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o buzzer.o switch-interrupt-handler-s.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load-s: wakedemo-s.elf
	msp430loader.sh $^
clean:
	rm -f *.o *.elf
