/**
Copyright (c) 2019 Dan Orban
*/

#ifndef WebSerVER_H_
#define WebSerVER_H_

#include <string>
#include <vector>
#include <map>
#include <libwebsockets.h>

class WebServerBase {
public:
	WebServerBase(int port = 1942, const std::string& webDir = ".");
	~WebServerBase();

	class Session {
	friend class WebServerBase;
	public:
		Session();
		virtual ~Session();
		virtual int getId() const { return id; }
		virtual void receiveMessage(const std::string& msg) {}
		virtual void sendMessage(const std::string& msg);
		virtual void update() {}
		virtual void onWrite();
	private:
		void* state;
		int id;
	};

	void service(int time = 10);

	virtual void createSession(void* info);
	

protected:
	virtual Session* createSession() { return new Session(); }

public:
	lws_context *context;
	std::vector<Session*> sessions;
	std::map<int, Session*> sessionMap;
	std::string webDir;
};

template <typename T>
class WebServer : public WebServerBase {
public:
	WebServer(int port = 1942, const std::string& webDir = ".") : WebServerBase(port, webDir) {}
protected:
	Session* createSession() { return new T(); }
};

template <typename T, typename STATE>
class WebServerWithState : public WebServerBase {
public:
	WebServerWithState(STATE state, int port = 1942, const std::string& webDir = ".") : WebServerBase(port, webDir), state(state) {}
protected:
	Session* createSession() { return new T(state); }
private:
	STATE state;
};

// JSON support
#include "picojson.h"

class JSONSession : public WebServerBase::Session {
public:
	virtual void receiveJSON(picojson::value& val) {}
	
	virtual void sendJSON(picojson::value& val) {
		sendMessage(val.serialize());
	}

	void receiveMessage(const std::string& msg) {
		picojson::value val;
		std::string err = picojson::parse(val, msg);
		if (!err.empty() || msg[0] != '{' || msg[msg.length()-1] != '}') {
			std::cerr << msg << std::endl;
			std::cerr << "Parse error: " << err << std::endl;
		}
		else {
			receiveJSON(val);	
		}
	}
};

#endif
