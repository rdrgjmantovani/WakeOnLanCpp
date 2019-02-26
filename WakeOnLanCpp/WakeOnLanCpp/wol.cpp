#include "wol.h"

WOL::WOL() { }
WOL::~WOL() { }

void WOL::setup() {
	std::ifstream ifs;

	ifs.open(JSON_FILE, std::ifstream::in);
		
	if (!ifs.is_open()) {

		std::stringstream ss;
		ss << "Could not open " << JSON_FILE;		
		throw std::exception(ss.str().c_str());
	}			

	Json::Reader reader;
	Json::Value obj;

	reader.parse(ifs, obj);

	const Json::Value arr = obj[JSON_PROPERTY];

	boost::regex expression{ MAC_PATTERN };

	for (int i = 0; i < arr.size(); i++) {

		std::string mac = arr[i].asString();

		if (!boost::regex_match(mac, expression)) {
			std::cout << "WARNING: Invalid MAC Address (" << mac << ")" << std::endl;
			continue;
		}
		_macAddresses.push_back(arr[i].asString());
	}
		
	ifs.close();
}

void WOL::sendMagicPackets() {
	boost::asio::io_context context;
	
	boost::asio::ip::udp::socket socket(context);

	boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4::broadcast(), PORT);
	boost::asio::socket_base::broadcast option(true);

	socket.open(boost::asio::ip::udp::v4());
	socket.set_option(option);

	for (std::string mac : _macAddresses) {

		uint8_t* macArr = getMACBytes(mac);

		uint8_t data[PACKET_SIZE] = { 0 };

		for (int i = 0; i < MAC_BYTES; i++)
			data[i] = 0xFF;

		for (int i = MAC_BYTES; i < PACKET_SIZE; i += MAC_BYTES) {

			for (int j = 0; j < MAC_BYTES; ++j)
				data[i + j] = macArr[j];
		}

		socket.send_to(boost::asio::buffer(data), endpoint);

		std::cout << "Wake up " << mac << "!" << std::endl;

		delete[] macArr;		
	}

	socket.close();
}

uint8_t* WOL::getMACBytes(std::string mac) {

	std::stringstream ss(mac);

	uint8_t* macArr = new uint8_t[MAC_BYTES];

	char colon;

	for (int i = 0; i < MAC_BYTES; i++) {
		int b;

		ss >> std::hex >> b >> colon;

		macArr[i] = b;
	}
	
	return macArr;
}

