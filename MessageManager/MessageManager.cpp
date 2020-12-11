#include "MessageManager/MessageManager.h"

using std::string;
using std::vector;
using namespace std::literals::chrono_literals;

MessageManager::MessageManager(const string &url)
    : road_contact_(), road_query_(), carsim_control_(), carsim_state_(),
      road_contact_mutex_(), road_query_mutex_(),
      carsim_control_mutex_(), carsim_state_mutex_(),
      subers_(), pubers_(), lcm_subscriptions_(),
      need_stop_(false), tunnel_(url){};

MessageManager::~MessageManager()
{
    need_stop_ = true;
    for (auto &t : pubers_)
    {
        if (t.joinable())
            t.join();
    }
    for (auto &t : subers_)
    {
        if (t.joinable())
            t.join();
    }
    for (auto s : lcm_subscriptions_)
    {
        tunnel_.unsubscribe(s);
    }
}

void MessageManager::PublishRoadQuery() const
{
    tunnel_.publish(CHANNEL_NAME_ROAD_QUERY, &road_query_);
}

void MessageManager::PublishState() const
{
    tunnel_.publish(CHANNEL_NAME_CARSIM_STATE, &carsim_state_);
}

void MessageManager::PublishRoadQueryWithLock() const
{
    std::lock_guard<std::mutex> lock(road_query_mutex_);
    this->PublishRoadQuery();
}

void MessageManager::PublishStateWithLock() const
{
    std::lock_guard<std::mutex> lock(carsim_state_mutex_);
    this->PublishState();
}

void MessageManager::PublishAllAsync(int freq_query, int freq_state)
{
    pubers_.push_back(std::thread(
        &MessageManager::PubLoopRoadQuery, this, freq_query));
    pubers_.push_back(std::thread(
        &MessageManager::PubLoopCarsimState, this, freq_state));
    for (auto &t : pubers_)
    {
        t.detach();
    }
}

void MessageManager::SubscribeAll()
{
    auto lcm_sub1 = tunnel_.subscribe(
        CHANNEL_NAME_CARSIM_CONTROL,
        &MessageManager::HandlerCarsimControl,
        this);
    auto lcm_sub2 = tunnel_.subscribe(
        CHANNEL_NAME_ROAD_CONTACT,
        &MessageManager::HandlerRoadContact,
        this);
    lcm_subscriptions_.push_back(lcm_sub1);
    lcm_subscriptions_.push_back(lcm_sub2);

    subers_.push_back(std::thread(
        &MessageManager::SubLoopAll, this));
    for (auto &t : subers_)
    {
        t.detach();
    }
}

void MessageManager::HandlerRoadContact(
    const lcm::ReceiveBuffer *rbuf,
    const std::string &channel,
    const carsim::RoadContact *msg)
{
    this->road_contact_ = *msg;
}

void MessageManager::HandlerCarsimControl(
    const lcm::ReceiveBuffer *rbuf,
    const std::string &channel,
    const carsim::Control *msg)
{
    this->carsim_control_ = *msg;
}

void MessageManager::PubLoopCarsimState(int freq)
{
    while (!need_stop_)
    {
        auto time_point = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000 / freq);
        this->PublishStateWithLock();
        std::this_thread::sleep_until(time_point);
    }
}

void MessageManager::PubLoopRoadQuery(int freq)
{
    while (!need_stop_)
    {
        auto time_point = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000 / freq);
        this->PublishRoadQueryWithLock();
        std::this_thread::sleep_until(time_point);
    }
}

void MessageManager::SubLoopAll()
{
    while (!need_stop_)
    {
        tunnel_.handle();
    }
}