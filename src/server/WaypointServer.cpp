#include <jsonrpc/rpc.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <string>

#include "Waypoint.hpp"

#include "abstractwaypointstubserver.h"

using namespace jsonrpc;
using namespace std;

class WaypointServer : public AbstractwaypointstubServer
{
public:
   WaypointServer(int port);

   virtual std::string serviceInfo();

   virtual bool addWaypoint(const double& latInput, const double& lonInput, const double& eleInput, const string& nameInput);
   virtual bool removeWaypoint(const string& nameInput);
private:
   map<string, Waypoint> waypoints;
   int portNum;
};

WaypointServer::WaypointServer(int port) :
   AbstractwaypointstubServer(new HttpServer(port))
{
   portNum = port;
}

string WaypointServer::serviceInfo()
{
   std::string msg =
                "Waypoint Service providing addWaypoint is active on port: ";
   stringstream ss;
   ss << portNum;
   return  msg.append(ss.str());
}

bool WaypointServer::addWaypoint(const double& latInput, const double& lonInput, const double& eleInput, const std::string& nameInput)
{
	Waypoint temp(latInput, lonInput, eleInput, nameInput);
	waypoints.insert(pair<string,Waypoint>(nameInput,temp));
	cout << "Added:		" << "Waypoint(" << latInput << ", " << lonInput << ", " << eleInput << ", " << nameInput << ");" << endl;
	return true;
}
bool WaypointServer::removeWaypoint(const std::string& nameInput)
{
	if (waypoints.find(nameInput) != waypoints.end())
	{
		waypoints.erase(nameInput);
		cout << "Removed:	" << "Waypoint(" << nameInput << ");" << endl;
		return true;
	}
	else
	{
		cout << "Error:		There was no '" << nameInput << "' Waypoint!" << endl;
		return false;
	}
}

int main(int argc, char * argv[])
{
   int port = 8080;

   if(argc > 1)
   {
      port = atoi(argv[1]);
   }

   WaypointServer ws(port);
   ws.StartListening();
   getchar();
   ws.StopListening();
   return 0;
}
