#pragma once
#include <string>
#include <vector>

namespace TextGame
{
	struct Position {
		int x;
		int y;

		bool operator==(const Position& otherPosition) const
		{
			return x == otherPosition.x && y == otherPosition.y;
		}
		bool operator!=(const Position& otherPosition) const
		{
			return !(*this == otherPosition);
		}
	};

	enum ItemType
	{
		ItemType_Key,
		ItemType_Flint,
		ItemType_Drywood
	};

	struct InventoryItem
	{
		ItemType Type;
		Position ItemPosition;
	};

	struct PlayerState
	{
		bool WantsToExit;
		bool WantsDescription;
		bool WantsInventoryListed;
		bool WantsToGet;
		bool WantsToWin;
		
		Position CurrentPosition;
		Position DesiredPosition;
		int CurrentRoomIndex;
		std::vector<InventoryItem> Inventory;
	};

	struct LockedDoorData
	{
		Position DoorPosition;
	};

	struct RoomData
	{
		std::string Name;
		std::string Description;

		int RoomMapWidth;
		std::string RoomMap;
		Position RoomPosition;
		std::vector<InventoryItem> Inventory;
		std::vector<LockedDoorData> LockedDoors;
	};

	struct WorldState
	{
		std::vector<RoomData> Rooms;
	};

	void InitializeGame(PlayerState& playerState, WorldState& worldState);
	void GetInput(PlayerState& playerState, const WorldState& worldState);
	void RenderGame(const PlayerState& playerState, const WorldState& worldState);
	void UpdateGame(PlayerState& playerState, WorldState& worldState);
	void CleanupGame(PlayerState& playerState, WorldState& worldState);

	int PositionToIndex(const Position& position, int roomWidth);
	bool IsSpaceOpen(const Position& position, const RoomData& currRoom);
	bool IsSpaceOutside(const Position& position, const RoomData& currRoom);

	std::string GetItemName(ItemType itemtype);
	char GetItemIcon(ItemType itemtype);
	void TryToUnlockDoor(const Position& position,RoomData& currRoom, PlayerState& playerState);
}
