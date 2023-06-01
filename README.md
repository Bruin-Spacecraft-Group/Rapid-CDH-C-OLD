# Rapid CDH

### [Google Drive](https://drive.google.com/drive/folders/1UrGinDj2qladcFWh7qRNBbjipHgWgkza?usp=share_link)

## Build to Arduino ATMEGA2560 (Flat Sat)
Arduino will be used for the flat sat and for general testing. Please ask Randall also we should definitely write some documentation tutorial on doing it here.

## Build to Simulation
In this mode, you can test driver code on your local machine without MPLAB or Arduino. By substituting the HAL_*.c file with HAL_simulation.c, you can run/simulate the code for CDH down to its I2C implementation on your local machine. This does not require MPLAB and is simply a binary file.
```
    mkdir bin
    g++ -D __SIMULATION__ -o bin/test *.c
    ./bin/test
```

## Build to Atmel SAM D51
Atmel SAM D51 will be used for production. Rapid CDH uses SAM D51s which requires MPLAB to compile/run. This configuration allows the flight code to use actual hardware I2C implementations.
