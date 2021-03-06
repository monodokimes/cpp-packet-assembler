// packet-assembler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Packet.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::string filename = argv[1];
	std::ifstream inputFile(filename);

	if (!inputFile.is_open())
	{
		std::cout << "Could not open the file" << std::endl;
		return 1;
	}

	std::string line;
	std::vector<int> messageIds;
	std::vector<Packet> packets;

	while (std::getline(inputFile, line))
	{
		Packet packet(line);
		packets.push_back(packet);

		int messageId = packet.getMessageId();
		bool newMessage = true;
		for (int i = 0; i < messageIds.size(); i++)
		{
			if (messageId == messageIds[i])
			{
				newMessage = false;
				break;
			}
		}

		if (newMessage)
		{
			messageIds.push_back(messageId);
		}
	}

	for (size_t i = 0; i < messageIds.size(); i++)
	{
		std::vector<Packet> messagePackets;
		for (size_t j = 0; j < packets.size(); j++)
		{
			Packet packet = packets[j];
			if (packet.getMessageId() == messageIds[i])
			{
				messagePackets.push_back(packet);
				if (messagePackets.size() == packet.getMessageLength())
					break;
			}
		}

		sort(messagePackets.begin(), messagePackets.end(), [](Packet& p1, Packet& p2) {return p1.getId() < p2.getId(); });

		std::cout << "MESSAGE START" << std::endl;
		for (size_t i = 0; i < messagePackets.size(); i++)
		{
			std::cout << messagePackets[i].getContent() << std::endl;
		}
	}

	std::cout << std::endl;
	return 0;
}

