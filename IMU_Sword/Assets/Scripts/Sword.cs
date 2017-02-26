using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sword : MonoBehaviour 
{
	public AudioClip[] swordSFX;
	public AudioClip bloodySword;

	AudioSource audioSource;
	AudioClip clip;
	void Start()
	{
		audioSource = GetComponent<AudioSource> ();
	}

	void OnTriggerEnter(Collider other)
	{
		if (other.gameObject.name == "Sword") 
		{
			clip = swordSFX [Random.Range (0, swordSFX.Length)];
			audioSource.PlayOneShot (clip);
		}
		else if(other.gameObject.name == "Enemy")
		{
			clip = bloodySword;
			audioSource.PlayOneShot(clip);
		}
	}
}
