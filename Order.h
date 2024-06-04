#pragma once
#include <string>
class Order
{

public:

	// do not alter signature of this constructor
	Order(const std::string& ordId, const std::string& secId, const std::string& side, const unsigned int qty, const std::string& user,
		const std::string& company);

	// do not alter these accessor methods 
	std::string orderId() const;
	std::string securityId() const;
	std::string side() const;
	std::string user() const;
	std::string company() const;
	unsigned int qty() const;

private:

	// use the below to hold the order data
	// do not remove the these member variables  
	std::string m_orderId;     // unique order id
	std::string m_securityId;  // security identifier
	std::string m_side;        // side of the order, eg Buy or Sell
	unsigned int m_qty;        // qty for this order
	std::string m_user;        // user name who owns this order
	std::string m_company;     // company for user

};
