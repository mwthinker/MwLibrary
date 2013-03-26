#ifndef MW_SIGNALS_CONNECTION_H
#define MW_SIGNALS_CONNECTION_H

#include <tuple>
#include <memory>
#include <list>

namespace mw {

	template<class A, class B>
	class Signal;

	namespace signals {

		class SignalInterface {
		protected:
			friend class Connection;

			SignalInterface() {}
			~SignalInterface() {}

			virtual void disconnect(int id) = 0;
		};

		struct ConnectionInfo {
			ConnectionInfo(int id_, void* signal_) {
				id = id_;
				signal = signal_;
			}

			void* signal;
			int id;
		};

		class Connection {
		public:
			Connection() {
				c_ = 0;
			}

			// Works in vc11! Works on gcc compiler?
			template<class A, class B> friend class mw::Signal;

			void disconnect() {
				if (c_ && c_->signal != nullptr) {
					SignalInterface* tmp = (SignalInterface*) c_->signal;
					tmp->disconnect(c_->id);
				}
			}

			bool connected() const {
				return c_ && c_->signal != nullptr;
			}

		private:
			Connection(const std::shared_ptr<ConnectionInfo>& c) {
				c_ = c;
			}

			std::shared_ptr<ConnectionInfo> c_;
		};

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
