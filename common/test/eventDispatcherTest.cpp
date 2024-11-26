#include "common/eventDispatcher.hpp"
#include "gtest/gtest.h"

namespace Common::Test {

struct EventA {
  unsigned id;
};

struct EventB {
  std::vector<int> numbers;
};

TEST(EventDispatcherTest, RegisterAndDispatch) {
  int counter{0};
  auto callA = [&counter](const EventA&) { ++counter; };

  std::vector<int> result;
  auto storeB = [&result](const EventB& e) { result = e.numbers; };

  EventDispatcher eventDispatcher;
  eventDispatcher.RegisterHandler<EventA>(callA);
  eventDispatcher.RegisterHandler<EventA>(callA);
  eventDispatcher.RegisterHandler<EventB>(storeB);

  EventB testEventB1{std::vector<int>{1, 2, 3}};
  eventDispatcher.Dispatch(testEventB1);
  EXPECT_EQ(testEventB1.numbers, result);

  EventB testEventB2{std::vector<int>{2, 3, 4}};
  eventDispatcher.Dispatch(testEventB2);
  EXPECT_EQ(testEventB2.numbers, result);

  EventA testEventA{60};
  eventDispatcher.Dispatch(testEventA);
  EXPECT_EQ(counter, 2);
}

}  // namespace Common::Test
