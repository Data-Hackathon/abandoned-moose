-- Inspired by: http://book.realworldhaskell.org/read/sockets-and-syslog.html
-- import Network (connectTo, PortNumber)
import Network.Socket hiding (recv)
import Network.Socket.ByteString (recv, sendAll)
import qualified Data.ByteString.Char8 as C
import System.IO

createConnection :: HostName -> ServiceName -> IO Socket
createConnection hostname port = 
    do
        addrinfos <- getAddrInfo Nothing (Just hostname) (Just port)
        let serveraddr = head addrinfos
        sock <- socket (addrFamily serveraddr) Stream defaultProtocol
        connect sock (addrAddress serveraddr)

        return sock

getInputAndSend sock = 
    do

        putStr "send> "
        str <- getLine 
        sendAll sock $ C.pack str
        msg <- recv sock 4096
        putStr "got> " 
        C.putStrLn msg


        getInputAndSend sock

main :: IO ()
main = withSocketsDo $ -- Used for Windows compability
    do
        let hostname = "localhost" :: HostName
        let port     = "9001"

        hSetBuffering stdout NoBuffering

        sock <- createConnection hostname port

        h <- socketToHandle sock WriteMode
        hSetBuffering h NoBuffering 

        getInputAndSend sock

        hClose h

