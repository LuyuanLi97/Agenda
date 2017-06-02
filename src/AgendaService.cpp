//AgendaService.cpp
#include<iostream>
#include"AgendaService.hpp"
using namespace std;
AgendaService::AgendaService() {
    m_storage = Storage::getInstance();
}

AgendaService:: ~AgendaService() {
	m_storage->sync();
}

bool AgendaService::userLogIn(const std::string userName, const std::string password) {
	list<User> find = m_storage->queryUser( [&](const User & user)->bool {
		return ( userName == user.getName() && password == user.getPassword() ); } );
	if ( find.size()==1 ) return true;
	else return false; 
}

bool AgendaService::userRegister(const std::string userName, const std::string password, const std::string email, const std::string phone) {
	list<User> find = m_storage->queryUser( [&](const User & user)->bool {return ( userName == user.getName() );} );
	if ( find.size()==0 ) {
		m_storage->createUser( User( userName,password,email,phone) );
		return true;
	}
	else return false;
}

bool AgendaService::deleteUser(const std::string userName, const std::string password) {
	int find = m_storage->deleteUser( [&](const User& user)->bool
		{return ( userName == user.getName() && password == user.getPassword() );} );
	if ( find==1 ) {
		deleteAllMeetings(userName);
		//delete meeting (praticipant)
		int find2 = m_storage->deleteMeeting( [&](const Meeting &meeting)->bool {
			if( meeting.isParticipator(userName) ) return true;
			else return false;
		} );
	    return true;
	} 
	else return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
	list<User> find = m_storage->queryUser( [&](const User & user)->bool {return true;} );
	return find;
}

bool AgendaService::createMeeting(const std::string userName, const std::string title,
                       const std::string startDate, const std::string endDate,
                       const std::vector<std::string> participator) {
	if ( !Date::isValid( Date::stringToDate(startDate) ) || !Date::isValid( Date::stringToDate(endDate) ) )
		return false;  //dates are valid

	list<User> find = m_storage->queryUser( [&](const User & user)->bool {return ( userName == user.getName() );} );
	if ( find.size()!=1 ) return false;  //sponsor exists
    
	vector<string>::const_iterator iter;
	for ( iter = participator.begin(); iter!=participator.end(); iter++ ) {
		list<User> find = m_storage->queryUser( [&](const User & user)->bool {return ( *iter == user.getName() );} );
		if ( find.size() != 1 ) return false;
	}      //participator exist

	if ( participator.size()==0 ) return false;  //participator is not zero

	for ( iter=participator.begin(); iter!=participator.end(); iter++ ) {
		if ( userName==*iter ) return false;
	}

	for ( int i=0; i<participator.size()-1; i++ )
		for ( int j=i; j<participator.size()-1; j++ ) {
			if ( participator[i] == participator[j] )
				return false;
		}

	if( Date::stringToDate(startDate)>=Date::stringToDate(endDate) )
		return false;     //no time point

	Date start = Date::stringToDate(startDate);
	Date end = Date::stringToDate(endDate);
	list<Meeting> sp = meetingQuery( userName,startDate,endDate );
	if( sp.size()!=0 ) return false;   //sponcor date is available

	for ( iter=participator.begin(); iter!=participator.end(); iter++ ) {
		list<Meeting> pa = meetingQuery( *iter,startDate,endDate);
		if( pa.size()!=0 ) return false;
	}       //participator date are avilable
    
    list<Meeting> find1 = m_storage->queryMeeting( [&](const Meeting & meeting)->bool {return ( title == meeting.getTitle() );} );
	if ( find1.size()!=0 ) return false;    //title is the only one

    m_storage->createMeeting( Meeting(userName,participator,Date::stringToDate(startDate),Date::stringToDate(endDate),title) );
	return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName, const std::string title) const {
	list<Meeting> rightmeeting = m_storage->queryMeeting( [&](const Meeting & meeting)->bool 
	{
		if( title == meeting.getTitle() && ( userName==meeting.getSponsor()||meeting.isParticipator(userName)) )
			return true;
		else return false;
	} );
	return rightmeeting;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string startDate,
                                    const std::string endDate) const {
	Date start = Date::stringToDate(startDate);
	Date end = Date::stringToDate(endDate);
	list<Meeting> rightmeeting = m_storage->queryMeeting( [&](const Meeting & meeting)->bool 
	{
		if ( (userName==meeting.getSponsor()||meeting.isParticipator(userName)) && !(start>meeting.getEndDate()||end<meeting.getStartDate()) )
			return true;
		else return false;
	} );
	return rightmeeting;
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
	list<Meeting> rightmeeting = m_storage->queryMeeting( [&](const Meeting & meeting)->bool 
	{
		if ( meeting.getSponsor() == userName || meeting.isParticipator(userName) )
			return true;
		else
			return false;
	} );
	return rightmeeting;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
	list<Meeting> rightmeeting = m_storage->queryMeeting( [&](const Meeting & meeting)->bool 
	{
		if ( meeting.getSponsor() == userName )
			return true;
		else
			return false;
	} );
	return rightmeeting;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string userName) const {
	list<Meeting> rightmeeting = m_storage->queryMeeting( [&](const Meeting & meeting)->bool 
	{
		if ( meeting.isParticipator(userName) )
			return true;
		else
			return false;
	} );
	return rightmeeting;
}

bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
	int de = m_storage->deleteMeeting( [&](const Meeting& meeting)->bool
		{return ( userName == meeting.getSponsor() && title==meeting.getTitle() );} );
	if ( de==1 ) return true;
	else return false;
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
	int de = m_storage->deleteMeeting( [&](const Meeting& meeting)->bool
		{return ( userName == meeting.getSponsor() );} );
	if ( de==1 ) return true;
	else return false;
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage->sync();
}