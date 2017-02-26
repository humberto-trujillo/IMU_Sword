using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Threading;

public class SerialCommManager : Singleton<SerialCommManager>
{
	SerialPort stream = new SerialPort("/dev/cu.usbmodem1411", 9600);
	string returnData;
	Thread thread;

	void Start () 
	{
		thread = new Thread(new ThreadStart(ReceiveThread));
		thread.Start();	
	}

	void Update () 
	{
		//Debug.Log("Received: [" + returnData+"]");
	}

	void ReceiveThread()
	{
		Debug.Log ("Iniciando thread de comm serial!");
		stream.Open();
		Debug.Log ("Puerto Serial abierto!");
		while (true)
		{
			returnData = stream.ReadLine();
			if (returnData != null) 
			{
				stream.BaseStream.Flush(); //Limpiar stream para recibir nuevo frame
			}
		}
	}

	public string GetLatestFrame()
	{
		return returnData;
	}
}
