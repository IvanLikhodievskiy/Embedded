# Solutions of the tasks from the Embedded course of Multimedia and Telecommunication Technologies Department of MIPT

## How to clone the repository

```console
git clone https://github.com/IvanLikhodievskiy/Embedded.git
```

## [Task 1](./calculate.s)

The programm written on ARM Assembly language performs the same action as the given program on C. The function calculate is called from _start where parameters repeat, x and y are set in registers r0, r1 and r2 respectively.

Parameters repeat, x and y can be set in the Assembly code changing initial values in r0, r1 and r2 for the corresponding parameters.

Due to the limitations of the register size, the program returnes mod 256 of the calculated value. For this reason in the example in the repository repeat=3, x=2 and y=3, so the program returnes mathematically correct answer (in this case, 69).

### How to run

In order to run the program on x86 processors, QEMU emulator is required. Then, you should be able to execute the following commands:

```console
arm-linux-gnueabihf-as calculate.s -o calculate.o
arm-linux-gnueabihf-ld calculate.o -o calculate
qemu-arm ./calculate
```

To see the returned value, execute the following command:

```console
echo $?
```
