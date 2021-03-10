using UnityEngine;

public class ObjectPicker : MonoBehaviour
{
    [SerializeField]
    private GameObject player = null;

    private GameObject currentPickedObject = null;

    private GameObject grabAnchor = null;

    private Camera playerCam = null;

    private bool error = false;

    void Start()
    {
        if (player == null)
        {
            Debug.LogError("PlayerController is provided with null player object!");
            error = true;
            return;
        }

        playerCam = player.GetComponentInChildren<Camera>();

        if (playerCam == null)
        {
            Debug.LogError("PlayerController can not find camera on provided player object!");
            error = true;
            return;
        }

        grabAnchor = new GameObject("Grab Anchor");
        grabAnchor.transform.parent = playerCam.transform;
        grabAnchor.AddComponent<Rigidbody>().isKinematic = true;
    }

    void Update()
    {
        if (error) return;

        if(Input.GetMouseButton(0))//left mouse button is currently down
        {
            if(currentPickedObject == null)
            {
                //make ray from camera through crosshair and detect object
                RaycastHit hitInfo = new RaycastHit();
                if (Physics.Raycast(new Ray(playerCam.transform.position, playerCam.transform.forward), out hitInfo, 1000.0F))
                {
                    if(hitInfo.collider.attachedRigidbody != null && hitInfo.collider.gameObject != player)
                    {
                        //pick the object if it has a rigidbody and is not the player.
                        currentPickedObject = hitInfo.collider.gameObject;
                        doStuffWithPickedObject(hitInfo.point);
                    }
                }
            }
        }
        else if(currentPickedObject != null)
        {
            //un-pick the object
            onObjectUnPick();
            currentPickedObject = null;
        }
        
    }


    private void doStuffWithPickedObject(Vector3 grabPos)
    {
        grabAnchor.transform.position = grabPos;
        currentPickedObject.AddComponent<SpringJoint>().damper = 0.0F;
        currentPickedObject.GetComponent<SpringJoint>().spring = 100.0F;
        currentPickedObject.GetComponent<SpringJoint>().massScale = 10.0F;
        currentPickedObject.GetComponent<SpringJoint>().connectedBody = grabAnchor.GetComponent<Rigidbody>();
        currentPickedObject.GetComponent<SpringJoint>().connectedAnchor = grabAnchor.transform.position;
    }


    private void onObjectUnPick()
    {
        SpringJoint[] pickedObjectSprings = currentPickedObject.GetComponents<SpringJoint>();
        if(pickedObjectSprings != null)
        {
            foreach(SpringJoint sj in pickedObjectSprings)
            {
                if(sj.connectedBody == grabAnchor.GetComponent<Rigidbody>())
                {
                    Destroy(sj);
                }
            }
        }
    }


}
