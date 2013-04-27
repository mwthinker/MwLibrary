#ifndef MW_SIGNAL_H
#define MW_SIGNAL_H

#include "signals/connection.h"

#include <tuple>
#include <functional>
#include <list>
#include <memory>

namespace mw {

	// Is a class that holds functions. A slot/callbacks class.
	template <class A=void, class B=void>
	class Signal : public signals::SignalInterface {
	public:
		typedef std::function<void(A,B)> Callback;

		Signal() {
			id = 0;
		}

		~Signal() {
			for (TupleIdFunctionC& tuple : functions_) {
				auto c = std::get<2>(tuple);
				c->signal = nullptr;
			}
		}

		signals::Connection connect(std::function<void(A,B)> function) {
			ConnectionInfoPtr c = std::make_shared<signals::ConnectionInfo>(++id, this);
			functions_.push_back(TupleIdFunctionC(id,function,c));
			return signals::Connection(c);
		}

		void operator()(A a, B b) {
			for (TupleIdFunctionC& tuple : functions_) {
				auto& function = std::get<1>(tuple);
				function(a,b);
			}
		}

		void disconnect(int id) {
			functions_.remove_if([&](const TupleIdFunctionC& tuple) {
				if (std::get<0>(tuple) == id) {
					ConnectionInfoPtr cI = std::get<2>(tuple);
					cI->signal = nullptr;
					return true;
				}
				return false;
			});
		}

	private:
		Signal(const Signal& signal) {
			// Not be used. Is not copyable.
		}

		Signal& operator=(const Signal& signal) {
			// Not be used. Is not copyable.
			return *this;
		}

		int id;
		typedef std::shared_ptr<signals::ConnectionInfo> ConnectionInfoPtr;
		typedef std::tuple<int, std::function<void(A,B)>, ConnectionInfoPtr> TupleIdFunctionC;
		std::list<TupleIdFunctionC> functions_;
	};

	template <class A>
	class Signal<A, void> : public signals::SignalInterface {
	public:
		typedef std::function<void(A)> Callback;

		Signal() {
			id = 0;
		}

		~Signal() {
			for (TupleIdFunctionC& tuple : functions_) {
				auto c = std::get<2>(tuple);
				c->signal = nullptr;
			}
		}

		signals::Connection connect(std::function<void(A)> function) {
			ConnectionInfoPtr c = std::make_shared<signals::ConnectionInfo>(++id, this);
			functions_.push_back(TupleIdFunctionC(id,function,c));
			return signals::Connection(c);
		}

		void operator()(A a) {
			for (TupleIdFunctionC& tuple : functions_) {
				auto& function = std::get<1>(tuple);
				function(a);
			}
		}

		void disconnect(int id) {
			functions_.remove_if([&](const TupleIdFunctionC& tuple) {
				if (std::get<0>(tuple) == id) {
					ConnectionInfoPtr cI = std::get<2>(tuple);
					cI->signal = nullptr;
					return true;
				}
				return false;
			});
		}

	private:
		Signal(const Signal& signal) {
			// Not be used. Is not copyable.
		}

		Signal& operator=(const Signal& signal) {
			// Not be used. Is not copyable.
			return *this;
		}

		int id;
		typedef std::shared_ptr<signals::ConnectionInfo> ConnectionInfoPtr;
		typedef std::tuple<int, std::function<void(A)>, ConnectionInfoPtr> TupleIdFunctionC;
		std::list<TupleIdFunctionC> functions_;
	};

	template <>
	class Signal<void, void> : public signals::SignalInterface {
	public:
		typedef std::function<void()> Callback;

		Signal() {
			id = 0;
		}

		~Signal() {
			for (TupleIdFunctionC& tuple : functions_) {
				auto c = std::get<2>(tuple);
				c->signal = nullptr;
			}
		}

		signals::Connection connect(std::function<void()> function) {
			ConnectionInfoPtr c = std::make_shared<signals::ConnectionInfo>(++id, this);
			functions_.push_back(TupleIdFunctionC(id,function,c));
			return signals::Connection(c);
		}

		void operator()() {
			for (TupleIdFunctionC& tuple : functions_) {
				auto& function = std::get<1>(tuple);
				function();
			}
		}

		void disconnect(int id) {
			functions_.remove_if([&](const TupleIdFunctionC& tuple) {
				if (std::get<0>(tuple) == id) {
					ConnectionInfoPtr cI = std::get<2>(tuple);
					cI->signal = nullptr;
					return true;
				}
				return false;
			});
		}

	private:
		Signal(const Signal& signal) {
			// Not be used. Is not copyable.
		}

		Signal& operator=(const Signal& signal) {
			// Not be used. Is not copyable.
			return *this;
		}

		int id;
		typedef std::shared_ptr<signals::ConnectionInfo> ConnectionInfoPtr;
		typedef std::tuple<int, std::function<void()>, ConnectionInfoPtr> TupleIdFunctionC;
		std::list<TupleIdFunctionC> functions_;
	};

} // Namespace mw.

#endif // MW_SIGNAL_H
