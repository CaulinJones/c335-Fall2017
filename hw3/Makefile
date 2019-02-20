


hw3: hw3.o
	cc hw3.o -o hw3


hw3.o: hw3.c
	cc -c hw3.c
test: hw3
	arm-none-eabi-as test.asm -o test.o
	arm-none-eabi-objcopy test.o -O binary --only-section=.text test.bin 
	hexdump -x test.bin |sed -e 's/^[^ ]*//' > test.hex
	./hw3 < test.hex | cat > test.out
	diff test.asm test.out > test_case.diff
