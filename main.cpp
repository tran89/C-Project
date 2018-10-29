#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>

#include "Room.h"
#include "Creature.h"
#include "Item.h"
#include "Container.h"
#include "Trigger.h"


using namespace std;
using namespace rapidxml;



map <string, Room> map_rooms;
map <string, Item> map_items;
map <string, Container> map_containers;
map <string, Creature> map_creatures;
map <string, Trigger> map_triggers;

int main(void)
{



	ifstream myfile("src/roomsample.xml");
	   rapidxml::xml_document<> doc;

	   /* "Read file into vector<char>"  See linked thread above*/
	   vector<char> buffer((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>( ));

	   buffer.push_back('\0');

	  // cout<<&buffer[0]<<endl; /*test the buffer */

	   doc.parse<0>(&buffer[0]);

	   //cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

	   xml_node<> *mroot = doc.first_node();

	   string main_name;



	   for (xml_node<> *parentNode = mroot->first_node(); parentNode; parentNode = parentNode->next_sibling())
	   {

		   main_name = parentNode->name();

		   std::cout << "rootValue = " << main_name <<std::endl;

		   if(main_name.compare("room") == 0)
		   {
			   Room p_room(parentNode);
			   map_rooms.insert( std::pair<string,Room>(p_room.name,p_room) );

		   }

		   else if(main_name.compare("item") == 0)
		   {
			   Item p_item(parentNode);
			   map_items.insert( std::pair<string,Item>(p_item.name,p_item) );
		   }

		   else if(main_name.compare("container") == 0)
		   {
			   Container p_container(parentNode);
			   map_containers.insert( std::pair<string,Container>(p_container.name,p_container) );
		   }

		   else if(main_name.compare("creature") == 0)
		   {
			   Creature p_creature(parentNode);
			   map_creatures.insert( std::pair<string,Creature>(p_creature.name,p_creature) );
		   }

		   else if(main_name.compare("trigger") == 0)
		   {
			   Trigger p_trigger(parentNode);
			   map_triggers.insert( std::pair<string,Trigger>(p_trigger.print,p_trigger) );

		   }


	   }

		  cout << "testing" <<std::endl;

		   Room * current_room;

		   current_room = &(map_rooms.at("Entrance"));

		   int gameover = 0;
		   string input;
		   int errorFlag = 0;
		   string nextRoom;



		   while(!gameover)
		   {

			   if(!(current_room->description.empty()) && !errorFlag)
			   std::cout << current_room->description <<std::endl;
			   errorFlag = 1;


			   std::getline(std::cin,input);
			   std::cout << input <<std::endl;

			   if((input[0] == 'n')||(input[0] == 's')||(input[0] == 'e')||(input[0] == 'w') )
			   {


				   if(input.compare("n") == 0)
				   {
					   input = "north";
				   }
				   else if(input.compare("s") == 0)
				   {
					   input = "south";
				   }
				   else if(input.compare("w") == 0)
				   {
					   input = "west";
				   }
				   else if(input.compare("e") == 0)
				   {
					   input = "east";
				   }


				   for(Border b : current_room->border)
				   {

					   //std::cout << b.direction << " vs "<< input << std::endl;
					   if(input.compare(b.direction) == 0)
					   {
						   errorFlag = 0;
						   nextRoom = b.name;
						   current_room = &(map_rooms.at(nextRoom));
					   }

				   }

				   if(errorFlag == 1)
				   {
				   std::cout << "Can't go that way" <<std::endl;
				   }

			   }


			   else if(input.compare("open exit") == 0)
			   {

				   if(current_room->name.compare("Exit") == 0)
				   {
					   std::cout << "Game Over" << std::endl;
					    gameover = 1;
				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }



		   }


}
