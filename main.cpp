#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <regex>
#include <algorithm>

#include <stdio.h>
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

std::vector<string> inventory;
Room * current_room;



void run_action(string);

int gameover = 0;


int main(void)
{



	ifstream myfile("src/containersample.xml");
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

		   //std::cout << "rootValue = " << main_name <<std::endl;

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

		   //Room * current_room;

		   current_room = &(map_rooms.at("Entrance"));
		   Item * current_item;


		   string input;
		   int errorFlag = 0;
		   string nextRoom;

		   regex turnon_p("turn on ([a-zA-z]+)");
		   smatch item_m;
		   smatch container_m;
		   smatch item_in_container_m;


		   regex open_container_p("open ([a-zA-z]+)");
		   regex pickup_p("take ([a-zA-z]+)");
		   regex read_item_p("read ([a-zA-z]+)");
		   regex drop_item_p("drop ([a-zA-z]+)");
		   regex put_item_p("put ([a-zA-z]+) in ([a-zA-z]+)");

		   while(!gameover)
		   {

			   if(!(current_room->description.empty()) && !errorFlag)
			   std::cout << current_room->description <<std::endl;
			   errorFlag = 1;


			   std::getline(std::cin,input);

			   if(input.length() == 1 && ((input[0] == 'n')||(input[0] == 's')||(input[0] == 'e')||(input[0] == 'w')))
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

			   else if(input.length() == 1 && (input[0] == 'i'))
			   {

				   std::cout << "Inventory: " ;

				   if(inventory.size() == 0)
					   {
					   	   std::cout << "empty";
					   }
				   else
				   {
					   for(string ip : inventory)
						   {
							   std::cout << ip << ", ";
						   }
				   }
				   std::cout <<"\n" << std::endl;
			   }


			   else if(std::regex_match(input,item_m,turnon_p))
			   {
				   string item_name = item_m[1];
				   //std::cout << item_name << std::endl;

				   if(std::find(inventory.begin(), inventory.end(), item_name) != inventory.end()) //checks if item is in inventory vector
				   {
					   std::cout << "You activated "<< item_name << std::endl;
					   current_item = &(map_items.at(item_name));


					   if(!( current_item->turnon.print.empty()))
					   {
						   std::cout <<  current_item->turnon.print << std::endl;
					   }
					   for(string act: current_item->turnon.actions)
					   {
						   run_action(act);
					   }

				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }


			   else if(std::regex_match(input,item_m,pickup_p))
			   {
				   string item_name = item_m[1];

					   //std::cout << item_name << std::endl;

					   if(std::find(current_room->item.begin(), current_room->item.end(), item_name) != current_room->item.end()) //checks if item is in item vector of current room
					   {
						   std::cout << "Item "<< item_name << " added to inventory " <<std::endl;
						   current_item = &(map_items.at(item_name));
						   inventory.push_back(item_name);

						   current_room->item.erase(std::remove(current_room->item.begin(), current_room->item.end(), item_name), current_room->item.end());

						   run_action("delete "+ item_name);

					   }

					   else
					   {
						   std::cout << "Error" << std::endl;
					   }
			   }

			   else if(std::regex_match(input,item_m,read_item_p))
			   {
				   string item_name = item_m[1];
				   //std::cout << item_name << std::endl;

				   if(std::find(inventory.begin(), inventory.end(), item_name) != inventory.end()) //checks if item is in inventory vector
				   {

					   if(!( current_item->writing.empty()))
					   {
						   std::cout <<  current_item->writing << std::endl;
					   }

					   else
					   {
						   std::cout <<  "nothing is written" << std::endl;
					   }

				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }

			   else if(std::regex_match(input,item_m,drop_item_p))
			   {
				   string item_name = item_m[1];
				   //std::cout << item_name << std::endl;

				   if(std::find(inventory.begin(), inventory.end(), item_name) != inventory.end()) //checks if item is in inventory vector
				   {

					   current_room->item.push_back(item_name);
					   inventory.erase(std::remove(inventory.begin(), inventory.end(), item_name), inventory.end());
					   std::cout << "dropped " << item_name << std::endl;
				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }

			   else if(input.compare("open exit") == 0)
			   {

				   if(current_room->type.compare("exit") == 0)
				   {
					   std::cout << "Game Over" << std::endl;
					    gameover = 1;
				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }


			   else if(std::regex_match(input,container_m,open_container_p))
			   {
				   string container_name = container_m[1];
				   //std::cout << item_name << std::endl;
				   Container * the_container;


				   if(std::find(current_room->container.begin(), current_room->container.end(), container_name) != current_room->container.end()) //checks if is in room
				   {
					   the_container = &(map_containers.at(container_name));

					   if(!(the_container->item.empty()))
					   {


						   for(string item_inside : the_container->item)
						   {
								   std::cout << container_name << " contains " << item_inside << std::endl;
								   current_room->item.push_back(item_inside);
								   //the_container->item.erase(std::remove(current_room->container.begin(), current_room->container.end(), item_inside), current_room->container.end());
						   }
						   //the_container->item.clear();
					   }
					   else
					   {
						   std::cout << container_name << " is empty."<< std::endl;
					   }

				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }



			   else if(std::regex_match(input,item_in_container_m,put_item_p))
			   {
				   string item_name = item_in_container_m[1];
				   string container_name = item_in_container_m[2];

				   Container * the_container;

				   int in_inventory;

				   //std::cout << item_name << std::endl;
				   in_inventory = (std::find(inventory.begin(), inventory.end(), item_name) != inventory.end());


				   if((std::find(current_room->container.begin(), current_room->container.end(), container_name) != current_room->container.end()) && in_inventory) //checks if is in room
				   {
					   the_container = &(map_containers.at(container_name));

					   	   	   if(the_container->accept.size() == 0)
					   	   	   {
					   	   		//std::cout << "accept size zero" << std::endl;
					   	   		the_container->item.push_back(item_name);
					   	   		inventory.erase(std::remove(inventory.begin(), inventory.end(), item_name), inventory.end());
					   	   		std::cout << "added "<<item_name << " to "<< container_name<< std::endl;

					   	   	   }
					   	   	   else if(std::find(the_container->accept.begin(), the_container->accept.end(), item_name) != the_container->accept.end())
							   {
					   	   		the_container->item.push_back(item_name);
					   	   		inventory.erase(std::remove(inventory.begin(), inventory.end(), item_name), inventory.end());
					   	   		std::cout << "put "<<item_name << " in "<< container_name<< std::endl;
							   }
					   	   	   else
					   	   	   {
					   	   		std::cout << "Error" << std::endl;
					   	   	   }

				   }

				   else
				   {
					   std::cout << "Error" << std::endl;
				   }
			   }




		   }

}


void run_action(string act)
{
	regex i_drop_p("drop ([a-zA-z]+)");
	regex i_add_p("add ([a-zA-z]+) to ([a-zA-z]+)");
	regex i_delete_p("delete ([a-zA-z]+)");
	regex i_update_p("update ([a-zA-z]+) to ([a-zA-z]+)");
	regex i_gameover_p("gameover");

	smatch objects_m;
	Room* the_room;
    Container* the_container;
    Item* the_item;
    Creature* the_creature;

	   if(std::regex_match(act,objects_m,i_drop_p))
	   {
		   string item_name = objects_m[1];
		   //std::cout << item_name << std::endl;

		   if(std::find(inventory.begin(), inventory.end(), item_name) != inventory.end()) //checks if item is in inventory vector
		   {

			   current_room->item.push_back(item_name);
			   inventory.erase(std::remove(inventory.begin(), inventory.end(), item_name), inventory.end());
			   std::cout << "dropped " << item_name << std::endl;
		   }
	   }

	   if(std::regex_match(act,objects_m,i_add_p))
	   {
		   string objects_name = objects_m[1];
		   string room_container = objects_m[2];




		   int item_or_creature; //1 for item, 0 for creature

		   //std::cout << item_name << std::endl
		   if(map_items.find(objects_name) == map_items.end())
			{
				item_or_creature = 0;
			}
			else
			{
				item_or_creature = 1;
			}



		   if(map_rooms.find(room_container) == map_rooms.end()) // if true, then it is a container. Else it is a room
		   {
			   the_container = &(map_containers.at(room_container));

			   the_container->item.push_back(objects_name);

		   }

		   else
		   {

			   the_room = &(map_rooms.at(room_container));
			   if(item_or_creature)
			   {
				   the_room->item.push_back(objects_name);
			   }
			   else
			   {
				   the_room->item.push_back(objects_name);
			   }


		   }





	}

	if(std::regex_match(act,objects_m,i_delete_p))
	   {
		   string objects_name = objects_m[1];

		   Room * room_loop;
		   Container * container_loop;




		    if(map_rooms.find(objects_name) != map_rooms.end())
			{
				for (auto const& room_loop1: map_rooms)
				{
					room_loop = &(map_rooms.at(room_loop1.first));
					for(Border& b : room_loop->border)
					{

						if(b.name.compare(objects_name) == 0)
						{
							b.name = "destroyed";
							b.direction = "destroyed";
						}
					}
				}
			}

			else if(map_items.find(objects_name) != map_items.end())
			{

				for ( auto const& room_loop1: map_rooms)
				{

					room_loop = &(map_rooms.at(room_loop1.first));

					if(std::find(room_loop->item.begin(), room_loop->item.end(), objects_name) != room_loop->item.end()) //checks if item is in item vector of the current room
				   {
					    //std::cout << "destroyed "<< objects_name <<std::endl;
						room_loop->item.erase(std::remove(room_loop->item.begin(), room_loop->item.end(), objects_name), room_loop->item.end());
				   }
				}

				for (auto const& container_loop1: map_containers)
				{
					container_loop = &(map_containers.at(container_loop1.first));
					if(std::find(container_loop->item.begin(), container_loop->item.end(), objects_name) != container_loop->item.end()) //checks if item is in item vector of the current container
				   {
						//std::cout << "destroyed "<< objects_name <<std::endl;
						container_loop->item.erase(std::remove(container_loop->item.begin(), container_loop->item.end(), objects_name), container_loop->item.end());
				   }
				}

			}
			else if(map_containers.find(objects_name) != map_containers.end())

			{
				for ( auto const& room_loop1: map_rooms)
				{
					room_loop = &(map_rooms.at(room_loop1.first));
					if(std::find(room_loop->container.begin(), room_loop->container.end(), objects_name) != room_loop->container.end()) //checks if container is in container vector of the current room
				   {
					   //std::cout << "container "<< container_name << " added to inventory " <<std::endl;
						room_loop->container.erase(std::remove(room_loop->container.begin(), room_loop->container.end(), objects_name), room_loop->container.end());
				   }
				}
			}
			else if(map_creatures.find(objects_name) != map_creatures.end())
			{
				for ( auto const& room_loop1: map_rooms)
				{
					room_loop = &(map_rooms.at(room_loop1.first));
					if(std::find(room_loop->creature.begin(), room_loop->creature.end(), objects_name) != room_loop->creature.end()) //checks if creature is in creature vector of the current room
				   {
					   //std::cout << "creature "<< creature_name << " added to inventory " <<std::endl;
						room_loop->creature.erase(std::remove(room_loop->creature.begin(), room_loop->creature.end(), objects_name), room_loop->creature.end());
				   }
				}
			}

	   }

	 if(std::regex_match(act,objects_m,i_update_p))
	   {
		   string objects_name = objects_m[1];
		   string status_name = objects_m[2];

		   if(map_rooms.find(objects_name) != map_rooms.end())
			{
				the_room = &(map_rooms.at(objects_name));
				the_room->status = status_name;
			}

			else if(map_items.find(objects_name) != map_items.end())
			{
				the_item = &(map_items.at(objects_name));
				the_item->status = status_name;
			}

			else if(map_containers.find(objects_name) != map_containers.end())
			{
				the_container = &(map_containers.at(objects_name));
				the_container->status = status_name;
			}

			if(map_creatures.find(objects_name) != map_creatures.end())
			{
				the_creature = &(map_creatures.at(objects_name));
				the_creature->status = status_name;
			}

	   }

	 if(std::regex_match(act,objects_m,i_gameover_p))
	 {
		 gameover = 1;
	 }


}
