#include "OrderCache.h"
#include <assert.h>
#include <iostream>


void addOrderToCache(OrderCache& cache, const std::string& orderId, const std::string& securityId, const std::string& side, unsigned int qty, const std::string& user, const std::string& company) {
	Order order(orderId, securityId, side, qty, user, company);
	cache.addOrder(order);
}


void testCancelOrder() {
	OrderCache cache;
	addOrderToCache(cache, "OrdId1", "SecId1", "Buy", 100, "User1", "CompanyA");
	addOrderToCache(cache, "OrdId2", "SecId2", "Sell", 200, "User2", "CompanyB");

	cache.cancelOrder("OrdId1");

	auto allOrders = cache.getAllOrders();
	assert(allOrders.size() == 1);
	assert(allOrders[0].orderId() == "OrdId2");

	std::cout << "testCancelOrder passed!" << std::endl;
}


void testCancelOrdersForUser() {
	OrderCache cache;
	addOrderToCache(cache, "OrdId1", "SecId1", "Buy", 100, "User1", "CompanyA");
	addOrderToCache(cache, "OrdId2", "SecId2", "Sell", 200, "User1", "CompanyB");
	addOrderToCache(cache, "OrdId3", "SecId1", "Sell", 300, "User2", "CompanyA");

	cache.cancelOrdersForUser("User1");

	auto allOrders = cache.getAllOrders();
	assert(allOrders.size() == 1);
	assert(allOrders[0].orderId() == "OrdId3");

	std::cout << "testCancelOrdersForUser passed!" << std::endl;
}


void testCancelOrdersForSecIdWithMinimumQty() {
	OrderCache cache;
	addOrderToCache(cache, "OrdId1", "SecId1", "Buy", 100, "User1", "CompanyA");
	addOrderToCache(cache, "OrdId2", "SecId1", "Buy", 150, "User2", "CompanyB");
	addOrderToCache(cache, "OrdId3", "SecId1", "Sell", 200, "User3", "CompanyA");
	addOrderToCache(cache, "OrdId4", "SecId2", "Sell", 300, "User4", "CompanyB");

	cache.cancelOrdersForSecIdWithMinimumQty("SecId1", 150);

	auto allOrders = cache.getAllOrders();
	assert(allOrders.size() == 2);
	assert(allOrders[0].orderId() == "OrdId1");
	assert(allOrders[1].orderId() == "OrdId4");

	std::cout << "testCancelOrdersForSecIdWithMinimumQty passed!" << std::endl;
}


void runTests() {
	// Example 1
	{
		OrderCache cache;
		addOrderToCache(cache, "OrdId1", "SecId1", "Sell", 100, "User10", "Company2");
		addOrderToCache(cache, "OrdId2", "SecId3", "Sell", 200, "User8", "Company2");
		addOrderToCache(cache, "OrdId3", "SecId1", "Buy", 300, "User13", "Company2");
		addOrderToCache(cache, "OrdId4", "SecId2", "Sell", 400, "User12", "Company2");
		addOrderToCache(cache, "OrdId5", "SecId3", "Sell", 500, "User7", "Company2");
		addOrderToCache(cache, "OrdId6", "SecId3", "Buy", 600, "User3", "Company1");
		addOrderToCache(cache, "OrdId7", "SecId1", "Sell", 700, "User10", "Company2");
		addOrderToCache(cache, "OrdId8", "SecId1", "Sell", 800, "User2", "Company1");
		addOrderToCache(cache, "OrdId9", "SecId2", "Buy", 900, "User6", "Company2");
		addOrderToCache(cache, "OrdId10", "SecId2", "Sell", 1000, "User5", "Company1");
		addOrderToCache(cache, "OrdId11", "SecId1", "Sell", 1100, "User13", "Company2");
		addOrderToCache(cache, "OrdId12", "SecId2", "Buy", 1200, "User9", "Company2");
		addOrderToCache(cache, "OrdId13", "SecId1", "Sell", 1300, "User1", "Company1");

		std::cout << "Testing SecId1...\n";
		assert(cache.getMatchingSizeForSecurity("SecId1") == 300);
		std::cout << "Testing SecId2...\n";
		assert(cache.getMatchingSizeForSecurity("SecId2") == 1000);
		std::cout << "Testing SecId3...\n";
		assert(cache.getMatchingSizeForSecurity("SecId3") == 600);
	}

	// Example 2
	{
		OrderCache cache;
		addOrderToCache(cache, "OrdId1", "SecId3", "Sell", 100, "User1", "Company1");
		addOrderToCache(cache, "OrdId2", "SecId3", "Sell", 200, "User3", "Company2");
		addOrderToCache(cache, "OrdId3", "SecId1", "Buy", 300, "User2", "Company1");
		addOrderToCache(cache, "OrdId4", "SecId3", "Sell", 400, "User5", "Company2");
		addOrderToCache(cache, "OrdId5", "SecId2", "Sell", 500, "User2", "Company1");
		addOrderToCache(cache, "OrdId6", "SecId2", "Buy", 600, "User3", "Company2");
		addOrderToCache(cache, "OrdId7", "SecId2", "Sell", 700, "User1", "Company1");
		addOrderToCache(cache, "OrdId8", "SecId1", "Sell", 800, "User2", "Company1");
		addOrderToCache(cache, "OrdId9", "SecId1", "Buy", 900, "User5", "Company2");
		addOrderToCache(cache, "OrdId10", "SecId1", "Sell", 1000, "User1", "Company1");
		addOrderToCache(cache, "OrdId11", "SecId2", "Sell", 1100, "User6", "Company2");

		std::cout << "Testing SecId1...\n";
		assert(cache.getMatchingSizeForSecurity("SecId1") == 900);
		std::cout << "Testing SecId2...\n";
		assert(cache.getMatchingSizeForSecurity("SecId2") == 600);
		std::cout << "Testing SecId3...\n";
		assert(cache.getMatchingSizeForSecurity("SecId3") == 0);
	}

	// Provided matching example
	{
		OrderCache cache;
		addOrderToCache(cache, "OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA");
		addOrderToCache(cache, "OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB");
		addOrderToCache(cache, "OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA");
		addOrderToCache(cache, "OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC");
		addOrderToCache(cache, "OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB");
		addOrderToCache(cache, "OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD");
		addOrderToCache(cache, "OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE");
		addOrderToCache(cache, "OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE");

		std::cout << "Testing SecId1...\n";
		assert(cache.getMatchingSizeForSecurity("SecId1") == 0);  // No matching orders
		std::cout << "Testing SecId2...\n";
		assert(cache.getMatchingSizeForSecurity("SecId2") == 2700);  // Total matching quantity
		std::cout << "Testing SecId3...\n";
		assert(cache.getMatchingSizeForSecurity("SecId3") == 0);  // No matching orders
	}

	{
		testCancelOrder();
		testCancelOrdersForUser();
		testCancelOrdersForSecIdWithMinimumQty();
	}

	std::cout << "All tests passed!" << std::endl;
}


int main() {
	runTests();
	return 0;
}
