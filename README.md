# Rapid-CDH

## Configure HAL for Simulation
In this mode, you can test flight code on your local machine without MPLAB or Arduino. By substituting the HAL_*.c file with HAL_simulation.c, you can run/simulate the code for CDH down to its I2C implementation on your local machine. This does not require MPLAB and is simply a binary file.
```
    mkdir bin
    g++ main.c HAL_simulation.c util.c FlightStateMachine.c checksum.c -o bin/test
    ./bin/test
```

## Configure HAL for Flight
Rapid CDH uses SAM D51s which requires MPLAB to compile/run. This configuration allows the flight code to use actual hardware I2C implementations.