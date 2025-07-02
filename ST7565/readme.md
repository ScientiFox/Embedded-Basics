<h2>ST7565 Readme</h2>

The purpose of the ST7565 library is to implement a custom graphic management policy that allows for simpler, but more limited, usage of graphical objects on graphic LCD screens implementing he ST7565 communication and control protocol. The basic idea is to allow the user to implement graphics objects without having to track position and overlap issues within their code. It is originally drawn at the low level from the Adafruit glcd library, but now contains radical changes which make it almost completely distinct from that code.

To that end, this library generates a mathematically-enforced boundary around each object as it is drawin on the screen, which other objects may not cross. Furthermore, each object is, at time of creation, told to which corner of the screen it to be inset. The software then dynamically computes where, precisely, the object ought to be placed by fitting in all objects in order, by sliding their bounding boxes into place against previously drawn objects.

To illustrate this action, conside a screen with a few objects drawn on it, as below (bounding boxes in red):

![image](https://github.com/user-attachments/assets/79df4972-cb4c-4cd2-9bcc-a8d1a33acce4)

If we should then create a new circle, and specify to place it in the bottom left corner, displacing vertically, then horizontally until fit, then we would end up with:

![image](https://github.com/user-attachments/assets/c97e1ad8-4c81-4953-b046-36237f1d3c31)

Then, adding an identical circle, but in the top right, displacing horizontally, then vertically, we'd have:

![image](https://github.com/user-attachments/assets/364be6de-95d0-445b-bfa5-8e73ce631f67)

The library performs this computation using the bounding boxes, and a simple mathematical operation to hypothesize the locations of corners of the bounding box (which is sized according to a specific formula for each possible shape- including custom shapes and graphics) and check if any of those corners is withing another box. The 'horizontal' and 'vertical' displacement arguments tell it whether to move horizontally over (changing vertical position each time one of the corners gors off-screen) to fit, or vertically (moving horizontally each time a corner is off-screen).

To ensure fast computation, the locations of all drawn bounding box corners are contained within an array, which allows the tracing function which scans for a valid box placement for the impending object to evaluate points quicly with respect to all boxes, and not have to track all individual objects within the image array. The find_box function does this for a specified width and height of box, given the desired starting corner and preferred direction of displacement.

The functional component of the library consists of a collection of different draw operations that let you put various geometric shapes on the screen. These functions collect various arguments about the properties of the shape, depending on its type, and also positioning arguments suggesting where to draw them. We also have a collection of differing composite objects which are built up from simpler ones (like a sun graphic made of triangles and circles), and use a measurement of the width and height of that composite to supply the needed width and height info to the find_box function.

One important note about this library is that the object list, graphical array, and associated data take up a large amount of memory within the Arduino, and thus can, in cases where complex code is written containing large amounts of variables, it is possible to over consume memory within the chip and start having strange errors. One of the ways to deal with this is have all graphics processing on one board, and another talking to that one over serial controlling the graphics remotely and handling all other processing in a separate thread. The Serial Screen Controller library supports this.

<h3>Functions Reference:</h3>
In all functions, there are 'crn' and 'dir' arguments. These tell the controller where to put the object that you are drawing. crn specifies which corner to start the placement of the object in, and dir tells it how to move 

Valid corners are TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
Valid directions are VERTICAL and HORIZONTAL

```
  void write_string(String string,byte crn, byte dir)
```

Draw the text 'string' on the screen.

```
  void write_rect(byte h, byte w,byte crn,byte dir);
```

Draw a rectangle of height 'h' and width 'w' on the screen.

```
  void write_rect_fill(byte h, byte w,byte crn,byte dir);
```

Draw a filled rectangle of height 'h' and width 'w'.

```
  void write_circle(byte r,byte crn,byte dir);
```

Draw a circle of radius 'r'.

```
  void write_circle_fill(byte r,byte crn,byte dir);
```

Draw a filled circle of radius 'r'.

```
  void write_thermometer(byte temp, byte crn,byte dir);
```

Draw a thermometer graphic, displaying temperature 'temp', ranging from -40 to 120.

```
  void write_clock(byte hr, byte min, byte r, byte crn,byte dir);
```

Draw a clock of radius 'r' (greater than 5) displaying the time hr:min.

```
  void write_graph(int data[],byte size,byte data_max,byte h, byte crn, byte dir);
```

Draw a line graph of height 'h' of the values in 'data'. You must also supply the upper limit of the desired scaling as 'data_max' (not necessarily the maximum in 'data', just the maximum you want to assign to the top of the graph) and the length of the data array as 'size'

```
  void write_bar_chart(byte data[],byte size,byte data_max,byte h, byte crn, byte dir);
```

Draw a bar graph of height 'h' of the values in 'data'. You must also supply the upper limit of the desired scaling as 'data_max' (not necessarily the maximum in 'data', just the maximum you want to assign to the top of the graph) and the length of the data array as 'size'

