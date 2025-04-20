#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define LS_COUNT 1
#define PORT 2316
#define IP "127.0.0.1"
#define SERVER_ADDR "SERVER_ADDR=127.0.0.1"
#define REMOTE_ADDR "REMOTE_ADDR=127.0.0.1"
#define SERVER_PORT "SERVER_ADDR=2315"
#define SERVER_PROTOCOL "SERVER_PROTOCOL=HTTP/1.0"
#define QUERY_STRING "QUERY_STRING="
#define SCRIPT_NAME "SCRIPT_NAME="
#define CONTENT_TYPE "CONTENT_TYPE=text/html"

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
    Socket(int i) : pd(i) {}
    virtual void shutdown_() { shutdown(pd, 2); }
    int Get_pd() const { return pd; }
    virtual ~Socket() { close(pd); }
};

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
    SocketInfo(short port, const char *ip)
    {
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = inet_addr(ip);
    }
    struct sockaddr *get_addr() const { return (sockaddr *)&saddr; }
    int get_len() const { return sizeof(saddr); }
};

class ServerSocket : public Socket
{
public:
    ServerSocket() : Socket() {}
    void bind_(const SocketInfo &ServerAddress)
    {
        if (bind(Get_pd(), ServerAddress.get_addr(), ServerAddress.get_len()) == -1)
        {
            perror("bind");
            exit(1);
        }
    }
    void listen_(int count)
    {
        if (listen(Get_pd(), count) == -1)
        {
            std::cout << "Error: Can't do listen" << std::endl;
            exit(1);
        }
    }
    int accept_(SocketInfo &ClientAddress) const
    {
        socklen_t len = ClientAddress.get_len();
        int i = accept(Get_pd(), ClientAddress.get_addr(), &len);
        if (i < 0)
        {
            std::cout << "Error: Can't do accept" << std::endl;
            exit(1);
        }
        return i;
    }
};

class ConnectedSocket : public Socket
{
public:
    ConnectedSocket(int i) : Socket(i) {}
    void to_write(const std::string &str)
    {
        if (send(Get_pd(), str.c_str(), str.length(), 0) < 0)
        {
            std::cout << "Error: Can't do send" << std::endl;
            exit(1);
        }
    }
    void to_write(const std::vector<uint8_t> &bytes)
    {
        if (send(Get_pd(), bytes.data(), bytes.size(), 0) < 0)
        {
            std::cout << "Error: Can't write in ConnectedSocket" << std::endl;
            exit(1);
        }
    }
    void to_read(std::string &str)
    {
        int buflen = 1024, last;
        char buf[buflen];
        if ((last = recv(Get_pd(), buf, buflen, 0)) < 0)
        {
            std::cout << "Error: Can't do recive" << std::endl;
            exit(1);
        }
        buf[last] = 0;
        str = buf;
    }
    int to_read_one_str(std::string &str)
    {
        char c;
        int last;
        while (true)
        {
            if ((last = recv(Get_pd(), &c, 1, 0)) < 0)
            {
                perror("recive");
                exit(1);
            }
            if (last == 0)
            {
                break;
            }
            str.push_back(c);
            if (c == '\n')
                break;
        }
        return last;
    }
    void read_vector(std::vector<std::string> &lines)
    {
        while (true)
        {
            std::string str;
            if (to_read_one_str(str))
                lines.push_back(str);
            else
                break;

            if (str == "\r\n" || str == "\n")
            {
                break;
            }
        }
    }
};

std::string set_path(std::string str)
{
    std::string path;

    for (size_t i = 0; i < str.length() - 1; i++)
    {
        if (str[i] == ' ') // после метода
        {
            i++;
            while (str[i] != ' ' && str[i] != '\n' && str[i] != '\r')
                path += str[i++];
            break;
        }
    }
    if (path == "/")
        path = "index.html";
    return path;
}

bool cgi_connection(std::string path)
{
    int res = path.find('?');
    return res != -1;
}

std::string set_file_name(std::string path)
{
    std::string str;
    int i = 0;
    while (path[i] != '?')
    {
        str.push_back(path[i]);
        i++;
    }
    return str;
}

char **set_env(std::vector<std::string> &vec)
{
    char **env = new char *[vec.size() + 1];
    for (int i = 0; i < (int)vec.size(); i++)
        env[i] = (char *)vec[i].c_str();
    env[vec.size()] = NULL;
    return env;
}

char *set_interpr_name(std::string file)
{
    char *res = NULL;
    std::string str;
    FILE *fd;
    fd = fopen(file.c_str(), "r");
    if (fd == NULL)
        return NULL;
    int c = fgetc(fd);
    while (c != EOF)
    {
        if (c == '#')
        {
            c = fgetc(fd);
            if (c == '!')
                break;
        }
        else
            c = fgetc(fd);
    }
    if (c == EOF)
        return NULL;
    c = fgetc(fd);
    while (c == ' ' || c == '\t')
        c = fgetc(fd);
    while (c != ' ' && c != '\t' && c != '\n' && c != '\r')
    {
        str += c;
        c = fgetc(fd);
    }
    res = new char[str.length() + 1];
    strcpy(res, (char *)str.c_str());
    return res;
}

void make_cgi_response(std::string &response, std::string const path, std::string const request)
{
    int fd;
    int pid = fork();
    if (pid == -1)
    {
        std::cout << "Error: can't do fork\n";
        exit(1);
    }
    else if (pid == 0) // son
    {
        fd = open("tmp.txt", O_TRUNC | O_CREAT | O_WRONLY);
        if (fd < 0)
        {
            std::cout << "Error: can't create tmp file\n";
            exit(1);
        }
        dup2(fd, 1);
        close(fd);

        std::string request_data = set_file_name(path);
        char *exec_file = set_interpr_name(request_data);
        char *cmd;
        char *argv_1 = NULL;

        if (exec_file == NULL)
            cmd = (char *)request_data.c_str();
        else
        {
            cmd = exec_file;
            argv_1 = (char *)request_data.c_str();
        }

        char *argv[] = {cmd, argv_1, NULL};

        std::vector<std::string> vec;
        vec.push_back(SERVER_ADDR);
        vec.push_back(REMOTE_ADDR);
        vec.push_back(SERVER_PORT);
        vec.push_back(SERVER_PROTOCOL);
        vec.push_back(SCRIPT_NAME + request_data);
        vec.push_back(CONTENT_TYPE);

        char **env = set_env(vec);

        execve(cmd, argv, env);
        perror("exec");
        exit(1);
    }
    else if (pid) // father
    {
        int status;
        wait(&status);
        if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0)
        {
            fd = open("tmp.txt", O_RDONLY);
            char c;
            std::string str;
            while (read(fd, &c, 1))
                str.push_back(c);
            response += "HTTP/1.1 200 OK\n";
            response += "Version: HTTP/1.1\n";
            response += "Content-Type: text/html\n";
            response += "Content-length: " + std::to_string(str.length()) + "\n\n";
            response += str + '\n';
        }
        else
        {
            response += "HTTP/1.1 404 Not Found \n\n";
        }
    }
}

void make_response(std::string &response, std::string const path)
{

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0)
    {
        std::cout << "HTTP/1.1 404 Not Found" << std::endl;
        response += "HTTP/1.1 404 Not Found\n";
    }

    std::string text;
    char c;
    while (read(fd, &c, sizeof(char)))
        text.push_back(c);

    response += "HTTP/1.1 200 OK\n";
    response += "Version: HTTP/1.1\n";
    response += "Content-Type: text/html\n";
    response += "Content-length: " + std::to_string(text.length()) + "\n\n";
    response += text + "\n";
    close(fd);
}

void ProcessConnection(int sck, SocketInfo &ClientAddress)
{
    ConnectedSocket cnt_sck(sck);
    std::vector<std::string> lines;
    std::cout << "Read:\n";
    cnt_sck.read_vector(lines);
    std::cout << "RequestHeader: " << lines[0];
    std::string path = set_path(lines[0]);
    std::string path_2;
    if(path[0] == '/')
        for(size_t i = 1; i < path.length(); i++)
            path_2.push_back(path[i]);
    else path_2 = path;
    std::string response;
    if (cgi_connection(path_2))
        make_cgi_response(response, path_2, lines[0]);
    else
        make_response(response, path_2);
    cnt_sck.to_write(response);
    cnt_sck.shutdown_();
}

void Server_loop()
{
    SocketInfo Serveraddr(PORT, IP);
    ServerSocket sock;
    sock.bind_(Serveraddr);
    sock.listen_(LS_COUNT);
    for (;;)
    {
        std::cout << "Wait" << std::endl;
        SocketInfo ClientAddress;
        int cl_csk = sock.accept_(ClientAddress);
        std::cout << "Process" << std::endl;
        ProcessConnection(cl_csk, ClientAddress);
    }
    sock.shutdown_();
}

int main()
{
    Server_loop();

    return 0;
}
