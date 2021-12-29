#include <random>
#include <benchmark/benchmark.h>
#include "uuid_v4/uuid_v4.h"

#define SCALE 100
#define LOOP_START for(int i=0;i<SCALE;i++) {
#define LOOP_END }

#define GENERATOR std::mt19937_64

static void UUIDGeneration(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  for (auto _ : state) {
    LOOP_START
    UUID::UUID uuid = uuidGenerator.getUUID();
    ((void)uuid); // Prevents unused warning
    LOOP_END
  }
}
BENCHMARK(UUIDGeneration);

static void UUIDSerializeAlloc(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  for (auto _ : state) {
    LOOP_START
    std::string bytes = uuid.bytes();
    LOOP_END
  }
}
BENCHMARK(UUIDSerializeAlloc);

static void UUIDSerializeByRef(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  std::string bytes;
  for (auto _ : state) {
    LOOP_START
    uuid.bytes(bytes);
    LOOP_END
  }
}
BENCHMARK(UUIDSerializeByRef);

static void UUIDSerializeCharArray(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  char bytes[16];
  for (auto _ : state) {
    LOOP_START
    uuid.bytes(bytes);
    LOOP_END
  }
}
BENCHMARK(UUIDSerializeCharArray);

static void UUIDPretty(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  std::string pretty;
  for (auto _ : state) {
    LOOP_START
    pretty = uuid.str();
    LOOP_END
  }
}
BENCHMARK(UUIDPretty);

static void UUIDPrettyByRef(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  std::string pretty;
  pretty.resize(36);
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(pretty.data());
    uuid.str(pretty);
    benchmark::ClobberMemory();
    LOOP_END
  }
}
BENCHMARK(UUIDPrettyByRef);

static void UUIDPrettyCharArray(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  char* pretty = new char[36];
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(pretty);
    uuid.str(pretty);
    benchmark::ClobberMemory();
    LOOP_END
  }
  delete pretty;
}
BENCHMARK(UUIDPrettyCharArray);

static void UUIDLoad(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  std::string bytes = uuid.bytes();
  UUID::UUID uuidNew;
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(uuidNew = UUID::UUID(bytes));
    LOOP_END
  }
}
BENCHMARK(UUIDLoad);

static void UUIDParse(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  std::string pretty = uuid.str();
  const char *raw = pretty.c_str();
  UUID::UUID uuidNew;
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(UUID::UUID::fromStrFactory(raw));
    LOOP_END
  }
}
BENCHMARK(UUIDParse);

static void UUIDParseInPlace(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  std::string pretty = uuid.str();
  const char *raw = pretty.c_str();
  UUID::UUID uuidNew;
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(uuidNew);
    uuidNew.fromStr(raw);
    benchmark::ClobberMemory();
    LOOP_END
  }
}
BENCHMARK(UUIDParseInPlace);


static void UUIDEqual(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  UUID::UUID uuid1 = uuidGenerator.getUUID();
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(uuid == uuid1);
    LOOP_END
  }
}
BENCHMARK(UUIDEqual);

static void UUIDCompare(benchmark::State& state) {
  UUID::UUIDGenerator<GENERATOR> uuidGenerator;
  UUID::UUID uuid = uuidGenerator.getUUID();
  UUID::UUID uuid1 = uuidGenerator.getUUID();
  for (auto _ : state) {
    LOOP_START
    benchmark::DoNotOptimize(uuid < uuid1);
    LOOP_END
  }
}
BENCHMARK(UUIDCompare);

BENCHMARK_MAIN();
