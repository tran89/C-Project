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

string input;



void run_action(string);
bool if_string_in_vec(string s, vector<string> the_vector);
bool check_condition(Condition c);
void run_trigger_vector( vector<Trigger> &trig_vec);
void check_triggers();

int gameover = 0;
bool trigger_override = false;

int main(void)
{



	ifstream myfile("src/theSample.xml");
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

		  //cout << "testing" <<std::endl;

		   //Room * current_room;

		   current_room = &(map_rooms.at("Entrance"));
		   Item * current_item;


		   //string input;
		   int errorFlag = 0;
		   string nextRoom;

		   regex turnon_p("turn on ([a-zA-z]+)");
		   smatch item_m;
		   smatch container_m;
		   smatch item_in_container_m;
		   smatch creature_with_item;


		   regex open_container_p("open ([a-zA-z]+)");
		   regex pickup_p("take ([a-zA-z]+)");
		   regex read_item_p("read ([a-zA-z]+)");
		   regex drop_item_p("drop ([a-zA-z]+)");
		   regex put_item_p("put ([a-zA-z]+) in ([a-zA-z]+)");
		   regex attack_creature_p("attack ([a-zA-z]+) with ([a-zA-z]+)");

		   while(!gameover)
		   {

			   if(!(current_room->description.empty()) && !errorFlag)
			   std::cout << current_room->description <<std::endl;
			   errorFlag = 1;


			   std::getline(std::cin,input);

			   trigger_override = false;

			   check_triggers();

			   if(trigger_override)
			   {

			   }
			   else if(input.length() == 1 && ((input[0] == 'n')||(input[0] == 's')||(input[0] == 'e')||(input[0] == 'w')))
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


					   if(input.compare(b.direction) == 0)
					   {
						   errorFlag = 0;
						   nextRoom = b.name;
						   current_room = &(map_rooms.at(nextRoom));

						   //std::cout << "typed: " << input << "to go to"  << nextRoom << std::endl;
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

						   run_action("Delete "+ item_name);

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


			   else if(std::regex_match(input,creature_with_item,attack_creature_p))
			   {
				   string creature_name = creature_with_item[1];
				   string item_name = creature_with_item[2];

				   //std::cout << item_name << std::endl;
				   Creature * the_creature;
				   Attack * the_attack;

				   //std::cout << "You assaulted the "<< creature_name << " with the " << item_name << std::endl;

				   if(if_string_in_vec(item_name, inventory) == false)
				   {
					   std::cout << "Error no item" << std::endl;
				   }
				   else if(if_string_in_vec(creature_name, current_room->creature) == false)
				   {
					   std::cout << "Error no creature" << std::endl;
				   }
				   //else if(if_string_in_vec(item_name, the_creature->vulnerability) == false)
				   //{
					//   std::cout << "Error not vulnerable" << std::endl;
				   //}
				   else
				   {

					   the_creature = &(map_creatures.at(creature_name));
					   the_attack = &(the_creature->attack);

					   //std::cout << "You assaulted the "<< creature_name << " with the " << item_name << std::endl;

					   if((if_string_in_vec(item_name, the_creature->vulnerability) == false))
					   {
						   std::cout << "Error not vulnerable" << std::endl;
					   }

					   else if(the_attack->condition.owner.empty())
					   {
						   std::cout << "You assaulted the "<< creature_name << " with the " << item_name << std::endl;


						   //the_attack = &(the_creature->attack);


						   if(!(the_attack->print.empty()))
						   {
							   std::cout << the_attack->print << std::endl;
						   }

						   for (string act: the_attack->actions)
							   {
							   run_action(act);
							   }
					   }

					   else
					   {
						   if(check_condition(the_attack->condition))
						   {
							   std::cout << "You assaulted the "<< creature_name << " with the " << item_name << std::endl;

							   if(!(the_attack->print.empty()))
							   {
								   std::cout << the_attack->print << std::endl;
							   }

							   for (string act: the_attack->actions)
								   {
								   run_action(act);
								   }
						   }
						   else
						   {
							   std::cout << "Error" << std::endl;
						   }
					   }


				   }


			   }
			   if(!trigger_override)
			 	{
				   check_triggers();
			 	}
			//check_triggers();


		   }

}


bool if_string_in_vec(string s, vector<string> the_vector)
{
	//std::cout << "vuln vector" << s << std::endl;
	return(std::find(the_vector.begin(), the_vector.end(), s) != the_vector.end());
}

void run_action(string act)
{

	//std::cout << "meanwhile... " << act << std::endl;

	regex i_drop_p("Drop ([a-zA-z]+)");
	regex i_add_p("Add ([a-zA-z]+) to ([a-zA-z]+)");
	regex i_delete_p("Delete ([a-zA-z]+)");
	regex i_update_p("Update ([a-zA-z]+) to ([a-zA-z]+)");
	regex i_gameover_p("Game Over");

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
				//std::cout << "is creature" << std::endl;

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
			   //std::cout << "adding to room" << std::endl;

			   the_room = &(map_rooms.at(room_container));
			   if(item_or_creature)
			   {
				   the_room->item.push_back(objects_name);
			   }
			   else
			   {
				   the_room->creature.push_back(objects_name);
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
		 std::cout << "Victory!"<<std::endl;

	 }

	 if(act.length() == 1 && ((act[0] == 'n')||(act[0] == 's')||(act[0] == 'e')||(act[0] == 'w')))
	 {


	    if(act.compare("n") == 0)
	    {
	 	   act = "north";
	    }
	    else if(act.compare("s") == 0)
	    {
	 	   act = "south";
	    }
	    else if(act.compare("w") == 0)
	    {
	 	   act = "west";
	    }
	    else if(act.compare("e") == 0)
	    {
	 	   act = "east";
	    }
	    for(Border b : current_room->border)
	    {

	 	   //std::cout << b.direction << " vs "<< act << std::endl;
	 	   if(act.compare(b.direction) == 0)
	 	   {
	 		   current_room = &(map_rooms.at(b.name));

	 		  if(!(current_room->description.empty()))
	 		 	 	  {
	 		 	 	  std::cout << current_room->description <<std::endl;
	 		 	 	  }
	 	   }




	    }

	 }


}


bool check_condition(Condition c)
{
	bool condition_return = false;

	string the_object = c.object;

	Room* c_room;
	Item* c_item;
	Container * c_container;
	Creature * c_creature;

	if(!(c.owner.empty())) //has, owner,object
	{

		if(c.owner.compare("inventory") == 0)
		{
			if(c.has.compare("yes") == 0)
				{
				condition_return = if_string_in_vec(the_object, inventory);
				}
			else
				{
				condition_return = !(if_string_in_vec(the_object, inventory));
				}
		}
		else if(map_containers.find(c.owner) != map_containers.end())
			{
				c_container = &(map_containers.at(c.owner));

				if(c.has.compare("yes") == 0)
					{
					condition_return = if_string_in_vec(the_object, c_container->item);
					}
				else
					{
					condition_return = !(if_string_in_vec(the_object, c_container->item));
					}

			}
	}
	else // object, status
	{
		if (map_rooms.find(the_object) != map_rooms.end())
			{
				c_room = &(map_rooms.at(the_object));
				condition_return = (c.status.compare(c_room->status) == 0);
			}
		else if (map_items.find(the_object) != map_items.end())
			{


				c_item = &(map_items.at(the_object));

				condition_return = (c.status.compare(c_item->status) == 0);

				//std::cout << "found creature ITEM trigger return:" << c.status << "vs" << c_item->status <<std::endl;
				//std::cout << "condition_return:" << condition_return  <<std::endl;

			}
		else if (map_containers.find(the_object) != map_containers.end())
			{
				c_container = &(map_containers.at(the_object));
				condition_return = (c.status.compare(c_container->status) == 0);
			}
		else if (map_creatures.find(the_object) != map_creatures.end())
			{
				c_creature = &(map_creatures.at(the_object));
				condition_return = (c.status.compare(c_creature->status) == 0);
			}

	}

	return condition_return;
}

void check_triggers()
{
	/*
	int room_trig_status = 1;

	for(Trigger& trig: current_room->trigger)
	{
		for(Condition& cond: trig.condition)
			if(!check_condition(cond))
			{
				room_trig_status = 0;
			}
		if(!(if_string_in_vec(input, trig.commands)))
			{
				room_trig_status = 0;
			}

		if(room_trig_status)
			{
				if(!(trig.print.empty()))
				std::cout << trig.print <<std::endl;

				for(string act:trig.actions)
				{
					run_action(act);
				}


			}



	}
	*/



	run_trigger_vector(current_room->trigger);












	Item* trig_item;
	Creature* trig_creat;
	Container* trig_contain;

	for(string itm: inventory)
	{
		trig_item = &(map_items.at(itm));
		if (!(trig_item->trigger.empty()))
		{
			run_trigger_vector(trig_item->trigger);
		}

	}

	for(string creat: current_room->creature)
	{
		trig_creat = &(map_creatures.at(creat));
		if (!(trig_creat->trigger.empty()))
		{
			run_trigger_vector(trig_creat->trigger);
			//std::cout << "found creature trigger"  <<std::endl;
		}


	}

	for(string contain: current_room->container)
	{
		trig_contain = &(map_containers.at(contain));
		if (!(trig_contain->trigger.empty()))
		{
			run_trigger_vector(trig_contain->trigger);
		}

		for(string itm: trig_contain->item)
		{
			trig_item = &(map_items.at(itm));
			if (!(trig_item->trigger.empty()))
			{
				run_trigger_vector(trig_item->trigger);
			}

		}

	}




}

void run_trigger_vector(vector<Trigger> &trig_vec)
{
	int trig_bool = 1;

	for(Trigger& trig: trig_vec)
		{
			//if((trig.type.compare("permanent")== 0) || (trig.type.compare("single")== 0))
			if((trig.type.compare("done")!= 0))
			{
				//std::cout << "trigger activated" <<std::endl;

				for(Condition& cond: trig.condition)
					if(!check_condition(cond))
					{
						trig_bool = 0;
						//std::cout << "CHECK CONDTIONS:" << trig_bool <<std::endl;
					}
				if(!(trig.commands.empty()))
				{
					if(!(if_string_in_vec(input, trig.commands)))
						{
							trig_bool = 0;
							//std::cout << "CHECK COMMANDS:" << trig_bool <<std::endl;
						}
				}
				//std::cout << "trigger bool:" << trig_bool <<std::endl;

				if(trig_bool)
					{
						if(!(trig.print.empty()))
						{
						std::cout << trig.print <<std::endl;
						}

						for(string act:trig.actions)
						{
							run_action(act);
						}
						if(trig.type.compare("permanent")!= 0)
						{
							trig.type = "done";
						}
						trigger_override = true;

					}

			}
		}
}










