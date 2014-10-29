package cst420.jsonrpc;

/**
 * Purpose: demonstrate connecting to a CPP JsonRpc http service
 * @author Tim Lindquist (tim.lindquist@asu.edu), ASU Software Engineering
 * @version July 2014
 */
public interface Calculate {
   public double plus(double left, double right);
   public double minus(double left, double right);
   public double times(double left, double right);
   public double divide(double left, double right);
   public String serviceInfo();
}
