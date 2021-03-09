using UnityEngine;

public class GameManager : MonoBehaviour
{
    private static GameManager privateInstance;
    public static GameManager instance
    {
        get
        {
            if (privateInstance == null)
            {
                privateInstance = FindObjectOfType<GameManager>();
                if (privateInstance == null)
                {
                    GameObject obj = new GameObject();
                    obj.name = typeof(GameManager).Name;
                    privateInstance = obj.AddComponent<GameManager>();
                }
            }
            return privateInstance;
        }
    }

    void Awake()
    {

    }
    void Start()
    {
       
    }
    void Update()
    {
    }
}