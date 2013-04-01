#ifndef MW_SIGNALS_CONNECTION_H
#define MW_SIGNALS_CONNECTION_H

#include <tuple>
#include <memory>
#include <list>

namespace mw {

	template<class A, class B>
	class Signal;

	namespace signals {

		// Is used by mw::signal.
		class SignalInterface {
		protected:
			friend class Connection;

			SignalInterface() {}
			~SignalInterface() {}

			virtual void disconnect(int id) = 0;
		};

		// Used internally in mw::Signal to remember a connection made.
		struct ConnectionInfo {
			ConnectionInfo(int id_, void* signal_) {
				id = id_;
				signal = signal_;
			}

			void* signal;
			int id;
		};

		// A connection Object remembers a connection and gives infomation
		// if the connection is active or not.
		class Connection {
		public:
			// Creates a empty connection. By default the connection is not active.
			Connection() {
			}

			// Works in vc11! Works on gcc compiler?
			template<class A, class B> friend class mw::Signal;

			// Disconnect the active connection. The function connected to this connection 
			// will be removed from mw::signal automaticly.
			void disconnect() {
				if (c_ && c_->signal != nullptr) {
					SignalInterface* tmp = (SignalInterface*) c_->signal;
					tmp->disconnect(c_->id);
				}
			}

			// Returns true if the connection is still active else false.
			bool connected() const {
				return c_ && c_->signal != nullptr;
			}

		private:
			// Is called from mw::Signal to bind a connection.
			Connection(const std::shared_ptr<ConnectionInfo>& c) {
				c_ = c;
			}

			std::shared_ptr<ConnectionInfo> c_;
		};

		// Is to be extended by classes that wants to automaticly disconnect to mw::Signal when
		// instances of it is removed from memory.
		class Trackable {
		protected:
			// Works in vc11! Works on gcc compiler?
			template<class A, class B> friend class mw::Signal;		

			~Trackable() {
				for (Connection& connection : connections_) {
					connection.disconnect();
				}			
			}

			void add(const Connection& connection) {
				connections_.push_back(connection);
			}

			void removeDisconnectedConnection() {
				connections_.remove_if([&](const Connection& connection) {
					if (!connection.connected()) {
						return true;
					}
					return false;
				});
			}

			std::list<Connection> connections_;
		};

	} // Namespace signals.

} // Namespace mw.

#endif // MW_SIGNALS_CONNECTION_H
