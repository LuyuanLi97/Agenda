//Date.cpp
#include<iostream>
#include<string>
#include<sstream>
#include"Date.hpp"
Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(std::string dateString) {
	bool valid = (dateString[0]>='0'&&dateString[0]<='9'&&dateString[1]>='0'&&dateString[1]<='9'&&
				 dateString[2]>='0'&&dateString[2]<='9'&&dateString[3]>='0'&&dateString[3]<='9'&&
				 dateString[4]=='-'&&dateString[5]>='0'&&dateString[5]<='9'&&
				 dateString[6]>='0'&&dateString[6]<='9'&&dateString[7]=='-'&&
				 dateString[8]>='0'&&dateString[8]<='9'&&dateString[9]>='0'&&dateString[9]<='9'&&
				 dateString[10]=='/'&&dateString[11]>='0'&&dateString[11]<='9'&&
				 dateString[12]>='0'&&dateString[12]<='9'&&dateString[13]==':'&&
				 dateString[14]>='0'&&dateString[14]<='9'&&dateString[15]>='0'&&dateString[15]<='9');
	if( valid && dateString.size()==16 ) {
		m_year = (dateString[0]-'0')*1000 + (dateString[1]-'0')*100 + (dateString[2]-'0')*10 + (dateString[3]-'0');
		m_month = (dateString[5]-'0')*10 + (dateString[6]-'0');
		m_day = (dateString[8]-'0')*10 + (dateString[9]-'0');
		m_hour = (dateString[11]-'0')*10 + (dateString[12]-'0');
		m_minute = (dateString[14]-'0')*10 + (dateString[15]-'0');
	}
	else {
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_hour = 0;
		m_minute = 0;
	}
	
}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
	int monthday[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	int monthdayleap[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
	if ( t_date.getYear()>9999 || t_date.getYear()<1000 )
		return false;
	else if ( t_date.getMonth()>12 || t_date.getMonth()<1 )
		return false;
	else if ( t_date.getHour()>23 || t_date.getHour()<0 )
		return false;
	else if ( t_date.getMinute()>59 || t_date.getMinute()<0 )
		return false;
	else {
		if (  (t_date.getYear()%4==0 && t_date.getYear()%100!=0) || t_date.getYear()%400==0 ) {
			if ( t_date.getDay()<1 || t_date.getDay()>monthdayleap[t_date.getMonth()-1] )
				return false;
			else return true;
		}
		else {
 			if ( t_date.getDay()<1 || t_date.getDay()>monthday[t_date.getMonth()-1] )
				return false;
			else return true;
		}
	}
}

Date Date::stringToDate(const std::string t_dateString) {
	bool valid = (t_dateString[0]>='0'&&t_dateString[0]<='9'&&t_dateString[1]>='0'&&t_dateString[1]<='9'&&
				 t_dateString[2]>='0'&&t_dateString[2]<='9'&&t_dateString[3]>='0'&&t_dateString[3]<='9'&&
				 t_dateString[4]=='-'&&t_dateString[5]>='0'&&t_dateString[5]<='9'&&
				 t_dateString[6]>='0'&&t_dateString[6]<='9'&&t_dateString[7]=='-'&&
				 t_dateString[8]>='0'&&t_dateString[8]<='9'&&t_dateString[9]>='0'&&t_dateString[9]<='9'&&
				 t_dateString[10]=='/'&&t_dateString[11]>='0'&&t_dateString[11]<='9'&&
				 t_dateString[12]>='0'&&t_dateString[12]<='9'&&t_dateString[13]==':'&&
				 t_dateString[14]>='0'&&t_dateString[14]<='9'&&t_dateString[15]>='0'&&t_dateString[15]<='9');
	if (  valid && t_dateString.size()==16 )
		return Date( t_dateString );
	else
		return Date( "0000-00-00/00:00" );
}

std::string Date::dateToString(Date t_date) {
	if ( isValid(t_date) ) {
		std::stringstream sstr;
		sstr<<t_date.getYear()<<'-';
		if ( t_date.getMonth()<10 ) 
			sstr<<'0';
		sstr<<t_date.getMonth()<<'-';
		if ( t_date.getDay()<10 ) 
			sstr<<'0';
		sstr<<t_date.getDay()<<'/';
		if ( t_date.getHour()<10 ) 
			sstr<<'0';
		sstr<<t_date.getHour()<<':';
		if ( t_date.getMinute()<10 ) 
			sstr<<'0';
		sstr<<t_date.getMinute();
		return sstr.str();
	}
	else return "0000-00-00/00:00";
}

Date &Date::operator=(const Date &t_date) {
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
	return *this;
}

bool Date::operator==(const Date &t_date) const {
	return ( m_year==t_date.m_year && m_month==t_date.m_month && m_day==t_date.m_day && m_hour==t_date.m_hour && m_minute==t_date.m_minute );
}

bool Date::operator>(const Date &t_date) const {
	if ( m_year>t_date.m_year )
		return true;
	else if ( m_year==t_date.m_year ) {
		if ( m_month>t_date.m_month )
			return true;
		else if ( m_month==t_date.m_month ) {
			if ( m_day>t_date.m_day )
				return true;
			else if ( m_day==t_date.m_day ) {
				if ( m_hour>t_date.m_hour )
					return true;
				else if ( m_hour==t_date.m_hour ) {
					if ( m_minute>t_date.m_minute )
						return true;
					else return false;
				}
			}
		}
	}
	return false;
}

bool Date::operator<(const Date &t_date) const {
	return !( *this==t_date || *this>t_date );
}

bool Date::operator>=(const Date &t_date) const {
	return ( *this==t_date || *this>t_date );
}

bool Date::operator<=(const Date &t_date) const {
	return ( *this==t_date || *this<t_date );
}

