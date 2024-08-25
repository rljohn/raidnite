#include "engine/pch.h"
#include "map.h"

#include "engine/system/log/logging.h"
#include "engine/game/game_events.h"
#include "engine/entity/transform.h"

namespace raid
{

void Map::BuildMap(const PositionScalar width, const PositionScalar height)
{
	m_Tiles.resize(width, std::vector<Tile>(height));

	for (PositionScalar i = 0; i < width; i++)
	{
		for (PositionScalar j = 0; j < height; j++)
		{
			m_Tiles[i][j].SetPosition(Position(i, j));
		}
	}

	m_Width = width;
	m_Height = height;
}

void Map::RegisterForEvents()
{
	Game::RegisterGameEventListener(this);
}

void Map::Shutdown()
{
	Game::UnregisterGameEventListener(this);

	m_Width = 0;
	m_Height = 0;
	m_Tiles.clear();
}

void Map::OnGameEvent(const GameEvent& evt)
{
	if (evt.GetType() == GameEventType::EntityPositionChanged)
	{
		const EntityPositionChangedEvent& upcEvent = static_cast<const EntityPositionChangedEvent&>(evt);
		OnEntityPositionChanged(upcEvent.GetEntity(), upcEvent.m_Previous, upcEvent.m_Position);
	}
}

PositionScalar Map::GetWidth() const
{
	return m_Width;
}

PositionScalar Map::GetHeight() const
{
	return m_Height;
}

bool Map::IsPositionValid(const PositionScalar x, const PositionScalar y) const
{
	if (x < 0 || x >= GetWidth())
	{
		return false;
	}

	if (y < 0 || y >= GetHeight())
	{
		return false;
	}

	return true;
}

bool Map::IsPositionValid(const Position& pos) const
{
	return IsPositionValid(pos.GetX(), pos.GetY());
}

Tile* Map::GetTile(const PositionScalar x, const PositionScalar y)
{
	if (!IsPositionValid(x, y))
	{
		mapError("Invalid tile: {},{}", x, y);
		return nullptr;
	}

	return &m_Tiles[x][y];
}

Tile* Map::GetTile(const Position& position)
{
	return GetTile(position.GetX(), position.GetY());
}

const Tile* Map::GetTile(const PositionScalar x, const PositionScalar y) const
{
	if (!IsPositionValid(x, y))
	{
		mapError("Invalid tile: {},{}", x, y);
		return nullptr;
	}

	return &m_Tiles[x][y];
}

const Tile* Map::GetTile(const Position& position) const
{
	return GetTile(position.GetX(), position.GetY());
}

bool Map::HasTile(const Position& position) const
{
	if (!IsPositionValid(position.GetX(), position.GetY()))
	{
		return false;
	}

	return GetTile(position)->IsValid();
}

bool Map::IsTileEnabled(const Position& position) const
{
	if (const Tile* tile = GetTile(position))
	{
		return tile->IsValid();
	}
}

void Map::SetTileEnabled(const Position& position, bool enabled)
{
	if (Tile* tile = GetTile(position))
	{
		if (tile->IsValid() != enabled)
		{
			tile->SetValid(enabled);

			TilePropertiesChangedEvent evt(tile);
			Game::DispatchGameEvent(evt);
		}
	}
}

bool Map::CanOccupy(const Position& position) const
{
	const Tile* tile = GetTile(position);
	return tile ? (tile->AllowsOccupancy() && !tile->IsOccupied()) : false;
}

bool Map::IsOccupied(const Position& position) const
{
	const Tile* tile = GetTile(position);
	return tile ? tile->IsOccupied() : false;
}

bool Map::IsMovementAllowed(const Position& position) const
{
	const Tile* tile = GetTile(position);
	return tile && tile->AllowsMovement();
}

bool Map::BuildPath(Tile* start, Tile* end, TilePath& out_path)
{
	if (start == nullptr || end == nullptr)
	{
		out_path.Reset();
		return false;
	}

	const Position& startPos = start->GetPosition();
	const Position& endPos = end->GetPosition();

	return BuildPath(startPos, endPos, out_path);
}

bool Map::BuildPath(const Position& start, const Position& end, TilePath& out_path)
{
	out_path.Reset();

	// That tile isn't available, find another one
	if (!CanOccupy(end))
	{
		out_path.Reset();
		return false;
	}

	// Build a path
	std::unordered_map<Position, Position> came_from;
	std::unordered_map<Position, double> cost_so_far;
	Pathfinding::AStarSearch(this, start, end, came_from, cost_so_far);
	Pathfinding::ReconstructPath(this, start, end, came_from, out_path);
	return out_path.m_Tiles.size() > 0;
}

std::vector<Position> Map::GetNeighbors(const Position& pos) const
{
	auto IsPassable = [this](const Position& p)
	{
		return HasTile(p) && IsMovementAllowed(p);
	};

	static std::array<Position, 4> DIRS =
	{
		/* East, West, North, South */
		Position(1, 0),
		Position(-1, 0),
		Position(0, -1),
		Position(0, 1),
	};

	std::vector<Position> results;
	results.reserve(8);

	for (const Position& dir : DIRS)
	{
		Position next(pos.GetX() + dir.GetX(), pos.GetY() + dir.GetY());
		if (IsPositionValid(next.GetX(), next.GetY()) && IsPassable(next))
		{
			results.push_back(next);
		}
	}

	static bool AllowDiagonalMotion = true;
	if (AllowDiagonalMotion)
	{
		struct DiagonalDirection
		{
			DiagonalDirection(Position p, Position r1, Position r2)
				: pos(p), req1(r1), req2(r2) {}

			Position pos;
			Position req1;
			Position req2;
		};

		static std::array<DiagonalDirection, 4> DIAGS =
		{
			/* Diagnonals */
			DiagonalDirection(Position(1, 1), Position(1, 0), Position(0, 1)),
			DiagonalDirection(Position(-1, -1), Position(-1, 0), Position(0, -1)),
			DiagonalDirection(Position(-1, 1), Position(-1, 0), Position(0, 1)),
			DiagonalDirection(Position(1, -1), Position(1, 0), Position(0, -1)),
		};

		for (const DiagonalDirection& dir : DIAGS)
		{
			Position next( pos.GetX() + dir.pos.GetX(), pos.GetY() + dir.pos.GetY() );
			Position req1( pos.GetX() + dir.req1.GetX(), pos.GetY() + dir.req1.GetY() );
			Position req2( pos.GetX() + dir.req2.GetX(), pos.GetY() + dir.req2.GetY() );

			if (IsPositionValid(next.GetX(), next.GetY()) && IsPassable(next) && IsPassable(req1) && IsPassable(req2))
			{
				results.push_back(next);
			}
		}
	}

	return results;
}

double Map::GetCost(const Position& from, const Position& to)
{
	// TODO
	// Add hazard modifiers

	if (from.GetX() == to.GetX() || from.GetY() == to.GetY())
	{
		// Horiziontal
		return 1;
	}
	else
	{
		// Double
		return 1.001;
	}
}

void Map::SetMovementAllowed(const Position& position, bool allow)
{
	if (Tile* tile = GetTile(position))
	{
		if (tile->AllowsMovement() != allow)
		{
			tile->SetMovementEnabled(allow);

			TilePropertiesChangedEvent evt(tile);
			Game::DispatchGameEvent(evt);
		}
	}
}

void Map::SetOccupancyAllowed(const Position& position, bool allow)
{
	if (Tile* tile = GetTile(position))
	{
		if (tile->AllowsOccupancy() != allow)
		{
			tile->SetOccupancyAllowed(allow);

			TilePropertiesChangedEvent evt(tile);
			Game::DispatchGameEvent(evt);
		}
	}
}

bool Map::GetNearestUnoccupiedTile(const Position& target, Position& out_result)
{
	return GetNearestUnoccupiedTile(target, target, out_result);
}

bool Map::GetNearestUnoccupiedTile(const Position& target, const Position& from, Position& out_result)
{
	Position tmp;
	if (Pathfinding::FindClosestTile(this, from, target, tmp))
	{
		out_result = tmp;
		return true;
	}
	else
	{
		return false;
	}
}

void Map::OnEntityPositionChanged(Entity* entity, const Position& from, const Position& to)
{
	if (!entity)
		return;

	if (Tile* prev = GetTile(from))
	{
		prev->OnEntityExit(entity);
	}

	if (Tile* next = GetTile(to))
	{
		next->OnEntityEnter(entity);
	}
}

void Map::SetTileOccupation(const Position& pos, Entity* entity, TransformComponent& transform)
{
	const Position& previous = transform.GetOccupyingTile();
	if (IsPositionValid(previous))
	{
		if (Tile* prevTile = GetTile(previous))
		{
			prevTile->SetOccupant(nullptr);

			EntityOccupancyChangedEvent e(nullptr, previous);
			Game::DispatchGameEvent(e);
		}
	}

	if (Tile* t = GetTile(pos))
	{
		transform.SetOccupyingTile(pos);
		t->SetOccupant(entity);

		EntityOccupancyChangedEvent e(entity, pos);
		Game::DispatchGameEvent(e);
	}
}

} // namespace raid

