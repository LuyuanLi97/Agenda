//AgendaUI.cpp
#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<list>
#include"AgendaUI.hpp"
using namespace std;
void agendaline() {
	cout<<"---------------------"<<"agenda"<<"-----------------------------"<<endl;
}

void line() {
	cout<<"--------------------------------------------------------"<<endl;
}

string split( int &startindex, string &line ) {
	int spaceindex;
	for ( int i=startindex; line[i]!='\0'; i++ ) {
		if ( line[i] == ' ' ) {
			spaceindex = i;
			break;
		}
	}	
	string s = line.substr( startindex, spaceindex-startindex );
	startindex = spaceindex+1;
	return s;
}

void menu() {
	agendaline();
	cout<<"Action:"<<endl;
	cout<<"o   -log out Agenda"<<endl;
	cout<<"dc  -delete Agenda account"<<endl;
	cout<<"lu  -list all Agenda user"<<endl;
	cout<<"cm  -create a meeting"<<endl;
	cout<<"la  -list all meetings"<<endl;
	cout<<"las -list all sponser meetings"<<endl;
	cout<<"lap -list all participate meetings"<<endl;
	cout<<"qm  -query meeting by title"<<endl;
	cout<<"qt  -query meeting by time interval"<<endl;
	cout<<"dm  -delete meeting by title"<<endl;
	cout<<"da  -delete all meetings"<<endl;
	line();
}

void login_memu() {
	agendaline();
	cout<<"Action:"<<endl;
	cout<<"l  - log in Agenda by user name and password"<<endl;
	cout<<"r  - register an Agenda account"<<endl;
	cout<<"q  - quit Agenda"<<endl;
	line();
	cout<<endl<<endl<<"Agenda: ~$ ";
}

AgendaUI::AgendaUI() {
	m_userName = "";
	m_userPassword = "";
}

void AgendaUI::OperationLoop(void) {
	while(1) {
		login_memu();
		string t_operation;
		t_operation = getOperation();
		if (t_operation=="l")
			userLogIn();
		else if (t_operation=="r")
			userRegister();
		else if (t_operation=="q") {
			quitAgenda();
			break;
		}
	}
}

void AgendaUI::startAgenda(void) {
	login_memu();
}

std::string AgendaUI::getOperation() {
	string operation;
	getline( cin, operation );
	return operation;
}

bool AgendaUI::executeOperation(std::string t_operation) {
		}
		

void AgendaUI::userLogIn(void) {
	cout<<"[log in] [username] [password]"<<endl;
	cout<<"[log in] ";
	string enter = getOperation();
	int startindex = 0;
	m_userName = split( startindex, enter );
	m_userPassword = split( startindex, enter );
	if ( m_userName=="" || m_userPassword=="" ) {
		cout<<"[error] log in fail!"<<endl;
	}
	else {
		if ( !( m_agendaService.userLogIn(m_userName,m_userPassword) ) ) {
			cout<<"[error] log in fail!"<<endl;
		}
		else {
			cout<<"[register] succeed!"<<endl;
			 while (1) {
			 	menu();
			 	cout<<"Agenda@"<<m_userName<<": # ";
			 	string t_operation = getOperation();
			 	if (t_operation=="o") {
					userLogOut();
					break;
				}
				else if (t_operation=="dc") {
					deleteUser();
					break;
				}
				else if (t_operation=="lu")
					listAllUsers();
				else if (t_operation=="cm")
					createMeeting();
				else if (t_operation=="la")
					listAllMeetings();
				else if (t_operation=="las")
					listAllSponsorMeetings();
				else if (t_operation=="lap")
					listAllParticipateMeetings();
				else if (t_operation=="qm")
					queryMeetingByTitle();
				else if (t_operation=="qt")
					queryMeetingByTimeInterval();
				else if (t_operation=="dm")
					deleteMeetingByTitle();
				else if (t_operation=="da")
					deleteAllMeetings();
		     }
	    }
	}
}

void AgendaUI::userRegister(void) {
	cout<<"[register] [user name] [password] [email] [phone]"<<endl;
	cout<<"[register] ";
	string enter = getOperation();
	int startindex = 0;
	string name, password,email,phone;
	name = split( startindex, enter ); 
	password = split( startindex, enter );
	email = split( startindex, enter );
	phone = split( startindex, enter );
	if ( name=="" || password=="" ) 
		cout<<"[error] register fail!"<<endl;
	else {
		if ( !( m_agendaService.userRegister(name,password,email,phone) ) )
			cout<<"[error] register fail!"<<endl;
		else
			cout<<"[register] succeed!"<<endl;
	}
}

void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut(void) {
	m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(void) {
	if ( m_agendaService.deleteUser(m_userName,m_userPassword) )
		cout<<"[delete agenda account] succeed!"<<endl;
}

void AgendaUI::listAllUsers(void) {
	cout<<endl<<endl<<"[list all users]"<<endl<<endl;
	cout<<left<<setw(15)<<"name"<<setw(20)<<"email"<<setw(15)<<"phone"<<endl;
	auto iter = m_agendaService.listAllUsers();
	for ( auto i = iter.begin(); i != iter.end(); i++ )
		cout<<left<<setw(15)<<i->getName()<<setw(20)<<i->getEmail()<<setw(15)<<i->getPhone()<<endl;
}

void AgendaUI::createMeeting(void) {
	vector<string> v; 
	cout<<"[create meeting] [the number of participators]"<<endl;
	cout<<"[create meeting] ";
	int par;
	cin>>par;
	getchar();
	for ( int i=0; i<par; i++ ) {
		cout<<"[create meeting] [please enter the participator "<<i+1<<" ]"<<endl;
		cout<<"[create meeting] ";
		string enter = getOperation();
		v.push_back( enter );
	}
	cout<<"[create meeting] [title][start time(yyyy-mm-dd/hh:mm)][end time(yyyy-mm-dd/hh:mm)]"<<endl;
	cout<<"[create meeting] ";
	string title,start,end;
	string enter = getOperation();
	int startindex = 0;
	title = split( startindex, enter );
	start = split( startindex, enter );
	end = split( startindex, enter );
	if ( m_agendaService.createMeeting(m_userName,title,start,end,v) )
		cout<<"[create meeting] succeed!"<<endl;
	else
		cout<<"[create meeting] error!"<<endl;
}

void AgendaUI::listAllMeetings(void) {
	cout<<endl<<endl<<"[list all meetings]"<<endl<<endl;
	printMeetings( m_agendaService.listAllMeetings(m_userName) );
}

void AgendaUI::listAllSponsorMeetings(void) {
	cout<<endl<<endl<<"[list all sponsor meetings]"<<endl<<endl;
	printMeetings( m_agendaService.listAllSponsorMeetings(m_userName) );
}

void AgendaUI::listAllParticipateMeetings(void) {
	cout<<endl<<endl<<"[list all participate meetings]"<<endl<<endl;
	printMeetings( m_agendaService.listAllParticipateMeetings(m_userName) );
}

void AgendaUI::queryMeetingByTitle(void) {
	cout<<endl<<endl<<"[query meeting] [title]"<<endl;
	cout<<"[query meeting] ";
	string title = getOperation();
	printMeetings( m_agendaService.meetingQuery(m_userName,title) );
}

void AgendaUI::queryMeetingByTimeInterval(void) {
	cout<<endl<<endl<<"[query meetings] [start time(yyyy-mm--dd/hh:mm)] [end time(yyyy-mm--dd/hh:mm)]"<<endl;
	cout<<"[query meetings] ";
	string line = getOperation();
	string start,end;
	int startindex = 0;
	start = split( startindex,line );
	end =  split( startindex,line );
	printMeetings( m_agendaService.meetingQuery(m_userName,start,end) );
}

void AgendaUI::deleteMeetingByTitle(void) {
	cout<<endl<<endl<<"[delete meeting] [title]"<<endl;
	cout<<"[delete meeting] ";
	string title = getOperation();
	if ( m_agendaService.deleteMeeting(m_userName,title) )
		cout<<"[delete meeting by title] succeed!"<<endl;
	else
		cout<<"[error] delete meeting fail!"<<endl;
}

void AgendaUI::deleteAllMeetings(void) {
	if ( m_agendaService.deleteAllMeetings(m_userName) )
		cout<<"[delete all meetings] succeed!"<<endl;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
	cout<<left<<setw(18)<<"title"<<setw(18)<<"sponsor"<<setw(18)<<"start time"<<setw(18)<<"end time"<<"participators"<<endl;
	list<Meeting>::iterator iter;
	for ( iter=t_meetings.begin(); iter!=t_meetings.end(); iter++ ) {
		cout<<left<<setw(18)<<iter->getTitle()<<setw(18)<<iter->getSponsor();
		cout<<setw(18)<<Date::dateToString( iter->getStartDate() )<<setw(18)<<Date::dateToString( iter->getEndDate() );
		auto p_list = iter->getParticipator();
		for ( auto i=p_list.begin(); i!=p_list.end(); i++ ) {
			if ( i==p_list.begin() )
				cout<<*i;
			else cout<<","<<*i;
		}
		cout<<endl;
	}
}


