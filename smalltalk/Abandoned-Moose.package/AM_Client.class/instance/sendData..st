as yet unclassified
sendData: aString
	clientSocket isConnected ifFalse: [ self connect. ].
	
	^ clientSocket sendData: aString.