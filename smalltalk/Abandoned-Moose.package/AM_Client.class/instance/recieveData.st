as yet unclassified
recieveData
	| data |
	data := clientSocket receiveDataTimeout: 3.
	Transcript show: data; cr.
	^data.