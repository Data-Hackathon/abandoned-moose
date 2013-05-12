as yet unclassified
connectTo: aString port: aNumber
	| serverAddress |
	clientSocket := Socket newTCP.
	serverAddress := NetNameResolver addressForName: aString.
	
	clientSocket
		connectTo: serverAddress port: aNumber;
		waitForConnectionFor: 10.
	
	^ clientSocket isConnected