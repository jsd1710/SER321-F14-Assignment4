package assignment4;

import java.io.*;
import java.util.*;
import java.net.URL;
import org.json.*;
import org.json.JSONObject;

public class WaypointJavaClient extends Object implements Waypoint 
{

	public String serviceURL;
	public JsonRpcRequestViaHttp server;
	public static int id = 0;

	public WaypointJavaClient(String serviceURL) 
	{
		this.serviceURL = serviceURL;
		try 
		{
			this.server = new JsonRpcRequestViaHttp(new URL(serviceURL));
		} 
		catch (Exception ex) 
		{
			System.out.println("Malformed URL " + ex.getMessage());
		}
	}

	private boolean packageWaypointCall(String method, String params) 
	{
		boolean result = false;
		try 
		{
			JSONObject jsonObj = new JSONObject();
			jsonObj.put("jsonrpc", "2.0");
			jsonObj.put("method", method);
			jsonObj.put("id", ++id);
			
			String almost = jsonObj.toString();

			String toInsert = ",\"params\":" + params;

			String begin = almost.substring(0, almost.length() - 1);
			String end = almost.substring(almost.length() - 1);

			String ret = begin + toInsert + end;
			System.out.println(ret);
			String resultString = server.call(ret);
			
			JSONObject resultJSON = new JSONObject(resultString);
			result = resultJSON.getBoolean("result");
			
			return result;
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			return false;
		}
	}

	public boolean add(double lat, double lon, double ele, String name) 
	{
		String toInsert = "[" +
				String.valueOf(lat) + 
				", " + String.valueOf(lon) + 
				", " + String.valueOf(ele) + 
				", \"" + name + "\"" +
						"]";
				
		System.out.println(toInsert);
		boolean added = packageWaypointCall("add", toInsert);
		if (added) 
		{
			return true;
		} 
		else 
		{
			System.out.println("Failed to add " + name);
			return false;
		}
	}

	public boolean removeWaypoint(String name) 
	{
		String toInsert = "[\"" + name + "\"]";
		System.out.println(toInsert);
		boolean removed = packageWaypointCall("removeWaypoint", toInsert);
		if (removed) 
		{
			return true;
		} 
		else 
		{
			System.out.println("Failed to remove " + name);
			return false;
		}
	}

	public boolean addWaypoint(String jsonString) 
	{
		JSONObject waypointJSON = new JSONObject(jsonString);
		/*System.out.println(waypointJSON.toString());
		double lat = waypointJSON.getDouble("lat");
		double lon = waypointJSON.getDouble("lon");
		double ele = waypointJSON.getDouble("ele");
		String name = waypointJSON.getString("name");
		*/
		String toInsert = "[" + jsonString + "]";
				
		System.out.println(toInsert);
		
		boolean added = packageWaypointCall("addWaypoint", toInsert);
		if (added) 
		{
			return true;
		} 
		else 
		{
			//System.out.println("Failed to add " + name);
			return false;
		}
	}

	public String serviceInfo() 
	{
		return "Service information";
	}

	public static void main(String args[]) 
	{
		try 
		{
			String url = "http://127.0.0.1:8080/";

			if (args.length > 1) 
			{
				url = args[1];
			}
			
			System.out.print("Input> ");
			WaypointJavaClient wjc = new WaypointJavaClient(url);
			BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));

			

			String inStr = stdin.readLine();
			StringTokenizer st = new StringTokenizer(inStr);
			String opn = st.nextToken();

			while (!opn.equalsIgnoreCase("end")) 
			{
				if (opn.equalsIgnoreCase("add")) 
				{
					boolean result = wjc.add(
							Double.parseDouble(st.nextToken()),
							Double.parseDouble(st.nextToken()),
							Double.parseDouble(st.nextToken()), 
							st.nextToken()
							);
					System.out.println("response: " + result);
				} 
				else if (opn.equalsIgnoreCase("addWaypoint")) 
				{
					boolean result = wjc.addWaypoint(st.nextToken());
					System.out.println("response: " + result);
				} 
				else if (opn.equalsIgnoreCase("removeWaypoint")) 
				{
					boolean result = wjc.removeWaypoint(st.nextToken());
					System.out.println("response: " + result);
				}

				System.out.print("Input>	");

				inStr = stdin.readLine();
				st = new StringTokenizer(inStr);
				opn = st.nextToken();
			}
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			System.out.println("Oops, you didn't enter the right stuff");
		}
	}
}
