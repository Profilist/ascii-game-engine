export module events.manager;

import <functional>;
import <memory>;
import <string>;
import <vector>;

import events.event;

export namespace age {

// Callback type for event handlers
using EventCallback = std::function<void(const Event&)>;

// Pub/sub system for game events
class EventManager {
public:
    EventManager() = default;
    ~EventManager() = default;

    // Subscribe to events of a specific type
    // Returns a subscription ID that can be used to unsubscribe
    int subscribe(const std::string& eventType, EventCallback callback);

    // Subscribe to all events
    int subscribeAll(EventCallback callback);

    // Unsubscribe using subscription ID
    void unsubscribe(int subscriptionId);

    // Queue an event to be processed later
    void emit(std::unique_ptr<Event> event);

    // Helper to emit events without manually creating unique_ptr
    template<typename T, typename... Args>
    void emit(Args&&... args);

    // Process all queued events
    void processEvents();

    // Immediately dispatch an event (bypass queue)
    void dispatchImmediate(const Event& event);

    // Clear all pending events
    void clearPending();

    // Clear all subscriptions
    void clearSubscriptions();

    // Get count of pending events
    size_t pendingCount() const;

private:
    struct Subscription {
        int id;
        std::string eventType;
        EventCallback callback;
    };

    void dispatch(const Event& event);

    std::vector<Subscription> subscriptions_;
    std::vector<std::unique_ptr<Event>> pendingEvents_;
    int nextId_{1};
};

}
