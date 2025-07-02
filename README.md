<h2>Embedded Libraries</h2>

A set of libraries written over the years for microcontrollers, mostly Arduino-compatibles with AVR chips, but also several that are made to be Teensy compatible.

Currently Includes:
- **BKlib**: for managing a hub/node network of AT command controlled serial-over-bluetooth chips.
- **Clock**: an asynchronous clock tracking library that handles formatting and tracking without a regular interrupt request
- **Durham-EPD**: One of the projects I'm most proud of, it was a library for doing real-time text and drawing updates on an E-paper display back in 2014, which was pretty unheard of at the time.
- **LEDStrip**: A library for managing indexing and color mixing on a standard SPI type LED strip easy- used in several games elsewhere
- **Ping**: A small one to handle measurements and data conversions from the timed ultrasonic sensors
- **QTR**: Another small one for reading values from QTR infrared distance sensors, which use capacitor discharge time as the measure
- **Serial Screen Controller**: A two part setup that has firmware for one MCU to control a 16x2 LCD screen, and another to issue commands over a serial connection, freeing up one MCU to do other tasks
- **Servo360**: A library to manage 360 degree rotation and other servos easily, including soem support for tracking position, setting speeds, and assigning left and right side wheel motors
- **Sharp**: A tiny little library for interacting with Sharp brand IR distance sensors, mostly existing to implement an empirically measured and vetted voltage to distance conversion
- **speakerSimple**: A library for interacting with and controlling a small speaker module, and playign songs. The module used a passive wave shaping circuit to make the output square waves more sine-like for better quality and note production
- **SRF02**: A library made to manage a large number of SRF02 I2C ultrasonic distance sensors. It tracks and addresses each sensor independently, on up to the 3 I2C lines on the Teensy, and all asynchronously. Used on a robot that had 12 of them.
- **ST7565**: Another one I'm really proud of, this is a graphics manager to make working with dynamic updates and animated objects feasible on a 64x128 full color display using the resources in an AtMega168. Uses some RLE and a block-stacking algorithm to manage objects.
