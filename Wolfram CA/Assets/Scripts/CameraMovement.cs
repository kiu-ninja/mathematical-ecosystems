using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    void Update()
    {
        if (Input.GetMouseButton(0))
        {
            Vector3 worldPosition = Camera.main.ScreenToViewportPoint(Input.mousePosition);
            Vector3 k = new Vector3(-0.5f, -0.5f, 0);

            transform.position = transform.position + (worldPosition + k) / 10;
        }

        float deltaScroll = Input.mouseScrollDelta.y;
        if (deltaScroll != 0)
        {
            Camera.main.orthographicSize -= (Camera.main.orthographicSize/20)*deltaScroll;
        }
    }
}
