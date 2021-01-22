require 'socket'                 # Get sockets from stdlib

port = 8000
# TODO: change this to udp and find a way to not have to use local host
server = TCPServer.open(port)    # Client should listen to this port

loop {
   client = server.accept         # Wait for a client to connect
   client.puts("Hello, World!")   # Send the time to the client
   client.puts("Closing the connection. Bye!")
   client.close                   # Disconnect from the client
}



# Client example in ruby
#require 'socket'        # Sockets are in standard library
#
#hostname = 'localhost'                                        
#port = 8000                                           
#                                                              
#s = TCPSocket.open(hostname, port)
#                                   
#while line = s.gets     # Read lines from the socket
#   puts line.chop       # And print with platform line termina
#end
#s.close