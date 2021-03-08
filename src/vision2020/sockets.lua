local socket = require("socket")
local tcp = assert(socket.tcp())

local host, port = "127.0.0.1", 8000

tcp:connect(host, port);

while true do
    tcp:send("hello world\n");
end
