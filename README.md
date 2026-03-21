# Solutions of the tasks from the Embedded course of Multimedia and Telecommunication Technologies Department of MIPT

## How to clone the repository

```
git clone https://github.com/IvanLikhodievskiy/Embedded.git
```

## [Task 1](https://github.com/IvanLikhodievskiy/Embedded/blob/main/calculate.s)

The programm written on ARM Assembly language realizes the following C function and runs it:

```
uint32_t calculate(uint32_t repeat, uint32_t x, uint32_t y)
{
    uint32_t max = 10;
    uint32_t res = 0;
    for (int i = 0; i < repeat; i++)
    {
        uint32_t sum = x + y;
        uint32_t mul = x * y;
        res += sum + mul;
        x = (sum < max) ? sum : max;
    }
    return res;
}
```

The function *calculate* is called from \_*start* where parameters *repeat*, *x* and *y* are set in registers *r0*, *r1* and *r2* respectively.

Parameters *repeat*, *x* and *y* can be set in the Assembly code changing initial values in *r0*, *r1* and *r2* for the corresponding parameters.

Due to the limitations of the register size, the program returnes mod 256 of the calculated value. For this reason in the example in the repository *repeat=3*, *x=2* and *y=3*, so the program returnes mathematically correct answer (in this case, 69).

### How to run

In order to run the program on x86 processors, QEMU emulator is required. Then, you should be able to execute the following commands:

```
arm-linux-gnueabihf-as calculate.s -o calculate.o
arm-linux-gnueabihf-ld calculate.o -o calculate
qemu-arm ./calculate
```

To see the returned value, execute the following command:

```
echo $?
```

## [Task 2](https://github.com/IvanLikhodievskiy/Embedded/tree/main/Task%202)

The program runs on the **STM32F746G-DISCO** board and implements timer-based LED blinking with frequency control via a button.

**Behavior:**
- After reset: LED1 blinks at 1 Hz with 50% duty cycle
- Each press of the USER button doubles the frequency until 16 Hz: 1 -> 2 -> 4 -> 8 -> 16 -> 1

### How to build and run

The project requires the **STM32CubeF7 firmware package** (v1.17.0 or later). Clone it with submodules:

```
git clone --recurse-submodules https://github.com/STMicroelectronics/STM32CubeF7.git
```

Change the folders `Inc`, `Src`, and `SW4STM32` inside the `TIM_TimeBase` folder to the content of the corresponding folders inside `Task 2`. The `TIM_TimeBase` folder should be located under the following path:

```
STM32Cube_FW_F7_V1.17.0/Projects/STM32746G-Discovery/Examples/TIM/TIM_TimeBase/
```

Then import the project into **STM32CubeIDE**:

1. Open STM32CubeIDE
2. **File → Open Projects from File System...**
3. Select the folder: `TIM_TimeBase/SW4STM32/STM32746G_DISCOVERY/`
4. Click **Finish**, agree to project conversion when prompted
5. Build: **Ctrl+B**
6. Flash and debug: right-click the project -> **Debug As -> STM32 Cortex-M C/C++ Application**
