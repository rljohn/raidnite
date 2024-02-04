#pragma once

#include "engine/entity/component.h"
#include "engine/game/game.h"
#include "engine/system/time.h"

namespace raid
{
	///////////////////////////
	// Buff Effects
	///////////////////////////

	struct IBuffEffect
	{
		virtual void OnAdd(Entity& /* owner */) {}
		virtual void OnUpdate(GameFrame& frame) {}
		virtual void OnRemove() {}
	};

	///////////////////////////
	// Buffs
	///////////////////////////

	class Buff
	{
	public:

		void Begin(const TimeStamp& startTime = Time::GetCurrent());

		void AddEffect(IBuffEffect* effect);
		void SetDuration(const Duration& duration);

		bool IsExpired() const;

		void OnAdd(Entity& owner);
		void OnUpdate(GameFrame& frame);

		enum RemoveReason
		{
			Invalid,
			Expired,
			Cleansed,
			Removed
		};

		void OnRemove(RemoveReason reason);

	private:

		TimeStamp m_StartTime;
		Duration m_Duration;
		Duration m_TimeRemaining;
		std::vector<IBuffEffect*> m_Effects;
	};

	///////////////////////////
	// Buff Component
	///////////////////////////

	class BuffsComponent : public Component
	{
	public:

		BuffsComponent(Entity& parent)
			: Component(parent)
		{
		}

		void Update(GameFrame& frame);

		void AddBuff(Buff* buff);

		void CleanseBuff(Buff* buff);
		void RemoveBuff(Buff* buff);
		void ExpireBuff(Buff* buff);

	private:

		std::vector<Buff*> m_Buffs;
	};
}
