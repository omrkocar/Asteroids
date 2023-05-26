#include <Catch2/catch.hpp>
#include <Core/Vector.h>

TEST_CASE("Vector2")
{
	SECTION("Add")
	{
		Vector2 posOne = Vector2(5.0f, 5.0f);
		Vector2 posTwo = Vector2(5.0f, 6.0f);

		CHECK(posOne + posTwo == Vector2(10.0f, 11.0f));
	}

	SECTION("Subtract")
	{
		Vector2 posOne = Vector2(5.0f, 5.0f);
		Vector2 posTwo = Vector2(5.0f, 6.0f);

		CHECK(posOne - posTwo == Vector2(0.0f, -1.0f));
	}
}
