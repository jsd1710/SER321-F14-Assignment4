#include "Waypoint.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <math.h> // log

/*
 * Purpose: demonstrate classes, alloc, init, in the context of
 * distance and direction calculations from one earth position (waypoint)
 * to another.
 */
Waypoint::Waypoint()
{
	lat = lon = ele = 0;
	name = "";
}

Waypoint::Waypoint(double aLat, double aLon, double anElevation, string aName)
{
	lat = aLat;
	lon = aLon;
	ele = anElevation;
	name = aName;
}

Waypoint::~Waypoint()
{
	//cout << "Waypoint destructor.\n";
	lat = lon = ele = 0;
	name = "";
}

void Waypoint::setValues(double aLat, double aLon, double anElevation,
		string aName)
{
	lat = aLat;
	lon = aLon;
	ele = anElevation;
	name = aName;
}

double Waypoint::distanceGCTo(Waypoint wp, int scale)
{
	double ret = 0.0;
	double dlatRad = toRadians(wp.lat - lat);
	double dlonRad = toRadians(wp.lon - lon);
	double latOrgRad = toRadians(lat);
	double lonOrgRad = toRadians(lon);
	double a = sin(dlatRad / 2) * sin(dlatRad / 2)
			+ sin(dlonRad / 2) * sin(dlonRad / 2) * cos(latOrgRad)
					* cos(toRadians(wp.lat));
	ret = radiusE * (2 * atan2(sqrt(a), sqrt(1 - a)));
	// ret is in kilometers. switch to either Statute or Nautical?
	switch (scale)
	{
	case STATUTE:
		ret = ret * 0.62137119;
		break;
	case NAUTICAL:
		ret = ret * 0.5399568;
		break;
	}
	return ret;
}

double Waypoint::bearingGCInitTo(Waypoint wp, int scale)
{
	double ret = 0.0;
	double dlatRad = toRadians(wp.lat - lat);
	double dlonRad = toRadians(wp.lon - lon);
	double latOrgRad = toRadians(lat);
	double lonOrgRad = toRadians(lon);
	double y = sin(dlonRad) * cos(toRadians(wp.lat));
	double x = cos(latOrgRad) * sin(toRadians(wp.lat))
			- sin(latOrgRad) * cos(toRadians(wp.lat)) * cos(dlonRad);
	ret = toDegrees(atan2(y, x));
	ret = fmod((ret + 360.0), 360.0);
	return ret;
}

double Waypoint::distanceRhumbTo(Waypoint wp, int scale) {
	double latDiff = toRadians(wp.lat - lat); // Δφ
	double lonDiff = toRadians(wp.lon - lon); // Δλ
	double lat1R = toRadians(lat); // Original latitude
	double lat2R = toRadians(wp.lat); // Destination latitude

	double projectedLatDiff = log(
			tan(pi / 4 + lat2R / 2) / tan(pi / 4 + lat1R / 2));
	double q =
			abs(projectedLatDiff) > 10 * exp(-12) ?
					latDiff / projectedLatDiff : cos(lat1R);

	// if dLon over 180° take shorter rhumb across anti-meridian:
	if (abs(lonDiff) > pi)
		lonDiff = lonDiff > 0 ? -(2 * pi - lonDiff) : (2 * pi + lonDiff);

	double rhumbLineDist = sqrt(latDiff * latDiff + q * q * lonDiff * lonDiff)
			* radiusE;

	return rhumbLineDist;
}

double Waypoint::bearingRhumbTo(Waypoint wp, int scale) {
	double latDiff = toRadians(wp.lat - lat); // Δφ
	double lonDiff = toRadians(wp.lon - lon); // Δλ
	double lat1R = toRadians(lat); // Original latitude
	double lat2R = toRadians(wp.lat); // Desitination latitude

	double projectedLatDiff = log(
			tan(pi / 4 + lat2R / 2) / tan(pi / 4 + lat1R / 2)); // Δψ

	double q =
			abs(projectedLatDiff) > 10 * exp(-12) ?
					latDiff / projectedLatDiff : cos(lat1R);

	// if dLon over 180° take shorter rhumb across anti-meridian:
	if (abs(lonDiff) > pi)
		lonDiff = lonDiff > 0 ? -(2 * pi - lonDiff) : (2 * pi + lonDiff);

	double rhumbBearing = toDegrees(atan2(lonDiff, projectedLatDiff));

	while ((rhumbBearing < 0) || (rhumbBearing > 360)) {
		if (rhumbBearing < 0) {
			rhumbBearing = 360 + rhumbBearing;
		} else if (rhumbBearing > 360) {
			rhumbBearing = 360 - rhumbBearing;
		}
	}

	return rhumbBearing;
}

void Waypoint::print() {
	cout << "Waypoint " << name << " lat " << lat << " lon " << lon << "\n";
}
