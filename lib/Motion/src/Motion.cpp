/*
Copyright (C) 2017 Damian Nogueiras - Gamesp
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
USA.
See LICENSE.txt for details
*/

#include "Motion.h"
#include <Arduino.h>

// Debug motion function
#define DEBUG_MO 1

Motion::Motion(){
  _myPosition[0] = MAXCELL-1;
  _myPosition[1] = MAXCELL-1;
  _myAuxPosition[0] = getX();
  _myAuxPosition[1] = getY();
  _myCompass=0;
}
/**
 * mov Forward or Backward if squares is Negative
 * @param squares number of movement
 * return true if mov or false if out of outboard (not necessary in v3)
 **/
bool Motion::movForward(int8_t squares){
    // need add or subtrac if forward or Backward
    _signal = squares/abs(squares);
    // new X position
    _myPosition[0] = _myPosition[0] + _signal*steepX(_myCompass);
    // new Y position
    _myPosition[1] = _myPosition[1] + _signal*steepY(_myCompass);
    // move motors
    gear.i2c(_signal>0 ? 'F':'B',HOWMANYLOOPS_FB_15);
    return true;
}

void Motion::turn(bool rightHanded){
  if (rightHanded) {
    if (DEBUG_MO) {
      Serial.println("Turn RIGHT");
    }
    // change compass 90 degrees right
    if (_myCompass == 3) {
      //West to North
      _myCompass = 0;
    } else {
      _myCompass = _myCompass + 1;
    }
    gear.i2c('R',HOWMANYLOOPS_LR);
  }
  else {
    if (DEBUG_MO) {
      Serial.println("Turn LEFT");
    }
    // change compass 90 degrees left
    if (_myCompass == 0) {
      //East to North
      _myCompass = 3;
    } else {
      _myCompass = _myCompass - 1;
    }
    gear.i2c('L',HOWMANYLOOPS_LR);
  }
}

void Motion::stop(){
  if (DEBUG_MO) {
    Serial.println("STOP");
  }
  gear.i2c('S',1);
}
/**
 * movement X axis
 * @param myCompass cardinal direction
 */
int Motion::steepX(int myCompass) {
  switch (myCompass) {
    //North
    case 0:
      return -1;
    //East
    case 1:
      return 0;
    //South
    case 2:
      return +1;
    //West
    case 3:
      return 0;
    default:
      return 0;
  }
}

/**
 * movement Y axis
 * @param myCompass cardinal direction
 */
int Motion::steepY(int myCompass) {
  switch (myCompass) {
    //North
    case 0:
      return 0;
    //East
    case 1:
      return +1;
    //South
    case 2:
      return 0;
    //West
    case 3:
      return -1;
    default:
      return 0;
  }
}

/**
 * get X position
 */
int Motion::getX() {
  return _myPosition[0];
}
/**
 * get Y position
 */
int Motion::getY() {
  return _myPosition[1];
}
/**
 * get compass direction
 */
char Motion::getCardinal() {
  return _cardinal[_myCompass];
}
/**
 * set X position
 */
void Motion::setX(int x) {
  _myPosition[0]=x;
}
/**
 * set Y position
 */
void Motion::setY(int y) {
  _myPosition[1] = y;
}
/**
 * set compass direction
 */
void Motion::setCardinal(char compass) {
    //convert char -> index _cardinal[4] = {'N','E','S','W'};
    switch (compass) {
        case 'N' : _myCompass = 0; break;
        case 'E' : _myCompass = 1; break;
        case 'S' : _myCompass = 2; break;
        case 'W' : _myCompass = 3; break;
        default : _myCompass = 0;
    }


}
