#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <conio.h>

namespace TextGame
{
	RoomData CreateRoom(const std::string& inName, const std::string& inDescription)
	{
		RoomData room = {};
		room.Name = inName;
		room.Description = inDescription;

		return room;
	}

	void InitializeGame(PlayerState& playerState, WorldState& worldState)
	{
		printf("Welcome to...\n");
		printf("FIRE.\n\n");
		printf("Your objective: navigate rooms and unlock doors with keys to find the flint and drywood to keep you warm. The Paleolithic Era didn't have heaters.\n\n");

		playerState.WantsDescription = true;
		playerState.CurrentRoomIndex = 0;
		playerState.CurrentPosition.x = 3;
		playerState.CurrentPosition.y = 2;
		playerState.DesiredPosition = playerState.CurrentPosition;
		worldState.Rooms.clear();

		{
			RoomData room = CreateRoom("Family Bedroom", "A massive cave bear once lived in here. Emphasis on lived, he helped survive a harsh winter with meat and fur.");
			room.RoomMapWidth = 8;
			room.RoomPosition = { 0,0 };
			room.RoomMap =
				"###..###"
				"#......#"
				"#......#"
				"#......."
				".......#"
				"########";
			room.Inventory.push_back({ ItemType_Key, {1, 1} });
			room.LockedDoors.push_back({ {7,3} });
			worldState.Rooms.push_back(room);
		}

		{
			RoomData room = CreateRoom("Guest Room", "There aren't too many guests. The last time a fire was lit was around here...");
			room.RoomMapWidth = 8;
			room.RoomPosition = { 1,0 };
			room.RoomMap =
				"###..###"
				"#......#"
				"#......#"
				".......#"
				"#......#"
				"########";
			worldState.Rooms.push_back(room);
		}

		{
			RoomData room = CreateRoom("Man Cave", "Fun fact: Cavemen are incredible pool players. You bitterly remember losing to neighboring cave-dwellers.");
			room.RoomMapWidth = 8;
			room.RoomPosition = { -1,0 };
			room.RoomMap =
				"###..###"
				"#......#"
				"#......#"
				"#..##..#"
				"#......#"
				"########";
			worldState.Rooms.push_back(room);

			{
				RoomData room = CreateRoom("Dining Room ", "Unsurprisingly barren save for the scattered bones of various animals. Every footstep in the darkness is guaranteed to be extremely loud.");
				room.RoomMapWidth = 8;
				room.RoomPosition = { 1,-1 };
				room.RoomMap =
					"########"
					"....#..#"
					"#......#"
					"#..##..#"
					"#......#"
					"###..###";
				worldState.Rooms.push_back(room);
			}

			{
				RoomData room = CreateRoom("Family Room", "The rest of your clan are huddled together here, though it seems like your son Ooga is holding a familiar-looking object...");
				room.RoomMapWidth = 8;
				room.RoomPosition = { -1,-1 };
				room.RoomMap =
					"########"
					"#...#..#"
					"#......#"
					"#..##..."
					"#....#.#"
					"###..###";
				room.Inventory.push_back({ ItemType_Flint, {6, 1} });
				worldState.Rooms.push_back(room);
			}

			{
				RoomData room = CreateRoom("Entrance", "The mouth of the cave that is the barrier between the outside elements and safety. Through countless entries and exits it seems like a bunch of dry branches wound up on the floor.");
				room.RoomMapWidth = 8;
				room.RoomPosition = { 0,-1 };
				room.RoomMap =
					"###..###"
					"#......."
					"#......#"
					"...##..#"
					"#......#"
					"########";
				room.Inventory.push_back({ ItemType_Drywood, {3, 2} });
				worldState.Rooms.push_back(room);
			}
		}
	}

	void GetInput(PlayerState& playerState, const WorldState& worldState)
	{
		playerState.WantsToExit = false;
		playerState.WantsDescription = false;
		playerState.WantsInventoryListed = false;
		playerState.WantsInventoryListed = false;
		playerState.DesiredPosition = playerState.CurrentPosition;
		playerState.WantsToGet = false;
		playerState.WantsToWin;

		printf("What do you do?\n");
		printf("> ");
		TextAdventureCommand command = {};
		command.Verb = "";
		while (command.Verb == "")
		{
			char key = _getch();
			if (key == 'A' || key == 'a') command.Verb = "w";
			else if (key == 'W' || key == 'w') command.Verb = "n";
			else if (key == 'S' || key == 's') command.Verb = "s";
			else if (key == 'D' || key == 'd') command.Verb = "e";
			else if (key == 'C' || key == 'c') command.Verb = "commandMode";
		}
		
		if (command.Verb == "commandMode") {
			printf("COMMAND MODE: ");
			command = ParseAdventureCommand();
		}

		if (command.Verb == "quit")
		{
			playerState.WantsToExit = true;
		}
		else if (command.Verb == "north" || command.Verb == "n")
		{
			playerState.DesiredPosition.y = playerState.CurrentPosition.y - 1;
		}
		else if (command.Verb == "south" || command.Verb == "s")
		{
			playerState.DesiredPosition.y = playerState.CurrentPosition.y + 1;
		}
		else if (command.Verb == "west" || command.Verb == "w")
		{
			playerState.DesiredPosition.x = playerState.CurrentPosition.x - 1;
		}
		else if (command.Verb == "east" || command.Verb == "e")
		{
			playerState.DesiredPosition.x = playerState.CurrentPosition.x + 1;
		}
		else if (command.Verb == "look")
		{
			playerState.WantsDescription = true;
		}
		else if (command.Verb == "inventory" || command.Verb == "inv")
		{
			playerState.WantsInventoryListed = true;
		}
		else if (command.Verb == "get")
		{
			playerState.WantsToGet = true;
		}
		else if (command.Verb == "win")
		{
			playerState.WantsToWin = true;
		}
		else if (command.Verb == "help")
		{
			printf("Command List: look, quit, inv/inventory, get, north, south, west, east\n");
			printf("Key:\n");
			printf("  @ - Player\n");
			printf("  d - Drywood\n");
			printf("  f - Flint\n");
			printf("  k - Key\n");
			printf("  . - Floor\n");
			printf("  # - Wall\n");
			printf("  D - Door (Locked)\n");
		}
		else
		{
			printf("I don't understand\n");
		}
		printf("\n");

	}

	void RenderGame(const PlayerState& playerState, const WorldState& worldState)
	{
		if (playerState.WantsDescription)
		{
			const RoomData& currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
			printf("================================================\n");
			printf("LOCATION: %s\n", currRoom.Name.c_str());
			printf("%s\n\n", currRoom.Description.c_str());

			std::string renderedMapString = "";
			unsigned int currentSpace = 0;

			while (currentSpace < currRoom.RoomMap.size())
			{
				char charToDisplay = currRoom.RoomMap[currentSpace];

				for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
				{
					const LockedDoorData& currDoor = currRoom.LockedDoors[i];
					if (PositionToIndex(currDoor.DoorPosition, currRoom.RoomMapWidth) == currentSpace)
					{
						charToDisplay = 'D';
					}
				}

				for (unsigned int i = 0; i < currRoom.Inventory.size(); ++i)
				{
					const InventoryItem& currItem = currRoom.Inventory[i];
					if (PositionToIndex(currItem.ItemPosition, currRoom.RoomMapWidth) == currentSpace)
					{
						charToDisplay = GetItemIcon(currItem.Type);
					}
				}

				if (PositionToIndex(playerState.CurrentPosition, currRoom.RoomMapWidth) == currentSpace)
				{
					charToDisplay = '@';
				}

				renderedMapString += charToDisplay;
				if (currentSpace % currRoom.RoomMapWidth == (currRoom.RoomMapWidth - 1)) {
					renderedMapString += '\n';
				}
				currentSpace++;
			}

			printf("%s\n", renderedMapString.c_str());

			printf("\n");
		}
		else if (playerState.WantsInventoryListed)
		{
			printf("================================================\n");
			printf("INVENTORY:\n");
			if (playerState.Inventory.size() == 0)
			{
				printf("You have nothing inside...\n");
			}
			for (unsigned int i = 0; i < playerState.Inventory.size(); ++i)
			{
				printf("%s\n", GetItemName(playerState.Inventory[i].Type).c_str());
			}
			printf("\n");
		}
	}

	void UpdateGame(PlayerState& playerState, WorldState& worldState)
	{
		RoomData& currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
		if (playerState.DesiredPosition != playerState.CurrentPosition)
		{
			TryToUnlockDoor(playerState.DesiredPosition, currRoom, playerState);
			if (IsSpaceOutside(playerState.DesiredPosition, currRoom))
			{
				Position desiredRoomPosition = currRoom.RoomPosition;
				int playerPositionType = 0;
				if (playerState.DesiredPosition.x < 0)
				{
					desiredRoomPosition.x--;
					playerPositionType = 1;
				}
				else if (playerState.DesiredPosition.x >= currRoom.RoomMapWidth)
				{
					desiredRoomPosition.x++;
					playerPositionType = 2;
				}
				else if (playerState.DesiredPosition.y < 0)
				{
					desiredRoomPosition.y--;
					playerPositionType = 3;
				}
				else if (playerState.DesiredPosition.y >= ((int)currRoom.RoomMap.size() / currRoom.RoomMapWidth))
				{
					desiredRoomPosition.y++;
					playerPositionType = 4;
				}
				bool foundNewRoom = false;
				for (unsigned int i = 0; i < worldState.Rooms.size(); ++i)
				{
					if (worldState.Rooms[i].RoomPosition == desiredRoomPosition)
					{
						playerState.CurrentRoomIndex = i;
						playerState.WantsDescription = true;
						switch (playerPositionType)
						{
						case 1:
							playerState.CurrentPosition.x = worldState.Rooms[playerState.CurrentRoomIndex].RoomMapWidth - 1;
							break;
						case 2:
							playerState.CurrentPosition.x = 0;
							break;
						case 3:
							playerState.CurrentPosition.y = (worldState.Rooms[playerState.CurrentRoomIndex].RoomMap.size() / worldState.Rooms[playerState.CurrentRoomIndex].RoomMapWidth) - 1;
							break;
						case 4:
							playerState.CurrentPosition.y = 0;
							break;
						}
						foundNewRoom = true;
					}
				}
				if (!foundNewRoom)
				{
					printf("Nothing out there!\n");
				}
			}
			else if (IsSpaceOpen(playerState.DesiredPosition, currRoom))
			{

				playerState.WantsDescription = true;
				playerState.CurrentPosition = playerState.DesiredPosition;
			}
			else
			{
				printf("Watch out for walls!\n");
			}
		}
		else if (playerState.WantsToGet)
		{
			bool foundItem = false;
			for (unsigned int i = 0; i < currRoom.Inventory.size(); ++i)
			{
				const InventoryItem& currItem = currRoom.Inventory[i];
				if (currItem.ItemPosition == playerState.CurrentPosition)
				{
					printf("I got %s!\n", GetItemName(currItem.Type).c_str());
					playerState.Inventory.push_back(currItem);
					currRoom.Inventory.erase(currRoom.Inventory.begin() + i);
					foundItem = true;
					break;
				}
			}
			if (!foundItem)
			{
				printf("There was nothing to get...");
			}
		}
		else if (playerState.WantsToWin)
		{
			int numberofflint = 0;
			int numberofdrywood = 0;
			for (unsigned int i = 0; i < playerState.Inventory.size(); ++i)
			{
				const InventoryItem& currItem = playerState.Inventory[i];
				if (currItem.Type == ItemType_Drywood)
				{
					numberofdrywood++;
				}
				else if (currItem.Type == ItemType_Flint)
				{
					numberofflint++;
				}
			}

			if (numberofdrywood >= 1 && numberofflint >= 1)
			{
				printf("Awesome! You have found the necessary materials to start a fire! Your family is now happy and warm.\n");
				playerState.WantsToExit = true;
			}
			else
			{
				printf("You cannot light a fire yet as you are missing one or more necessary materials.\n");
			}
		}
	}

	void CleanupGame(PlayerState& playerState, WorldState& worldState)
	{
		worldState.Rooms.clear();
	}

	int PositionToIndex(const Position& position, int roomWidth)
	{
		return position.y * roomWidth + position.x;
	}

	bool IsSpaceOpen(const Position& position, const RoomData& currRoom)
	{
		for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
		{
			const LockedDoorData& currDoor = currRoom.LockedDoors[i];
			if (currDoor.DoorPosition == position)
			{
				return false;
			}
		}
		int spaceIndex = PositionToIndex(position, currRoom.RoomMapWidth);
		return currRoom.RoomMap[spaceIndex] == '.';
	}

	bool IsSpaceOutside(const Position& position, const RoomData& currRoom)
	{
		return position.x < 0 ||
			position.x >= currRoom.RoomMapWidth ||
			position.y < 0 ||
			position.y >= ((int)currRoom.RoomMap.size() / currRoom.RoomMapWidth);
	}

	std::string GetItemName(ItemType itemtype)
	{
		switch (itemtype)
		{
		case ItemType_Key:
			return "Key";
			break;

		case ItemType_Flint:
			return "Flint";
			break;

		case ItemType_Drywood:
			return "Drywood";
			break;
		}
		return "UKNOWN ITEM";
	}

	char GetItemIcon(ItemType itemtype)
	{
		switch (itemtype)
		{
		case TextGame::ItemType_Key:
			return 'k';
			break;

		case TextGame::ItemType_Flint:
			return 'f';
			break;
		case TextGame::ItemType_Drywood:
			return 'd';
			break;
		}
		return 'i';
	}

	void TryToUnlockDoor(const Position& position, RoomData& currRoom, PlayerState& playerState)
	{
		for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
		{
			const LockedDoorData& currDoor = currRoom.LockedDoors[i];
			if (currDoor.DoorPosition == position)
			{
				for (unsigned int j = 0; j < playerState.Inventory.size(); ++j)
				{
					const InventoryItem& currItem = playerState.Inventory[j];
					if (currItem.Type == ItemType_Key)
					{
						currRoom.LockedDoors.erase(currRoom.LockedDoors.begin() + i);
						playerState.Inventory.erase(playerState.Inventory.begin() + j);
						return;
					}
				}
			}
		
		}
	}
}
