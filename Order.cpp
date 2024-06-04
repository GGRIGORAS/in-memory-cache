#include "Order.h"

Order::Order(const std::string& ordId, const std::string& secId, const std::string& side, const unsigned int qty, const std::string& user,
	const std::string& company)
	: m_orderId(ordId),
	m_securityId(secId),
	m_side(side),
	m_qty(qty),
	m_user(user),
	m_company(company)
{ }


std::string Order::orderId() const
{
	return m_orderId;
}


std::string Order::securityId() const
{
	return m_securityId;
}


std::string Order::side() const
{
	return m_side;
}


std::string Order::user() const
{
	return m_user;
}


std::string Order::company() const
{
	return m_company;
}


unsigned int Order::qty() const
{
	return m_qty;
}