/* Q1: Consider a source node, a destination node, and an intermediate router (respectively as shown in Fig.1.1). The link between nodes S and R (Link-1) has a bandwidth of 1Mbps and 50ms latency. The link between nodes R and D (Link-2) has a bandwidth of 100kbps and 5ms latency. Vary the Max Packet as 6, Interval as 1.0 and packet size as 1024. Show the results of the simulation on the console. 
*/

//n0-----n1----n2----n3----n4
//src     intermediates   dst


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("lab1q1");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (5);

  //S and R1 (link1)
  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("50ms"));

  //R1 and R2 (link2)
  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("100kbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("5ms"));

  //R2 and R3 (link3)
  PointToPointHelper pointToPoint3;
  pointToPoint3.SetDeviceAttribute ("DataRate", StringValue ("100kbps"));
  pointToPoint3.SetChannelAttribute ("Delay", StringValue ("5ms"));

    //R3 and D (link4)
  PointToPointHelper pointToPoint4;
  pointToPoint4.SetDeviceAttribute ("DataRate", StringValue ("100kbps"));
  pointToPoint4.SetChannelAttribute ("Delay", StringValue ("5ms"));

  NetDeviceContainer device1 = pointToPoint1.Install (nodes.Get(0), nodes.Get(1));
  NetDeviceContainer device2 = pointToPoint2.Install (nodes.Get(1), nodes.Get(2));
  NetDeviceContainer device3 = pointToPoint1.Install (nodes.Get(2), nodes.Get(3));
  NetDeviceContainer device4 = pointToPoint1.Install (nodes.Get(3), nodes.Get(4));

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4AddressHelper address3;
  address3.SetBase ("10.1.4.0", "255.255.255.0");
  
  Ipv4InterfaceContainer if1 = address.Assign (device1);
  Ipv4InterfaceContainer if2 = address1.Assign (device2);
  Ipv4InterfaceContainer if3 = address2.Assign (device3);
  Ipv4InterfaceContainer if4 = address3.Assign (device4);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get(4));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  

  UdpEchoClientHelper echoClient (if4.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
 
  AnimationInterface anim("5nodes.xml"); 
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
