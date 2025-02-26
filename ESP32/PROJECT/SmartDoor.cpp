#include "SmartDoor.h"
// #include "mega.h"
// #include "web.h"
#include "ledST.h"
#include "debug.h"
#include "HttpServer.h"
////////////////////////////////////////////////////
ledST stl;
debug dbg;
HttpServer s;
 
///////////////////////////
 
SmartDoor::SmartDoor()  // Constructor
{   
}
//__________________________________________________________________________________________________________________________________________________________________
void SmartDoor::init(char pin)
{
  stl.init(2);
  dbg.init();
  s.init();  
}
//__________________________________________________________________________________________________________________________________________________________________
 void SmartDoor::init(char pin)
{
  // dbg.send("count",12344);
    s.handle();
}
 
