This is a Time-of-day server that tells clients the actual date and time.

1. Request-reply protocol
	* In this method clients send a query message to the server, and the server responds with the actual time value in the above format. The client query is a simple message containing only a string “What is the time?”.
	* Communication is via SSL; the server has a self-signed certificate, the client does not need any certificate. 

2. Multicast protocol
	* Clients can subscribe to this service and unsubscribe. The server tells the time regularly (a minute after the previous time announcement) to the clients that subscribed. You should implement this as application-layer multicast, i.e. the server sends the message to the first client, the first client then forwards the information to the second client and so on, until the last client receives the time. You can, but don’t have to impose a limit (greater than three) on the number clients who can subscribe for the multicast. If there is a limit, you should notify clients when their subscription is rejected.  