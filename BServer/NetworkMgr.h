#ifndef NETWORKMGR_H
#define NETWORKMGR_H


#include <netinet/in.h>
#include <string>
#include <sys/poll.h>
#include <vector>
#include <functional>

class NetworkMgr
{
public:
    NetworkMgr();
    ~NetworkMgr();
    void Process();

    void Send(int clientID, const std::string& msg);
    void SendToAll(std::string msg);
    void RemoveConnection(int clientID);

    void OnAcceptConnection(std::function<void(int)> on_accept);
    void OnRemoveConnection(std::function<void(int)> on_remove);
    void OnRecieve(std::function<void(int, const std::string)> on_recieve);

    bool HasConnections();
private:

    std::function<void(int)> m_on_accept;
    std::function<void(int)> m_on_remove;
    std::function<void(int, const std::string)> m_on_recieve;

    // Set up poll structure
    std::vector<pollfd> fds;
    sockaddr_in sockaddr;
    int sockfd;

};

#endif // NETWORKMGR_H
