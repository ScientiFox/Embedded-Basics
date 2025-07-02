<h2>Serial Screen Controller Readme</h2>

The Serial Screen Controller library is designed to support the use of two makerboards by providing a packaged serial interface between the two. The library itself resides on the side which is running all the control code, and transmits to the side which is just controlling the screen. The side which controls the screen had an interpreter script running on it, which is the 'serialScreenControllerChildFirmware' sketch in the examples within he library. That sketch listens for the serial strings sent from the parent when a method call to this library is made on the parent.

The serial formatting starts with an '&' to signal the start of a data packet, followed by a control code (which is just a number), telling the child which function it is trying to invoke. The following number is a length of an optionally appended list of data, which is used to supply data points to the graphing functions. For most functions, it is 0 because we need not supply so much data. After that is a pair of values intended to be filled by characteristic values of the shape (like the height and width of a square), which vary among different types of objects. The two numbers after that are the corner and direction arguments used to place the object on screen. If the optional data packet is not included, the string ends here. If the data packet has a length greater than 0, then the packet is appended. All numbers in the sequence are separated by commas:

For instance, the string to draw a clock indicating 12:15 would look like this:
"&,11,0,12,15,"+BOTTOM_LEFT+","+VERTICAL

Valid corners are TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
Valid directions are VERTICAL and HORIZONTAL

The functions supplied to send commands are outlined below:

void draw_rect(int h, int w, int crn, int dir);
	Draw a rectangle of height 'h' and width 'w'.

void draw_rect_fill(int h, int w, int crn, int dir);
	Draw a filled rectangle of height 'h' and width 'w'.

void draw_circle(int r, int crn, int dir);
	Draw a circle of radius 'r'.

void draw_circle_fill(int r, int crn, int dir);
	Draw a filled circle of radius 'r'.

void draw_string(String str, int len, int crn, int dir);
	Draw a string 'str'. You must supply the length of the string as 'len'.

void draw_graph(int data[], int len, int h, int M, int crn, int dir);
	Draw a graph of height 'h' from the values in 'data'. You must also supply the length of 	the array 'data' as len, and the maximum value for the top of the graph as 'M' for 	scaling.

void clr();
	Tell the controller to clear the screen.

void draw();
	Display all the things that have been drawn to the screen since the last clr().
