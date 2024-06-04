#include "OrderCache.h"

void OrderCache::addOrder(Order order) {
    std::lock_guard<std::mutex> lock(mtx);
    orders.emplace(order.orderId(), order);
    if (order.side() == "Buy") {
        buyOrders[order.securityId()].emplace(order.orderId(), order);
        securityIdToBuyQty[order.securityId()].insert(order);
    }
    else {
        sellOrders[order.securityId()].emplace(order.orderId(), order);
        securityIdToSellQty[order.securityId()].insert(order);
    }
    userOrders[order.user()].insert(order.orderId());
}

void OrderCache::cancelOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = orders.find(orderId);
    if (it != orders.end()) {
        const Order& order = it->second;
        if (order.side() == "Buy") {
            buyOrders[order.securityId()].erase(orderId);
            securityIdToBuyQty[order.securityId()].erase(order);
        }
        else {
            sellOrders[order.securityId()].erase(orderId);
            securityIdToSellQty[order.securityId()].erase(order);
        }
        userOrders[order.user()].erase(orderId);
        orders.erase(it);
    }
}

void OrderCache::cancelOrdersForUser(const std::string& user) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = userOrders.find(user);
    if (it != userOrders.end()) {
        for (const auto& orderId : it->second) {
            auto orderIt = orders.find(orderId);
            if (orderIt != orders.end()) {
                const Order& order = orderIt->second;
                if (order.side() == "Buy") {
                    buyOrders[order.securityId()].erase(orderId);
                    securityIdToBuyQty[order.securityId()].erase(order);
                }
                else {
                    sellOrders[order.securityId()].erase(orderId);
                    securityIdToSellQty[order.securityId()].erase(order);
                }
                orders.erase(orderIt);
            }
        }
        userOrders.erase(it);
    }
}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) {
    std::lock_guard<std::mutex> lock(mtx);
    cancelOrdersWithMinimumQty(buyOrders[securityId], securityIdToBuyQty[securityId], minQty);
    cancelOrdersWithMinimumQty(sellOrders[securityId], securityIdToSellQty[securityId], minQty);
}

unsigned int OrderCache::getMatchingSizeForSecurity(const std::string& securityId) {
    std::lock_guard<std::mutex> lock(mtx);
    unsigned int totalMatchingQty = 0;

    auto buyIt = securityIdToBuyQty.find(securityId);
    auto sellIt = securityIdToSellQty.find(securityId);
    if (buyIt != securityIdToBuyQty.end() && sellIt != securityIdToSellQty.end()) {
        auto& buyOrdersSet = buyIt->second;
        auto& sellOrdersSet = sellIt->second;

        // Create vectors of buy and sell orders
        std::vector<Order> buyOrdersVec, sellOrdersVec;
        // In descending order
        for (const auto& elem : buyOrdersSet) {
            buyOrdersVec.push_back(elem);
        }

        for (const auto& elem : sellOrdersSet) {
            sellOrdersVec.push_back(elem);
        }

        // Two pointers for matching buy and sell orders
        size_t buyIndex = 0, sellIndex = 0;
        while (buyIndex < buyOrdersVec.size() && sellIndex < sellOrdersVec.size()) {
            auto& buyOrder = buyOrdersVec[buyIndex];
            auto& sellOrder = sellOrdersVec[sellIndex];

            if (buyOrder.company() != sellOrder.company()) {
                unsigned int matchQty = std::min(buyOrder.qty(), sellOrder.qty());
                totalMatchingQty += matchQty;

                // Adjust quantities or move to next order
                if (buyOrder.qty() > matchQty) {
                    buyOrder = Order(buyOrder.orderId(), buyOrder.securityId(), buyOrder.side(), buyOrder.qty() - matchQty, buyOrder.user(), buyOrder.company());
                    ++sellIndex; // Move to next sell order
                }
                else if (sellOrder.qty() > matchQty) {
                    sellOrder = Order(sellOrder.orderId(), sellOrder.securityId(), sellOrder.side(), sellOrder.qty() - matchQty, sellOrder.user(), sellOrder.company());
                    ++buyIndex; // Move to next buy order
                }
                else {
                    ++buyIndex; // Move to next buy order
                    ++sellIndex; // Move to next sell order
                }
            }
            else {
                // Skip orders from the same company
                ++sellIndex;
            }
        }
    }

    return totalMatchingQty;
}

std::vector<Order> OrderCache::getAllOrders() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Order> allOrders;
    for (const auto& entry : orders) {
        allOrders.push_back(entry.second);
    }
    return allOrders;
}

bool OrderCache::ReverseOrderComparator::operator()(const Order& lhs, const Order& rhs) const {
    return lhs.qty() != rhs.qty() ? lhs.qty() > rhs.qty() : lhs.orderId() < rhs.orderId();
}

bool OrderCache::AscendingOrderComparator::operator()(const Order& lhs, const Order& rhs) const {
    return lhs.qty() != rhs.qty() ? lhs.qty() < rhs.qty() : lhs.orderId() < rhs.orderId();
}

void OrderCache::cancelOrdersWithMinimumQty(std::unordered_map<std::string, Order>& ordersMap, std::set<Order, ReverseOrderComparator>& qtySet, unsigned int minQty) {
    std::unordered_set<std::string> toRemove;
    for (const auto& [orderId, order] : ordersMap) {
        if (order.qty() >= minQty) {
            userOrders[order.user()].erase(orderId);
            orders.erase(orderId);
            qtySet.erase(order);
            toRemove.insert(orderId);
        }
    }
    for (const auto& orderId : toRemove) {
        ordersMap.erase(orderId);
    }
}

void OrderCache::cancelOrdersWithMinimumQty(std::unordered_map<std::string, Order>& ordersMap, std::set<Order, AscendingOrderComparator>& qtySet, unsigned int minQty) {
    std::unordered_set<std::string> toRemove;
    for (const auto& [orderId, order] : ordersMap) {
        if (order.qty() >= minQty) {
            userOrders[order.user()].erase(orderId);
            orders.erase(orderId);
            qtySet.erase(order);
            toRemove.insert(orderId);
        }
    }
    for (const auto& orderId : toRemove) {
        ordersMap.erase(orderId);
    }
}
