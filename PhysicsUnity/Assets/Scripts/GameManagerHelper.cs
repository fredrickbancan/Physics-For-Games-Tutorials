using UnityEngine;

public class GameManagerHelper : MonoBehaviour
{
    public static bool restarted = false;
    private GameManager gmInstance;
    private void Start()
    {
        gmInstance = GameManager.instance;
    }
}