#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
#include "Creature.h"
#include "Container.h"
#include "Item.h"
#include "Trigger.h"


using namespace std;

struct Border{
	string direction;
	string name;
};



class Room{
public:
   Room(xml_node<> * node);
   virtual ~Room();


   string name;
   string status;
   string type;
   string description;


   std::vector<Border> border;
   std::vector<string> container;
   std::vector<string> item;
   std::vector<string> creature;
   std::vector<Trigger> trigger;



};

#endif /* ROOM_H */
