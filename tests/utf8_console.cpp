#include <gtest/gtest.h>
#include <locale>

namespace {
class Utf8Env : public ::testing::Environment {
public:
	void SetUp() override {
        if (std::setlocale(LC_ALL, "ru_RU.UTF-8") == nullptr) {
            std::setlocale(LC_ALL, "");
        }
        try { std::locale::global(std::locale("ru_RU.UTF-8")); }
        catch (...) {}
	}
};
static ::testing::Environment* const utf8_env = ::testing::AddGlobalTestEnvironment(new Utf8Env());
}
