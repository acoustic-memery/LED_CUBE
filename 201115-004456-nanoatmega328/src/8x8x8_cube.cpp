//Control Program for an 8x8x8 LED cube
//Author: Emery Nagy
//Date: Aug 21, 2020
//

// Globals
#include <Arduino.h>
#include <SoftwareSerial.h>
#define x_z 0
#define y_z 1
#define x_y 2

const int groundPin = 0;  //data pin for ground layers
const int latchPin = 2; 
const int clockPin = 1;
const int row1 = 3;
const int row2 = 4;
const int row3 = 5;
const int row4 = 6;
const int row5 = 7;
const int row6 = 8;
const int row7 = 9;
const int row8 = 10;
//SoftwareSerial mySerial(11, 12); //soft serial init
 
void setup() {
  // initialize I/O pins
  pinMode(groundPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  pinMode(row3, OUTPUT);
  pinMode(row4, OUTPUT);
  pinMode(row5, OUTPUT);
  pinMode(row6, OUTPUT);
  pinMode(row7, OUTPUT);
  pinMode(row8, OUTPUT);
 
 // mySerial.begin(9600);
}

byte cube[8][8]; //create an empty 2d byte array which can be used to specify y and z axis in which bits get modified


/*////////////////////////////////////////////////////////////////////////////////////////////////////
Custom shiftout function

-> (Output): Draws a complete X-Y layer i.e. 64 flat nodes

-> (PARAM): Takes an 8 bit value for each row where each bit corisponds to a "node on the x axis" and each row corisponds to the equivelent y axis

-> (PARAM): Takes the ground value
    -> "Ground value" needs to be low for the given layer to be on

*///////////////////////////////////////////////////////////////////////////////////////////////////

void shift(uint8_t val,uint8_t val_2, uint8_t val_3, uint8_t val_4, uint8_t val_5, uint8_t val_6, uint8_t val_7, uint8_t val_8, uint8_t val_ground, int frame_delay) //shift out all 8 rows as well as ground layer row
{
    digitalWrite(latchPin,LOW); //lower latch of each register
     uint8_t i;
     for (i = 0; i < 8; i++)  {
                 digitalWrite(row1, !!(val & (1 << i))); //write the bit of each row
                 digitalWrite(row2, !!(val_2 & (1 << i)));
                 digitalWrite(row3, !!(val_3 & (1 << i)));
                 digitalWrite(row4, !!(val_4 & (1 << i)));
                 digitalWrite(row5, !!(val_5 & (1 << i)));
                 digitalWrite(row6, !!(val_6 & (1 << i)));
                 digitalWrite(row7, !!(val_7 & (1 << i)));
                 digitalWrite(row8, !!(val_8 & (1 << i)));
                 digitalWrite(groundPin, !!(val_ground & (1 << i))); //set the ground pin
           digitalWrite(clockPin, HIGH); //write to the clock
           digitalWrite(clockPin, LOW);       
}
   digitalWrite(latchPin,HIGH); //set the latch high
   //delayMicroseconds(frame_delay);
}


/*//////////////////////////////////////////////////////////////////////////////////////////////////
Frame drawing function

-> (Output): Print a whole 3d frame by using the technique of LED multiplexing
  -> LED multiplexing involves setting each layer and running through them very quickly, giving an illusion of a 3D effect


-> (PARAM): iterations: Number of times the frame should be drawn

-> (PARAM): layer: frame layer preset to run (by default to be ground for one layer at a time)

*////////////////////////////////////////////////////////////////////////////////////////////////////
void frame(int f_delay[], int iterations, uint8_t layer[]){ //shift out all 64 bytes in frame buffer
  int n;
  for(n = 0; n < iterations; n++){ //run the total number of iterations
  int i;
  for (i = 0; i < 8; i++){ //run through each layer in the frame buffer
   shift(cube[0][i], cube[1][i], cube[2][i], cube[3][i], cube[4][i], cube[5][i], cube[6][i], cube[7][i], layer[i], f_delay[i]); //shiftout layers
  }}  
  }

uint8_t layer[8] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111110}; //ground layer presets

int delay_amount[8] = {5, 5, 5, 5, 5, 5, 5, 5}; //delay presets

int inRange(int num){ //chack if inputs fit within the 8x8x8 cube
  if (0 <=num <=7){
    return 1;
    }
    else{
      return 0;
      }}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

setNode and clearNode

-> (PARAM): x, y, z: The address (physical location) of bit to be set or cleared

-> (OUTPUT): set or clear node at value x, y, z
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setNode(int x, int y, int z){ //set a specified node at x, y, z
  if ((inRange(x)) && (inRange(y)) && (inRange(z))){ //make sure that the node to be set is in range of the cube and buffer
  cube[y][z] |= (1<< x);}} //run bitwise operation to set the specified bit 

void clearNode(int x, int y, int z){ //clear a specified node at x,y,z
  if ((inRange(x)) && (inRange(y)) && (inRange(z))){ //make sure that the node to be set is in range of the cube and buffer
  cube[y][z] &= ~(1<<x);}} //run bitwise operation to set the specified bit



void set_col(int x, int y){ //set whole column
  int i;
  for(i=0; i < 8; i++){
    setNode(x, y, i);
    }}


void clear_col(int x, int y){ //clear whole column
  int i;
  for(i=0; i < 8; i++){
    clearNode(x, y, i);
    }}

void set_row(int y, int z){ //set row down x axis
  int i;
  for(i=0; i<8; i++){
    setNode(i, y, z);
    }}

  
void clear_row(int y, int z){  //clear row down x axis
  int i;
  for(i=0; i<8; i++){
    clearNode(i, y, z);
    }}
void set_ray(int x, int z){  //set row down y axis
  int i;
  for(i=0; i<8; i++){
    setNode(x, i, z);
    }}

  
void clear_ray(int x, int z){ //clear row down y axis
  int i;
  for(i=0; i<8; i++){
    clearNode(x, i, z);
    }}

    
void set_plane(int orientation, int start_point){
  if (orientation == x_y){
    int i;
    for(i = 0; i < 8; i++){
      set_col(i, start_point);
      }
    }
     if (orientation == y_z){
    int n;
    for(n = 0; n < 8; n++){
      set_ray(n, start_point);
      }
    }
  }

void clear_plane(int orientation, int start_point){
  if (orientation == x_y){
    int i;
    for(i = 0; i < 8; i++){
      clear_col(i, start_point);    
      }
    } 

    if (orientation == y_z){
    int n;
    for(n = 0; n < 8; n++){
      clear_ray(n, start_point);    
      }
    } 
  }

void clear_cube(){
  int i;
  for (i=0; i<8; i++){
    clear_plane(x_y, i);
  }
}  

void set_random(int numRand, int range){
int i;
for(i=0;i<numRand;i++){
  int randX = random(0, range);
  int randY = random(0, range);
  int randZ = random(0, range);
  setNode(randX, randY, randZ);
}
}


void clear_random(int numRand, int range){
int i;
for(i=0;i<numRand;i++){
  int randX = random(0, range);
  int randY = random(0, range);
  int randZ = random(0, range);
  clearNode(randX, randY, randZ);
}
}

void full_test_demo(){
set_plane(x_y, 0);
frame(delay_amount, 100,layer);
clear_plane(x_y, 0);
frame(delay_amount, 100,layer);


set_plane(x_y, 1);
frame(delay_amount, 100,layer);
clear_plane(x_y, 1);
frame(delay_amount, 100,layer);

set_plane(x_y, 2);
frame(delay_amount, 100,layer);
clear_plane(x_y, 2);
frame(delay_amount, 100,layer);

set_plane(x_y, 3);
frame(delay_amount, 100,layer);
clear_plane(x_y, 3);
frame(delay_amount, 100,layer);

set_plane(x_y, 4);
frame(delay_amount, 100,layer);
clear_plane(x_y, 4);
frame(delay_amount, 100,layer);

set_plane(x_y, 5);
frame(delay_amount, 100,layer);
clear_plane(x_y, 5);
frame(delay_amount, 100,layer);  

set_plane(x_y, 6);
frame(delay_amount, 100,layer);
clear_plane(x_y, 6);
frame(delay_amount, 100,layer);

set_plane(x_y, 7);
frame(delay_amount, 100,layer);
clear_plane(x_y, 7);
frame(delay_amount, 100,layer);


set_plane(y_z, 0);
frame(delay_amount, 100,layer);
clear_plane(y_z, 0);
frame(delay_amount, 100,layer);


set_plane(y_z, 1);
frame(delay_amount, 100,layer);
clear_plane(y_z, 1);
frame(delay_amount, 100,layer);

set_plane(y_z, 2);
frame(delay_amount, 100,layer);
clear_plane(y_z, 2);
frame(delay_amount, 100,layer);

set_plane(y_z, 3);
frame(delay_amount, 100,layer);
clear_plane(y_z, 3);
frame(delay_amount, 100,layer);

set_plane(y_z, 4);
frame(delay_amount, 100,layer);
clear_plane(y_z, 4);
frame(delay_amount, 100,layer);

set_plane(y_z, 5);
frame(delay_amount, 100,layer);
clear_plane(y_z, 5);
frame(delay_amount, 100,layer);  

set_plane(y_z, 6);
frame(delay_amount, 100,layer);
clear_plane(y_z, 6);
frame(delay_amount, 100,layer);

set_plane(y_z, 7);
frame(delay_amount, 100,layer);
clear_plane(y_z, 7);
frame(delay_amount, 100,layer);
}



void H (int i){
  set_col(1,i);
set_col(0,i);

set_col(6,i);
set_col(7,i);

set_row(i,4);
set_row(i,3);
frame(delay_amount, 150,layer);

clear_cube();
}

void I (int i){
  set_col(4,i);
set_col(3,i);

frame(delay_amount, 150,layer);
clear_cube();
}

void R (int i){
  
set_col(7,i);
set_col(6,i);

set_col(3,i);
set_col(2,i);
set_row(i,7);
set_row(i,6);

set_row(i,3);
set_row(i,2);

clearNode(0,i,7);
clearNode(1,i,7);
clearNode(0,i,6);
clearNode(1,i,6);

clearNode(2,i,7);
clearNode(3,i,7);
clearNode(2,i,6);
clearNode(3,i,6);

clearNode(0,i,3);
clearNode(1,i,3);
clearNode(0,i,2);
clearNode(1,i,2);

clearNode(2,i,3);
clearNode(3,i,2);
clearNode(3,i,3);
clearNode(2,i,2);


frame(delay_amount, 100,layer);
clear_cube();
}

void E (int i){
  set_col(7,i);
set_col(6,i);

set_row(i,7);
set_row(i,6);


set_row(i,0);
set_row(i,1);
set_row(i,4);
set_row(i,3);

clearNode(0,i,7);
clearNode(1,i,7);
clearNode(0,i,6);
clearNode(1,i,6);

clearNode(0,i,1);
clearNode(1,i,1);
clearNode(0,i,0);
clearNode(1,i,0);


clearNode(0,i,4);
clearNode(1,i,4);
clearNode(0,i,3);
clearNode(1,i,3);

frame(delay_amount, 100,layer);
clear_cube();

}

void M (int i){

set_col(7,i);
set_col(6,i);
set_col(0,i);
set_col(1,i);


setNode(3,i,5);
setNode(2,i,4);
setNode(3,i,4);


setNode(5,i,5);
setNode(4,i,4);
setNode(5,i,4);

setNode(3,i,3);
setNode(4,i,3);
setNode(3,i,2);
setNode(4,i,2);

frame(delay_amount, 100,layer);
clear_cube();

}


void hire_me(){

/*
////////////////H
set_col(1,7);
set_col(0,7);

set_col(6,7);
set_col(7,7);

set_row(7,4);
set_row(7,3);
frame(delay_amount, 100,layer);

clear_cube();


///////////////////I
set_col(4,7);
set_col(3,7);

frame(delay_amount, 100,layer);
clear_cube();

////////////////R

set_col(7,7);
set_col(6,7);

set_col(3,7);
set_col(2,7);
set_row(7,7);
set_row(7,6);

set_row(7,3);
set_row(7,2);

clearNode(0,7,7);
clearNode(1,7,7);
clearNode(0,7,6);
clearNode(1,7,6);

clearNode(2,7,7);
clearNode(3,7,7);
clearNode(2,7,6);
clearNode(3,7,6);

clearNode(0,7,3);
clearNode(1,7,3);
clearNode(0,7,2);
clearNode(1,7,2);

clearNode(2,7,3);
clearNode(3,7,2);
clearNode(3,7,3);
clearNode(2,7,2);


frame(delay_amount, 100,layer);
clear_cube();

/////E
set_col(7,7);
set_col(6,7);

set_row(7,7);
set_row(7,6);


set_row(7,0);
set_row(7,1);
set_row(7,4);
set_row(7,3);

clearNode(0,7,7);
clearNode(1,7,7);
clearNode(0,7,6);
clearNode(1,7,6);

clearNode(0,7,1);
clearNode(1,7,1);
clearNode(0,7,0);
clearNode(1,7,0);


clearNode(0,7,4);
clearNode(1,7,4);
clearNode(0,7,3);
clearNode(1,7,3);

frame(delay_amount, 100,layer);
clear_cube();

///M

set_col(7,7);
set_col(6,7);
set_col(0,7);
set_col(1,7);


setNode(3,7,5);
setNode(2,7,4);
setNode(3,7,4);


setNode(5,7,5);
setNode(4,7,4);
setNode(5,7,4);

setNode(3,7,3);
setNode(4,7,3);
setNode(3,7,2);
setNode(4,7,2);

frame(delay_amount, 100,layer);
clear_cube();

//E

set_col(7,7);
set_col(6,7);

set_row(7,7);
set_row(7,6);


set_row(7,0);
set_row(7,1);
set_row(7,4);
set_row(7,3);

clearNode(0,7,7);
clearNode(1,7,7);
clearNode(0,7,6);
clearNode(1,7,6);

clearNode(0,7,1);
clearNode(1,7,1);
clearNode(0,7,0);
clearNode(1,7,0);


clearNode(0,7,4);
clearNode(1,7,4);
clearNode(0,7,3);
clearNode(1,7,3);

frame(delay_amount, 100,layer);
clear_cube();*/

int i;
for(i=8; i>=0; i--){
  H(i);}
for(i=8; i>=0; i--){
  I(i);}
for(i=8; i>=0; i--){
  R(i);}
for(i=8; i>=0; i--){
  E(i);}  
for(i=8; i>=0; i--){
  M(i);}
for(i=8; i>=0; i--){
  E(i);}
}


void rand_demo(){
set_random(3,8);
frame(delay_amount, 30,layer);
clear_random(3,8);
}

void loop() {
 
hire_me();




}
