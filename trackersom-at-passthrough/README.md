# trackersom-at-passthrough

Tracker SoM AT Command passthrough firmware source

Just compile and flash onto your Tracker SoM and test before sending to the certification lab

### Connect the DUT to a PC's USB port to see the outputs 

```
RDY
  AT+QCFG="nwscanmode",0,1

  OK

  +PACSP0

  APP RDY
  AT+QCFG="iotopmode",2,1

  OK
```

You can also send commands via the USB port using a serial tool, for example, sending AT+ICCID to see the SIM card ID to check if the ‘white’ SIM card is functioning.

```
  AT+ICCID

  +ICCID: 89014103272880433046

  OK
```

This command AT+COPS? shows which operator the DUT is connected (a China Mobile SIM was used here)

```
  AT+COPS?

  +COPS: 0,0,"CHINA MOBILE",3

  OK
```

#### ```/src``` folder:  
This is the source folder that contains the firmware files for your project. It should *not* be renamed. 
Anything that is in this folder when you compile your project will be sent to our compile service and compiled into a firmware binary for the Particle device that you have targeted.

#### ```.ino``` file:
This file is the firmware that will run as the primary application on your Particle device. It contains a `setup()` and `loop()` function, and can be written in Wiring or C/C++. For more information about using the Particle firmware API to create firmware for your Particle device, refer to the [Firmware Reference](https://docs.particle.io/reference/firmware/) section of the Particle documentation.

## Compiling your project

When you're ready to compile your project, make sure you have the correct Particle device target selected and run `particle compile <platform>` in the CLI or click the Compile button in the Desktop IDE. The following files in your project folder will be sent to the compile service:

- Everything in the `/src` folder, including your `.ino` application file
- The `project.properties` file for your project
- Any libraries stored under `lib/<libraryname>/src`
