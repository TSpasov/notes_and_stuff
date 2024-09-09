#include "Timer.h"
#include <algorithm>


void Timer::Process()
{
    RemoveOld();

    for (auto& tasks : m_activeTasks)
    {
        for (auto& timers : tasks.second)
        {
            const auto& now  = std::chrono::system_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - timers.start_point);
            if(timers.period <= dur)
            {
                timers.callback();
                timers.start_point = now;

                if(timers.times > 0)
                {
                    --timers.times;
                }
            }
        }
    }
}

void Timer::ScheduleTimer(void* cl, std::string timerID, expireCallback callback, std::chrono::milliseconds period, int times)
{
    auto it = std::find_if(m_activeTasks.begin(), m_activeTasks.end(),
                           [&](const auto& task){ return task.first == cl;});

    if(it == m_activeTasks.end())
    {
        m_activeTasks.emplace(cl, TasksList{});
    }

    m_activeTasks[cl].push_back(TimerTask(timerID, callback, period, times));
}

void Timer::StopTimer(void* cl,const  std::string& timerID)
{
    auto it = std::find_if(m_activeTasks.begin(), m_activeTasks.end(),
                           [&](const auto& client){ return client.first == cl;});

    if(it == m_activeTasks.end())
    {
        return;
    }

    auto it2 = std::find_if(it->second.begin(), it->second.end(),
                            [&](const auto& timer){ return timer.timerID == timerID;});

    if(it != m_activeTasks.end())
    {
        it->second.erase(it2);
    }
}

void Timer::RemoveOld()
{
    for (auto itOuter = m_activeTasks.begin(); itOuter != m_activeTasks.end();)
    {
        for (auto itInner = itOuter->second.begin(); itInner != itOuter->second.end();)
        {
            if(itInner->times == 0)
            {
                itInner = itOuter->second.erase(itInner);
            }
            else
            {
                ++itInner;
            }
        }

        if(itOuter->second.empty())
        {
            itOuter = m_activeTasks.erase(itOuter);
        }
        else
        {
            ++itOuter;
        }
    }
}

