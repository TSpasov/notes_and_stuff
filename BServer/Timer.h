#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

class Timer
{
public:
    using expireCallback = std::function<void(void)>;

    void Process();
    void ScheduleTimer(void * cl, std::string timerID, expireCallback callback, std::chrono::milliseconds period, int times = -1);
    void StopTimer(void * cl, std::string timerID);
private:

    void RemoveOld();

    struct TimerTask
    {
       explicit TimerTask(const std::string& ptimerID,
                  expireCallback pcallback,
                  const  std::chrono::milliseconds& pperiod,
                  int ptimes):
            timerID(ptimerID),
            callback(pcallback),
            period(pperiod),
            times(ptimes)
        {
           start_point =  std::chrono::system_clock::now();
        }

        std::string timerID;
        expireCallback callback;
        std::chrono::milliseconds period;
        std::chrono::time_point<std::chrono::system_clock>  start_point;
        int times;
    };

    using TasksList = std::vector<TimerTask>;
    using ActiveTasks = std::unordered_map<void *, TasksList>;

    ActiveTasks m_activeTasks;
};

#endif // TIMER_H
