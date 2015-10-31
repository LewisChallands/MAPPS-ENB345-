//
//  Object.cpp
//  cvtest
//
//  Created by Lewis on 5/09/2015.
//  Copyright (c) 2015 Lewis. All rights reserved.
//

#include "Object.h"

Object::Object(void){}

Object::~Object(void){}

int Object::getxPos(){
    
    return xPos;
}

int Object::getyPos(){
    
    return yPos;
}

string Object::get_objectName(){

    return objectName;
}

void Object::setxPos(int x){

    xPos = x;

}

void Object::setyPos(int y){
    
    yPos = y;
    
}