#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include <functional>
#include <string>

class NetworkMgr
{
public:
    NetworkMgr();
    auto TryToConnect() -> bool;
    void Process();



    void OnAcceptConnection(std::function<void(void)> on_accept);
    void OnRemoveConnection(std::function<void(void)> on_remove);
    void OnRecieve(std::function<void (const std::string)> on_recieve);

    auto IsConnected() const -> bool {return m_Connected;}

private:

    std::function<void()> m_on_accept;
    std::function<void()> m_on_remove;
    std::function<void(const std::string)> m_on_recieve;
    int m_fd = 0;
    bool m_Connected = false;
};

#endif // NETWORKMGR_H
