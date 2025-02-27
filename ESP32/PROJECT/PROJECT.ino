#include "SmartDoor.h"
#include "ledST.h"
SmartDoor   door;

ledST stl;
///////////////////
void setup() 
{
  stl.init(2);
  door.init();
}
////////////////////////////////////////////////////
void loop() 
{
    stl.update();  
    door.Handle();
}
