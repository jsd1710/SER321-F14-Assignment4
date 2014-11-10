#include <jsonrpc/rpc.h>
#include <jsonrpc/json/json.h>
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

   virtual bool add(const double& latInput, const double& lonInput, const double& eleInput, const string& nameInput);
   virtual bool addWaypoint(const Json::Value& waypoint);
   virtual bool removeWaypoint(const string& nameInput);
   virtual Json::Value getWaypoint(const string& name);
   virtual Json::Value getWaypoints();
   
   virtual double getDistanceGCTo(const string& w1, const string& w2);
   virtual double getBearingGCInitTo(const string& w1, const string& w2);

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

bool WaypointServer::add(const double& latInput, const double& lonInput, const double& eleInput, const std::string& nameInput)
{
	Waypoint temp(latInput, lonInput, eleInput, nameInput);
	bool added = waypoints.insert(pair<string,Waypoint>(nameInput,temp)).second;
	if (added)
	{
		cout << "Added:		" << "Waypoint(" << latInput << ", " << lonInput << ", " << eleInput << ", " << nameInput << ");" << endl;
	}
	return added;
}

bool WaypointServer::addWaypoint(const Json::Value& waypoint)
{
	string name(waypoint.get("name", "ERROR").asString());
	double lat(waypoint.get("lat", 0).asDouble());
	double lon(waypoint.get("lon", 0).asDouble());
	double ele(waypoint.get("ele", 0).asDouble());
	Waypoint temp(lat,lon,ele,name);

	bool added = waypoints.insert(pair<string,Waypoint>(name,temp)).second;
	if (added)
	{
		cout << "Added:		" << "Waypoint(" << lat << ", " << lon << ", " << ele << ", " << name << ");" << endl;
	}
	return added;
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

Json::Value WaypointServer::getWaypoint(const string& name)
{
	if (waypoints.find(name) != waypoints.end())
	{
		Waypoint temp(waypoints.at(name));
		string name = temp.name;
		double lat = temp.lat;
		double lon = temp.lon;
		double ele = temp.ele;

		std::ostringstream waypointStringStream;
		waypointStringStream << "{\"name\":" << name << ","
				<< "\"lat\":" << lat << ","
				<< "\"lon\":" << lon << ","
				<< "\"ele\":" << ele << "}";
		std::string waypointString = waypointStringStream.str();

		Json::Value jsonWaypoint(waypointString);
		return jsonWaypoint;

	}
	else
	{
		cout << "Error:		There was no '" << name << "' Waypoint!" << endl;
		Json::Value result("{\"result\":\"ERROR\"}");
		return result;
	}
}

Json::Value WaypointServer::getWaypoints()
{
	Json::Value waypointJSONObject;
	Json::Value waypointJSONArray;
	waypointJSONArray.append(Json::Value::null);
	waypointJSONArray.clear();
	waypointJSONObject["waypoints"] = waypointJSONArray;

	for (map<string,Waypoint>::iterator itr = waypoints.begin(); itr != waypoints.end(); itr++)
	{
		Waypoint temp(itr->second);
		string name = temp.name;
		double lat = temp.lat;
		double lon = temp.lon;
		double ele = temp.ele;

		Json::Value jsonWaypoint;
		jsonWaypoint["name"] = name.c_str();
		jsonWaypoint["lat"] = lat;
		jsonWaypoint["lon"] = lon;
		jsonWaypoint["ele"] = ele;

		waypointJSONArray[waypointJSONArray.size()] = jsonWaypoint;
	}
	waypointJSONObject["waypoints"] = waypointJSONArray;

	Json::FastWriter writer;
	Json::Features::strictMode();
	string output = writer.write(waypointJSONObject);
	Json::Value temp(output);

	cout << "SENT JSONArray:	" << temp.asString() << endl;
	return temp;
}

double WaypointServer::getDistanceGCTo(const string& w1, const string& w2)
{
	string name;
	double lat,lon,ele;
	double result;
	if ((waypoints.find(w1) != waypoints.end()) && (waypoints.find(w2) != waypoints.end()))
	{
		Waypoint temp1(waypoints.at(w1));
		Waypoint temp2(waypoints.at(w2));

		result = temp1.distanceGCTo(temp2, 0);

		cout << "SENT Distance:	" << result << "\n" << endl;
		return result;
	}
	else
	{
		cout << "Error:		Could not calculate GC Distance!" << "\n" << endl;
		return 0;
	}
}

double WaypointServer::getBearingGCInitTo(const string& w1, const string& w2)
{
	string name;
	double lat,lon,ele;
	double result;
	if ((waypoints.find(w1) != waypoints.end()) && (waypoints.find(w2) != waypoints.end()))
	{
		Waypoint temp1(waypoints.at(w1));
		Waypoint temp2(waypoints.at(w2));

		result = temp1.bearingGCInitTo(temp2,0);

		cout << "SENT Bearing:	" << result << "\n" << endl;
		return result;
	}
	else
	{
		cout << "EERROR:		Could not calculate GC Bearing!" << "\n" << endl;
		return 0;
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
