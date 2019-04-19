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
	cout << "entry\n";
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
protected:
    int nSocket;
public:
    HTTPSocket(const char* domain, int port = 80){
        hostent* h = gethostbyname(domain);
        sockaddr_in sin;
	sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        memcpy(&sin.sin_addr.s_addr, h->h_addr, sizeof(int));

	SSLCONTEXT::init();

    	if((nSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            throw runtime_error("Error creating HTTP socket");

        if(connect(nSocket, reinterpret_cast<sockaddr*>(&sin), sizeof (sin))){
		cout << "errno: " << nSocket << endl;
            throw runtime_error("Error creating HTTP connection");
        }

    }
    virtual ~HTTPSocket(){
        close();
    }
    virtual void close(){
        ::close(nSocket);
    }


};

class HTTPSSocket : public HTTPSocket{
    SSL_CTX* ctx;
    SSL* sslSocket;

public:
    HTTPSSocket(const char* domain, int port = 443) : HTTPSocket(domain, port), ctx(SSL_CTX_new(meth)){
        if(!ctx){
		cout << ERR_get_error() << endl;
		 throw runtime_error("Error creating SLL context");
		cout << ERR_get_error() << endl;
	}
	sslSocket = SSL_new(ctx);
	if(!sslSocket){
		cout << "errno: " << errno << endl;
		throw runtime_error("Error creating SSL socket");
	}
	SSL_set_fd(sslSocket, nSocket);
        if(SSL_connect(sslSocket) != 1)
            throw runtime_error("Error creating SSL connection");
 	
	} 


    virtual HTTPResponse query(string sendBytes){
        int toShip = sendBytes.size();
        int sended = 0;

        do{
            sended += SSL_write(sslSocket, sendBytes.c_str() + sended, toShip);
		cout << "1\n";        
}while(sended != toShip);

        int received = 0;
        constexpr int sizeBuff = 65535;
        char buffer[sizeBuff];

        do{
		 cout << "2\n";
            received += SSL_read(sslSocket, buffer + received, sizeBuff);
		cout << "2\n";        
}while(received > 0 && received == sizeBuff);
	cout << buffer << endl;
        return HTTPResponse(buffer);
    }
};

class TelegramBot{
    HTTPSSocket httpsSocket;
    string token;
public:
    TelegramBot(string tok);
    HTTPResponse method(string method, string args);
};
