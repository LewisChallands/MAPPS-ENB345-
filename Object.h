//
//  Object.h
//  cvtest
//
//  Created by Lewis on 5/09/2015.
//  Copyright (c) 2015 Lewis. All rights reserved.
//

#ifndef __cvtest__Object__
#define __cvtest__Object__

#include <stdio.h>
#include <string>
using namespace std;

class Object{
    
public:
    
    Object(void);
    ~Object(void);
    
    int getxPos();
    int getyPos();
    
    void setxPos(int x);
    void setyPos(int y);
    
    string get_objectName();
    
private:
    
    int xPos,yPos;
    string objectName;
};


#endif /* defined(__cvtest__Object__) */
