#include "pch.h"
#include "test.h"

#include "engine/entity/component.h"
#include "engine/entity/entity.h"

TEST(ComponentTest, ComponentBasics)
{
	using namespace raid;

	class TestComponent : public raid::Component
	{
	public:

		TestComponent(Entity& parent, int state)
			: Component(parent)
			, State(state)
		{
		}

		int State;
	};

	raid::Entity e;
	const TestComponent& c1 = e.AddComponent<TestComponent>(10);
	ASSERT_EQ(c1.State, 10);

	TestComponent* c2 = e.GetComponent<TestComponent>();
	ASSERT_NE(c2, nullptr);
	ASSERT_EQ(c2->State, 10);
}