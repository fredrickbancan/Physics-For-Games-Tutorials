using UnityEngine;

public class ElevatorTrigger : MonoBehaviour
{
    [SerializeField]
    private GameObject elevatorPlatform = null;

    [SerializeField]
    private float groundFloorHeight = 0.5F;

    [SerializeField]
    private float topFloorHeight = 15.0F;

    [SerializeField]
    private float enterExitTimerSeconds = 1.0F;//number of seconds for the elevator to wait when object enters or exits the trigger before moving

    [SerializeField]
    private float moveSpeed = 1.0F;

    private bool isProgressing = false;

    private bool hasPassenger = false;

    private float touchTimer = 0.0F;//how long an object has been in the trigger

    private CharacterController playerPassenger = null;

    private bool error;

    void Start()
    {
        if (!elevatorPlatform)
        {
            Debug.LogError("Elevator script provided with null elevator platform object!");
            error = true;
        }
    }

    void Update()
    {
        if (error) return;

        if(isProgressing)
        {
            if (topFloorHeight > groundFloorHeight)//incase somebody wants to make an elevator that works downwards instead
            {
                if (elevatorPlatform.transform.position.y < topFloorHeight)
                {
                    elevatorPlatform.transform.Translate(Vector3.up * moveSpeed * Time.deltaTime, Space.World);
                    if (playerPassenger != null)
                    {
                        playerPassenger.Move(Vector3.up * moveSpeed * Time.deltaTime);
                    }
                }
            }
            else
            {
                if (elevatorPlatform.transform.position.y > topFloorHeight)
                    elevatorPlatform.transform.Translate(-Vector3.up * moveSpeed * Time.deltaTime, Space.World);
                if (playerPassenger != null)
                {
                    playerPassenger.Move(-Vector3.up * moveSpeed * Time.deltaTime);
                }
            }
        }
        else
        {
            if (topFloorHeight > groundFloorHeight)//incase somebody wants to make an elevator that works downwards instead
            {
                if (elevatorPlatform.transform.position.y > groundFloorHeight)
                    elevatorPlatform.transform.Translate(-Vector3.up * moveSpeed * Time.deltaTime, Space.World);
                if (playerPassenger != null)
                {
                    playerPassenger.Move(-Vector3.up * moveSpeed * Time.deltaTime);
                }
            }
            else
            {
                if (elevatorPlatform.transform.position.y < groundFloorHeight)
                    elevatorPlatform.transform.Translate(Vector3.up * moveSpeed * Time.deltaTime, Space.World);
                if (playerPassenger != null)
                {
                    playerPassenger.Move(Vector3.up * moveSpeed * Time.deltaTime);
                }
            }
        }
    }

    private void FixedUpdate()
    {
        if (hasPassenger)//as long as there is passenger on elevator, continue to count
        {
            if (touchTimer >= enterExitTimerSeconds)
            {
                isProgressing = true;
            }
            else
            {
                touchTimer += Time.deltaTime;
            }

            hasPassenger = false;//reset to false, if there is still a passenger detected in the next oncollisionstay() then it will be true again.
        }
        else
        {
            isProgressing = false;
            touchTimer = 0.0F;
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player")
        {
            playerPassenger = other.gameObject.GetComponent<CharacterController>();
        }
    }

    private void OnTriggerStay()
    {
        hasPassenger = true;
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.tag == "Player")
        {
            playerPassenger = null;
        }
    }
}
