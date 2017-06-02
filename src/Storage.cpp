//Storage.cpp
#include<iostream>
#include<fstream>
#include<string>
#include"Storage.hpp"
#include"Path.hpp"
#include"User.hpp"
#include"Date.hpp"
using namespace std;

std::shared_ptr<Storage> Storage::m_instance = NULL;
Storage::Storage() {
	readFromFile();
}

bool Storage::readFromFile(void) {
	ifstream fin;
	if ( fin.fail() )
		return false;
	fin.open(Path::userPath);
	if ( fin.is_open() ) {
		string line;
		while ( getline(fin,line) ) {
			int commaindex[8];
			int k = 0;
			for ( int i=0; line[i]!='\0'; i++ ) {
				if ( line[i]=='"' )
					commaindex[k++] = i;
			}
			User u;
			u.setName( line.substr(commaindex[0]+1, commaindex[1]-commaindex[0]-1) );
			u.setPassword( line.substr(commaindex[2]+1, commaindex[3]-commaindex[2]-1) );
			u.setEmail( line.substr(commaindex[4]+1, commaindex[5]-commaindex[4]-1) );
			u.setPhone( line.substr(commaindex[6]+1, commaindex[7]-commaindex[6]-1) );
			m_userList.push_back( u );
		}
		fin.close();
	}
	else return false;
	fin.open(Path::meetingPath);
	if ( fin.is_open() ) {
		string line;
		while ( getline(fin,line) ) {
			int commaindex[10];
			int k = 0;
			for ( int i=0; line[i]!='\0'; i++ ) {
				if ( line[i]=='"' )
					commaindex[k++] = i;
			}
			Meeting m;
			m.setSponsor( line.substr(commaindex[0]+1, commaindex[1]-commaindex[0]-1) );
			string par = line.substr(commaindex[2]+1, commaindex[3]-commaindex[2]-1);
			string sdate = line.substr(commaindex[4]+1, commaindex[5]-commaindex[4]-1);
			string edate = line.substr(commaindex[6]+1, commaindex[7]-commaindex[6]-1);
			//participants:string to vector
			int symbol = 0;
			for ( int i=0; par[i]!='\0'; i++ ) {
				if ( par[i]=='&' ) symbol++;
			}
			int symbolindex[symbol+2];
			symbolindex[0] = -1;
			symbolindex[symbol+1] = par.size();
			k = 1;
			for ( int i=0; par[i]!='\0'; i++ ) {
				if ( par[i]=='&' ) symbolindex[k++] = i;
			}
			vector<string> v;
			for ( int i=0; i<symbol+1; i++ ) {
				v.push_back( par.substr(symbolindex[i]+1, symbolindex[i+1]-symbolindex[0]-1) );
			}
			m.setParticipator( v );
			//date: str to Date
			m.setStartDate( Date(sdate) );
			m.setEndDate( Date(edate) );
			m.setTitle( line.substr(commaindex[8]+1, commaindex[9]-commaindex[8]-1) );
			m_meetingList.push_back( m );
		}
		fin.close();
	}
	else return false;
	return true;
}

bool Storage::writeToFile(void) {
	ofstream fout;
	if ( fout.fail() )
		return false;
	fout.open(Path::userPath);
	if ( fout.is_open() ) {
		list<User>::iterator iter;
		for ( iter=m_userList.begin(); iter!=m_userList.end(); iter++ ) {
			fout<<'\"'<<iter->getName()<<"\",\""<<iter->getPassword()<<"\",\""<<iter->getEmail()<<"\",\""<<iter->getPhone()<<'\"'<<endl;
		}
	}
	fout.close();
	fout.open(Path::meetingPath);
	if ( fout.is_open() ) {
		list<Meeting>::iterator iter;
		for ( iter=m_meetingList.begin(); iter!=m_meetingList.end(); iter++ ) {
			fout<<"\""<<iter->getSponsor()<<"\",\"";     //sponser
			auto p_list = iter->getParticipator();
			for (auto i = p_list.begin(); i != p_list.end(); i++ ) {
				if ( i==p_list.begin() ) fout<<*i;
				else fout<<'&'<<*i;
			}
			fout<<"\",\"";    //participante
			string bdate = Date::dateToString( iter->getStartDate() );
			fout<<bdate<<"\",\"";    //startdate
			string edate = Date::dateToString( iter->getEndDate() );
			fout<<edate<<"\",\"";    //enddate
			fout<<iter->getTitle()<<"\""<<endl;  //title
		}
		fout.close();
	}
	return true;
}

std::shared_ptr<Storage> Storage::getInstance(void) {
	if ( m_instance==NULL ) 
		m_instance = shared_ptr<Storage>(new Storage());
	return m_instance;
}

Storage::~Storage() {
	writeToFile();
	m_instance == nullptr;
}

void Storage::createUser(const User & t_user) {
	m_userList.push_back( t_user );
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	list<User>::const_iterator iter;
	list<User> newlist;
	for ( iter=m_userList.begin(); iter!=m_userList.end(); iter++ ) {
		if ( filter( *iter )==true )
			newlist.push_back(*iter);
	}
	return newlist;
}

int Storage::updateUser(std::function<bool(const User &)> filter,std::function<void(User &)> switcher) {
	int trueuser = 0;
	list<User>::iterator iter;
	for ( iter=m_userList.begin(); iter!=m_userList.end(); iter++ ) {
		if ( filter( *iter )==true ) {
			switcher(*iter);
			trueuser++;
		}
	}
	return trueuser;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int duser = 0;
	list<User>::iterator iter;
	for ( iter=m_userList.begin(); iter!=m_userList.end(); iter++ ) {
		if ( filter( *iter )==true ) {
			iter = m_userList.erase( iter );
			duser++;
			iter--;
		}
	}
	return duser;
}

void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back( t_meeting );
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
	list<Meeting> newlist;
	for ( auto iter=m_meetingList.begin(); iter!=m_meetingList.end(); iter++ ) {
		if ( filter( *iter )==true )
			newlist.push_back(*iter);
	}
	return newlist;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter, std::function<void(Meeting &)> switcher) {
	int truemeeting = 0;
	list<Meeting>::iterator iter;
	for ( iter=m_meetingList.begin(); iter!=m_meetingList.end(); iter++ ) {
		if ( filter( *iter )==true ) {
			switcher(*iter);
			truemeeting++;
		}
	}
	return truemeeting;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int dmeeting = 0;
	list<Meeting>::iterator iter;
	for ( iter=m_meetingList.begin(); iter!=m_meetingList.end(); iter++ ) {
		if ( filter( *iter )==true ) {
			iter = m_meetingList.erase( iter );
			dmeeting++;
			iter--;
		}
	}
	return dmeeting;
}

bool Storage::sync(void) {
	writeToFile();
}