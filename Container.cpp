#include <iostream>
#include "Container.h"

Container::Container(xml_node<> * node){

	string tempString;

			for(xml_node<> *rnodes = node->first_node(); rnodes; rnodes=rnodes->next_sibling())
			{


					tempString = rnodes->name();

					std::cout << "	" << tempString << "  "<< rnodes->value()  <<std::endl;

				   if(tempString.compare("name") == 0)
				   {
					  name = rnodes->value();
				   }

				   else if(tempString.compare("status") == 0)
				   {
					   status = rnodes->value();
				   }

				   else if(tempString.compare("description") == 0)
				   {
					   description = rnodes->value();
				   }

				   else if(tempString.compare("accept") == 0)
				   {
					   accept.push_back(rnodes->value());
				   }

				   else if(tempString.compare("item") == 0)
				   {
					   item.push_back(rnodes->value());
				   }

				   else if(tempString.compare("trigger") == 0)
				   {
					   Trigger strigger(rnodes);
					   trigger.push_back(strigger);
				   }


			}





}

Container::~Container( ){

}
