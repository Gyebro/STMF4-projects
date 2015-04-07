import processing.serial.*;
 
Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph
float axprev = 0;
float ayprev = 0;
float azprev = 0;
float ax, ay, az;
float scl = 0.3;
 
void setup () {
 // set the window size:
 size(1280, 720);        
 myPort = new Serial(this, "COM8", 9600);
 myPort.bufferUntil('\n');
 background(0);
}
 
 
void draw () {
 // everything happens in the serialEvent()
}
 
void serialEvent (Serial myPort) {
 // get the ASCII string:
 String inString = myPort.readStringUntil('\n');
 if (inString != null) {
   // trim off any whitespace:
   String[] accString = split(inString,' ');
   if (accString.length > 2) {
     ax = float(accString[0]); 
     ay = float(accString[1]); 
     az = float(accString[2]); 
     // draw the line:
     stroke(255,40,40);
     line(xPos, (1-axprev*scl)*height/2, xPos+1, (1-ax*scl)*height/2);
     stroke(40,255,40);
     line(xPos, (1-ayprev*scl)*height/2, xPos+1, (1-ay*scl)*height/2);
     stroke(40,100,255);
     line(xPos, (1-azprev*scl)*height/2, xPos+1, (1-az*scl)*height/2);
     axprev = ax;
     ayprev = ay;
     azprev = az;
     // at the edge of the screen, go back to the beginning:
     if (xPos >= width) {
       xPos = 0; background(0); 
     } 
     else {
       xPos++; // increment the horizontal position:
     }
   }
 }
}
