addOrder:                           O(log n)
cancelOrder:                        O(log n)
cancelOrdersForUser:                O(m log n)
cancelOrdersForSecIdWithMinimumQty: O(m log n)
getMatchingSizeForSecurity:         O(m)
getAllOrders:                       O(n)
cancelOrdersWithMinimumQty:         O(m log n)

Where:
  n is the total number of orders.
  m is the number of relevant orders (e.g., orders for a specific user or security).
