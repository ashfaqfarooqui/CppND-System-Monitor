#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // Following
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  //
  int prevIdle = 0;
  int prevNonIdle = 0;

  if (oldProcessor == nullptr) {
    prevIdle = 0;
    prevNonIdle = 0;

  } else {
    prevIdle = oldProcessor->idle + oldProcessor->iowait;
    prevNonIdle = oldProcessor->user + oldProcessor->nice +
                  oldProcessor->system + oldProcessor->irq +
                  oldProcessor->softirq + oldProcessor->steal;
  }
  auto idleTime = idle + iowait;

  auto nonIdle = user + nice + system + irq + softirq + steal;

  auto prevTotal = prevIdle + prevNonIdle;
  auto total = idleTime + nonIdle;

  float totald = total - prevTotal;
  float idled = idleTime - prevIdle;

  float cpu = (totald - idled) / (totald + 1.0);
  // PrevIdle = previdle + previowait
  // Idle = idle + iowait

  // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
  // prevsteal NonIdle = user + nice + system + irq + softirq + steal

  // PrevTotal = PrevIdle + PrevNonIdle
  // Total = Idle + NonIdle

  // # differentiate: actual value minus the previous one
  // totald = Total - PrevTotal
  // idled = Idle - PrevIdle

  // CPU_Percentage = (totald - idled)/totald

  return cpu;
}
Processor::Processor()
    : oldProcessor{nullptr},
      user{0},
      nice{0},
      system{0},
      idle{0},
      iowait{0},
      irq{0},
      softirq{0},
      steal{0},
      guest{0},
      guest_nice{0} {}

Processor::Processor(Processor *prevP, int user, int nice, int system, int idle,
                     int iowait, int irq, int softirq, int steal, int guest,
                     int guest_nice)
    : oldProcessor{prevP},
      user{user},
      nice{nice},
      system{system},
      idle{idle},
      iowait{iowait},
      irq{irq},
      softirq{softirq},
      steal{steal},
      guest{guest},
      guest_nice{guest_nice} {}
