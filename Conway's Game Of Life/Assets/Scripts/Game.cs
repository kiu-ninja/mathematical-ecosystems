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
    
    private float timer = 0f;

    public Cell prefab;

    private Dictionary<int2, Cell> cellMap;

    private int lx = 0;
    private int ly = 0;

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

                    if (!tempCellMap.ContainsKey(newCoordinates))
                    {
                        Cell newCell = Instantiate(prefab, new Vector3(newCoordinates.x, newCoordinates.y, 1), Quaternion.identity);
                        tempCellMap[newCoordinates] = newCell;
                    }
                }
            }
        }

        cellMap = tempCellMap;

        foreach (var item in cellMap)
        {
            Cell cell = item.Value;

            int cnt = GetNeighbours(item.Key);

            if (cnt == 3 && !cell.alive) cell.SetAlive(true);
            else if ((cnt == 2 || cnt == 3) && cell.alive) cell.SetAlive(true);
            else
            {
                cell.SetAlive(false);
            }
        }
    }

    public void ApplyRules(int2 cooridnates, ref Cell cell)
    {
        int cnt = GetNeighbours(cooridnates);

        if (cnt == 3 && !cell.alive) cell.SetAlive(true);
        if ((cnt == 2 || cnt == 3) && cell.alive) cell.SetAlive(true);
        else cell.SetAlive(false);
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

            if(x < 0)
            {
                x--;
            }
            if (y < 0)
            {
                y--;
            }

            if (lx == (int)x && ly == (int)y) return;

            lx = (int)x;
            ly = (int)y;

            updateCell((int)x, (int)y);
            Debug.Log(positionOfNewCell);
        }
    }

    private void updateCell(int x, int y)
    {
        if (cellMap.ContainsKey(new int2(x, y)))
        {
            cellMap[new int2(x, y)].Reverse();
        }
        else
        {
            Cell cell = Instantiate(prefab, new Vector3(x, y, 1), Quaternion.identity);
            cell.SetAlive(true);
            cellMap[new int2(x, y)] = cell;
        }
    }
}
