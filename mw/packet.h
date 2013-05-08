#ifndef MW_PACKET_H
#define MW_PACKET_H

#include <vector>

namespace mw {

	enum PacketType {RELIABLE,UN_RELIABLE};

	class Packet {
	public:
		Packet() {
			index_ = -1;
		}

		Packet(const std::vector<char>& data) {
			index_ = -1;
			data_ = data;
		}

		Packet& operator<<(const Packet& packet) {
			data_.insert( data_.end(), packet.data_.begin(), packet.data_.end());
			return *this;
		}

		Packet& operator>>(char& data) {
			data = data_[++index_];
			return *this;
		}

		Packet& operator<<(char data) {
			data_.push_back(data);
			return *this;
		}

		const std::vector<char>& getData() const {
			return data_;
		}

		int size() const {
			return data_.size();
		}

		void push_back(char byte) {
			data_.push_back(byte);
		}

		char operator[](int index) const {
			return data_[index];
		}

		unsigned int dataLeftToRead() const {
			return data_.size() - index_ - 1;
		}

	private:
		std::vector<char> data_;
		int index_;	
	};

} // Namespace mw.

#endif // MW_PACKET_H
