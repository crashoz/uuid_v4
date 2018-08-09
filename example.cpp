#include <iostream>
#include <random>
#include <chrono>

#include "uuid_v4best.h"

#define IT 100000000

int main(int argc, char* argv[]) {
  UUIDGenerator<std::mt19937_64> uuidGenerator(1);

  auto t1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < IT; i++) {
    UUID uuid = uuidGenerator.getUUID();
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto diff = t2-t1;
  std::cout << "gen: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  UUID uuid = uuidGenerator.getUUID();
  for (int i = 0; i < IT; i++) {
    std::string str = uuid.bytes();
  }
  t2 = std::chrono::high_resolution_clock::now();
  diff = t2-t1;
  std::cout << "bytes: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  UUID uuid1 = uuidGenerator.getUUID();
  for (int i = 0; i < IT; i++) {
    std::string str = uuid1.str();
  }
  t2 = std::chrono::high_resolution_clock::now();
  diff = t2-t1;
  std::cout << "str: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  UUID uuid2 = uuidGenerator.getUUID();
  std::string bytes = uuid2.bytes();
  for (int i = 0; i < IT; i++) {
    UUID x(bytes);
    if (x.bytes().empty()) {
      printf("error");
    }
  }
  t2 = std::chrono::high_resolution_clock::now();
  diff = t2-t1;
  std::cout << "parse: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  UUID uuid3 = uuidGenerator.getUUID();
  std::string str = uuid2.str();
  for (int i = 0; i < IT; i++) {
    UUID x = UUID::fromStr(str);
  }
  t2 = std::chrono::high_resolution_clock::now();
  diff = t2-t1;
  std::cout << "fromStr: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << std::endl;

  std::cout << uuid2.str() << std::endl;

  UUID test(uuid2.bytes());
  std::cout << test << std::endl;

  UUID test1 = UUID::fromStr(test.str());
  std::cout << test1 << std::endl;

  return 0;
}
