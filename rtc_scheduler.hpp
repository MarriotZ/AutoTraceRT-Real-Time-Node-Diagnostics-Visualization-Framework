#pragma once
#include <functional>
#include <atomic>
#include <cstdint>

namespace autotracert {

struct RtcTaskCfg {
  int hz = 100;            // 周期频率
  int priority = 80;       // SCHED_FIFO 优先级 (1-99)
  int cpu_core = -1;       // 绑定核，-1 不绑定
  int deadline_ms = 10;    // 软截止
};

class RtcScheduler {
public:
  RtcScheduler() = default;
  ~RtcScheduler();

  // 启动一个周期任务（内部新线程）
  void start(const RtcTaskCfg& cfg, std::function<void()> job);

  // 停止（等待线程退出）
  void stop();

  // 指标
  uint64_t deadline_miss() const { return deadline_miss_.load(); }

private:
  int tfd_ = -1;
  std::atomic<bool> running_{false};
  std::atomic<uint64_t> deadline_miss_{0};
  void loop_(RtcTaskCfg cfg, std::function<void()> job);
};

} // namespace autotracert
