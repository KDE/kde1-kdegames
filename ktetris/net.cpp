#include "net.h"
#include "hack.h"

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#include <qmsgbox.h>


Player::Player( NetObject *pa )
{
	parent = pa;
	sock = -1;
	label = NULL;
	height = 0;
	lines = 0;
}

Player::~Player()
{
	if ( label!=NULL )
		delete label;
	
	/* if connected : try to send an END_MSG */
	if ( sock!=-1 ) {
		parent->writeSelect();
		if ( parent->writeIsset(sock) ) {
			write( sock, END_MSG, (int)strlen(END_MSG) );
		}
		close(sock);
	}
}

NetObject::NetObject( QString& tmp_address, QString& tmp_name,
					 QString& tmp_port )
{
	if ( tmp_address.isEmpty() )
		t_address = getenv("HOSTNAME");
	else
		t_address = (const char *)tmp_address;
	if ( tmp_name.isEmpty() )
		t_name = "Nikos";
	else
		t_name = (const char *)tmp_name;
	if ( tmp_port.isEmpty() )
		t_port = "5001";
	else
		t_port = (const char *)tmp_port;
	
	init();
}

void NetObject::init( )
{
	max_fd = 0;
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	
	/* create a first player */
	nb_pl = 0;
	pl = new Player *;
	pl[0] = new Player(this);
}


void NetObject::clean()
{
	for (int i=0; i<=nb_pl; i++)
		delete pl[i];
	delete pl;

	init();
}


void NetObject::deleteLabel(int i)
{
	if ( pl[i]->label!=NULL ) {
		delete pl[i]->label;
		pl[i]->label = NULL;
	}
}


void NetObject::initSelect(int time)
{
	read_tmp = read_set;
	write_tmp = write_set;
	
	tv.tv_sec = time;
	tv.tv_usec = 0;
}


void NetObject::readSelect(int time)
{
	initSelect(time);
	select(max_fd+1, &read_tmp, NULL, NULL, &tv);
}


void NetObject::writeSelect(int time)
{
	initSelect(time);
	select(max_fd+1, NULL, &write_tmp, NULL, &tv);
}


bool NetObject::checkParam( const char *nname, const char *naddress,
						    const char *nport, QString& serror )
{
	struct hostent *host;
	bool ok;
	
	serror = "";
	
	/* set address and name of the first player */
	pl[0]->address = naddress;
	pl[0]->name = nname;

	/* test & set the port */
	t_port = nport;
	int tmp_port = t_port.toInt(&ok);
	if ( !ok || tmp_port<=0 ) {
		serror = "Illegal port";
		return FALSE;
	}
	
	/* test the address */
	if ( naddress[0]==0 ) {
		serror += "Null address";
		return FALSE;
	}
	if ( isdigit(naddress[0]) ) {
		netaddr = inet_addr(naddress);
		if( netaddr==-1 ) {
			serror += "Unknown address";
			return FALSE;
		}
	} else {
		host = gethostbyname(naddress);
		if ( host==NULL ) {
			serror += "Unknown address";
			return FALSE;
		}
		netaddr = *(long *)(host->h_addr);
	}
	return TRUE;
}


bool NetObject::createSocket( QString& serror )
{
	int sock;
	
	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if ( sock<0 ) {
		serror = "Cannot create socket";
		return FALSE;
	}
	max_fd = sock;
	
	/* set socket's options */
	int one = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));
	
	/* assign the descriptor to the first player */
	pl[0]->sock = sock;
	return TRUE;
}


bool NetObject::hasReceivedMsg(const char *msg)
{
	return ( strncmp(buff, msg, strlen(msg))==0 );
}


bool NetObject::checkData( const char *serror1, const char *serror2,
						  QString& serror )
{
	/* contact died or other error */
	if ( nb_b<=0 ) {
		serror = serror1;
		return FALSE;
	}
	/* test if END_MSG */
	if ( hasReceivedMsg(END_MSG) ) {
		serror = serror2;
		return FALSE;
	}
	return TRUE;
}


void NetObject::getPlayerInfos(QString &infos, int i)
{
	infos = (const char *)pl[i]->name;
	infos += " at ";
	infos += (const char *)pl[i]->address;
}


void NetObject::ownStatus(int height, int lines)
{
	pl[0]->height = height;
	pl[0]->lines = lines;
}


void NetObject::gameOver( QString& serror )
{
	QString msg = "Game over :" + serror;
	ERROR(msg);
	endClientConnection();
}


/* client net object */
ClientNetObject::ClientNetObject( QString& tmp_address, QString& tmp_name,
								  QString& tmp_port )
: NetObject(tmp_address, tmp_name, tmp_port)
{
	timeout = CLIENT_TIMEOUT;
	mode = CLIENT_MODE;
}


bool ClientNetObject::connectSocket( QString& serror )
{
	/* set the net address */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = netaddr;
	
	/* connect the socket to server */
	if ( Connect(pl[0]->sock, (struct sockaddr *)&sin, sizeof(sin))<0 ) {
		serror ="Connect error !";
		return FALSE;
	}
	
	FD_SET(pl[0]->sock, &read_set);
	FD_SET(pl[0]->sock, &write_set );

	/* send name */
	writeSelect();
    if ( !writeIsset(pl[0]->sock) ) {	
		serror = "Server unreachable";
        return FALSE;    
	}
	strncpy(name, (const char *)pl[0]->name, NAME_LENGTH);
    write( pl[0]->sock, name, (int)strlen(name)+1 );
	
	return TRUE;
}


bool ClientNetObject::dialogTimeout( QString& serror )
{
	/* message from server :
	 * 	   <=0 means the server died (or an error)
	 * 	   END_MSG means a cancel of the server
	 * 	   PLAY_MSG means to get ready for the following */
	readSelect();
	
	if ( !readIsset(pl[0]->sock) ) {
		serror = "";
		return FALSE;
	}
	nb_b = read(pl[0]->sock, buff, MAX_BUFF);   
	
	if ( !checkData("The server has died", "Cancel from server", serror) )
		return FALSE;

	if ( hasReceivedMsg(PLAY_MSG) )
		return TRUE;
	
	serror = "Unknown data from server";
	return FALSE;
}

bool ClientNetObject::initGame( QString& serror )
{
	/* create the previous and the next opponent ie the players #1 & #2 */
	new_pl = new Player *[3];
	new_pl[0] = pl[0];
	delete pl;
	pl = new_pl;
	pl[1] = new Player(this);
	pl[2] = new Player(this);
	nb_pl = 2;
	
	/* read name of these opponents */
	strncpy( names_msg.prev, &(buff[strlen(PLAY_MSG)]), NAME_LENGTH );
	strncpy( names_msg.next, &(buff[strlen(PLAY_MSG)+NAME_LENGTH]),
			 NAME_LENGTH );
	pl[1]->name = names_msg.next;
	pl[2]->name = names_msg.prev;
	
	/* send READY_MSG to server */
	writeSelect();
	
	if ( !writeIsset(pl[0]->sock) ) {
		serror = "Server unreachable";
		return FALSE;
	}
	write( pl[0]->sock, READY_MSG, (int)strlen(READY_MSG) );
	
	return TRUE;
}

bool ClientNetObject::playTimeout( QString& serror )
{
	/* send client message to server */
	writeSelect();
	if ( !writeIsset(pl[0]->sock) ) {
		serror = "Cannot send to server";
		return FALSE;
	}
	c_msg.height = pl[0]->height;
	c_msg.lines = pl[0]->lines;
	c_msg.end = 0;
	write(pl[0]->sock, &c_msg, sizeof(c_msg));
	fsync(pl[0]->sock);

	/* read server message */
	readSelect(1);
	if ( !readIsset(pl[0]->sock) ) {
		serror = "Client timeout (No message from server)";
		return FALSE;
	}
	nb_b = read(pl[0]->sock, &s_msg, sizeof(s_msg));
	if ( nb_b<=0 ) {
		serror = "Server has vanished";
		return FALSE;
	}
	if ( nb_b!=sizeof(s_msg) ) {
		serror = "Unknown data from server";
		return FALSE;
	}
	pl[2]->height = s_msg.height_prev;
	pl[2]->name = s_msg.name_prev;
	pl[1]->height = s_msg.height_next;
	pl[1]->name = s_msg.name_next;
	/* the full lines done by the previous opponent */
	pl[2]->lines = s_msg.lines;
	
	return TRUE;
}


/* server net object */
ServerNetObject::ServerNetObject( QString& tmp_address, QString& tmp_name,
								  QString& tmp_port )
: NetObject(tmp_address, tmp_name, tmp_port)
{
	timeout = SERVER_TIMEOUT;
	mode = SERVER_MODE;
	isConnected = FALSE;
}


bool ServerNetObject::connectSocket( QString& serror )
{
	/* set the net address */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl (INADDR_ANY);

	/* bind the socket */
	if ( bind(pl[0]->sock, (struct sockaddr *) &sin, sizeof (sin))<0 ) {
		serror = "Bind error !";
		return FALSE;
	}
	/* listen */
	if ( listen(pl[0]->sock, 1)<0 ) {
		serror ="Listen error !";
		return FALSE;
	}
	FD_SET(pl[0]->sock, &read_set);
	
	isConnected = TRUE;
	
	return TRUE;
}


bool ServerNetObject::dialogTimeout( QString& serror )
{
	readSelect();
	
	/* contact from a new player */
	if ( readIsset(pl[0]->sock) ) {
		addrlen = sizeof(sin);
		new_sock = Accept( pl[0]->sock, (struct sockaddr *)&sin, &addrlen );
		if ( new_sock<0 ) {
			serror = "Unknown message from unknown player ??";
			return FALSE;
		}
		
		/* create a list of players bigger of one */
		nb_pl++;
		new_pl = new Player *[nb_pl+1];
		for (int k=0; k<=(nb_pl-1); k++)
			new_pl[k] = pl[k];
		new_pl[nb_pl] = new Player(this);
		delete pl;
		pl = new_pl;
		
		/* set the new player parameters */
		pl[nb_pl]->sock = new_sock;
		pl[nb_pl]->name = "";
		
		addr = inet_ntoa(sin.sin_addr);
		netaddr = inet_addr( (const char *)addr );
		host = gethostbyaddr( (char *)&netaddr, sizeof(netaddr), AF_INET );
		pl[nb_pl]->address = host->h_name;
		pl[nb_pl]->address += " (";
		pl[nb_pl]->address += addr;
		pl[nb_pl]->address += ")";
			
		FD_SET(new_sock, &read_set);
		FD_SET(new_sock, &write_set);
		if ( new_sock>max_fd )
			max_fd = new_sock;
		return TRUE;
	}
	
	/* message from other known players
	 * 	 <=0 means player died (or another error)
	 * 	 END_MSG means a cancel from player */
	for (int i=1; i<=nb_pl; i++)
		if ( readIsset(pl[i]->sock) ) {
			nb_b = read(pl[i]->sock, buff, MAX_BUFF);

			if ( !checkData( "", "", serror ) ) {
				/* delete the corresponding player */
				nb_pl--;
				FD_CLR(pl[i]->sock, &read_set);
				FD_CLR(pl[i]->sock, &write_set);
				
				delete pl[i];
				
				/* reorganize list of players */
				new_pl = new Player *[nb_pl+1];
				for (int k=0; k<=nb_pl; k++) {
					if ( k<i )
						new_pl[k] = pl[k];
					else
						new_pl[k] = pl[k+1];
				}
				delete pl;
				pl = new_pl;
			} else {
				if ( nb_b>NAME_LENGTH ) {
					serror = "Unknown message from client";
					return FALSE;
				}
				pl[i]->name = buff;
			}
			return TRUE;
		}

	serror = "";
	return FALSE;
}


bool ServerNetObject::initGame( QString& serror)
{
	for (int i=1; i<=nb_pl; i++) {	
		writeSelect(1);
		if ( writeIsset(pl[i]->sock) ) {
			/* send PLAY_MSG to clients */
			write( pl[i]->sock, PLAY_MSG, (int)strlen(PLAY_MSG) );
			
			/* send opponents' names */
			strncpy(names_msg.prev, (const char *)pl[i-1]->name, NAME_LENGTH);
			if ( i!=nb_pl )
				strncpy( names_msg.next, (const char *)pl[i+1]->name,
						NAME_LENGTH );
			 else
				strncpy( names_msg.next, (const char *)pl[0]->name,
						NAME_LENGTH );
			write( pl[i]->sock, &names_msg, sizeof(names_msg) );
		}
	 }

	/* receive READY_MSG from clients */
	for (int i=1; i<=nb_pl; i++) {
		readSelect(1);
		if ( readIsset(pl[i]->sock) ) {
			nb_b = read( pl[i]->sock, buff, MAX_BUFF );
			 if ( !checkData("Client died", "Client sent cancel", serror) )
				return FALSE;
			if ( !hasReceivedMsg(READY_MSG) ) {
				serror = "Unknown message from client";
				return FALSE;
			}
		} else {
			serror = "Timeout !";
			return FALSE;        
		}
	}
	
	/* set the "first time" bool */
	firstTime = TRUE;
	
	return TRUE;
}


bool ServerNetObject::playTimeout( QString& serror )
{
	int i;

	readSelect();
	
	if ( !firstTime ) {
		/* read the message sent by client : */
		/*   - no message
		 *   - nb_b<=0 : client canceled or died 
		 *   - nb_b != sizeof(clientMsg) : unknown data
	     *   else store data */
		
		for (i=1; i<=nb_pl; i++) {
			if ( !readIsset(pl[i]->sock) ) {
				serror = "No message from client";
				return FALSE;
			}
			nb_b = read( pl[i]->sock, &c_msg, sizeof(c_msg) );
			if ( nb_b<=0 ) {
				serror = "Client has vanished";
				return FALSE;
			}
			if ( nb_b!=sizeof(c_msg) ) {
				serror = "Unknown data from client";
				return FALSE;
			}
			if ( c_msg.end!=0 ) {
				serror = "End message from client";
				return FALSE;
			}
			
			pl[i]->height = c_msg.height;
			pl[i]->lines = c_msg.lines;
		}
	} else
		firstTime = FALSE;
	
	writeSelect();
	
	for (i=1; i<=nb_pl; i++) {
		/* send message to all clients : */
		/*   - cannot send
		 * 	 else send s_msg */
		if ( !writeIsset(pl[i]->sock) ) {
			serror = "Cannot send to client";
			return FALSE;
		}
		strncpy(s_msg.name_prev, (const char *)pl[i-1]->name, NAME_LENGTH);
		s_msg.height_prev = pl[i-1]->height;
		s_msg.lines = pl[i-1]->lines;
		if ( i!=nb_pl ) {
			s_msg.height_next = pl[i+1]->height;
			strncpy(s_msg.name_next, (const char *)pl[i+1]->name, NAME_LENGTH);
		} else {
			s_msg.height_next = pl[0]->height;
			strncpy(s_msg.name_next, (const char *)pl[0]->name, NAME_LENGTH);
		}
		write(pl[i]->sock, &s_msg, sizeof(s_msg));
		fsync(pl[i]->sock);
	}
	return TRUE;
}


void ServerNetObject::endClientConnection()
{
	/* if not connected : clean
	 else delete all connections except the original one */
	if ( !isConnected )
		clean();
	else {
		for (int i=1; i<=nb_pl; i++)
			delete pl[i];
		
		new_pl = new Player*[1];
		new_pl[0] = pl[0];
		delete pl;
		pl = new_pl;

		nb_pl = 0;
		max_fd = pl[0]->sock;
		
		/* clean fd_set */
		FD_ZERO(&read_set);
		FD_SET(pl[0]->sock, &read_set);
		FD_ZERO(&write_set);
	}
}

	
