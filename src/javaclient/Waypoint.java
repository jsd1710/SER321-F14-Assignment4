package assignment4;
/**
 * Purpose: demonstrate connecting to a CPP JsonRpc http service
 * @author Tim Lindquist (tim.lindquist@asu.edu), ASU Software Engineering
 * @version July 2014
 */
public interface Waypoint 
{
   public boolean add(double lat, double lon, double ele, String name);
   public boolean addWaypoint(String jsonString);
   public boolean removeWaypoint(String name);
   public String serviceInfo();
}
