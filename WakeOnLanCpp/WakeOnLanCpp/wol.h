#pragma once

#pragma warning(disable : 4996)


#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <boost/asio.hpp>
#include <boost/regex.hpp>

#define JSON_FILE "mac_addresses.json"
#define JSON_PROPERTY "mac_addresses"

#define PORT 9
#define MAC_BYTES 6
#define PACKET_SIZE 102

#define MAC_PATTERN "^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$"

class WOL {
public:
	WOL();
	~WOL();
	void setup();
	void sendMagicPackets();

private:

	uint8_t* getMACBytes(std::string mac);
	std::vector<std::string> _macAddresses;

};

