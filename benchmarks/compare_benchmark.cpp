#include <benchmark/benchmark.h>
#include <random>
#include "uuid_v4.h"
#include "basic.h"
#include <uuid/uuid.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


#define SCALE 100
#define LOOP_START for(int i=0;i<SCALE;i++) {
#define LOOP_END }

static void Basic(benchmark::State& state) {
  std::string uuid;
  for (auto _ : state) {
    LOOP_START
    uuid = MathUtils::generateUUID();
    LOOP_END
  }
}
BENCHMARK(Basic);

static void Libuuid(benchmark::State& state) {
  char str[37] = {0};
  uuid_t uuid;
  for (auto _ : state) {
    LOOP_START
    uuid_generate_random(uuid);
    //uuid_unparse(uuid, str);
    LOOP_END
  }
}
BENCHMARK(Libuuid);

static void BoostUUID(benchmark::State& state) {
  boost::uuids::uuid uuid;
  std::string str;
  for (auto _ : state) {
    LOOP_START
    uuid = boost::uuids::random_generator()();
    //str = to_string(uuid);
    LOOP_END
  }
}
BENCHMARK(BoostUUID);

static void BoostUUIDmt19937(benchmark::State& state) {
  boost::uuids::uuid uuid;
  boost::uuids::random_generator_mt19937 gen;

  std::string str;
  for (auto _ : state) {
    LOOP_START
    uuid = gen();
    //str = to_string(uuid);
    LOOP_END
  }
}
BENCHMARK(BoostUUIDmt19937);

static void UUID_v4(benchmark::State& state) {
  UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
  UUIDv4::UUID uuid;
  for (auto _ : state) {
    LOOP_START
    uuid = uuidGenerator.getUUID();
    LOOP_END
  }
}
BENCHMARK(UUID_v4);

BENCHMARK_MAIN();
