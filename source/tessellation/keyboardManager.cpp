#include "keyboardManager.h"

#include <iostream>

// keyQueueWrap
// ======================================

keyQueueWrap::keyQueueWrap(std::queue<keyStateInfo>& queue)
    : queue{queue} 
{}

keyQueueWrap::keyQueueWrap(keyQueueWrap&& old)
    : queue{old.queue} 
{}

keyQueueWrap& keyQueueWrap::AddState(std::string name, std::function<void(unsigned)> action)
{
    keyStateInfo info;
    info.name = name;
    info.action = action;
    info.id = queue.size();

    queue.push(std::move(info));
    return *this;
}

// keyboardMenager
// ======================================

keyboardMenager::keyboardMenager()
{}

keyboardMenager::~keyboardMenager()
{}

keyQueueWrap keyboardMenager::RegisterKey(int key)
{
    auto& queue = m_keyToFunQueueMap[key];
    keyQueueWrap wrapper(queue);
    return std::move(wrapper);
}

void keyboardMenager::GoToNextState(int key)
{
    auto& queue = m_keyToFunQueueMap[key];
    
    // move current state at the end of the queue
    keyStateInfo oldState = queue.front();
    queue.pop();
    queue.push(oldState);

    // perform action binded to the next state
    keyStateInfo& nextState = queue.front();
    nextState.action(nextState.id);
}

const keyStateInfo& keyboardMenager::GetCurrentState(int key)
{
    return m_keyToFunQueueMap[key].front();
}