require 'socket'
require 'mysql2'

client = Mysql2::Client.new(:host => "localhost", :username => "root", :password => "Evqauyekll11", :database => "things-of-net-rails_development")
u1 = UDPSocket.new(Socket::AF_INET6)
u1.bind("aaaa::1", 1234)

while true
    data, addr = u1.recvfrom(1024)
    temp = data.unpack("cc")
    value = temp[1] * 16 + temp[0]
    client.query("INSERT INTO sensors VALUES (NULL, \"" + addr[3] + "\"," + value.to_s + ",1, NULL, NULL)")
    puts temp[0], temp[1], addr
    puts addr[3]
end
