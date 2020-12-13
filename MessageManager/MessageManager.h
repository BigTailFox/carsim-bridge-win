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
    void PublishAllAsync(int freq_query, int freq_state);
    void SubscribeAll();
    std::chrono::steady_clock::time_point Tick();
    std::chrono::steady_clock::time_point GetTimePointSleepUntil(int freq);
    void Sync(int freq);

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
    std::chrono::steady_clock::time_point last_time_;
    std::chrono::steady_clock clock_;
    bool timer_setup_;

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