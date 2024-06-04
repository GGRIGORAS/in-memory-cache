#pragma once
#include "OrderCacheInterface.h"
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <set>

class OrderCache : public OrderCacheInterface {
public:
    void addOrder(Order order) override;
    void cancelOrder(const std::string& orderId) override;
    void cancelOrdersForUser(const std::string& user) override;
    void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) override;
    unsigned int getMatchingSizeForSecurity(const std::string& securityId) override;
    std::vector<Order> getAllOrders() const override;

private:
    struct ReverseOrderComparator {
        bool operator()(const Order& lhs, const Order& rhs) const;
    };

    struct AscendingOrderComparator {
        bool operator()(const Order& lhs, const Order& rhs) const;
    };

    void cancelOrdersWithMinimumQty(std::unordered_map<std::string, Order>& ordersMap, std::set<Order, ReverseOrderComparator>& qtySet, unsigned int minQty);
    void cancelOrdersWithMinimumQty(std::unordered_map<std::string, Order>& ordersMap, std::set<Order, AscendingOrderComparator>& qtySet, unsigned int minQty);

    mutable std::mutex mtx;
    std::unordered_map<std::string, Order> orders; // Map of orderId to Order
    std::unordered_map<std::string, std::unordered_map<std::string, Order>> buyOrders; // Map of securityId to map of orderId to Buy orders
    std::unordered_map<std::string, std::set<Order, ReverseOrderComparator>> securityIdToBuyQty;
    std::unordered_map<std::string, std::unordered_map<std::string, Order>> sellOrders; // Map of securityId to map of orderId to Sell orders
    std::unordered_map<std::string, std::set<Order, AscendingOrderComparator>> securityIdToSellQty;
    std::unordered_map<std::string, std::unordered_set<std::string>> userOrders; // Map of user to set of orderIds
};

