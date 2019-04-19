#define LINUX
#ifdef LINUX
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
#else
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define close closesocket
#endif

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <string>

#include <ssl.h>
#include <err.h>


#include <json.hpp>

using json = nlohmann::json;
using namespace std;

static SSL_METHOD *meth;

struct SSLCONTEXT{
    static void init(){
        SSL_library_init();
        SSLeay_add_ssl_algorithms();
        SSL_load_error_strings();
        meth = const_cast<SSL_METHOD*>(TLSv1_2_client_method());
    }
};

class HTTPResponse{
    string response;
public:
    HTTPResponse(char* ptrmessage) : response(ptrmessage){}
    static const char* findBody(string s){
        const char* str = s.c_str();
        while(*str != 0)
               if(*str++ == '\r')
                   if(*str++ == '\n')
                       if(*str++ == '\r')
                           if(*str++ == '\n')
                               return const_cast<char*>(str);

    }
    const char* getText(){
        return response.c_str();
    }
    json toJSON(){
        return json::parse(findBody(response));
    };

};

class HTTPSocket{
private:
    sockaddr_in sin;
protected:
    int nSocket;
public:
    HTTPSocket(const char* domain, int port = 80){

        cout << "1\n";
        hostent* h = gethostbyname(domain);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        memcpy(&sin.sin_addr.s_addr, h->h_addr, sizeof(int));

        open();
    }
    virtual ~HTTPSocket(){
        close();
    }

    virtual void open(){
        cout << "2\n";
        if((nSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            throw runtime_error("Error creating HTTP socket");

        if(connect(nSocket, reinterpret_cast<sockaddr*>(&sin), sizeof (sin)))
            throw runtime_error("Error creating HTTP connection");
    }

    virtual void close(){
        ::close(nSocket);
    }


};

class HTTPSSocket : public HTTPSocket{
    SSL_CTX* ctx;
    SSL* sslSocket;
    bool opened;
public:
    HTTPSSocket(const char* domain, int port = 443) : HTTPSocket(domain, port){
        cout << "3\n";
        open();
    }

    void open(){
        cout << "4\n";

        if(opened)
            HTTPSocket::open();

        SSLCONTEXT::init();
        ctx = SSL_CTX_new(meth);

        if(!ctx){
            cout << ERR_get_error()  << endl;
            throw runtime_error("Error creating SLL context");
        }

        sslSocket = SSL_new(ctx);
        if(!sslSocket)
            throw runtime_error("Error creating SSL socket");

        SSL_set_fd(sslSocket, nSocket);
        if(SSL_connect(sslSocket) != 1)
            throw runtime_error("Error creating SSL connection");
        opened = true;
    }

    ~HTTPSSocket(){
        close();
    }

    void close(){
        HTTPSocket::close();
        SSL_shutdown(sslSocket);
        SSL_CTX_free(ctx);
    }

    virtual HTTPResponse query(string sendBytes){
        int toShip = sendBytes.size();
        int sended = 0;

        do
            sended += SSL_write(sslSocket, sendBytes.c_str() + sended, toShip);
        while(sended != toShip);

        int received = 0;
        constexpr int sizeBuff = 65535;
        char buffer[sizeBuff];

        do
            received += SSL_read(sslSocket, buffer + received, sizeBuff);
        while(received > 0 && received == sizeBuff);
        return HTTPResponse(buffer);
    }
};

class TelegramBot{
    HTTPSSocket httpsSocket;
    string token;
    map<string, string> qa;
    int offset;
public:
    TelegramBot(string tok);
    HTTPResponse method(const string& method, const string& args);
    void refresh(){
        httpsSocket.close();
        httpsSocket.open();
    }
    json getUpdates(){
        string arg = "?timeout=60&limit=3&offset=";
        arg.append(to_string(offset));
        return method("/getUpdates", arg).toJSON;
    }
    void regAnswer(string msg, string answer){
        qa.insert({msg, answer});
    }
    void unregAnswer(string msg, string answer){
        qa.erase(msg);
    }
};
