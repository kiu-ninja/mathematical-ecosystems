using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cell : MonoBehaviour
{
    public bool alive = false;

    public void Reverse()
    {
        if (alive == true) SetAlive(false);
        else SetAlive(true);
    }

    public void SetAlive (bool alive)
    {
        this.alive = alive; 
        
        if (alive)
        {
            GetComponent<SpriteRenderer>().enabled = true;
        }
        else
        {
            GetComponent<SpriteRenderer>().enabled = false;
        }
    }
}
