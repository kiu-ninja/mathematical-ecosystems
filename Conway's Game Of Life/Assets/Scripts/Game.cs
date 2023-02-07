using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;

public class Game : MonoBehaviour
{
    public int maxHeight = 20;
    public int maxWidth = 32;
    public bool chooseByRandom = true;
    public float randAlive = 0.8f;
    public float timerMax = 1f;
    public Cell prefab;

    private float timer = 0f;
    private int lx = 0;
    private int ly = 0;

    private Dictionary<int2, Cell> cellMap;

    void Start()
    {
        cellMap = new();

        if (chooseByRandom) ReviveRandomly();
    }

    void Update()
    {
        InsertCellsByMouseClicking();

        timer += Time.deltaTime;
        if (timer < timerMax) return;
        timer = 0;

        Dictionary<int2, Cell> tempCellMap = new();

        foreach (var item in cellMap)
        {
            tempCellMap.Add(item.Key, item.Value);
        }

        foreach (var cellInfo in cellMap)
        {
            for (int k1 = -1; k1 <= 1; k1++)
            {
                for (int k2 = -1; k2 <= 1; k2++)
                {
                    int2 newCoordinates = new int2(cellInfo.Key.x + k1, cellInfo.Key.y + k2);

                    if (!tempCellMap.ContainsKey(newCoordinates) && cellInfo.Value.alive)
                    {
                        Cell newCell = Instantiate(prefab, new Vector3(newCoordinates.x, newCoordinates.y, 1), Quaternion.identity);
                        newCell.SetAlive(false);
                        tempCellMap[newCoordinates] = newCell;
                    }
                }
            }
        }

        cellMap = tempCellMap;

        foreach (var item in cellMap)
        {
            ApplyRules(item.Key, item.Value);
        }

        foreach (var item in cellMap)
        {
            item.Value.SetAlive(item.Value.possibleValue);
        }
    }

    public void ApplyRules(int2 cooridnates, Cell cell)
    {
        int cnt = GetNeighbours(cooridnates);

        if (cell.alive && cnt != 2 && cnt != 3) cell.possibleValue = false;
        else if (!cell.alive && cnt == 3) cell.possibleValue = true;
    }

    public int GetNeighbours(int2 coordinates)
    {
        int cntAliveNeighbours = 0;

        for (int k1 = -1; k1 <= 1; k1++)
        {
            for (int k2 = -1; k2 <= 1; k2++)
            {
                if (k1 == 0 && k2 == 0) continue;

                if (cellMap.ContainsKey(new int2(coordinates.x + k1, coordinates.y + k2))
                    && cellMap[new int2(coordinates.x + k1, coordinates.y + k2)].alive) cntAliveNeighbours++;
            }
        }

        return cntAliveNeighbours;
    }

    private void ReviveRandomly()
    {
        for (int i = -maxWidth; i < maxWidth; i++)
        {
            for (int j = -maxHeight; j < maxHeight; j++)
            {
                float x = UnityEngine.Random.Range(0f, 1f);

                if (x < randAlive)
                {
                    Cell cell = Instantiate(prefab, new Vector3(i, j, 1), Quaternion.identity);
                    cell.SetAlive(true);
                    cellMap[new int2(i, j)] = cell;
                }
            }
        }
    }

    private void InsertCellsByMouseClicking()
    {
        if (Input.GetMouseButton(1))
        {
            Vector3 positionOfClick = Input.mousePosition;
            Vector3 positionOfNewCell = Camera.main.ScreenToWorldPoint(positionOfClick);
            float x = positionOfNewCell.x;
            float y = positionOfNewCell.y;
            
            if (x < 0)
            {
                x--;
            }
            if (y < 0)
            {
                y--;
            }


            if ((lx == (int)x) && (ly == (int)y)) return;

            lx = (int)x;
            ly = (int)y;

            Debug.Log((int)x);
            Debug.Log(lx);
            Debug.Log((int)y);
            Debug.Log(ly);

            updateCell(new int2((int)x, (int)y));
        }
    }

    private void updateCell(int2 coordinates)
    {
        if (cellMap.ContainsKey(coordinates))
        {
            cellMap[coordinates].Reverse();
        }
        else
        {
            Cell cell = Instantiate(prefab, new Vector3(coordinates.x, coordinates.y, 1), Quaternion.identity);
            cell.SetAlive(true);
            cellMap[coordinates] = cell;
        }
    }
}
