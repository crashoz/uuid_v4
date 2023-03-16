#include <iostream>
#include <random>
#include <chrono>

#include "uuid_v4.h"

#define IT 1000000000

void debugUUID(const UUIDv4::UUID &uuid) {
  std::string bytes = uuid.bytes();
  for (int i=0; i<16; i++) {
    printf("%02hhx", bytes[i]);
  }
  printf("\n");
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
  char txt[37];
  auto t1 = std::chrono::high_resolution_clock::now();
  for (int i=0; i<IT; i++) {
    UUIDv4::UUID test = uuidGenerator.getUUID();
    test.str(txt);
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto diff = t2-t1;
  std::cout << "ops/sec: " << IT / (std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count() / 1e9) << std::endl;

  UUIDv4::UUID load;
  std::cin >> load;
  std::cout << load << std::endl;

  return 0;
}
