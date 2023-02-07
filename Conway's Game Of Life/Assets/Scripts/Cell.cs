using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cell : MonoBehaviour
{
    public bool alive = false;
    public bool possibleValue = false;

    public void Awake()
    {
        SetAlive(false);
    }

    public void Reverse()
    {
        SetAlive(!alive);
    }

    public void SetAlive (bool alive)
    {
        GetComponent<SpriteRenderer>().enabled = this.alive = possibleValue = alive;
    }
}
