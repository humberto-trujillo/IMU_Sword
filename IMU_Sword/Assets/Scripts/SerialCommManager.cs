using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Threading;

public class SerialCommManager : Singleton<SerialCommManager>
{
    public string port = "COM3";
    //public string serialPort = "/dev/cu.usbmodem1411";
    SerialPort stream;
	string returnData;
	Thread thread;

	void Start () 
	{
        stream = new SerialPort(port, 9600);
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
