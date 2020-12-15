#pragma once

#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

#include "lcm/lcm-cpp.hpp"
#include "carsim/RoadContact.hpp"
#include "carsim/RoadQuery.hpp"
#include "carsim/Control.hpp"
#include "carsim/State.hpp"

//#define DEBUG

const static std::string CHANNEL_NAME_CARSIM_CONTROL = "CARSIM_CONTROL";
const static std::string CHANNEL_NAME_CARSIM_STATE = "CARSIM_STATE";
const static std::string CHANNEL_NAME_ROAD_QUERY = "CARSIM_ROADQUERY";
const static std::string CHANNEL_NAME_ROAD_CONTACT = "CARSIM_ROADCONTACT";

class MessageManager
{
public:
    MessageManager(const std::string &url);
    MessageManager(const MessageManager &rhs) = delete;
    MessageManager &operator=(MessageManager const &) = delete;
    ~MessageManager();

    void PublishRoadQuery() const;
    void PublishState() const;
    void PublishRoadQueryWithLock() const;
    void PublishStateWithLock() const;
    void PublishAsync(int freq_query, int freq_state);

    void SubscribeRoadContact();
    void SubscribeControl();
    void SubscribeAll();
    void SubscribeAsync();
    void Handle() { tunnel_.handle(); }
    void UnsubscribeAll();

    std::chrono::steady_clock::time_point Tick();
    std::chrono::steady_clock::time_point GetSyncTimePoint(int freq);
    void Sync(int freq);
    std::chrono::steady_clock::time_point GetLastTick() { return last_tick_; }
    int GetTickCount() { return tick_count_; }

    carsim::Control carsim_control_;
    carsim::State carsim_state_;
    carsim::RoadQuery road_query_;
    carsim::RoadContact road_contact_;

    mutable std::mutex road_contact_mutex_;
    mutable std::mutex road_query_mutex_;
    mutable std::mutex carsim_state_mutex_;
    mutable std::mutex carsim_control_mutex_;

private:
    mutable lcm::LCM tunnel_;
    volatile bool need_stop_;
    std::vector<std::thread> pubers_;
    std::vector<std::thread> subers_;
    std::vector<lcm::Subscription *> lcm_subscriptions_;
    std::chrono::high_resolution_clock::time_point last_tick_;
    bool timer_setup_;
    int tick_count_;

    void HandlerRoadContact(
        const lcm::ReceiveBuffer *rbuf,
        const std::string &channel,
        const carsim::RoadContact *msg);
    void HandlerCarsimControl(
        const lcm::ReceiveBuffer *rbuf,
        const std::string &channel,
        const carsim::Control *msg);

    void PubLoopCarsimState(int freq);
    void PubLoopRoadQuery(int freq);

    void SubLoopAll();
};