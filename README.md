# Abandoned Moose

This repository contains the server and client for the DataMayHack 2013

Several versions of the server and clients exist:
    * Naive Python version
    * Threaded Python client
    * Haskell client prototype
    * Full implementation in Smalltalk

## Implementation details

Connections are made on port 9001. 

The protocol used is text based and used for RPC (Remote Procedure Call) on the server. Each command consists of a command name and a data payload separated by `;`. Thus, the command name may not contain a `;`. When the RPC has been run, the result is returned, the call is blocking.
