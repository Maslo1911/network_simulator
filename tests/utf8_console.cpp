#include <gtest/gtest.h>
#ifdef _WIN32
#include <windows.h>
#endif

namespace {
class Utf8Env : public ::testing::Environment {
public:
	void SetUp() override {
#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
#endif
	}
};
static ::testing::Environment* const utf8_env = ::testing::AddGlobalTestEnvironment(new Utf8Env());
}
