using System;
using Unity.VisualScripting;
using UnityEngine;

public class Simulation : MonoBehaviour
{
    public bool[,] map = null;
    public int depth = 100;
    public int rule = 30;

    public void Start()
    {
        init();
        Draw();
    }

    private void init()
    {
        map = new bool[depth, 2 * depth - 1];

        map[0, depth - 1] = true;
        for (int i = 0; i < depth - 1; i++)
        {
            for (int j = 0; j < 2 * depth - 3; j++)
            {
                int operationType = 0;

                if (map[i , j]) operationType += 4;
                if (map[i, j + 1]) operationType += 2;
                if (map[i, j + 2]) operationType += 1;

                operationType = (int)Math.Pow(2, operationType);

                int tempRule = rule;
                if (operationType != 0)
                {
                    tempRule = tempRule / operationType;
                }
                if (tempRule % 2 == 1)
                {
                    map[i + 1, j + 1] = true;
                }
            }
        }
    }

    private void Draw()
    {

        Texture2D texture = new Texture2D(depth * 16 - 8, depth * 8);
        Sprite sprite = Sprite.Create(texture, new Rect(0, 0, depth * 16 - 9, depth * 8 - 1), Vector2.zero);
        GetComponent<SpriteRenderer>().sprite = sprite;

        for (int y = 0; y < depth; y++)
        {
            for (int x = 0; x < 2 * depth - 1; x++) //Goes through each pixel
            {
                Color pixelColour;
                if (map[depth - y - 1,x]) 
                {
                    pixelColour = new Color(0, 0, 0, 1);
                }
                else
                {
                    pixelColour = new Color(1, 1, 1, 1);
                }
                for(int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        texture.SetPixel(8 * x + i, 8 * y + j, pixelColour);
                    }
                }
            }
        }
        transform.position = sprite.rect.position;
       // Camera.main.transform.position = sprite.rect.position;
        texture.Apply();
    }
}
