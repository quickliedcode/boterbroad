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
        SSLCONTEXT::init();

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
public:
    HTTPSSocket(const char* domain, int port = 443) : HTTPSocket(domain, port){
        open();
    }

    void open(){

        ctx = SSL_CTX_new(meth);

        if(!ctx){
            cout << ERR_get_error() << endl;
            throw runtime_error("Error creating SLL context");
        }

        sslSocket = SSL_new(ctx);
        if(!sslSocket)
            throw runtime_error("Error creating SSL socket");

        SSL_set_fd(sslSocket, nSocket);
        if(SSL_connect(sslSocket) != 1)
            throw runtime_error("Error creating SSL connection");
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
public:
    TelegramBot(string tok);
    HTTPResponse method(const string& method, const string& args);
    void refresh(){
        httpsSocket.close();
        httpsSocket.open();
    }
};
