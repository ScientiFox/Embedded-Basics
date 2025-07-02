<h2>LEDStrip Readme</h2>

The LEDStrip library is a derivation of the Adafruit APA102 driver library, with most of the mechanisms put in place to handle multiple processors and other configurations stripped out. Additionally, there is a collection of new functions which revolve around converting the color argument to be a 1-dimensional value.

The color values are computed from a mixing function as illustrated below:

![image](https://github.com/user-attachments/assets/c1bcdf3b-2d5e-4806-9bf7-5f3c5ea08de5)

Where each position along  the 0-300 axis indicates a certain proportion of Red, Green, and Blue component color. This mapping of course reduces the total span of color space available, but is convenient for general purpose 1D math and the like.

This means that there are a set of bounded functions which calculate the components of RGB color as proportions of full-scale brightness:

  int region = (0<=color)*(color<100) + 2*(100<=color)*(color<200) + 4*(200<=color)*(color<300);
  color_out [0] = (color)*((region&1)>0) + (200 - color)*((region&2)>0);
  color_out [1] = (color-100)*((region&2)>0) + (300 - color)*((region&4)>0);
  color_out [2] = (100-color)*((region&1)>0) + (color-200)*((region&4)>0);

The first function calaculates 'region', which indicates whether the input (0-300) is in the 1st, 2nd, or 3rd division of 100. This makes calculating the value of the functions simpler by not requiring any if statements to handle the piecewise components of the functions.

The real meat of the library, in terms of the functional activity, is in the sw_spi_out function, which actually pushes a byte of data to the LED strip; the setPixel function, which inserts LED values into the storage array; and the draw function, which takes the values from the array and actually writes them to the strip. Their operation is straightforward.
