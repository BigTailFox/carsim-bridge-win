#include "MessageManager/MessageManager.h"

using std::string;
using std::vector;
using namespace std::literals::chrono_literals;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

// busy tick for more precision sleep
void SpinUntil(high_resolution_clock::time_point tp)
{
    high_resolution_clock::time_point now;
    do
    {
        now = high_resolution_clock::now();
    } while (now < tp);
}

MessageManager::MessageManager(const string &url)
    : road_contact_(), road_query_(), carsim_control_(), carsim_state_(),
      road_contact_mutex_(), road_query_mutex_(),
      carsim_control_mutex_(), carsim_state_mutex_(),
      subers_(), pubers_(), lcm_subscriptions_(),
      need_stop_(false), tunnel_(url),
      timer_setup_(false), tick_count_(0)
{
    road_contact_.valid = 0;
    road_query_.valid = 0;
    carsim_control_.valid = 0;
    carsim_state_.valid = 0;
};

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

void MessageManager::PublishAsync(int freq_query, int freq_state)
{
    if (freq_query > 0)
    {
        pubers_.push_back(std::thread(
            &MessageManager::PubLoopRoadQuery, this, freq_query));
    }
    if (freq_state > 0)
    {
        pubers_.push_back(std::thread(
            &MessageManager::PubLoopCarsimState, this, freq_state));
    }
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
#ifdef DEBUG
        auto start = std::chrono::high_resolution_clock::now();
#endif
        auto time_point = std::chrono::high_resolution_clock::now() + std::chrono::microseconds(1000000 / freq);
        PublishStateWithLock();
        SpinUntil(time_point);
#ifdef DEBUG
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        printf("[DEBUG] spend %d us to publish state\n", duration);
#endif
    }
}

void MessageManager::PubLoopRoadQuery(int freq)
{
    while (!need_stop_)
    {
#ifdef DEBUG
        auto start = std::chrono::high_resolution_clock::now();
#endif
        auto time_point = std::chrono::high_resolution_clock::now() + std::chrono::microseconds(1000000 / freq);
        PublishRoadQueryWithLock();
        SpinUntil(time_point);
#ifdef DEBUG
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        printf("[DEBUG] spend %d us to publish roadquery\n", duration);
#endif
    }
}

void MessageManager::SubLoopAll()
{
    while (!need_stop_)
    {
        tunnel_.handle();
    }
}

std::chrono::steady_clock::time_point
MessageManager::Tick()
{
    last_tick_ = high_resolution_clock::now();
    return last_tick_;
}

std::chrono::steady_clock::time_point
MessageManager::GetSyncTimePoint(int freq)
{
    return last_tick_ + std::chrono::microseconds(1000000 / freq);
}

void MessageManager::Sync(int freq)
{
    auto tp = GetSyncTimePoint(freq);
    ++tick_count_;
    SpinUntil(tp);
}