#pragma once

#include "Definitions.h"
#include "StdFix.h"
#include "GlobalConsole.h"
#include "Guid.h"

#define M_INFO MAGE::ConsoleType::Info
#define M_VERBOSE MAGE::ConsoleType::Verbose
#define M_WARNING MAGE::ConsoleType::Warning
#define M_ERROR MAGE::ConsoleType::Error

#define CORE_LOG(level, message, ...) MAGE::GlobalConsole::Log(level, message, ##__VA_ARGS__)
#define CORE_PRINT(message, ...) MAGE::GlobalConsole::Print(message, ##__VA_ARGS__)
#define CORE_ASSERT(condition, title, message, ...) MAGE::GlobalConsole::Assert(condition, title, message, ##__VA_ARGS__)