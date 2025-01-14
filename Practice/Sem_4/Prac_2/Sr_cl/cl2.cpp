#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2222
#define IP "127.0.0.1"


class SocketInfo
{
    struct sockaddr_in saddr;
public:
    SocketInfo()
    {
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(PORT);
        saddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    SocketInfo(short port, const char* ip)
    {
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = inet_addr(ip);
    }
    struct sockaddr* get_addr() const { return (sockaddr*)&saddr; } 
    int get_len() const { return sizeof(saddr); }
};

class Socket
{
    int pd;
public:
    Socket()
    {
        pd = socket(AF_INET, SOCK_STREAM, 0);
        if (pd < 0)
        {
            std::cout << "Error: Can't create socket" << std::endl;
            exit(1);
        }
    }
    Socket(int i): pd(i){}
    virtual void shutdown_() { shutdown(pd, 2); }
    int Get_pd() const { return pd; }
    virtual ~Socket() { close(pd); }
};

class ClientSocket: public Socket
{
public:
    ClientSocket() : Socket() {}
    void connect_(SocketInfo& addr)
    {
        socklen_t len = addr.get_len();
        if(connect(Get_pd(), addr.get_addr(), len) == -1)
        {
            std::cout << "Error: Can't do connect" << std::endl;
            exit(1);
        }
    }
   void to_write(const std::string& str)
    {
        if(send(Get_pd(), str.c_str(), str.length(), 0) < 0)
        {
            std::cout << "Error: Can't do send" << std::endl;
            exit(1);
        }
    }
    void to_read(std::string& str)
    {
        int buflen = 1024, last;
        char buf[buflen];
        if((last = recv(Get_pd(), buf, buflen, 0)) < 0)
        {
            std::cout << "Error: Can't do recive" << std::endl;
            exit(1);
        }
        buf[last] = 0;
        str = buf;
    }
    void to_read_one_str(std::string& str)
    {
        char c;
        int last;
        while(true)
        {
             if((last = recv(Get_pd(), &c, 1, 0)) < 0)
            {
                perror("recive");
                exit(1);
            }
            if(last == 0) break;
            str.push_back(c);
            if(c == '\n') break;
        }
    }
    void read_vector(std::vector<std::string>& lines)
    {
        while(true)
        {
            std::string str;
            to_read_one_str(str);
            lines.push_back(str);
            if(str == "\n")
            {
                break;
            } 
        }
    }
    void shutdown_() { shutdown(Get_pd(), 2); }
};



int main()
{
    SocketInfo addr(PORT, IP);
    ClientSocket a;
    a.connect_(addr);

    //std::string request = "GET 1.txt HTTP/1.1\n\n";
    std::string prog = "program{ int t = 0; if(t == 0) write(1234); else write(\"Hello Word\"); }";
    std::string request ="GET a5?<?" + prog + "?>HTTP/1.1\n\n";
    a.to_write(request);
    
    std::string answer;
    a.to_read(answer);
    std::cout << answer;
    a.shutdown_();

    return 0;
}
