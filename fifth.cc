#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"

// Network topology (default)
//
//        n2 n3 n4              .
//         \ | /                .
//          \|/                 .
//     n1--- n0---n5            .
//          /|\                 .
//         / | \                .
//        n8 n7 n6              .
//

using namespace ns3;

int main()
{

  std::string animFile = "fifth.xml";

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
  PointToPointStarHelper star(8, pointToPoint);

  InternetStackHelper stack;
  star.InstallStack(stack);

  star.AssignIpv4Addresses(Ipv4AddressHelper("10.1.1.0", "255.255.255.0"));
  Address hubLocalAddress(InetSocketAddress(Ipv4Address::GetAny(), 50000));
  PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", hubLocalAddress);
  ApplicationContainer hubApp = packetSinkHelper.Install(star.GetHub());
  hubApp.Start(Seconds(1.0));
  hubApp.Stop(Seconds(10.0));

  pointToPoint.EnablePcapAll("star");
  AnimationInterface anim(animFile);
  anim.SetConstantPosition(star.GetHub(), 10.0, 60.0);

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
