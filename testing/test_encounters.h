#pragma once

#include "pch.h"
#include "engine/types.h"


class EncounterLogTest : public testing::Test
{
protected:

	std::wstring m_LogPath;

	void SetUp() override;
	void TearDown() override;
};