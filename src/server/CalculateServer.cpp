/**
 * Copyright (c) 2014 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ calculate server using json-rpc-cpp.
 * This class provides methods to providing remote arithmetic functions.
 * Meant to run on OSX, Cygwin, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Cst420
 * @author Tim Lindquist (Tim.Lindquist@asu.edu),ASU-IAFSE,Software Engineering
 * @file    CalculateServer.cpp
 * @date    July, 2014
 * @license See above
 **/
#include <jsonrpc/rpc.h>
#include <iostream>
#include <stdlib.h>

#include "abstractcalculatestubserver.h"

using namespace jsonrpc;
using namespace std;

class CalculateServer : public AbstractcalculatestubServer {
public:
   CalculateServer(int port);

   virtual std::string serviceInfo();
   virtual double plus(const double& param1, const double& param2);
   virtual double minus(const double& param1, const double& param2);
   virtual double times(const double& param1, const double& param2);
   virtual double divide(const double& param1, const double& param2);
private:
   int portNum;
};

CalculateServer::CalculateServer(int port) :
   AbstractcalculatestubServer(new HttpServer(port)){
   portNum = port;
}

string CalculateServer::serviceInfo(){
   std::string msg =
                "Calculate service providing plus,minus,times,divide on port ";
   stringstream ss;
   ss << portNum;
   return  msg.append(ss.str());
}

double CalculateServer::plus(const double& param1, const double& param2) {
   cout << "Requested " << param1 << " + " << param2 << " returning "
        << (param1 + param2) << endl;
   return param1 + param2;
}

double CalculateServer::minus(const double& param1, const double& param2) {
   cout << "Requested " << param1 << " - " << param2 << " returning "
        << (param1 - param2) << endl;
   return param1 - param2;
}

double CalculateServer::times(const double& param1, const double& param2) {
   cout << "Requested " << param1 << " * " << param2 << " returning "
        << (param1 * param2) << endl;
   return param1 * param2;
}

double CalculateServer::divide(const double& param1, const double& param2) {
   cout << "Requested " << param1 << " / " << param2 << " returning "
        << (param1 / param2) << endl;
   return param1 / param2;
}

int main(int argc, char * argv[]) {
   int port = 8080;
   if(argc > 1){
      port = atoi(argv[1]);
   }
   CalculateServer cs(port);
   cs.StartListening();
   getchar();
   cs.StopListening();
   return 0;
}
