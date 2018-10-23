#include <iostream>
#include "Room.h"

Room::Room(xml_node<> * node)
{


	string tempString;

	for(xml_node<> *rnodes = node->first_node(); rnodes; rnodes=rnodes->next_sibling()){


		   tempString = rnodes->name();
		   std::cout << "	" << tempString << " "<< rnodes->value() <<std::endl;

		   if(tempString.compare("name") == 0)
		   {
			   name = rnodes->value();
		   }

		   else if(tempString.compare("status") == 0)
		   {
			   status = rnodes->value();
		   }

		   else if(tempString.compare("type") == 0)
		   {
			   type = rnodes->value();
		   }

		   else if(tempString.compare("description") == 0)
		   {
			   description = rnodes->value();
		   }

		   else if(tempString.compare("border") == 0)
		   {
		  			   Border roomBorder;
		  			   string b_tempstring;

		  			 for(xml_node<> *bnodes = rnodes->first_node(); bnodes; bnodes=bnodes->next_sibling()){

		  				 b_tempstring = bnodes->name();
		  				 std::cout << "		" << b_tempstring << "  " << bnodes->value() <<std::endl;

		  				 if(b_tempstring.compare("name") == 0)
		  				 {
		  					 roomBorder.name = bnodes->value();
		  				 }
		  				 else if(b_tempstring.compare("direction") == 0)
		  				 {
		  					roomBorder.direction = bnodes->value();
		  				 }
		  			 }

		  			 border.push_back(roomBorder);

		   }

		   else if(tempString.compare("container") == 0)
		   {
		 		  	container.push_back(rnodes->value());
		   }

		   else if(tempString.compare("item") == 0)
		   {
			   	   item.push_back(rnodes->value());
		   }

		   else if(tempString.compare("creature") == 0)
		   {
			   	   creature.push_back(rnodes->value());
		    }

		   else if(tempString.compare("trigger") == 0)
		  	{
		  		   Trigger strigger(rnodes);
		  		   trigger.push_back(strigger);
		  	}






	}


}

Room::~Room( ) {

}



