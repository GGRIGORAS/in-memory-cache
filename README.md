in-memory cache of order objects that supports 
    adding new orders, removing existing orders and matching buy and sell orders. 
		- On "order" is a request to buy or sell a financial security (eg. bond, stock, 
		  commodity, etc.)
		- Each order is uniquely identified by an order id
		- Each security has a different security id 
		- Order matching occurs for orders with the same security id, different side (buy or sell),
		  and different company (company of person who requested the order)
 
  - Provide an implementation for the OrderCacheInterface class in OrderCache.h 
    - An Order class is provided for you:
       - This class holds basic order info
       - Do not remove the provided member variables and methods in the Order class
       - You may add additional members if you like
       - For your implementation of OrderCacheInterface:
            - Write a class that derives OrderCacheInterface
            - Choose appropriate data structure(s) to hold Order objects and any additional data you'd like            
            - Implement the following methods (do not change their signatures)
                - addOrder()
                - cancelOrder()
                - cancelOrdersForUser()
                - cancelOrdersForSecIdWithMinimumQty()
			    - getMatchingSizeForSecurity()
                - getAllOrders()
            - Add any additional methods and variables you'd like to your class


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
