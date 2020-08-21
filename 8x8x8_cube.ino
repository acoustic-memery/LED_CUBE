//Control Program for an 8x8x8 LED cube
//Author: Emery Nagy
//Date: Aug 21, 2020
//

// Globals
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
SoftwareSerial mySerial(11, 12); //soft serial init
 
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
 
  mySerial.begin(9600);
}

byte cube[8][8]; //create an empty 2d byte array which can be used to specify y and z axis in which bits get modified



void shift(uint8_t val,uint8_t val_2, uint8_t val_3, uint8_t val_4, uint8_t val_5, uint8_t val_6, uint8_t val_7, uint8_t val_8, uint8_t val_ground, int frame_delay) //shift out all 8 rows as well as ground layer row
{
    digitalWrite(latchPin,LOW);
     uint8_t i;
     for (i = 0; i < 8; i++)  {
                 digitalWrite(row1, !!(val & (1 << i)));
                 digitalWrite(row2, !!(val_2 & (1 << i)));
                 digitalWrite(row3, !!(val_3 & (1 << i)));
                 digitalWrite(row4, !!(val_4 & (1 << i)));
                 digitalWrite(row5, !!(val_5 & (1 << i)));
                 digitalWrite(row6, !!(val_6 & (1 << i)));
                 digitalWrite(row7, !!(val_7 & (1 << i)));
                 digitalWrite(row8, !!(val_8 & (1 << i)));
                 digitalWrite(groundPin, !!(val_ground & (1 << i)));
           digitalWrite(clockPin, HIGH);
           digitalWrite(clockPin, LOW);       
}
   digitalWrite(latchPin,HIGH);
   //delayMicroseconds(frame_delay);
}



void frame(int f_delay[], int iterations, uint8_t layer[]){ //shift out all 64 bytes in frame buffer
  int n;
  for(n = 0; n < iterations; n++){
  int i;
  for (i = 0; i < 8; i++){
   shift(cube[0][i], cube[1][i], cube[2][i], cube[3][i], cube[4][i], cube[5][i], cube[6][i], cube[7][i], layer[i], f_delay[i]);
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

void setNode(int x, int y, int z){ //set a specified node at x, y, z
  if ((inRange(x)) && (inRange(y)) && (inRange(z))){
  cube[y][z] |= (1<< x);}}

void clearNode(int x, int y, int z){ //clear a specified node at x,y,z
  if ((inRange(x)) && (inRange(y)) && (inRange(z))){
  cube[y][z] &= ~(1<<x);}}



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
  }

void clear_plane(int orientation, int start_point){
  if (orientation == x_y){
    int i;
    for(i = 0; i < 8; i++){
      clear_col(i, start_point);    
      }
    } 
  }


void loop() {
 

set_plane(x_y, 0);
frame(delay_amount, 100,layer);
clear_plane(x_y, 0);
frame(delay_amount, 100,layer);
}
