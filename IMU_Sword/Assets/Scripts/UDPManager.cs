using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Text;

public class UDPManager : Singleton<UDPManager> 
{
	public int udpPort = 12345;

	static UdpClient udp;
	string returnData;

	Thread thread;
	static readonly object lockObject = new object();
	bool processData = false;

	void Start () 
	{
		thread = new Thread(new ThreadStart(ReceiveThread));
		thread.Start();	
	}

	void Update()
	{
		if (processData)
		{
			lock (lockObject)
			{
				processData = false;
				//Debug.Log("Received: [" + returnData+"]");
			}
		}
	}

	void ReceiveThread()
	{
		udp = new UdpClient (udpPort);
		while (true)
		{
			IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);

			byte[] receiveBytes = udp.Receive(ref RemoteIpEndPoint);

			lock (lockObject)
			{
				string incomingString = Encoding.ASCII.GetString(receiveBytes);
				returnData = incomingString;

				//Debug.Log(returnData);
				processData = true;
			}
		}
	}

	public string GetLatestFrame()
	{
		return returnData;
	}
}
