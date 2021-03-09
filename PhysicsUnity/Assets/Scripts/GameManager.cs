using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager instance = null;

    private GameObject playerSpawn = null;
    
    [SerializeField]
    private GameObject playerPrefab = null;

    private bool error = false;

    /// <summary>
    /// creating singleton instance
    /// </summary>
    void Awake()
    {
        if(instance)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(this);
            instance = this;
        }
    }

    void Start()
    {
        playerSpawn = GameObject.Find("PlayerSpawn");
        if(!playerSpawn)
        {
            Debug.LogError("GameManager could not find player spawn object!");
            error = true;
            return;
        }
        spawnPlayer();
    }

    public void spawnPlayer()
    {
        if (!playerPrefab)
        {
            Debug.LogError("GameManager could not spawn player! object is provided with null player prefab!");
            error = true;
            return;
        }
        Instantiate(playerPrefab, playerSpawn.transform.position, Quaternion.identity);
    }

    void Update()
    {
        if (error) return;
    }
}