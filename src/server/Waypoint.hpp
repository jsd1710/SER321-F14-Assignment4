#include <string>
#include <cmath>
using namespace std;

class Waypoint 
{
protected:
   static constexpr double pi = 3.14159265;
   
   double toRadians(double deg)
   {
      return (deg*pi)/180.0;
   }
   
   double toDegrees(double rad)
   {
      return (rad*180.0)/pi;
   }
   
public:
   static const int STATUTE = 0;
   static const int NAUTICAL = 1;
   static const int KMETER = 2;
   static const int radiusE = 6371;

   double lat;
   double lon;
   double ele;
   string name;

   Waypoint();
   ~Waypoint();
   Waypoint(double aLat, double aLon, double anElevation, string aName);

   void setValues(double aLat, double aLon, double anElevation, string aName);
   
   double distanceGCTo(Waypoint wp, int scale);
   double bearingGCInitTo(Waypoint wp, int scale);
   
   double distanceRhumbTo(Waypoint wp, int scale);
   double bearingRhumbTo(Waypoint wp, int scale);
   
   void print();
};
