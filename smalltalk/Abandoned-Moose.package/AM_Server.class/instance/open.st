as yet unclassified
open
	| connectionSocket interactionSocket receivedData |
	true
		ifFalse: [ 
			"Prepare socket for handling client connection requests"
			connectionSocket := Socket newTCP.
			connectionSocket listenOn: 9999 backlogSize: 10.	"Build a new socket for interaction with a client which connection request is accepted"
			interactionSocket := connectionSocket waitForAcceptFor: 60.	"Get rid of the connection socket since it is useless for the rest of this example"
			connectionSocket closeAndDestroy.	"Get and display data from the client"
			receivedData := interactionSocket receiveData.
			Transcript
				cr;
				show: receivedData.	"Send echo back to client and finish interaction"
			interactionSocket sendData: 'ECHO: ' , receivedData.
			interactionSocket closeAndDestroy ].
	^ false