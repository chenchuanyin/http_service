//
// Created by silver chan on 2017/7/12.
//

#ifndef __ENGINE_CLIENT_H__
#define __ENGINE_CLIENT_H__

#include <Poco/Timestamp.h>
#include <Poco/Net/Socket.h>
#include <Poco/Net/StreamSocket.h>

class EngineClient : public Poco::RefCountedObject {
public:
    EngineClient(std::string ip, int port);

    ~EngineClient();

    void setStatus(bool status) { status_ = status; }

    bool status() const { return status_; }

    bool timeLimit();

    void updateTime();

    Poco::Net::StreamSocket &socket() { return socket_; }

    std::string host() const { return host_; }

    int port() const { return port_; }

    std::string toString() const;

    bool operator==(const EngineClient& other);

    bool operator==(const std::string& other);

    bool operator!=(const EngineClient& other);

    bool operator!=(const std::string& other);

private:
    std::string host_;
    int port_;
    bool status_;
    Poco::Timestamp timestamp_;
    Poco::Net::StreamSocket socket_;
};

bool operator==(Poco::AutoPtr<EngineClient> lhs, const std::string &rhs);

bool operator!=(Poco::AutoPtr<EngineClient> lhs, const std::string &rhs);

#endif // __ENGINE_CLIENT_H__
