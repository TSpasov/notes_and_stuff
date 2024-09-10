#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <chrono>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Timer
{
public:
    using expireCallback = std::function<void(void)>;

    auto Process() -> void;
    auto ScheduleTimer(void * cl, std::string timerID, expireCallback callback, std::chrono::milliseconds period, int times = -1) -> void;
    auto StopTimer(void * cl, const std::string &timerID) -> void;
private:

    auto RemoveOld() -> void;

    struct TimerTask
    {
       explicit TimerTask(std::string  ptimerID,
                  expireCallback pcallback,
                  const  std::chrono::milliseconds& pperiod,
                  int ptimes):
            timerID(std::move(ptimerID)),
            callback(std::move(pcallback)),
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
