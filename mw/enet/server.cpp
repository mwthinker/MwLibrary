#include "server.h"

#include <vector>
#include <iostream>

namespace mw {
	namespace enet {
		
		Server::Server(int port, ServerFilter* serverFilter) {
			maxNbrOfRemoteClients_ = 4;

			address_.host = ENET_HOST_ANY;
			address_.port = port;

			// Garanties that the server always has the correct id.
			id_ = Network::getServerId();

			// The id to be assigned to the next connected client.
			currentId_ = id_+1;
			serverFilter_ = serverFilter;
		}

		Server::~Server() {
			for (auto it = peers_.begin(); it != peers_.end(); ++it) {
				enet_peer_reset(it->first);
			}
			if (server_ != 0) {
				enet_host_destroy(server_);
			}
		}

		void Server::start() {
			if (getStatus() == NOT_ACTIVE) {
				setStatus(ACTIVE);
				// Remove old package.
				while (!sendPackets_.empty()) {
					sendPackets_.pop();
				}
				while (!receivePackets_.empty()) {
					receivePackets_.pop();
				}

				// Create a host using enet_host_create
				server_ = enet_host_create(&address_, 32, 2, 0, 0);

				if (server_ == NULL) {
					fprintf(stderr, "An error occured while trying to create an ENet server host\n");
					exit(EXIT_FAILURE);
				}
			}
		}

		void Server::stop() {
			if (getStatus() == ACTIVE) {
				setStatus(DISCONNECTING);
				for (auto it = peers_.begin(); it != peers_.end(); ++it) {
					ENetPeer* peer = it->first;
					enet_peer_disconnect(peer,0);
				}
			}
		}

		void Server::update() {
			if (getStatus() != NOT_ACTIVE) {
				ENetEvent eNetEvent;

				int eventStatus = 0;
				while (getStatus() != NOT_ACTIVE &&
					(eventStatus = enet_host_service(server_, &eNetEvent, 0)) > 0) {
						switch(eNetEvent.type) {
						case ENET_EVENT_TYPE_CONNECT:
							//printf("(Server) We got a new connection from %x\n",eNetEvent.peer->address.host);
							if (getStatus() != DISCONNECTING) {
								// Signal the client that a new client is connected!
								// Is the connection accepted?
								if (serverFilter_->sendThrough(Packet(),currentId_+1,currentId_+1,ServerFilter::NEW_CONNECTION)) {
									// Assign id to client and set the next id to an uniqe value.
									Pair pair(eNetEvent.peer,++currentId_);
									peers_.push_back(pair);

									// Send info about the new client to everybody.
									sendConnectInfoToPeers(peers_);
									Packet iPacket;
								} else {
									enet_peer_disconnect(eNetEvent.peer,0);
								}
							} else {
								// Stops new connections to be made.
								enet_peer_disconnect(eNetEvent.peer,0);
							}
							break;
						case ENET_EVENT_TYPE_RECEIVE:
							if (getStatus() != NOT_ACTIVE) {
								InternalPacket iPacket = receive(eNetEvent);
								// No data to send? Or data through the filter is to be sent through?
								if (iPacket.data_.size() > 0 && serverFilter_->sendThrough(iPacket.data_,iPacket.fromId_,iPacket.toId_,ServerFilter::PACKET)) {
									// Sent to you specific?
									if (iPacket.toId_ == getId()) {
										receivePackets_.push(iPacket);
									} else if (iPacket.toId_ != 0) { // Send to a specific client?
										sendPackets_.push(iPacket);
									} else { // Send to all!
										receivePackets_.push(iPacket);
										sendPackets_.push(iPacket);
									}
								}
							}
							enet_packet_destroy (eNetEvent.packet);
							break;
						case ENET_EVENT_TYPE_DISCONNECT:
							{
								printf("%s disconnected.\n", (char*)eNetEvent.peer->data);
								// Reset client's information
								auto it = peers_.begin();
								for (; it != peers_.end(); ++it) {
									if (it->first == eNetEvent.peer) {
										break;
									}
								}

								// Remove the connection if its old (i.e. it is in the vector
								// and not a turned down connection).
								if (it != peers_.end()) {
									InternalPacket iPacket(Packet(),it->second,PacketType::RELIABLE);

									// Signal the server that a client is disconnecting.
									serverFilter_->sendThrough(iPacket.data_,iPacket.fromId_,iPacket.toId_,ServerFilter::DISCONNECTED);							

									// Remove peer from vector.
									std::swap(*it,peers_.back());
									peers_.pop_back();

									// Send the updated clientlist to all clients.
									sendConnectInfoToPeers(peers_);
								}

								// When all peers is disconnected, then clean up.
								if (getStatus() == DISCONNECTING && peers_.size() == 0) {
									eNetEvent.peer->data = NULL;
									setStatus(NOT_ACTIVE);
								}

								eNetEvent.peer->data = NULL;					
							}
							break;
						case ENET_EVENT_TYPE_NONE:
							break;
						}
				}

				// Send all packets in send buffert to all clients.
				while (getStatus() != NOT_ACTIVE && !sendPackets_.empty()) {
					InternalPacket& iPacket = sendPackets_.front();

					// Data to send? And data through the filter is allowed to be sent?
					if (iPacket.data_.size() > 0 && serverFilter_->sendThrough(iPacket.data_,iPacket.fromId_,iPacket.toId_,ServerFilter::PACKET)) {
						// Send the packet to the peer over channel id 0.
						// enet handles the cleen up of eNetPacket;
						for (auto it = peers_.begin(); it != peers_.end(); ++it) {
							int id = it->second;					

							// Send to all?
							if (iPacket.toId_== 0) {
								// The sender?
								if (iPacket.fromId_ == id) {
									// Skip to return data to the sender.
									continue;
								}

								ENetPacket* eNetPacket = createEnetPacket(iPacket.data_,iPacket.fromId_,iPacket.type_);
								ENetPeer* peer = it->first;
								enet_peer_send(peer, 0, eNetPacket);
							} else if (id == iPacket.toId_) { // Send to the specific client?
								ENetPacket* eNetPacket = createEnetPacket(iPacket.data_,iPacket.fromId_,iPacket.type_);
								ENetPeer* peer = it->first;
								enet_peer_send(peer, 0, eNetPacket);

								// Only send to one client.
								break;
							}
						}
					}

					sendPackets_.pop();
				}

				enet_host_flush(server_);
				
				// The server is not active? Or the disconnection is finish?
				if (getStatus() == NOT_ACTIVE || (getStatus() == DISCONNECTING && peers_.size() == 0)) {
					enet_host_destroy(server_);
					server_ = 0;
					setStatus(NOT_ACTIVE);
				}
			}
		}

		int Server::getId() const {
			return id_;
		}

		std::vector<int> Server::getConnectionIds() const {
			std::vector<int> ids;
			for (auto it = peers_.begin(); it != peers_.end(); ++it) {
				ids.push_back(it->second);
			}
			return ids;
		}

		// protected
		Server::InternalPacket Server::receive(ENetEvent eNetEvent) {
			ENetPacket* packet = eNetEvent.packet;

			//char id = packet->data[1];
			//Find the id for the client which sent the package.
			auto it = peers_.begin();
			for (; it != peers_.end(); ++it) {
				if (it->first == eNetEvent.peer) {
					break;
				}
			}

			char id = it->second;

			// TODO!! Stop connection which violates the protocol.
			char type = packet->data[0];
			char toId = packet->data[1];

			switch (type) {
			case CONNECT_INFO: 
				// ERROR. SERVER SHOULD ONLY SEND NOT RECEIVE CONNECT_INFO.
				// TODO!! Stop connection which violates the protocol.
				break;
			case PACKET:
				{
					// [0]=type,[1]=id,[2...]=data
					std::vector<char> data(packet->data+2,packet->data+packet->dataLength);
					return InternalPacket(Packet(data),id,PacketType::RELIABLE,toId);
				}
			}

			// TODO!! Stop connection which violates the protocol. ERROR.
			return InternalPacket(Packet(),0,PacketType::RELIABLE);
		}

		// private

		// Sends connectInfo to new connected client. Client is assigned
		// the number id.
		// 0   char type = |CONNECT_INFO
		// 1   char id   = |id
		// 2   char id1  = |?
		//		...
		// N-1 char idN  = |?
		void Server::sendConnectInfoToPeers(const std::vector<Pair>& peers) const {
			char data[256];
			data[0] = CONNECT_INFO;
			//peers_.push_back(Pair(peer,id));
			int size = peers.size();
			for (auto it = peers.begin(); it != peers.end(); ++it) {
				data[1] = it->second;
				for (int i = 0; i < size; ++i) {
					data[i+2] = peers[i].second;
				}

				ENetPacket* eNetPacket = enet_packet_create(data,size+2,ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(it->first, 0, eNetPacket);
			}
		}

	} // Namespace enet.
} // Namespace mw.
