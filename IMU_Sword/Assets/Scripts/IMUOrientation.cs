using UnityEngine;
using UnityEngine.UI;

public class IMUOrientation : MonoBehaviour 
{
	float checksum;
	[Range(0,1)]
	public float rotationSpeed = 0.5f;
	public bool useUDP = false;

	public Text systemCalibText;
	public Text accelCalibText;
	public Text gyroCalibText;
	public Text magnCalibText;

	Color[] calibStatusColors = { Color.red, Color.yellow, Color.blue, Color.green };

	void Start()
	{
		transform.rotation = Quaternion.identity;
	}

	void Update()
	{
		string latestFrame;
		if (useUDP) {
			latestFrame = UDPManager.Instance.GetLatestFrame ();
		} else {
			latestFrame = SerialCommManager.Instance.GetLatestFrame ();
		}

		if (latestFrame != null)
		{
			int calibStatus = 0;
			Quaternion hsQuat = ParseRotation(latestFrame, ref calibStatus);
			UpdateCalibrationText (calibStatus);
			if (hsQuat != Quaternion.identity)
			{
				transform.rotation = Quaternion.Slerp(transform.rotation, hsQuat, rotationSpeed);
			}
		}
	}

	Quaternion ParseRotation(string frame, ref int calibrationStatus)
	{
		Quaternion rotation = Quaternion.identity;
		string[] tokens = frame.Split (',');

		float w = float.Parse (tokens [1]);
		float x = float.Parse (tokens [2]);
		float y = float.Parse (tokens [3]);
		float z = float.Parse (tokens [4]);

		rotation.Set (x, y, z, w);
		calibrationStatus = int.Parse(tokens[tokens.Length - 3]);
		checksum = Mathf.Sqrt (w*w + x*x + y*y + z*z);
		if(Mathf.Abs(checksum - float.Parse(tokens[tokens.Length-2])) < 0.1)
		{
			Quaternion rotationAdjust = Quaternion.identity;
			// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/examples/index.htm
			// 90 degree adjustment because of the mounting orientation
			rotationAdjust.Set(0.7071f, 0, 0, 0.7071f);
			return rotationAdjust*rotation;
		}
		return transform.rotation;
	}

    void UpdateCalibrationText(int calibrationStatus)
    {
		systemCalibText.color = calibStatusColors[calibrationStatus & 3];
		accelCalibText.color = calibStatusColors[(calibrationStatus>>2) & 3];
		gyroCalibText.color = calibStatusColors[(calibrationStatus >> 4) & 3];
		magnCalibText.color = calibStatusColors[(calibrationStatus >> 6) & 3];
    }
}
