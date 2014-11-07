#include <jsonrpc/rpc.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "waypointstubclient.h"

using namespace jsonrpc;
using namespace std;

void split(
			vector<string> & aStringVector, /* return value */
			const string & aString,
			const string & aDelimiter
			)
{
	size_t start = 0, end = 0;
	while (end != string::npos)
	{
		end = aString.find(aDelimiter, start);
		// If at end, use length=maxLength.  Else use length=end-start.
		aStringVector.push_back(
								aString.substr(
												start,
												(end == string::npos) ? string::npos : end - start
												)
								);
		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = (
					(end > (string::npos - aDelimiter.size())) ?
						string::npos : end + aDelimiter.size()
				);
	}
}

int main(int argc, char*argv[])
{
	string host = "http://127.0.0.1:8080";

	 if(argc > 1)
	 {
		 host = "http://" + string(argv[1])+ ":" + string(argv[2]);
		 cout << host << endl;
	 }

	waypointstubClient wc(new HttpClient(host));
	try {
		string inLine;
		cout << "Connected to: " << wc.serviceInfo() << endl;

		while (true)
		{
			cout << "Methods: \n"
					"	add lat lon ele name -- eg:add 1 2 3 ASU-Poly \n"
					"	addWaypoint jsonWaypoint -- eg:addWaypoint {\"lat\":1,\"lon\":2,\"ele\":3,\"name\":\"ASU-POLY\"} \n"
					"	removeWaypoint name -- eg:removeWaypoint ASU-Poly \n"
					"Input>	";
			getline(cin, inLine);

			vector<string> tokens;
			split(tokens, inLine, " ");

			if (tokens.size() > 1)
			{
				//Waypoints
				if (tokens[0] == "add")
				{
					double lat, lon, ele;

					string name = tokens[4];
					istringstream latStream(tokens[1]);
					latStream >> lat;
					istringstream lonStream(tokens[2]);
					lonStream >> lon;
					istringstream eleStream(tokens[3]);
					eleStream >> ele;

					wc.add(lat, lon, ele, name);
					cout << "Sent: 	Waypoint(" << lat << ", " << lon << ", " << ele << ", " << name << "); \n" << endl;
				}
				else if (tokens[0] == "addWaypoint")
				{
					Json::Reader reader;
					Json::Value root;

					reader.parse(tokens[1], root);

					wc.addWaypoint(root);
					cout << "Sent: 	Waypoint " << tokens[1] << "\n" << endl;
				}
				else if (tokens[0] == "removeWaypoint")
				{
					string name = tokens[1];

					if (wc.removeWaypoint(name) == true)
					{
						cout << "Removed: 	Waypoint(" << name << "); \n" << endl;
					}
					else
					{
						cout << "Error:	Waypoint(" << name << ") did not exist! \n" << endl;
					}

				}
			}
			else if (tokens[0] == "end")
			{
				break;
			}
		}
	}
	catch (JsonRpcException e)
	{
		cerr << e.what() << endl;
	}
}

